#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

// need to forward parser types to lexer
#include "grammar.tab.hh"

// here all lexems should be processed
class Lexer : public yyFlexLexer {
    std::string current_lexem{};
    std::string current_value{};

    yy::parser::token_type process_plus() {
        current_lexem = "operator";
        current_value = "+";
        return yy::parser::token_type::PLUS;
    }

    yy::parser::token_type process_minus() {
        current_lexem = "operator";
        current_value = "-";
        std::cerr << "MINUS" << "\n";
        return yy::parser::token_type::MINUS;
    }

    yy::parser::token_type process_equal() {
        current_lexem = "operator";
        current_value = "*";
        return yy::parser::token_type::EQUAL;
    }

    yy::parser::token_type process_scolon() {
        current_lexem = "operator";
        current_value = ";";
        return yy::parser::token_type::SCOLON;
    }

    yy::parser::token_type process_number() {
        current_lexem = "value";
        current_value = "number";
        return yy::parser::token_type::NUMBER;
    }

    yy::parser::token_type process_error() {
        current_lexem = "debug";
        current_value = "error";
        return yy::parser::token_type::ERR;
    }

  public:
    int yylex() override;
    void print_current() const {
        std::cout << current_lexem << " <" << current_value << ">" << std::endl;
    }
};