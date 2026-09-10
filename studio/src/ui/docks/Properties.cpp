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
#include <engine/datamodel/Property.h>
#include <engine/core/Reflection.h>
#include <engine/datamodel/ClassDescriptor.h>
#include <engine/services/selection/Selection.h>

namespace {

    bool parseValidValue(const QString& value, const QString& expectedType) {
        return false;
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
}

Properties::Properties(QMainWindow* window, Project* project)
    : QObject(window), m_project(project) {

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
        "    show-decoration-selected: 0;"
        "}"
        "QTreeWidget::item {"
        "    color: #cccccc;"
        "    border-bottom: 1px solid #222222;"
        "    border-right: 1px solid #222222;"
        "    height: 22px;"
        "}"
        "QTreeWidget::item:hover,"
        "QTreeWidget::item:selected,"
        "QTreeWidget::item:selected:hover,"
        "QTreeWidget::branch:hover,"
        "QTreeWidget::branch:selected {"
        "    background-color: #161616;"
        "}"
        "QTreeWidget QLineEdit {"
        "    background: transparent;"
        "    border: none;"
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
    treeWidget->setAttribute(Qt::WA_Hover, false);

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

    auto services = project->dataModel->m_services;

    std::shared_ptr<Engine::Selection> selectionService = nullptr;

    for (const auto& [name, instance] : services) {
        if (name == "Selection") {
            selectionService = std::dynamic_pointer_cast<Engine::Selection>(instance);
            break;
        }
    }

    selectionService->selectionChanged.connect([this](Engine::Instance* instance) {
        this->InspectInstance(instance);
        });
}

QTreeWidgetItem* Properties::GetOrCreateCategory(const std::string& categoryName) {
    QString qname = QString::fromStdString(categoryName);

    QList<QTreeWidgetItem*> items = this->treeWidget->findItems(
        qname,
        Qt::MatchExactly | Qt::MatchExactly,
        0
    );

    if (!items.isEmpty()) {
        return items.first();
    }

    auto* categoryItem = new QTreeWidgetItem(this->treeWidget);
    categoryItem->setText(0, qname);
    categoryItem->setFirstColumnSpanned(true);
    categoryItem->setFont(0, QFont("Segoe UI", 10, QFont::Bold));
    categoryItem->setExpanded(true);

	//categoryItem->setBackground(0, QBrush(QColor(102, 102, 102)));

    return categoryItem;
}

void Properties::AddProperty(Engine::Instance* instance, const Engine::Property* property) {
    if (!instance || !property) return;

    QTreeWidgetItem* categoryItem = GetOrCreateCategory(property->m_category);

    QTreeWidgetItem* propertyItem = new QTreeWidgetItem(categoryItem);
    propertyItem->setText(0, QString::fromStdString(property->m_name));

    std::any rawValue = property->getValue(instance);
    QString initialText = "";

    if (rawValue.type() == typeid(std::string)) {
        initialText = QString::fromStdString(std::any_cast<std::string>(rawValue));
    }
    else if (rawValue.type() == typeid(std::string_view)) {
        auto sv = std::any_cast<std::string_view>(rawValue);
        initialText = QString::fromUtf8(sv.data(), static_cast<qsizetype>(sv.size()));
    }

    auto* valueEdit = new QLineEdit();
    valueEdit->setText(initialText);
    valueEdit->setStyleSheet(
        "QLineEdit {"
        "   background: transparent;"
        "   border: none;"
        "   color: #cccccc;"
        "   padding-left: 2px;"
        "}"
        "QLineEdit:focus {"
        "   background-color: #2a2a2a;"
        "}"
    );

    instance->changed.connect([this, instance, valueEdit](std::string name, std::any oldValue) {
        valueEdit->setText(QString::fromStdString(std::string(instance->getName())));
        });

    treeWidget->setItemWidget(propertyItem, 1, valueEdit);
}

void Properties::InspectInstance(Engine::Instance* selectedInstance) {
    treeWidget->clear();
    if (!selectedInstance) return;

    Engine::ClassDescriptor* desc = Engine::GetClassDescriptor(std::string(selectedInstance->getClassName()));

    for (const Engine::Property* prop : desc->getAllProperties()) {
        this->AddProperty(selectedInstance, prop);
    }
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