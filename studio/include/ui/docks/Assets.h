#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QObject>
#include <project/Project.h>

class Assets : public QDockWidget {
    Q_OBJECT

public:
    Project* m_project;

    Assets(QMainWindow* window, Project* project);
};