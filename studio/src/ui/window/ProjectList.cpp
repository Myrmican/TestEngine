#include <QFrame>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QButtonGroup>
#include <ui/window/ProjectList.h>

namespace MainMenu {

	void displayProjectsList(QVBoxLayout* contentLayout) {

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

        QLabel* thVersion = new QLabel("Publication");
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

        contentLayout->addWidget(MainMenu::createProjectRow("Untitled Game", "~/Documents/Untitled Game.tep", "Pismyren", "Public", "1 hour ago"));
        contentLayout->addWidget(MainMenu::createProjectRow("Crazy Shooter Game", "~/Downloads/Crazy Shooter game.tep", "Pismyren", "Private", "3 hours ago"));
        contentLayout->addWidget(MainMenu::createProjectRow("Maple County", "~/Downloads/Maple County.tep", "Big Games", "Not published", "3 hours ago"));

        contentLayout->addStretch();
	}

    QFrame* createProjectRow(const QString& name, const QString& path, const QString& creator, const QString& publication, const QString& modified) {
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
        pubLabel->setFixedWidth(110);
        rowLayout->addWidget(pubLabel);

        QLabel* lastEditLabel = new QLabel(modified);
        lastEditLabel->setObjectName("RowMeta");
        lastEditLabel->setFixedWidth(110);
        rowLayout->addWidget(lastEditLabel);

        QPushButton* moreBtn = new QPushButton("⋯");
        moreBtn->setObjectName("RowIconButton");
        moreBtn->setFixedWidth(30);
        rowLayout->addWidget(moreBtn);

        return row;
    };
}