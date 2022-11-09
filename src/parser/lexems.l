%option yyclass="Lexer"
%option c++

%{

#include "lexer.hpp"
// #include "grammar.tab.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
COLON   ":"
COMMENT \/\/(.*)$  

AND     "&&"  
OR      "||"

LAYER   "layer"
INPUT   "input"
OUTPUT  "output"
REPEAT  "repeat"
GLUE    "glue"
BIND    "bind"
VECTOR  "vector"
IF      "if"
ELSE    "else"
FOR     "for"
WHILE   "while"
IN      "in"
RETURN  "return"
CHAR    "char"
INT     "int"
FLOAT   "float"
DOUBLE  "double"
DEBUG_TOKEN "alex_great"

INT_REGEX {DIGIT1}{DIGIT}*|"0"
FLOAT_REGEX \b(\+|\-)?({DIGIT}{DIGIT1}*|{DIGIT1}{DIGIT}*)\.{DIGIT}*
CHAR_REGEX \'.\'
VAR_REGEX     [A-Za-z_\$]+[A-Za-z_\$0-9]*
%%

{WS}              /* skip blanks and tabs */
"="               { set_lexem(yytext, yylineno); return process_equal(); }
";"               { set_lexem(yytext, yylineno); return process_scolon(); }
":"               { set_lexem(yytext, yylineno); return process_colon(); }
","               { set_lexem(yytext, yylineno); return process_comma(); }
".."              { set_lexem(yytext, yylineno); return process_range(); }
\(|\)|\{|\}|\[|\]     { set_lexem(yytext, yylineno); return process_brackets(yytext); }
{INT_REGEX}  { set_lexem(yytext, yylineno); return process_int(); }
{FLOAT_REGEX} { set_lexem(yytext, yylineno); return process_float(); }
{CHAR_REGEX} {set_lexem(yytext, yylineno); return process_char(); }
("=="|"!=")       { set_lexem(yytext, yylineno); return process_comp(yytext); }
("<="|">=")        { set_lexem(yytext, yylineno); return process_gele(yytext); }
("<"|">")         { set_lexem(yytext, yylineno); return process_gl(yytext); }
{COMMENT}         { set_lexem(yytext, yylineno); return process_comment(); }
{INPUT}            {set_lexem(yytext, yylineno); return process_input();}
{DEBUG_TOKEN}       {set_lexem(yytext, yylineno); return yy::parser::token_type::DEBUG_TOKEN; }
{CHAR}|{INT}|{FLOAT}|{DOUBLE} { set_lexem(yytext, yylineno); return process_typename(yytext); }
{VECTOR}                        { set_lexem(yytext, yylineno); return process_vector(); }
{REPEAT}                        { set_lexem(yytext, yylineno); return process_repeat(); }
{GLUE}                          { set_lexem(yytext, yylineno); return process_glue(); }
{IF}|{ELSE}|{WHILE}|{FOR}                            { set_lexem(yytext, yylineno); return process_condition(yytext); }
{VAR_REGEX}              { set_lexem(yytext, yylineno); return process_varname(); }
"+"|"-"|"/"|"*"|{AND}|{OR}                      { set_lexem(yytext, yylineno); return process_arithmetic_op(yytext); }                    
.			      return process_error();

%%

// nothing