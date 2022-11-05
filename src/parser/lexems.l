%option yyclass="Lexer"
%option c++

%{

#include "lexer.hpp"
// #include "grammar.tab.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
VAR     [A-Za-z_\$]+[A-Za-z_\$0-9]*
COLON   ":"
COMMENT \/\/(.*)$    
LAYER   "layer"
INPUT   "input"
OUTPUT  "output"
REPEAT  "repeat"
GLUE    "glue"
BIND    "bind"
IF      "if"
ELSE    "else"
FOR     "for"
IN      "in"
RETURN  "return"
CHAR    "char"
INT     "int"
FLOAT   "float"
DOUBLE  "double"
%%

{WS}              /* skip blanks and tabs */
"+"               return process_plus();
"-"               return process_minus();
"="               return process_equal();
";"               return process_scolon();
":"               { set_lexem(yytext, yylineno); return process_colon();}
{DIGIT1}{DIGIT}*  return process_number();
("=="|"!=")       { set_lexem(yytext, yylineno); return process_comp(yytext); }
("<="|">=")        { set_lexem(yytext, yylineno); return process_gele(yytext); }
("<"|">")         { set_lexem(yytext, yylineno); return process_gl(yytext); }
{COMMENT}         { set_lexem(yytext, yylineno); return process_comment(); }
{VAR}              { set_lexem(yytext, yylineno); return process_varname();}
.			      return process_error();

%%

// nothing