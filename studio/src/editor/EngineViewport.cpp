#include "editor/EngineViewport.h"
#include <engine/datamodel/Game.h>
#include <engine/services/world/World.h>
#include <project/Project.h>
#include <QResizeEvent>
#include <QShowEvent>
#include <editor/StudioEngine.h>
#include <QVariant>

EngineViewport::EngineViewport(QWidget* parent)
    : QWidget(parent)
{
    // Tell Qt: this widget paints itself natively (via Win32/D3D),
    // don't waste time giving it a background or double-buffering.
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

    StudioEngine::Get().getRenderPipeline()->AddPart(part);
}

EngineViewport::~EngineViewport()
{
    if (m_renderTimer)
        m_renderTimer->stop();
}

void EngineViewport::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if (!m_initialized)
    {
        if (!m_renderDevice)
        {
            qWarning() << "EngineViewport::showEvent - m_renderDevice is null!";
            return;
        }

        HWND hwnd = reinterpret_cast<HWND>(this->winId());
        if (!m_swapChain.Init(m_renderDevice, hwnd, width(), height()))
        {
            qWarning() << "EngineViewport::showEvent - SwapChain::Init failed!";
            return;
        }

        m_initialized = true;
        qDebug() << "EngineViewport initialized successfully";

        m_renderTimer = new QTimer(this);
        connect(m_renderTimer, &QTimer::timeout, this, &EngineViewport::Render);
        m_renderTimer->start(16);
    }
}

void EngineViewport::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (m_initialized)
        m_swapChain.Resize(width(), height());
}

void EngineViewport::Render()
{
    if (!m_initialized || !m_renderPipeline)
    {
        qWarning() << "Render() bailing early - initialized:" << m_initialized
            << "pipeline:" << (m_renderPipeline != nullptr);
        return;
    }

    float clearColor[4] = { 0.1f, 0.1f, 0.15f, 1.0f };
    m_swapChain.BeginFrame(m_renderDevice->getContext(), clearColor);
    m_renderPipeline->RenderScene(m_camera, m_swapChain.getAspectRatio());
    m_swapChain.Present();
}