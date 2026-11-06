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
#include <QComboBox>
#include <QToolButton>
#include <project/Project.h>
#include <util/Languages.h>
#include <ui/docks/Output.h>

Output::Output(QMainWindow* window, Project* project)
    : QDockWidget("Output", window) {

    setWindowFlags(Qt::SubWindow);
    setObjectName("OutputDock");

    QWidget* dockContainer = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(dockContainer);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    QWidget* toolbarStrip = new QWidget(dockContainer);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbarStrip);
    toolbarLayout->setContentsMargins(0, 0, 0, 0);
    toolbarLayout->setSpacing(6);

    QComboBox* msgFilterCombo = new QComboBox(toolbarStrip);
    msgFilterCombo->addItems({ "All Messages", "Errors", "Warnings" });

    QComboBox* outputContexts = new QComboBox(toolbarStrip);
    outputContexts->setCurrentIndex(-1);
    outputContexts->addItems({ "All contexts", "Server", "Client", "Studio"});

    QLineEdit* searchField = new QLineEdit(toolbarStrip);
    searchField->setPlaceholderText("Filter...");
    searchField->setClearButtonEnabled(true);

    QToolButton* clearLogBtn = new QToolButton(toolbarStrip);
    clearLogBtn->setToolTip("Clear Log");
    clearLogBtn->setIcon(QIcon(":/icons/broom.png"));
    clearLogBtn->setAutoRaise(true);
    
    QComboBox* moreOptionsBtn = new QComboBox(toolbarStrip);
    moreOptionsBtn->setCurrentIndex(-1);
    moreOptionsBtn->addItems({ "Show timestamps", "Option 1", "Option 2" });

    toolbarLayout->addWidget(msgFilterCombo);
	toolbarLayout->addWidget(outputContexts);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(searchField);
    toolbarLayout->addWidget(clearLogBtn);
    toolbarLayout->addWidget(moreOptionsBtn);

    auto* logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);
    
    mainLayout->addWidget(toolbarStrip);
    mainLayout->addWidget(logOutput);

    setWidget(dockContainer);

    QPalette palette = logOutput->palette();
    palette.setColor(QPalette::Highlight, QColor(51, 153, 255));
    logOutput->setPalette(palette);

	toolbarStrip->setStyleSheet(
		"QWidget { border-bottom: 1px solid #000000; }"
	);

    setStyleSheet(
        "QTextEdit { outline: 0; border-radius: 0px; background-color: #1E1E1E; }"
    );

    textEdit = logOutput;

    QTimer::singleShot(0, window, [window, this]() {
        window->resizeDocks({ this }, { 300 }, Qt::Vertical);
        });
}

bool Output::eventFilter(QObject* watched, QEvent* event) {
    if (textEdit && watched == textEdit->viewport()) {
    }
    return QObject::eventFilter(watched, event);
}