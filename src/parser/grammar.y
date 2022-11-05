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
  EQUAL   //"="
  MINUS   // "-"
  PLUS    // "+"
  SCOLON  // ";"
  ERR
;

%token <int> NUMBER
%nterm <int> equals
%nterm <int> expr

%left '+' '-'

%start program

%%

program: eqlist
;

eqlist: equals SCOLON eqlist
      | %empty
;

equals: expr EQUAL expr       { 
                                driver->hello_world();
                                $$ = ($1 == $3); 
                                std::cout << "Checking: " << $1 << " vs " << $3 
                                          << "; Result: " << $$
                                          << std::endl; 
                              }
;

expr: expr PLUS NUMBER        { $$ = $1 + $3; }
    | expr MINUS NUMBER       { $$ = $1 - $3; }
    | NUMBER                  { $$ = $1; }
;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         LexerDriver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&){}
}