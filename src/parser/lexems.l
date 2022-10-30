%option yyclass="Lexer"
%option c++

%{

#include "lexer.hpp"

%}

%%

"+"	return process_plus();

%%
