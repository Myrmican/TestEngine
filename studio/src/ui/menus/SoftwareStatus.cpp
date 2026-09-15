#include <ui/menus/SoftwareStatus.h>
#include <ui/menus/MenuManager.h>
#include <Velopack.h>
#include <QString>
#include <QPalette>
#include <format>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QWKWidgets/widgetwindowagent.h>

SoftwareStatus::SoftwareStatus(QMenuBar* menuBar) {
    QPushButton* updateButton = new QPushButton("Update");
    updateButton->setFlat(true);
    updateButton->setCursor(Qt::PointingHandCursor);
    updateButton->setAttribute(Qt::WA_Hover, true);
    updateButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(34, 197, 94, 25);"
        "   font-weight: bold;"
        "   border: 1px solid rgba(34, 197, 94, 1);"
        "   color: rgb(34, 197, 94);"
        "   padding: 4px 12px;"
        "   border-radius: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(34, 197, 94, 50);"
        "}"
    );

    QObject::connect(updateButton, &QPushButton::clicked, updateButton, [updateButton]() {
        std::exit(0);
        });

    updateButton->setHidden(true);

    menuBar->setCornerWidget(updateButton, Qt::TopLeftCorner);
}