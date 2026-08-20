#include <project/Project.h>
#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QDockWidget>
#include <QMainWindow>
#include <QHeaderView>
#include <QToolBar>
#include <QLabel>
#include <QWidgetAction>
#include <QPushButton>
#include <QTimer>
#include <QMenu>
#include <QStackedWidget>
#include <QActionGroup>
#include <QToolButton>
#include <project/CodeEditor.h>
#include <core/Logger.h>
#include <util/Languages.h>
#include <ui/docks/Explorer.h>
#include <ui/docks/Properties.h>
#include <ui/docks/Output.h>
#include <ui/docks/Toolbox.h>

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

namespace EditorWindow {
    void initialize(Project* project, QWidget* editorPage, QMainWindow* window) {

        if (editorPage) {
            editorPage->setProperty("projectInstance", QVariant::fromValue(static_cast<void*>(project)));
            project->setParent(editorPage);
        }

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

        auto* centerSplitter = new QSplitter(Qt::Vertical, editorPage);

        auto* editorLayout = new QVBoxLayout(editorPage);
        editorLayout->setContentsMargins(0, 0, 0, 0);
        editorLayout->addWidget(centerSplitter);

        window->setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

        QTabWidget* documentTabs = new QTabWidget(centerSplitter);
        documentTabs->setTabsClosable(true);
        documentTabs->setMovable(true);
		documentTabs->setObjectName("DocumentTabs");

        documentTabs->setDocumentMode(true);

        documentTabs->setStyleSheet(
            "QTabWidget {"
            "    background-color: #1a1a1a;"
            "}"
            "QTabWidget::pane {"
            "    border: none;"
            "    background-color: #1a1a1a;"
            "}"
            "QTabBar {"
            "    background: transparent;"
            "}"
            "QTabBar::tab {"
            "    background-color: 1e1e1e;"
            "    color: #b1b1b1;"
            "    padding: 6px 28px 6px 16px;"
            "    margin-right: 2px;"
            "    border-radius: 0px;"
            "    font-size: 12px;"
            "}"
            "QTabBar::tab:hover {"
            "    background-color: #25262d;"
            "    color: #ffffff;"
            "}"
            "QTabBar::tab:selected {"
            "    background-color: #2d2e36;"
            "    color: #ffffff;"
            "    border-bottom: 2px solid #3a7afe;"
            "}"
            "QTabBar::close-button {"
            "    image: url(:/assets/icons/close.png);"
            "    width: 16px;"
            "    height: 16px;"
            "    subcontrol-origin: padding;"
            "    subcontrol-position: right center;"
            "    right: 6px;"
            "}"
            "QTabBar::close-button:hover {"
            "    background-color: #ff4d4d;"
            "    border-radius: 2px;"
            "}"
        );

        QWidget* placeView = new QWidget();

        Explorer* explorerDock = new Explorer(window, project);
		Properties* propertiesDock = new Properties(window, project);
		Output* outputDock = new Output(window, project);
        Toolbox* toolboxDock = new Toolbox(window, project);

        toolboxDock->dockWidget->setVisible(false);

        window->setDockNestingEnabled(true);

        window->addDockWidget(Qt::RightDockWidgetArea, explorerDock->dockWidget);
        window->addDockWidget(Qt::LeftDockWidgetArea, propertiesDock->dockWidget);
        window->addDockWidget(Qt::BottomDockWidgetArea, outputDock->dockWidget);
        window->addDockWidget(Qt::LeftDockWidgetArea, toolboxDock->dockWidget);

        const int projectTabIndex = documentTabs->addTab(placeView, project->name);

        QObject::connect(documentTabs, &QTabWidget::tabCloseRequested, [documentTabs, projectTabIndex, editorPage, window, mainToolBar](int index) {
            QWidget* w = documentTabs->widget(index);
            documentTabs->removeTab(index);
            delete w;

            if (projectTabIndex != index) return;

            if (!editorPage) return;

            auto* workspaceStack = qobject_cast<QStackedWidget*>(editorPage->parentWidget());
            if (workspaceStack) {
                for (auto* oldDock : window->findChildren<QDockWidget*>()) {
                    window->removeDockWidget(oldDock);
                    delete oldDock;
                }

                delete mainToolBar;

                workspaceStack->setCurrentIndex(0);
            }
            });

        centerSplitter->addWidget(documentTabs);
        centerSplitter->setStretchFactor(0, 3);
        centerSplitter->setStretchFactor(1, 1);

        //Add project to Recent list

        QWidget* topLevelWindow = window->window();

        auto recentMenus = topLevelWindow->findChildren<QMenu*>("RecentOpenProjects");

        for (QMenu* fileRecentMenu : recentMenus) {
            QAction* projectInRecents = fileRecentMenu->findChild<QAction*>("RecentProject_" + project->name);
            if (projectInRecents) {
                delete projectInRecents;
            }

            QAction* recentAction = fileRecentMenu->addAction(project->name, [project, editorPage, window]() {
				EditorWindow::initialize(project, editorPage, window);
            });
			recentAction->setObjectName("RecentProject_" + project->name);
        }

		Logger* outputLogger = new Logger(outputDock);
		outputLogger->Info("Project '" + project->name.toStdString() + "' opened successfully.");

		project->logger = outputLogger;

        QObject::connect(playAction, &QAction::toggled, [playAction, documentTabs, outputLogger](bool checked) mutable {
            if (!checked) return;

			playAction->setChecked(false);

			documentTabs->setCurrentIndex(0);
			outputLogger->Error("Error starting server.");
            });
	}
}