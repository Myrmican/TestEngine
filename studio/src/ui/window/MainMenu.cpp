#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QFrame>
#include <QGridLayout>
#include <QString>
#include <QTimer>
#include <QRadioButton>
#include <QButtonGroup>
#include <QApplication>
#include <project/Project.h>
#include <editor/EditorWindow.h>
#include <ui/ProjectLoadingDialog.h>
#include <ui/window/MainMenu.h>
#include <ui/window/ProjectList.h>
#include <ui/window/ProjectGrid.h>

namespace MainMenu {
    void initialize(QMainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage) {
        QWidget* homePage = new QWidget();
        homePage->setObjectName("HomePage");

        QHBoxLayout* rootLayout = new QHBoxLayout(homePage);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);

        QWidget* sidebar = new QWidget();
        sidebar->setObjectName("Sidebar");
        sidebar->setFixedWidth(220);

        QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setContentsMargins(12, 16, 12, 16);
        sidebarLayout->setObjectName("SidebarLayout");
        sidebarLayout->setSpacing(6);

        QPushButton* newProjectButton = new QPushButton("+ New Project");
        newProjectButton->setObjectName("NewProjectButton");
        QObject::connect(newProjectButton, &QPushButton::clicked, [=]() {
            if (auto project = ProjectManager::createNew(window)) {
                auto loadingDialog = new ProjectLoadingDialog(project->name, window);
                loadingDialog->setAttribute(Qt::WA_DeleteOnClose);
                loadingDialog->show();

                QTimer::singleShot(0, window, [window, workspaceStack, editorPage, project, loadingDialog]() {
                    loadingDialog->setStatus("Building editor...");
                    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

                    new Engine::EditorWindow(project, editorPage, window);

                    workspaceStack->setCurrentIndex(1);
                    loadingDialog->close();
                    });
            }
            });
        sidebarLayout->addWidget(newProjectButton);
        sidebarLayout->addSpacing(12);

        QStringList navItems = { "Projects", "Learn" };
        for (int i = 0; i < navItems.size(); ++i) {
            QPushButton* navBtn = new QPushButton(navItems[i]);
            navBtn->setObjectName("NavBtn");
            navBtn->setCheckable(true);
            navBtn->setAutoExclusive(true);
            if (i == 0) navBtn->setChecked(true);
            sidebarLayout->addWidget(navBtn);
        }

        sidebarLayout->addStretch();
        rootLayout->addWidget(sidebar);

        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setObjectName("MainScroll");

        QWidget* contentContainer = new QWidget();
        contentContainer->setObjectName("ContentContainer");
        
        QVBoxLayout* contentLayout = new QVBoxLayout(contentContainer);
        contentLayout->setContentsMargins(32, 24, 32, 40);
        contentLayout->setSpacing(0);

		QStackedWidget* projectsStack = new QStackedWidget(contentContainer);

        QVBoxLayout* projectsListLayout = new QVBoxLayout();
        projectsListLayout->setContentsMargins(0, 0, 0, 0);
        projectsListLayout->setSpacing(0);

        QWidget* listPage = new QWidget();
        listPage->setLayout(projectsListLayout);

        QGridLayout* projectsGridLayout = new QGridLayout();
        projectsGridLayout->setContentsMargins(0, 0, 0, 0);
        projectsGridLayout->setSpacing(16);
        projectsGridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

        QWidget* gridPage = new QWidget();
        gridPage->setLayout(projectsGridLayout);

        QHBoxLayout* topHeaderLayout = new QHBoxLayout();
		topHeaderLayout->setObjectName("TopHeaderLayout");
        topHeaderLayout->setSpacing(12);

        QLabel* titleLabel = new QLabel("Projects");
        titleLabel->setObjectName("SectionTitle");
        topHeaderLayout->addWidget(titleLabel);
        topHeaderLayout->addStretch();

        QLineEdit* searchBar = new QLineEdit();
        searchBar->setObjectName("SearchInput");
        searchBar->setPlaceholderText("Search");
        searchBar->setFixedWidth(240);
        topHeaderLayout->addWidget(searchBar);

        QRadioButton* viewProjectList = new QRadioButton("");
        viewProjectList->setIcon(QIcon(":/assets/icons/list.svg"));
        viewProjectList->setIconSize(QSize(24, 24));
        viewProjectList->setToolTip("List view");

        QRadioButton* viewProjectGrid = new QRadioButton("");
        viewProjectGrid->setIcon(QIcon(":/assets/icons/layout-grid.svg"));
        viewProjectGrid->setIconSize(QSize(24, 24));
        viewProjectGrid->setToolTip("Grid view");

        QButtonGroup* optionsGroup = new QButtonGroup(contentContainer);
        optionsGroup->setExclusive(true);
        optionsGroup->addButton(viewProjectList);
        optionsGroup->addButton(viewProjectGrid);

        viewProjectList->setChecked(true);

        QString segmentedStyle = R"(
            QRadioButton {
                background: transparent;
                border: 1px solid transparent;
                border-radius: 6px;
                padding: 8px;
            }
            QRadioButton::indicator {
                width: 0px;
                height: 0px;
            }
            QRadioButton:checked {
                background-color: #2a2d32; 
                border: 1px solid #3a3f4b;
            }
            QRadioButton:hover {
                background-color: #22252a;
            }
        )";

        viewProjectList->setStyleSheet(segmentedStyle);
        viewProjectGrid->setStyleSheet(segmentedStyle);

        topHeaderLayout->addWidget(viewProjectList);
        topHeaderLayout->addWidget(viewProjectGrid);

        contentLayout->addLayout(topHeaderLayout);
        contentLayout->addSpacing(10);

		contentLayout->addWidget(projectsStack);

        projectsStack->addWidget(listPage);
        projectsStack->addWidget(gridPage);

        MainMenu::displayProjectsList(projectsListLayout);

        scrollArea->setWidget(contentContainer);
        rootLayout->addWidget(scrollArea);

        QObject::connect(searchBar, &QLineEdit::textChanged, [=](const QString& text) {
            for (int i = 0; i < contentLayout->count(); ++i) {
                QWidget* widget = contentLayout->itemAt(i)->widget();
                if (widget && widget->objectName() == "ProjectRow") {
                    bool matches = widget->findChild<QLabel*>("RowTitle")->text().contains(text, Qt::CaseInsensitive);
                    widget->setVisible(matches);
                }
            }
            });

		QObject::connect(viewProjectList, &QRadioButton::toggled, [=](bool checked) {
			if (checked) {
                projectsStack->setCurrentIndex(0);
			}
			});

        QObject::connect(viewProjectGrid, &QRadioButton::toggled, [=](bool checked) {
            if (checked) {
                if(projectsGridLayout->count() == 0) {
                    MainMenu::displayProjectsGrid(projectsGridLayout);
                }
                projectsStack->setCurrentIndex(1);
            }
            });

        homePage->setStyleSheet(R"(
            #HomePage, #ContentContainer, #MainScroll {
                background-color: #191B1D;
                font-family: "Segoe UI", sans-serif;
            }
            #Sidebar {
                background-color: transparent;
            }
            #SidebarLayout {
                background-color: #1a1a1a;
            }
            #NewProjectButton {
                background-color: #125fcb;
                color: #FFFFFF;
                font-size: 15px;
                text-align: center;
                padding: 6px 5px;
                border-radius: 6px;
            }
            #NavBtn {
                background-color: transparent;
                color: #BDBEC0;
                font-size: 13px;
                text-align: left;
                padding: 10px 14px;
                border-radius: 6px;
                border: none;
            }
            #NavBtn:hover {
                background-color: #252729;
                color: #FFFFFF;
            }
            #NavBtn:checked {
                background-color: #454545;
                color: #FFFFFF;
            }
            #SectionTitle {
                color: #FFFFFF;
                font-size: 24px;
            }
            #ToolbarDropdownBtn {
                background-color: #232527;
                color: #D0D1D3;
                font-size: 13px;
                padding: 6px 12px;
                border-radius: 4px;
                border: 1px solid #323436;
            }
            #ToolbarDropdownBtn:hover {
                background-color: #2D2F31;
            }
            #SearchInput {
                background-color: #232527;
                color: #FFFFFF;
                font-size: 13px;
                padding: 6px 12px;
                border-radius: 4px;
                border: 1px solid #323436;
            }
            #PrimaryActionBtn {
                background-color: #0E70EA;
                color: #FFFFFF;
                font-size: 13px;
                padding: 6px 14px;
                border-radius: 4px;
                border: none;
            }
            #PrimaryActionBtn:hover {
                background-color: #117FF8;
            }
            #TableHeaderRow {
                background-color: transparent;
                border-bottom: 1px solid #2b2d2f;
            }
            #TableHeaderText {
                color: #8E9092;
                font-size: 12px;
                font-weight: 600;
            }
            #ProjectRow {
                background-color: transparent;
                border-bottom: 1px solid #222426;
            }
            #ProjectRow:hover {
                background-color: #202224;
            }
            #RowTitle {
                color: #f2f2f2;
                font-size: 13px;
            }
            #RowSubtitle {
                color: #8E9092;
                font-size: 11px;
            }
            #RowMeta {
                color: #C0C1C3;
                font-size: 12px;
            }
            #RowIconButton {
                background-color: transparent;
                color: #8E9092;
                border: none;
                font-size: 14px;
            }
            #RowIconButton:hover {
                color: #FFFFFF;
            }
            #RowDropdownBtn {
                background-color: transparent;
                color: #D0D1D3;
                font-size: 12px;
                text-align: left;
                padding: 4px 8px;
                border-radius: 4px;
                border: 1px solid transparent;
            }
            #RowDropdownBtn:hover {
                background-color: #2A2C2E;
                border-color: #353739;
            }
        )");

        workspaceStack->addWidget(homePage);
        workspaceStack->setCurrentWidget(homePage);
    }
}