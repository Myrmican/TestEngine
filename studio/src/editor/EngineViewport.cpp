#include "editor/EngineViewport.h"

#include <engine/rendering/Renderer.h>   // adjust to match your include path
#include <project/Project.h>
#include <services/world/World.h>

#include <QShowEvent>
#include <QResizeEvent>
#include <QTimer>

EngineViewport::EngineViewport(QWidget* parent, Project* project)
    : QWidget(parent), m_project(project)
{
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(320, 240);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &EngineViewport::renderFrame);

    // Keep the World, not the Camera: the current camera can change at runtime
    if (project && project->engine && project->engine->getProvider())
        m_world = project->engine->getProvider()->getService<Engine::World>();
}

EngineViewport::~EngineViewport()
{
    if (m_timer) m_timer->stop();

    // Renderer first: it needs the window to still exist while it shuts down
    m_renderer.reset();

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

void EngineViewport::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    // Only set up once, the first time the widget is shown
    if (!m_window && initGpu())
        m_timer->start(16); // roughly 60 fps
}

void EngineViewport::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (m_window)
        SDL_SetWindowSize(m_window, event->size().width(), event->size().height());
}

bool EngineViewport::initGpu()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    // Wrap Qt's native window handle in an SDL window
    SDL_PropertiesID props = SDL_CreateProperties();
#if defined(Q_OS_WIN)
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER,
        reinterpret_cast<void*>(winId()));
#elif defined(Q_OS_MACOS)
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_COCOA_VIEW_POINTER,
        reinterpret_cast<void*>(winId()));
#else
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER,
        static_cast<Sint64>(winId()));
#endif
    m_window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    if (!m_window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return false;
    }

    m_renderer = std::make_unique<Engine::Renderer>();
    if (!m_renderer->init(m_window)) {
        m_renderer.reset();
        return false;
    }

    return true;
}

void EngineViewport::renderFrame()
{
    if (!m_renderer) return;

    // Later: fetch the camera here each frame and pass it to the renderer
    Engine::Camera* camera = m_world ? m_world->getCurrentCamera() : nullptr;
    //   m_renderer->renderFrame(camera);
    m_renderer->renderFrame();
}