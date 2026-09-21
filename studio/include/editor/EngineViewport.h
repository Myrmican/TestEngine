#pragma once

#include <QWidget>
#include <memory>

#include <SDL3/SDL.h>
#include "engine/rendering/Renderer.h"

class QTimer;
class QResizeEvent;
class BasePart;
class QShowEvent;
class QPaintEngine;

namespace Engine
{
    class Camera;
}

class EngineViewport : public QWidget
{
    Q_OBJECT

public:
    explicit EngineViewport(QWidget* parent = nullptr);
    ~EngineViewport() override;

protected:
    // Qt calls this once the widget has a real native window to draw
    // into — this is where we create the SDL_Window wrapping it and
    // build the Renderer, since doing it in the constructor risks
    // running before the native window (and its correct size) exists.
    void showEvent(QShowEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

    // Returning nullptr here tells Qt "don't set up your own paint
    // backing store for this widget" — required alongside
    // WA_PaintOnScreen, since we're doing all drawing ourselves via
    // SDL_GPU rather than through QPainter.
    QPaintEngine* paintEngine() const override { return nullptr; }

private:
    void createRenderer();

    Engine::Camera* m_camera = nullptr;

    std::vector<Engine::BasePart*> m_parts;

    SDL_Window* m_sdlWindow = nullptr;
    std::unique_ptr<Engine::Rendering::Renderer> m_renderer;

    // Drives DrawFrame() at a fixed interval. WA_PaintOnScreen widgets
    // don't get regular Qt paint events, so we pull rather than wait to
    // be pushed.
    QTimer* m_renderTimer = nullptr;
};