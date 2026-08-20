#include <QTreeWidget>
#include <QDockWidget>
#include <QLineEdit>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QTimer>
#include <QMenu>
#include <QInputDialog>
#include <QTabWidget>
#include <QMouseEvent>
#include <QHeaderView>
#include <project/Project.h>
#include <project/CodeEditor.h>
#include <util/Languages.h>
#include <ui/docks/Toolbox.h>

namespace {

    void ConnectSearch(QLineEdit* searchBar, QTreeWidget* ToolboxTree) {
        QObject::connect(searchBar, &QLineEdit::textChanged, ToolboxTree, [ToolboxTree](const QString& text) {
            if (text.isEmpty()) {
                for (int i = 0; i < ToolboxTree->topLevelItemCount(); ++i) {
                    ToolboxTree->topLevelItem(i)->setHidden(false);
                }
                return;
            }

            for (int i = 0; i < ToolboxTree->topLevelItemCount(); ++i) {
                QTreeWidgetItem* item = ToolboxTree->topLevelItem(i);
                bool matches = item->text(0).contains(text, Qt::CaseInsensitive);
                item->setHidden(!matches);
            }
            });
    }

    void ConnectContextMenu(QTreeWidget* ToolboxTree, QMainWindow* window, Project* project, Toolbox* self) {
        QTabWidget* documentTabs = window->findChild<QTabWidget*>("DocumentTabs");

        QObject::connect(ToolboxTree, &QTreeWidget::customContextMenuRequested,
            [ToolboxTree, window, project, self](const QPoint& pos) {
                QTreeWidgetItem* item = ToolboxTree->itemAt(pos);

                if (!item) {
                    return;
                }

                bool FilesAllowed = false;
				QTreeWidgetItem* currentIteratedItem = item;

				while (currentIteratedItem) {
					if (currentIteratedItem->text(0) == "Src") {
						FilesAllowed = true;
						break;
					}
					currentIteratedItem = currentIteratedItem->parent();
				}

                QMenu contextMenu(ToolboxTree);

                QAction* openAction = nullptr;
                QMenu* openWithMenu = nullptr;

                if (FilesAllowed) {
					openAction = contextMenu.addAction("Open");
					openWithMenu = contextMenu.addMenu("Open With");

					openWithMenu->addAction("Default Editor");

                    contextMenu.addSeparator();
                }

                QAction* cutAction = contextMenu.addAction("Cut");
                QAction* copyAction = contextMenu.addAction("Copy");
                QAction* pasteAction = contextMenu.addAction("Paste");

                contextMenu.addSeparator();

                QAction* duplicateAction = contextMenu.addAction("Duplicate");
                QAction* deleteAction = contextMenu.addAction("Delete");
                QAction* renameAction = contextMenu.addAction("Rename");

                contextMenu.addSeparator();

                QAction* addFileAction = nullptr;
				if (FilesAllowed) {
					addFileAction = contextMenu.addAction("Add File");
				}

                QAction* addInstanceAction = contextMenu.addAction("Add Instance");

                QAction* selectedAction = contextMenu.exec(ToolboxTree->viewport()->mapToGlobal(pos));

                if (selectedAction == cutAction) {
                    qDebug() << "Cut requested for item:" << item->text(0);
                }
                else if (selectedAction == copyAction) {
                    qDebug() << "Copy requested for item:" << item->text(0);
                }
                else if (selectedAction == pasteAction) {
                    qDebug() << "Paste requested for item:" << item->text(0);
                }
                else if (selectedAction == deleteAction) {
                    qDebug() << "Delete requested for item:" << item->text(0);
                }
                else if (selectedAction == renameAction) {
                    ToolboxTree->editItem(item, 0);
                }
                else if (FilesAllowed && selectedAction == addFileAction) {
                    QStringList langExtensions = getExtensionsForLanguage(project->primaryLanguage);
                    QString finalExtension = langExtensions.isEmpty() ? ".txt" : langExtensions.first();

                    bool ok;
                    QString fileName = QInputDialog::getText(
                        window,
                        "Create New File",
                        "File Name:",
                        QLineEdit::Normal,
                        "" + finalExtension,
                        &ok
                    );
                }
                else if (selectedAction == addInstanceAction) {
                    qDebug() << "Add Instance requested for item:" << item->text(0);
                }
            }
        );
    }
}

Toolbox::Toolbox(QMainWindow* window, Project* project)
    : QObject(window) {

    auto* toolboxDock = new QDockWidget("Toolbox", window);
    toolboxDock->setWindowFlags(Qt::SubWindow);
    toolboxDock->setObjectName("ToolboxDock");

    auto* containerWidget = new QWidget(toolboxDock);
    auto* layout = new QVBoxLayout(containerWidget);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);

    auto* searchBar = new QLineEdit(containerWidget);
    searchBar->setPlaceholderText("Search...");
    searchBar->setClearButtonEnabled(true);
    searchBar->setStyleSheet(
        "QLineEdit {"
        "    padding-top: 1px;"
        "    padding-bottom: 1px;"
        "    padding-left: 4px;"
        "    padding-right: 4px;"
        "    border: 0px;"
        "    border-radius: 3px;"
        "    background-color: #1e1e1e;"
        "    color: #f2f2f2;"
        "}"
    );

    dockWidget = toolboxDock;
    toolboxDock->setWidget(containerWidget);

    layout->addWidget(searchBar);

    

    QTimer::singleShot(0, window, [window, toolboxDock]() {
        window->resizeDocks({ toolboxDock }, { 400 }, Qt::Horizontal);
        });
}

bool Toolbox::eventFilter(QObject* watched, QEvent* event) {
    
    return QObject::eventFilter(watched, event);
}