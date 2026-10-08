#pragma once

#include <string>
#include <QString>
#include <QObject>
#include <QFile>
#include <core/Logger.h>
#include <engine/Engine.h>
#include <engine/scripting/ScriptEngine.h>

class QMainWindow;
class QFile;
class Explorer;

class Project : public QObject {
    Q_OBJECT
public:
    QString name;
    std::string projectPath;
    QFile* projectFile = nullptr;

    bool savingInCloud = false;
	std::string primaryLanguage = "Kotlin";
    Logger* logger = nullptr;
	Explorer* explorer = nullptr;

    std::shared_ptr<Engine::EngineInstance> engine;
    std::shared_ptr<Engine::ScriptEngine> scriptEngine;

    bool isSaved = false;
    bool hasPendingChanges = false;

    Project(const std::string& projectName);
    ~Project();
};

namespace ProjectManager {
    Project* onNewProject(QMainWindow* parent);
    Project* promptOpenFile(QMainWindow* parent);
    Project* openProject();
	Project* getProject(QWidget* contextWidget);
}