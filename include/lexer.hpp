#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

// need to forward parser types to lexer
#include "grammar.tab.hh"

// here all lexems should be processed
class Lexer : public yyFlexLexer {
    using token_type = yy::parser::token_type;
    std::string current_lexem{};
    std::string current_value{};

    token_type sym_type(const char *yytext) { return token_type::IDENTIFIER; }

    // yy::parser::token_type process_plus() {

    void lxWrap(const char *yytext) { std::cout << "lexem: " << yytext << std::endl; }

    token_type check_type() { return token_type::IDENTIFIER; }

    void comment(void) {
        int c;

        while ((c = yyinput()) != 0)
            if (c == '*') {
                while ((c = yyinput()) == '*')
                    ;

                if (c == '/')
                    return;

                if (c == 0)
                    break;
            }
        LexerError("unterminated comment");
    }

    token_type check_type(const char *yytext) {
        switch (sym_type(yytext)) {
        case token_type::TYPEDEF_NAME: /* previously defined */
            return token_type::TYPEDEF_NAME;
        // case token_type::ENUMERATION_CONSTANT: /* previously defined */
            // return token_type::ENUMERATION_CONSTANT;
        default: /* includes undefined */
            return token_type::IDENTIFIER;
        }
    }

    token_type process_error() { LexerError("Unrecognized lexem"); return token_type::ERR; }

  public:
    int yylex() override;
};