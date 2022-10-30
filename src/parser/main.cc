#include <iostream>
#include <memory>

#include "lexer.hpp"

int yyFlexLexer::yywrap() { return 1; }

int main() {
    auto lexer = std::make_unique<Lexer>();
    while (lexer->yylex() != 0) {
        lexer->print_current();
    }
}
