#include "editor/file/ast/TreeSitterASTHandler.h"
#include <QByteArray>
#include <cstring>

namespace Engine {

    TreeSitterASTHandler::TreeSitterASTHandler(const TSLanguage* language, const char* querySource) {
        if (!language || !querySource) {
            return;
        }

        // Initialize parser with the provided grammar
        parser = ts_parser_new();
        ts_parser_set_language(parser, language);

        // Compile the provided query
        uint32_t errorOffset = 0;
        TSQueryError errorType = TSQueryErrorNone;
        query = ts_query_new(
            language,
            querySource,
            uint32_t(strlen(querySource)),
            &errorOffset,
            &errorType
        );
    }

    TreeSitterASTHandler::~TreeSitterASTHandler() {
        if (query) {
            ts_query_delete(query);
            query = nullptr;
        }
        if (parser) {
            ts_parser_delete(parser);
            parser = nullptr;
        }
    }

    QSet<QString> TreeSitterASTHandler::extractSymbols(const QString& sourceCode) {
        QSet<QString> symbols;
        if (!parser || !query || sourceCode.isEmpty()) {
            return symbols;
        }

        QByteArray bytes = sourceCode.toUtf8();

        TSTree* tree = ts_parser_parse_string(
            parser,
            nullptr,
            bytes.constData(),
            uint32_t(bytes.size())
        );

        if (!tree) {
            return symbols;
        }

        TSNode rootNode = ts_tree_root_node(tree);
        TSQueryCursor* cursor = ts_query_cursor_new();
        ts_query_cursor_exec(cursor, query, rootNode);

        TSQueryMatch match;
        while (ts_query_cursor_next_match(cursor, &match)) {
            for (uint16_t i = 0; i < match.capture_count; ++i) {
                TSNode node = match.captures[i].node;
                uint32_t start = ts_node_start_byte(node);
                uint32_t end = ts_node_end_byte(node);

                QString symbol = QString::fromUtf8(bytes.mid(start, end - start));
                if (!symbol.isEmpty()) {
                    symbols.insert(symbol);
                }
            }
        }

        ts_query_cursor_delete(cursor);
        ts_tree_delete(tree);

        return symbols;
    }

}