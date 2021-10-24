/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C

      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_WSCRIPT_TAB_H_INCLUDED
# define YY_YY_WSCRIPT_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/* Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  */
enum yytokentype {
	FOR = 258,
	FOREACH = 259,
	AS = 260,
	DO = 261,
	WHILE = 262,
	SWITCH = 263,
	CASE = 264,
	DEFAULT = 265,
	FUNCTION = 266,
	LIST = 267,
	EQUAL = 268,
	EQUAL2 = 269,
	IF = 270,
	ELSE = 271,
	RETURN = 272,
	BREAK = 273,
	CONTINUE = 274,
	ARRAY = 275,
	ARRAY_ASSIGN = 276,
	GLOBAL = 277,
	ISSET = 278,
	UNSET = 279,
	CONSTANT = 280,
	NAME = 281,
	MAGIC_NAME = 282,
	VALUE_NUM = 283,
	VALUE_ONUM = 284,
	VALUE_HNUM = 285,
	VALUE_BNUM = 286,
	VALUE_EXP_DNUM = 287,
	VALUE_STR = 288,
	VALUE_BOOL = 289,
	VALUE_NULL = 290,
	VALUE_OUTPUT = 291,
	VALUE_UNKNOWN = 292,
	BIN_SHIFTR_EQUAL = 293,
	BIN_SHIFTL_EQUAL = 294,
	BIN_XOR_EQUAL = 295,
	BIN_OR_EQUAL = 296,
	BIN_AND_EQUAL = 297,
	POW_EQUAL = 298,
	MOD_EQUAL = 299,
	DIV_EQUAL = 300,
	MUL_EQUAL = 301,
	MINUS_EQUAL = 302,
	PLUSSTR_EQUAL = 303,
	PLUS_EQUAL = 304,
	BOOL_OR = 305,
	BOOL_AND = 306,
	BIN_OR = 307,
	BIN_XOR = 308,
	CMP_NE2 = 309,
	CMP_NE = 310,
	CMP_GE = 311,
	CMP_LE = 312,
	BIN_SHIFTR = 313,
	BIN_SHIFTL = 314,
	DEC = 315,
	INC = 316,
	POW = 317,
	UMINUS = 318
};
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE {
	/* Line 2058 of yacc.c  */
#line 67 "wscript.y"

	ExpressionList* pexplist;
	Expression* pexp;
	string* pstr;


	/* Line 2058 of yacc.c  */
#line 127 "wscript.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (yyscan_t  scanner, ExpressionList** pexplist);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_WSCRIPT_TAB_H_INCLUDED  */
