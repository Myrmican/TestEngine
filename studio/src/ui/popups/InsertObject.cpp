#include <ui/popups/InsertObject.h>
#include <engine/core/Reflection.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QTreeWidgetItem>
#include <QOBject>

namespace Engine {
	InsertObjectPopup::InsertObjectPopup(QWidget* parent, QTreeWidgetItem* item) : QWidget(parent) {
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

		for (const auto& className : GetCreateableClasses()) {
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
	}
}