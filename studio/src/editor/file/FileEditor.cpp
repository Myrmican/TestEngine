#include <Qsci/qsciapis.h>
#include <QWidget>
#include <QFont>
#include <QColor>
#include <Qsci/qsciglobal.h>
#include <editor/file/FileEditor.h>

namespace Engine {
	FileEditor::FileEditor(QWidget* parent) : QsciScintilla(parent) {

		QColor darkBg("#2b2b2b");
		QColor defaultFg("#D4D4D4");

		this->setAutoIndent(true);
		this->setIndentationGuides(true);
		this->setUtf8(true);
		this->setIndentationsUseTabs(true);
		this->setTabWidth(4);

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
	}
}