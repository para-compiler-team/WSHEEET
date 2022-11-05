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

    yy::parser::token_type process_plus() {
        current_lexem = "operator";
        current_value = "+";
        return yy::parser::token_type::PLUS;
    }

    yy::parser::token_type process_minus() {
        current_lexem = "operator";
        current_value = "-";
        return yy::parser::token_type::MINUS;
    }

    yy::parser::token_type process_equal() {
        current_lexem = "operator";
        current_value = "*";
        return yy::parser::token_type::EQUAL;
    }

    token_type process_colon() {
        return yy::parser::token_type::COLON;
    }

    yy::parser::token_type process_scolon() {
        current_lexem = "operator";
        current_value = ";";
        return yy::parser::token_type::SCOLON;
    }

    token_type process_varname() {
        return token_type::VARNAME;
    }

    yy::parser::token_type process_number() {
        current_lexem = "value";
        current_value = "number";
        return yy::parser::token_type::NUMBER;
    }

    yy::parser::token_type process_error() {
        current_lexem = "debug";
        current_value = "error";
        std::cerr << "Flex lexer error\n";
        return yy::parser::token_type::ERR;
    }

    void process_operand(const char *opnd) { std::cout << opnd << std::endl; }

    void set_lexem(const char *lexem, int line) {}

    token_type process_comp(const char *lexem) {
        switch(lexem[0]) {
            case '!':
                return token_type::NOTEQ;
            case '=':
                return token_type::EQ;
            default:
                return token_type::ERR;
        }
    }

    token_type process_gele(const char *lexem) {
        switch(lexem[0]) {
            case '<':
                return token_type::LE;
            case '>':
                return token_type::GE;
            default:
                return token_type::ERR;
        }
    }

    token_type process_gl(const char *lexem) {
        switch(lexem[0]) {
            case '<':
                return token_type::L;
            case '>':
                return token_type::G;
            default:
                return token_type::ERR;
        }
    }

    token_type process_comment() {
        return token_type::LINE_COMMENT;
    }
    

  public:
    int yylex() override;
    void print_current() const {
        std::cout << current_lexem << " <" << current_value << ">" << std::endl;
    }
};