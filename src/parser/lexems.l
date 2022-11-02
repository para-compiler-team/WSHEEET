%option yyclass="Lexer"
%option c++

%{

#include "lexer.hpp"
// #include "grammar.tab.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]

%%

{WS}              /* skip blanks and tabs */
"+"               return process_plus();
"-"               return process_minus();
"="               return process_equal();
";"               return process_scolon();
{DIGIT1}{DIGIT}*  return process_number();
.			      return process_error();

%%

// nothing