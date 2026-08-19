#include <QDockWidget>
#include <QLineEdit>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QTimer>
#include <QMenu>
#include <QInputDialog>
#include <QTabWidget>
#include <QMouseEvent>
#include <QTextEdit>
#include <QPalette>
#include <project/Project.h>
#include <project/CodeEditor.h>
#include <util/Languages.h>
#include <ui/docks/Output.h>

Output::Output(QMainWindow* window, Project* project)
    : QObject(window) {

    auto* outputDock = new QDockWidget("Output", window);

    auto* outputLog = new QTextEdit(outputDock);
    outputLog->setReadOnly(true);
    outputLog->setPlaceholderText("Output Log...");
    outputDock->setWidget(outputLog);

    QPalette palette = outputLog->palette();
    palette.setColor(QPalette::Highlight, QColor(51, 153, 255));
    outputLog->setPalette(palette);

    outputDock->setStyleSheet(
        "QTextEdit { outline: 0; border-radius: 0px }"
    );

    dockWidget = outputDock;
    textEdit = outputLog;

    QTimer::singleShot(0, window, [window, outputDock]() {
        window->resizeDocks({ outputDock }, { 300 }, Qt::Vertical);
        });
}

bool Output::eventFilter(QObject* watched, QEvent* event) {
    if (textEdit && watched == textEdit->viewport()) {
    }
    return QObject::eventFilter(watched, event);
}