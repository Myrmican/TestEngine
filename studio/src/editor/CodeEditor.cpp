#pragma once

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qsciapis.h>
#include <QWidget>
#include <QFont>
#include <QColor>
#include <Qsci/qsciglobal.h>

//namespace CodeEditor {
//    QWidget* createEditor(QTabWidget* parent = nullptr) {
//        auto* editor = new QsciScintilla(parent);
//        auto* lexer = new QsciLexerJava(editor);
//
//        QFont editorFont("Consolas", 10);
//        editorFont.setStyleHint(QFont::Monospace);
//        lexer->setFont(editorFont);
//
//        QColor darkBg("#2b2b2b");
//        QColor defaultFg("#D4D4D4");
//
//        lexer->setDefaultPaper(darkBg);
//        lexer->setDefaultColor(defaultFg);
//        lexer->setPaper(darkBg);
//        lexer->setColor(defaultFg);
//
//        lexer->setColor(defaultFg, QsciLexerJava::Default);
//        lexer->setColor(defaultFg, QsciLexerJava::Identifier);
//        lexer->setColor(QColor("#57A64A"), QsciLexerJava::Comment);
//        lexer->setColor(QColor("#57A64A"), QsciLexerJava::CommentLine);
//        lexer->setColor(QColor("#57A64A"), QsciLexerJava::CommentDoc);
//
//        lexer->setColor(QColor("#569CD6"), QsciLexerJava::Keyword);
//        lexer->setColor(QColor("#C586C0"), QsciLexerJava::KeywordSet2);
//        lexer->setColor(QColor("#B5CEA8"), QsciLexerJava::Number);
//        lexer->setColor(QColor("#CE9178"), QsciLexerJava::DoubleQuotedString);
//        lexer->setColor(QColor("#CE9178"), QsciLexerJava::SingleQuotedString);
//
//        editor->setLexer(lexer);
//
//        editor->setCaretForegroundColor(QColor("#AEAFAD"));
//        editor->setCaretLineVisible(true);
//        editor->setCaretLineBackgroundColor(QColor("#282828"));
//
//        editor->setAutoIndent(true);
//        editor->setIndentationGuides(true);
//        editor->setUtf8(true);
//        editor->setIndentationsUseTabs(true);
//        editor->setTabWidth(4);
//
//        editor->setMarginType(0, QsciScintilla::NumberMargin);
//        editor->setMarginWidth(0, "0000");
//        editor->setMarginsBackgroundColor(QColor("#252526"));
//        editor->setMarginsForegroundColor(QColor("#858585"));
//        editor->setMarginLineNumbers(0, true);
//
//        editor->setSelectionBackgroundColor(QColor("#264F78"));
//        editor->resetSelectionForegroundColor();
//
//        editor->setCallTipsStyle(QsciScintilla::CallTipsContext);
//        editor->setCallTipsPosition(QsciScintilla::CallTipsBelowText);
//
//        auto* api = new QsciAPIs(lexer);
//        api->add("public");
//        api->add("class");
//        api->add("System.out.println");
//
//        api->add("drawRect(int x, int y, int width, int height) Draws a rectangle.");
//        api->add("drawLine(int x1, int y1, int x2, int y2) Draws a line between two points.");
//
//        api->prepare();
//
//        editor->setAutoCompletionSource(QsciScintilla::AcsAll);
//        editor->setAutoCompletionThreshold(1);
//        editor->setAutoCompletionReplaceWord(true);
//
//        editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//        editor->setScrollWidthTracking(true);
//        editor->setScrollWidth(1);
//
//		editor->setText("// Java code example\n"
//			"public class HelloWorld {\n"
//			"   public static void main(String[] args) {\n"
//			"       System.out.println(\"Hello, World!\");\n"
//			"   }\n"
//			"}\n");
//
//        return editor;
//    }
//}

class CodeEditor : public QsciScintilla {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget* parent = nullptr);
};