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

namespace Engine
{
    class Camera;
}

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

    SDL_Window* m_window = nullptr;
    SDL_GPUDevice* m_device = nullptr;
    QTimer* m_timer = nullptr;

	Project* m_project = nullptr;
	Engine::Camera* m_camera = nullptr;
};