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
/* %define api.value.type variant */
%define api.value.type { myType }
%param { yy::LexerDriver* driver }

%code requires
{
#include <iostream>
#include <string>
#include <utility>
#include "AST/TreeNode.hpp"
#include "AST/Expr.hpp"
// forward decl of argument to parser
namespace yy { class LexerDriver; }

struct myType {
	wsheeet::ast::TreeNodeBase* m_node{nullptr};
	enum class NodeT {
		UDF,
		INT_LITERAL,
		DOUBLE_LITERAL,
		FLOAT_LITERAL,
		UNOP_EXPRESSION,
		BINOP_EXPRESSION,
		EXPRESSION_STATEMENT,
		COMPOUND_STATEMENT,
		LAYER_DECL,
		GLOBL_SCOPE,
	};

	NodeT m_nodeType{NodeT::UDF};


	int a;
	int b;
	void hello() {
		std::cout << "HELLLLLO" << std::endl;
		/* std::cerr << yylval->as<std::string>(); */
		}
	int i_const{200000000};
	float f_const{12222.}; 

};

}

%code
{
#include "lexerdriver.hpp"

namespace yy {

namespace ast = wsheeet::ast; 

parser::token_type yylex(parser::semantic_type* yylval,                         
                         LexerDriver* driver);
}
}
%token ERR
%token	IDENTIFIER I_CONSTANT F_CONSTANT STRING_LITERAL FUNC_NAME
%token  INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME

%token COLON_INT
%token COLON_CHAR
%token COLON_FLOAT
%token COLON_DOUBLE
%token COLON_VECTOR_LESS
%token COLON_VARNAME
%token CHAR
%token INT
%token FLOAT
%token DOUBLE
%token IF ELSE THEN
%token WHILE
%token FOR 
%token RETURN

%token INLINE_RANGE LAYER INPUT OUTPUT REPEAT GLUE BIND VECTOR IN

%start translation_unit

// for x.x & x.x() parcing
/* %precedence IDENTIFIER */
/* %precedence ':' */
/* %precedence '(' */
/* %left IDENTIFIER */

// for fixing conflicts in direct_declarator
/* %left ':' */

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
%precedence ')'
%precedence ':'

%precedence INT
%precedence CHAR
%precedence FLOAT
%precedence DOUBLE
%precedence VECTOR
%precedence IDENTIFIER

%token DEBUG_LEXEM

%%

translation_unit
	: layer_definition_list { std::cout << &(driver->m_builder) << std::endl; }
	| block_item_list
	;

layer_definition_list	
	: layer_definition
	| layer_definition_list layer_definition

layer_definition:
	LAYER '(' I_CONSTANT ',' STRING_LITERAL ')' compound_statement
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
	: general_statement
	| explicit_decl_statement
	| selection_statement { $$.hello(); }
	| iteration_statement
	| output_statement
	| empty_statement
	;

general_statement
	: composite_primary_expression_without_braces assignment_operator assignment_expression_rval ';'
	| rval_expression ';'
	;

/* // for array idx
explicit_scalar_decl_expression
	: scalar_direct_declarator '=' scalar_assignment_expression
	| scalar_direct_declarator
	;

// for array idx
scalar_direct_declarator
	: IDENTIFIER colon_scalar_type_specifier
	; */

explicit_decl_expression
	: direct_declarator '=' assignment_expression_rval
	| direct_declarator

explicit_decl_statement
	: explicit_decl_expression ';'
	;

direct_declarator
	: basic_declarator // {std::cout << "here" << std::endl;}
	| function_declarator
	| struct_declarator
	;

basic_declarator
	: IDENTIFIER colon_type_specifier
	;

function_declarator
	: IDENTIFIER ':' '(' func_arguments_decllist ')' // for paraSL functions
	| IDENTIFIER ':' '(' func_arguments_decllist ')' colon_type_specifier // for paraSL functions
	;

struct_declarator
	: IDENTIFIER ':' '{' struct_arguments_decllist '}'
	;

struct_arguments_decllist
	: IDENTIFIER colon_type_specifier
	| IDENTIFIER ':' '(' func_arguments_decllist ')'
	| struct_arguments_decllist ',' IDENTIFIER colon_type_specifier
	| struct_arguments_decllist ',' IDENTIFIER ':' '(' func_arguments_decllist ')'
	;

func_block_item
	: block_item
	| return_statement
	;

func_block_list
	: func_block_item
	| func_block_list func_block_item
	;

output_statement
	: OUTPUT '(' I_CONSTANT ',' expression ')' ';'
	;

empty_statement
	: ';'
	;

parrot
	: child { $$.a = $1.a; $$.b = 3; }
	;

child
	: DEBUG_LEXEM { $$.a = 4; }
	;

literal_primary_expression
	: constant_literal
	| STRING_LITERAL
	/* | FUNC_NAME */
	;

primary_expression
	: IDENTIFIER
	;

constant_literal
	: I_CONSTANT	{
						auto &type = driver->m_builder.get<ast::IntTy>();
						auto *Imm = new ast::ConstSimpleValueExpr<ast::IntTy>{type, $1.i_const};
						// $$.m_node = dynamic_cast<ast::TreeNodeBase*>(Imm);
						$$.m_node = dynamic_cast<ast::ExprBase*>($1.m_node);
						// $$.m_nodeType = myType::NodeT::INT_LITERAL;
					}	// includes character_constant
	| F_CONSTANT
	;

// used in function params
argument_list
	: argument_expression
	| argument_list ',' argument_expression
	| %empty
	;

// used in function params
argument_expression
	: expression
	;

composite_primary_expression_without_braces
	: primary_expression
	| composite_primary_expression_without_braces '[' expression_in_square_brackets ']'
	| composite_primary_expression_without_braces '.' IDENTIFIER
	;

expression_in_square_brackets
	: expression
	;

composite_primary_expression
	: composite_primary_expression_without_braces
	| composite_primary_expression '(' argument_list ')'
	;

single_expression
	: literal_primary_expression
	| composite_primary_expression
	| input_expression
	;

input_expression
	: INPUT '(' I_CONSTANT ')'
	| INPUT '(' I_CONSTANT ')' colon_type_specifier
	| INPUT '(' I_CONSTANT INLINE_RANGE I_CONSTANT ')' colon_array_type_specifier
	| INPUT '(' I_CONSTANT INLINE_RANGE I_CONSTANT ')' colon_vector_type_specifier
	;

postfix_expression_or_single
	: single_expression
	| single_expression INC_OP
	| single_expression DEC_OP
	;

unary_expression
	: postfix_expression_or_single
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator premul_expression
	;

unary_operator
	: '+'
	| '-'
	| '~'
	| '!'
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
	| additive_expression '+' multiplicative_expression {
															auto &type = driver->m_builder.get<ast::IntTy>();
															// auto *LHS = dynamic_cast<ast::ExprBase*>($1.m_node);
															// auto *RHS = dynamic_cast<ast::ExprBase*>($3.m_node);
															auto *LHS = new ast::ExprBase{type};
															auto *RHS = new ast::ExprBase{type};
															auto *Expr = new ast::BinOpExpr{*LHS, *RHS, ast::BinOpcode::PLUS};

														}
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
	;

rval_expression
	: conditional_expression	// with constraints
	;

/* scalar_assignment_expression
	: rval_expression
	| composite_primary_expression_without_braces assignment_operator scalar_assignment_expression
	; */

assignment_expression_rval
	: rval_expression
	| builtin_rval_expression
	| agregate_rval
	| function_decl_rval
	| inline_function_rval
	| composite_primary_expression_without_braces assignment_operator assignment_expression_rval
	;

function_decl_rval
	: '{' func_block_list '}'
	;

agregate_rval
	: '{' argument_list '}'
	;

inline_function_rval
	: '{' func_block_list '}' '(' argument_list ')'
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
	| GLUE '(' glue_initialyzer_list3 ')'
	;

glue_initialyzer_list
	: expression
	| glue_initialyzer_list ',' expression
	;

glue_initialyzer_list3
	: expression COLON_VARNAME
	| glue_initialyzer_list3 ',' expression COLON_VARNAME

bind_rval
	: BIND '(' expression ',' expression ')'

func_arguments_decllist
	: IDENTIFIER
	| IDENTIFIER colon_type_specifier
	| func_arguments_decllist ',' IDENTIFIER
	| func_arguments_decllist ',' IDENTIFIER colon_type_specifier
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
	: assignment_expression_rval { std::cout << "heeerre" << std::endl;  $$.b = 4; }
	;

basic_type_specifier
	: INT
	| INT '(' I_CONSTANT ')'
	| CHAR
	| FLOAT
	| DOUBLE
	;

colon_scalar_type_specifier
	: COLON_INT
	| COLON_INT '(' I_CONSTANT ')'
	| COLON_CHAR
	| COLON_FLOAT
	| COLON_DOUBLE
	;

colon_common_type_specifier
	: colon_scalar_type_specifier
	| colon_array_type_specifier 
	;

colon_array_type_specifier
	: colon_scalar_type_specifier '[' I_CONSTANT ']'
	;

colon_vector_type_specifier
	: COLON_VECTOR_LESS basic_type_specifier ',' I_CONSTANT '>'
	;
	
colon_type_specifier
	: colon_common_type_specifier
	| COLON_VECTOR_LESS basic_type_specifier ',' I_CONSTANT '>'
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
	| FOR '(' for_iterator IN for_range ')' iteration_body
	;

for_iterator
	: IDENTIFIER colon_scalar_type_specifier /* may be errors */
	| IDENTIFIER
	;

for_range
	: expression ':' conditional_expression ':' expression
	| expression for_cond_expression for_expression
	| expression ':' conditional_expression
	| expression for_cond_expression
	| expression
	/* : I_CONSTANT ':' I_CONSTANT ':' I_CONSTANT */
	/* | I_CONSTANT ':' I_CONSTANT */
	/* | IDENTIFIER */
	/* | IDENTIFIER COLON_VARNAME_CURVED_BRACKET */
	/* | IDENTIFIER ':' IDENTIFIER COLON_VARNAME_CURVED_BRACKET */
	;

for_expression
	: for_cond_expression
	| COLON_VARNAME for_assign_symbol expression
	;

for_cond_expression
	: COLON_VARNAME
	| COLON_VARNAME for_conditional_symbol expression
	/* | COLON_VECTOR_LESS expression */

for_assign_symbol
	: ADD_ASSIGN
	| SUB_ASSIGN
	| XOR_ASSIGN
	| AND_ASSIGN
	| OR_ASSIGN
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	;

for_binary_symbol
	: '+'
	| '-'
	| '*'
	| '%'
	| '/'
	| LEFT_OP
	| RIGHT_OP
	| '&'
	| '^'

for_conditional_symbol
	: NE_OP
	| EQ_OP
	| LE_OP
	| GE_OP
	| '<'
	| '>'
	| AND_OP
	| OR_OP

iteration_body
	: selection_body
	;

return_statement
	: RETURN expression ';'
	;
/* =============================================================== */

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