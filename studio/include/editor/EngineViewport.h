#pragma once

#include <QWidget>
#include <memory>

//#include <SDL3/SDL.h>
//#include "engine/rendering/Renderer.h"

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
};