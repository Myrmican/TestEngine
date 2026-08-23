#include <QMenu>
#include <QMenuBar>
#include <QStyle>
#include <QStyleFactory>

namespace Menu {
    QMenu* create(QWidget* parent) {
        QMenu* menu = new QMenu(parent);

        QPalette palette = menu->palette();
        palette.setColor(QPalette::Window, QColor(43, 43, 43));
        palette.setColor(QPalette::Base, QColor(30, 30, 30));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Text, QColor(242, 242, 242));
        palette.setColor(QPalette::Highlight, QColor(26, 26, 26));

        menu->setPalette(palette);
        if (QStyle* fusion = QStyleFactory::create("Fusion")) {
            fusion->setParent(menu);
            menu->setStyle(fusion);
        }

        return menu;
    }

    QMenu* create(QWidget* parent, const QString& title) {
        QMenu* menu = create(parent);
        menu->setTitle(title);
        return menu;
    }
}