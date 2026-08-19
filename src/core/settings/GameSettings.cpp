#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QToolButton>
#include <QMenu>

namespace GameSettings {
    void open(QWidget* parent) {
        auto* settingsDialog = new QDialog(parent);
        settingsDialog->setAttribute(Qt::WA_DeleteOnClose);
        settingsDialog->setWindowTitle("Game Settings");
        settingsDialog->resize(800, 500);

        auto* listWidget = new QListWidget(settingsDialog);
        listWidget->setFixedWidth(150);
        listWidget->addItem("General");
        listWidget->addItem("Technical");

        auto* stackedWidget = new QStackedWidget(settingsDialog);

        auto* generalPage = new QWidget(stackedWidget);
        auto* generalLayout = new QVBoxLayout(generalPage);
        generalPage->setLayout(generalLayout);

        //Technical settings page

        auto* technicalPage = new QWidget(stackedWidget);
        auto* technicalLayout = new QHBoxLayout(technicalPage);
        technicalLayout->setContentsMargins(0, 0, 0, 0);

        auto* boxContainer = new QWidget(technicalPage);
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

        technicalLayout->addWidget(boxContainer, 0, Qt::AlignTop);
        technicalLayout->addStretch();

        technicalPage->setLayout(technicalLayout);

        stackedWidget->addWidget(generalPage);
        stackedWidget->addWidget(technicalPage);

        QObject::connect(listWidget, &QListWidget::currentRowChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);

        listWidget->setCurrentRow(0);

        auto* mainLayout = new QHBoxLayout(settingsDialog);
        mainLayout->addWidget(listWidget);
        mainLayout->addWidget(stackedWidget);

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

        settingsDialog->show();

        if (QFile file(":/settings.qss"); file.open(QFile::ReadOnly | QFile::Text)) {
            settingsDialog->setStyleSheet(QTextStream(&file).readAll());
        }
    }
}