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
// #include "AST/TreeNode.hpp"
#include <AST/TypeBuilder.hpp>

namespace yy {

class LexerDriver {
    Lexer *plex_;
    

  public:
    wsheeet::ast::TypeBuilder m_builder{};
    void hello_world() { std::cout << "hello world" << std::endl; }
    LexerDriver(Lexer *plex) : plex_(plex) {plex_->set_debug(1);}

    parser::token_type yylex(parser::semantic_type *yylval) {
        parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());
        if (tt == yy::parser::token_type::I_CONSTANT) {
            std::cerr << "I_CONSTANT\n"; 
            yylval->i_const = std::stoi(plex_->YYText());
        }
        // if (tt == yy::parser::token_type::INT_NUMBER)
        //     yylval->as<int>() = std::stoi(plex_->YYText());
        // else if (tt == yy::parser::token_type::VARNAME) {
        //     const char* var = plex_->YYText();
        //     if (var == nullptr) {
        //         std::cerr << "YYText error\n";
        //         // return parser::token_type::ERR;
        //     }
        //     yylval->as<const char*>() = var;
        // } else if (tt == yy::parser::token_type::CHAR) {
        //     const char* lexem = plex_->YYText();
        //     yylval->as<char>() = lexem[1];
        // }
        return tt;
    }

    bool parse() {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }
};

} // namespace yy