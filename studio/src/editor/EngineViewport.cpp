#include "editor/EngineViewport.h"
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEngine>
#include <d3dcompiler.h>

EngineViewport::EngineViewport(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_DontShowOnScreen, false);

    setFocusPolicy(Qt::StrongFocus);

    // D3D needs a real HWND to exist first — that happens once the
    // widget is shown, so defer init to the first showEvent instead
    // of the constructor.
    //connect(&m_renderTimer, &QTimer::timeout, this, &EngineViewport::renderFrame);
}