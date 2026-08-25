#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjava.h>

class CodeEditor {

public:
    QsciScintilla* editor = nullptr;
    QsciLexer* lexer = nullptr;

    explicit CodeEditor(QWidget* parent = nullptr);
};