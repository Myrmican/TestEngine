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
    mainToolBar->setFixedHeight(80);
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

    auto addTool = [&](QString name, QString iconPath, QString keySequence) {
        QAction* action = new QAction(QIcon(iconPath), name, transformToolsGroup);
        action->setCheckable(true);
        QAction* toolAction = transformToolsGroup->addAction(action);
        toolAction->setShortcut(QKeySequence(keySequence));

        window->addAction(action);

        QToolButton* btn = ToolManager::createTool(QIcon(iconPath), name, mainToolBar, true);
        btn->setDefaultAction(action);
        btn->setCheckable(true);

        QWidget* wrapper = new QWidget(mainToolBar);
        wrapper->setFixedSize(64, 80);

        QVBoxLayout* layout = new QVBoxLayout(wrapper);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(btn, 0, Qt::AlignHCenter | Qt::AlignVCenter);

        mainToolBar->addWidget(wrapper);
        };

    addTool("Select", ":/assets/icons/MoveTool.png", "1");
    addTool("Move", ":/assets/icons/MoveTool.png", "2");
    addTool("Scale", ":/assets/icons/MoveTool.png", "3");
    addTool("Rotate", ":/assets/icons/MoveTool.png", "4");

    mainToolBar->addSeparator();

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
        "   color: #9ca3af;"
        "   background: transparent;"
        "   border: 1px solid transparent;"
        "   border-radius: 4px;"
        "   padding: 4px;"
        "}"
        "QToolButton:hover {"
        "    background-color: #2d2d2d;"
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