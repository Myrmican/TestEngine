#include <QWidget>
#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QFile>

namespace StudioSettings {
	void open(QWidget* parent) {
        auto* settingsDialog = new QDialog(parent);
        settingsDialog->setAttribute(Qt::WA_DeleteOnClose);
        settingsDialog->setWindowTitle("Studio Settings");
		settingsDialog->setWindowModality(Qt::ApplicationModal);
        settingsDialog->resize(800, 500);

        auto* listWidget = new QListWidget(settingsDialog);
        listWidget->setFixedWidth(150);
        listWidget->addItem("Studio");
        listWidget->addItem("Script Editor");

        auto* stackedWidget = new QStackedWidget(settingsDialog);

        auto* studioPage = new QWidget(stackedWidget);
        auto* studioLayout = new QVBoxLayout(studioPage);
        studioPage->setLayout(studioLayout);

        //Script Editor settings page

        auto* scriptEditorPage = new QWidget(stackedWidget);
        auto* scriptEditorLayout = new QHBoxLayout(scriptEditorPage);
        scriptEditorLayout->setContentsMargins(0, 0, 0, 0);

        scriptEditorPage->setLayout(scriptEditorLayout);

        stackedWidget->addWidget(studioPage);
        stackedWidget->addWidget(scriptEditorPage);

        QObject::connect(listWidget, &QListWidget::currentRowChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);

        listWidget->setCurrentRow(0);

        auto* mainLayout = new QHBoxLayout(settingsDialog);
        mainLayout->addWidget(listWidget);
        mainLayout->addWidget(stackedWidget);

        auto* rowWidget = new QWidget();
        auto* rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(6, 2, 4, 2);
        rowLayout->setSpacing(0);

        settingsDialog->show();

        if (QFile file(":/settings.qss"); file.open(QFile::ReadOnly | QFile::Text)) {
            settingsDialog->setStyleSheet(QTextStream(&file).readAll());
        }
	}
}