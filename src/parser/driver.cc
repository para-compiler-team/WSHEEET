#include <iostream>
#include <memory>

#include "lexerdriver.hpp"

int yyFlexLexer::yywrap() { return 1; }
// int Lexer::yylex() { return 1; }

int main() {
    FlexLexer* lexer = new yyFlexLexer;
#if 0
    while (lexer->yylex() != 0) {
        lexer->print_current();
    }
#endif
    yy::LexerDriver driver(lexer);
    driver.parse();

    delete lexer;
}
