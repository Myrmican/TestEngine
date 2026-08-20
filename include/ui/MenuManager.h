class QMenu;
class QMenuBar;
class QString;

namespace Menu {
    QMenu* create(QMenuBar* menuBar, const QString& title);
    QMenu* create(QMenu* parentMenu, const QString& title);
}