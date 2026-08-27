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
#include <string>
#include <project/Project.h>
#include <editor/CodeEditor.h>
#include <util/Languages.h>
#include <ui/menus/MenuManager.h>
#include <ui/docks/Explorer.h>
#include <engine/datamodel/instances/File.h>

using namespace Engine;

constexpr int InstancePointerRole = Qt::UserRole + 1;

namespace {
    Instance* GetEngineInstance(QTreeWidgetItem* item) {
        if (!item) return nullptr;
        QVariant data = item->data(0, InstancePointerRole);
        return static_cast<Instance*>(data.value<void*>());
    }

    void ConnectSearch(QLineEdit* searchBar, QTreeWidget* explorerTree) {
        QObject::connect(searchBar, &QLineEdit::textChanged, explorerTree, [explorerTree](const QString& text) {
            if (text.isEmpty()) {
                for (int i = 0; i < explorerTree->topLevelItemCount(); ++i) {
                    explorerTree->topLevelItem(i)->setHidden(false);
                }
                return;
            }

            for (int i = 0; i < explorerTree->topLevelItemCount(); ++i) {
                QTreeWidgetItem* item = explorerTree->topLevelItem(i);
                bool matches = item->text(0).contains(text, Qt::CaseInsensitive);
                item->setHidden(!matches);
            }
            });
    }

    void ConnectContextMenu(QTreeWidget* explorerTree, QMainWindow* window, Project* project, Explorer* self) {
        QTabWidget* documentTabs = window->findChild<QTabWidget*>("DocumentTabs");

        QObject::connect(explorerTree, &QTreeWidget::customContextMenuRequested,
            [explorerTree, window, project, self](const QPoint& pos) {
                QTreeWidgetItem* item = explorerTree->itemAt(pos);

                if (!item) {
                    return;
                }

                bool filesAllowed = false;
				QTreeWidgetItem* currentIteratedItem = item;

				while (currentIteratedItem) {
					if (currentIteratedItem->text(0) == "Src") {
						filesAllowed = true;
						break;
					}
					currentIteratedItem = currentIteratedItem->parent();
				}

                QMenu* contextMenu = Menu::create(explorerTree);

                QAction* openAction = nullptr;
                QMenu* openWithMenu = nullptr;

                if (filesAllowed) {
					openAction = contextMenu->addAction("Open");
					openWithMenu = contextMenu->addMenu("Open With");

					openWithMenu->addAction("Default Editor");

                    contextMenu->addSeparator();
                }

                QAction* cutAction = contextMenu->addAction("Cut");
                QAction* copyAction = contextMenu->addAction("Copy");
                QAction* pasteAction = contextMenu->addAction("Paste");

                contextMenu->addSeparator();

                QAction* duplicateAction = contextMenu->addAction("Duplicate");
                QAction* deleteAction = contextMenu->addAction("Delete");
                QAction* renameAction = contextMenu->addAction("Rename");

                contextMenu->addSeparator();

                QAction* addFileAction = nullptr;
				if (filesAllowed) {
					addFileAction = contextMenu->addAction("Add File");
				}

                QAction* addInstanceAction = contextMenu->addAction("Add Instance");
                addInstanceAction->setShortcut(QKeySequence("Ctrl+I"));
                addInstanceAction->setShortcutContext(Qt::WindowShortcut);

                QAction* selectedAction = contextMenu->exec(explorerTree->viewport()->mapToGlobal(pos));

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
                    explorerTree->editItem(item, 0);
                }
                else if (filesAllowed && selectedAction == addFileAction) {
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

                    if (ok && !fileName.isEmpty()) {
                        File* file = new File();
                        file->setName(fileName.toStdString());
                        QTreeWidgetItem* insertedItem = self->AddItem(item, file);
                        insertedItem->setSelected(true);
                        self->treeWidget->scrollToItem(insertedItem);
                    }
                }
                else if (selectedAction == addInstanceAction) {
                    qDebug() << "Add Instance requested for item:" << item->text(0);
                }
            }
        );
    }
}

Explorer::Explorer(QMainWindow* window, Project* project)
    : QObject(window), m_project(project) {

    auto* explorerDock = new QDockWidget("Explorer", window);
    explorerDock->setWindowFlags(Qt::SubWindow);
    explorerDock->setObjectName("ExplorerDock");

    auto* containerWidget = new QWidget(explorerDock);
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

    auto* explorerTree = new QTreeWidget(containerWidget);
    explorerTree->setHeaderHidden(true);
    explorerTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    explorerTree->setUniformRowHeights(true);
    explorerTree->setStyleSheet(
        "QTreeWidget { outline: 0; border-radius: 0px; }"
        "QTreeView { show-decoration-selected: 0; }"
        "QTreeView::item {"
        "    font-size: 14px;"
        "    color: #f2f2f2;"
        "    padding: 2px 0px;"
        "    margin: 0px;"
        "}"
    );

    dockWidget = explorerDock;
    treeWidget = explorerTree;

    explorerTree->viewport()->installEventFilter(this);
    explorerTree->viewport()->installEventFilter(explorerTree);
    explorerTree->setContextMenuPolicy(Qt::CustomContextMenu);
    explorerTree->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
    explorerDock->setWidget(containerWidget);

    layout->addWidget(searchBar);
    layout->addWidget(explorerTree);

    explorerTree->setMaximumWidth(1310);
    explorerTree->setMinimumWidth(200);

    QTimer::singleShot(0, window, [window, explorerDock]() {
        window->resizeDocks({ explorerDock }, { 400 }, Qt::Horizontal);
        });

    ConnectSearch(searchBar, explorerTree);
    ConnectContextMenu(explorerTree, window, project, this);
    AssembleRoot();
}

QTreeWidgetItem* Explorer::AddItem(QTreeWidgetItem* parentItem, Instance* instance) {
    Instance* parentInstance = GetEngineInstance(parentItem);
    QString instanceName = QString::fromStdString(instance->getName());

    if (!parentInstance) {
        parentInstance = m_project->dataModel.get();
    }

    if (!parentInstance) return nullptr;

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, instanceName);

    if (parentItem) {
        parentItem->addChild(item);
    }
    else {
        treeWidget->addTopLevelItem(item);
    }

    item->setData(0, InstancePointerRole, QVariant::fromValue(static_cast<void*>(instance)));

	if (instance->getClassName() == "File") {
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        QMainWindow* mainWindow = qobject_cast<QMainWindow*>(treeWidget->window());
        QTabWidget* documentTabs = mainWindow->findChild<QTabWidget*>("DocumentTabs");

        CodeEditor* codeEditor = new CodeEditor(documentTabs);

        int newTabIndex = documentTabs->addTab(codeEditor->editor, instanceName);
        documentTabs->setCurrentIndex(newTabIndex);
	}

    return item;
}

void Explorer::AssembleRoot() {
    auto it = m_project->dataModel->m_services.find("World");
    Engine::Instance* worldPtr = (it != m_project->dataModel->m_services.end()) ? it->second.get() : nullptr;

    AddItem(nullptr, worldPtr);

    auto it2 = m_project->dataModel->m_services.find("Players");
    Engine::Instance* playersPtr = (it2 != m_project->dataModel->m_services.end()) ? it2->second.get() : nullptr;

    AddItem(nullptr, playersPtr);

    auto it3 = m_project->dataModel->m_services.find("Client");
    Engine::Instance* clientPtr = (it3 != m_project->dataModel->m_services.end()) ? it3->second.get() : nullptr;

    AddItem(nullptr, clientPtr);

    auto it4 = m_project->dataModel->m_services.find("Shared");
    Engine::Instance* sharedPtr = (it4 != m_project->dataModel->m_services.end()) ? it4->second.get() : nullptr;

    AddItem(nullptr, sharedPtr);

    auto it5 = m_project->dataModel->m_services.find("Server");
    Engine::Instance* serverPtr = (it5 != m_project->dataModel->m_services.end()) ? it5->second.get() : nullptr;

    AddItem(nullptr, serverPtr);
}

bool Explorer::eventFilter(QObject* watched, QEvent* event) {
    if (treeWidget && watched == treeWidget->viewport()) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);

            if (!treeWidget->itemAt(mouseEvent->pos())) {
                treeWidget->clearSelection();
                treeWidget->setCurrentItem(nullptr);
            }
        }
    }
    return QObject::eventFilter(watched, event);
}