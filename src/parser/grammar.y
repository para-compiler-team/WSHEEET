/* ------------------------------------------------------------------------- **
 *
 * ------------------------------------------------------------------------- **
 *
 *  Simple, but incorrect grammar
 *  EQL -> EQ; EQL | empty
 *  EQ -> E = E
 *  E -> number | E + E | E - E
 *
 * ------------------------------------------------------------------------- */

%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::LexerDriver* driver}

%code requires
{
#include <iostream>
#include <string>
#include <utility>

// forward decl of argument to parser
namespace yy { class LexerDriver; }
}

%code
{
#include "lexerdriver.hpp"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         LexerDriver* driver);
}
}

%token
  EQUAL
  MINUS
  PLUS
  COLON
  SCOLON
  ERR
  EQ
  NOTEQ
  GE
  LE
  G
  L
  LINE_COMMENT
;

%token <int> NUMBER
%token <const char*> VARNAME
%nterm <int> vardecl

%left '+' '-'

%start program

%%

program: eqlist
;

eqlist : vardecl
       | %empty
;

vardecl: VARNAME COLON NUMBER { std::cout << ":" << "-" << strdup($1) << std::endl; }
;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         LexerDriver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string& msg){
  std::cerr << "Bison parser error:    " << msg << "\n";
}
}