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

%token
  DEBUG_TOKEN "alex_great"
  EQUAL
  MINUS
  PLUS
  MUL
  DIV
  AND
  OR
  COLON
  SCOLON
  COMMA
  DOT
  RANGE
  ERR

  IF
  ELSE
  WHILE
  FOR

  LOGIC_AND
  LOGIC_INV
  LOGIC_OR
  LOGIC_XOR

  BIT_SHIFT_LEFT
  BIT_SHIFT_RIGHT

  NOT

  FLOAT_NUMBER
  
  EQ
  NOTEQ
  GE
  LE
  G_BRACKET
  L_BRACKET
  
  LINE_COMMENT
  
  INT
  FLOAT
  DOUBLE
  EXTERN_INPUT
  REPEAT
  GLUE
  BIND
  RETURN
  VECTOR

  CURVED_BRACKET_LEFT
  CURVED_BRACKET_RIGHT
  SQUARE_BRACKET_LEFT
  SQUARE_BRACKET_RIGHT
  ROUND_BRACKET_LEFT
  ROUND_BRACKET_RIGHT
;

%token <int> INT_NUMBER
%token <const char*> VARNAME
%token <char> CHAR

%left PLUS MINUS
%left MUL DIV
%left NEG //

%left LOGIC_AND
%left LOGIC_OR
%left LOGIC_XOR
%left LOGIC_INV
%left LOGIC_NEG //

%left NOT
%left COND_NEG // 

%left AND
%left OR
%left EQ

%left NOTEQ
%left GE
%left LE
%left G_BRACKET
%left L_BRACKET
%left BIT_SHIFT_LEFT
%left BIT_SHIFT_RIGHT

%left EQUAL
%left COLON

%start program

%%

program : statements
;

/* scope & statements */
scope : CURVED_BRACKET_LEFT statements CURVED_BRACKET_RIGHT statements
;
single_scope : CURVED_BRACKET_LEFT statements CURVED_BRACKET_RIGHT
             | statement
;

statements : %empty
           | statement statements
           | scope
;

statement : DEBUG_TOKEN SCOLON
          | var_decl SCOLON
          | func_decl SCOLON
          | array_decl SCOLON
       //   | decl_rval_expression SCOLON
          | if_statement
          | else_statement
          | while_statement
          | return_statement SCOLON
          | struct_decl SCOLON
;

// basic types & elements
basic_type :  CHAR
           |  INT_NUMBER
           |  FLOAT_NUMBER
           |  DOUBLE
;

lval :    VARNAME
        | VARNAME SQUARE_BRACKET_LEFT right_expression SQUARE_BRACKET_RIGHT
        | struct_field_lval
;

rval : basic_rval
     | array_elem_rval
     | func_usage_rval
     | method_rval
     | struct_field_rval
;

basic_rval : INT_NUMBER
           | FLOAT_NUMBER
           | CHAR
           | DOUBLE
;

// expressions
right_expression: rval_expression
//| ROUND_BRACKET_LEFT decl_right_expression ROUND_BRACKET_RIGHT
//| ROUND_BRACKET_LEFT conditions ROUND_BRACKET_RIGHT
//| decl_right_expression PLUS rval_expression
;

rval_expression: rval
| decl_rval_expression
| rval_expression PLUS rval_expression
| rval_expression MINUS rval_expression
| rval_expression MUL rval_expression
| rval_expression DIV rval_expression
| MINUS rval_expression %prec NEG
| rval_expression LOGIC_XOR rval_expression
| rval_expression LOGIC_OR rval_expression
| rval_expression LOGIC_AND rval_expression
| LOGIC_INV rval_expression %prec LOGIC_NEG
| rval_expression EQ rval_expression
| rval_expression NOTEQ rval_expression
| rval_expression AND rval_expression 
| rval_expression OR rval_expression
| rval_expression L_BRACKET rval_expression
| rval_expression G_BRACKET rval_expression
| rval_expression GE rval_expression
| rval_expression LE rval_expression
| NOT rval_expression %prec COND_NEG
| ROUND_BRACKET_LEFT rval_expression ROUND_BRACKET_RIGHT
;

decl_rval_expression : lval EQUAL rval_expression
 

/*
1.2. Types

Every entity introduced by its first mention. Entity have associated type that is either specified or deduced.
Special input and output are generic IO entities.
You may imagine input(0) as a typed stdin and output(0) as a typed stdout.
Definite meaning of those are given at layer 3.

v0 = 0;
v1 : double = v0;
v2 = input(0) : int; // not input(v0)
v3 : int;
output(0, v1);

For int type you may directly specify size.

v0 : int(16) = 0; // 16-bit int

Default int is 32.

Integer are two's complement signed numbers with two's complement signed wrap. No UB here.
*/

// var declarations
var_decl : VARNAME COLON var_type EQUAL right_expression
         | VARNAME COLON var_type
         | VARNAME EQUAL builtin_input_single_rval
         | VARNAME EQUAL builtin_input_single_rval_with_type
         //| VARNAME EQUAL right_expression
;

var_type:  basic_type
        |  INT ROUND_BRACKET_LEFT INT_NUMBER ROUND_BRACKET_RIGHT
;



/*
1.5. Functions

Function entity is an abstraction mechanism. It is a callable.

f0 = { v3 = input(0) + 1; v3 * v3; };
v7 = f0(2); // but not f0(2, 3)
f1 : (x) = { v3 = x + 1; v3 * v3; };
f2 : (x, y) = { v3 = x + y; v3 * v3; };
f3 : (x : int, y : double) : int = { v3 = x + y; v3 * v3; };
v8 = f3(v0, v1);

Normal function return is an entity generated by the last expression.
*/

// function declarations
func_decl : VARNAME func_args_decl EQUAL func_body
          | VARNAME EQUAL func_body
;

func_body : CURVED_BRACKET_LEFT statements CURVED_BRACKET_RIGHT
            statement
;

func_args_decl : COLON ROUND_BRACKET_LEFT func_args_decl_list ROUND_BRACKET_RIGHT COLON basic_type
               | COLON ROUND_BRACKET_LEFT func_args_decl_list ROUND_BRACKET_RIGHT
;

func_args_decl_list : func_arg_decl
                    | func_arg_decl COMMA func_args_decl_list
;

func_arg_decl : VARNAME COLON basic_type
              | VARNAME
;

// function usage
func_usage_rval : VARNAME ROUND_BRACKET_LEFT func_args_usage_list ROUND_BRACKET_RIGHT
;

func_args_usage_list : func_arg_usage
                     | func_arg_usage COMMA func_args_usage_list
                     | %empty
;

func_arg_usage : func_arg_usage_expression
;

func_arg_usage_expression : right_expression
;

// return statement
return_statement : RETURN right_expression

// builtin statements
// glue
glue_rval : GLUE glue_args_decl
;

glue_args_decl: ROUND_BRACKET_LEFT glue_args_decl_list ROUND_BRACKET_RIGHT
;

glue_args_decl_list : glue_arg_decl
                    | glue_arg_decl COMMA glue_args_decl_list
;

glue_right_expression : right_expression
                      | builtin_bind_rval
;

glue_arg_decl : glue_right_expression COLON VARNAME
              | glue_right_expression
;

// input()
builtin_input_single_rval : EXTERN_INPUT ROUND_BRACKET_LEFT INT_NUMBER ROUND_BRACKET_RIGHT
;
builtin_input_single_rval_with_type : EXTERN_INPUT ROUND_BRACKET_LEFT INT_NUMBER ROUND_BRACKET_RIGHT COLON basic_type
;

builtin_input_array_rval : EXTERN_INPUT ROUND_BRACKET_LEFT INT_NUMBER RANGE INT_NUMBER ROUND_BRACKET_RIGHT
;
builtin_input_array_rval_with_type : EXTERN_INPUT ROUND_BRACKET_LEFT INT_NUMBER RANGE INT_NUMBER ROUND_BRACKET_RIGHT COLON array_basic_type
;

// output()

// bind()
builtin_bind_rval : BIND ROUND_BRACKET_LEFT VARNAME COMMA struct_fields_list ROUND_BRACKET_RIGHT
                  | BIND ROUND_BRACKET_LEFT VARNAME ROUND_BRACKET_RIGHT
;

// repeat()
builtin_repeat_rval : REPEAT ROUND_BRACKET_LEFT VARNAME COMMA right_expression ROUND_BRACKET_RIGHT
;

// vector<>
builtin_vector_rval : VECTOR L_BRACKET basic_type COMMA INT_NUMBER G_BRACKET
;

struct_fields_list: struct_field_lval
                  | struct_field_lval COMMA struct_fields_list
;

/*
1.3. Arrays

Array entity is syntactic glue for the couple of entities of the same type. Array have CT-known size.
Special arryas are vectors. Vectors have both CT-known size and CT-known indexes.

arr0 = repeat(v0, 5); // array of 5
arr1 : int[5] = {1, 2, 3, 4}; // array of 5
arr2 : vector<int, 5> = {1, 2, 3, 4, 5}; // vector
v3 = 3;
v4 = arr0[v3];
v5 = arr2[3]; // not arr2[v3]
arr_inputs = input(0..3) : int[4]; // array of 4
*/

// array declarations
array_decl: VARNAME COLON array_type EQUAL array_body
          | VARNAME EQUAL array_body
;

/*
array_elem_lval : VARNAME SQUARE_BRACKET_LEFT right_expression SQUARE_BRACKET_RIGHT EQUAL right_expression
                | VARNAME SQUARE_BRACKET_LEFT right_expression SQUARE_BRACKET_RIGHT EQUAL builtin_input_single_rval
;
*/

array_elem_rval : VARNAME SQUARE_BRACKET_LEFT right_expression SQUARE_BRACKET_RIGHT

array_type: array_basic_type
          | builtin_vector_rval
;

array_basic_type : basic_type SQUARE_BRACKET_LEFT INT_NUMBER SQUARE_BRACKET_RIGHT 
;

array_body : CURVED_BRACKET_LEFT array_list CURVED_BRACKET_RIGHT
           | builtin_repeat_rval
           | builtin_input_array_rval
           | builtin_input_array_rval_with_type
;

array_list : basic_rval COMMA array_list
           | basic_rval
;

// structures

struct_field_lval : VARNAME DOT VARNAME
struct_field_rval : VARNAME DOT VARNAME

struct_decl_rval : glue_rval

struct_decl : VARNAME struct_args_decl EQUAL struct_decl_rval
            | VARNAME struct_args_decl
            | VARNAME EQUAL struct_decl_rval
;

struct_args_decl : COLON CURVED_BRACKET_LEFT struct_args_decl_list CURVED_BRACKET_RIGHT
;

struct_args_decl_list : struct_arg_decl
                    | struct_arg_decl COMMA struct_args_decl_list
;

struct_arg_decl : VARNAME COLON basic_type
                | VARNAME COLON array_basic_type
                | VARNAME COLON func_args_decl // for method
;

// method usage
method_rval : VARNAME DOT VARNAME ROUND_BRACKET_LEFT func_args_usage_list ROUND_BRACKET_RIGHT
;

// conditions

if_statement: IF ROUND_BRACKET_LEFT conditions ROUND_BRACKET_RIGHT condition_body
;


else_statement: ELSE condition_body
;

while_statement : WHILE ROUND_BRACKET_LEFT conditions ROUND_BRACKET_RIGHT condition_body

conditions : rval 
           | rval condition_operator conditions
           | conditional_scope
;

condition_operator: L_BRACKET
                    | G_BRACKET
                    | GE
                    | LE
                    | AND
                    | OR
                    | EQ
                    | NOTEQ
;

conditional_scope : ROUND_BRACKET_LEFT conditions ROUND_BRACKET_RIGHT
;

condition_body: single_scope
;

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