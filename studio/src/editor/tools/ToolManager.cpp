#include <editor/tools/ToolManager.h>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QAction>
#include <QToolBar>
#include <QObject>
#include <QString>
#include <QIcon>
#include <QApplication>

namespace ToolManager {
    QAction* createToolAction(const QIcon& icon, const QString& text, QObject* parent) {
        QAction* action = new QAction(icon, text, parent);
        action->setCheckable(true);
        return action;
    }

    QWidget* createToolButtonWidget(QAction* action, QToolBar* toolBar) {
        QWidget* container = new QWidget(toolBar);
        QVBoxLayout* layout = new QVBoxLayout(container);
        layout->setContentsMargins(0, 4, 0, 4);
        layout->setSpacing(4);

        QToolButton* iconButton = new QToolButton(container);
        iconButton->setIcon(action->icon());
        iconButton->setIconSize(QSize(32, 32));
        iconButton->setFixedSize(44, 44);
        iconButton->setFocusPolicy(Qt::NoFocus);
        iconButton->setCheckable(true);
        iconButton->setChecked(action->isChecked());

        iconButton->setStyleSheet(
            "QToolButton {"
            "    background: transparent;"
            "    border: none;"
            "    border-radius: 6px;"
            "}"
            "QToolButton:hover {"
            "    background-color: #2f3039;"
            "}"
            "QToolButton:checked {"
            "    background-color: #3a7afe;"
            "}"
            "QToolButton:checked:hover {"
            "    background-color: #4a86ff;"
            "}"
        );

        QLabel* textLabel = new QLabel(action->text(), container);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet("color: #b1b1b1; font-size: 11px; background: transparent;");
        layout->addWidget(iconButton, 0, Qt::AlignCenter);
        layout->addWidget(textLabel, 0, Qt::AlignCenter);

        QObject::connect(iconButton, &QToolButton::clicked, action, [action](bool checked) {
            action->setChecked(checked);
            });

        QObject::connect(action, &QAction::toggled, iconButton, &QToolButton::setChecked);

        return container;
    }

	QWidget* getTool(const QString& objectName, QWidget* parent) {
        QWidget* toolWidget = createToolButtonWidget(createToolAction(QIcon(), objectName, parent), nullptr);



		return toolWidget;
	}
}