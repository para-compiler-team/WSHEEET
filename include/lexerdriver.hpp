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
#include <cstring>

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
        else if (tt == yy::parser::token_type::VARNAME) {
            char varname[1000] = "unknown_varname";
            const char* var = strndup(plex_->YYText(), 1000);
            if (var == nullptr) {
                std::cerr << varname << "\n";
            } else {
                std::cerr << var << std::endl;
            }
            yylval->as<const char*>() = var;
        }
        return tt;
    }

    bool parse() {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }
};

} // namespace yy