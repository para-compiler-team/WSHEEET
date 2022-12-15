%option yyclass="Lexer"
%option c++

%{

#include "lexer.hpp"
#include "grammar.tab.hh"
using token_type = yy::parser::token_type;
/*
ON_CHAR_CURVED_BRACKET_RIGHT}              {lxWrap(yytext); return token_type::COLON_CHAR_CURVED_BRACKET_RIGHTWARNING}
{COLON_INT_CURVED_BRACKET_RIGHT}              {lxWrap(yytext); return token_type::COLON_INT_CURVED_BRACKET_RIGHT; RNING}
{COLON_FLOAT_CURVED_BRACKET_RIGHT}              {lxWrap(yytext); return token_type::COLON_FLOAT_CURVED_BRACKET_RIGHT; WARNIN}
{COLON_DOUBLE_CURVED_BRACKET_RIGHT}              {lxWrap(yytext); return token_type::COLON_DOUBLE_CURVED_BRACKET_RIGHT; WARNING}
{COLON_CHAR_COMMA}              {lxWrap(yytext); return token_type::COLON_CHAR_COMMA; //WARNING }
{COLON_INT_COMMA}              {lxWrap(yytext); return token_type::COLON_INT_COMMA; WARNING}
{COLON_FLOAT_COMMA}              {lxWrap(yytext); return token_type::COLON_FLOAT_COMMA; WARNING}
{COLON_DOUBLE_COMMA}              {lxWrap(yytext); return token_type::COLON_DOUBLE_COMMA; WARNING}

{COLON_VARNAME_COMMA}              {lxWrap(yytext); return token_type::COLON_VARNAME_COMMA; WARNING}
{COLON_VARNAME_CURVED_BRACKET}              {lxWrap(yytext); return token_type::COLON_VARNAME_CURVED_BRACKET; WARNING}
*/

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
WSnl  [ \t\v\n\f]
WS    [ \t\v\f]
COLON_INT   \:[ \t]*int
COLON_CHAR   \:[ \t]*char
COLON_FLOAT   \:[ \t]*float
COLON_DOUBLE   \:[ \t]*double
COLON_VECTOR_LESS    \:[ \t]*vector[ \t]*\<
COLON_INT_CURVED_BRACKET_RIGHT      \:[ \t]*int[ \t]*\)
COLON_CHAR_CURVED_BRACKET_RIGHT     \:[ \t]*char[ \t]*\)
COLON_FLOAT_CURVED_BRACKET_RIGHT    \:[ \t]*float[ \t]*\)
COLON_DOUBLE_CURVED_BRACKET_RIGHT   \:[ \t]*double[ \t]*\)
COLON_INT_COMMA                     \:[ \t]*int[ \t]*,
COLON_CHAR_COMMA                    \:[ \t]*char[ \t]*,
COLON_FLOAT_COMMA                   \:[ \t]*float[ \t]*,
COLON_DOUBLE_COMMA                  \:[ \t]*double[ \t]*,
COLON_VARNAME_CURVED_BRACKET        \:[ \t]*{L}{A}*[ \t]*\)
COLON_VARNAME_COMMA                 \:[ \t]*{L}{A}*[ \t]*,
COLON_VARNAME                       \:[ \t]*{L}{A}*
%%

"HAHA"                      { lxWrap(yytext); return process_error(); }
"DEBUG_LEXEM"               {return token_type::DEBUG_LEXEM;}

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
"in"                       { lxWrap(yytext); std::cout << "in!\n"; return token_type::IN;}

{COLON_CHAR}              {lxWrap(yytext); return token_type::COLON_CHAR; /*WARNING*/}
{COLON_INT}              {lxWrap(yytext); return token_type::COLON_INT; /*WARNING*/}
{COLON_FLOAT}              {lxWrap(yytext); return token_type::COLON_FLOAT; /*WARNING*/}
{COLON_DOUBLE}              {lxWrap(yytext); return token_type::COLON_DOUBLE; /*WARNING*/}
{COLON_VECTOR_LESS}              {lxWrap(yytext); return token_type::COLON_VECTOR_LESS; /*WARNING*/}
{COLON_VARNAME}              {lxWrap(yytext); return token_type::COLON_VARNAME; /*WARNING*/}


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
\n                      { lxWrap(yytext); }
.			{ lxWrap(yytext); return process_error(); }
%%

// nothing