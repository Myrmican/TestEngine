#pragma once

#include <string>
#include <memory>
#include <QString>
#include <QObject>
#include <core/Logger.h>

class QMainWindow;
class QFile;

class Project : public QObject {
    Q_OBJECT
public:
    QString name;
    std::string projectPath;
    QFile* projectFile;
    bool savingInCloud = false;
	std::string primaryLanguage = "Kotlin";
	Logger* logger = nullptr;

    Project(const std::string& projectName);
    ~Project();
};

namespace ProjectManager {
    Project* onNewProject(QMainWindow* parent);
    void onOpenProject(QMainWindow* parent);
	Project* getProject(QWidget* contextWidget);
}