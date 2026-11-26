#pragma once

#include <project/Project.h>
#include <project/saving/SaveProject.h>
#include <QCloseEvent>
#include <QMainWindow>
#include <QMessageBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    bool unsavedChanges = false;

    explicit MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        resize(1024, 768);
        setWindowTitle(tr("Test Engine"));
    }

protected:
    void closeEvent(QCloseEvent* event) override
    {
        if (!unsavedChanges) {
            event->accept();
            return;
        }

        const auto reply = QMessageBox::warning(
            this,
            tr("Unsaved Changes"),
            tr("You have unsaved changes. Do you want to save before closing?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save
        );

        if (reply == QMessageBox::Save) {
            if (saveChanges(this)) {
                event->accept();
            }
            else {
                event->ignore();
            }
        }
        else if (reply == QMessageBox::Discard) {
            event->accept();
        }
        else {
            event->ignore();
        }
    }

private:
    bool saveChanges(MainWindow* window)
    {
        QWidget* top = window->findChild<QWidget*>("ProjectEditorTopWidget");
        Project* currentProject = ProjectManager::getProject(top);

        if (!SaveProject::initSave(currentProject, top)) {
            return false;
        }

        unsavedChanges = false;
        return true;
    }
};