/*************************************
*
*  API to work with v1.dll / libv1.so
*
*  (c) Matthias Wiede '2016-2021
*  https://v1-script.net
*
**************************************/

#ifndef _V1_H
#define _V1_H

#ifdef _WIN32
#ifdef _V1_DLL_EXPORT
#define _V1_DLL __declspec( dllexport)
#else
#define _V1_DLL __declspec( dllimport)
#endif
#else
#ifdef _V1_DLL_EXPORT
#define _V1_DLL  __attribute__ ((visibility ("default")))
#else
#define _V1_DLL
#endif
#endif

// V1 data types
#define V1_DATATYPE_NULL      0  // NULL
#define V1_DATATYPE_NO_PARAM  1  // No parameter
#define V1_DATATYPE_NUM       2  // Number, int and double
#define V1_DATATYPE_BOOL      3  // Boolean
#define V1_DATATYPE_STR       4  // String
#define V1_DATATYPE_HANDLE    5  // Files, sockets ...
#define V1_DATATYPE_ARRAY    10  // Array

// Return codes for native functions
#define V1_RET_PARAM1              0x00000100
#define V1_RET_PARAM2              0x00000200
#define V1_RET_PARAM3              0x00000300
#define V1_RET_PARAM4              0x00000400
#define V1_RET_PARAM5              0x00000500
#define V1_RET_PARAM6              0x00000600
#define V1_RET_PARAM7              0x00000700
#define V1_RET_PARAM8              0x00000800
#define V1_RET_PARAM9              0x00000900
#define V1_RET_PARAM10             0x00000A00
#define V1_RET_PARAM11             0x00000B00
#define V1_RET_NUM_REQUIRED        0x00000002
#define V1_RET_BOOL_REQUIRED       0x00000003
#define V1_RET_STR_REQUIRED        0x00000004
#define V1_RET_HANDLE_REQUIRED     0x00000005
#define V1_RET_ARRAY_REQUIRED      0x00000010
#define V1_RET_ABORT               0x00010000

// Flags for data
#define V1_FLAGS_USEREF            0x01 // Use reference pointer
#define V1_FLAGS_NOWRITE           0x10 // Only readable
#define V1_FLAGS_DUPARG            0x20 // Argument is duplicated
#define V1_FLAGS_SYNCHRONIZE       0x40 // Variable is synchronized by threads

// Handle types
#define V1_HANDLETYPE_FILE         1
#define V1_HANDLETYPE_SOCKET       2
#define V1_HANDLETYPE_DIRECTORY    3
#define V1_HANDLETYPE_SERVERSOCKET 4
#define V1_HANDLETYPE_THREAD       5

// Symbol types
#define V1_SYMBOLS_CONSTANT        0
#define V1_SYMBOLS_GLOBAL          1

// Structs
typedef void* V1Context;
typedef void* V1ArgList;
typedef void* V1Data;
typedef void* V1Handle;

// Callbacks
typedef int (* V1_NATIVE_FUNCTION) (V1ArgList args, V1Data ret, V1Context ctx);
typedef int (* V1_ERROR_FUNCTION) (const char* filename, int line, const char* text);

// API functions
extern _V1_DLL void v1Startup ();
extern _V1_DLL void v1Shutdown ();
extern _V1_DLL void v1SetErrorFunction (V1_ERROR_FUNCTION errFunc);
extern _V1_DLL V1Context v1CreateContext (bool fWebMode=false);
extern _V1_DLL void v1DeleteContext (V1Context ctx);
extern _V1_DLL int v1Execute (V1Context ctx, const char* filename, int argc, const char* argv[]);
extern _V1_DLL int v1Eval (V1Context ctx, const char* code, unsigned int length, V1Data retValue=NULL);
extern _V1_DLL void v1RegisterFunction (V1Context ctx, const char* name, V1_NATIVE_FUNCTION function);
extern _V1_DLL void v1RemoveFunction (V1Context ctx, const char* name);
extern _V1_DLL bool v1FunctionRegistered (V1Context ctx, const char* name);
extern _V1_DLL void v1SetConstant (V1Context ctx, const char* name, double number, const char* str);
extern _V1_DLL void v1UnsetConstant (V1Context ctx, const char* name);
extern _V1_DLL void v1WarnInterprete (V1Context ctx, const char* text);
extern _V1_DLL void v1AbortInterprete (V1Context ctx, const char* text);
extern _V1_DLL V1Data v1CreateData ();
extern _V1_DLL void v1DeleteData (V1Data data);
extern _V1_DLL V1ArgList v1CreateArgList ();
extern _V1_DLL void v1DeleteArgList (V1ArgList argList);
extern _V1_DLL int v1ArgumentCount (V1ArgList argList);
extern _V1_DLL V1Data v1Argument (V1ArgList argList, int pos);
extern _V1_DLL void v1SetArgument (V1ArgList argList, int pos, V1Data data);
extern _V1_DLL bool v1Argument1NoString (V1ArgList argList);
extern _V1_DLL int v1Argument1Type (V1ArgList argList);
extern _V1_DLL int v1Argument2Type (V1ArgList argList);
extern _V1_DLL int v1Argument3Type (V1ArgList argList);
extern _V1_DLL int v1Argument4Type (V1ArgList argList);
extern _V1_DLL int v1Argument5Type (V1ArgList argList);
extern _V1_DLL int v1Argument6Type (V1ArgList argList);
extern _V1_DLL int v1CallUserFunction (V1Context ctx, const char* name, V1ArgList argList, V1Data ret);
extern _V1_DLL int v1Type (V1Data data);
extern _V1_DLL unsigned int v1Flags (V1Data data);
extern _V1_DLL int v1HandleType (V1Data data);
extern _V1_DLL const char* v1String (V1Data data);
extern _V1_DLL unsigned int v1StringSize (V1Data data);
extern _V1_DLL double v1Number (V1Data data);
extern _V1_DLL bool v1Bool (V1Data data);
extern _V1_DLL V1Handle v1Handle (V1Data data);
extern _V1_DLL unsigned int v1ArraySize (V1Data data);
extern _V1_DLL void v1ArrayIterate (V1Data data);
extern _V1_DLL V1Data v1ArrayNext (V1Data data, const char** key=NULL);
extern _V1_DLL V1Data v1ArrayValue (V1Data data, const char* key);
extern _V1_DLL V1Data v1SetNumber (V1Data data, double number);
extern _V1_DLL V1Data v1SetBool (V1Data data, bool flag);
extern _V1_DLL V1Data v1SetString (V1Data data, const char* str);
extern _V1_DLL V1Data v1SetBuffer (V1Data data, const char* str, unsigned int length);
extern _V1_DLL V1Data v1SetHandle (V1Context ctx, V1Data data, int type, V1Handle handle);
extern _V1_DLL V1Data v1SetArray (V1Data data);
extern _V1_DLL V1Data v1PutArrayString (V1Data data, const char* key, const char* str);
extern _V1_DLL V1Data v1PutArrayBuffer (V1Data data, const char* key, const char* str, unsigned int length);
extern _V1_DLL V1Data v1PutArrayNumber (V1Data data, const char* key, double number);
extern _V1_DLL V1Data v1PutArrayBool (V1Data data, const char* key, bool flag);
extern _V1_DLL V1Data v1PutArrayArray (V1Data data, const char* key);
extern _V1_DLL V1Data v1PushArrayString (V1Data data, const char* str);
extern _V1_DLL V1Data v1PushArrayBuffer (V1Data data, const char* str, unsigned int length);
extern _V1_DLL V1Data v1PushArrayNumber (V1Data data, double number);
extern _V1_DLL V1Data v1PushArrayBool (V1Data data, bool flag);
extern _V1_DLL V1Data v1PushArrayArray (V1Data data);
extern _V1_DLL void v1UnsetArrayKey (V1Data data, const char* key);
extern _V1_DLL V1Data v1Reference (V1Data data);
extern _V1_DLL V1Data v1Symbol (V1Context ctx, const char* name, int table);
extern _V1_DLL V1Data v1CreateSymbol (V1Context ctx, const char* name, int table);
extern _V1_DLL void v1RemoveSymbol (V1Context ctx, const char* name, int table);
extern _V1_DLL void v1SymbolIterate (V1Context ctx, int table);
extern _V1_DLL V1Data v1SymbolNext (V1Context ctx, int table, const char** name=NULL);
extern _V1_DLL unsigned int v1SymbolTableCount (V1Context ctx);
extern _V1_DLL int v1CurrentSymbolTable (V1Context ctx);
extern _V1_DLL void v1SyncEnter (V1Context ctx);
extern _V1_DLL void v1SyncLeave (V1Context ctx);
extern _V1_DLL bool v1Aborted (V1Context ctx);
extern _V1_DLL bool v1Exited (V1Context ctx);
extern _V1_DLL const char* v1LastError (V1Context ctx);
extern _V1_DLL int v1ExitCode (V1Context ctx);
extern _V1_DLL double v1Version ();

#endif
