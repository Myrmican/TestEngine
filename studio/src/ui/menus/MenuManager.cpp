#include <QMenu>
#include <QMenuBar>
#include <QStyle>
#include <QStyleFactory>
#include <QIcon>
#include <QGuiApplication>
#include <QScreen>

namespace Menu {
    QMenu* create(QWidget* parent) {
        QMenu* menu = new QMenu(parent);

        QPalette palette = menu->palette();
        palette.setColor(QPalette::Window, QColor(43, 43, 43));
        palette.setColor(QPalette::Base, QColor(30, 30, 30));
        palette.setColor(QPalette::WindowText, QColor(242, 242, 242));
        palette.setColor(QPalette::Text, QColor(242, 242, 242));
        palette.setColor(QPalette::Highlight, QColor(26, 26, 26));
        menu->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        menu->setAttribute(Qt::WA_TranslucentBackground);

        menu->setPalette(palette);

        menu->setStyleSheet(
            "QMenu { border-radius: 6px; }"
            "QMenu::item { border: none; background: transparent; }"
            "QMenu::seperator { background-color: #666666; }"
            "QMenu::item:selected { border: none; background-color: #1a1a1a; }"
        );

        return menu;
    }

    QMenu* create(QWidget* parent, const QString& title) {
        QMenu* menu = create(parent);
        menu->setTitle(title);
        return menu;
    }

    QMenu* create(QWidget* parent, const QIcon& icon) {
        QMenu* menu = create(parent);
        menu->setIcon(icon);
        return menu;
    }

	QPoint getMenuPosition(QWidget* parent, QWidget* menu) {
        QPoint globalPos = QCursor::pos();

        QScreen* screen = QGuiApplication::screenAt(globalPos);
        if (!screen) {
            screen = QGuiApplication::primaryScreen();
        }

        QRect screenGeometry = screen->availableGeometry();
        QSize menuSize = menu->sizeHint();

        QPoint targetPos = globalPos;

        if (globalPos.x() + menuSize.width() > screenGeometry.right()) {
            targetPos.setX(globalPos.x() - menuSize.width());
        }

        if (globalPos.y() + menuSize.height() > screenGeometry.bottom()) {
            targetPos.setY(globalPos.y() - menuSize.height());
        }

        return targetPos;
	}
}