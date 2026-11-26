#include <QFrame>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QButtonGroup>
#include <QAction>
#include <QDateTime>
#include <QMessageBox>
#include <util/Time.h>
#include <ui/window/hub/ProjectList.h>
#include <ui/window/hub/pages/Projects.h>
#include <ui/menus/MenuManager.h>

namespace MainMenu {

	void displayProjectsList(QVBoxLayout* contentLayout, QStackedWidget* workspaceStack) {

        QFrame* tableHeaderRow = new QFrame();
        tableHeaderRow->setObjectName("TableHeaderRow");
        QHBoxLayout* thLayout = new QHBoxLayout(tableHeaderRow);
        thLayout->setContentsMargins(16, 8, 16, 8);
        thLayout->setSpacing(16);

        QLabel* thFav = new QLabel("☆");
        thFav->setFixedWidth(20);
        thFav->setObjectName("TableHeaderText");
        thLayout->addWidget(thFav);

        QLabel* thThumb = new QLabel("");
        thThumb->setFixedWidth(64);
        thLayout->addWidget(thThumb);

        QLabel* thName = new QLabel("Name");
        thName->setMinimumWidth(250);
        thName->setObjectName("TableHeaderText");
        thLayout->addWidget(thName);

        thLayout->addStretch();

        QLabel* thPlatform = new QLabel("Creator");
        thPlatform->setFixedWidth(120);
        thPlatform->setObjectName("TableHeaderText");
        thLayout->addWidget(thPlatform);

        QLabel* thPlayability = new QLabel("Playability");
        thPlayability->setFixedWidth(110);
        thPlayability->setObjectName("TableHeaderText");
        thLayout->addWidget(thPlayability);

        QLabel* thModified = new QLabel("Modified");
        thModified->setFixedWidth(110);
        thModified->setObjectName("TableHeaderText");
        thLayout->addWidget(thModified);

        QLabel* thActions = new QLabel("");
        thActions->setFixedWidth(30);
        thLayout->addWidget(thActions);

        contentLayout->addWidget(tableHeaderRow);
	}

    QFrame* createProjectRow(const QString& name, const QString& path, const QString& creator, const QString& publication, const std::time_t modified) {
        QString publicationTooltip = "";
		if (publication == "Public") {
			publicationTooltip = "Can be played by anyone.";
		}
		else if (publication == "Private") {
			publicationTooltip = "Can be played by users you give access to.";
		}
		else if (publication == "Unpublished") {
			publicationTooltip = "Cannot be accessed by anyone.";
		}

        QFrame* row = new QFrame();
        row->setObjectName("ProjectRow");

        QHBoxLayout* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(16, 12, 16, 12);
        rowLayout->setSpacing(16);

        QPushButton* favBtn = new QPushButton("☆");
        favBtn->setObjectName("RowIconButton");
        favBtn->setFixedWidth(20);
        rowLayout->addWidget(favBtn);

        QLabel* thumbLabel = new QLabel();
        thumbLabel->setObjectName("ProjectThumbnail");
        thumbLabel->setFixedSize(64, 36);
        thumbLabel->setScaledContents(true);

        QPixmap pixmap(":/icons/gameThumb.png");
        if (pixmap.isNull()) {
            pixmap = QPixmap(64, 36);
            pixmap.fill(QColor("#232527"));
        }
        thumbLabel->setPixmap(pixmap);
        rowLayout->addWidget(thumbLabel);

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

        QString creatorText(creator);
        creatorText.truncate(15);

        if (creator.length() > 15) {
            creatorText += "...";
        }

        QLabel* creatorLabel = new QLabel("@" + creatorText);
        creatorLabel->setObjectName("RowMeta");
        creatorLabel->setFixedWidth(120);
        creatorLabel->setToolTip(creator);
        rowLayout->addWidget(creatorLabel);

        QLabel* pubLabel = new QLabel(publication);
        pubLabel->setObjectName("RowMeta");
		pubLabel->setToolTip(publicationTooltip);
        pubLabel->setFixedWidth(110);
        rowLayout->addWidget(pubLabel);

        QLabel* lastEditLabel = new QLabel(TimeUtils::formatTime(modified));
        lastEditLabel->setObjectName("RowMeta");
        lastEditLabel->setFixedWidth(110);
        lastEditLabel->setToolTip(QDateTime::fromSecsSinceEpoch(modified).toString());
        rowLayout->addWidget(lastEditLabel);

        QPushButton* optionsButton = new QPushButton("⋯");
        optionsButton->setObjectName("RowIconButton");
        optionsButton->setFixedWidth(30);
        rowLayout->addWidget(optionsButton);

        QObject::connect(optionsButton, &QPushButton::clicked, [name, optionsButton, row, path]() {
            QMenu* moreMenu = Menu::create(optionsButton);

            QAction* favoriteAction = moreMenu->addAction("Favorite");
            QAction* deleteAction = moreMenu->addAction("Delete");

            moreMenu->adjustSize();

            QPoint bottomRight = optionsButton->mapToGlobal(QPoint(optionsButton->width(), optionsButton->height()));
            QPoint topRightAligned(bottomRight.x() - moreMenu->width(), bottomRight.y());

            QAction* selectedAction = moreMenu->exec(topRightAligned);
            if (selectedAction == favoriteAction) {

            }
            else if (selectedAction == deleteAction) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(row, "Delete Project", "Are you sure you want to delete " + name + "?");

                if (reply == QMessageBox::Yes) {
                    if (!QFile::remove(path)) {
                        return;
                    }

                    delete row;
                }
                else {
                    delete moreMenu;
                }
            }
            });

        return row;
    };
}