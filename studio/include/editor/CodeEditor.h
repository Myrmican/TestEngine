#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qsciapis.h>
#include <QString>

class CodeEditor : public QsciScintilla {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget* parent = nullptr);
};