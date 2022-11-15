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

"auto"					{ lxWrap(yytext); return token_type::AUTO; }
"break"					{ lxWrap(yytext); return token_type::BREAK; }
"case"					{ lxWrap(yytext); return token_type::CASE; }
"char"					{ lxWrap(yytext); return token_type::CHAR; }
"const"					{ lxWrap(yytext); return token_type::CONST; }
"continue"				{ lxWrap(yytext); return token_type::CONTINUE; }
"default"				{ lxWrap(yytext); return token_type::DEFAULT; }
"do"					{ lxWrap(yytext); return token_type::DO; }
"double"				{ lxWrap(yytext); return token_type::DOUBLE; }
"else"					{ lxWrap(yytext); return token_type::ELSE; }
"enum"					{ lxWrap(yytext); return token_type::ENUM; }
"extern"				{ lxWrap(yytext); return token_type::EXTERN; }
"float"					{ lxWrap(yytext); return token_type::FLOAT; }
"for"					{ lxWrap(yytext); return token_type::FOR; }
"goto"					{ lxWrap(yytext); return token_type::GOTO; }
"if"					{ lxWrap(yytext); return token_type::IF; }
"inline"				{ lxWrap(yytext); return token_type::INLINE; }
"int"					{ lxWrap(yytext); return token_type::INT; }
"long"					{ lxWrap(yytext); return token_type::LONG; }
"register"				{ lxWrap(yytext); return token_type::REGISTER; }
"restrict"				{ lxWrap(yytext); return token_type::RESTRICT; }
"return"				{ lxWrap(yytext); return token_type::RETURN; }
"short"					{ lxWrap(yytext); return token_type::SHORT; }
"signed"				{ lxWrap(yytext); return token_type::SIGNED; }
"sizeof"				{ lxWrap(yytext); return token_type::SIZEOF; }
"static"				{ lxWrap(yytext); return token_type::STATIC; }
"struct"				{ lxWrap(yytext); return token_type::STRUCT; }
"switch"				{ lxWrap(yytext); return token_type::SWITCH; }
"typedef"				{ lxWrap(yytext); return token_type::TYPEDEF; }
"union"					{ lxWrap(yytext); return token_type::UNION; }
"unsigned"				{ lxWrap(yytext); return token_type::UNSIGNED; }
"void"					{ lxWrap(yytext); return token_type::VOID; }
"volatile"				{ lxWrap(yytext); return token_type::VOLATILE; }
"while"					{ lxWrap(yytext); return token_type::WHILE; }
"_Alignas"                              { lxWrap(yytext); return token_type::ALIGNAS; }
"_Alignof"                              { lxWrap(yytext); return token_type::ALIGNOF; }
"_Atomic"                               { lxWrap(yytext); return token_type::ATOMIC; }
"_Bool"                                 { lxWrap(yytext); return token_type::BOOL; }
"_Complex"                              { lxWrap(yytext); return token_type::COMPLEX; }
"_Generic"                              { lxWrap(yytext); return token_type::GENERIC; }
"_Imaginary"                            { lxWrap(yytext); return token_type::IMAGINARY; }
"_Noreturn"                             { lxWrap(yytext); return token_type::NORETURN; }
"_Static_assert"                        { lxWrap(yytext); return token_type::STATIC_ASSERT; }
"_Thread_local"                         { lxWrap(yytext); return token_type::THREAD_LOCAL; }
"__func__"                              { lxWrap(yytext); return token_type::FUNC_NAME; }

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

"..."					{ lxWrap(yytext); return token_type::ELLIPSIS; }
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
"->"					{ lxWrap(yytext); return token_type::PTR_OP; }
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