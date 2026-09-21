#include "editor/EngineViewport.h"
#include <engine/datamodel/Game.h>
#include <engine/datamodel/instances/BasePart.h>
#include <engine/datamodel/instances/Camera.h>
#include <engine/services/world/World.h>
#include <engine/core/math/Vector3.h>
#include <engine/core/math/Color3.h>
#include <project/Project.h>
#include <QDebug>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>
#include <QVariant>

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

// SDL_Log's default output goes to stdout, which a Windows GUI app
// (subsystem:windows, no console attached) usually doesn't have — so
// failures here can go completely silent. qWarning() always lands in
// Qt Creator's Application Output / the debugger's Output window
// regardless of subsystem, so route through that for anything we need
// to actually see while diagnosing this.

EngineViewport::EngineViewport(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);

    // Render loop timer. Created unconditionally, up front, so that
    // createRenderer() (called later from showEvent, independent of
    // whether `project` below resolves) always has a valid timer to
    // start. Not started until we actually have a Renderer.
    m_renderTimer = new QTimer(this);
    m_renderTimer->setTimerType(Qt::PreciseTimer);
    connect(m_renderTimer, &QTimer::timeout, this, [this]() {
        if (!m_renderer || !m_camera) {
            static bool loggedOnce = false;
            if (!loggedOnce) {
                loggedOnce = true;
                qDebug() << "Render loop bailing early — m_renderer:"
                    << (m_renderer ? "valid" : "NULL")
                    << "m_camera:" << (m_camera ? "valid" : "NULL");
            }
            return;
        }

        // NOTE: getViewMatrix()/getProjectionMatrix() are guesses at
        // your Camera class's API, matching the getX() naming already
        // used elsewhere in your engine (getSize, getPosition, ...). If
        // Camera doesn't expose these exact names, this is the one spot
        // to fix — Renderer itself doesn't know Camera exists at all.
        float aspectRatio = height() > 0
            ? static_cast<float>(width()) / static_cast<float>(height())
            : 1.0f;

        DirectX::XMMATRIX view = m_camera->getViewMatrix();
        DirectX::XMMATRIX projection = m_camera->getProjectionMatrix(aspectRatio);

        // TEMPORARY — print the actual values feeding this draw call
        // once, so we're looking at real numbers instead of guessing.
        // Delete this block once the cube is showing up.
        static bool loggedOnce = false;
        if (!loggedOnce && !m_parts.empty()) {
            loggedOnce = true;
            Engine::BasePart* p = m_parts.front();
            Engine::Vector3 size = p->getSize();
            Engine::Vector3 pos = p->getPosition();
            Engine::Color3 color = p->getColor();

            DirectX::XMFLOAT4X4 viewDump, projDump;
            DirectX::XMStoreFloat4x4(&viewDump, view);
            DirectX::XMStoreFloat4x4(&projDump, projection);
        }

        m_renderer->DrawFrame(view, projection, m_parts);
        });

    Project* project = nullptr;
    if (parent) {
        QVariant prop = parent->property("projectInstance");
        if (!prop.isNull()) {
            void* vp = prop.value<void*>();
            project = static_cast<Project*>(vp);

        }
    }

    if (project == NULL) {
        qWarning() << "EngineViewport: no projectInstance property found on parent"
            << parent << "- scene data won't be available, but the"
            << "viewport will still render.";
        return;
    }

    Engine::Game* dataModel = project->dataModel.get();

    Engine::World* worldService = static_cast<Engine::World*>(dataModel->findChildClass("World"));
    m_camera = static_cast<Engine::Camera*>(worldService->findChildClass("Camera"));

    Engine::BasePart* part = static_cast<Engine::BasePart*>(worldService->findChild("Baseplate"));
    qDebug() << part;
    if (part) {
        m_parts.push_back(part);
    }
}

EngineViewport::~EngineViewport()
{
    if (m_renderTimer) {
        m_renderTimer->stop();
    }

    // Destroy the Renderer (releases the GPU pipeline/buffers/device)
    // BEFORE releasing the SDL_Window it was built on top of.
    m_renderer.reset();

    if (m_sdlWindow) {
        SDL_DestroyWindow(m_sdlWindow);
        m_sdlWindow = nullptr;
    }
}

void EngineViewport::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    // Layout may not have run yet at this point — a widget inside a
    // splitter/dock commonly gets shown before it's been given its
    // final size, so width()/height() can still read 0x0 here. Wrapping
    // a 0x0 window gives SDL_GPU a degenerate swapchain, which is
    // exactly what produced the solid-black result: presentation
    // "worked", there was just nothing valid to present. resizeEvent
    // below will retry once we actually have a size.
    if (!m_renderer && width() > 0 && height() > 0) {
        createRenderer();
    }
}

void EngineViewport::createRenderer()
{
    // winId() forces Qt to realize the native platform window if it
    // hasn't already (WA_NativeWindow means it always will be one, not
    // an "alien widget"). This handle is what we hand to SDL.
    WId nativeHandle = winId();

    SDL_PropertiesID props = SDL_CreateProperties();

    // Ask SDL for a window it doesn't own the lifetime of, sized to
    // match this widget right now.
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width());
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height());

#if defined(Q_OS_WIN)
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER,
        reinterpret_cast<void*>(nativeHandle));
#elif defined(Q_OS_MAC)
    // Qt's winId() on macOS returns the NSView*, not the NSWindow*.
    // Wrapping the view directly (rather than pulling out its
    // containing NSWindow) is what we want here, since we're embedding
    // into a region of an existing window, not taking over the window.
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_COCOA_VIEW_POINTER,
        reinterpret_cast<void*>(nativeHandle));
#elif defined(Q_OS_LINUX)
    // Assumes X11. If you're also targeting Wayland, this branch needs
    // SDL_PROP_WINDOW_CREATE_WAYLAND_WL_SURFACE_POINTER instead, pulled
    // from the relevant Qt platform-native interface — worth checking
    // which platform plugin Studio actually runs under on Linux before
    // relying on this.
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER,
        static_cast<Sint64>(nativeHandle));
#endif

    m_sdlWindow = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    if (!m_sdlWindow) {
        qWarning() << "EngineViewport: SDL_CreateWindowWithProperties failed:"
            << SDL_GetError();
        return;
    }

    qDebug() << "EngineViewport: wrapped native window" << nativeHandle
        << "size" << width() << "x" << height();

    try {
        m_renderer = std::make_unique<Engine::Rendering::Renderer>(m_sdlWindow);
    }
    catch (const std::exception& e) {
        qWarning() << "EngineViewport: failed to create Renderer:" << e.what();
        SDL_DestroyWindow(m_sdlWindow);
        m_sdlWindow = nullptr;
        return;
    }

    qDebug() << "EngineViewport: renderer created, starting render loop.";

    // ~60fps. Swap for vsync-driven presentation later if SDL_GPU's
    // present mode ends up doing that for you — this timer is the
    // simple starting point.
    m_renderTimer->start(16);
}

void EngineViewport::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (width() <= 0 || height() <= 0) {
        return;
    }

    if (!m_renderer) {
        // showEvent ran before we had a real size — this is the first
        // resize with valid dimensions, so create the renderer now.
        createRenderer();
        return;
    }

    // Confirmed needed (not just a "might be needed" — this was the
    // actual bug): SDL isn't picking up this wrapped window's size
    // changes on its own, so tell it explicitly, keeping the swapchain
    // in sync with the widget's actual size on every subsequent resize.
    SDL_SetWindowSize(m_sdlWindow, width(), height());
}