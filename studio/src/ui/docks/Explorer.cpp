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
#include <editor/CodeEditor.h>
#include <util/Languages.h>
#include <ui/menus/MenuManager.h>
#include <ui/docks/Explorer.h>

namespace {
    struct NodeSpec {
        QString name;
        std::vector<NodeSpec> children = {};
        Qt::ItemFlags flagsToEnable = Qt::ItemIsEditable;
    };

    void AssembleExplorerRoot(QTreeWidget* explorerTree) {
        explorerTree->clear();

        const std::vector<NodeSpec> treeStructure = {
            { "World", { { "Camera" } } },
            { "Players" },
            { "Lighting" },
            { "Client", { { "Assets" }, { "Src" } } },
            { "Shared", { { "Assets" }, { "Src" } } },
            { "Server", { { "Assets" }, { "Src" } } }
        };

        auto populateTree = [](auto& self, QTreeWidgetItem* parentItem, QTreeWidget* parentWidget, const std::vector<NodeSpec>& nodes) -> void {
            for (const auto& node : nodes) {
                QTreeWidgetItem* item = nullptr;

                if (parentWidget) {
                    item = new QTreeWidgetItem(parentWidget, QStringList() << node.name);
                }
                else if (parentItem) {
                    item = new QTreeWidgetItem();
                    item->setText(0, node.name);
                    parentItem->addChild(item);
                }

                if (item) {
                    item->setFlags(item->flags() | node.flagsToEnable);
                    if (!node.children.empty()) {
                        self(self, item, nullptr, node.children);
                    }
                }
            }
            };

        const bool prevSignals = explorerTree->blockSignals(true);
        populateTree(populateTree, nullptr, explorerTree, treeStructure);
        explorerTree->blockSignals(prevSignals);
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

                bool FilesAllowed = false;
				QTreeWidgetItem* currentIteratedItem = item;

				while (currentIteratedItem) {
					if (currentIteratedItem->text(0) == "Src") {
						FilesAllowed = true;
						break;
					}
					currentIteratedItem = currentIteratedItem->parent();
				}

                QMenu* contextMenu = Menu::create(explorerTree);

                QAction* openAction = nullptr;
                QMenu* openWithMenu = nullptr;

                if (FilesAllowed) {
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
				if (FilesAllowed) {
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

                    if (ok && !fileName.isEmpty()) {
                        self->AddItem(item, fileName, "File");
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
    : QObject(window) {

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

	AssembleExplorerRoot(explorerTree);

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
}

void Explorer::AddItem(QTreeWidgetItem* parentItem, const QString& itemName, const std::string className) {
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(treeWidget->window());

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, itemName);
    parentItem->addChild(item);

    item->setSelected(true);
    treeWidget->scrollToItem(item);

	if (className == "File") {
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        QTabWidget* documentTabs = mainWindow->findChild<QTabWidget*>("DocumentTabs");

        auto* codeEditor = CodeEditor::createEditor(documentTabs);

        int newTabIndex = documentTabs->addTab(codeEditor, itemName);
        documentTabs->setCurrentIndex(newTabIndex);
	}
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