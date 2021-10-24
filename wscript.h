/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: V1 Script
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/


#ifndef _WSCRIPT_H
#define _WSCRIPT_H

#include <wlib.h>
#include <string>
#include <map>
#include <vector>

#define WSCRIPT_VERSION 0.96

// Definition of user data given in InterpreterContext
#ifndef WScriptUserData
#define WScriptUserData void*
#endif

#define WSCRIPT_LINE(line) (line&0x00FFFFFF)
#define WSCRIPT_FILE(line) (line>>24)

// A thread safe variable prefix
#define WSCRIPT_THREAD_SAFE __thread

#ifndef HMODULE
#define HMODULE void*
#endif

// Parser error function
typedef int (* WSCRIPT_ERROR_FUNCTION) (WCSTR filename, int line, WCSTR text);

// Predefinitions of basic classes defined in ast.cpp
using namespace std;
class Handle;
class DataValue;
class InterpreterContext;
class Expression;
class ExpressionList;

typedef WHashtable<WString,ExpressionList,WCSTR,ExpressionList&> FunctionDictionary;
typedef WHashtable<WString,DataValue,WCSTR,DataValue&> SymbolTable;
typedef WHashtable<WString,DataValue,WCSTR,DataValue&> ArrayHT;

// Functions
typedef int (* WSCRIPT_NATIVE_FUNCTION) (vector<DataValue*>&, DataValue&, InterpreterContext&);
typedef DataValue* (* WSCRIPT_QUICK_FUNCTION) (DataValue*, DataValue*, DataValue*);
typedef WHashtable<WString, WSCRIPT_NATIVE_FUNCTION, WCSTR, WSCRIPT_NATIVE_FUNCTION> WSCRIPT_NATIVE_FUNCTIONHT;


typedef struct {
	WCSTR name;
	WSCRIPT_NATIVE_FUNCTION func;
} WScriptFuncDef;

// DLL functions
typedef int (* WSCRIPT_DLL_STARTUP) (InterpreterContext&);
typedef int (* WSCRIPT_DLL_SHUTDOWN) (InterpreterContext&);
typedef int (* WSCRIPT_DLL_CLEANUPALL) (InterpreterContext&);
typedef int (* WSCRIPT_FREE_HANDLE_FUNCTION) (void*, int);
typedef WHashtable<WString, HMODULE, WCSTR, HMODULE> DLLModuleHT;

// Hashtable with locations of function definitions
typedef WHashtable<WString, unsigned int, WCSTR, unsigned int> FunctionLocationHT;

// Hashtable to lookup files
typedef WHashtable<WString, unsigned int, WCSTR, unsigned int> File2IdxHT;
typedef WHashtable<unsigned int, WString, unsigned int, const char*> Idx2FileHT;

// Hashtable to lookup handles
typedef WHashtable<void*, Handle*, void*, Handle*> HandleHT;

// Ret codes for user functions
#define WSCRIPT_RET_PARAM1          0x00000100
#define WSCRIPT_RET_PARAM2          0x00000200
#define WSCRIPT_RET_PARAM3          0x00000300
#define WSCRIPT_RET_PARAM4          0x00000400
#define WSCRIPT_RET_PARAM5          0x00000500
#define WSCRIPT_RET_PARAM6          0x00000600
#define WSCRIPT_RET_PARAM7          0x00000700
#define WSCRIPT_RET_PARAM8          0x00000800
#define WSCRIPT_RET_PARAM9          0x00000900
#define WSCRIPT_RET_PARAM10         0x00000A00
#define WSCRIPT_RET_PARAM11         0x00000B00
#define WSCRIPT_RET_NUM_REQUIRED    0x00000002
#define WSCRIPT_RET_BOOL_REQUIRED   0x00000003
#define WSCRIPT_RET_STR_REQUIRED    0x00000004
#define WSCRIPT_RET_HANDLE_REQUIRED 0x00000005
#define WSCRIPT_RET_ARRAY_REQUIRED  0x00000010
#define WSCRIPT_RET_ABORT           0x00010000

// Check if first param is no string
#define PARAM1_NOSTR (argvalues[0]->datatype==DataValue::DATATYPE_NO_PARAM || argvalues[0]->datatype>DataValue::DATATYPE_STR)

/* WScript higher level functions */
extern void wScriptStartup (InterpreterContext& ctx);
extern void wScriptShutdown (InterpreterContext& ctx);
extern void wScriptInitCGIVariables (InterpreterContext& ctx);

#endif
