#pragma once

#include <string>
#include <QString>
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <core/Logger.h>
#include <engine/Engine.h>
#include <engine/scripting/ScriptEngine.h>

class QMainWindow;
class Explorer;

class Project : public QObject {
    Q_OBJECT
public:
    QString name;
    std::string projectPath;
    QFile* projectFile = nullptr;
    QFileInfo fileInfo;
    bool is2D = false;

    bool savingInCloud = false;
	std::string primaryLanguage = "Kotlin"; //Remove

    Logger* logger = nullptr;

    bool isSaved = false;
    bool hasPendingChanges = false;

    explicit Project(const QFileInfo fileInfo);
    ~Project();

    Engine::EngineInstance* getEngine() const { return engine.get(); }
    Engine::ScriptEngine* getScriptEngine() const { return scriptEngine.get(); }

private:
    std::unique_ptr<Engine::EngineInstance> engine;
    std::unique_ptr<Engine::ScriptEngine> scriptEngine;
};

namespace ProjectManager {
    Project* createNew(QMainWindow* parent);
    Project* openFile(QMainWindow* parent);
    Project* openProject();
	Project* getProject(QWidget* contextWidget);
}