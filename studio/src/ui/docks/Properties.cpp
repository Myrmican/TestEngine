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
#include <QLabel>
#include <project/Project.h>
#include <util/Languages.h>
#include <ui/docks/Properties.h>
#include <ui/docks/Explorer.h>
#include <engine/datamodel/Instance.h>

namespace {

    bool parseValidValue(const QString& value, const QString& expectedType) {
        return false;
    }

    void connectValueEdit(QTreeWidget* treeWidget) {
		QObject::connect(treeWidget, &QTreeWidget::itemClicked, [treeWidget](QTreeWidgetItem* item, int column) {
            if (column != 1 || !item) return;

            auto* existingLabel = qobject_cast<QLabel*>(treeWidget->itemWidget(item, 1));
            if (!existingLabel) return;

            auto* lineEdit = new QLineEdit(existingLabel->text(), treeWidget);
            treeWidget->setItemWidget(item, 1, lineEdit);
            lineEdit->setFocus();
            lineEdit->selectAll();

            auto commitEdit = [treeWidget, item, lineEdit]() {
                QString newValue = lineEdit->text();

                auto* newLabel = new QLabel(newValue);
                treeWidget->setItemWidget(item, 1, newLabel);
                };

            QObject::connect(lineEdit, &QLineEdit::editingFinished, commitEdit);
			});
    }

    void connectSearch(QLineEdit* searchBar, QTreeWidget* propertiesTree) {
        QObject::connect(searchBar, &QLineEdit::textChanged, propertiesTree, [propertiesTree](const QString& text) {
            if (text.isEmpty()) {
                for (int i = 0; i < propertiesTree->topLevelItemCount(); ++i) {
                    propertiesTree->topLevelItem(i)->setHidden(false);
                }
                return;
            }

            for (int i = 0; i < propertiesTree->topLevelItemCount(); ++i) {
                QTreeWidgetItem* item = propertiesTree->topLevelItem(i);
                bool matches = item->text(0).contains(text, Qt::CaseInsensitive);
                item->setHidden(!matches);
            }
            });
    }

    void connectRename(Properties* self) {
        QObject::connect(self->treeWidget, &QTreeWidget::itemDoubleClicked, self, [](QTreeWidgetItem* item, int column) {
            if (column == 1 && (item->flags() & Qt::ItemIsEditable)) {
                item->treeWidget()->editItem(item, column);
            }
            });

        QObject::connect(self->treeWidget, &QTreeWidget::itemChanged,
            [self](QTreeWidgetItem* item, int column) {
                if (column == 1) {
                    QString newName = item->text(column);
                    
                    Engine::Instance* instance = Engine::GetEngineInstance(item);
                    //instance->setName(newName);
                }
            });
    }
}

Properties::Properties(QMainWindow* window, Project* project)
    : QObject(window) {

    auto* propertiesDock = new QDockWidget("Properties", window);
    propertiesDock->setWindowFlags(Qt::SubWindow);
    propertiesDock->setObjectName("PropertiesDock");

    auto* containerWidget = new QWidget(propertiesDock);
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

    auto* propertiesTree = new QTreeWidget(containerWidget);
    propertiesTree->setColumnCount(2);
    propertiesTree->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    propertiesTree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    propertiesTree->setIndentation(12);
    propertiesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    propertiesTree->setHeaderHidden(true);
    propertiesTree->setUniformRowHeights(true);
    propertiesTree->setStyleSheet(
        "QTreeWidget {"
        "    background-color: #161616;"
        "    outline: none;"
        "    border: none;"
        "    show-decoration-selected: 1;"
        "}"
        "QTreeWidget::item {"
        "    color: #cccccc;"
        "    border-bottom: 1px solid #222222;"
        "    border-right: 1px solid #222222;"
        "    height: 22px;"
        "}"
        "QTreeWidget::branch:has-children:closed {"
        "    image: url(:/icons/branch_closed.png);"
        "}"
        "QTreeWidget::branch:has-children:open {"
        "    image: url(:/icons/branch_open.png);"
        "}"
    );

    QPalette palette = propertiesTree->palette();
    palette.setColor(QPalette::Base, QColor(22, 22, 22));
    propertiesTree->setPalette(palette);

    dockWidget = propertiesDock;
    treeWidget = propertiesTree;

    propertiesTree->viewport()->installEventFilter(this);
    propertiesDock->setWidget(containerWidget);

    layout->addWidget(searchBar);
    layout->addWidget(propertiesTree);

    propertiesTree->setMaximumWidth(1310);
    propertiesTree->setMinimumWidth(200);

    QTimer::singleShot(0, window, [window, propertiesDock]() {
        window->resizeDocks({ propertiesDock }, { 400 }, Qt::Horizontal);
        });

    connectSearch(searchBar, propertiesTree);
    connectValueEdit(propertiesTree);
    connectRename(this);
}

QTreeWidgetItem* Properties::GetOrCreateCategory(const QString& categoryName) {
    QList<QTreeWidgetItem*> items = this->treeWidget->findItems(
        categoryName,
        Qt::MatchExactly | Qt::MatchExactly,
        0
    );

    if (!items.isEmpty()) {
        return items.first();
    }

    auto* categoryItem = new QTreeWidgetItem(this->treeWidget);
    categoryItem->setText(0, categoryName);
    categoryItem->setFirstColumnSpanned(true);
    categoryItem->setFont(0, QFont("Segoe UI", 10, QFont::Bold));
    categoryItem->setExpanded(true);

	categoryItem->setBackground(0, QBrush(QColor(102, 102, 102)));

    return categoryItem;
}

void Properties::AddProperty(const QString& category, const QString& property,
        const QString& propertyType, const QString& defaultValue) {

    QTreeWidgetItem* categoryItem = GetOrCreateCategory(category);

    QTreeWidgetItem* testItem = new QTreeWidgetItem();
    testItem->setText(0, "Name");
    testItem->setText(1, "Workspace");
    testItem->setFlags(testItem->flags() | Qt::ItemIsEditable);

    treeWidget->addTopLevelItem(testItem);
}

bool Properties::eventFilter(QObject* watched, QEvent* event) {
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