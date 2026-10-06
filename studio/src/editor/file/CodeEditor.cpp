#include <Qsci/qsciapis.h>
#include <QWidget>
#include <QFont>
#include <QColor>
#include <Qsci/qsciglobal.h>
#include <editor/file/CodeEditor.h>
#include <editor/file/lexers/AssemblyScript.h>

namespace Engine {
    CodeEditor::CodeEditor(QWidget* parent) : QsciScintilla(parent) {

        auto* lexer = new EngineEditorLexers::AssemblyScript(this);
        this->setLexer(lexer);

        // Editor styling & features
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

        // Setup APIs
        auto* api = new QsciAPIs(lexer);
        const char* keywordList = lexer->keywords(1);
		const char* annotationList = lexer->annotations();

        if (keywordList) {
            QStringList keywords = QString(keywordList).split(QChar(' '), Qt::SkipEmptyParts);
            for (const QString& keyword : keywords) {
                api->add(keyword);
            }
        }

        if (annotationList) {
            QStringList annotations = QString(annotationList).split(QChar(';'), Qt::SkipEmptyParts);
            for (QString& annotation : annotations) {
                annotation = annotation.trimmed();
                api->add(annotation);
            }
        }

        api->prepare();
    }
}