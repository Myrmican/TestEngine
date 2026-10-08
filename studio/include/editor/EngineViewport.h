#pragma once

#include <QWidget>
#include <memory>

#include <SDL3/SDL.h>

class QTimer;
class QResizeEvent;
class BasePart;
class QShowEvent;
class QPaintEngine;
class Project;

namespace Engine { class World; class Renderer; }

class EngineViewport : public QWidget
{
    Q_OBJECT

public:
    explicit EngineViewport(QWidget* parent = nullptr, Project* project = nullptr);
    ~EngineViewport() override;

    QPaintEngine* paintEngine() const override { return nullptr; }

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    bool initGpu();
    void renderFrame();

    Project* m_project = nullptr;
    Engine::World* m_world = nullptr;
    SDL_Window* m_window = nullptr;
    QTimer* m_timer = nullptr;
    std::unique_ptr<Engine::Renderer> m_renderer;
};