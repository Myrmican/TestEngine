#pragma once

#include <Qsci/qsciapis.h>
#include <QWidget>
#include <QFont>
#include <QColor>
#include <Qsci/qsciglobal.h>
#include <editor/CodeEditor.h>

//namespace CodeEditor {
//    QWidget* createEditor(QTabWidget* parent = nullptr) {
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
//        this->setCaretForegroundColor(QColor("#AEAFAD"));
//        this->setCaretLineVisible(true);
//        this->setCaretLineBackgroundColor(QColor("#282828"));
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
//		this->setText("// Java code example\n"
//			"public class HelloWorld {\n"
//			"   public static void main(String[] args) {\n"
//			"       System.out.println(\"Hello, World!\");\n"
//			"   }\n"
//			"}\n");
//
//        return this;
//    }
//}

namespace Engine {
	CodeEditor::CodeEditor(QWidget* parent) : QsciScintilla(parent) {

		lexer = new QsciLexerJava(this);

		QFont thisFont("Consolas", 10);
		thisFont.setStyleHint(QFont::Monospace);
		lexer->setFont(thisFont);

		this->setLexer(lexer);

		this->setAutoIndent(true);
		this->setIndentationGuides(true);
		this->setUtf8(true);
		this->setIndentationsUseTabs(true);
		this->setTabWidth(4);

		this->setMarginType(0, QsciScintilla::NumberMargin);
		this->setMarginWidth(0, "0000");
		this->setMarginsBackgroundColor(QColor("#252526"));
		this->setMarginsForegroundColor(QColor("#858585"));
		this->setMarginLineNumbers(0, true);

		this->setSelectionBackgroundColor(QColor("#264F78"));
		this->resetSelectionForegroundColor();

		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		this->setScrollWidthTracking(true);
		this->setScrollWidth(1);

		this->setAutoCompletionSource(QsciScintilla::AcsAll);
		this->setAutoCompletionThreshold(1);
		this->setAutoCompletionReplaceWord(true);

		this->setCallTipsStyle(QsciScintilla::CallTipsContext);
		this->setCallTipsPosition(QsciScintilla::CallTipsBelowText);
	}
}