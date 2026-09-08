#pragma once

#include <QWidget>
#include <QPoint>
#include <Graphics/RenderPipeline.h>
#include <datamodel/instances/Camera.h>

class EngineViewport : public QWidget {
    Q_OBJECT

public:
    explicit EngineViewport(QWidget* parent = nullptr);
    ~EngineViewport() override = default;

    // Call this after the widget is visible in your Qt layout.
    // NOTE: You generally no longer need to call this manually — resizeEvent()
    // will call it automatically the first time the widget gets a valid,
    // non-zero size from the Qt layout system. It's left public in case you
    // want to force-initialize early for some other reason.
    bool initializeEngine();

    // Driven by your main loop or a QTimer tick (~60 FPS)
    void renderFrame();

    // Link a scene camera to this viewport
    void setActiveCamera(Engine::Camera* camera) { m_activeCamera = camera; }

    // Disable Qt's default software paint engine
    QPaintEngine* paintEngine() const override { return nullptr; }

protected:
    // Window lifecycle events
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    // Mouse input events for flying the editor camera
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    Engine::RenderPipeline m_pipeline; // Owns DirectX 11 device, swapchain, and shaders
    Engine::Camera* m_activeCamera = nullptr;

    QPoint m_lastMousePos;

    // Tracks whether the D3D device/swapchain have been created yet. We can't
    // safely initialize until Qt has laid the widget out and given it a real,
    // non-zero size — which doesn't happen until the first legitimate resizeEvent.
    bool m_engineInitialized = false;
};