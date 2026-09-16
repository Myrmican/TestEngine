#pragma once

#include <QWidget>
#include <QTimer>
#include <engine/datamodel/instances/Camera.h>
#include <engine/rendering/RenderDevice.h>
#include <engine/rendering/RenderPipeline.h>
#include <engine/rendering/SwapChain.h>

class EngineViewport : public QWidget
{
    Q_OBJECT

public:
    explicit EngineViewport(QWidget* parent = nullptr);
    ~EngineViewport();

    // Injected from outside (your Engine bootstrap owns these, not the widget).
    // EngineViewport does NOT own the device or pipeline - only its own SwapChain.
    void setRenderDevice(Engine::RenderDevice* device) { m_renderDevice = device; }
    void setRenderPipeline(Engine::RenderPipeline* pipeline) { m_renderPipeline = pipeline; }
    void setCamera(Engine::Camera* camera) { m_camera = camera; }
    Engine::Camera* getCamera() const { return m_camera; }

protected:
    // Qt calls this automatically once the widget's native window is ready -
    // the correct place to create our SwapChain, since only now do we have
    // a real, final HWND to give it.
    void showEvent(QShowEvent* event) override;

    // Called whenever the widget is resized - the swap chain's back buffer
    // must be resized to match, or rendering will be stretched/invalid.
    void resizeEvent(QResizeEvent* event) override;

    // Overriding this to return nullptr stops Qt from trying to paint over
    // our D3D output with its own software renderer.
    QPaintEngine* paintEngine() const override { return nullptr; }

private:
    void Render();

    Engine::RenderDevice* m_renderDevice = nullptr;     // shared, injected - not owned
    Engine::RenderPipeline* m_renderPipeline = nullptr;  // shared, injected - not owned
    Engine::SwapChain m_swapChain;                       // owned - this window's own back buffer
    Engine::Camera* m_camera = nullptr;                  // not owned - just observed

    QTimer* m_renderTimer = nullptr;
    bool m_initialized = false;
};