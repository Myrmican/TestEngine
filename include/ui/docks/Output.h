#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <QTextEdit>
#include <project/Project.h>

class Output : public QObject {
    Q_OBJECT

public:
    QDockWidget* dockWidget;
    QTextEdit* textEdit;

    Output(QMainWindow* window, Project* project);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};