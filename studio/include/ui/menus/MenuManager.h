#include <QMenu>

class QString;
class QWidget;
class QPoint;
class QIcon;

namespace Menu {
    QMenu* create(QWidget* parent);
    QMenu* create(QWidget* parent, const QString& title);
    QMenu* create(QWidget* parent, const QIcon& icon);
    QPoint getMenuPosition(QWidget* parent, QWidget* menu);
}