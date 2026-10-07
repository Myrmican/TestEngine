#include <QWidget>
#include <QFont>
#include <QColor>
#include <Qsci/qsciglobal.h>
#include <editor/file/Intellisense.h>
#include <editor/file/CodeEditor.h>
#include <editor/file/lexers/AssemblyScript.h>

namespace Engine {
    CodeEditor::CodeEditor(QWidget* parent) : QsciScintilla(parent) {

        auto* lexer = new EngineEditorLexers::AssemblyScript(this);
        this->setLexer(lexer);

        this->setAutoIndent(true);
        this->setIndentationGuides(true);
        this->setUtf8(true);
        this->setIndentationsUseTabs(true);
        this->setTabWidth(4);

        QColor darkBg("#2b2b2b");
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

        this->setAutoCompletionSource(QsciScintilla::AcsAPIs);
        this->setAutoCompletionThreshold(1);
        this->setAutoCompletionReplaceWord(true);

        this->setCallTipsStyle(QsciScintilla::CallTipsContext);
        this->setCallTipsPosition(QsciScintilla::CallTipsBelowText);

        this->setBraceMatching(QsciScintilla::SloppyBraceMatch);
        this->setAutoCompletionCaseSensitivity(false);

        this->setMatchedBraceForegroundColor(QColor(255, 215, 0));

        Engine::Intellisense* intellisense = new Engine::Intellisense(this, static_cast<QsciLexer*>(lexer));

        setStyleSheet(
            "QsciScintilla QListWidget {"
            "   font-family: 'Consolas';"
            "   font-size: 10pt;"
            "   background-color: #252526;"
            "   color: #DCDCDC;"
            "   border-radius: 0px;"
            "   border: 1px solid #454545;"
            "}"
            "QsciScintilla QListWidget::item:selected {"
            "   background-color: #04395E;"
            "   color: #FFFFFF;"
            "}"
        );
    }
}