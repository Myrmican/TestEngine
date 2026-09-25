#include "editor/EngineViewport.h"
#include <engine/datamodel/DataModel.h>
#include <engine/datamodel/instances/BasePart.h>
#include <engine/datamodel/instances/Camera.h>
//#include <engine/services/world/World.h>
//#include <engine/core/math/Vector3.h>
//#include <engine/core/math/Color3.h>
#include <project/Project.h>
#include <QDebug>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>
#include <QVariant>

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

// SDL_Log's default output goes to stdout, which a Windows GUI app
// (subsystem:windows, no console attached) usually doesn't have — so
// failures here can go completely silent. qWarning() always lands in
// Qt Creator's Application Output / the debugger's Output window
// regardless of subsystem, so route through that for anything we need
// to actually see while diagnosing this.

EngineViewport::EngineViewport(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);

    // Render loop timer. Created unconditionally, up front, so that
    // createRenderer() (called later from showEvent, independent of
    // whether `project` below resolves) always has a valid timer to
    // start. Not started until we actually have a Renderer.
    //m_renderTimer = new QTimer(this);
    //m_renderTimer->setTimerType(Qt::PreciseTimer);
    //connect(m_renderTimer, &QTimer::timeout, this, [this]() {
    //    if (!m_renderer || !m_camera) {
    //        static bool loggedOnce = false;
    //        if (!loggedOnce) {
    //            loggedOnce = true;
    //            qDebug() << "Render loop bailing early — m_renderer:"
    //                << (m_renderer ? "valid" : "NULL")
    //                << "m_camera:" << (m_camera ? "valid" : "NULL");
    //        }
    //        return;
    //    }

    //    // NOTE: getViewMatrix()/getProjectionMatrix() are guesses at
    //    // your Camera class's API, matching the getX() naming already
    //    // used elsewhere in your engine (getSize, getPosition, ...). If
    //    // Camera doesn't expose these exact names, this is the one spot
    //    // to fix — Renderer itself doesn't know Camera exists at all.
    //    float aspectRatio = height() > 0
    //        ? static_cast<float>(width()) / static_cast<float>(height())
    //        : 1.0f;

    //    DirectX::XMMATRIX view = m_camera->getViewMatrix();
    //    DirectX::XMMATRIX projection = m_camera->getProjectionMatrix(aspectRatio);

    //    // TEMPORARY — print the actual values feeding this draw call
    //    // once, so we're looking at real numbers instead of guessing.
    //    // Delete this block once the cube is showing up.
    //    static bool loggedOnce = false;
    //    if (!loggedOnce && !m_parts.empty()) {
    //        loggedOnce = true;
    //        Engine::BasePart* p = m_parts.front();
    //        Engine::Vector3 size = p->getSize();
    //        Engine::Vector3 pos = p->getPosition();
    //        Engine::Color3 color = p->getColor();

    //        DirectX::XMFLOAT4X4 viewDump, projDump;
    //        DirectX::XMStoreFloat4x4(&viewDump, view);
    //        DirectX::XMStoreFloat4x4(&projDump, projection);
    //    }

    //    m_renderer->DrawFrame(view, projection, m_parts);
    //    });

    //Project* project = nullptr;
    //if (parent) {
    //    QVariant prop = parent->property("projectInstance");
    //    if (!prop.isNull()) {
    //        void* vp = prop.value<void*>();
    //        project = static_cast<Project*>(vp);

    //    }
    //}

    //if (project == NULL) {
    //    qWarning() << "EngineViewport: no projectInstance property found on parent"
    //        << parent << "- scene data won't be available, but the"
    //        << "viewport will still render.";
    //    return;
    //}

    //Engine::Game* dataModel = project->dataModel.get();

    //Engine::World* worldService = static_cast<Engine::World*>(dataModel->findChildClass("World"));
    //m_camera = static_cast<Engine::Camera*>(worldService->findChildClass("Camera"));

    //Engine::BasePart* part = static_cast<Engine::BasePart*>(worldService->findChild("Baseplate"));
    //qDebug() << part;
    //if (part) {
    //    m_parts.push_back(part);
    //}
}