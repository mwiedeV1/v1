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
| File: wscript.l
|
| Author(s): M.W.
|
+-----------------------------------------------------------------------------*/
%option reentrant bison-bridge yylineno
%x ST_IN_SCRIPTING
%x COMMENTS
%{
#include <string>
#include <algorithm>
#include <assert.h>
#include "ast.h"
using namespace std;
#include "wscript.h"
#include "wscript.tab.h"

extern "C"
{
typedef void* yyscan_t;
struct YYLTYPE;
extern void yyerror(YYLTYPE *location, yyscan_t scanner, ExpressionList** pexplist, const char* s);
}


#undef YY_INPUT
extern int yyScriptReader (char*, size_t *, int);
extern bool yyStrnCaseCmp (const char* s1, const char* s2, int len);
extern WSCRIPT_THREAD_SAFE unsigned int yyReadCurr;
extern WSCRIPT_THREAD_SAFE unsigned int yyReadMax;
extern WSCRIPT_THREAD_SAFE char *yyInputBuf;
extern WSCRIPT_THREAD_SAFE unsigned int yyLine;
#define YY_DECL int yylex (YYSTYPE * yylval_param , YYLTYPE* yylloc, yyscan_t yyscanner)
#define YY_USER_ACTION { yylloc->first_line = yylloc->last_line = yylineno;  yylloc->first_column = yycolumn; yylloc->last_column = yycolumn+yyleng-1; yycolumn += yyleng; }
#define YY_INPUT(b,r,s) yyScriptReader (b,&r,s)

#undef YY_BUF_SIZE
#define YY_BUF_SIZE 1048576

%}


HNUM "0x"[0-9a-fA-F]+
LNUM [0-9]+
DNUM ([0-9]*"."[0-9]+)|([0-9]+"."[0-9]*)
EXP_DNUM (({LNUM}|{DNUM})[eE][+-]?{LNUM})
BNUM "0b"[01]+
NEWLINE ("\n"|"\r\n")
%%


<INITIAL>(.|\n|\r) {
	
	// Read until next V1 element
	yytext[1]=yyg->yy_hold_char;
	char* max = yyg->yy_c_buf_p +  yyg->yy_n_chars - yyg->yy_start;
	char* c0 = yytext;
	char* c  = yytext;

	
	while (*c!=0) {		
check_v1_tag:
		if (*c == '<') {
			if ((c+5)<=max) {
				bool fNL = false;
				if (yyStrnCaseCmp(c+1, "?v1", 3) && ((c[4]==32 || c[4]=='\t') || (fNL= ( (c[4]=='\r' && c[5]=='\n') || c[4]=='\n') ) )) {
					yyg->yy_c_buf_p+=4;				
					yycolumn+=5;
					if (fNL) {
						yycolumn = 1;
						
						if (c[4]=='\n') {
							yyLine++; 
						}
						
					}
					BEGIN(ST_IN_SCRIPTING);
					break; 
				}
			}
			else {

				// Load next buffer
				if (yyg->yy_n_chars==YY_BUF_SIZE-1)
					break; // Buffer full, <?v1 tag maybe not read correctly
				bool fSpecial = false;
				if (*yyg->yy_c_buf_p==0) {
					fSpecial = true;
				}
				else
					yyg->yy_c_buf_p = max+1;

				int ret = yy_get_next_buffer( yyscanner ); 
				if (ret!=0)
					break;
				if (fSpecial) {
					c--; *c='<'; // Because yy_get_next_buffer did not copied the <, we assign it manually
				}
				yyg->yy_c_buf_p = c;

				max = c0 +  yyg->yy_n_chars - yyg->yy_start;
				goto check_v1_tag;
			}
		}
	
		if (*c=='\n') {
			yycolumn = 1; yyLine++;
		}
		else {
			yycolumn++;
		}
		c++;
		yyg->yy_c_buf_p++;

	}
	
	if (*c==0) {
		yyg->yy_c_buf_p--;
		yycolumn--;
	}
	
	yyg->yy_hold_char = *yyg->yy_c_buf_p;
	yylineno = yyLine;
	if (c0!=c) { 
		
		yylval->pstr = new string (c0, c-c0);
		return VALUE_OUTPUT;
	}

}

<ST_IN_SCRIPTING>"?>"{NEWLINE}? {
	if (strstr ((const char*) yytext,"\n"))
		yyLine++;
	BEGIN(INITIAL);	
}

<ST_IN_SCRIPTING>function		{return FUNCTION;}
<ST_IN_SCRIPTING>array			{return ARRAY;}
<ST_IN_SCRIPTING>=\>			{return ARRAY_ASSIGN;}
<ST_IN_SCRIPTING>do				{return DO;}
<ST_IN_SCRIPTING>while			{return WHILE;}
<ST_IN_SCRIPTING>for			{return FOR;}
<ST_IN_SCRIPTING>foreach		{return FOREACH;}
<ST_IN_SCRIPTING>as				{return AS;}
<ST_IN_SCRIPTING>if				{return IF;}
<ST_IN_SCRIPTING>else			{return ELSE;}
<ST_IN_SCRIPTING>return			{return RETURN;}
<ST_IN_SCRIPTING>break			{return BREAK;}
<ST_IN_SCRIPTING>continue		{return CONTINUE;}
<ST_IN_SCRIPTING>switch			{return SWITCH;}
<ST_IN_SCRIPTING>case			{return CASE;}
<ST_IN_SCRIPTING>default		{return DEFAULT;}
<ST_IN_SCRIPTING>assign			{return LIST;}
<ST_IN_SCRIPTING>global			{return GLOBAL;}
<ST_IN_SCRIPTING>isset			{return ISSET;}
<ST_IN_SCRIPTING>unset			{return UNSET;}
<ST_IN_SCRIPTING>const			{return CONSTANT;}
<ST_IN_SCRIPTING>\=\=			{return EQUAL;}
<ST_IN_SCRIPTING>\=\=\=			{return EQUAL2;}
<ST_IN_SCRIPTING>\<=			{return CMP_LE; }
<ST_IN_SCRIPTING>\>=			{return CMP_GE; }
<ST_IN_SCRIPTING>\!=			{return CMP_NE; }
<ST_IN_SCRIPTING>\!==			{return CMP_NE2; }
<ST_IN_SCRIPTING>"&&"  			{return BOOL_AND; }
<ST_IN_SCRIPTING>"||"  			{return BOOL_OR; }
<ST_IN_SCRIPTING>"&="  			{return BIN_AND_EQUAL; }
<ST_IN_SCRIPTING>"|"  			{return BIN_OR; }
<ST_IN_SCRIPTING>"|="  			{return BIN_OR_EQUAL; }
<ST_IN_SCRIPTING>"^"  			{return BIN_XOR; }
<ST_IN_SCRIPTING>"^="  			{return BIN_XOR_EQUAL; }
<ST_IN_SCRIPTING>"<<"  			{return BIN_SHIFTL; }
<ST_IN_SCRIPTING>"<<="  		{return BIN_SHIFTL_EQUAL; }
<ST_IN_SCRIPTING>">>"  			{return BIN_SHIFTR; }
<ST_IN_SCRIPTING>">>="  		{return BIN_SHIFTR_EQUAL; }
<ST_IN_SCRIPTING>"+="			{return PLUS_EQUAL;}
<ST_IN_SCRIPTING>".="			{return PLUSSTR_EQUAL;}
<ST_IN_SCRIPTING>"-="			{return MINUS_EQUAL;}
<ST_IN_SCRIPTING>"*="			{return MUL_EQUAL;}
<ST_IN_SCRIPTING>"/="			{return DIV_EQUAL;}
<ST_IN_SCRIPTING>"%="			{return MOD_EQUAL;}
<ST_IN_SCRIPTING>"**"			{return POW;}
<ST_IN_SCRIPTING>"**="			{return POW_EQUAL;}
<ST_IN_SCRIPTING>"--"			{return DEC;}
<ST_IN_SCRIPTING>"++"			{return INC;}

<ST_IN_SCRIPTING>{LNUM}			{ string* s = new string (yytext); yylval->pstr = s; if (yytext[0]=='0') return VALUE_ONUM; else return VALUE_NUM; }
<ST_IN_SCRIPTING>{BNUM}			{ string* s = new string (yytext); yylval->pstr = s; return VALUE_BNUM; }
<ST_IN_SCRIPTING>{DNUM}			{ string* s = new string (yytext); yylval->pstr = s; return VALUE_NUM; }
<ST_IN_SCRIPTING>{HNUM}			{ string* s = new string (yytext); yylval->pstr = s; return VALUE_HNUM; }
<ST_IN_SCRIPTING>{EXP_DNUM}		{ string* s = new string (yytext); yylval->pstr = s; return VALUE_EXP_DNUM; }
<ST_IN_SCRIPTING>true			{ string* s = new string (yytext); yylval->pstr = s; return VALUE_BOOL; }
<ST_IN_SCRIPTING>false			{ string* s = new string (yytext); yylval->pstr = s; return VALUE_BOOL; }
<ST_IN_SCRIPTING>null			{ string* s = new string (""); yylval->pstr = s; return VALUE_NULL; }

<ST_IN_SCRIPTING>__FILE__|__LINE__ {
string* s = new string (yytext);
// transform (s->begin(),s->end(), s->begin(), tolower);
yylval->pstr = s;
return MAGIC_NAME;
}


<ST_IN_SCRIPTING>[a-zA-Z_][a-zA-Z0-9_]* {
string* s = new string (yytext);
// transform (s->begin(),s->end(), s->begin(), tolower);
yylval->pstr = s;
return NAME;
}

<ST_IN_SCRIPTING>L?'(\\.|[^\\'])*'	{
// Replace quotes
string s1;
bool flag = false;
char* c = yytext;
while (*c!=0) {
	int offs = 1;
	if (*c=='\n') {
		yyLine++; yycolumn = 1;
	}
	if (flag) {
		if (*c=='\'')
			*c='\'';
		else
			s1+='\\'; 
		flag = false;
	}
	else
	if (*c=='\\') {
		flag = true;
	}
	
	if (!flag) {
		s1+=*c; 
	}
	if (*c!=0)
		c+=offs;
}
yylval->pstr = new string(s1, 1, s1.length ()-2);
return VALUE_STR;
}

<ST_IN_SCRIPTING>L?\"(\\.|[^\\"])*\" {
// Replace quotes and line feeds
string s1;
bool flag = false;
char* c = yytext;
while (*c!=0) {
	if (*c=='\n') {
		yyLine++; yycolumn = 1;
	}
	int offs = 1;
	if (flag) {
		if (*c=='\'')
			*c='\'';
		else
		if (*c=='"')
			*c='\"';
		else
		if (*c=='r')
			*c='\r';
		else
		if (*c=='?')
			*c='?';
		else
		if (*c=='\\')
			*c='\\';
		else
		if (*c=='n')
			*c='\n';
		else
		if (*c=='t')
			*c='\t';
		else
		if (*c=='v')
			*c='\v';
		else
		if (*c=='a')
			*c='\a';
		else
		if (*c=='b')
			*c='\b';
		else
		if (*c=='f')
			*c='\f';
		else
		if (*c=='0')
			*c=0;
		else
		if (*c=='x') {
			// Hex
			unsigned int hex=0;
			if (sscanf ((c+1), "%x", &hex)==1) {
				if (hex>255) {
					yyerror (yylloc, yyscanner, NULL, WFormattedString ("String hex value %X too big for character", hex)); 
					return 0;
				}

				const char *c2 = c;
				c2++;
				while ((*c2>='0' && *c2<='9') || (*c2>='A' && *c2<='F') || (*c2>='a' && *c2<='f')) {
					offs++; c2++;
					if (offs==3)
						break;
				}
				*c=(unsigned char) hex;
			}
			else {
				// Invalid Hex value
				yyerror (yylloc, yyscanner, NULL, "Invalid terminated hex value"); 
				return 0;
			}
		}
		else 
		{
			// Octal
			unsigned int oct=0;
			if (sscanf (c, "%o", &oct)==1) {
				if (oct>255) {
					yyerror (yylloc, yyscanner, NULL, WFormattedString ("String octal value %o too big for character", oct)); 
					return 0;
				}
				const char *c2 = c;
				c2++;
				while (*c2>='0' && *c2<='7') {
					offs++; c2++;
				}
				*c=(unsigned char) oct;
			}
			else {
				// Invalud Octal value
				yyerror (yylloc, yyscanner, NULL, "Invalid terminated octal value"); 
				return 0;

			}

		}
		flag = false;
	}
	else
	if (*c=='\\') {
		flag = true;
	}
	if (!flag) {
		s1+=*c; 
	}
	// if (*c!=0)
	c+=offs;

}
yylval->pstr = new string(s1, 1, s1.size () - 2);
return VALUE_STR;
}


<ST_IN_SCRIPTING>\/\*			{ BEGIN(COMMENTS); }
<COMMENTS>\*\/					{ BEGIN(ST_IN_SCRIPTING); }
<COMMENTS>\n					{ yycolumn = 1; yyLine++; }   
<COMMENTS>.						;   // ignore every other character while we're in this state
<COMMENTS><<EOF>>				{ yyerror (yylloc, yyscanner, NULL, "Unclosed comment"); return yytext[0]; }
<ST_IN_SCRIPTING>[\t\ \r]		{/* ignore these as spaces*/}
<ST_IN_SCRIPTING>\n				{ yycolumn = 1; yyLine++; }
<ST_IN_SCRIPTING>\/\/.*			{/* ignore as comment */}
<ST_IN_SCRIPTING>.				{ return yytext[0]; }
