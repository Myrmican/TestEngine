#pragma once

#include <QEvent>

class QGridLayout;
class QFrame;
class QString;
class QStackedWidget;

namespace MainMenu {
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

	void displayProjectsGrid(QGridLayout* contentContainer, QStackedWidget* workspaceStack);
	QFrame* createProjectCard(const QString& name, const QString& path, const QString& creator, const QString& publication, const std::time_t modified);
}