%option yyclass="Lexer"
%option c++

%{

#include "lexer.hpp"
#include "grammar.tab.hh"
using token_type = yy::parser::token_type;

%}

%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]
%%

"HAHA"                      { lxWrap(yytext); return process_error(); }

"/*"                                    { comment(); }
"//".*                                    { /* consume //-comment */ }
"char"					{ lxWrap(yytext); return token_type::CHAR; }
"double"				{ lxWrap(yytext); return token_type::DOUBLE; }
"else"					{ lxWrap(yytext); return token_type::ELSE; }
"float"					{ lxWrap(yytext); return token_type::FLOAT; }
"for"					{ lxWrap(yytext); return token_type::FOR; }
"if"					{ lxWrap(yytext); return token_type::IF; }
"int"					{ lxWrap(yytext); return token_type::INT; }
"return"				{ lxWrap(yytext); return token_type::RETURN; }
"while"					{ lxWrap(yytext); return token_type::WHILE; }

"layer"                        { lxWrap(yytext); return token_type::LAYER; }
"input"                        { lxWrap(yytext); return token_type::INPUT; }
"output"                       { lxWrap(yytext); return token_type::OUTPUT; }
"repeat"                       { lxWrap(yytext); return token_type::REPEAT; }
"glue"                         { lxWrap(yytext); return token_type::GLUE; }
"bind"                         { lxWrap(yytext); return token_type::BIND; }
"vector"                       { lxWrap(yytext); return token_type::VECTOR;}

{L}{A}*					{ lxWrap(yytext); return check_type(); }

{HP}{H}+{IS}?				{ lxWrap(yytext); return token_type::I_CONSTANT; }
{NZ}{D}*{IS}?				{ lxWrap(yytext); return token_type::I_CONSTANT; }
"0"{O}*{IS}?				{ lxWrap(yytext); return token_type::I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ lxWrap(yytext); return token_type::I_CONSTANT; }

{D}+{E}{FS}?				{ lxWrap(yytext); return token_type::F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			{ lxWrap(yytext); return token_type::F_CONSTANT; }
{D}+"."{E}?{FS}?			{ lxWrap(yytext); return token_type::F_CONSTANT; }
{HP}{H}+{P}{FS}?			{ lxWrap(yytext); return token_type::F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			{ lxWrap(yytext); return token_type::F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			{ lxWrap(yytext); return token_type::F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ lxWrap(yytext); return token_type::STRING_LITERAL; }

".."                    { lxWrap(yytext); return token_type::INLINE_RANGE; }
">>="					{ lxWrap(yytext); return token_type::RIGHT_ASSIGN; }
"<<="					{ lxWrap(yytext); return token_type::LEFT_ASSIGN; }
"+="					{ lxWrap(yytext); return token_type::ADD_ASSIGN; }
"-="					{ lxWrap(yytext); return token_type::SUB_ASSIGN; }
"*="					{ lxWrap(yytext); return token_type::MUL_ASSIGN; }
"/="					{ lxWrap(yytext); return token_type::DIV_ASSIGN; }
"%="					{ lxWrap(yytext); return token_type::MOD_ASSIGN; }
"&="					{ lxWrap(yytext); return token_type::AND_ASSIGN; }
"^="					{ lxWrap(yytext); return token_type::XOR_ASSIGN; }
"|="					{ lxWrap(yytext); return token_type::OR_ASSIGN; }
">>"					{ lxWrap(yytext); return token_type::RIGHT_OP; }
"<<"					{ lxWrap(yytext); return token_type::LEFT_OP; }
"++"					{ lxWrap(yytext); return token_type::INC_OP; }
"--"					{ lxWrap(yytext); return token_type::DEC_OP; }
"&&"					{ lxWrap(yytext); return token_type::AND_OP; }
"||"					{ lxWrap(yytext); return token_type::OR_OP; }
"<="					{ lxWrap(yytext); return token_type::LE_OP; }
">="					{ lxWrap(yytext); return token_type::GE_OP; }
"=="					{ lxWrap(yytext); return token_type::EQ_OP; }
"!="					{ lxWrap(yytext); return token_type::NE_OP; }
";"					{ lxWrap(yytext); return ';'; }
("{"|"<%")				{ lxWrap(yytext); return '{'; }
("}"|"%>")				{ lxWrap(yytext); return '}'; }
","					{ lxWrap(yytext); return ','; }
":"					{ lxWrap(yytext); return ':'; }
"="					{ lxWrap(yytext); return '='; }
"("					{ lxWrap(yytext); return '('; }
")"					{ lxWrap(yytext); return ')'; }
("["|"<:")				{ lxWrap(yytext); return '['; }
("]"|":>")				{ lxWrap(yytext); return ']'; }
"."					{ lxWrap(yytext); return '.'; }
"&"					{ lxWrap(yytext); return '&'; }
"!"					{ lxWrap(yytext); return '!'; }
"~"					{ lxWrap(yytext); return '~'; }
"-"					{ lxWrap(yytext); return '-'; }
"+"					{ lxWrap(yytext); return '+'; }
"*"					{ lxWrap(yytext); return '*'; }
"/"					{ lxWrap(yytext); return '/'; }
"%"					{ lxWrap(yytext); return '%'; }
"<"					{ lxWrap(yytext); return '<'; }
">"					{ lxWrap(yytext); return '>'; }
"^"					{ lxWrap(yytext); return '^'; }
"|"					{ lxWrap(yytext); return '|'; }
"?"					{ lxWrap(yytext); return '?'; }

{WS}+					{ /* whitespace separates tokens */ }
.			{ lxWrap(yytext); return process_error(); }
%%

// nothing