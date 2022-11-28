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
%token ERR

%token	IDENTIFIER I_CONSTANT F_CONSTANT STRING_LITERAL FUNC_NAME
/* %token  SIZEOF */
/* %token	PTR_OP */
%token  INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME
/* %token ENUMERATION_CONSTANT */

/* %token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE */
/* %token	CONST RESTRICT VOLATILE */
/* %token	BOOL */
%token CHAR
/* %token SHORT */
%token INT
/* %token LONG */
/* %token SIGNED */
/* %token UNSIGNED */
%token FLOAT
%token DOUBLE
/* %token VOID */
/* %token	COMPLEX IMAGINARY  */
/* %token	STRUCT UNION ENUM ELLIPSIS */

/* %token	CASE DEFAULT SWITCH  */
%token IF ELSE THEN
%token WHILE
/* %token DO */
%token FOR 
/* %token GOTO CONTINUE BREAK */
%token RETURN

/* %token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL */

%token INLINE_RANGE LAYER, INPUT, OUTPUT, REPEAT, GLUE, BIND, VECTOR, IN

%start translation_unit

// for x.x & x.x() parcing
/* %precedence IDENTIFIER */
/* %precedence ':' */
/* %precedence '(' */
%left IDENTIFIER

// for fixing conflicts in direct_declarator
%left ':'

%right '='
%right MUL_ASSIGN
%right DIV_ASSIGN
%right MOD_ASSIGN
%right ADD_ASSIGN
%right SUB_ASSIGN
%right LEFT_ASSIGN
%right RIGHT_ASSIGN
%right AND_ASSIGN
%right XOR_ASSIGN
%right OR_ASSIGN

%precedence THEN
%precedence ELSE

%%
// node
primary_assignable_expression
	: IDENTIFIER
	| IDENTIFIER '.' primary_assignable_expression
	/* | primary_assignable_expression '[' expression ']' */
	| primary_assignable_expression '[' ']'
	;

nucleus_primary_expression
	: constant
	| string
	;

primary_expression
	: IDENTIFIER
	/* | constant */
	/* | string */
	/* | INPUT '(' I_CONSTANT INLINE_RANGE I_CONSTANT ')' */
	/* | INPUT '(' I_CONSTANT ')' */
	/* | '(' expression ')' */
	/* | generic_selection */
	;

builin_primary_expression
	: INPUT '(' I_CONSTANT INLINE_RANGE I_CONSTANT ')'
	| INPUT '(' I_CONSTANT INLINE_RANGE I_CONSTANT ')' ':' type_specifier
	| INPUT '(' I_CONSTANT ')' ':' type_specifier
	| INPUT '(' I_CONSTANT ')'
	| REPEAT '(' primary_expression ',' expression ')'
	/* | GLUE '(' argument_expression_list ')' */
	/* | GLUE '(' glue_expression_list2 ')' */
	;

constant
	: I_CONSTANT		// includes character_constant
	| F_CONSTANT
	/* | ENUMERATION_CONSTANT	// after it has been defined as such */
	;


/* enumeration_constant		// before it has been defined as such
	: IDENTIFIER
	; */

string
	: STRING_LITERAL
	| FUNC_NAME
	;

/* generic_selection
	: GENERIC '(' assignment_expression ',' generic_assoc_list ')'
	; */

/* generic_assoc_list
	: generic_association
	| generic_assoc_list ',' generic_association
	; */

/* generic_association
	: type_name ':' assignment_expression
	| DEFAULT ':' assignment_expression
	; */

composite_primary_expression_without_braces
	: primary_expression
	/* | builin_primary_expression */
	/* | composite_primary_expression '[' expression ']' */
	| composite_primary_expression_without_braces '[' expression ']'
	/* | composite_primary_expression '(' argument_expression_list ')' */
	| composite_primary_expression_without_braces '.' IDENTIFIER // paraSL
	/* | postfix_expression PTR_OP IDENTIFIER */
	;

composite_primary_expression_postfix_increment
	: composite_primary_expression_without_braces INC_OP
	| composite_primary_expression_without_braces DEC_OP
	;

composite_primary_expression_prefix_increment
	: composite_primary_expression_postfix_increment
	/* | INC_OP composite_primary_expression_without_braces
	| DEC_OP composite_primary_expression_without_braces */
	;

prefix_postfix_statement
	: composite_primary_expression_prefix_increment
	;

composite_primary_expression
	: composite_primary_expression_without_braces
	| composite_primary_expression '(' initialyzer_list ')'
	;

single_expression
	: nucleus_primary_expression
	| composite_primary_expression
	| input_expression
	;

input_expression
	: INPUT '(' I_CONSTANT ')'
	| INPUT '(' I_CONSTANT ')' ':' type_specifier
	;

postfix_expression_or_single
	: single_expression
	| single_expression INC_OP
	| single_expression DEC_OP
	/* | '(' type_name ')' '{' initializer_list '}' */
	//| '(' type_name ')' '{' initializer_list ',' '}'
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression_or_single
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator premul_expression
	/* | SIZEOF unary_expression */
	/* | SIZEOF '(' type_name ')' */
	/* | ALIGNOF '(' type_name ')' */
	;

unary_operator
	: '+'
	| '-'
	| '~'
	| '!'
	/* : '&' */
	/* | '*' */
	;

cast_expression
	: unary_expression
	/* | '(' type_name ')' cast_expression */
	;

premul_expression
	: cast_expression
	| '(' expression ')'
	;

multiplicative_expression
	: premul_expression
	| multiplicative_expression '*' premul_expression
	| multiplicative_expression '/' premul_expression
	| multiplicative_expression '%' premul_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	/* | logical_or_expression '?' expression ':' conditional_expression */
	;

rval_expression
	: conditional_expression	// with constraints
	;

assignment_expression
	: rval_expression
	| builtin_rval_expression
	| '{' initialyzer_list '}'
	| '{' func_block_list '}'
	| '{' func_block_list '}' '(' initialyzer_list ')'
	| composite_primary_expression_without_braces assignment_operator assignment_expression
	/* |  unary_expression ... ... // так работает */
	;

builtin_rval_expression
	: glue_rval
	| repeat_rval
	| bind_rval
	;

repeat_rval
	: REPEAT '(' expression ',' expression ')'

glue_rval
	: GLUE '(' glue_initialyzer_list ')'
	| GLUE '(' glue_initialyzer_list2 ')'

glue_initialyzer_list
	: expression
	| glue_initialyzer_list ',' expression

initialyzer_list
	: expression
	| initialyzer_list ',' expression
	| %empty

glue_initialyzer_list2
	: expression ':' IDENTIFIER
	| glue_initialyzer_list2 ',' expression ':' IDENTIFIER

bind_rval
	: BIND '(' expression ',' expression ')'

func_arguments_decllist
	: IDENTIFIER
	| IDENTIFIER ':' type_specifier
	| func_arguments_decllist ',' IDENTIFIER
	| func_arguments_decllist ',' IDENTIFIER ':' type_specifier
	| %empty

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	/* | expression ',' assignment_expression */
	;

// decl statement
declaration
	: init_declarator_list ';'
	/* : declaration_specifiers ';' */
	/* | static_assert_declaration */
	;
	
/* int, double, ... */
declaration_specifiers
	: type_specifier
	/* : type_specifier declaration_specifiers */
	/* : storage_class_specifier declaration_specifiers */
	/* | storage_class_specifier */
	
	/* | type_qualifier declaration_specifiers */
	/* | type_qualifier */
	/* | function_specifier declaration_specifiers */
	/* | function_specifier */
	/* | alignment_specifier declaration_specifiers */
	/* | alignment_specifier */
	;

init_declarator_list
	: init_declarator
	/* | init_declarator_list ',' init_declarator */
	;

init_declarator
	: declarator_and_specifiers
	| declarator_and_specifiers '=' initializer
	;

declarator_and_specifiers
	: declarator
	/* | declarator ':' type_specifier */
	;

/* storage_class_specifier
	: TYPEDEF	// identifiers must be flagged as TYPEDEF_NAME
	| EXTERN
	| STATIC
	| THREAD_LOCAL
	| AUTO
	| REGISTER
	; */

basic_type_specifier
	: INT
	| INT '(' I_CONSTANT ')'
	| CHAR
	| FLOAT
	| DOUBLE
	;

common_type_specifier
	: basic_type_specifier
	| basic_type_specifier '[' expression ']'
	;
	
type_specifier
	: common_type_specifier
	| VECTOR '<' basic_type_specifier ',' I_CONSTANT '>'
	;

/* struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	; */

/* struct_or_union
	: STRUCT
	| UNION
	; */

/* struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	; */

/* struct_declaration
	: specifier_qualifier_list ';'	// for anonymous struct/union
	| specifier_qualifier_list struct_declarator_list ';'
	// | static_assert_declaration
	; */

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	/* | type_qualifier specifier_qualifier_list */
	/* | type_qualifier */
	;

/* struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	; */

/* struct_declarator
	: ':' constant_expression
	| declarator ':' constant_expression
	| declarator
	; */

/* enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: enumeration_constant '=' constant_expression
	| enumeration_constant
	; */

/* atomic_type_specifier
	: ATOMIC '(' type_name ')'
	; */

/* type_qualifier
	: CONST
	| RESTRICT
	| VOLATILE
	| ATOMIC
	; */

/* function_specifier
	: INLINE
	| NORETURN
	; */

/* alignment_specifier
	: ALIGNAS '(' type_name ')'
	| ALIGNAS '(' constant_expression ')'
	; */

declarator
	: direct_declarator
	/* : pointer direct_declarator */
	;

// var, array_elem, ...
direct_declarator:
	 /* IDENTIFIER {std::cout << "id" << std::endl;} */
	 IDENTIFIER ':' type_specifier // {std::cout << "here" << std::endl;}

	/* | '(' declarator ')' // ???? */
	/* | direct_declarator '[' ']' */
	/* | direct_declarator '[' '*' ']' */
	/* | direct_declarator '[' STATIC type_qualifier_list assignment_expression ']' */
	/* | direct_declarator '[' STATIC assignment_expression ']' */
	/* | direct_declarator '[' type_qualifier_list '*' ']' */
	/* | direct_declarator '[' type_qualifier_list STATIC assignment_expression ']' */
	/* | direct_declarator '[' type_qualifier_list assignment_expression ']' */
	/* | direct_declarator '[' type_qualifier_list ']' */
	/* | direct_declarator '[' assignment_expression ']' */
	| IDENTIFIER ':' '(' func_arguments_decllist ')' // for paraSL functions
	| IDENTIFIER ':' '(' func_arguments_decllist ')' ':' type_specifier // for paraSL functions
	| IDENTIFIER ':' '{' struct_arguments_decllist '}'
	/* | IDENTIFIER ':' '(' ')' // for paraSL functions */
	/* | IDENTIFIER ':' '(' ')' ':' type_specifier // for paraSL functions */
	/* | IDENTIFIER '(' identifier_list ')' // for paraSL functions usage */
	/* | IDENTIFIER '(' ')' // for paraSL functions usage */
	/* | IDENTIFIER ':' '{' identifier_list '}' */
	/* | implicit_declarator */
	;

struct_arguments_decllist
	: IDENTIFIER ':' type_specifier
	| IDENTIFIER ':' '(' func_arguments_decllist ')'
	| struct_arguments_decllist ',' IDENTIFIER ':' type_specifier
	| struct_arguments_decllist ',' IDENTIFIER ':' '(' func_arguments_decllist ')'

implicit_declarator
	: IDENTIFIER
	| IDENTIFIER '.' primary_assignable_expression
	| primary_assignable_expression '[' expression ']'
	;

function_arg_decl_list
	: function_arg_decl
	| function_arg_decl_list ',' function_arg_decl
	;

function_arg_decl
	: IDENTIFIER ':' type_specifier
	| IDENTIFIER
	;

function_parameter_usage_list
	:

/* pointer
	: '*' type_qualifier_list pointer
	| '*' type_qualifier_list
	| '*' pointer
	| '*'
	; */

/* type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	; */


parameter_type_list
	: parameter_list
	/* : parameter_list ',' ELLIPSIS */
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declarator
	/* : declaration_specifiers declarator */
	/* | declaration_specifiers abstract_declarator */
	/* | declaration_specifiers */
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

/* type_name
	: specifier_qualifier_list abstract_declarator
	| specifier_qualifier_list
	; */

/* abstract_declarator
	: direct_abstract_declarator
	: pointer direct_abstract_declarator
	| pointer
	; */

/* direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' '*' ']'
	//| '[' STATIC type_qualifier_list assignment_expression ']'
	//| '[' STATIC assignment_expression ']'
	//| '[' type_qualifier_list STATIC assignment_expression ']'
	//| '[' type_qualifier_list assignment_expression ']'
	//| '[' type_qualifier_list ']'
	| '[' assignment_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' '*' ']'
	//| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	//| direct_abstract_declarator '[' STATIC assignment_expression ']'
	//| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
	//| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	//| direct_abstract_declarator '[' type_qualifier_list ']'
	| direct_abstract_declarator '[' assignment_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	; */

initializer:
	/* : '{' initializer_list '}' */
	/* | '{' initializer_list ',' '}' */
	  assignment_expression
	;

/* initializer_list
	: designation initializer
	| initializer
	| initializer_list ',' designation initializer
	| initializer_list ',' initializer
	; */

/* designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' constant_expression ']'
	| '.' IDENTIFIER
	; */

/* static_assert_declaration
	: STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';'
	; */

statement:
	/* labeled_statement */
	 compound_statement
	| expression_statement
	/* | selection_statement */
	/* | iteration_statement */
	/* | jump_statement */
	;

labeled_statement
	: IDENTIFIER ':' statement
	/* | CASE constant_expression ':' statement */
	/* | DEFAULT ':' statement */
	;

compound_statement
	: '{' '}'
	| '{'  block_item_list '}'
	;

block_item_list
	: block_item 
	| block_item_list block_item
	;

/* =====================================================================*/
block_item
	: composite_primary_expression_without_braces assignment_operator assignment_expression ';'
	| rval_expression ';'
	/* | cast_expression ';' */
	/* | prefix_postfix_statement ';' */
	| direct_declarator '=' assignment_expression ';'
	| direct_declarator ';'
	| selection_statement
	| iteration_statement
	| OUTPUT '(' I_CONSTANT ',' expression ')' ';'
	| ';'
	/* : declaration */
	/* | statement */
	;

func_block_item
	: block_item
	| return_statement
	;

func_block_list
	: func_block_item
	| func_block_list func_block_item
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' selection_body ELSE selection_body
	| IF '(' expression ')' selection_body %prec THEN
	/* | SWITCH '(' expression ')' statement */
	;

selection_body
	: '{' func_block_list '}'
	| '{' '}'
	| func_block_item
	;

iteration_statement
	: WHILE '(' expression ')' iteration_body
	/* | DO statement WHILE '(' expression ')' ';' */
	/* | FOR '(' expression_statement expression_statement ')' statement */
	/* | FOR '(' expression_statement expression_statement expression ')' statement */
	/* | FOR '(' declaration expression_statement ')' statement */
	/* | FOR '(' declaration expression_statement expression ')' statement */
	| FOR '(' for_iterator IN for_range ')' iteration_body
	;

for_iterator
	: IDENTIFIER ':' basic_type_specifier
	| IDENTIFIER
	;

for_range
	: I_CONSTANT ':' I_CONSTANT ':' I_CONSTANT
	| I_CONSTANT ':' I_CONSTANT
	| IDENTIFIER
	;

iteration_body
	: selection_body
	;

jump_statement
	: RETURN expression ';'
	/* : RETURN ';' */
	/* : GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';' */
	;

return_statement
	: RETURN expression ';'
	;
/* =============================================================== */
translation_unit
	: layer_definition_list
	| block_item_list
	;

external_declaration:
	/* function_definition */
	/*| declaration */
	layer_definition
	;

layer_definition_list	
	: layer_definition
	| layer_definition_list layer_definition

layer_definition:
	LAYER '(' I_CONSTANT ',' STRING_LITERAL ')' compound_statement
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

/* GLUUUUUE */
/*
composite_primary_expression_without_braces_glue
	: primary_expression
	| composite_primary_expression_without_braces_glue '[' expression ']'
	| composite_primary_expression_without_braces_glue '.' IDENTIFIER // paraSL
	;

composite_primary_expression_postfix_increment_glue
	: composite_primary_expression_without_braces_glue INC_OP
	| composite_primary_expression_without_braces_glue DEC_OP
	;

composite_primary_expression_prefix_increment_glue
	: composite_primary_expression_postfix_increment_glue
	// | INC_OP composite_primary_expression_without_braces
	// | DEC_OP composite_primary_expression_without_braces 
	;

prefix_postfix_statement_glue
	: composite_primary_expression_prefix_increment_glue
	;

composite_primary_expression_glue
	: composite_primary_expression_without_braces_glue
	| composite_primary_expression_glue '(' initialyzer_list ')'
	;

single_expression_glue
	: nucleus_primary_expression
	| composite_primary_expression_glue
	| input_expression_glue
	;

input_expression_glue
	: INPUT '(' I_CONSTANT ')' ':' type_specifier

postfix_expression_or_single_glue
	: single_expression_glue INC_OP
	| single_expression_glue DEC_OP
	//| '(' type_name ')' '{' initializer_list '}' 
	//| '(' type_name ')' '{' initializer_list ',' '}'
	;

unary_expression_glue
	: postfix_expression_or_single_glue
	| INC_OP unary_expression_glue
	| DEC_OP unary_expression_glue
	| unary_operator premul_expression_glue
	;

cast_expression_glue
	: unary_expression_glue
	//| '(' type_name ')' cast_expression
	;

premul_expression_glue
	: cast_expression_glue
	// | '(' expression_glue ')'
	;

multiplicative_expression_glue
	: premul_expression_glue
	| multiplicative_expression_glue '*' premul_expression_glue
	| multiplicative_expression_glue '/' premul_expression_glue
	| multiplicative_expression_glue '%' premul_expression_glue
	;

additive_expression_glue
	: multiplicative_expression_glue
	| additive_expression_glue '+' multiplicative_expression_glue
	| additive_expression_glue '-' multiplicative_expression_glue
	;

shift_expression_glue
	: additive_expression_glue
	| shift_expression_glue LEFT_OP additive_expression_glue
	| shift_expression_glue RIGHT_OP additive_expression_glue
	;

relational_expression_glue
	: shift_expression_glue
	| relational_expression_glue '<' shift_expression_glue
	| relational_expression_glue '>' shift_expression_glue
	| relational_expression_glue LE_OP shift_expression_glue
	| relational_expression_glue GE_OP shift_expression_glue
	;

equality_expression_glue
	: relational_expression_glue
	| equality_expression_glue EQ_OP relational_expression_glue
	| equality_expression_glue NE_OP relational_expression_glue
	;

and_expression_glue
	: equality_expression_glue
	| and_expression_glue '&' equality_expression_glue
	;

exclusive_or_expression_glue
	: and_expression_glue
	| exclusive_or_expression_glue '^' and_expression_glue
	;

inclusive_or_expression_glue
	: exclusive_or_expression_glue
	| inclusive_or_expression_glue '|' exclusive_or_expression_glue
	;

logical_and_expression_glue
	: inclusive_or_expression_glue
	| logical_and_expression_glue AND_OP inclusive_or_expression_glue
	;

logical_or_expression_glue
	: logical_and_expression_glue
	| logical_or_expression_glue OR_OP logical_and_expression_glue
	;

conditional_expression_glue
	: logical_or_expression_glue
	;

rval_expression_glue
	: conditional_expression_glue	// with constraints
	;

assignment_expression_glue
	: rval_expression_glue
	// | builtin_rval_expression
	// | '{' initialyzer_list '}'
	// | '{' func_block_list '}'
	// | '{' func_block_list '}' '(' initialyzer_list ')'
	| composite_primary_expression_without_braces_glue assignment_operator assignment_expression_glue
	// |  unary_expression ... ... // так работает 
	;

	
expression_glue
	: assignment_expression_glue
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