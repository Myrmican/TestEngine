#include <ui/menus/MenuManager.h>
#include <editor/tools/ToolManager.h>
#include <ui/Toolbar.h>
#include <ui/ribbon/Ribbon.h>
#include <ui/ribbon/RibbonStats.h>
#include <ui/window/MainWindow.h>
#include <QMenu>
#include <QFrame>
#include <QStackedWidget>
#include <QTabBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QComboBox>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <algorithm>
#include <vector>

std::vector<std::string> ribbonTabNames = { "Home", "Model", "Avatar", "Test", "Plugins", "Script", "UI" };

void ConnectContextMenu(QWidget* ribbonBar, QTabBar* ribbonTabs, MainWindow* window, QWidget* statsWidget) {
    QObject::connect(ribbonBar, &QWidget::customContextMenuRequested,
        [ribbonBar, ribbonTabs, statsWidget, window](const QPoint& pos) {
            QPoint tabBarPos = ribbonBar->mapFrom(ribbonBar, pos);

            QMenu* contextMenu = Menu::create(window);

            int index = ribbonTabs->tabAt(tabBarPos);

            if (index != -1) {
				QMenu* tabMenu = Menu::create(ribbonTabs);
                //tabMenu->addAction("Rename tab");
                tabMenu->addAction("Hide tab", [ribbonTabs, index]() {
                    ribbonTabs->setTabVisible(index, false);
                });

                QAction* selectedAction = tabMenu->exec(ribbonBar->mapToGlobal(pos));

            } else {
                QMenu* ribbonTabMenu = Menu::create(contextMenu, "Ribbon tabs");

                contextMenu->addMenu(ribbonTabMenu);

                QAction* manageRibbonTabs = ribbonTabMenu->addAction("Manage");
				QMenu* ribbonTabsList = Menu::create(ribbonTabMenu, "Toggle");
                ribbonTabMenu->addMenu(ribbonTabsList);

                QMenu* ribbonStats = Engine::RibbonStats::createStatsToggleMenu(contextMenu, statsWidget);
                contextMenu->addMenu(ribbonStats);

                QList<QAction*> tabActions = ribbonTabs->findChildren<QAction*>();

                for (const auto& tabName : ribbonTabNames) {
                    QAction* ribbonTabAction = ribbonTabsList->addAction(QString::fromStdString(tabName));
                    ribbonTabAction->setCheckable(true);

                    for (int i = 0; i < ribbonTabs->count(); ++i) {
                        if (ribbonTabs->tabText(i) != QString::fromStdString(tabName)) continue;

                        ribbonTabAction->setChecked(ribbonTabs->isTabVisible(i));
                        QObject::connect(ribbonTabAction, &QAction::triggered, [ribbonTabs, i]() {
                            ribbonTabs->setTabVisible(i, !ribbonTabs->isTabVisible(i));
                        });
                        break;
                    }
                }

                QAction* selectedAction = contextMenu->exec(ribbonBar->mapToGlobal(pos));

                if (selectedAction == manageRibbonTabs) {
                    qDebug() << "Manage ribbon tabs action triggered";
                }
            }
        }
    );
}

static QWidget* setupStats(QWidget* parent) {
    QWidget* statsContainer = new QWidget(parent);
    statsContainer->setObjectName("RibbonStats");

    QHBoxLayout* statsLayout = new QHBoxLayout(statsContainer);
    statsLayout->setContentsMargins(8, 8, 30, 8);
    statsLayout->setSpacing(16);
    statsLayout->setObjectName("RibbonStatsLayout");

    statsContainer->setStyleSheet(
        "#RibbonStats QLabel {"
        "    color: #cccccc;"
        "    font-size: 13px;"
        "}"
        "#RibbonStats QLabel[role=\"value\"] {"
        "    color: #ffffff;"
        "    font-weight: bold;"
        "}"
    );

    return statsContainer;
};

static QWidget* setupQuickActions(QWidget* parent) {
	QWidget* actionsContainer = new QWidget(parent);
	actionsContainer->setObjectName("RibbonQuickActions");

    QHBoxLayout* actionsLayout = new QHBoxLayout(actionsContainer);
    actionsLayout->setContentsMargins(8, 8, 8, 8);
    actionsLayout->setSpacing(16);

    /*QWidget* playAction = ToolManager::createQuickTool("Play", actionsContainer);
	actionsLayout->addWidget(playAction);*/

    return actionsContainer;
}

static void applyComboStyle(QComboBox* comboBox, bool darkMode) {
    QPalette palette = comboBox->palette();

    QColor background;
    QColor hoverBackground;
    QColor textColor;
    QColor borderColor;
    if (darkMode) {
        background = QColor("#242424");
        hoverBackground = QColor("#3a3a3a");
        textColor = QColor("#d8d8d8");
        borderColor = QColor("#383838");
    }

    palette.setColor(QPalette::Window, background);
    palette.setColor(QPalette::Base, background);
    palette.setColor(QPalette::Button, background);
    palette.setColor(QPalette::ButtonText, textColor);
    palette.setColor(QPalette::WindowText, textColor);
    palette.setColor(QPalette::Text, textColor);
    palette.setColor(QPalette::Highlight, hoverBackground);
    palette.setColor(QPalette::HighlightedText, Qt::white);
    palette.setColor(QPalette::Mid, borderColor);

    comboBox->setPalette(palette);

    if (QAbstractItemView* popupView = comboBox->view()) {
        popupView->setPalette(palette);
        if (QWidget* popupWin = popupView->window()) {
            popupWin->setPalette(palette);
        }
    }
}

static bool validateSceneCreation(QString sceneName) {
    if (sceneName.length() > 15 || sceneName.length() <= 5) return false;

    return true;
}

static QComboBox* createSceneSwitcher(QFrame* parent) {
    QComboBox* switcher = new QComboBox(parent);
    switcher->setFixedSize(130, 28);
    switcher->setIconSize(QSize(20, 20));
    switcher->setFont(QFont("Inter", 10, QFont::Medium));

    auto* listView = new QListView(switcher);
    listView->setMouseTracking(true);
    listView->setFrameShape(QFrame::NoFrame);
    switcher->setView(listView);

    listView->setStyleSheet(
        "QListView {"
        "   background-color: rgb(30, 30, 30)"
        "}"
    );

    applyComboStyle(switcher, true);

    switcher->addItem("Main");

    switcher->insertSeparator(switcher->count());

    switcher->addItem("New scene...");

    QObject::connect(switcher, &QComboBox::currentTextChanged, switcher, [=](QString text) {
        if (text == "New scene...") {
            const int newIndex = switcher->count() - 2;

            QDialog dialog(parent);
            dialog.setWindowTitle("Create Project");
            dialog.setMinimumSize(480, 250);
            dialog.setModal(true);

            QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
            mainLayout->setContentsMargins(20, 20, 20, 20);
            mainLayout->setSpacing(25);

            QFormLayout* formLayout = new QFormLayout();
            formLayout->setLabelAlignment(Qt::AlignLeft);
            formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

            QLineEdit* nameEdit = new QLineEdit("Scene " + QString::number(newIndex + 1));
            nameEdit->setMaxLength(15);
            formLayout->addRow("Scene Name", nameEdit);

            QLabel* sceneNameError = new QLabel("");
            sceneNameError->setStyleSheet("color: red;");
            sceneNameError->hide();

            formLayout->addWidget(sceneNameError);

            mainLayout->addLayout(formLayout);
            mainLayout->addStretch();

            auto* buttonBox = new QDialogButtonBox(&dialog);

            QPushButton* cancelButton = buttonBox->addButton("Cancel", QDialogButtonBox::NoRole);
            QPushButton* saveButton = buttonBox->addButton("Save", QDialogButtonBox::NoRole);

            QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
            QObject::connect(saveButton, &QPushButton::clicked, &dialog, &QDialog::accept);

            if (QPushButton* saveButton = buttonBox->button(QDialogButtonBox::Save)) {
                saveButton->setText("Create Scene");
            }

            mainLayout->addWidget(buttonBox);

            nameEdit->setFocus();

            if (dialog.exec() != QDialog::Accepted || !validateSceneCreation(nameEdit->text())) {
                switcher->setCurrentIndex(0);
                return nullptr;
            }

            switcher->insertItem(newIndex, nameEdit->text());
            switcher->setCurrentIndex(newIndex);
        }
        });

    return switcher;
}

namespace Engine {
    Ribbon::Ribbon(MainWindow* parent) : QFrame(parent) {
        setObjectName("RibbonFrame");
        setStyleSheet("QFrame { border-bottom: 1px solid #2A2A2A; background-color: #1E1E1E; }");
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        setContextMenuPolicy(Qt::CustomContextMenu);

        QToolBar* ribbonToolBar = new QToolBar("RibbonToolBar", parent);
        ribbonToolBar->setMovable(false);
        ribbonToolBar->setFloatable(false);
        ribbonToolBar->toggleViewAction()->setVisible(false);
        ribbonToolBar->addWidget(this);

        ribbonToolbar = ribbonToolBar;

        QTabBar* ribbonTabs = new QTabBar(this);
        ribbonTabs->setObjectName("RibbonTabs");
        ribbonTabs->setDrawBase(false);
        ribbonTabs->setElideMode(Qt::ElideRight);
        ribbonTabs->setUsesScrollButtons(false);
        ribbonTabs->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        ribbonTabs->setExpanding(false);
        ribbonTabs->setFixedHeight(32);

        ribbonTabs->setStyleSheet(
            "QTabBar {"
            "    background: transparent;"
            "    border: none;"
            "    outline: none;"
            "}"
            "QTabBar::tab {"
            "   background: transparent;"
            "   color: #9ca3af;"
            "   height: 32px;"
            "   min-width: 36px;"
            "   padding: 0 16px;"
            "   margin-right: 2px;"
            "   border-top-left-radius: 5px;"
            "   border-top-right-radius: 5px;"
            "   border-top: 2px solid transparent;"
            "}"
            "QTabBar::tab:hover {"
            "    background-color: #252525;"
            "    color: #cccccc;"
            "}"
            "QTabBar::tab:selected {"
            "    background-color: #2A2A2A;"
            "    color: #ffffff;"
            "    font-weight: 750;"
            "    border-top: 2px solid #3b82f6;"
            "}"
        );

        QStackedWidget* ribbonStack = new QStackedWidget(this);

        QWidget* statsWidget = setupStats(this);
        QWidget* quickActions = setupQuickActions(this);

        

        QHBoxLayout* topLayout = new QHBoxLayout();
        topLayout->setContentsMargins(8, 4, 8, 0);
        topLayout->setSpacing(16);
        topLayout->addWidget(ribbonTabs);
        topLayout->addWidget(quickActions);
        topLayout->addStretch();
        topLayout->addWidget(statsWidget);
        topLayout->addWidget(createSceneSwitcher(this));

        for (const auto& tabName : ribbonTabNames) {
            QWidget* pageWidget = ToolManager::createTools(ribbonStack, tabName);
            ribbonStack->addWidget(pageWidget);
            ribbonTabs->addTab(QString::fromStdString(tabName));
        }

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->addLayout(topLayout);
        setLayout(mainLayout);

        toolbar = Toolbar::create(parent);

        toolbar->addWidget(ribbonStack);

        parent->addToolBar(Qt::TopToolBarArea, ribbonToolbar);
        parent->addToolBarBreak(Qt::TopToolBarArea);
        parent->addToolBar(Qt::TopToolBarArea, toolbar);

        ConnectContextMenu(this, ribbonTabs, parent, statsWidget);

        QObject::connect(ribbonTabs, &QTabBar::currentChanged,
            ribbonStack, &QStackedWidget::setCurrentIndex);

        QObject::connect(ribbonTabs, &QTabBar::tabBarDoubleClicked, [ribbonStack](int index) {
            ribbonStack->setVisible(!ribbonStack->isVisible());
            });
    }
}