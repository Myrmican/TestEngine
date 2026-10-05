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
#include <editor/file/CodeEditor.h>
#include <util/Languages.h>
#include <ui/menus/MenuManager.h>
#include <ui/docks/Explorer.h>
#include <ui/popups/InsertObject.h>
#include <engine/datamodel/instances/File.h>
#include <QString>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QProcessEnvironment>
#include <memory>
#include <engine/services/selection/Selection.h>
#include <engine/core/Reflection.h>
#include <editor/file/FileManager.h>
#include <core/InstanceHandler.h>
#include <QGuiApplication>
#include <QClipboard>

using namespace Engine;

constexpr int InstancePointerRole = Qt::UserRole + 1;

namespace {

    void ConnectSearch(QLineEdit* searchBar, QTreeWidget* explorerTree) {
        QObject::connect(searchBar, &QLineEdit::textChanged, explorerTree, [explorerTree](const QString& text) {
            QTreeWidgetItemIterator it(explorerTree);
            if (text.isEmpty()) {
                while (*it) {
                    explorerTree->collapseAll();
                    (*it)->setHidden(false);
                    ++it;
                }
                return;
            }

            while (*it) {
                bool matches = (*it)->text(0).contains(text, Qt::CaseInsensitive);
                explorerTree->scrollToItem((*it));

                (*it)->setHidden(!matches);
                ++it;
            }
            });
    }

    void ConnectContextMenu(QTreeWidget* explorerTree, QMainWindow* window, Project* project, Explorer* self) {
        QTabWidget* documentTabs = window->findChild<QTabWidget*>("DocumentTabs");

        QObject::connect(explorerTree, &QTreeWidget::customContextMenuRequested,
            [explorerTree, window, project, self](const QPoint& pos) {
                QTreeWidgetItem* item = explorerTree->itemAt(pos);
                Instance* instance = Engine::GetEngineInstance(item);

                if (!item) {
                    return;
                }

                bool filesAllowed = false;
				QTreeWidgetItem* currentIteratedItem = item;

				while (currentIteratedItem) {
                    Instance* iteratedInstance = Engine::GetEngineInstance(currentIteratedItem);
					if (iteratedInstance->getClassName() == "SourceFolder") {
						filesAllowed = true;
						break;
					}
                    
					currentIteratedItem = currentIteratedItem->parent();
				}

                QMenu* contextMenu = Menu::create(window);

                QAction* copyPathAction = contextMenu->addAction("Copy as path");
                contextMenu->addSeparator();

                QAction* openAction = nullptr;
                QMenu* openWithMenu = nullptr;
                QAction* openWithCode = nullptr;

                if (filesAllowed) {
					openAction = contextMenu->addAction("Open");
					openWithMenu = Menu::create(window, "Open With");
					contextMenu->addMenu(openWithMenu);

					openWithCode = openWithMenu->addAction("Default Editor");

                    contextMenu->addSeparator();
                }

                if (!instance->internalLocked) {
                    QAction* cutAction = contextMenu->addAction("Cut");
                    QAction* copyAction = contextMenu->addAction("Copy");
                    QAction* pasteAction = contextMenu->addAction("Paste");

                    contextMenu->addSeparator();

                    QAction* duplicateAction = contextMenu->addAction("Duplicate");
                    QAction* deleteAction = contextMenu->addAction("Delete");
                }

                QAction* renameAction = contextMenu->addAction("Rename");

                contextMenu->addSeparator();

                QAction* addInstanceAction = nullptr;
				if (filesAllowed) {
                    addInstanceAction = contextMenu->addAction("Add File");
                }
                else {
                    addInstanceAction = contextMenu->addAction("Add Instance");
                }

                addInstanceAction->setShortcut(QKeySequence("Ctrl+I"));
                addInstanceAction->setShortcutContext(Qt::WindowShortcut);

                QAction* selectedAction = contextMenu->exec(explorerTree->viewport()->mapToGlobal(pos));
                if (selectedAction == nullptr) return;

                QString actionText = selectedAction->text();

				if (selectedAction == copyPathAction) {
					QString path = QString::fromStdString(instance->getPath());
					QClipboard* clipboard = QGuiApplication::clipboard();
					clipboard->setText(path);
				}
				else

                if (selectedAction == openWithCode) {
                    
                }
                else if (actionText == "Cut") {
                    Instance* instance = GetEngineInstance(item);
                    if (instance) {
                        instance->destroy();
                        delete item;
                    }
                }
                else if (actionText == "Copy") {
                    
                }
                else if (actionText == "Paste") {
                    
                }
                else if (actionText == "Delete") {
					Instance* instance = GetEngineInstance(item);
					if (instance) {
						instance->destroy();
						delete item;
					}
                }
                else if (selectedAction == renameAction) {
                    explorerTree->editItem(item, 0);
                }
                else if (selectedAction == addInstanceAction) {
                    if (!filesAllowed) {
                        auto* popup = new Engine::InsertObjectPopup(window, item);
                        QPoint globalPos = Menu::getMenuPosition(window, popup);
                        popup->move(globalPos);
                        popup->show();
                    }
                    else {
                        QStringList langExtensions = getExtensionsForLanguage(project->primaryLanguage);
                        QString finalExtension = langExtensions.isEmpty() ? ".txt" : langExtensions.first();

                        bool ok;
                        QString fileName = QInputDialog::getText(
                            window,
                            "Create New File",
                            "File Name:",
                            QLineEdit::Normal,
                            finalExtension,
                            &ok
                        );

                        if (ok && !fileName.isEmpty()) {
                            InsertInstanceSet insertResult = Engine::insertInstance("File", item, window);
                            if (!insertResult.instance) {
                                qDebug() << "Failed to create File instance.";
                                return;
                            }

                            std::string nameStr = fileName.toStdString();
                            insertResult.instance->setName(nameStr);

                            explorerTree->clearSelection();
                            explorerTree->setCurrentItem(insertResult.item);
                        }
                    }
                }
            }
        );
    }

    void SelectionChanged(Explorer* self) {
        Project* project = self->m_project;
        Selection* selectionService = project->engine->getProvider()->getService<Selection>();

        QObject::connect(self->treeWidget, &QTreeWidget::currentItemChanged, self->treeWidget, [self, selectionService](QTreeWidgetItem* current, QTreeWidgetItem* previous) {
            if (!selectionService) return;

            if (previous) {
                Instance* previousInstance = Engine::GetEngineInstance(previous);
                if (previousInstance) {
                    selectionService->deselect(previousInstance);
                }
            }

            if (!current) return;

            Instance* instance = Engine::GetEngineInstance(current);
            if (instance) {
                selectionService->select(instance);
            }
            });
    }
}

namespace Engine {
    Instance* GetEngineInstance(QTreeWidgetItem* item) {
        if (!item) return nullptr;

        QVariant data = item->data(0, InstancePointerRole);
        return static_cast<Instance*>(data.value<void*>());
    }

    QTreeWidgetItem* GetEngineInstance(QTreeWidget* treeWidget, Instance* instance) {
        if (!treeWidget || !instance) return nullptr;

        QTreeWidgetItemIterator it(treeWidget);
        while (*it) {
            QVariant data = (*it)->data(0, InstancePointerRole);
            if (static_cast<Instance*>(data.value<void*>()) == instance) {
                return *it;
            }
            ++it;
        }
        return nullptr;
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
    explorerTree->setDragEnabled(true);
	explorerTree->setAcceptDrops(true);
	explorerTree->setDropIndicatorShown(true);
    explorerTree->setStyleSheet(
        "QTreeView {"
        "    outline: 0;"
        "    border-radius: 0px;"
        "    show-decoration-selected: 1;"
        "}"
        "QTreeView::item {"
        "    font-size: 14px;"
        "    color: #f2f2f2;"
        "    padding: 2px 0px;"
        "    margin: 0px;"
        "}"
        "QTreeView::item:hover, QTreeView::branch:hover {"
        "    background-color: #2a2a2a;"
        "}"
        "QTreeView::item:selected, QTreeView::branch:selected {"
        "    background-color: #383838;"
        "}"
        "QTreeView::branch:has-children:!has-siblings:closed,"
        "QTreeView::branch:closed:has-children:has-siblings {"
        "    image: url(:/assets/icons/chevron-right.svg);"
        "}"
        "QTreeView::branch:open:has-children:!has-siblings,"
        "QTreeView::branch:open:has-children:has-siblings {"
        "    image: url(:/assets/icons/chevron-down.png);"
        "}"
        "QTreeWidget QLineEdit {"
        "    border: none;"
        "    outline: none;"
        "    selection-background: transparent;"
        "}"
    );

    QPalette palette = explorerTree->palette();
    palette.setColor(QPalette::Base, QColor(22, 22, 22));
    explorerTree->setPalette(palette);

    dockWidget = explorerDock;
    treeWidget = explorerTree;

    explorerTree->viewport()->installEventFilter(this);
    explorerTree->viewport()->installEventFilter(explorerTree);
    explorerTree->setContextMenuPolicy(Qt::CustomContextMenu);
    explorerTree->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
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
    SelectionChanged(this);
}

QTreeWidgetItem* Explorer::AddItem(QTreeWidgetItem* parentItem, Instance* instance) {
    auto* desc = Engine::GetClassDescriptor(std::string(instance->getClassName()));
    if (desc && !desc->isEditorVisible) return nullptr;

    Instance* parentInstance = GetEngineInstance(parentItem);
    QString instanceName = QString::fromStdString(std::string(instance->getName()));

    if (!parentInstance && m_project->engine->getDataModel()) {
        parentInstance = m_project->engine->getDataModel();
    }

    if (!parentInstance) return nullptr;

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, instanceName);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    if (parentItem) {
        parentItem->addChild(item);
    }
    else {
        treeWidget->addTopLevelItem(item);
    }

    item->setData(0, InstancePointerRole, QVariant::fromValue(static_cast<void*>(instance)));

    instance->changed.connect([item, instance](std::string name, std::any oldValue) {
        item->setText(0, QString::fromStdString(std::string(instance->getName())));
        });

    return item;
}

void Explorer::AssembleRoot() {

    for (auto& service : m_project->engine->getDataModel()->getChildren()) {
        AddItem(nullptr, service.get());

        const auto& children = service->getChildren();
        if (children.empty() || !children.front()) continue;

        for (const auto& child : children) {
            if (child) {
				AddItem(GetEngineInstance(treeWidget, service.get()), child.get());

				const auto& childChildren = child->getChildren();
                if (childChildren.empty() || !childChildren.front()) continue;

                for (const auto& child2 : childChildren) {
                    if (child2) {
                        AddItem(GetEngineInstance(treeWidget, child.get()), child2.get());
                    }
                }
            }
        }
    }
}

bool Explorer::eventFilter(QObject* watched, QEvent* event) {
    if (treeWidget && watched == treeWidget->viewport()) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (event->type() == QEvent::MouseButtonPress) {
            QTreeWidgetItem* item = treeWidget->itemAt(mouseEvent->pos());

            if (!item) {
                treeWidget->clearSelection();
                treeWidget->setCurrentItem(nullptr);
            }
        }
        else {
            if (event->type() == QEvent::MouseButtonDblClick) {
                QTreeWidgetItem* item = treeWidget->itemAt(mouseEvent->pos());
                Instance* instance = Engine::GetEngineInstance(item);
                if (!instance) return false;

                if (instance->getClassName() == "File") {
                    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(treeWidget->window());
                    QTabWidget* documentTabs = mainWindow->findChild<QTabWidget*>("DocumentTabs");

                    FileManager::openFile(instance, documentTabs);
                }
            }
        }
           

    }
    return QObject::eventFilter(watched, event);
}