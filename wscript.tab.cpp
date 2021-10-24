/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 21 "wscript.y"

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

extern DataValue* quickFuncEqual (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncNot (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncNotEqual (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncUMinus (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncLess (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncLessEqual (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncGreater (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncGreaterEqual (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncInc (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncDec (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncPlus (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncPlusEqual (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncMinus (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncMinusEqual (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncMul (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncMulEqual (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncDiv (DataValue*,DataValue*,DataValue*);
extern DataValue* quickFuncDivEqual (DataValue*,DataValue*,DataValue*);



/* Line 371 of yacc.c  */
#line 114 "wscript.tab.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "wscript.tab.h".  */
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
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 86 "wscript.y"

ExpressionList* pexplist;
Expression* pexp;
string* pstr;


/* Line 387 of yacc.c  */
#line 227 "wscript.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 267 "wscript.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  88
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1541

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  143
/* YYNRULES -- Number of states.  */
#define YYNSTATES  286

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   318

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    72,     2,     2,     2,    71,    57,     2,
      78,    79,    69,    67,    82,    66,    68,    70,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    83,
      59,    38,    58,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    80,     2,    81,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    84,     2,    85,    73,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    53,    54,    55,    56,    60,
      61,    62,    63,    64,    65,    74,    75,    76,    77
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    13,    17,    21,    25,    29,
      33,    38,    42,    47,    51,    55,    59,    63,    67,    71,
      75,    79,    83,    87,    91,    95,    98,   101,   104,   107,
     110,   114,   118,   122,   126,   130,   134,   138,   142,   146,
     150,   154,   158,   162,   166,   170,   173,   176,   182,   186,
     188,   191,   196,   201,   203,   205,   209,   214,   217,   222,
     226,   233,   235,   237,   239,   241,   243,   245,   247,   249,
     253,   255,   257,   258,   263,   267,   271,   274,   276,   277,
     279,   283,   285,   286,   288,   291,   295,   299,   301,   303,
     304,   306,   307,   309,   313,   317,   319,   321,   324,   326,
     331,   335,   337,   340,   342,   347,   348,   353,   357,   363,
     369,   377,   385,   395,   405,   413,   419,   427,   436,   438,
     441,   443,   446,   450,   453,   456,   458,   461,   463,   464,
     465,   467,   469,   473,   476,   480,   482,   485,   488,   490,
     493,   497,   501,   503
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      87,     0,    -1,   107,    -1,    88,    67,    88,    -1,    88,
      68,    88,    -1,    88,    66,    88,    -1,    88,    69,    88,
      -1,    88,    76,    88,    -1,    88,    70,    88,    -1,    88,
      71,    88,    -1,    25,    26,    38,    88,    -1,    26,    38,
      88,    -1,    26,    92,    38,    88,    -1,   100,    50,    88,
      -1,   100,    49,    88,    -1,   100,    48,    88,    -1,   100,
      47,    88,    -1,   100,    44,    88,    -1,   100,    46,    88,
      -1,   100,    45,    88,    -1,   100,    43,    88,    -1,   100,
      42,    88,    -1,   100,    41,    88,    -1,   100,    40,    88,
      -1,   100,    39,    88,    -1,   100,    75,    -1,    75,   100,
      -1,   100,    74,    -1,    74,   100,    -1,    72,    88,    -1,
      88,    13,    88,    -1,    88,    14,    88,    -1,    88,    58,
      88,    -1,    88,    59,    88,    -1,    88,    62,    88,    -1,
      88,    61,    88,    -1,    88,    60,    88,    -1,    88,    63,
      88,    -1,    88,    53,    88,    -1,    88,    54,    88,    -1,
      88,    57,    88,    -1,    88,    55,    88,    -1,    88,    56,
      88,    -1,    88,    65,    88,    -1,    88,    64,    88,    -1,
      73,    88,    -1,    66,    88,    -1,    88,    51,    88,    52,
      88,    -1,    78,    88,    79,    -1,    89,    -1,    22,    99,
      -1,    24,    78,    90,    79,    -1,    23,    78,    90,    79,
      -1,    26,    -1,    27,    -1,    26,    78,    79,    -1,    26,
      78,    90,    79,    -1,    26,    92,    -1,    20,    78,    93,
      79,    -1,    80,    93,    81,    -1,    12,    78,   102,    79,
      38,    88,    -1,    28,    -1,    32,    -1,    29,    -1,    30,
      -1,    31,    -1,    33,    -1,    34,    -1,    35,    -1,    90,
      82,    88,    -1,    88,    -1,    90,    -1,    -1,    92,    80,
      88,    81,    -1,    80,    88,    81,    -1,    93,    82,    94,
      -1,    93,    82,    -1,    94,    -1,    -1,    88,    -1,    88,
      21,    88,    -1,    90,    -1,    -1,    26,    -1,    57,    26,
      -1,    26,    38,    88,    -1,    96,    82,    95,    -1,    95,
      -1,    96,    -1,    -1,    96,    -1,    -1,    26,    -1,    26,
      38,    88,    -1,    99,    82,    98,    -1,    98,    -1,    26,
      -1,    26,    92,    -1,   100,    -1,    12,    78,   102,    79,
      -1,   102,    82,   101,    -1,   101,    -1,   103,   104,    -1,
     104,    -1,     9,    88,    52,   107,    -1,    -1,     9,    26,
      52,   107,    -1,    10,    52,   107,    -1,     7,    78,    90,
      79,    83,    -1,     7,    78,    90,    79,   112,    -1,     8,
      78,    90,    79,    84,   103,    85,    -1,     6,   112,     7,
      78,    90,    79,    83,    -1,     3,    78,    91,    83,    91,
      83,    91,    79,   112,    -1,     4,    78,   100,     5,   100,
      21,   100,    79,   112,    -1,     4,    78,   100,     5,   100,
      79,   112,    -1,    15,    78,    90,    79,   112,    -1,    15,
      78,    90,    79,   112,    16,   112,    -1,    11,    26,    78,
      97,    79,    84,   107,    85,    -1,    83,    -1,    88,    83,
      -1,   105,    -1,    17,    83,    -1,    17,    88,    83,    -1,
      18,    83,    -1,    19,    83,    -1,    36,    -1,   107,   108,
      -1,   108,    -1,    -1,    -1,   109,    -1,   110,    -1,    84,
     108,    85,    -1,   109,   106,    -1,   109,    90,    83,    -1,
     106,    -1,    90,    83,    -1,   110,   111,    -1,   111,    -1,
      84,    85,    -1,    84,   109,    85,    -1,    84,   107,    85,
      -1,   106,    -1,    90,    83,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   118,   118,   123,   126,   129,   132,   135,   138,   141,
     144,   147,   150,   158,   161,   164,   167,   170,   173,   176,
     179,   182,   185,   188,   191,   194,   197,   200,   203,   211,
     214,   217,   220,   223,   226,   229,   232,   235,   238,   241,
     244,   247,   250,   253,   256,   259,   262,   265,   282,   285,
     288,   291,   294,   297,   300,   303,   306,   309,   312,   315,
     318,   327,   331,   342,   353,   367,   386,   389,   392,   397,
     400,   405,   408,   415,   418,   423,   432,   435,   438,   444,
     447,   463,   466,   474,   477,   480,   485,   488,   493,   496,
     503,   506,   514,   517,   522,   525,   531,   540,   567,   570,
     584,   593,   599,   602,   607,   616,   616,   627,   639,   652,
     665,   677,   689,   711,   726,   741,   753,   769,   786,   791,
     794,   797,   804,   811,   818,   825,   834,   844,   847,   855,
     856,   859,   862,   868,   873,   883,   890,   896,   906,   912,
     916,   923,   926,   932
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FOR", "FOREACH", "AS", "DO", "WHILE",
  "SWITCH", "CASE", "DEFAULT", "FUNCTION", "LIST", "EQUAL", "EQUAL2", "IF",
  "ELSE", "RETURN", "BREAK", "CONTINUE", "ARRAY", "ARRAY_ASSIGN", "GLOBAL",
  "ISSET", "UNSET", "CONSTANT", "NAME", "MAGIC_NAME", "VALUE_NUM",
  "VALUE_ONUM", "VALUE_HNUM", "VALUE_BNUM", "VALUE_EXP_DNUM", "VALUE_STR",
  "VALUE_BOOL", "VALUE_NULL", "VALUE_OUTPUT", "VALUE_UNKNOWN", "'='",
  "BIN_SHIFTR_EQUAL", "BIN_SHIFTL_EQUAL", "BIN_XOR_EQUAL", "BIN_OR_EQUAL",
  "BIN_AND_EQUAL", "POW_EQUAL", "MOD_EQUAL", "DIV_EQUAL", "MUL_EQUAL",
  "MINUS_EQUAL", "PLUSSTR_EQUAL", "PLUS_EQUAL", "'?'", "':'", "BOOL_OR",
  "BOOL_AND", "BIN_OR", "BIN_XOR", "'&'", "'>'", "'<'", "CMP_NE2",
  "CMP_NE", "CMP_GE", "CMP_LE", "BIN_SHIFTR", "BIN_SHIFTL", "'-'", "'+'",
  "'.'", "'*'", "'/'", "'%'", "'!'", "'~'", "DEC", "INC", "POW", "UMINUS",
  "'('", "')'", "'['", "']'", "','", "';'", "'{'", "'}'", "$accept",
  "program", "expression", "constant", "argument_list",
  "argument_list_or_empty", "index_list", "array_list", "array_value",
  "param", "param_list", "param_list_or_empty", "variable_def",
  "variable_def_list", "variable", "list_param", "list_list",
  "switch_case_list", "switch_case", "block_expression", "statement",
  "program_statements", "universal_statements", "statements",
  "compound_statements", "compound_statement", "if_statement", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    61,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,    63,    58,   305,   306,   307,   308,    38,    62,    60,
     309,   310,   311,   312,   313,   314,    45,    43,    46,    42,
      47,    37,    33,   126,   315,   316,   317,   318,    40,    41,
      91,    93,    44,    59,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    86,    87,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    89,    89,    89,    89,    89,    89,    89,    89,    90,
      90,    91,    91,    92,    92,    93,    93,    93,    93,    94,
      94,    91,    91,    95,    95,    95,    96,    96,    97,    97,
      97,    97,    98,    98,    99,    99,   100,   100,   101,   101,
     102,   102,   103,   103,   104,   104,   104,   104,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   106,   106,
     106,   106,   106,   106,   106,   106,   107,   107,   107,   108,
     108,   108,   108,   109,   109,   109,   109,   110,   110,   111,
     111,   112,   112,   112
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     3,     3,     3,     3,     3,     3,
       4,     3,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     5,     3,     1,
       2,     4,     4,     1,     1,     3,     4,     2,     4,     3,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     0,     4,     3,     3,     2,     1,     0,     1,
       3,     1,     0,     1,     2,     3,     3,     1,     1,     0,
       1,     0,     1,     3,     3,     1,     1,     2,     1,     4,
       3,     1,     2,     1,     4,     0,     4,     3,     5,     5,
       7,     7,     9,     9,     7,     5,     7,     8,     1,     2,
       1,     2,     3,     2,     2,     1,     2,     1,     0,     0,
       1,     1,     3,     2,     3,     1,     2,     2,     1,     2,
       3,     3,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,    61,
      63,    64,    65,    62,    66,    67,    68,   125,     0,     0,
       0,     0,     0,     0,    78,   118,     0,     0,    70,    49,
       0,     0,   120,   135,     2,   127,   130,   131,   138,    72,
       0,   128,     0,   142,     0,     0,     0,     0,     0,     0,
     121,     0,   123,   124,    78,    92,    95,    50,     0,     0,
       0,     0,     0,     0,    57,    46,    29,    45,    96,    28,
      26,     0,    79,     0,    77,   139,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,     0,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,    25,   126,
       0,   133,     0,   137,    70,    71,     0,     0,     0,   143,
       0,     0,     0,    89,     0,    98,   101,     0,     0,   122,
       0,     0,     0,     0,     0,     0,    11,    55,     0,     0,
       0,     0,    97,    48,     0,    59,    76,   132,   140,    30,
      31,     0,    38,    39,    41,    42,    40,    32,    33,    36,
      35,    34,    37,    44,    43,     5,     3,     4,     6,     8,
       9,     7,    69,    24,    23,    22,    21,    20,    17,    19,
      18,    16,    15,    14,    13,   134,     0,    72,     0,   141,
       0,     0,     0,    83,     0,    87,    88,     0,     0,     0,
       0,     0,    58,    93,    94,    52,    51,    10,    56,    74,
      12,     0,    80,    75,     0,     0,     0,     0,   108,   109,
     105,     0,    84,     0,     0,     0,     0,   100,   115,    73,
      47,    72,     0,     0,     0,     0,     0,     0,   103,    85,
      86,   128,    99,    60,     0,     0,     0,   114,   111,    53,
       0,   128,   110,   102,     0,   116,     0,     0,   128,   128,
     107,   117,   112,   113,   106,   104
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    37,    38,    39,    40,   136,    74,    83,    84,   215,
     216,   217,    66,    67,    41,   146,   147,   257,   258,    42,
      43,    44,    45,    46,    47,    48,    54
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -206
static const yytype_int16 yypact[] =
{
     717,   -69,   -63,   799,   -61,   -43,    14,   -27,     7,   988,
     -52,    13,    20,    37,    21,    22,    49,  1375,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  1116,  1116,
    1116,    54,    54,  1116,  1116,  -206,   302,   101,   290,  -206,
     -15,  1430,  -206,  -206,   717,  -206,   963,    18,  -206,  1116,
      54,   717,    -4,  -206,    96,  1116,  1116,    26,     2,  1116,
    -206,   785,  -206,  -206,  1116,    67,  -206,    24,  1116,  1116,
      70,  1116,  1052,  1116,  1418,  -206,    33,    33,    31,  -206,
    -206,  1170,  1229,     0,  -206,  -206,    27,   551,  -206,  1116,
    1116,  1116,  1116,  1116,  1116,  1116,  1116,  1116,  1116,  1116,
    1116,  1116,  1116,  1116,  1116,  1116,  1116,  1116,  1116,  1116,
    1116,  1116,  -206,  1116,  -206,  1116,  1116,  1116,  1116,  1116,
    1116,  1116,  1116,  1116,  1116,  1116,  1116,  -206,  -206,  -206,
       5,  -206,   634,  -206,  1288,    32,    30,   110,   385,  -206,
      40,   -46,   -45,    -8,    41,  -206,  -206,   -40,   -38,  -206,
     -32,  1116,    37,   -24,   -22,  1116,  1288,  -206,   -18,   867,
    1116,  1116,    42,  -206,  1116,  -206,  1116,  -206,  -206,   697,
     697,  1203,   223,   453,   536,   614,  1321,  1448,  1448,  1465,
    1465,  1465,  1465,   -47,   -47,   -44,   -44,   -44,    33,    33,
      33,    33,  1288,  1288,  1288,  1288,  1288,  1288,  1288,  1288,
    1288,  1288,  1288,  1288,  1288,  -206,   551,  1116,    54,  -206,
    1116,   881,    39,    86,    95,  -206,    44,    48,     2,    90,
       2,   799,  -206,  1288,  -206,  -206,  -206,  1288,  -206,  -206,
    1288,  1038,  1288,  -206,  1116,    52,   -17,    -6,  -206,  -206,
      83,  1116,  -206,    -8,    47,    -5,  1116,  -206,   120,  -206,
     370,  1116,    54,   799,    55,  1140,    85,    -2,  -206,  1288,
    -206,   717,  -206,  1288,   799,    60,    62,  -206,  -206,  1360,
    1262,   717,  -206,  -206,   468,  -206,   799,   799,   717,   717,
     717,  -206,  -206,  -206,   717,   717
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -206,  -206,    61,  -206,    -3,  -197,    64,    76,   -23,   -99,
    -206,  -206,    -7,  -206,   -20,   -74,   -71,  -206,  -109,  -206,
      -1,   -48,   -31,   -35,  -206,   102,  -205
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -98
static const yytype_int16 yytable[] =
{
      52,    87,    53,   138,   252,    86,   239,   255,   256,    49,
     235,    79,    80,   129,   144,    50,   248,    55,   213,   105,
     106,   107,   108,   109,   110,   108,   109,   110,    78,   111,
     137,    62,   111,   211,   212,    56,   113,   113,   145,   219,
      57,   221,   220,   130,   113,   131,   135,   222,   267,   214,
     166,    58,   141,   142,   265,   225,   148,   226,   113,   275,
     113,   228,   253,    65,   113,   153,   154,   113,   114,   158,
      61,   282,   283,   254,   262,    70,   113,   220,   113,   139,
      78,   165,   166,   272,   130,    59,   131,   113,   205,    75,
      76,    77,   255,   256,    81,    82,    63,   206,    64,    68,
      69,    88,   132,   140,   143,   151,   152,   129,   155,   111,
     134,    73,   167,   207,   113,   208,   134,   134,   210,   218,
     134,   242,   161,   240,   241,    82,   243,   244,   246,   134,
     134,   261,   156,   134,   159,   251,   264,   271,   268,   276,
     150,   277,   162,   233,   260,   224,   247,   245,   273,   133,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   192,     0,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,     0,
     145,     0,     0,   130,   135,   131,     0,   237,    52,     0,
      53,     0,   223,   274,     0,     0,   227,     0,    52,     0,
      53,   230,   231,   280,     0,   232,     0,    82,     0,     0,
     284,   285,   266,     0,     0,     0,    89,    90,     0,     0,
       0,     0,     0,   129,     0,     0,     0,     0,   135,   129,
      52,     0,    53,   129,   129,     0,     0,     0,     0,     0,
       0,    52,     0,    53,     0,     0,     0,     0,   134,     0,
       0,   134,     0,    52,    52,    53,    53,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   250,     0,     0,     0,   111,
       0,     0,   259,    89,    90,     1,     2,   263,     3,     4,
       5,     0,   134,     6,     7,     0,   270,     8,     0,     9,
      10,    11,    12,     0,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
       0,    91,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,     0,     0,     0,     0,   111,     0,    28,     0,
       0,     0,     0,   112,    29,    30,    31,    32,     0,     0,
      33,     0,    34,    89,    90,    35,    36,    85,     1,     2,
       0,     3,     4,     5,     0,     0,     6,     7,     0,     0,
       8,     0,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,     0,     0,     0,     0,   111,     0,     0,     0,
       0,    28,     0,     0,     0,     0,     0,    29,    30,    31,
      32,     0,     0,    33,     0,    34,    89,    90,    35,    36,
     209,     1,     2,     0,     3,     4,     5,     0,     0,     6,
       7,     0,     0,     8,     0,     9,    10,    11,    12,     0,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,     0,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,     0,     0,     0,     0,   111,
       0,     0,     0,     0,    28,     0,     0,     0,     0,     0,
      29,    30,    31,    32,     0,     0,    33,     0,    34,    89,
      90,    35,    36,   281,     1,     2,     0,     3,     4,     5,
       0,     0,     6,     7,     0,     0,     8,     0,     9,    10,
      11,    12,     0,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,     0,
       0,     0,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,     0,     0,
       0,     0,   111,     0,     0,     0,     0,    28,     0,     0,
       0,     0,     0,    29,    30,    31,    32,    89,    90,    33,
       0,    34,     0,     0,    35,     0,   168,     1,     2,     0,
       3,     4,     5,     0,     0,     6,     7,     0,     0,     8,
       0,     9,    10,    11,    12,     0,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,     0,     0,     0,     0,
     111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      28,     0,     0,     0,     0,     0,    29,    30,    31,    32,
     -98,   -98,    33,     0,    34,     0,     0,    35,     0,    85,
       1,     2,     0,     3,     4,     5,     0,     0,     6,     7,
       0,     0,     8,     0,     9,    10,    11,    12,     0,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,     0,
       0,     0,     0,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,     0,     0,     0,     0,     0,    29,
      30,    31,    32,     0,     0,    33,     0,    34,    89,    90,
      35,    36,     1,     2,     0,     3,     4,     5,     0,     0,
       6,     7,     0,     0,     8,     0,     9,    10,    11,    12,
       0,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    91,     0,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,     0,     0,     0,
       0,   111,     0,     0,     0,    28,     0,     0,   149,     0,
       0,    29,    30,    31,    32,     0,     0,    33,     0,    34,
      89,    90,    35,    51,     1,     2,     0,     3,     4,     5,
       0,     0,     6,     7,     0,     0,     8,     0,     9,    10,
      11,    12,     0,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    91,     0,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,     0,
       0,     0,     0,   111,     0,     0,     0,    28,   229,     0,
       0,     0,     0,    29,    30,    31,    32,     0,     0,    33,
       0,    34,     0,     0,   238,    51,     1,     2,     0,     3,
       4,     5,     0,     0,     6,     7,     0,     0,     8,     0,
       9,    10,    11,    12,     0,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       7,     0,     0,     0,     0,     0,     0,     0,    12,     0,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,     0,     0,     0,     0,    28,
       0,     0,     0,     0,     0,    29,    30,    31,    32,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,     0,
       0,    89,    90,     0,    28,     0,     0,     0,     0,     0,
      29,    30,    31,    32,     7,     0,    33,     0,    34,     0,
       0,    60,    12,     0,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    91,
       0,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
       0,     0,     0,     0,   111,     0,     0,     0,    28,   249,
       0,     0,     0,     0,    29,    30,    31,    32,     7,     0,
      33,   157,    34,     0,     0,     0,    12,     0,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     7,     0,     0,     0,     0,     0,     0,     0,
      12,     0,    13,    14,    15,    16,   269,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,     0,     0,     0,
       0,     0,    28,    89,    90,     0,     0,     0,    29,    30,
      31,    32,     0,     0,    33,     0,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    28,     0,     0,     0,
       0,     0,    29,    30,    31,    32,    89,    90,    33,     0,
      34,    91,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    89,    90,     0,     0,   111,     0,     0,   163,
     164,     0,     0,     0,    91,   234,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    89,    90,     0,     0,   111,
      91,     0,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,    89,    90,     0,     0,   111,     0,     0,     0,     0,
       0,     0,     0,    91,   279,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    89,    90,     0,     0,   111,    91,
       0,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,     0,     0,     0,     0,   111,    71,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,     0,   278,    71,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,     0,     0,     0,     0,
       0,     0,     0,     0,   -96,   -96,     0,     0,    72,     0,
      73,     0,     0,     0,     0,     0,     0,     0,     0,   -96,
     -96,     0,     0,    72,     0,    73,   160,   -97,   -97,   -97,
     -97,   -97,   -97,   -97,   -97,   -97,   -97,   -97,   -97,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -97,   -97,     0,     0,     0,     0,   161,     0,
       0,     0,     0,     0,   127,   128,   -98,   -98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
       0,     0,     0,     0,   111,   -98,   -98,   -98,   -98,   103,
     104,   105,   106,   107,   108,   109,   110,     0,     0,     0,
       0,   111
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-206)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-98)))

static const yytype_int16 yycheck[] =
{
       3,    36,     3,    51,    21,    36,   211,     9,    10,    78,
     207,    31,    32,    44,    12,    78,   221,    78,    26,    66,
      67,    68,    69,    70,    71,    69,    70,    71,    26,    76,
      50,    83,    76,    79,    79,    78,    82,    82,    58,    79,
      26,    79,    82,    46,    82,    46,    49,    79,   253,    57,
      82,    78,    55,    56,   251,    79,    59,    79,    82,   264,
      82,    79,    79,    26,    82,    68,    69,    82,    83,    72,
       9,   276,   277,    79,    79,    26,    82,    82,    82,    83,
      26,    81,    82,    85,    87,    78,    87,    82,    83,    28,
      29,    30,     9,    10,    33,    34,    83,   132,    78,    78,
      78,     0,    84,     7,    78,    38,    82,   138,    38,    76,
      49,    80,    85,    83,    82,     5,    55,    56,    78,    78,
      59,    26,    80,    84,    38,    64,    82,    79,    38,    68,
      69,    84,    71,    72,    73,    83,    16,    52,    83,    79,
      64,    79,    78,   166,   243,   152,   220,   218,   257,    47,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,   113,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,    -1,   206,   207,   206,    -1,   210,   211,    -1,
     211,    -1,   151,   261,    -1,    -1,   155,    -1,   221,    -1,
     221,   160,   161,   271,    -1,   164,    -1,   166,    -1,    -1,
     278,   279,   252,    -1,    -1,    -1,    13,    14,    -1,    -1,
      -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,   251,   280,
     253,    -1,   253,   284,   285,    -1,    -1,    -1,    -1,    -1,
      -1,   264,    -1,   264,    -1,    -1,    -1,    -1,   207,    -1,
      -1,   210,    -1,   276,   277,   276,   277,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,   234,    -1,    -1,    -1,    76,
      -1,    -1,   241,    13,    14,     3,     4,   246,     6,     7,
       8,    -1,   251,    11,    12,    -1,   255,    15,    -1,    17,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    51,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    66,    -1,
      -1,    -1,    -1,    83,    72,    73,    74,    75,    -1,    -1,
      78,    -1,    80,    13,    14,    83,    84,    85,     3,     4,
      -1,     6,     7,     8,    -1,    -1,    11,    12,    -1,    -1,
      15,    -1,    17,    18,    19,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    -1,    -1,    78,    -1,    80,    13,    14,    83,    84,
      85,     3,     4,    -1,     6,     7,     8,    -1,    -1,    11,
      12,    -1,    -1,    15,    -1,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    -1,    -1,    78,    -1,    80,    13,
      14,    83,    84,    85,     3,     4,    -1,     6,     7,     8,
      -1,    -1,    11,    12,    -1,    -1,    15,    -1,    17,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    13,    14,    78,
      -1,    80,    -1,    -1,    83,    -1,    85,     3,     4,    -1,
       6,     7,     8,    -1,    -1,    11,    12,    -1,    -1,    15,
      -1,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      13,    14,    78,    -1,    80,    -1,    -1,    83,    -1,    85,
       3,     4,    -1,     6,     7,     8,    -1,    -1,    11,    12,
      -1,    -1,    15,    -1,    17,    18,    19,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    -1,    -1,    78,    -1,    80,    13,    14,
      83,    84,     3,     4,    -1,     6,     7,     8,    -1,    -1,
      11,    12,    -1,    -1,    15,    -1,    17,    18,    19,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    51,    -1,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    66,    -1,    -1,    83,    -1,
      -1,    72,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      13,    14,    83,    84,     3,     4,    -1,     6,     7,     8,
      -1,    -1,    11,    12,    -1,    -1,    15,    -1,    17,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    51,    -1,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    66,    81,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    -1,    -1,    78,
      -1,    80,    -1,    -1,    83,    84,     3,     4,    -1,     6,
       7,     8,    -1,    -1,    11,    12,    -1,    -1,    15,    -1,
      17,    18,    19,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    -1,
      -1,    78,    -1,    80,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    13,    14,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    12,    -1,    78,    -1,    80,    -1,
      -1,    83,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    66,    81,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    12,    -1,
      78,    79,    80,    -1,    -1,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    13,    14,    -1,    -1,    -1,    72,    73,
      74,    75,    -1,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    13,    14,    78,    -1,
      80,    51,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    13,    14,    -1,    -1,    76,    -1,    -1,    79,
      21,    -1,    -1,    -1,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    13,    14,    -1,    -1,    76,
      51,    -1,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    13,    14,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    13,    14,    -1,    -1,    76,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    -1,    76,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    52,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    75,    -1,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      75,    -1,    -1,    78,    -1,    80,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    75,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    74,    75,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    76,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    76
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     6,     7,     8,    11,    12,    15,    17,
      18,    19,    20,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    66,    72,
      73,    74,    75,    78,    80,    83,    84,    87,    88,    89,
      90,   100,   105,   106,   107,   108,   109,   110,   111,    78,
      78,    84,    90,   106,   112,    78,    78,    26,    78,    78,
      83,    88,    83,    83,    78,    26,    98,    99,    78,    78,
      26,    38,    78,    80,    92,    88,    88,    88,    26,   100,
     100,    88,    88,    93,    94,    85,   108,   109,     0,    13,
      14,    51,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    76,    83,    82,    83,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    74,    75,   108,
      90,   106,    84,   111,    88,    90,    91,   100,   107,    83,
       7,    90,    90,    78,    12,   100,   101,   102,    90,    83,
      93,    38,    82,    90,    90,    38,    88,    79,    90,    88,
      38,    80,    92,    79,    21,    81,    82,    85,    85,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    83,   109,    83,     5,    85,
      78,    79,    79,    26,    57,    95,    96,    97,    78,    79,
      82,    79,    79,    88,    98,    79,    79,    88,    79,    81,
      88,    88,    88,    94,    52,    91,   100,    90,    83,   112,
      84,    38,    26,    82,    79,   102,    38,   101,   112,    81,
      88,    83,    21,    79,    79,     9,    10,   103,   104,    88,
      95,    84,    79,    88,    16,    91,   100,   112,    83,    26,
      88,    52,    85,   104,   107,   112,    79,    79,    52,    52,
     107,    85,   112,   112,   107,   107
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, scanner, pexplist, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (YYID (0))
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, scanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, scanner, pexplist); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t  scanner, ExpressionList** pexplist)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, pexplist)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    yyscan_t  scanner;
    ExpressionList** pexplist;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (pexplist);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t  scanner, ExpressionList** pexplist)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, pexplist)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    yyscan_t  scanner;
    ExpressionList** pexplist;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, pexplist);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t  scanner, ExpressionList** pexplist)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, scanner, pexplist)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    yyscan_t  scanner;
    ExpressionList** pexplist;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , scanner, pexplist);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, scanner, pexplist); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t  scanner, ExpressionList** pexplist)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, scanner, pexplist)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    yyscan_t  scanner;
    ExpressionList** pexplist;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (pexplist);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (yyscan_t  scanner, ExpressionList** pexplist)
#else
int
yyparse (scanner, pexplist)
    yyscan_t  scanner;
    ExpressionList** pexplist;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc = yyloc_default;


    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 119 "wscript.y"
    {
	*pexplist = (yyvsp[(1) - (1)].pexplist);
}
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 123 "wscript.y"
    {
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("_plus", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION|OPCODE_QUICK_FUNC);  e->userFunc = (void*) quickFuncPlus; (yyval.pexp)=e;
}
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 126 "wscript.y"
    {
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("_plusstr", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION); (yyval.pexp)=e;
}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 129 "wscript.y"
    {
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("_minus", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncMinus;  (yyval.pexp)=e;
}
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 132 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("_mul", a, 0, WSCRIPT_LINE_FILE,  OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncMul; (yyval.pexp)=e; 
}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 135 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("_pow", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 138 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("_div", a, 0, WSCRIPT_LINE_FILE,  OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncDiv; (yyval.pexp)=e; 
}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 141 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("_mod", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 144 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(4) - (4)].pexp)); Expression* e = new Expression("=", a, (yyvsp[(2) - (4)].pstr), -1, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_EQUAL); (yyval.pexp)=e; 
}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 147 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=", a, (yyvsp[(1) - (3)].pstr), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_EQUAL); (yyval.pexp)=e; 
}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 150 "wscript.y"
    { 
int indexCnt = (yyvsp[(2) - (4)].pexplist)->size (); (yyvsp[(2) - (4)].pexplist)->push_back(*(yyvsp[(4) - (4)].pexp)); Expression* e = new Expression("=", *(yyvsp[(2) - (4)].pexplist), (yyvsp[(1) - (4)].pstr), 10+indexCnt, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_EQUAL); (yyval.pexp)=e; 
}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 158 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_plus", *(yyvsp[(1) - (3)].pexplist),  0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncPlusEqual;  (yyval.pexp)=e; 
}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 161 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_plusstr", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 164 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_minus", *(yyvsp[(1) - (3)].pexplist),  0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncMinusEqual; (yyval.pexp)=e; 
}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 167 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_mul", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncMulEqual; (yyval.pexp)=e; 
}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 170 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_pow", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 173 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_div", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncDivEqual; (yyval.pexp)=e; 
}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 176 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_mod", *(yyvsp[(1) - (3)].pexplist),  0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 179 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_bin_and", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 182 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_bin_or", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 185 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_bin_xor", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 188 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_bin_shiftl", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION);  (yyval.pexp)=e; 
}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 191 "wscript.y"
    { 
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("=_bin_shiftr", *(yyvsp[(1) - (3)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION);  (yyval.pexp)=e; 
}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 194 "wscript.y"
    { 
Expression* e = new Expression("=_inc", *(yyvsp[(1) - (2)].pexplist), -1, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncInc;  (yyval.pexp)=e; 
}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 197 "wscript.y"
    { 
Expression* e = new Expression("=_inc", *(yyvsp[(2) - (2)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION);  (yyval.pexp)=e; 
}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 200 "wscript.y"
    { 
Expression* e = new Expression("=_dec", *(yyvsp[(1) - (2)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION|OPCODE_QUICK_FUNC);  e->userFunc = (void*) quickFuncDec; (yyval.pexp)=e; 
}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 203 "wscript.y"
    { 
Expression* e = new Expression("=_dec", *(yyvsp[(2) - (2)].pexplist), -1, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_FUNCTION);  (yyval.pexp)=e; 
}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 211 "wscript.y"
    {
ExpressionList a;  a.push_back(*(yyvsp[(2) - (2)].pexp));  Expression* e = new Expression("_not", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION|OPCODE_QUICK_FUNC);  e->userFunc = (void*) quickFuncNot; (yyval.pexp)=e; 
}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 214 "wscript.y"
    {
ExpressionList a;  a.push_back(*(yyvsp[(1) - (3)].pexp));  a.push_back(*(yyvsp[(3) - (3)].pexp));  Expression* e = new Expression("_equal", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION|OPCODE_QUICK_FUNC);  e->userFunc = (void*) quickFuncEqual;  (yyval.pexp)=e; 
}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 217 "wscript.y"
    {
ExpressionList a;  a.push_back(*(yyvsp[(1) - (3)].pexp));  a.push_back(*(yyvsp[(3) - (3)].pexp));  Expression* e = new Expression("_equal2", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION);  (yyval.pexp)=e; 
}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 220 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));  Expression* e = new Expression("_greater", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncGreater; (yyval.pexp)=e;
}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 223 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp)); Expression* e = new Expression("_less", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncLess; (yyval.pexp)=e;
}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 226 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_cmp_ge", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncGreaterEqual; (yyval.pexp)=e; 
}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 229 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_cmp_ne", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncNotEqual; (yyval.pexp)=e; 
}
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 232 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_cmp_ne2", a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 235 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_cmp_le", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncLessEqual; (yyval.pexp)=e;
}
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 238 "wscript.y"
    { 			   
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_bool_or", a, 0, WSCRIPT_LINE_FILE, OPCODE_BOOL_OR|OPCODE_FUNCTION); (yyval.pexp)=e;
}
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 241 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_bool_and", a, 0, WSCRIPT_LINE_FILE, OPCODE_BOOL_AND|OPCODE_FUNCTION);  (yyval.pexp)=e; 
}
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 244 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_bin_and", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 247 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_bin_or", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 250 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_bin_xor", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 253 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_bin_shiftl", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 256 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(1) - (3)].pexp)); a.push_back(*(yyvsp[(3) - (3)].pexp));   Expression* e = new Expression("_bin_shiftr", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION); (yyval.pexp)=e; 
}
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 259 "wscript.y"
    {
ExpressionList a;  a.push_back(*(yyvsp[(2) - (2)].pexp));    Expression* e = new Expression("_bin_neg", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION);  (yyval.pexp)=e; 
}
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 262 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(2) - (2)].pexp));  Expression* e = new Expression("_uminus", a, 0, WSCRIPT_LINE_FILE,OPCODE_FUNCTION|OPCODE_QUICK_FUNC); e->userFunc = (void*) quickFuncUMinus; (yyval.pexp)=e;
}
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 266 "wscript.y"
    {
ExpressionList x;
x.push_back(*(yyvsp[(3) - (5)].pexp));
ExpressionList y;
y.push_back(*(yyvsp[(5) - (5)].pexp));
Expression* l = new Expression ("=_loop", x, 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP);
Expression* m = new Expression  ("=_loop", y, 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP);
ExpressionList a;
a.push_back(*(yyvsp[(1) - (5)].pexp));
a.push_back(*l);
a.push_back(*m);

Expression* e = new Expression("~if2", a, 0, WSCRIPT_LINE_FILE, OPCODE_SYNTAX_OP|OPCODE_IF2);
(yyval.pexp)=e;
}
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 282 "wscript.y"
    { 
(yyval.pexp)=(yyvsp[(2) - (3)].pexp); 
}
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 285 "wscript.y"
    {
(yyval.pexp)=(yyvsp[(1) - (1)].pexp); 
}
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 288 "wscript.y"
    {
Expression* e = new Expression("global", *(yyvsp[(2) - (2)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_GLOBAL|OPCODE_FUNCTION|OPCODE_MAGIC_FUNC);  (yyval.pexp)=e;
}
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 291 "wscript.y"
    {
Expression* e = new Expression("unset", *(yyvsp[(3) - (4)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_UNSET|OPCODE_FUNCTION|OPCODE_MAGIC_FUNC);  (yyval.pexp)=e;
}
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 294 "wscript.y"
    {
Expression* e = new Expression("isset", *(yyvsp[(3) - (4)].pexplist),  0, WSCRIPT_LINE_FILE, OPCODE_ISSET|OPCODE_FUNCTION|OPCODE_MAGIC_FUNC);  (yyval.pexp)=e;
}
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 297 "wscript.y"
    {
ExpressionList a; Expression* e = new Expression(*(yyvsp[(1) - (1)].pstr), a, 0, WSCRIPT_LINE_FILE, OPCODE_VARIABLE);  (yyval.pexp)=e;
}
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 300 "wscript.y"
    {
ExpressionList a;  Expression* empty = new Expression ("", a, 1, WSCRIPT_LINE_FILE); a.push_back (*empty);  Expression* e = new Expression(*(yyvsp[(1) - (1)].pstr), a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION);  (yyval.pexp)=e;
}
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 303 "wscript.y"
    {
ExpressionList a;  Expression* empty = new Expression ("", a, 1, WSCRIPT_LINE_FILE); a.push_back (*empty);  Expression* e = new Expression(*(yyvsp[(1) - (3)].pstr), a, 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION);  (yyval.pexp)=e;
}
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 306 "wscript.y"
    {
Expression* e = new Expression(*(yyvsp[(1) - (4)].pstr), *(yyvsp[(3) - (4)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_FUNCTION);   (yyval.pexp)=e;
}
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 309 "wscript.y"
    {
Expression* e = new Expression(*(yyvsp[(1) - (2)].pstr), *(yyvsp[(2) - (2)].pexplist), 10, WSCRIPT_LINE_FILE, OPCODE_ARRAY);   (yyval.pexp)=e;
}
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 312 "wscript.y"
    {
Expression* e = new Expression("_array", *(yyvsp[(3) - (4)].pexplist), 0x7FFFFFFC, WSCRIPT_LINE_FILE);  (yyval.pexp)=e;
}
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 315 "wscript.y"
    {
Expression* e = new Expression("_array", *(yyvsp[(2) - (3)].pexplist), 0x7FFFFFFC, WSCRIPT_LINE_FILE);  (yyval.pexp)=e;
}
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 318 "wscript.y"
    {
ExpressionList a;
Expression* b = new Expression ("", *(yyvsp[(3) - (6)].pexplist), -1,  WSCRIPT_LINE_FILE);
a.push_back(*b);
a.push_back(*(yyvsp[(6) - (6)].pexp));
Expression* e = new Expression("~assign", a, 0, WSCRIPT_LINE_FILE, OPCODE_SYNTAX_OP|OPCODE_ASSIGN);   (yyval.pexp)=e;
}
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 327 "wscript.y"
    { 
ExpressionList a; Expression* e = new Expression("", a, (yyvsp[(1) - (1)].pstr), 2, WSCRIPT_LINE_FILE, OPCODE_CONSTANT);  (yyval.pexp)=e;
}
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 331 "wscript.y"
    {
char* end;
double numvalue = strtod ((yyvsp[(1) - (1)].pstr)->c_str (), &end);
char cstr[40];
sprintf(cstr, "%.14g", numvalue);
(yyvsp[(1) - (1)].pstr)->assign (cstr);
ExpressionList a; 
Expression* e = new Expression("", a, (yyvsp[(1) - (1)].pstr), 2, WSCRIPT_LINE_FILE, OPCODE_CONSTANT); 
 
(yyval.pexp)=e;
}
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 342 "wscript.y"
    { ExpressionList a; 
unsigned int num;
char* end;
num = strtol((yyvsp[(1) - (1)].pstr)->c_str (), &end, NULL);
char cstr[40];
sprintf(cstr, "%u", num);
(yyvsp[(1) - (1)].pstr)->assign (cstr);
Expression* e = new Expression("", a, (yyvsp[(1) - (1)].pstr), 2, WSCRIPT_LINE_FILE, OPCODE_CONSTANT); 
 
(yyval.pexp)=e;
}
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 353 "wscript.y"
    { ExpressionList a; 
long long unsigned int num;

if (sscanf((yyvsp[(1) - (1)].pstr)->c_str (), "%llx", &num)==1) {
	char cstr[40];
	double num2 = (double) num;
	sprintf(cstr, "%.14g", num2);
	(yyvsp[(1) - (1)].pstr)->assign (cstr);
}
Expression* e = new Expression("", a, (yyvsp[(1) - (1)].pstr), 2, WSCRIPT_LINE_FILE, OPCODE_CONSTANT); 
 
(yyval.pexp)=e;
}
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 367 "wscript.y"
    { ExpressionList a; 
long long unsigned int num=0;
char* c = (char*) (yyvsp[(1) - (1)].pstr)->c_str () + 2;
int size = (yyvsp[(1) - (1)].pstr)->size () - 3;
int count = 0;
while ( *c != '\0' ) {
	if ( *c == '1' ) 
		num = num +  (((long long unsigned int) 0x0000000000000001) << (size - count));
	count++; 
	c++;
}
char cstr[40];
double num2 = (double) num;
sprintf(cstr, "%.14g", num2);
(yyvsp[(1) - (1)].pstr)->assign (cstr);
Expression* e = new Expression("", a, (yyvsp[(1) - (1)].pstr), 2, WSCRIPT_LINE_FILE, OPCODE_CONSTANT); 
 
(yyval.pexp)=e;
}
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 386 "wscript.y"
    { 
ExpressionList a; Expression* e = new Expression("", a, (yyvsp[(1) - (1)].pstr), 4, WSCRIPT_LINE_FILE, OPCODE_CONSTANT);  (yyval.pexp)=e;
}
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 389 "wscript.y"
    { 
ExpressionList a; Expression* e = new Expression("", a, (yyvsp[(1) - (1)].pstr), 3, WSCRIPT_LINE_FILE, OPCODE_CONSTANT);  (yyval.pexp)=e;
}
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 392 "wscript.y"
    { 
ExpressionList a; Expression* e = new Expression("", a, 0, WSCRIPT_LINE_FILE, OPCODE_CONSTANT);  (yyval.pexp)=e;
}
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 397 "wscript.y"
    {
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp));  (yyval.pexplist)=(yyvsp[(1) - (3)].pexplist);
}
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 400 "wscript.y"
    {
ExpressionList* l = new ExpressionList; l->push_back(*(yyvsp[(1) - (1)].pexp));  (yyval.pexplist)=l;
}
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 405 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 408 "wscript.y"
    {
// Empty argument_list
(yyval.pexplist)=new ExpressionList;

}
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 415 "wscript.y"
    {
(yyvsp[(1) - (4)].pexplist)->push_back(*(yyvsp[(3) - (4)].pexp));  (yyval.pexplist)=(yyvsp[(1) - (4)].pexplist);
}
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 418 "wscript.y"
    {
ExpressionList* l = new ExpressionList; l->push_back(*(yyvsp[(2) - (3)].pexp));  (yyval.pexplist)=l;
}
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 423 "wscript.y"
    {
ExpressionList *l = new ExpressionList;
l->reserve( (yyvsp[(1) - (3)].pexplist)->size() + (yyvsp[(3) - (3)].pexplist)->size()); // preallocate memory
l->append( *(yyvsp[(1) - (3)].pexplist));
l->append( *(yyvsp[(3) - (3)].pexplist));

 
(yyval.pexplist)=l;
}
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 432 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (2)].pexplist);
}
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 435 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 438 "wscript.y"
    {
// Empty array_list
(yyval.pexplist)=new ExpressionList;
}
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 444 "wscript.y"
    {
ExpressionList* l = new ExpressionList; l->push_back(*(yyvsp[(1) - (1)].pexp));  (yyval.pexplist)=l;
}
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 447 "wscript.y"
    {

ExpressionList a;
a.push_back (*(yyvsp[(1) - (3)].pexp));
a.push_back (*(yyvsp[(3) - (3)].pexp));
Expression* e = new Expression("_key", a, 0x7FFFFFFD, WSCRIPT_LINE_FILE);
ExpressionList* l = new ExpressionList; 
l->push_back(*e); 
 
 
 
(yyval.pexplist)=l;
}
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 463 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 466 "wscript.y"
    {
// Empty argument_list
(yyval.pexplist)=new ExpressionList;

}
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 474 "wscript.y"
    { 
ExpressionList a; Expression* e = new Expression(*(yyvsp[(1) - (1)].pstr), a, 0, WSCRIPT_LINE_FILE, OPCODE_VARIABLE);  (yyval.pexp)=e;
}
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 477 "wscript.y"
    { 
ExpressionList a; Expression* e = new Expression(*(yyvsp[(2) - (2)].pstr), a, 0x7FFFFFFE, WSCRIPT_LINE_FILE, OPCODE_VARIABLE);  (yyval.pexp)=e;
}
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 480 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(3) - (3)].pexp));  Expression* e = new Expression("=", a, (yyvsp[(1) - (3)].pstr), 0, WSCRIPT_LINE_FILE,OPCODE_EQUAL_OP|OPCODE_EQUAL);  (yyval.pexp)=e; 
}
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 485 "wscript.y"
    {
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp));  (yyval.pexplist)=(yyvsp[(1) - (3)].pexplist);
}
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 488 "wscript.y"
    {
ExpressionList* l = new ExpressionList; l->push_back(*(yyvsp[(1) - (1)].pexp));  (yyval.pexplist)=l;
}
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 493 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 496 "wscript.y"
    {
// Empty param_list
(yyval.pexplist)=new ExpressionList;

}
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 503 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 506 "wscript.y"
    {
// Empty param_list
(yyval.pexplist)=new ExpressionList;

}
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 514 "wscript.y"
    { 
ExpressionList a; Expression* e = new Expression(*(yyvsp[(1) - (1)].pstr), a, 0, WSCRIPT_LINE_FILE, OPCODE_VARIABLE);  (yyval.pexp)=e;
}
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 517 "wscript.y"
    { 
ExpressionList a; a.push_back(*(yyvsp[(3) - (3)].pexp));  Expression* e = new Expression("=", a, (yyvsp[(1) - (3)].pstr), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_EQUAL);  (yyval.pexp)=e; 
}
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 522 "wscript.y"
    {
(yyvsp[(1) - (3)].pexplist)->push_back(*(yyvsp[(3) - (3)].pexp));  (yyval.pexplist)=(yyvsp[(1) - (3)].pexplist);
}
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 525 "wscript.y"
    {
ExpressionList* l = new ExpressionList; l->push_back(*(yyvsp[(1) - (1)].pexp));  (yyval.pexplist)=l;
}
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 531 "wscript.y"
    { 
ExpressionList a; 
Expression* e = new Expression(*(yyvsp[(1) - (1)].pstr), a, 0, WSCRIPT_LINE_FILE, OPCODE_VARIABLE); 
ExpressionList* l = new ExpressionList;
l->push_back (*e);
 
 
(yyval.pexplist)=l;
}
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 540 "wscript.y"
    { 
ExpressionList a; 
Expression* e = new Expression(*(yyvsp[(1) - (2)].pstr), a, 10 + (yyvsp[(2) - (2)].pexplist)->size (), WSCRIPT_LINE_FILE, OPCODE_ARRAY); 
ExpressionList* l = new ExpressionList;
l->reserve( (yyvsp[(1) - (2)].pstr)->size() + 1); // preallocate memory
l->push_back (*e);
l->append( *(yyvsp[(2) - (2)].pexplist));


(yyval.pexplist)=l;
}
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 567 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 570 "wscript.y"
    {

ExpressionList a;
Expression* b = new Expression ("", *(yyvsp[(3) - (4)].pexplist), 0,  WSCRIPT_LINE_FILE);
a.push_back(*b);
Expression* e = new Expression ("~assign", a, -1, WSCRIPT_LINE_FILE, OPCODE_SYNTAX_OP|OPCODE_ASSIGN); 
ExpressionList* l = new ExpressionList;
l->push_back (*e);

(yyval.pexplist)=l;
}
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 584 "wscript.y"
    {
ExpressionList* l = new ExpressionList;
l->reserve( (yyvsp[(1) - (3)].pexplist)->size() + (yyvsp[(3) - (3)].pexplist)->size()); // preallocate memory
l->append( *(yyvsp[(1) - (3)].pexplist));
l->append( *(yyvsp[(3) - (3)].pexplist));


(yyval.pexplist)=l;
}
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 593 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 599 "wscript.y"
    {
(yyvsp[(1) - (2)].pexplist)->push_back(*(yyvsp[(2) - (2)].pexp));  (yyval.pexplist)=(yyvsp[(1) - (2)].pexplist);
}
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 602 "wscript.y"
    {
ExpressionList* l = new ExpressionList; l->push_back(*(yyvsp[(1) - (1)].pexp));  (yyval.pexplist)=l;
}
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 607 "wscript.y"
    {
Expression* l = new Expression ("_loop", *(yyvsp[(4) - (4)].pexplist),  0, WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
a.push_back(*(yyvsp[(2) - (4)].pexp));
a.push_back(*l);
Expression* e = new Expression("~case", a, 0, WSCRIPT_LINE_FILE, OPCODE_CASE);    (yyval.pexp)=e; 

}
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 616 "wscript.y"
    {
Expression* l = new Expression ("_loop", *(yyvsp[(4) - (4)].pexplist),  0, WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
Expression* list = new Expression (*(yyvsp[(2) - (4)].pstr), ExpressionList(),  0, WSCRIPT_LINE_FILE, OPCODE_VARIABLE);
a.push_back(*list);
a.push_back(*l);
Expression* e = new Expression("~case", a, 0, WSCRIPT_LINE_FILE, OPCODE_CASE);    (yyval.pexp)=e; 

}
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 627 "wscript.y"
    {
Expression* l= new Expression  ("_loop", *(yyvsp[(3) - (3)].pexplist),  0, WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
Expression* def = new Expression ("default", ExpressionList(), 0, WSCRIPT_LINE_FILE);
a.push_back(*def);
a.push_back(*l);
Expression* e = new Expression("~default", a, 0, WSCRIPT_LINE_FILE, OPCODE_DEFAULT);   (yyval.pexp)=e; 
}
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 640 "wscript.y"
    {
Expression *l= new Expression  ("_loop", ExpressionList(), 0, 	WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
Expression *m= new Expression  ("=_loop", *(yyvsp[(3) - (5)].pexplist), 0, 	WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
a.push_back(*m);
a.push_back(*l);
Expression* e = new Expression("~while", a, 0,  WSCRIPT_LINE_FILE2((yylsp[(1) - (5)]).first_line), OPCODE_SYNTAX_OP|OPCODE_WHILE);

(yyval.pexp)=e;
}
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 653 "wscript.y"
    {
Expression *l= new Expression  ("_loop", *(yyvsp[(5) - (5)].pexplist), 0, 	WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
Expression *m= new Expression  ("=_loop", *(yyvsp[(3) - (5)].pexplist), 0, 	WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
a.push_back(*m);
a.push_back(*l);
Expression* e = new Expression("~while", a, 0,  WSCRIPT_LINE_FILE2((yylsp[(1) - (5)]).first_line), OPCODE_SYNTAX_OP|OPCODE_WHILE);


(yyval.pexp)=e;
}
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 666 "wscript.y"
    {
Expression* l= new Expression  ("_loop", *(yyvsp[(6) - (7)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
Expression* m= new Expression  ("=_loop", *(yyvsp[(3) - (7)].pexplist), 0, 	WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
a.push_back(*m);
a.push_back(*l);
Expression* e = new Expression("~switch", a, 0,  WSCRIPT_LINE_FILE2((yylsp[(1) - (7)]).first_line), OPCODE_SYNTAX_OP|OPCODE_SWITCH);


(yyval.pexp)=e;
}
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 678 "wscript.y"
    {
Expression* l = new Expression ("_loop", *(yyvsp[(2) - (7)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
Expression* m = new Expression ("=_loop", *(yyvsp[(5) - (7)].pexplist), 0, 	WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
a.push_back(*m);
a.push_back(*l);
Expression* e = new Expression("~do_while", a, 0, WSCRIPT_LINE_FILE, OPCODE_SYNTAX_OP|OPCODE_DO_WHILE);


(yyval.pexp)=e;
}
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 690 "wscript.y"
    {
Expression* l = new Expression  ("_loop", *(yyvsp[(9) - (9)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
Expression* m = new Expression ("=_loop", *(yyvsp[(3) - (9)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
a.push_back(*m);
if ((yyvsp[(5) - (9)].pexplist)->size ()==0) {
	Expression* n = new Expression ("", *(yyvsp[(5) - (9)].pexplist), new string ("true"), 3, WSCRIPT_LINE_FILE);
	a.push_back(*n);
}
else {
	Expression* n = new Expression ("=_loop", *(yyvsp[(5) - (9)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
	a.push_back(*n);
}
Expression* o = new Expression ("=_loop", *(yyvsp[(7) - (9)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
a.push_back(*o);
a.push_back(*l);
Expression* e = new Expression("~for", a, 0,  WSCRIPT_LINE_FILE2((yylsp[(1) - (9)]).first_line), OPCODE_SYNTAX_OP|OPCODE_FOR);

(yyval.pexp)=e;
}
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 712 "wscript.y"
    {
Expression* l = new Expression ("_loop", *(yyvsp[(9) - (9)].pexplist),  0, WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
Expression* m = new Expression ("", *(yyvsp[(3) - (9)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_VARIABLE);
ExpressionList a;
a.push_back(*m);
Expression* n = new Expression ("=", *(yyvsp[(5) - (9)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_EQUAL|OPCODE_VARIABLE);
a.push_back(*n);
Expression* o = new Expression ("=", *(yyvsp[(7) - (9)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_EQUAL|OPCODE_VARIABLE);
a.push_back(*o);
a.push_back(*l);
Expression* e = new Expression("~foreach", a, 0, WSCRIPT_LINE_FILE2((yylsp[(1) - (9)]).first_line), OPCODE_SYNTAX_OP|OPCODE_FOREACH);

(yyval.pexp)=e;
}
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 727 "wscript.y"
    {
Expression* l = new Expression ("_loop", *(yyvsp[(7) - (7)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_LOOP|OPCODE_FUNCTION);
Expression* m = new Expression ("", *(yyvsp[(3) - (7)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_VARIABLE);
ExpressionList a;
a.push_back(*m);
Expression* n = new Expression  ("=", *(yyvsp[(5) - (7)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_EQUAL|OPCODE_VARIABLE);
a.push_back(*n);
a.push_back(*l);
Expression* e = new Expression("~foreach", a,  0, WSCRIPT_LINE_FILE2((yylsp[(1) - (7)]).first_line), OPCODE_SYNTAX_OP|OPCODE_FOREACH);



(yyval.pexp)=e;
}
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 742 "wscript.y"
    {
Expression *l = new Expression  ("_when1", *(yyvsp[(5) - (5)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_WHEN1);
ExpressionList a;
Expression *m = new Expression ("=_loop", *(yyvsp[(3) - (5)].pexplist), 0, 	WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
a.push_back(*m);
a.push_back(*l);
Expression* e = new Expression("~if1", a, 0,  WSCRIPT_LINE_FILE2((yylsp[(1) - (5)]).first_line), OPCODE_SYNTAX_OP|OPCODE_IF1);


(yyval.pexp)=e;
}
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 754 "wscript.y"
    {
Expression* l = new Expression  ("_when2", *(yyvsp[(5) - (7)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_WHEN2);
Expression* m = new Expression  ("_when3", *(yyvsp[(7) - (7)].pexplist), 0, WSCRIPT_LINE_FILE, OPCODE_WHEN3);
Expression* n = new Expression ("=_loop", *(yyvsp[(3) - (7)].pexplist), 0, 	WSCRIPT_LINE_FILE, OPCODE_EQUAL_OP|OPCODE_LOOP|OPCODE_FUNCTION);
ExpressionList a;
a.push_back(*n);
a.push_back(*l);
a.push_back(*m);



Expression* e = new Expression("~if2", a, 0,  WSCRIPT_LINE_FILE2((yylsp[(1) - (7)]).first_line), OPCODE_SYNTAX_OP|OPCODE_IF2);
(yyval.pexp)=e;
}
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 770 "wscript.y"
    {
Expression* arguments = new Expression (*(yyvsp[(2) - (8)].pstr), *(yyvsp[(4) - (8)].pexplist), 0,  WSCRIPT_LINE_FILE);
Expression* body = new Expression (*(yyvsp[(2) - (8)].pstr), *(yyvsp[(7) - (8)].pexplist), 0,  WSCRIPT_LINE_FILE);
ExpressionList a;
a.push_back(*arguments);
a.push_back(*body);
Expression* e = new Expression("~funcdef", a, 0,  WSCRIPT_LINE_FILE2((yylsp[(1) - (8)]).first_line), OPCODE_SYNTAX_OP|OPCODE_FUNCDEF);



(yyval.pexp)=e;
}
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 786 "wscript.y"
    {
// Empty statement
Expression* a = new Expression;
(yyval.pexp)=a;
}
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 791 "wscript.y"
    {
(yyval.pexp)=(yyvsp[(1) - (2)].pexp);
}
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 794 "wscript.y"
    {
(yyval.pexp)=(yyvsp[(1) - (1)].pexp);
}
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 797 "wscript.y"
    {
ExpressionList a;
Expression* empty = new Expression ("", a, 1, WSCRIPT_LINE_FILE);
a.push_back(*empty);
Expression* e = new Expression("return", a, 0,WSCRIPT_LINE_FILE, OPCODE_RETURN|OPCODE_FUNCTION);
(yyval.pexp)=e;
}
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 804 "wscript.y"
    {
ExpressionList a;
a.push_back(*(yyvsp[(2) - (3)].pexp));
Expression* e = new Expression("return", a, 0,WSCRIPT_LINE_FILE, OPCODE_RETURN|OPCODE_FUNCTION);

(yyval.pexp)=e;
}
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 811 "wscript.y"
    {
ExpressionList a;
Expression* empty = new Expression ("", a, 1, WSCRIPT_LINE_FILE);
a.push_back(*empty);
Expression* e = new Expression("break", a, 0,WSCRIPT_LINE_FILE, OPCODE_BREAK|OPCODE_FUNCTION);
(yyval.pexp)=e;
}
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 818 "wscript.y"
    {
ExpressionList a;
Expression* empty = new Expression  ("", a, 1, WSCRIPT_LINE_FILE);
a.push_back(*empty);
Expression* e = new Expression("continue", a, 0,WSCRIPT_LINE_FILE, OPCODE_CONTINUE|OPCODE_FUNCTION);
(yyval.pexp)=e;
}
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 825 "wscript.y"
    {
ExpressionList a;
Expression* e = new Expression("", a, (yyvsp[(1) - (1)].pstr), 0x7FFFFFFF, WSCRIPT_LINE_FILE);

(yyval.pexp)=e;
}
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 834 "wscript.y"
    {

ExpressionList *l = new ExpressionList;
l->reserve( (yyvsp[(1) - (2)].pexplist)->size() + (yyvsp[(2) - (2)].pexplist)->size() ); // preallocate memory
l->append(*(yyvsp[(1) - (2)].pexplist));
l->append(*(yyvsp[(2) - (2)].pexplist));


(yyval.pexplist)=l;
}
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 844 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 847 "wscript.y"
    {
// Empty program statement
(yyval.pexplist)=new ExpressionList;

}
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 856 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 859 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 862 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(2) - (3)].pexplist);
}
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 868 "wscript.y"
    {
(yyvsp[(1) - (2)].pexplist)->push_back(*(yyvsp[(2) - (2)].pexp));

(yyval.pexplist)=(yyvsp[(1) - (2)].pexplist);
}
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 873 "wscript.y"
    {
ExpressionList *l = new ExpressionList;
l->reserve( (yyvsp[(1) - (3)].pexplist)->size() + (yyvsp[(2) - (3)].pexplist)->size() ); // preallocate memory
l->append( *(yyvsp[(1) - (3)].pexplist));
l->append( *(yyvsp[(2) - (3)].pexplist));


(yyval.pexplist)=l;
}
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 883 "wscript.y"
    {
ExpressionList* l = new ExpressionList;
l->push_back(*(yyvsp[(1) - (1)].pexp));

(yyval.pexplist)=l;
}
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 890 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (2)].pexplist);
}
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 896 "wscript.y"
    {

ExpressionList *l = new ExpressionList;
l->reserve( (yyvsp[(1) - (2)].pexplist)->size() + (yyvsp[(2) - (2)].pexplist)->size() ); // preallocate memory
l->append( *(yyvsp[(1) - (2)].pexplist));
l->append( *(yyvsp[(2) - (2)].pexplist));


(yyval.pexplist)=l;
}
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 906 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (1)].pexplist);
}
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 912 "wscript.y"
    {
ExpressionList* l = new ExpressionList;
(yyval.pexplist)=l;
}
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 916 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(2) - (3)].pexplist);
}
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 923 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(2) - (3)].pexplist);
}
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 926 "wscript.y"
    {
ExpressionList* l = new ExpressionList;
l->push_back(*(yyvsp[(1) - (1)].pexp));

(yyval.pexplist)=l;
}
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 932 "wscript.y"
    {
(yyval.pexplist)=(yyvsp[(1) - (2)].pexplist);
}
    break;


/* Line 1792 of yacc.c  */
#line 3517 "wscript.tab.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, scanner, pexplist, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, scanner, pexplist, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, scanner, pexplist);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, scanner, pexplist);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, pexplist, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner, pexplist);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, scanner, pexplist);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


