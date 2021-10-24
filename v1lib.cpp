#include "wscript.h"
#include "ast.h"
#include "v1.h"

extern int freeHandle (void* handle, int handletype);
extern void wScriptInitFunctions (InterpreterContext& ctx);
extern void wScriptCleanupFunctions (InterpreterContext& ctx);
extern WSCRIPT_ERROR_FUNCTION wScriptErrorFunction;
extern int callUserFunc  (const string& operation, vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx, int argOffs=0);

void v1Startup ()
{
	WSystem::startup ();
	WSocketUtils::startup ();
}

void v1Shutdown ()
{
	WSocketUtils::shutdown ();
	WSystem::shutdown ();
}

void v1SetErrorFunction (V1_ERROR_FUNCTION errFunc)
{
	wScriptErrorFunction = errFunc;
}

int v1Execute (V1Context ctx, const char* filename, int argc, const char* argv[])
{
	int ret = 0;
	InterpreterContext* oldGlobalCtx = InterpreterContext::ctx;
	try {
		// Create context
		InterpreterContext::ctx = (InterpreterContext*) ctx;


		// Init arguments
		DataValue datavalue;
		datavalue.datatype=DataValue::DATATYPE_ARRAY;
		datavalue.flags = DataValue::FLAGS_NOWRITE;
		((InterpreterContext*) ctx)->symbols[0]->put ("argv", datavalue);
		for (int argIdx=0; argIdx<argc; argIdx++) {
			DataValue v ((WCSTR) argv[argIdx]);
			((InterpreterContext*) ctx)->symbols[0]->get ("argv").arrayList.put(WFormattedString("%u",argIdx), v)->m_value.flags|=DataValue::FLAGS_NOWRITE;
		}
		ret = ((InterpreterContext*) ctx)->execute (filename, 2);

		// Print Headers in Web/CGI Mode finally!
		((InterpreterContext*) ctx)->printHeaders ();
	}
	catch (WException& e) {
		((InterpreterContext*) ctx)->fAbort = true;
		((InterpreterContext*) ctx)->errText = WFormattedString ("Fatal error: %s", (WCSTR) e.toString ());
		if (wScriptErrorFunction)
			wScriptErrorFunction ("", 0, ((InterpreterContext*) ctx)->errText);
		fprintf(stderr, "%s\r\n", (WCSTR) ((InterpreterContext*) ctx)->errText);
		ret = -1;
	}
	// Finally flush output
	fflush(stdout);
	InterpreterContext::ctx = oldGlobalCtx;
	return ret;
}


int v1Eval (V1Context ctx, const char* code, unsigned int length, V1Data retValue)
{
	int ret = 0;
	InterpreterContext* oldGlobalCtx = InterpreterContext::ctx;
	try {
		((InterpreterContext*) ctx)->line=0; // Reset the line so every eval() show the correct error line
		if (!retValue) {
			DataValue retValue2;
			ret = ((InterpreterContext*) ctx)->eval (code, length, retValue2);
		}
		else
			ret = ((InterpreterContext*) ctx)->eval (code, length, *(DataValue*) retValue);
	}
	catch (WException& e) {
		((InterpreterContext*) ctx)->fAbort = true;
		((InterpreterContext*) ctx)->errText = WFormattedString ("Fatal error: %s", (WCSTR) e.toString ());
		if (wScriptErrorFunction)
			wScriptErrorFunction ("", 0, ((InterpreterContext*) ctx)->errText);
		fprintf(stderr, "%s\r\n", (WCSTR) ((InterpreterContext*) ctx)->errText);
		ret = -1;
	}
	// Finally flush output
	fflush(stdout);
	InterpreterContext::ctx = oldGlobalCtx;
	return ret;
}




V1Context v1CreateContext (bool fWebMode)
{
	InterpreterContext* ctx = new InterpreterContext;
	InterpreterContext::ctx = ctx;
	wScriptInitFunctions (*ctx);
	if (fWebMode) {
		ctx->programMode = InterpreterContext::PROGRAM_MODE_WEB;
		ctx->fAutoNL = false;
		wScriptInitCGIVariables (*ctx);
	}
	return (V1Context) ctx;
}


void v1DeleteContext (V1Context ctx)
{
	((InterpreterContext*) ctx)->cleanupAll ();
	wScriptCleanupFunctions (*(InterpreterContext*) ctx);
	delete (InterpreterContext*) ctx;
}

void v1RegisterFunction (V1Context ctx, const char* name, V1_NATIVE_FUNCTION function)
{
	((InterpreterContext* ) ctx)->nativeFunctionHT.put (name, (WSCRIPT_NATIVE_FUNCTION) function);
}

bool v1FunctionRegistered (V1Context ctx, const char* name)
{
	if (((InterpreterContext* ) ctx)->nativeFunctionHT.isKey (name))
		return true;
	if (((InterpreterContext* ) ctx)->functions.isKey (name))
		return true;
	return false;
}

void v1RemoveFunction (V1Context ctx, const char* name)
{
	((InterpreterContext* ) ctx)->nativeFunctionHT.remove (name);
	ExpressionList* userFunc = ((InterpreterContext* ) ctx)->functions.getPnt (name);
	if (userFunc) {
		userFunc->freeExpressions ();
		((InterpreterContext* ) ctx)->functions.remove (name);
		((InterpreterContext* ) ctx)->locationHT.remove (name);
	}
}

V1Data v1CreateData ()
{
	return (V1Data) new DataValue ();
}

void v1DeleteData (V1Data data)
{
	delete (DataValue*) data;
}

V1ArgList v1CreateArgList ()
{
	return (V1ArgList) new vector<DataValue*>;
}

void v1DeleteArgList (V1ArgList argList)
{
	delete (vector<DataValue*>*) argList;
}

void v1SetArgument (V1ArgList argList, int pos, V1Data data)
{
	((vector<DataValue*>*) argList)->resize (pos+1);
	(*((vector<DataValue*>*) argList))[pos]=(DataValue*) data;
}

int v1CallUserFunction (V1Context ctx, const char* name, V1ArgList argList, V1Data ret)
{
	DataValue* noParam = NULL;
	if (((vector<DataValue*>*) argList)->size ()==0) {
		noParam = new DataValue ();
		noParam->datatype = DataValue::DATATYPE_NO_PARAM;
		((vector<DataValue*>*) argList)->resize (1);
		(*((vector<DataValue*>*) argList))[0]=noParam;
	}
	int retCode = 0;
	try {
		retCode = callUserFunc (name, *(vector<DataValue*>*) argList, *(DataValue*) ret, *(InterpreterContext*) ctx);
	}
	catch (AbortException) {
		retCode=-2;
	}
	catch (WException& e) {
		((InterpreterContext*) ctx)->errText = e.getMessage ();
		if (wScriptErrorFunction)
			wScriptErrorFunction ("", 0, ((InterpreterContext*) ctx)->errText);
		retCode=-1;
	}
	if (noParam)
		delete noParam;
	return retCode;
}

void v1SetConstant (V1Context ctx, const char* name, double number, const char* str)
{
	((InterpreterContext* ) ctx)->_setConstant (name, number, str, *(InterpreterContext*) ctx);
}

void v1UnsetConstant (V1Context ctx, const char* name)
{
	((InterpreterContext* ) ctx)->symbols[0]->remove (name);
}

int v1ArgumentCount (V1ArgList argList)
{
	return ((vector<DataValue*>*) argList)->size ();
}

V1Data v1Argument (V1ArgList argList, int pos)
{
	return (V1Data) (*(vector<DataValue*>*) argList)[pos];
}

bool v1Argument1NoString (V1ArgList argList)
{
	DataValue* arg = (*(vector<DataValue*>*) argList)[0];
	return (arg->datatype==DataValue::DATATYPE_NO_PARAM ||arg->datatype>DataValue::DATATYPE_STR);
}

int v1Argument1Type (V1ArgList argList)
{
	return (*(vector<DataValue*>*) argList)[0]->datatype;
}

int v1Argument2Type (V1ArgList argList)
{
	return (*(vector<DataValue*>*) argList)[1]->datatype;
}

int v1Argument3Type (V1ArgList argList)
{
	return (*(vector<DataValue*>*) argList)[2]->datatype;
}

int v1Argument4Type (V1ArgList argList)
{
	return (*(vector<DataValue*>*) argList)[3]->datatype;
}

int v1Argument5Type (V1ArgList argList)
{
	return (*(vector<DataValue*>*) argList)[4]->datatype;
}

int v1Argument6Type (V1ArgList argList)
{
	return (*(vector<DataValue*>*) argList)[5]->datatype;
}

int v1Type (V1Data data)
{
	return ((DataValue*) data)->datatype;
}

int v1HandleType (V1Data data)
{
	if (((DataValue*) data)->hvalue)
		return ((Handle*) ((DataValue*) data)->hvalue)->handletype;
	return 0;
}


V1Handle v1Handle (V1Data data)
{
	switch (v1HandleType (data)) {
		case V1_HANDLETYPE_FILE:
			return (V1Handle) (FILE*) *(WFile*) (*(Handle*) ((DataValue*) data)->hvalue);
			break;
		case V1_HANDLETYPE_SOCKET:
			return (V1Handle) (int) *(WSocket*) (*(Handle*) ((DataValue*) data)->hvalue);
			break;
		case V1_HANDLETYPE_DIRECTORY:
			break;
		case V1_HANDLETYPE_SERVERSOCKET:
			return (V1Handle) (int) *(WServerSocket*) (*(Handle*) ((DataValue*) data)->hvalue);
			break;
		case V1_HANDLETYPE_THREAD:
			break;
	}
	return NULL;
}

const char* v1String (V1Data data)
{
	((DataValue*) data)->toString ();
	return ((DataValue*) data)->value.c_str ();
}

unsigned int v1StringSize (V1Data data)
{
	return ((DataValue*) data)->value.length ();
}

double v1Number (V1Data data)
{
	return  (double) (*(DataValue*) data);
}

bool v1Bool (V1Data data)
{
	return (bool) *((DataValue*) data);
}

V1Data v1ArrayValue (V1Data data, const char* key)
{
	return (V1Data) ((DataValue*) data)->arrayList.getPnt (key);
}

V1Data v1SetNumber (V1Data data, double number)
{
	((DataValue*) data)->numvalue = number;
	((DataValue*) data)->datatype = DataValue::DATATYPE_NUM;
	return data;
}

V1Data v1SetBool (V1Data data, bool flag)
{
	((DataValue*) data)->numvalue = flag ? 1 : 0;
	((DataValue*) data)->datatype = DataValue::DATATYPE_BOOL;
	return data;
}

V1Data v1SetString (V1Data data, const char* str)
{
	((DataValue*) data)->value.assign (str);
	((DataValue*) data)->datatype = DataValue::DATATYPE_STR;
	return data;
}

V1Data v1SetBuffer (V1Data data, const char* str, unsigned int length)
{
	((DataValue*) data)->value.assign (str, length);
	((DataValue*) data)->datatype = V1_DATATYPE_STR;
	return data;
}

V1Data v1SetHandle (V1Context ctx, V1Data data, int type, V1Handle handle)
{
	Handle* h = NULL;
	switch (type) {
		case V1_HANDLETYPE_FILE:
			try {
				h = new Handle;
				h->handletype = Handle::HANDLETYPE_FILE;
				h->handle = (void*) new WFile ((FILE*) handle);
			}
			catch (WException& e) {
				if (h->handle) {
					delete (WFile*) h->handle;
					h->handle = NULL;
				}
				delete h;
				h = NULL;
			}
			break;
		case V1_HANDLETYPE_SOCKET:
			try {
				h = new Handle;
				h->handletype = Handle::HANDLETYPE_SOCKET;
				h->handle = (void*) new WSocket (*(int*)  &handle);
			}
			catch (WException& e) {
				if (h->handle) {
					delete (WSocket*) h->handle;
					h->handle = NULL;
				}
				delete h;
				h = NULL;
			}
			break;
		case V1_HANDLETYPE_DIRECTORY:
			// Not supported
			break;
		case V1_HANDLETYPE_SERVERSOCKET:
			// Not supported
			break;
		case V1_HANDLETYPE_THREAD:
			// Not supported
			break;

	}
	if (h) {
		// Register handle
		h->freeFunction = freeHandle;
		((InterpreterContext*) ctx)->handleHT.put ((void*) h, h)->m_value;
		((DataValue*) data)->hvalue = h;
		((DataValue*) data)->datatype = DataValue::DATATYPE_HANDLE;
		return data;
	}
	return NULL;
}

V1Data v1SetArray (V1Data data)
{
	((DataValue*) data)->arrayList.removeAll ();
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return data;
}

V1Data v1PutArrayString (V1Data data, const char* key, const char* str)
{
	DataValue v (str);
	DataValue* newData = &((DataValue*) data)->arrayList.put (key, v)->m_value;
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return (V1Data) newData;
}

V1Data v1PutArrayBuffer (V1Data data, const char* key, const char* str, unsigned int length)
{
	DataValue v;
	DataValue* newData = &((DataValue*) data)->arrayList.put (key, v)->m_value;
	newData->value.assign (str, length);
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return (V1Data) newData;
}

V1Data v1PutArrayNumber (V1Data data, const char* key, double number)
{
	DataValue v (number);
	DataValue* newData = &((DataValue*) data)->arrayList.put (key, v)->m_value;
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return (V1Data) newData;
}

V1Data v1PutArrayBool (V1Data data, const char* key, bool flag)
{
	DataValue v (flag);
	DataValue* newData = &((DataValue*) data)->arrayList.put (key, v)->m_value;
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return newData;
}

V1Data v1PutArrayArray (V1Data data, const char* key)
{
	DataValue v;
	DataValue* newData = &((DataValue*) data)->arrayList.put (key, v)->m_value;
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	newData->datatype = V1_DATATYPE_ARRAY;
	return newData;
}

V1Data v1PushArrayString (V1Data data, const char* str)
{
	DataValue v (str);
	DataValue* newData = &((DataValue*) data)->arrayList.push (v)->m_value;
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return (V1Data) newData;
}

V1Data v1PushArrayBuffer (V1Data data, const char* str, unsigned int length)
{
	DataValue v;
	DataValue* newData = &((DataValue*) data)->arrayList.push (v)->m_value;
	newData->value.assign (str, length);
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return (V1Data) newData;
}

V1Data v1PushArrayNumber (V1Data data, double number)
{
	DataValue v (number);
	DataValue* newData = &((DataValue*) data)->arrayList.push (v)->m_value;
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return (V1Data) newData;
}

V1Data v1PushArrayBool (V1Data data, bool flag)
{
	DataValue v (flag);
	DataValue* newData = &((DataValue*) data)->arrayList.push (v)->m_value;
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	return newData;
}

V1Data v1PushArrayArray (V1Data data)
{
	DataValue v;
	DataValue* newData = &((DataValue*) data)->arrayList.push (v)->m_value;
	((DataValue*) data)->datatype = V1_DATATYPE_ARRAY;
	newData->datatype = V1_DATATYPE_ARRAY;
	return newData;
}

void v1UnsetArrayKey (V1Data data, const char* key)
{
	((DataValue*) data)->arrayList.remove (key);
}

unsigned int v1ArraySize (V1Data data)
{
	return ((DataValue*) data)->arrayList.size ();
}

void v1ArrayIterate (V1Data data)
{
	((DataValue*) data)->arrayList.reset ();
}

V1Data v1ArrayNext (V1Data data, const char** key)
{
	ArrayHT::WHash* hash;
	V1Data value=NULL;
	if (((DataValue*) data)->arrayList.hasMoreElements ()) {
		value = ((DataValue*) data)->arrayList.nextValuePnt (&hash);
		if (key)
			*key = (const char*) hash->m_key.getBuf ();
	}
	return value;
}

V1Data v1Reference (V1Data data)
{
	return (DataValue*) ((DataValue*) data)->refPnt;
}

void v1WarnInterprete (V1Context ctx, const char* text)
{
	((InterpreterContext*) ctx)->warnInterprete (text);
}

void v1AbortInterprete (V1Context ctx, const char* text)
{
	((InterpreterContext*) ctx)->abortInterprete (text);
}

V1Data v1Symbol (V1Context ctx, const char* name, int table)
{
	return (V1Data) ((InterpreterContext*) ctx)->symbols[table]->getPnt (name);
}

V1Data v1CreateSymbol (V1Context ctx, const char* name, int table)
{
	DataValue v;
	return (V1Data) &((InterpreterContext*) ctx)->symbols[table]->put (name, v)->m_value.value;
}

void v1RemoveSymbol (V1Context ctx, const char* name, int table)
{
	((InterpreterContext*) ctx)->symbols[table]->remove (name);
}

void v1SymbolIterate (V1Context ctx, int table)
{
	((InterpreterContext*) ctx)->symbols[table]->reset ();
}

V1Data v1SymbolNext (V1Context ctx, int table, const char** name)
{
	SymbolTable::WHash* hash;
	V1Data value=NULL;
	if (((InterpreterContext*) ctx)->symbols[table]->hasMoreElements ()) {
		value = ((InterpreterContext*) ctx)->symbols[table]->nextValuePnt (&hash);
		if (name)
			*name = (const char*) hash->m_key.getBuf ();
	}
	return value;
}

unsigned int v1SymbolTableCount (V1Context ctx)
{
	return ((InterpreterContext*) ctx)->symbolTblCnt;
}

int v1CurrentSymbolTable (V1Context ctx)
{
	return ((InterpreterContext*) ctx)->funcDeep;
}

unsigned int v1Flags (V1Data data)
{
	return ((DataValue*) data)->flags;
}

void v1SyncEnter (V1Context ctx)
{
	WSystem::enterMutex (((InterpreterContext*)ctx)->mutex);
}

void v1SyncLeave (V1Context ctx)
{
	WSystem::leaveMutex (((InterpreterContext*)ctx)->mutex);
}

bool v1Aborted (V1Context ctx)
{
	return ((InterpreterContext*)ctx)->fAbort==1;
}

bool v1Exited (V1Context ctx)
{
	return ((InterpreterContext*)ctx)->fAbort==2;
}

const char* v1LastError (V1Context ctx)
{
	return (const char*) ((InterpreterContext*)ctx)->errText;
}

int v1ExitCode (V1Context ctx)
{
	return ((InterpreterContext*)ctx)->exitCode;
}

double v1Version ()
{
	return WSCRIPT_VERSION;
}
