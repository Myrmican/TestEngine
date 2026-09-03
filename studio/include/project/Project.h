#pragma once

#include <string>
#include <QString>
#include <QObject>
#include <QFile>
#include <core/Logger.h>
#include <engine/datamodel/Game.h>

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

    std::shared_ptr<Engine::Game> dataModel;

    Project(const std::string& projectName);
    ~Project();
};

namespace ProjectManager {
    Project* onNewProject(QMainWindow* parent);
    void onOpenProject(QMainWindow* parent);
	Project* getProject(QWidget* contextWidget);
}