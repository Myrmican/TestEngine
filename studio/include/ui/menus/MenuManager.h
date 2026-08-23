class QString;
class QWidget;

namespace Menu {
    QMenu* create(QWidget* parent);
    QMenu* create(QWidget* parent, const QString& title);
}