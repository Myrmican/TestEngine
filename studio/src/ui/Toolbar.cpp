#include <ui/Toolbar.h>
#include <editor/tools/ToolManager.h>
#include <QToolBar>
#include <QActionGroup>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>

QToolBar* Toolbar::create(QMainWindow* window) {
    QToolBar* mainToolBar = new QToolBar("Main Toolbar", window);
    mainToolBar->setMovable(false);
    mainToolBar->setIconSize(QSize(32, 32));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    window->addToolBar(Qt::TopToolBarArea, mainToolBar);

    QWidget* spacer = new QWidget(mainToolBar);
    spacer->setFixedWidth(12);
    mainToolBar->addWidget(spacer);

    QActionGroup* transformToolsGroup = new QActionGroup(mainToolBar);
    transformToolsGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

    QAction* selectAction = ToolManager::createToolAction(QIcon(":/assets/icons/SelectTool.png"), "Select", mainToolBar);
    QAction* moveAction = ToolManager::createToolAction(QIcon(":/assets/icons/MoveTool.png"), "Move", mainToolBar);
    QAction* scaleAction = ToolManager::createToolAction(QIcon(":/assets/icons/ScaleTool.png"), "Scale", mainToolBar);
    QAction* rotateAction = ToolManager::createToolAction(QIcon(":/assets/icons/EraseTool.png"), "Rotate", mainToolBar);

    for (QAction* toolAction : { selectAction, moveAction, scaleAction, rotateAction }) {
        transformToolsGroup->addAction(toolAction);
        mainToolBar->addWidget(ToolManager::createToolButtonWidget(toolAction, mainToolBar));
    }

    mainToolBar->addSeparator();

    QAction* playAction = ToolManager::createToolAction(QIcon(":/assets/icons/Play.png"), "Play", mainToolBar);
    mainToolBar->addWidget(ToolManager::createToolButtonWidget(playAction, mainToolBar));

    mainToolBar->setStyleSheet(
        "QToolBar {"
        "    border: none;"
        "    border-bottom: 1px solid #252525;"
        "    padding-top: 7px;"
        "    padding-bottom: 7px;"
        "    spacing: 12px;"
        "}"
        "QToolBar::separator {"
        "    background-color: #4a4a4a;"
        "    width: 1px;"
        "    margin: 6px 6px;"
        "}"
        "QToolButton {"
        "    color: #ffffff;"
        "    background: transparent;"
        "    font-size: 11px;"
        "    border: 1px solid transparent;"
        "    border-radius: 6px;"
        "    padding-top: 6px;"
        "    padding-bottom: 6px;"
        "    padding-left: 10px;"
        "    padding-right: 10px;"
        "}"
        "QToolButton:hover {"
        "    background-color: #2f3039;"
        "}"
    );

    /*QObject::connect(playAction, &QAction::toggled, [playAction](bool checked) mutable {
        if (!checked) return;

        playAction->setChecked(false);

        documentTabs->setCurrentIndex(0);
        outputLogger->Error("Error starting server.");
        });*/

    return mainToolBar;
}