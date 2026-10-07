#include "editor/EngineViewport.h"
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

    Engine::World* world = project->engine->getProvider()->getService<Engine::World>();
    
	m_camera = world ? world->getCurrentCamera() : nullptr;
}

EngineViewport::~EngineViewport()
{
    if (m_timer) m_timer->stop();

    if (m_device && m_window)
        SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
    if (m_device)
        SDL_DestroyGPUDevice(m_device);
    if (m_window)
        SDL_DestroyWindow(m_window);
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

    m_device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        true, nullptr); // true = debug mode, turn off for release builds
    if (!m_device) {
        SDL_Log("GPU device creation failed: %s", SDL_GetError());
        return false;
    }

    if (!SDL_ClaimWindowForGPUDevice(m_device, m_window)) {
        SDL_Log("Claiming window failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

void EngineViewport::renderFrame()
{
    if (!m_device || !m_window) return;

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(m_device);
    if (!cmd) return;

    SDL_GPUTexture* swapchain = nullptr;
    SDL_WaitAndAcquireGPUSwapchainTexture(cmd, m_window, &swapchain, nullptr, nullptr);

    if (swapchain) {
        SDL_GPUColorTargetInfo target = {};
        target.texture = swapchain;
        target.clear_color = SDL_FColor{ 0.10f, 0.25f, 0.60f, 1.0f }; // blue
        target.load_op = SDL_GPU_LOADOP_CLEAR;
        target.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmd, &target, 1, nullptr);
        SDL_EndGPURenderPass(pass);
    }

    SDL_SubmitGPUCommandBuffer(cmd);
}