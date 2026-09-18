#include "editor/EngineViewport.h"
#include <engine/datamodel/Game.h>
#include <engine/services/world/World.h>
#include <engine/datamodel/instances/BasePart.h>
#include <project/Project.h>
#include <QResizeEvent>
#include <QShowEvent>
#include <QVariant>
#include <QTimer>

EngineViewport::EngineViewport(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);

    Project* project = nullptr;
    if (parent) {
        QVariant prop = parent->property("projectInstance");
        if (!prop.isNull()) {
            void* vp = prop.value<void*>();
            project = static_cast<Project*>(vp);

        }
    }

    if (project == NULL) return;

    Engine::Game* dataModel = project->dataModel.get();

    Engine::World* worldService = static_cast<Engine::World*>(dataModel->findChildClass("World"));
    m_camera = worldService->getCurrentCamera();

    Engine::BasePart* part = static_cast<Engine::BasePart*>(worldService->findChildClass("Baseplate"));

    m_renderTimer = new QTimer(this);
    m_renderTimer->setTimerType(Qt::PreciseTimer);
    connect(m_renderTimer, &QTimer::timeout, this, [this]() {
        if (m_renderer) {
            m_renderer->DrawFrame();
        }
        });
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

    if (!m_renderer) {
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
        SDL_Log("Failed to wrap viewport in an SDL_Window: %s", SDL_GetError());
        return;
    }

    try {
        m_renderer = std::make_unique<Engine::Rendering::Renderer>(m_sdlWindow);
    }
    catch (const std::exception& e) {
        SDL_Log("Failed to create viewport Renderer: %s", e.what());
        SDL_DestroyWindow(m_sdlWindow);
        m_sdlWindow = nullptr;
        return;
    }

    // ~60fps. Swap for vsync-driven presentation later if SDL_GPU's
    // present mode ends up doing that for you — this timer is the
    // simple starting point.
    m_renderTimer->start(16);
}

void EngineViewport::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    // IMPORTANT — verify this on each platform before relying on it:
    // when SDL wraps a foreign window, it's not guaranteed to be
    // subscribed to that window's native resize notifications the way
    // it is for windows it created itself. If the rendered image stops
    // matching the widget's size as you resize the panel, that's why —
    // SDL_GPU queries the window's current drawable size each frame via
    // the SDL_Window, so if THAT hasn't updated, neither will the
    // output. If you hit this, the fix is telling SDL about the new
    // size explicitly here, e.g. SDL_SetWindowSize(m_sdlWindow,
    // event->size().width(), event->size().height()) — left out for now
    // since whether it's needed is platform-dependent and untested.
}
