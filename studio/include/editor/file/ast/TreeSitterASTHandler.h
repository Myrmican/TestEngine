#pragma once

#include "editor/file/ast/ASTHandler.h"
extern "C" {
    #include <tree_sitter/api.h>
}

namespace Engine {

    class TreeSitterASTHandler : public ASTHandler {
    public:
        TreeSitterASTHandler(const TSLanguage* language, const char* querySource);
        ~TreeSitterASTHandler() override;

        TreeSitterASTHandler(const TreeSitterASTHandler&) = delete;
        TreeSitterASTHandler& operator=(const TreeSitterASTHandler&) = delete;

        QSet<QString> extractSymbols(const QString& sourceCode) override;

    private:
        TSParser* parser = nullptr;
        TSQuery* query = nullptr;
    };

}