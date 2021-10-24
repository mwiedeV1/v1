/********************************
*
*  Examples to work with libv1.so
*
*  (c) Matthias Wiede '2016-2020
*  https://v1-script.net
*
*********************************/

#include <stdio.h>
#include <string.h>
#include "v1.h"

int myErrorFunc (const char* filename, int line, const char* text)
{
	printf ("%s\r\n", text);
	return 0;
}

int myFunc (V1ArgList args, V1Data ret, V1Context ctx)
{

	// Check if first parameter is set and at least a string value (also number, bool, null possible)
	if (v1Argument1NoString (args))
		return V1_RET_PARAM1|V1_RET_NUM_REQUIRED;

	// Parameter 2 is optional and must be a string value (also number, bool, null possible)
	if (v1ArgumentCount(args)>1 && v1Argument2Type (args)>V1_DATATYPE_STR)
		return V1_RET_PARAM2|V1_RET_NUM_REQUIRED;

	printf ("myFunc() is called with %i arguments.\r\n", v1ArgumentCount(args));

	V1Data arg1 = v1Argument(args,0);
	printf ("First argument: %g\r\n", v1Number (arg1));

	/*
	// Raise a warning
	v1WarnInterprete (ctx, "Raise a warning");
	*/

	/*
	// Abort, you can check if interpreter is aborted by v1IsAborted (ctx)
	v1AbortInterprete (ctx, "Now abort");
	return V1_RET_ABORT;
	*/

	// Set return value as string
	v1SetString (ret, "My return value");
	return 0;
}


int myFunc2 (V1ArgList args, V1Data ret, V1Context ctx)
{

	// Check if first parameter is set and at least a string value (also number, bool, null possible)
	if (v1Argument1NoString (args))
		return V1_RET_PARAM1|V1_RET_STR_REQUIRED;

	// Parameter 2 is required and must be an array
	if (v1ArgumentCount(args)<2 || v1Argument2Type (args)<V1_DATATYPE_ARRAY)
		return V1_RET_PARAM2|V1_RET_ARRAY_REQUIRED;

	printf ("myFunc2() is called with %i arguments.\r\n", v1ArgumentCount(args));

	V1Data arg1 = v1Argument(args,0);
	printf ("First argument: %s\r\n", v1String (arg1));

	// Iterate argument 2 as array
	printf ("Second argument:\r\n");
	V1Data arg2 = v1Argument(args,1);
	v1ArrayIterate (arg2);
	V1Data value;
	const char* key;
	while (value=v1ArrayNext (arg2, &key)) {
		printf ("%s => %s\r\n", (const char*) key, (const char*) v1String (value));
	}

	// Set return value as bool
	v1SetBool (ret, false);
	return 0;
}

int  main (int argc, const char* arv[])
{

	printf ("V1 version: %g\r\n", v1Version ());

	// Startup V1 API
	v1Startup ();

	// Set own error function, otherwise errors will printed to stdout/stderr
	v1SetErrorFunction (myErrorFunc);

	// Create V1 context
	V1Context ctx = v1CreateContext ();

	// Evaluate V1 code inline
	const char* code = "print ('Hello World');";
	v1Eval (ctx, code, strlen (code));

	// Evaluate V1 code inline and get calculated data
	V1Data calcData = v1CreateData ();
	code = "10+22*8;";
	v1Eval (ctx, code, strlen (code), calcData);
	printf ("Return: %g\r\n", v1Number(calcData));
	v1DeleteData (calcData);

	// Evaluate V1 code with parser error
	code = "print X ('Hello World');";
	int retCode = v1Eval (ctx, code, strlen (code));
	if (v1Aborted (ctx))
		printf ("Aborted with: %s\r\n", (const char*) v1LastError (ctx));

	// Try to execute unknown filename
	retCode = v1Execute (ctx, "exampleXXX.v1", 0, NULL);
	if (v1Aborted (ctx)) {
		printf ("%s\r\n", (const char*) v1LastError (ctx));
	}

	// Exit manually with exit code
	code = "exit(99);";
	retCode = v1Eval (ctx, code, strlen (code));
	if (v1Exited (ctx))
		printf ("Exit Code: %i\r\n", (int) retCode);

	// Delete context
	v1DeleteContext (ctx);

	// Create V1 context
	ctx = v1CreateContext ();

	// Register own functions
	v1RegisterFunction (ctx, "myFunc", myFunc);
	v1RegisterFunction (ctx, "myFunc2", myFunc2);

	// Set simple constants
	v1SetConstant (ctx, "MY_CONSTANT", 0, "MY STRING"); // String
	v1SetConstant (ctx, "MY_CONSTANT2", 99, NULL); // Number

	code =
	    "print (MY_CONSTANT);"\
	    "print (MY_CONSTANT2);"\
	    "print (myFunc (22.2));"\
	    "print (myFunc2 ('OK', [1,2,3,4,5]));";
	v1Eval (ctx, code, strlen (code));
	// v1Execute (ctx, "example.v1", 0, NULL); // Execute a filename

	// Read a global variable written by V1
	code = "a = time ();";
	v1Eval (ctx, code, strlen (code));
	V1Data a = v1Symbol (ctx, "a", V1_SYMBOLS_GLOBAL); // 0 = constants, 1 = global vars
	if (a) {
		printf ("a = %.13g\r\n", (double) v1Number (a));

		// Change variable
		v1SetNumber (a, 99);
		code = "print('a is now ', a);";
		v1Eval (ctx, code, strlen (code));
	}

	// Manipulate a global array written by V1
	code = "b = array ('key1'=>'Value 1', 'key2'=>99, 'key3'=>true); print_r (b);";
	v1Eval (ctx, code, strlen (code));

	V1Data b = v1Symbol (ctx, "b", V1_SYMBOLS_GLOBAL); // 0 = constants, 1 = global vars
	if (b) {
		// Unset key2
		v1UnsetArrayKey (b, "key2");
		printf ("b has now %u elements\r\n", v1ArraySize (b));

		// Iterate array
		V1Data value;
		const char* key;
		v1ArrayIterate (b);
		while (value=v1ArrayNext (b, &key)) {
			printf ("%s => %s\r\n", (const char*) key, (const char*) v1String (value));
		}

		// Set a value in array safe:
		// Note! v1ArrayPut...() will insert/replace the complete array key/value pair
		// To change only the value of a key, first get the value with v1ArrayValue().
		// So references to this value will be not destroyed!
		V1Data v = v1ArrayValue (b, "key1");
		if (v) {
			v1SetNumber (v, 100);
		}
		code = "print ('key1 was changed by application.'); print_r (b);";
		v1Eval (ctx, code, strlen (code));
	}


	// Call a user function
	code = "function myUserFunc (arg1,arg2) { print ('myUserFunc() called from application with arg1=', arg1, ' and arg2=', arg2); return 'OK'; }";
	v1Eval (ctx, code, strlen (code));

	V1ArgList argList = v1CreateArgList ();
	V1Data ret = v1CreateData ();
	V1Data arg1 = v1CreateData ();
	v1SetString (arg1, "Value for Argument 1");
	V1Data arg2 = v1CreateData ();
	v1SetNumber (arg2, 99);
	v1SetArgument (argList, 0, arg1);
	v1SetArgument (argList, 1, arg2);
	if (!v1CallUserFunction (ctx, "myUserFunc", argList, ret)) {
		printf ("Return value: %s\r\n", (const char*) v1String (ret));
	}
	else {
		printf ("%s\r\n", (const char*) v1LastError (ctx));
	}
	v1DeleteData (arg1); // v1DeleteData() must only called for a corresponding v1CreateData()
	v1DeleteData (arg2);
	v1DeleteData (ret);
	v1DeleteArgList (argList); // v1DeleteArgList() must only called for a corresponding v1CreateArgList()


	// Read a file created by V1
	code = "fh = fopen ('testfile.txt', 'w+');";
	v1Eval (ctx, code, strlen (code));
	V1Data fh = v1Symbol (ctx, "fh", V1_SYMBOLS_GLOBAL); // 0 = constants, 1 = global vars
	if (fh && v1HandleType (fh)==V1_HANDLETYPE_FILE) {
		// Cast to FILE* HANDLE
		FILE* pFile = (FILE*) v1Handle (fh);
		if (pFile) {
			// Write
			const char* buf = "Written by application";
			unsigned int size = strlen (buf);
			if (fwrite (buf, sizeof(char), size, pFile) < size) {
				printf ("Problems to write file.\r\n");
			}
		}
	}

	// Create a file and assign to V1 variable
	FILE* pFile;
	if (!(pFile=fopen ("testfile2.txt", "w+"))) {
		printf ("Cannot create file.");
	}
	else {
		V1Data symbol = v1CreateSymbol (ctx, "fh2", V1_SYMBOLS_CONSTANT); // 0=constants, 1=global vars, 2=first running function ...
		v1SetHandle (ctx, symbol, V1_HANDLETYPE_FILE, (V1Handle) pFile); // only V1_HANDLETYPE_FILE, V1_HANDLETYPE_SOCKET allowed

		code = "fwrite (fh2, 'Written by V1'); fclose (fh2);";
		v1Eval (ctx, code, strlen (code));
		// Note! File handles will be closed also automatically in v1DeleteContext()
	}

	// Iterate all variables of current symbol table
	V1Data value;
	const char* name;
	v1SymbolIterate (ctx, v1CurrentSymbolTable (ctx));
	printf ("Max. %u symbol tables available, current symbol table is %u.\r\n", (unsigned int) v1SymbolTableCount (ctx), (unsigned int) v1CurrentSymbolTable (ctx));
	while (value=v1SymbolNext (ctx, v1CurrentSymbolTable (ctx), &name)) {
		printf ("%s => Type %i (string=\"%s\")\r\n", (const char*) name, (int) v1Type (value), (const char*) v1String (value));
	}

	// Remove a function
	code = "function userFunc (x) { return sqrt(x); }; print (userFunc(16));";
	v1Eval (ctx, code, strlen (code));
	if (v1FunctionRegistered (ctx, "userFunc")) {
		v1RemoveFunction (ctx, "userFunc");
		// Redefinition will cause no error, because function was removed
		v1Eval (ctx, code, strlen (code));
	}

	// Create a constant array
	V1Data symbol = v1CreateSymbol (ctx, "mySymbol", V1_SYMBOLS_CONSTANT); // 0=constants, 1=global vars, 2=first running function ...
	v1PutArrayString (symbol, "key1", "Value 1");
	v1PutArrayString (symbol, "key2", "Value 2");
	v1PutArrayBool (symbol, "key3", true);
	V1Data subArray = v1PutArrayArray (symbol, "subArray");
	v1PutArrayString (subArray, "key1", "Value 1");
	v1PutArrayString (subArray, "key2", "Value 2");
	v1PutArrayBool (subArray, "key3", true);
	V1Data subArray2 = v1PutArrayArray (symbol, "subArray2");
	// Push will create a vector (keys are set automatically 0,1,2 ...)
	v1PushArrayString (subArray2, "Value 1");
	v1PushArrayNumber (subArray2, 42.12);
	v1PushArrayBool (subArray2, true);

	code = "print_r (mySymbol);";
	v1Eval (ctx, code, strlen (code));

	// Try to overwrite constant array will cause error
	code = "mySymbol['key1']['key3']=42;";
	v1Eval (ctx, code, strlen (code));
	printf ("%s\r\n", (const char*) v1LastError (ctx));

	v1DeleteContext (ctx);

	// Shutdown V1 API
	v1Shutdown ();
	return 0;
}
