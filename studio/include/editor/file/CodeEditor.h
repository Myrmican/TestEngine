#pragma once

#include <editor/file/Intellisense.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjava.h>
#include <QKeyEvent>

namespace Engine {
    class CodeEditor : public QsciScintilla {
        Q_OBJECT

    public:
        QsciLexer* lexer = nullptr;
        Engine::Intellisense* intellisense = nullptr;

        explicit CodeEditor(QWidget* parent = nullptr);
    };
}