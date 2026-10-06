#include <Qsci/qscilexercustom.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjavascript.h>
#include <QColor>
#include <QFont>

namespace EngineEditorLexers {
    class AssemblyScript : public QsciLexerJavaScript {
        Q_OBJECT
    public:
        AssemblyScript(QObject* parent = nullptr) : QsciLexerJavaScript(parent) {
            QFont font("Consolas", 10);
            setDefaultFont(font);

            QColor darkBg("#2b2b2b");
            QColor defaultFg("#D4D4D4");

            setDefaultPaper(darkBg);
            setDefaultColor(defaultFg);
            setPaper(darkBg);

            setColor(defaultFg, Default);
            setColor(defaultFg, Identifier);
            setColor(QColor("#569CD6"), Keyword);
            setColor(QColor("#57A64A"), Comment);
            setColor(QColor("#57A64A"), CommentLine);
            setColor(QColor("#57A64A"), CommentDoc);
            setColor(QColor("#B5CEA8"), Number);
            setColor(QColor("#CE9178"), DoubleQuotedString);
            setColor(QColor("#CE9178"), SingleQuotedString);
            setColor(QColor("#B4B4B4"), Operator);


        }

        const char* language() const override {
            return "AssemblyScript";
        }

        const char* keywords(int set) const override {
            if (set == 1) {
                return "abstract as bool break case catch class const "
                    "continue declare default do else enum export extends "
                    "false final finally for function i8 i16 i32 i64 isize "
                    "if implements import in instanceof interface let new null "
                    "of private protected public return super switch "
                    "this throw true try type typeof u8 u16 u32 u64 usize "
                    "v128 f32 f64 void while yield";
            }
            return QsciLexerJavaScript::keywords(set);
        }

        const char* annotations() const {
            return "inline; noinline; alwaysInLine; external(module: string, name: string); external(name: string);"
                "export; unmanaged; packed; operator(op: string); operator.binary(op: string); operator.prefix(op: string); operator.postfix(op: string);"
                "lazy; final; global; pin; unsafe";
        }
    };
}