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
#include <project/CodeEditor.h>
#include <util/Languages.h>
#include <ui/docks/Properties.h>

namespace {

    bool ParseValidValue(const QString& value, const QString& expectedType) {
        return false;
    }

    void ConnectValueEdit(QTreeWidget* treeWidget) {
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

    void ConnectSearch(QLineEdit* searchBar, QTreeWidget* propertiesTree) {
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
    propertiesTree->setHeaderHidden(true);
    propertiesTree->setUniformRowHeights(true);
    propertiesTree->setStyleSheet(
        "QTreeWidget::item {"
        "    background: transparent;"
        "    border: none;"
        "    color: #ffffff;"
        "}"

        "QTreeWidget::item:hover {"
        "    background: transparent;"
        "    border: none;"
        "}"

        "QTreeWidget::item:selected {"
        "    background: transparent;"
        "    border: none;"
        "}"

        "QTreeWidget::item:selected:hover {"
        "    background: transparent;"
        "    border: none;"
        "}"

        "QTreeWidget {"
        "   outline: none;"
        "   border: none;"
        "   background-color: #1a1a1a;"
        "   padding-left: 14px;"
        "}"
        "QTreeWidget QLineEdit {"
        "    background-color: #1a1a1a;"
        "    color: #ffffff;"
        "    padding: 0px;"
        "    margin: 0px;"
        "    selection-background-color: #007acc;"
        "}"
    );

    dockWidget = propertiesDock;
	treeWidget = propertiesTree;

    treeWidget->setIndentation(0);
    treeWidget->setEditTriggers(QAbstractItemView::EditKeyPressed);
    propertiesTree->viewport()->installEventFilter(this);
    propertiesDock->setWidget(containerWidget);

    layout->addWidget(searchBar);
    layout->addWidget(propertiesTree);

    propertiesTree->setMaximumWidth(1310);
    propertiesTree->setMinimumWidth(200);

    QTimer::singleShot(0, window, [window, propertiesDock]() {
        window->resizeDocks({ propertiesDock }, { 400 }, Qt::Horizontal);
        });

    ConnectSearch(searchBar, propertiesTree);
	ConnectValueEdit(propertiesTree);
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

    auto* propertyItem = new QTreeWidgetItem(categoryItem);

	QLabel* valueLabel = new QLabel(defaultValue);
	valueLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    valueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextEditable);
    this->treeWidget->setItemWidget(propertyItem, 1, valueLabel);

    propertyItem->setText(0, property);
    propertyItem->setFlags(propertyItem->flags() | Qt::ItemIsEditable);
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