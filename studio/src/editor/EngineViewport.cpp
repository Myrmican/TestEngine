#include "editor/EngineViewport.h"
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEngine>

EngineViewport::EngineViewport(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_DontShowOnScreen, false);

    // Gives the viewport mouse focus for camera movement
    setFocusPolicy(Qt::StrongFocus);
}

bool EngineViewport::initializeEngine() {
    this->ensurePolished();
    this->setAttribute(Qt::WA_NativeWindow, true);

    HWND hwnd = reinterpret_cast<HWND>(this->winId());

    if (!m_pipeline.Initialize(hwnd, width(), height())) {
        return false;
    }

    // RenderPipeline::Initialize() already resizes internally with these same
    // dimensions, so no need to call Resize() again here.
    return true;
}

void EngineViewport::renderFrame() {
    // Don't try to render until the D3D device/swapchain actually exist.
    if (!m_engineInitialized) {
        return;
    }

    // Render the scene passing our active camera matrix to the pipeline
    if (m_activeCamera) {
        m_pipeline.RenderFrame(m_activeCamera);
    }
}

void EngineViewport::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    // Qt may fire resize events with a zero-sized rect during early layout
    // passes (e.g. before the widget is actually placed in its parent tab).
    // Ignore those; wait for a real, positive size.
    if (width() <= 0 || height() <= 0) {
        return;
    }

    if (!m_engineInitialized) {
        // This is the first time we have a valid size — safe to create the
        // D3D device/swapchain now, sized correctly from the start.
        m_engineInitialized = initializeEngine();
    }
    else {
        // Subsequent resizes (splitter drags, window resizes, etc.) just
        // resize the existing swapchain buffers.
        m_pipeline.Resize(width(), height());
    }
}

void EngineViewport::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    // Intentionally empty — DirectX presents directly to this window's HWND;
    // Qt's software paint engine is disabled via paintEngine() returning nullptr.
}

void EngineViewport::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        m_lastMousePos = event->pos(); // Capture starting position for mouse-look drag
    }
}

void EngineViewport::mouseMoveEvent(QMouseEvent* event) {
    // Look around when right-clicking and dragging inside the viewport
    if (event->buttons() & Qt::RightButton && m_activeCamera) {
        QPoint delta = event->pos() - m_lastMousePos;

        // Update Camera rotation (Yaw & Pitch)
        m_activeCamera->rotation.y += static_cast<float>(delta.x()) * 0.15f; // Horizontal drag -> Yaw
        m_activeCamera->rotation.x += static_cast<float>(delta.y()) * 0.15f; // Vertical drag -> Pitch

        m_lastMousePos = event->pos();
        update(); // Request immediate repaint tick
    }
}