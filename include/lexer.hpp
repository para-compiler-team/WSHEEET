#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

class Lexer : public yyFlexLexer {
    std::string current_lexem{};
    std::string current_value{};

    int process_plus() {
        current_lexem = "operator";
        current_value = "+";
        return 1;
    }

  public:
    int yylex() override;
    void print_current() const {
        std::cout << current_lexem << " <" << current_value << ">" << std::endl;
    }
};