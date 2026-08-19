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
#include <core/settings/game/tabs/GeneralTab.h>
#include <core/settings/game/tabs/AnticheatTab.h>
#include <core/settings/game/tabs/TechnicalTab.h>

namespace GameSettings {
    void open(QWidget* parent) {
        auto* settingsDialog = new QDialog(parent);
        settingsDialog->setAttribute(Qt::WA_DeleteOnClose);
        settingsDialog->setWindowTitle("Game Settings");
        settingsDialog->resize(800, 500);
        
        auto* listWidget = new QListWidget(settingsDialog);
        listWidget->setFixedWidth(150);

        auto* stackedWidget = new QStackedWidget(settingsDialog);

        new GeneralTab(listWidget, stackedWidget);
        new AnticheatTab(listWidget, stackedWidget);
        new TechnicalTab(listWidget, stackedWidget);

        QObject::connect(listWidget, &QListWidget::currentRowChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);

        listWidget->setCurrentRow(0);

        auto* mainLayout = new QHBoxLayout(settingsDialog);
        mainLayout->addWidget(listWidget);
        mainLayout->addWidget(stackedWidget);

        settingsDialog->show();

        if (QFile file(":/settings.qss"); file.open(QFile::ReadOnly | QFile::Text)) {
            settingsDialog->setStyleSheet(QTextStream(&file).readAll());
        }
    }
}