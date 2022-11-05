//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Driver for simplest grammar of comparsions. Note plex_->YYText() usage.
//
//----------------------------------------------------------------------------

#pragma once

#include "grammar.tab.hh"
#include <FlexLexer.h>
#include "lexer.hpp"

#if 0
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
#endif
namespace yy {

class LexerDriver {
    Lexer *plex_;

    

  public:
    void hello_world() { std::cout << "hello world" << std::endl; }
    LexerDriver(Lexer *plex) : plex_(plex) {}

    parser::token_type yylex(parser::semantic_type *yylval) {
        parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
        if (tt == yy::parser::token_type::NUMBER)
            yylval->as<int>() = std::stoi(plex_->YYText());
        return tt;
    }

    bool parse() {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }
};

} // namespace yy