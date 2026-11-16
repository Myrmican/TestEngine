#include <QStackedWidget>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QLabel>
#include <QButtonGroup>
#include <ui/window/hub/ProjectList.h>
#include <ui/window/hub/ProjectGrid.h>
#include <ui/window/hub/pages/Projects.h>

namespace MainMenu {
	QWidget* initializeProjectsPage(QStackedWidget* contentStack, QStackedWidget* workspaceStack) {
        QWidget* projectsContentContainer = new QWidget();
        projectsContentContainer->setObjectName("ProjectsContentContainer");

        QVBoxLayout* projectsContentLayout = new QVBoxLayout(projectsContentContainer);
        projectsContentLayout->setContentsMargins(32, 24, 32, 40);
        projectsContentLayout->setSpacing(0);

        contentStack->addWidget(projectsContentContainer);

        QStackedWidget* projectsStack = new QStackedWidget(projectsContentContainer);

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
        viewProjectList->setIconSize(QSize(18, 18));
        viewProjectList->setToolTip("List view");

        QRadioButton* viewProjectGrid = new QRadioButton("");
        viewProjectGrid->setIcon(QIcon(":/assets/icons/layout-grid.svg"));
        viewProjectGrid->setIconSize(QSize(18, 18));
        viewProjectGrid->setToolTip("Grid view");

        QButtonGroup* optionsGroup = new QButtonGroup(projectsContentContainer);
        optionsGroup->setExclusive(true);
        optionsGroup->addButton(viewProjectList);
        optionsGroup->addButton(viewProjectGrid);

        viewProjectList->setChecked(true);

        QString segmentedStyle = R"(
            QRadioButton {
                background-color: rgb(30, 30, 30);
                border: 1px solid rgb(102, 102, 102);
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

        projectsContentLayout->addLayout(topHeaderLayout);
        projectsContentLayout->addSpacing(10);

        projectsContentLayout->addWidget(projectsStack);

        projectsStack->addWidget(listPage);
        projectsStack->addWidget(gridPage);

        MainMenu::displayProjectsList(projectsListLayout);

        QObject::connect(searchBar, &QLineEdit::textChanged, [=](const QString& text) {
            QWidget* activePage = projectsStack->currentWidget();

            if (activePage == listPage) {
                QVBoxLayout* listLayout = qobject_cast<QVBoxLayout*>(activePage->layout());
                if (listLayout) {
                    for (int i = 0; i < listLayout->count(); ++i) {
                        QLayoutItem* item = listLayout->itemAt(i);
                        if (item && item->widget()) {
                            QWidget* widget = item->widget();
                            if (widget->objectName() == "ProjectRow") {
                                QLabel* titleLabel = widget->findChild<QLabel*>("RowTitle");
                                if (titleLabel) {
                                    bool matches = titleLabel->text().contains(text, Qt::CaseInsensitive);
                                    widget->setVisible(matches);
                                }
                            }
                        }
                    }
                }
            }
            else if (activePage == gridPage) {
                QGridLayout* gridLayout = qobject_cast<QGridLayout*>(activePage->layout());
                if (gridLayout) {
                    for (int i = 0; i < gridLayout->count(); ++i) {
                        QLayoutItem* item = gridLayout->itemAt(i);
                        if (item && item->widget()) {
                            QWidget* widget = item->widget();
                            if (widget->objectName() == "ProjectCard") {
                                QLabel* titleLabel = widget->findChild<QLabel*>("ProjectName");
                                if (titleLabel) {
                                    bool matches = titleLabel->text().contains(text, Qt::CaseInsensitive);
                                    widget->setVisible(matches);
                                }
                            }
                        }
                    }
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
                if (projectsGridLayout->count() == 0) {
                    MainMenu::displayProjectsGrid(projectsGridLayout);
                }
                projectsStack->setCurrentIndex(1);
            }
            });

        return projectsContentContainer;
	}
}