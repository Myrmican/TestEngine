#include <editor/file/Intellisense.h>
#include <editor/file/CodeEditor.h>
#include <Qsci/qsciapis.h>
#include <QString>
#include <QStringList>
#include <QRegularExpression>

namespace Engine {
    Intellisense::Intellisense(CodeEditor* editor, QsciLexer* lexer) {
        auto* api = new QsciAPIs(lexer);
        this->api = api;

        const char* keywordList = lexer->keywords(1);
        //const char* annotationList = lexer->annotations();

        if (keywordList) {
            QStringList keywords = QString(keywordList).split(QChar(' '), Qt::SkipEmptyParts);
            for (const QString& keyword : keywords) {
                api->add(keyword);
                intellisenseEntries.append(keyword);
            }
        }

        /*if (annotationList) {
            QStringList annotations = QString(annotationList).split(QChar(';'), Qt::SkipEmptyParts);
            for (const QString& annotation : annotations) {
                QString trimmedAnnotation = annotation.trimmed();

                api->add(trimmedAnnotation);
                intellisenseEntries.append(trimmedAnnotation);
            }
        }*/

        QObject::connect(editor, &QsciScintilla::SCN_AUTOCCOMPLETED, editor, [editor, this]() {
            int line, col;
            editor->getCursorPosition(&line, &col);

            int wordStart = col;
            QString lineText = editor->text(line);

            while (wordStart > 0 && (lineText[wordStart - 1].isLetterOrNumber() || lineText[wordStart - 1] == '.')) {
                wordStart--;
            }

            QString currentWord = lineText.mid(wordStart, col - wordStart);

            bool takesParams = false;
            for (const QString& keyword : intellisenseEntries) {
                if (keyword.startsWith(currentWord) && keyword.contains(paramRegex)) {
                    takesParams = true;
                    break;
                }
            }

            if (takesParams) {
                editor->insert("()");
                editor->setCursorPosition(line, col + 1);
            }
            });

        api->prepare();
    };
}