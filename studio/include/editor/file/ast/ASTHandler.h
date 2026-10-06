#pragma once

#include <QString>
#include <QSet>

namespace Engine {

    class ASTHandler {
    public:
        virtual ~ASTHandler() = default;

        /// Parses the given source text and returns extracted identifiers (variables, functions, types)
        virtual QSet<QString> extractSymbols(const QString& sourceCode) = 0;
    };

}