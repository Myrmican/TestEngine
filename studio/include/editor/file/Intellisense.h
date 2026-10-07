#pragma once

#include <Qsci/qsciapis.h>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <Qsci/qscilexer.h>

namespace Engine {

    class CodeEditor;

    class Intellisense {
        inline static const QRegularExpression paramRegex = QRegularExpression(R"(\(.*\)\s*$)");
    public:
        Intellisense(CodeEditor* editor, QsciLexer* lexer);

    private:
        QStringList intellisenseEntries;
        QsciAPIs* api = nullptr;
    };
}