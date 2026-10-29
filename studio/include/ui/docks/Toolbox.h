#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <project/Project.h>

class Toolbox : public QDockWidget {
    Q_OBJECT

public:
    Toolbox(QMainWindow* window, Project* project);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};