#pragma once

#include <QDockWidget>
#include <QObject>
#include <QEvent>
#include <QTextEdit>
#include <ui/window/MainWindow.h>
#include <project/Project.h>

class Output : public QDockWidget {
    Q_OBJECT

public:
    QTextEdit* textEdit;

    Output(MainWindow* window, Project* project);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};