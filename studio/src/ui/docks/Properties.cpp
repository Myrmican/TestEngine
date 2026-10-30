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
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <project/Project.h>
#include <util/Languages.h>
#include <ui/docks/Properties.h>
#include <ui/docks/Explorer.h>
#include <engine/datamodel/Instance.h>
#include <engine/datamodel/Property.h>
#include <engine/core/Reflection.h>
#include <engine/datamodel/ClassDescriptor.h>
#include <engine/services/selection/Selection.h>
#include <util/CheckType.h>
#include <QCheckBox>
#include <cmath>

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
    : QDockWidget("Properties", window), m_project(project) {

    setWindowFlags(Qt::SubWindow);
    setObjectName("PropertiesDock");

    auto* containerWidget = new QWidget(this);
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
    propertiesTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
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
        "QLineEdit {"
        "   background: transparent;"
        "   border: none;"
        "   padding-left: 2px;"
        "}"
        "QLineEdit:enabled {"
        "   color: #cccccc;"
        "}"
        "QLineEdit:focus {"
        "   background-color: #2a2a2a;"
        "}"

        "QSpinBox {"
        "   background: transparent;"
        "   border: none;"
        "   padding-left: 2px;"
        "}"

        "QDoubleSpinBox {"
        "   background: transparent;"
        //"   border: none;"
        "   padding-left: 2px;"
        "}"
    );

    QPalette palette = propertiesTree->palette();
    palette.setColor(QPalette::Base, QColor(22, 22, 22));
    propertiesTree->setPalette(palette);

    treeWidget = propertiesTree;
    treeWidget->setAttribute(Qt::WA_Hover, false);

    propertiesTree->viewport()->installEventFilter(this);
    this->setWidget(containerWidget);

    layout->addWidget(searchBar);
    layout->addWidget(propertiesTree);

    propertiesTree->setMaximumWidth(1310);
    propertiesTree->setMinimumWidth(200);

    QTimer::singleShot(0, window, [window, this]() {
        window->resizeDocks({ this }, { 400 }, Qt::Horizontal);
        });

    connectSearch(searchBar, propertiesTree);

    Engine::Selection* selectionService = project->getEngine()->getProvider()->getService<Engine::Selection>();

    selectionService->selectionChanged.connect([this](Engine::Instance* instance) {
        if (instance == nullptr) {
            treeWidget->clear();
            return;
        }

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

    Engine::Globals::Variant propertyValue = property->getFormatted(instance);

    bool readOnly = property->readOnly || instance->internalLocked && property->m_name == "Parent";

    setWindowTitle("Properties - " + instance->getClassName());

    std::visit([&](auto&& value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (Engine::CheckType::StringLike<T>) {
            auto* edit = new QLineEdit();
            edit->setText(QString::fromStdString(std::string(value)));
            edit->setDisabled(readOnly);
            edit->setReadOnly(readOnly);
            edit->setObjectName("PropertyValueEdit");
            treeWidget->setItemWidget(propertyItem, 1, edit);
        }
        else if constexpr (std::is_same_v<T, bool>) {
            auto* check = new QCheckBox();
            check->setChecked(value);
            check->setDisabled(readOnly);
            treeWidget->setItemWidget(propertyItem, 1, check);
        }
        else if constexpr (std::is_same_v<T, int>) {
            auto* spin = new QSpinBox();
            spin->setRange(INT_MIN, INT_MAX);
            spin->setValue(value);
            spin->setMinimum(0);
            spin->setEnabled(!readOnly);
            treeWidget->setItemWidget(propertyItem, 1, spin);
        }
        else if constexpr (std::is_same_v<T, double>) {
            auto* spin = new QDoubleSpinBox();
            spin->setRange(0, 1e12);
            spin->setValue(value);
            spin->setEnabled(!readOnly);
            treeWidget->setItemWidget(propertyItem, 1, spin);
        }
        }, propertyValue);
}

void Properties::InspectInstance(Engine::Instance* selectedInstance) {
    treeWidget->clear();
    if (!selectedInstance) return;

    std::string className(selectedInstance->getClassName());
    Engine::ClassDescriptor* desc = Engine::GetClassDescriptor(className);
    if (!desc) return;

    for (const Engine::Property* prop : desc->getAllProperties()) {
        this->AddProperty(selectedInstance, prop);
    }
}

bool Properties::eventFilter(QObject* watched, QEvent* event) {
    if (treeWidget && watched == treeWidget->viewport()) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            auto* item = treeWidget->itemAt(mouseEvent->pos());
            if (!item) {
                treeWidget->clearSelection();
                treeWidget->setCurrentItem(nullptr);
                setWindowTitle("Properties");
            }
        }
    }
    return QObject::eventFilter(watched, event);
}