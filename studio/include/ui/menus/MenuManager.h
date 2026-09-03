class QString;
class QWidget;
class QPoint;
class QMenu;

namespace Menu {
    QMenu* create(QWidget* parent);
    QMenu* create(QWidget* parent, const QString& title);
    QPoint getMenuPosition(QWidget* parent, QWidget* menu);
}