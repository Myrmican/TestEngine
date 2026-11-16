#pragma once

#include <QWidget>
#include <QEvent>
#include <project/Project.h>

class QWidget;
class QStackedWidget;

namespace MainMenu {
	QWidget* initializeProjectsPage(QStackedWidget* contentStack, QStackedWidget* workspaceStack);

    class ProjectEventFilter : public QObject {
        Q_OBJECT
    public:
        ProjectEventFilter(const QString& path, QStackedWidget* workspaceStack, QObject* parent = nullptr)
            : QObject(parent), m_path(path), m_workspaceStack(workspaceStack) {
        }

    protected:
        bool eventFilter(QObject* obj, QEvent* event) override {
            if (event->type() == QEvent::MouseButtonPress) {
                std::cout << parent()->objectName().toStdString() << std::endl;
                ProjectManager::openProject(m_path, m_workspaceStack);

                return true;
            }

            return QObject::eventFilter(obj, event);
        }

    private:
        QString m_path;
        QStackedWidget* m_workspaceStack;
    };
}