#include <Qsci/qsciapis.h>
#include <QWidget>
#include <QFont>
#include <QColor>
#include <Qsci/qsciglobal.h>
#include <editor/code/CodeEditor.h>

namespace Engine {
	CodeEditor::CodeEditor(QWidget* parent) : QsciScintilla(parent) {

		lexer = new QsciLexerJava(this);
		this->setLexer(lexer);

		QFont thisFont("Consolas", 10);
		thisFont.setStyleHint(QFont::Monospace);
		lexer->setDefaultFont(thisFont);
		lexer->setFont(thisFont);

		QColor darkBg("#2b2b2b");
		QColor defaultFg("#D4D4D4");

		lexer->setDefaultPaper(darkBg);
		lexer->setDefaultColor(defaultFg);
		lexer->setPaper(darkBg);
		lexer->setColor(defaultFg);

		this->setAutoIndent(true);
		this->setIndentationGuides(true);
		this->setUtf8(true);
		this->setIndentationsUseTabs(true);
		this->setTabWidth(4);

		this->SendScintilla(QsciScintilla::SCI_SETKEYWORDS, 1, "@Override @Deprecated @SuppressWarnings @Target @Retention");

		lexer->setColor(defaultFg, QsciLexerJava::Default);
		lexer->setColor(defaultFg, QsciLexerJava::Identifier);
		lexer->setColor(QColor("#57A64A"), QsciLexerJava::Comment);
		lexer->setColor(QColor("#57A64A"), QsciLexerJava::CommentLine);
		lexer->setColor(QColor("#57A64A"), QsciLexerJava::CommentDoc);
		lexer->setColor(QColor("#569CD6"), QsciLexerJava::Keyword);
		lexer->setColor(QColor("#C586C0"), QsciLexerJava::KeywordSet2);
		lexer->setColor(QColor("#B5CEA8"), QsciLexerJava::Number);
		lexer->setColor(QColor("#CE9178"), QsciLexerJava::DoubleQuotedString);
		lexer->setColor(QColor("#CE9178"), QsciLexerJava::SingleQuotedString);
		lexer->setColor(QColor("#4EC9B0"), 15);

		this->setMatchedBraceBackgroundColor(darkBg);
		this->setMatchedBraceForegroundColor(QColor("#569CD6"));

		this->setUnmatchedBraceBackgroundColor(darkBg);
		this->setUnmatchedBraceForegroundColor(QColor("#F44747"));

		this->setCaretForegroundColor(QColor("#AEAFAD"));
		this->setCaretLineVisible(true);
		this->setCaretLineBackgroundColor(QColor("#282828"));

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
		
		this->setBraceMatching(QsciScintilla::SloppyBraceMatch);
		this->setAutoCompletionCaseSensitivity(false);

		auto* api = new QsciAPIs(lexer);

		const char* kwList = lexer->keywords(1);

		if (kwList) {
			QStringList keywords = QString(kwList).split(QChar(' '), Qt::SkipEmptyParts);

			for (const QString& keyword : keywords) {
				api->add(keyword);
			}

			api->prepare();
		}
	}
}