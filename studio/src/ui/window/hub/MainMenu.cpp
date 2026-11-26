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
#include <ui/window/hub/MainMenu.h>
#include <ui/window/hub/pages/Projects.h>
#include <ui/window/MainWindow.h>

namespace MainMenu {
    void initialize(MainWindow* window, QStackedWidget* workspaceStack, QWidget* editorPage) {
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

        rootLayout->addWidget(sidebar);

        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setObjectName("MainScroll");

        QStackedWidget* contentStack = new QStackedWidget(homePage);

        QStringList navItems = { "Projects", "Learn" };
        for (int i = 0; i < navItems.size(); ++i) {
            QPushButton* navBtn = new QPushButton(navItems[i]);
            navBtn->setObjectName("NavBtn");
            navBtn->setCheckable(true);
            navBtn->setAutoExclusive(true);
            if (i == 0) navBtn->setChecked(true);
            sidebarLayout->addWidget(navBtn);

            QObject::connect(navBtn, &QRadioButton::toggled, [=](bool checked) {
                if (checked) {
                    contentStack->setCurrentIndex(i);
                }
                });
        }

        sidebarLayout->addStretch();

		QWidget* projectsContentContainer = MainMenu::initializeProjectsPage(contentStack, workspaceStack);

        scrollArea->setWidget(projectsContentContainer);
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