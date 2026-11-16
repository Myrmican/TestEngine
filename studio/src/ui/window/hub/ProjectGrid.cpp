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
#include <QEvent>
#include <QDateTime>
#include <ui/window/hub/ProjectGrid.h>

class CardHoverFilter : public QObject {
    Q_OBJECT
protected:
    bool eventFilter(QObject* watched, QEvent* event) override {
        if (event->type() == QEvent::Enter) {
            return true;
        }
        if (event->type() == QEvent::Leave) {
            return true;
        }
        return QObject::eventFilter(watched, event);
    }
};

namespace MainMenu {

    void displayProjectsGrid(QGridLayout* contentLayout) {
        struct ProjectInfo {
            QString name;
            QString path;
            QString creator;
            QString publication;
            std::time_t modified;
        };

        QList<ProjectInfo> projects = {
            { "Untitled Game", "~/Documents/Untitled Game.tep", "Pismyren", "Public", 1794690182 },
            { "Crazy Shooter Game", "~/Downloads/Crazy Shooter game.tep", "Pismyren", "Private", 1794690182 },
            { "Maple County", "~/Downloads/Maple County.tep", "Central Roleplay Community", "Unpublished", 1794690182 },
            { "Untitled Game", "~/Documents/Untitled Game.tep", "Pismyren", "Public", 1794690182 },
            { "Crazy Shooter Game", "~/Downloads/Crazy Shooter game.tep", "Pismyren", "Private", 1794690182 },
            { "Maple County", "~/Downloads/Maple County.tep", "Central Roleplay Community", "Unpublished", 1794690182 }
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

    QFrame* createProjectCard(const QString& name, const QString& path, const QString& creator, const QString& publication, const std::time_t modified) {
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

        QLabel* thumbnailLabel = new QLabel();
        thumbnailLabel->setObjectName("ProjectThumbnail");
        thumbnailLabel->setFixedHeight(110);
        thumbnailLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        thumbnailLabel->setScaledContents(true);
        thumbnailLabel->setStyleSheet(
            "border-radius: 5px;"
            "background-color: #1e1f22;"
        );

        QPixmap pixmap(":/icons/gameThumb.png");
        QPixmap scaledPixmap = pixmap.scaled(180, 110, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        QPixmap target(180, 110);
        target.fill(Qt::transparent);

        QPainter painter(&target);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        QPainterPath painterPath;
        painterPath.addRoundedRect(target.rect(), 5, 5);
        painter.setClipPath(painterPath);
        painter.drawPixmap(0, 0, scaledPixmap);

        thumbnailLabel->setPixmap(target);
        cardLayout->addWidget(thumbnailLabel);

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

        QLabel* modLabel = new QLabel("Modified " + QDateTime::fromSecsSinceEpoch(modified).toString("MM/dd/yyyy"));
        modLabel->setObjectName("ProjectModified");
        modLabel->setStyleSheet("color: #777777; font-size: 11px;");
        textLayout->addWidget(modLabel);

        QLabel* creatorLabel = new QLabel("@" + creator);
        creatorLabel->setObjectName("ProjectCreator");
        creatorLabel->setStyleSheet("color: #777777; font-size: 11px;");
        textLayout->addWidget(creatorLabel);

        cardLayout->addLayout(textLayout);

		projectCard->installEventFilter(new CardHoverFilter());

        return projectCard;
    }
}