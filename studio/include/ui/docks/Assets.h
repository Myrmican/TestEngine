#pragma once

#include <QDockWidget>
#include <QObject>
#include <project/Project.h>

class MainWindow;

class Assets : public QDockWidget {
    Q_OBJECT

public:
    Project* m_project;

    Assets(MainWindow* window, Project* project);
};