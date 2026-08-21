#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <project/Project.h>

class Toolbox : public QObject {
    Q_OBJECT

public:
    QDockWidget* dockWidget;

    Toolbox(QMainWindow* window, Project* project);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};