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
DEBUG_TOKEN "alex_great"
%%

{WS}              /* skip blanks and tabs */
"+"               return process_plus();
"-"               return process_minus();
"="               { set_lexem(yytext, yylineno); return process_equal(); }
";"               { set_lexem(yytext, yylineno); return process_scolon(); }
":"               { set_lexem(yytext, yylineno); return process_colon(); }
","               { set_lexem(yytext, yylineno); return process_comma(); }
\(|\)|\{|\}|\[|\]     { set_lexem(yytext, yylineno); return process_brackets(yytext); }
{DIGIT1}{DIGIT}*|"0"  { set_lexem(yytext, yylineno); return process_number(); }
("=="|"!=")       { set_lexem(yytext, yylineno); return process_comp(yytext); }
("<="|">=")        { set_lexem(yytext, yylineno); return process_gele(yytext); }
("<"|">")         { set_lexem(yytext, yylineno); return process_gl(yytext); }
{COMMENT}         { set_lexem(yytext, yylineno); return process_comment(); }
{INPUT}            {set_lexem(yytext, yylineno); return process_input();}
{DEBUG_TOKEN}       {set_lexem(yytext, yylineno); return yy::parser::token_type::DEBUG_TOKEN; }
{CHAR}|{INT}|{FLOAT}|{DOUBLE} { set_lexem(yytext, yylineno); return process_typename(yytext); }
{VAR}              { set_lexem(yytext, yylineno); return process_varname();}
.			      return process_error();

%%

// nothing