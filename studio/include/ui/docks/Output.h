#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <QTextEdit>
#include <project/Project.h>

class Output : public QDockWidget {
    Q_OBJECT

public:
    QTextEdit* textEdit;

    Output(QMainWindow* window, Project* project);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};