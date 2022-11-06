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
  DEBUG_TOKEN "alex_great"
  EQUAL
  MINUS
  PLUS
  COLON
  SCOLON
  COMMA
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
%nterm <int> var_decl
%nterm var_type
%nterm input

%left '+' '-'

%start program

%%

program : statements
;

/* scope & statements */
scope : CURVED_BRACKET_LEFT statements CURVED_BRACKET_RIGHT statements
;

statements : %empty
           | statement statements
           | scope
;

statement : DEBUG_TOKEN SCOLON
;

/*
// var declarations
var_decl: VARNAME COLON var_type EQUAL input
       | VARNAME COLON var_type { std::cout << strdup($1) << std::endl; }
       | VARNAME EQUAL input
;

basic_type :  CHAR
           |  INT
           |  FLOAT
           |  DOUBLE
;

var_type:  basic_type
       |  INT CURVED_BRACKET_LEFT NUMBER CURVED_BRACKET_RIGHT
;

input: NUMBER
     | EXTERN_INPUT CURVED_BRACKET_LEFT NUMBER CURVED_BRACKET_RIGHT
;

// function declarations
func_decl : func_args_decl EQUAL scope SCOLON
;

func_args_decl : %empty
               | COLON ROUND_BRACKET_LEFT func_args_decl_list ROUND_BRACKET_RIGHT COLON basic_type
               | COLON ROUND_BRACKET_LEFT func_args_decl_list ROUND_BRACKET_RIGHT
;

func_args_decl_list : func_arg_decl
                    | func_arg_decl COMMA func_args_decl_list
;

func_arg_decl : VARNAME COLON basic_type
              | VARNAME
;

*/

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