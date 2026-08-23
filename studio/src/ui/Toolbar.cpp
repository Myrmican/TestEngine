#include <ui/Toolbar.h>
#include <QToolBar>
#include <QActionGroup>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>

QAction* createToolAction(const QIcon& icon, const QString& text, QObject* parent) {
    QAction* action = new QAction(icon, text, parent);
    action->setCheckable(true);
    return action;
}

QWidget* createToolButtonWidget(QAction* action, QToolBar* toolBar) {
    QWidget* container = new QWidget(toolBar);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 4, 0, 4);
    layout->setSpacing(4);

    QToolButton* iconButton = new QToolButton(container);
    iconButton->setIcon(action->icon());
    iconButton->setIconSize(QSize(32, 32));
    iconButton->setFixedSize(44, 44);
    iconButton->setFocusPolicy(Qt::NoFocus);
    iconButton->setCheckable(true);
    iconButton->setChecked(action->isChecked());

    iconButton->setStyleSheet(
        "QToolButton {"
        "    background: transparent;"
        "    border: none;"
        "    border-radius: 6px;"
        "}"
        "QToolButton:hover {"
        "    background-color: #2f3039;"
        "}"
        "QToolButton:checked {"
        "    background-color: #3a7afe;"
        "}"
        "QToolButton:checked:hover {"
        "    background-color: #4a86ff;"
        "}"
    );

    QLabel* textLabel = new QLabel(action->text(), container);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet("color: #b1b1b1; font-size: 11px; background: transparent;");
    layout->addWidget(iconButton, 0, Qt::AlignCenter);
    layout->addWidget(textLabel, 0, Qt::AlignCenter);

    QObject::connect(iconButton, &QToolButton::clicked, action, [action](bool checked) {
        action->setChecked(checked);
        });

    QObject::connect(action, &QAction::toggled, iconButton, &QToolButton::setChecked);

    return container;
}

QToolBar* Toolbar::create(QMainWindow* window) {
    QToolBar* mainToolBar = new QToolBar("Main Toolbar", window);
    mainToolBar->setMovable(false);
    mainToolBar->setIconSize(QSize(32, 32));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    window->addToolBar(Qt::TopToolBarArea, mainToolBar);

    QWidget* spacer = new QWidget(mainToolBar);
    spacer->setFixedWidth(12);
    mainToolBar->addWidget(spacer);

    QActionGroup* transformToolsGroup = new QActionGroup(mainToolBar);
    transformToolsGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

    QAction* selectAction = createToolAction(QIcon(":/assets/icons/SelectTool.png"), "Select", mainToolBar);
    QAction* moveAction = createToolAction(QIcon(":/assets/icons/MoveTool.png"), "Move", mainToolBar);
    QAction* scaleAction = createToolAction(QIcon(":/assets/icons/ScaleTool.png"), "Scale", mainToolBar);
    QAction* rotateAction = createToolAction(QIcon(":/assets/icons/EraseTool.png"), "Rotate", mainToolBar);

    for (QAction* toolAction : { selectAction, moveAction, scaleAction, rotateAction }) {
        transformToolsGroup->addAction(toolAction);
        mainToolBar->addWidget(createToolButtonWidget(toolAction, mainToolBar));
    }

    mainToolBar->addSeparator();

    QAction* playAction = createToolAction(QIcon(":/assets/icons/Play.png"), "Play", mainToolBar);
    mainToolBar->addWidget(createToolButtonWidget(playAction, mainToolBar));

    mainToolBar->setStyleSheet(
        "QToolBar {"
        "    background-color: #1a1a1a;"
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