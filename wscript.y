/*------------------------------------------------------------------------------
|
| Copyright (c), 2017 mwiede Software
|
| Inspirated from EASY interpeter (telos GmbH, Hamburg/Germany)
|
+-------------------------------------------------------------------------------
|
| Name of the Project: WScript
|
| File: wscript.y
|
| Author(s): M.W.
|
+-----------------------------------------------------------------------------*/
%define api.pure full
%locations
%lex-param {yyscan_t  scanner}
%parse-param {yyscan_t  scanner}
%parse-param {ExpressionList** pexplist}
%{
#include <stdio.h>
#include "ast.h"
#include "wscript.h"
extern "C"
{
typedef void* yyscan_t;
struct YYLTYPE;
int yywrap(yyscan_t yyscanner) { return 1; }
extern int yyparse(yyscan_t scanner, ExpressionList** pexplist);
extern void yyerror(YYLTYPE *location, yyscan_t scanner, ExpressionList** pexplist, const char* s);

using namespace std;
}
#define YYERROR_VERBOSE 1
#define WSCRIPT_LINE_FILE (yyFileIdx<<24)|yyLine
#define WSCRIPT_LINE_FILE2(line) (yyFileIdx<<24)|line
extern int yylex(union YYSTYPE *yylval_param, YYLTYPE *location, yyscan_t scanner);
extern int yydebug;
extern WSCRIPT_THREAD_SAFE unsigned int yyLine;
extern WSCRIPT_THREAD_SAFE unsigned int yyFileIdx;
extern int func_bool_or (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx);
extern int func_bool_and (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx);


%}
%token FOR FOREACH AS DO WHILE SWITCH CASE DEFAULT FUNCTION LIST EQUAL EQUAL2 IF ELSE RETURN BREAK CONTINUE ARRAY ARRAY_ASSIGN GLOBAL CONSTANT
%token <pstr> NAME MAGIC_NAME VALUE_NUM VALUE_ONUM VALUE_HNUM VALUE_BNUM VALUE_EXP_DNUM VALUE_STR VALUE_BOOL VALUE_NULL VALUE_OUTPUT VALUE_UNKNOWN
%left '=' PLUS_EQUAL PLUSSTR_EQUAL MINUS_EQUAL MUL_EQUAL DIV_EQUAL MOD_EQUAL POW_EQUAL BIN_AND_EQUAL BIN_OR_EQUAL BIN_XOR_EQUAL BIN_SHIFTL_EQUAL BIN_SHIFTR_EQUAL
%left '?' ':'
%left BOOL_OR
%left BOOL_AND
%left BIN_OR
%left BIN_XOR
%left '&'
%nonassoc EQUAL EQUAL2
%nonassoc '>' '<'
%nonassoc CMP_LE CMP_GE CMP_NE CMP_NE2
%left BIN_SHIFTL BIN_SHIFTR 
%left '-' '+' '.'     /* left associative and lowest precedence */
%left '*' '/' '%'      /* higher level than + and - */
%right '!'
%right '~' INC DEC
%right POW
%nonassoc UMINUS   /* no associativity for unariy - sign but hightest level */

%union {
ExpressionList* pexplist;
Expression* pexp;
string* pstr;
}
%type <pexplist> statements
%type <pexplist> compound_statement 
%type <pexplist> compound_statements 
%type <pexplist> universal_statements
%type <pexplist> program_statements
%type <pexplist> if_statement 
%type <pexplist> argument_list
%type <pexplist> argument_list_or_empty
%type <pexplist> param_list
%type <pexplist> param_list_or_empty
%type <pexplist> index_list
%type <pexplist> switch_case_list
%type <pexplist> variable
%type <pexplist> variable_list
%type <pexplist> variable_def_list
%type <pexplist> list_param
%type <pexplist> list_list
%type <pexplist> array_value
%type <pexplist> array_list
%type <pexp> expression
%type <pexp> param
%type <pexp> variable_def
%type <pexp> block_expression
%type <pexp> statement
%type <pexp> switch_case
%type <pexp> constant
%%
program : program_statements
{
	*pexplist = $1;
};

expression  : expression '+' expression {
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_plus", a, "", 0, WSCRIPT_LINE_FILE); $$=e;
}
             | expression '.' expression {
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_plusstr", a, "", 0, WSCRIPT_LINE_FILE); $$=e;
}
             | expression '-' expression {
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_minus", a, "", 0, WSCRIPT_LINE_FILE); $$=e;
}
             | expression '*' expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_mul", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
			| expression POW expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_pow", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression '/' expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_div", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression '%' expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_mod", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | CONSTANT NAME '=' expression { 
ExpressionList a; a.push_back(*$4); delete $4; Expression* e = new Expression("=", a, *$2, -1, WSCRIPT_LINE_FILE); delete $2; $$=e; 
}
             | NAME '=' expression { 
ExpressionList a; a.push_back(*$3); delete $3; Expression* e = new Expression("=", a, *$1, 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | NAME index_list '=' expression { 
int indexCnt = $2->size (); $2->push_back(*$4); delete $4; Expression* e = new Expression("=", *$2, *$1, 10+indexCnt, WSCRIPT_LINE_FILE); delete $1; delete $2; $$=e; 
}
             | variable PLUS_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_plus", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable PLUSSTR_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_plusstr", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable MINUS_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_minus", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable MUL_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_mul", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable POW_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_pow", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable DIV_EQUAL expression { 
$1->push_back(*$3); delete $3;  Expression* e = new Expression("=_div", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable MOD_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_mod", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable BIN_AND_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_bin_and", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable BIN_OR_EQUAL expression { 
$1->push_back(*$3); delete $3;Expression* e = new Expression("=_bin_or", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable BIN_XOR_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_bin_xor", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable BIN_SHIFTL_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_bin_shiftl", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable BIN_SHIFTR_EQUAL expression { 
$1->push_back(*$3); delete $3; Expression* e = new Expression("=_bin_shiftr", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | variable INC { 
ExpressionList a; Expression* e = new Expression("=_inc", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1;  $$=e; 
}
             | INC variable { 
ExpressionList a; Expression* e = new Expression("=_inc", *$2, "", -1, WSCRIPT_LINE_FILE); delete $2;  $$=e; 
}
             | variable DEC { 
ExpressionList a; Expression* e = new Expression("=_dec", *$1, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
             | DEC variable { 
ExpressionList a; Expression* e = new Expression("=_dec", *$2, "", -1, WSCRIPT_LINE_FILE); delete $2; $$=e; 
}
             | '!' expression {
ExpressionList a;  a.push_back(*$2);  delete $2;  Expression* e = new Expression("_not", a, "", 0, WSCRIPT_LINE_FILE);  $$=e; 
}
             | expression EQUAL expression {
ExpressionList a;  a.push_back(*$1);  a.push_back(*$3);  delete $1;  delete $3;  Expression* e = new Expression("_equal", a, "", 0, WSCRIPT_LINE_FILE);  $$=e; 
}
             | expression EQUAL2 expression {
ExpressionList a;  a.push_back(*$1);  a.push_back(*$3);  delete $1;  delete $3;  Expression* e = new Expression("_equal2", a, "", 0, WSCRIPT_LINE_FILE);  $$=e; 
}
             | expression '>' expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_greater", a, "", 0, WSCRIPT_LINE_FILE); $$=e;
}
             | expression '<' expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_less", a, "", 0, WSCRIPT_LINE_FILE); $$=e;
}
             | expression CMP_GE expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_cmp_ge", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression CMP_NE expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_cmp_ne", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression CMP_NE2 expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_cmp_ne2", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression CMP_LE expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_cmp_le", a, "", 0, WSCRIPT_LINE_FILE); $$=e;
}
             | expression BOOL_OR expression { 			   
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_bool_or", a, "", 0, WSCRIPT_LINE_FILE); $$=e;
}
             | expression BOOL_AND expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_bool_and", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression '&' expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_bin_and", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression BIN_OR expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_bin_or", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression BIN_XOR expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_bin_xor", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression BIN_SHIFTL expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_bin_shiftl", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | expression BIN_SHIFTR expression { 
ExpressionList a; a.push_back(*$1); a.push_back(*$3); delete $1; delete $3; Expression* e = new Expression("_bin_shiftr", a, "", 0, WSCRIPT_LINE_FILE); $$=e; 
}
             | '~' expression {
ExpressionList a;  a.push_back(*$2);  delete $2;  Expression* e = new Expression("_bin_neg", a, "", 0, WSCRIPT_LINE_FILE);  $$=e; 
}
			 | '-' expression %prec UMINUS { 
ExpressionList a; a.push_back(*$2); delete $2; Expression* e = new Expression("_uminus", a, "", 0, WSCRIPT_LINE_FILE); $$=e;
}
			| expression '?' argument_list ':' argument_list
{
Expression l ("=_loop", *$3, "", 0, WSCRIPT_LINE_FILE);
Expression m ("=_loop", *$5, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
a.push_back(*$1);
a.push_back(l);
a.push_back(m);
delete $5;
delete $3;
delete $1;
Expression* e = new Expression("~if2", a, "", 0, WSCRIPT_LINE_FILE);
$$=e;
}
;
             | '(' expression ')' { 
$$=$2; 
}
             | constant {
$$=$1; 
}
			| GLOBAL variable_def_list {
Expression* e = new Expression("global", *$2, "", 0, WSCRIPT_LINE_FILE); delete $2; $$=e;
}
			| 'unset' '(' variable_list ')' {
Expression* e = new Expression("unset", *$3, "", 0, WSCRIPT_LINE_FILE); delete $3; $$=e;
}
			| 'isset' '(' variable_list ')' {
Expression* e = new Expression("isset", *$3, "", 0, WSCRIPT_LINE_FILE); delete $3; $$=e;
}
            | NAME {
ExpressionList a; Expression* e = new Expression(*$1, a, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e;
}
			| MAGIC_NAME {
ExpressionList a;  Expression empty ("", a, "", 1, WSCRIPT_LINE_FILE); a.push_back (empty);  Expression* e = new Expression(*$1, a, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e;
}
			| NAME '(' ')' {
ExpressionList a;  Expression empty ("", a, "", 1, WSCRIPT_LINE_FILE); a.push_back (empty);  Expression* e = new Expression(*$1, a, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e;
}
			| NAME '(' argument_list ')' {
Expression* e = new Expression(*$1, *$3, "", 0, WSCRIPT_LINE_FILE); delete $1; delete $3; $$=e;
}
			| NAME index_list {
Expression* e = new Expression(*$1, *$2, "", 10, WSCRIPT_LINE_FILE); delete $1; delete $2; $$=e;
}
			| ARRAY '(' array_list ')' {
Expression* e = new Expression("_array", *$3, "", 0x7FFFFFFC, WSCRIPT_LINE_FILE); delete $3; $$=e;
}
			| '[' array_list ']' {
Expression* e = new Expression("_array", *$2, "", 0x7FFFFFFC, WSCRIPT_LINE_FILE); delete $2; $$=e;
}
			| LIST '(' list_list ')' '=' expression  {
ExpressionList a;
Expression b ("", *$3, "", -1,  WSCRIPT_LINE_FILE);
a.push_back(b);
a.push_back(*$6);
Expression* e = new Expression("~assign", a, "", 0, WSCRIPT_LINE_FILE); delete $3; delete $6; $$=e;
}
;

constant :	VALUE_NUM { 
ExpressionList a; Expression* e = new Expression("", a, *$1, 2, WSCRIPT_LINE_FILE); delete $1; $$=e;
}

			 | VALUE_EXP_DNUM {
char* end;
double numvalue = strtod ($1->c_str (), &end);
char cstr[40];
sprintf(cstr, "%.14g", numvalue);
$1->assign (cstr);
ExpressionList a; 
Expression* e = new Expression("", a, *$1, 2, WSCRIPT_LINE_FILE); 
delete $1; 
$$=e;
}
			 | VALUE_ONUM { ExpressionList a; 
unsigned int num;
char* end;
num = strtol($1->c_str (), &end, NULL);
char cstr[40];
sprintf(cstr, "%u", num);
$1->assign (cstr);
Expression* e = new Expression("", a, *$1, 2, WSCRIPT_LINE_FILE); 
delete $1; 
$$=e;
}
			 | VALUE_HNUM { ExpressionList a; 
unsigned __int64 num;

if (sscanf($1->c_str (), "%llx", &num)==1) {
	char cstr[40];
	double num2 = (double) num;
	sprintf(cstr, "%.14g", num2);
	$1->assign (cstr);
}
Expression* e = new Expression("", a, *$1, 2, WSCRIPT_LINE_FILE); 
delete $1; 
$$=e;
}

			 | VALUE_BNUM { ExpressionList a; 
unsigned __int64 num=0;
char* c = (char*) $1->c_str () + 2;
int size = $1->size () - 3;
int count = 0;
while ( *c != '\0' ) {
	if ( *c == '1' ) 
		num = num +  (((unsigned __int64) 0x0000000000000001) << (size - count));
	count++; 
	c++;
}
char cstr[40];
double num2 = (double) num;
sprintf(cstr, "%.14g", num2);
$1->assign (cstr);
Expression* e = new Expression("", a, *$1, 2, WSCRIPT_LINE_FILE); 
delete $1; 
$$=e;
}
			| VALUE_STR { 
ExpressionList a; Expression* e = new Expression("", a, *$1, 4, WSCRIPT_LINE_FILE); delete $1; $$=e;
}
            | VALUE_BOOL { 
ExpressionList a; Expression* e = new Expression("", a, *$1, 3, WSCRIPT_LINE_FILE); delete $1; $$=e;
}
            | VALUE_NULL { 
ExpressionList a; Expression* e = new Expression("", a, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e;
}
;

argument_list : argument_list ',' expression {
$1->push_back(*$3); delete $3; $$=$1;
}
 | expression {
ExpressionList* l = new ExpressionList; l->push_back(*$1); delete $1; $$=l;
}
;

argument_list_or_empty : argument_list {
$$=$1;
}
			| {
// Empty argument_list
$$=new ExpressionList;

}
;

index_list : index_list '[' expression ']'  {
$1->push_back(*$3); delete $3; $$=$1;
}
 | '[' expression ']' {
ExpressionList* l = new ExpressionList; l->push_back(*$2); delete $2; $$=l;
}
;

array_list : array_list ',' array_value {
ExpressionList *l = new ExpressionList;
l->reserve( $1->size() + $3->size() ); // preallocate memory
l->insert( l->end(), $1->begin(), $1->end());
l->insert( l->end(), $3->begin(), $3->end());
delete $1;
delete $3; 
$$=l;
}
			| array_value {
$$=$1;
}
			|  {
// Empty array_list
$$=new ExpressionList;
}
;

array_value :	expression {
ExpressionList* l = new ExpressionList; l->push_back(*$1); delete $1; $$=l;
}
			| expression ARRAY_ASSIGN expression {

ExpressionList a;
a.push_back (*$1);
a.push_back (*$3);
Expression* e = new Expression("_key", a, "", 0x7FFFFFFD, WSCRIPT_LINE_FILE);
ExpressionList* l = new ExpressionList; 
l->push_back(*e); 
delete $1; 
delete $3; 
delete e; 
$$=l;
}
;


argument_list_or_empty : argument_list {
$$=$1;
}
			| {
// Empty argument_list
$$=new ExpressionList;

}
;


param :	NAME { 
ExpressionList a; Expression* e = new Expression(*$1, a, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e;
}
			| '&' NAME { 
ExpressionList a; Expression* e = new Expression(*$2, a, "", 0x7FFFFFFE, WSCRIPT_LINE_FILE); delete $2; $$=e;
}			
             | NAME '=' expression { 
ExpressionList a; a.push_back(*$3); delete $3; Expression* e = new Expression("=", a, *$1, 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
;

param_list : param_list ',' param {
$1->push_back(*$3); delete $3; $$=$1;
}
			| param {
ExpressionList* l = new ExpressionList; l->push_back(*$1); delete $1; $$=l;
}
;

param_list_or_empty : param_list {
$$=$1;
}
			| {
// Empty param_list
$$=new ExpressionList;

}
;

param_list_or_empty : param_list {
$$=$1;
}
			| {
// Empty param_list
$$=new ExpressionList;

}
;


variable_def :	NAME { 
ExpressionList a; Expression* e = new Expression(*$1, a, "", 0, WSCRIPT_LINE_FILE); delete $1; $$=e;
}
             | NAME '=' expression { 
ExpressionList a; a.push_back(*$3); delete $3; Expression* e = new Expression("=", a, *$1, 0, WSCRIPT_LINE_FILE); delete $1; $$=e; 
}
;

variable_def_list : variable_def_list ',' variable_def {
$1->push_back(*$3); delete $3; $$=$1;
}
			| variable_def {
ExpressionList* l = new ExpressionList; l->push_back(*$1); delete $1; $$=l;
}
;


variable :	NAME { 
ExpressionList a; 
Expression* e = new Expression(*$1, a, "", 0, WSCRIPT_LINE_FILE); 
ExpressionList* l = new ExpressionList;
l->push_back (*e);
delete $1; 
delete e; 
$$=l;
}
			| NAME index_list { 
ExpressionList a; 
Expression* e = new Expression(*$1, a, "", 10 + $2->size (), WSCRIPT_LINE_FILE); 
ExpressionList* l = new ExpressionList;
l->reserve( $1->size() + 1); // preallocate memory
l->push_back (*e);
l->insert( l->end(), $2->begin(), $2->end());
delete $1;
delete $2;
$$=l;
}			
;

variable_list : variable_list ',' variable {
ExpressionList* l = new ExpressionList;
l->reserve( $1->size() + $3->size()); // preallocate memory
l->insert( l->end(), $1->begin(), $1->end());
l->insert( l->end(), $3->begin(), $3->end());
delete $1;
delete $3;
$$=l;
}
			| variable {
$$=$1;
}
;

list_param : variable {
$$=$1;
}
			| LIST '(' list_list ')' {

ExpressionList a;
Expression b ("", *$3, "", 0,  WSCRIPT_LINE_FILE);
a.push_back(b);
Expression e ("~assign", a, "", -1, WSCRIPT_LINE_FILE); 
ExpressionList* l = new ExpressionList;
l->push_back (e);
delete $3;
$$=l;
}
;


list_list : list_list ',' list_param {
ExpressionList* l = new ExpressionList;
l->reserve( $1->size() + $3->size()); // preallocate memory
l->insert( l->end(), $1->begin(), $1->end());
l->insert( l->end(), $3->begin(), $3->end());
delete $1;
delete $3;
$$=l;
}
			| list_param {
$$=$1;
}
;


switch_case_list : switch_case_list switch_case {
$1->push_back(*$2); delete $2; $$=$1;
}
 | switch_case {
ExpressionList* l = new ExpressionList; l->push_back(*$1); delete $1; $$=l;
}
;

switch_case : CASE constant ':' program_statements {
Expression l ("_loop", *$4, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
a.push_back(*$2);
a.push_back(l);
Expression* e = new Expression("~case", a, "", 0, WSCRIPT_LINE_FILE); delete $2; delete $4;  $$=e; 

}
 | DEFAULT ':' program_statements {
Expression l ("_loop", *$3, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
a.push_back(l);
Expression* e = new Expression("~default", a, "", 0, WSCRIPT_LINE_FILE); delete $3;  $$=e; 
}
;

block_expression :  

			WHILE '(' argument_list ')' ';' 
{
Expression l ("_loop", ExpressionList(), "", 0, 	WSCRIPT_LINE_FILE);
ExpressionList a;
Expression m ("=_loop", *$3, "", 0, 	WSCRIPT_LINE_FILE);
a.push_back(m);
a.push_back(l);
Expression* e = new Expression("~while", a, "", 0,  WSCRIPT_LINE_FILE2(@1.first_line));
delete $3;
$$=e;
}

		
			| WHILE '(' argument_list ')' if_statement
{
Expression l ("_loop", *$5, "", 0, 	WSCRIPT_LINE_FILE);
ExpressionList a;
Expression m ("=_loop", *$3, "", 0, 	WSCRIPT_LINE_FILE);
a.push_back(m);
a.push_back(l);
Expression* e = new Expression("~while", a, "", 0,  WSCRIPT_LINE_FILE2(@1.first_line));
delete $5;
delete $3;
$$=e;
}

			| SWITCH '(' argument_list ')' '{' switch_case_list '}'
{
Expression l ("_loop", *$6, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
Expression m ("=_loop", *$3, "", 0, 	WSCRIPT_LINE_FILE);
a.push_back(m);
a.push_back(l);
Expression* e = new Expression("~switch", a, "", 0,  WSCRIPT_LINE_FILE2(@1.first_line));
delete $6;
delete $3;
$$=e;
}
			| DO if_statement WHILE '(' argument_list ')' ';'
{
Expression l ("_loop", *$2, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
Expression m ("=_loop", *$5, "", 0, 	WSCRIPT_LINE_FILE);
a.push_back(m);
a.push_back(l);
Expression* e = new Expression("~do_while", a, "", 0, WSCRIPT_LINE_FILE);
delete $2;
delete $5;
$$=e;
}
			| FOR '(' argument_list_or_empty ';' argument_list_or_empty ';' argument_list_or_empty ')' if_statement
{
Expression l ("_loop", *$9, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
Expression m ("=_loop", *$3, "", 0, WSCRIPT_LINE_FILE);
a.push_back(m);
if ($5->size ()==0) {
	Expression n ("", *$5, "true", 3, WSCRIPT_LINE_FILE);
	a.push_back(n);
}
else {
	Expression n ("=_loop", *$5, "", 0, WSCRIPT_LINE_FILE);
	a.push_back(n);
}
Expression o ("=_loop", *$7, "", 0, WSCRIPT_LINE_FILE);
a.push_back(o);
a.push_back(l);
Expression* e = new Expression("~for", a, "", 0,  WSCRIPT_LINE_FILE2(@1.first_line));
delete $3;
delete $5;
delete $7;
delete $9;
$$=e;
}

			| FOREACH '(' variable AS variable ARRAY_ASSIGN variable ')' if_statement
{
Expression l ("_loop", *$9, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
Expression m ("", *$3, "", 0, WSCRIPT_LINE_FILE);
a.push_back(m);
Expression n ("=", *$5, "", 0, WSCRIPT_LINE_FILE);
a.push_back(n);
Expression o ("=", *$7, "", 0, WSCRIPT_LINE_FILE);
a.push_back(o);
a.push_back(l);
Expression* e = new Expression("~foreach", a, "", 0, WSCRIPT_LINE_FILE2(@1.first_line));
delete $3;
delete $5;
delete $7;
delete $9;
$$=e;
}
			| FOREACH '(' variable AS variable ')' if_statement
{
Expression l ("_loop", *$7, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
Expression m ("", *$3, "", 0, WSCRIPT_LINE_FILE);
a.push_back(m);
Expression n ("=", *$5, "", 0, WSCRIPT_LINE_FILE);
a.push_back(n);
a.push_back(l);
Expression* e = new Expression("~foreach", a, "", 0, WSCRIPT_LINE_FILE2(@1.first_line));
delete $3;
delete $5;
delete $7;
$$=e;
}
			| IF '(' argument_list ')' if_statement
{
Expression l ("_when1", *$5, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
Expression m ("=_loop", *$3, "", 0, 	WSCRIPT_LINE_FILE);
a.push_back(m);
a.push_back(l);
Expression* e = new Expression("~if1", a, "", 0,  WSCRIPT_LINE_FILE2(@1.first_line));
delete $5;
delete $3;
$$=e;
}
			| IF '(' argument_list ')' if_statement ELSE if_statement
{
Expression l ("_when2", *$5, "", 0, WSCRIPT_LINE_FILE);
Expression m ("_when3", *$7, "", 0, WSCRIPT_LINE_FILE);
ExpressionList a;
Expression n ("=_loop", *$3, "", 0, 	WSCRIPT_LINE_FILE);
a.push_back(n);
a.push_back(l);
a.push_back(m);
delete $7;
delete $5;
delete $3;
Expression* e = new Expression("~if2", a, "", 0,  WSCRIPT_LINE_FILE2(@1.first_line));
$$=e;
}

			| FUNCTION NAME '(' param_list_or_empty ')' '{' program_statements '}'
{
Expression arguments (*$2, *$4, "", 0,  WSCRIPT_LINE_FILE);
Expression body (*$2, *$7, "", 0,  WSCRIPT_LINE_FILE);
ExpressionList a;
a.push_back(arguments);
a.push_back(body);
Expression* e = new Expression("~funcdef", a, "", 0,  WSCRIPT_LINE_FILE2(@1.first_line));
delete $2;
delete $4;
delete $7;
$$=e;
}

;

statement : 
			';' {
// Empty statement
Expression* a = new Expression;
$$=a;
}
			| expression ';' {
$$=$1;
}
			| block_expression {
$$=$1;
}
			| RETURN ';' {
ExpressionList a;
Expression empty ("", a, "", 0, WSCRIPT_LINE_FILE);
a.push_back(empty);
Expression* e = new Expression("return", a, "",0,WSCRIPT_LINE_FILE);
$$=e;
}
			| RETURN expression ';' {
ExpressionList a;
a.push_back(*$2);
Expression* e = new Expression("return", a, "",0,WSCRIPT_LINE_FILE);
delete $2;
$$=e;
}
			| BREAK ';' {
ExpressionList a;
Expression empty ("", a, "", 0, WSCRIPT_LINE_FILE);
a.push_back(empty);
Expression* e = new Expression("break", a, "",0,WSCRIPT_LINE_FILE);
$$=e;
}
			| CONTINUE ';' {
ExpressionList a;
Expression empty ("", a, "", 0, WSCRIPT_LINE_FILE);
a.push_back(empty);
Expression* e = new Expression("continue", a, "",0,WSCRIPT_LINE_FILE);
$$=e;
}
			| VALUE_OUTPUT {
ExpressionList a;
Expression* e = new Expression("", a, *$1, 0x7FFFFFFF, WSCRIPT_LINE_FILE);
delete $1;
$$=e;
}
;

program_statements : 
			program_statements universal_statements {

ExpressionList *l = new ExpressionList;
l->reserve( $1->size() + $2->size() ); // preallocate memory
l->insert( l->end(), $1->begin(), $1->end());
l->insert( l->end(), $2->begin(), $2->end());
delete $1;
delete $2;
$$=l;
}
			| universal_statements {
$$=$1;
}
			| {
// Empty program statement
$$=new ExpressionList;

}

;

universal_statements :
			| statements {
$$=$1;
}
			| compound_statements {
$$=$1;
}
			| '{' universal_statements '}' {
$$=$2;
}
;

statements : 
			statements statement {
$1->push_back(*$2);
delete $2;
$$=$1;
}
			| statements argument_list ';' {
ExpressionList *l = new ExpressionList;
l->reserve( $1->size() + $2->size() ); // preallocate memory
l->insert( l->end(), $1->begin(), $1->end());
l->insert( l->end(), $2->begin(), $2->end());
delete $1;
delete $2;
$$=l;
}

			| statement {
ExpressionList* l = new ExpressionList;
l->push_back(*$1);
delete $1;
$$=l;
}

			| argument_list ';' {
$$=$1;
}
;

compound_statements : 
			compound_statements compound_statement {

ExpressionList *l = new ExpressionList;
l->reserve( $1->size() + $2->size() ); // preallocate memory
l->insert( l->end(), $1->begin(), $1->end());
l->insert( l->end(), $2->begin(), $2->end());
delete $1;
delete $2;
$$=l;
}
			| compound_statement {
$$=$1;
}
;

compound_statement : 
			'{' '}' {
ExpressionList* l = new ExpressionList;
$$=l;
}
			| '{' statements '}' {
$$=$2;
}

;

if_statement : 
			'{' program_statements '}' {
$$=$2;
}
			| statement {
ExpressionList* l = new ExpressionList;
l->push_back(*$1);
delete $1;
$$=l;
}
			| argument_list ';' {
$$=$1;
}
;

