#include <QWidget>

class EngineRenderView : public QWidget {
    Q_OBJECT
public:
    explicit EngineRenderView(QWidget* parent = nullptr) : QWidget(parent) {
        // Prevent Qt from clearing or repainting the background
        setAttribute(Qt::WA_OpaquePaintEvent, true);
        setAttribute(Qt::WA_NoSystemBackground, true);

        // Indicates direct hardware rendering to the widget's native handle
        setAttribute(Qt::WA_NativeWindow, true);

        // Prevents paintEvent from firing unnecessarily when resized
        setAttribute(Qt::WA_PaintOnScreen, true);
    }

    // Return null engine paint engine to prevent Qt QPainter overhead
    QPaintEngine* paintEngine() const override {
        return nullptr;
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        // Leave empty: drive your engine render loop via QTimer, std::thread, 
        // or winId() / native surface hook.
        Q_UNUSED(event);
    }
};