#include <core/settings/game/tabs/TechnicalTab.h>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QLabel>
#include <QToolButton>

TechnicalTab::TechnicalTab(QListWidget* listWidget, QStackedWidget* stackedWidget) {
	listWidget->addItem("Technical");

    auto* pageWidget = new QWidget(stackedWidget);
    auto* pageLayout = new QHBoxLayout(pageWidget);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    auto* boxContainer = new QWidget(pageWidget);
    boxContainer->setObjectName("LanguagesBoxContainer");
    boxContainer->setFixedWidth(150);
    boxContainer->setFixedHeight(150);
    boxContainer->setAttribute(Qt::WA_StyledBackground, true);

    auto* boxLayout = new QVBoxLayout(boxContainer);
    boxLayout->setContentsMargins(6, 6, 6, 6);
    boxLayout->setSpacing(4);

    QListWidget* customLanguagesList = new QListWidget(boxContainer);
    customLanguagesList->setObjectName("CustomLanguagesList");

    customLanguagesList->setFrameShape(QFrame::NoFrame);
    customLanguagesList->setStyleSheet("QListWidget { background: transparent; border: none; }");

    auto* addLanguageButton = new QPushButton("Add Language", boxContainer);
    addLanguageButton->setObjectName("AddLanguageButton");
    addLanguageButton->setFixedWidth(130);

    boxLayout->addWidget(customLanguagesList, 1);
    boxLayout->addWidget(addLanguageButton, 0, Qt::AlignHCenter);

    pageLayout->addWidget(boxContainer, 0, Qt::AlignTop);
    pageLayout->addStretch();

    pageWidget->setLayout(pageLayout);

    stackedWidget->addWidget(pageWidget);

    auto* item = new QListWidgetItem(customLanguagesList);

    auto* rowWidget = new QWidget();
    auto* rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setContentsMargins(6, 2, 4, 2);
    rowLayout->setSpacing(0);

    auto* label = new QLabel("Rust", rowWidget);
    label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    auto* menuButton = new QToolButton(rowWidget);
    menuButton->setText("⋮");
    menuButton->setPopupMode(QToolButton::InstantPopup);
    menuButton->setCursor(Qt::PointingHandCursor);
    menuButton->setStyleSheet(
        "QToolButton { border: none; background: transparent; padding: 0px; font-size: 20px; }"
        "QToolButton::menu-indicator { image: none; }"
    );

    auto* menu = new QMenu(menuButton);
    menu->addAction("Edit", [label]() {
        // Edit action
        });
    menu->addAction("Delete", [customLanguagesList, item]() {
        delete customLanguagesList->takeItem(customLanguagesList->row(item));
        });
    menuButton->setMenu(menu);

    rowLayout->addWidget(label);
    rowLayout->addStretch();
    rowLayout->addWidget(menuButton);

    item->setSizeHint(rowWidget->sizeHint());
    customLanguagesList->setItemWidget(item, rowWidget);
}