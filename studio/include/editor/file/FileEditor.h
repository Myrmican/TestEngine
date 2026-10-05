#include <Qsci/qsciscintilla.h>

namespace Engine {
    class FileEditor : public QsciScintilla {
        Q_OBJECT

    public:
        QsciLexer* lexer = nullptr;

        explicit FileEditor(QWidget* parent = nullptr);
    };
}