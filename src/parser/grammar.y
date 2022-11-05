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
  
  CHAR
  INT
  FLOAT
  DOUBLE
  EXTERN_INPUT

  CURVED_BRACKET_LEFT
  CURVED_BRACKET_RIGHT
  SQUARE_BRACKET_LEFT
  SQUARE_BRACKET_RIGHT
  ROUND_BRACKET_LEFT
  ROUND_BRACKET_RIGHT
;

%token <int> NUMBER
%token <const char*> VARNAME
%nterm <int> vardecl
%nterm vartype
%nterm input

%left '+' '-'

%start program

%%

program : statements
;

scope : CURVED_BRACKET_LEFT statements CURVED_BRACKET_RIGHT
;

statements : vardecl
           | %empty
;

vardecl: VARNAME COLON vartype EQUAL input
       | VARNAME COLON vartype { std::cout << strdup($1) << std::endl; }
       | VARNAME EQUAL input
;

vartype:  CHAR
       |  INT
       |  FLOAT
       |  DOUBLE
       |  INT CURVED_BRACKET_LEFT NUMBER CURVED_BRACKET_RIGHT
;

input: NUMBER
     | EXTERN_INPUT CURVED_BRACKET_LEFT NUMBER CURVED_BRACKET_RIGHT
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