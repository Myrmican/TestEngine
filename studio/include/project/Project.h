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

    bool savingInCloud = false;
	std::string primaryLanguage = "Kotlin"; //Remove

    Logger* logger = nullptr;
	Explorer* explorer = nullptr;

    std::shared_ptr<Engine::EngineInstance> engine;
    std::shared_ptr<Engine::ScriptEngine> scriptEngine;

    bool isSaved = false;
    bool hasPendingChanges = false;

    Project(QFileInfo fileInfo);
    ~Project();
};

namespace ProjectManager {
    Project* createNew(QMainWindow* parent);
    Project* openFile(QMainWindow* parent);
    Project* openProject();
	Project* getProject(QWidget* contextWidget);
}