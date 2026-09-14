#include <ui/popups/InsertObject.h>
#include <engine/core/Reflection.h>
#include <ui/docks/Explorer.h>
#include <datamodel/Instance.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QTreeWidgetItem>
#include <QOBject>

namespace Engine {
	InsertObjectPopup::InsertObjectPopup(QWidget* widgetParent, QTreeWidgetItem* parentItem) : QWidget(widgetParent) {
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_DeleteOnClose);

        setFixedSize(220, 280);

        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(6, 6, 6, 6);

        searchBar = new QLineEdit(this);
        searchBar->setPlaceholderText("Search objects...");
        layout->addWidget(searchBar);

        objectList = new QListWidget(this);
        layout->addWidget(objectList);

        setLayout(layout);

        searchBar->setFocus();

		for (const auto& className : GetCreatableClasses()) {
			objectList->addItem(QString::fromStdString(className));
		}

        QObject::connect(searchBar, &QLineEdit::textChanged, objectList, [this](const QString& text) {
            if (text.isEmpty()) {
                for (int i = 0; i < objectList->count(); ++i) {
                    objectList->item(i)->setHidden(false);
                }
                return;
            }

            for (int i = 0; i < objectList->count(); ++i) {
                QListWidgetItem* item = objectList->item(i);
                bool matches = item->text().contains(text, Qt::CaseInsensitive);
                item->setHidden(!matches);
            }
            });

        QObject::connect(objectList, &QListWidget::itemPressed, this, [this, parentItem](QListWidgetItem* item) {
            if (item) {
                QTreeWidget* treeWidget = parentItem->treeWidget();

                QMainWindow* mainWindow = qobject_cast<QMainWindow*>(treeWidget->window());
                if (!mainWindow) return;

                Explorer* explorer = mainWindow->findChild<Explorer*>();
                if (!explorer) return;

                QString className = item->text();
                std::unique_ptr<Engine::Creatable> newInstance = Engine::CreateInstance(className.toStdString());
                if (!newInstance) return;

                Instance* parentInstance = Engine::GetEngineInstance(parentItem);
                if (!parentInstance && explorer->m_project->dataModel)
                    parentInstance = explorer->m_project->dataModel.get();

                Instance* rawInstance = newInstance.get();

                if (parentInstance) {
                    parentInstance->addChild(std::move(newInstance));
                }

                QTreeWidgetItem* treeItem = explorer->AddItem(parentItem, rawInstance);

                treeWidget->clearSelection();
                treeWidget->setCurrentItem(treeItem);

                //close();
            }
            });
	}
}