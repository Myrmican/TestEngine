#include <QTreeWidget>
#include <QDockWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTimer>
#include <QMenu>
#include <QInputDialog>
#include <QTabWidget>
#include <QMouseEvent>
#include <QHeaderView>
#include <project/Project.h>
#include <ui/docks/Assets.h>
#include <ui/window/MainWindow.h>

namespace {

    void ConnectSearch(QLineEdit* searchBar, QTreeWidget* AssetsTree) {
        QObject::connect(searchBar, &QLineEdit::textChanged, AssetsTree, [AssetsTree](const QString& text) {
            if (text.isEmpty()) {
                for (int i = 0; i < AssetsTree->topLevelItemCount(); ++i) {
                    AssetsTree->topLevelItem(i)->setHidden(false);
                }
                return;
            }

            for (int i = 0; i < AssetsTree->topLevelItemCount(); ++i) {
                QTreeWidgetItem* item = AssetsTree->topLevelItem(i);
                bool matches = item->text(0).contains(text, Qt::CaseInsensitive);
                item->setHidden(!matches);
            }
            });
    }
}

Assets::Assets(MainWindow* window, Project* project)
    : QDockWidget("Assets", window) {

    setWindowFlags(Qt::SubWindow);
    setObjectName("AssetsDock");

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

    this->setWidget(containerWidget);

    layout->addWidget(searchBar);

    QTimer::singleShot(0, window, [window, this]() {
        window->resizeDocks({ this }, { 400 }, Qt::Horizontal);
        });
}