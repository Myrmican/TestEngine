#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjava.h>

namespace Engine {
    class CodeEditor : public QsciScintilla {
        Q_OBJECT

    public:
        QsciLexer* lexer = nullptr;

        explicit CodeEditor(QWidget* parent = nullptr);
    };
}