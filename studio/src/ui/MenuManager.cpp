#include <QMenu>
#include <QWidget>
#include <QMenuBar>
#include <QStyleFactory>

namespace Menu {
	QMenu* create(QMenu* menuBar, const QString& title) {
		QMenu* menu = menuBar->addMenu(title);

        QPalette palette = menu->palette();

        palette.setColor(QPalette::Window, QColor(43, 43, 43));
        palette.setColor(QPalette::Base, QColor(30, 30, 30));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Text, QColor(242, 242, 242));
        palette.setColor(QPalette::Highlight, QColor(26, 26, 26));

        menu->setPalette(palette);
        menu->setStyle(QStyleFactory::create("Fusion"));

        return menu;
	}

    QMenu* create(QMenuBar* menuBar, const QString& title) {
        QMenu* menu = menuBar->addMenu(title);

        QPalette palette = menu->palette();

        palette.setColor(QPalette::Window, QColor(43, 43, 43));
        palette.setColor(QPalette::Base, QColor(30, 30, 30));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Text, QColor(242, 242, 242));
        palette.setColor(QPalette::Highlight, QColor(26, 26, 26));

        menu->setPalette(palette);
        menu->setStyle(QStyleFactory::create("Fusion"));

        return menu;
    }
}