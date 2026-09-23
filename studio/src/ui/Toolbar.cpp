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
    mainToolBar->setMinimumHeight(80);
    mainToolBar->setContentsMargins(8, 8, 8, 8);
    mainToolBar->setMovable(false);
    mainToolBar->setIconSize(QSize(18, 18));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    QWidget* leftSpacer = new QWidget();
    leftSpacer->setFixedWidth(20);
    mainToolBar->addWidget(leftSpacer);

    QActionGroup* transformToolsGroup = new QActionGroup(mainToolBar);
    transformToolsGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

    ToolManager::createTools(mainToolBar);

    mainToolBar->setStyleSheet(
        "QToolBar {"
        "    border: none;"
        "    border-bottom: 1px solid #252525;"
        "}"
        "QToolBar::separator {"
        "    background-color: #4a4a4a;"
        "    width: 1px;"
        "    margin: 6px 6px;"
        "}"
        "QToolButton {"
        "   color: rgba(156, 163, 175, 1.0);;"
        "   background: transparent;"
        "   border: 1px solid transparent;"
        "   border-radius: 4px;"
        "   padding: 4px;"
        "}"
        "QToolButton:hover {"
        "   background-color: rgba(63, 63, 70, 0.4);"
        "   color: rgba(255, 255, 255, 1.0);"
        "}"
        "QToolButton:checked {"
        "    background-color: rgba(59, 130, 246, 0.2);"
        "}"
        "QToolButton:checked:hover {"
        "    background-color: rgba(59, 130, 246, 0.3);"
        "    color: rgba(96, 165, 250, 1.0);"
        "}"
    );

    window->addToolBar(Qt::TopToolBarArea, mainToolBar);

    return mainToolBar;
}