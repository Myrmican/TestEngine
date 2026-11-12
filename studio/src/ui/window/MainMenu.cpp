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
#include <QTimer>
#include <QApplication>
#include <project/Project.h>
#include <editor/EditorWindow.h>
#include <ui/ProjectLoadingDialog.h>
#include <ui/window/MainMenu.h>

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

        QHBoxLayout* topHeaderLayout = new QHBoxLayout();
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

        contentLayout->addLayout(topHeaderLayout);
        contentLayout->addSpacing(10);

        QFrame* tableHeaderRow = new QFrame();
        tableHeaderRow->setObjectName("TableHeaderRow");
        QHBoxLayout* thLayout = new QHBoxLayout(tableHeaderRow);
        thLayout->setContentsMargins(16, 8, 16, 8);
        thLayout->setSpacing(16);

        QLabel* thFav = new QLabel("☆");
        thFav->setFixedWidth(20);
        thFav->setObjectName("TableHeaderText");
        thLayout->addWidget(thFav);

        QLabel* thName = new QLabel("Name");
        thName->setMinimumWidth(250);
        thName->setObjectName("TableHeaderText");
        thLayout->addWidget(thName);

        thLayout->addStretch();

        QLabel* thPlatform = new QLabel("Creator");
        thPlatform->setFixedWidth(120);
        thPlatform->setObjectName("TableHeaderText");
        thLayout->addWidget(thPlatform);

        QLabel* thVersion = new QLabel("Visibility");
        thVersion->setFixedWidth(110);
        thVersion->setObjectName("TableHeaderText");
        thLayout->addWidget(thVersion);

        QLabel* thModified = new QLabel("Modified ↓");
        thModified->setFixedWidth(110);
        thModified->setObjectName("TableHeaderText");
        thLayout->addWidget(thModified);

        QLabel* thActions = new QLabel("");
        thActions->setFixedWidth(30);
        thLayout->addWidget(thActions);

        contentLayout->addWidget(tableHeaderRow);

        auto createProjectRow = [=](const QString& name, const QString& path, const QString& platform, const QString& version, const QString& modified) -> QFrame* {
            QFrame* row = new QFrame();
            row->setObjectName("ProjectRow");
            QHBoxLayout* rowLayout = new QHBoxLayout(row);
            rowLayout->setContentsMargins(16, 12, 16, 12);
            rowLayout->setSpacing(16);

            QPushButton* favBtn = new QPushButton("☆");
            favBtn->setObjectName("RowIconButton");
            favBtn->setFixedWidth(20);
            rowLayout->addWidget(favBtn);

            QVBoxLayout* nameLayout = new QVBoxLayout();
            nameLayout->setContentsMargins(0, 0, 0, 0);
            nameLayout->setSpacing(2);

            QLabel* nameLbl = new QLabel(name);
            nameLbl->setObjectName("RowTitle");

            QLabel* pathLbl = new QLabel(path);
            pathLbl->setObjectName("RowSubtitle");
            nameLayout->addWidget(nameLbl);
            nameLayout->addWidget(pathLbl);

            QWidget* nameWidget = new QWidget();
            nameWidget->setLayout(nameLayout);
            nameWidget->setMinimumWidth(250);
            rowLayout->addWidget(nameWidget);

            rowLayout->addStretch();

            QLabel* platCombo = new QLabel(platform);
            platCombo->setObjectName("RowMeta");
            platCombo->setFixedWidth(120);
            rowLayout->addWidget(platCombo);

            QLabel* verLbl = new QLabel(version);
            verLbl->setObjectName("RowMeta");
            verLbl->setFixedWidth(110);
            rowLayout->addWidget(verLbl);

            QLabel* modLbl = new QLabel(modified);
            modLbl->setObjectName("RowMeta");
            modLbl->setFixedWidth(110);
            rowLayout->addWidget(modLbl);

            QPushButton* moreBtn = new QPushButton("⋯");
            moreBtn->setObjectName("RowIconButton");
            moreBtn->setFixedWidth(30);
            rowLayout->addWidget(moreBtn);

            return row;
            };

        contentLayout->addWidget(createProjectRow("Untitled Game", "~/Documents/Untitled Game.tep", "@Pismyren", "Public", "1 hour ago"));
        contentLayout->addWidget(createProjectRow("Crazy Shooter Game", "~/Downloads/Crazy Shooter game.tep", "@Pismyren", "Private", "3 hours ago"));

        contentLayout->addStretch();
        scrollArea->setWidget(contentContainer);
        rootLayout->addWidget(scrollArea);

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