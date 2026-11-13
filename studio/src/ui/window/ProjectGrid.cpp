#include <QFrame>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QButtonGroup>
#include <QGridLayout>
#include <QPainter>
#include <QPainterPath>
#include <ui/window/ProjectGrid.h>

namespace MainMenu {

    void displayProjectsGrid(QGridLayout* contentLayout) {
        struct ProjectInfo {
            QString name;
            QString path;
            QString creator;
            QString publication;
            QString modified;
        };

        QList<ProjectInfo> projects = {
            { "Untitled Game", "~/Documents/Untitled Game.tep", "Pismyren", "Public", "1 hour ago" },
            { "Crazy Shooter Game", "~/Downloads/Crazy Shooter game.tep", "Pismyren", "Private", "3 hours ago" },
            { "Maple County", "~/Downloads/Maple County.tep", "Central Roleplay Community", "Not published", "3 hours ago" },
            { "Untitled Game", "~/Documents/Untitled Game.tep", "Pismyren", "Public", "1 hour ago" },
            { "Crazy Shooter Game", "~/Downloads/Crazy Shooter game.tep", "Pismyren", "Private", "3 hours ago" },
            { "Maple County", "~/Downloads/Maple County.tep", "Central Roleplay Community", "Not published", "3 hours ago" }
        };

        int maxColumns = 6;
        for (int i = 0; i < projects.size(); ++i) {
            int row = i / maxColumns;
            int col = i % maxColumns;

            QFrame* card = MainMenu::createProjectCard(
                projects[i].name,
                projects[i].path,
                projects[i].creator,
                projects[i].publication,
                projects[i].modified
            );
            contentLayout->addWidget(card, row, col);
        }
    }

    QFrame* createProjectCard(const QString& name, const QString& path, const QString& creator, const QString& publication, const QString& modified) {
        QFrame* projectCard = new QFrame();
        projectCard->setObjectName("ProjectCard");
        projectCard->setFixedWidth(190);
		projectCard->setFixedHeight(250);

        projectCard->setStyleSheet(
            "#ProjectCard {"
            "   background-color: #2b2d30;"
            "   border: 1px solid #383a3f;"
            "   border-radius: 8px;"
            "}"
            "#ProjectCard:hover {"
            "   border: 1px solid #5a5e66;"
            "}"
        );

        QVBoxLayout* cardLayout = new QVBoxLayout(projectCard);
        cardLayout->setContentsMargins(5, 8, 5, 6);
        cardLayout->setSpacing(5);

        QLabel* thumbLabel = new QLabel();
        thumbLabel->setObjectName("ProjectThumbnail");
        thumbLabel->setFixedHeight(110);
        thumbLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        thumbLabel->setScaledContents(true);
        thumbLabel->setStyleSheet(
            "border-top-left-radius: 8px;"
            "border-top-right-radius: 8px;"
            "background-color: #1e1f22;"
        );

        QPixmap pixmap(":/icons/gameThumb.png");
        if (pixmap.isNull()) {
            pixmap = QPixmap(180, 110);
            pixmap.fill(QColor("#232527"));
        }
        else {
            pixmap = pixmap.scaled(180, 110, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        }

        thumbLabel->setPixmap(pixmap);
        cardLayout->addWidget(thumbLabel);

        QVBoxLayout* textLayout = new QVBoxLayout();
        textLayout->setContentsMargins(5, 4, 12, 0);
        textLayout->setSpacing(4);

        QLabel* pubLabel = new QLabel(publication.toUpper());
        pubLabel->setObjectName("ProjectPublication");
        pubLabel->setStyleSheet("color: #888888; font-size: 9px; font-weight: bold;");
        textLayout->addWidget(pubLabel);

        QLabel* nameLabel = new QLabel(name);
        nameLabel->setObjectName("ProjectName");
        nameLabel->setStyleSheet("color: #f2f2f2; font-size: 13px; font-weight: bold;");
        nameLabel->setWordWrap(true);
        textLayout->addWidget(nameLabel);

        QLabel* modLabel = new QLabel("Modified " + modified);
        modLabel->setObjectName("ProjectModified");
        modLabel->setStyleSheet("color: #777777; font-size: 11px;");
        textLayout->addWidget(modLabel);

        QLabel* creatorLabel = new QLabel("@" + creator);
        creatorLabel->setObjectName("ProjectCreator");
        creatorLabel->setStyleSheet("color: #777777; font-size: 11px;");
        textLayout->addWidget(creatorLabel);

        cardLayout->addLayout(textLayout);

        return projectCard;
    }
}