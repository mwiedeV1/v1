/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: V1 Script
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef AST_H
#define AST_H

#include "wscript.h"
#include "WSocket.h"
#include "WThread.h"


class AbortException : public WException {
public:
	inline AbortException (WCSTR msg, int id) : WException ()
	{
		set ("AbortException", msg, id);
	};
};


// Predefinitions
class InterpreterContext;

// Thread class
class WScriptThread : public WRunable {
public:
	WScriptThread ();
	~WScriptThread ();
	void run ();  // defined in interpret.cpp
	void create (vector<DataValue*>& argvalues, InterpreterContext& ctx);
	bool start (bool fWaitStarted=true);
	void finish ();
	bool join (unsigned int ms=0);
	bool wait (unsigned int ms=0);
	void signal ();
	void suspend ();
	void resume ();
	void kill ();
	bool isActive () const;
	bool isFinish () const;
private:
	vector<DataValue*> argvalues;
	vector<DataValue> argvalues2; // Duplicated arguments
	InterpreterContext* ctx;
	bool fFinish;
	// Temporary status vars
	int loopDeep;
	int switchDeep;
	int funcDeep;
	int flag;
	SymbolTable** parentSymbols; // Symbols of calling parent
	SymbolTable** threadSymbols; // Complete symbol table of thread
	unsigned int line;
	WSemaphore semaphore;
};

// Function pointers used by modules (DLL)
typedef int (* WSCRIPT_WARN_INTERPRETE) (const WException&, InterpreterContext&);
typedef int (* WSCRIPT_ABORT_INTERPRETE) (const char*, InterpreterContext&);
typedef int (* WSCRIPT_CALLUSERFUNC) (const string&, vector<DataValue*>&, DataValue&, InterpreterContext&, int);
typedef int (* WSCRIPT_SETCONSTANTFUNC) (const char*, double, const char*, InterpreterContext&);
typedef int (* WSCRIPT_UNSETCONSTANTFUNC) (const char*, InterpreterContext&);
typedef int (* WSCRIPT_BINARYPRINTOUTFUNC) (void*, unsigned int, const char*, InterpreterContext&);


// Context class of the interpreter
class InterpreterContext {

public:
	enum {
		PROGRAM_MODE_DEFAULT = 0,
		PROGRAM_MODE_WEB = 1, // CGI
		PROGRAM_MODE_WINDOWS = 2, // WIN32 Window
		FLAG_RETURN = 101,
		FLAG_BREAK  = 102,
		FLAG_CONTINUE = 103,
	};
	InterpreterContext ();
	~InterpreterContext ();

	// Methods of interpreter
	int eval (WCSTR code, unsigned int size, DataValue& retvalue);
	int execute (WCSTR filename, int fRequireOnce=0);
	void abortInterprete (WCSTR str, bool fPreInterprete=false);
	void warnInterprete (WCSTR str);
	void warnInterprete (const WException& e);
	void printHeaders ();
	WCSTR getFilenameByIdx (unsigned int idx) const;
	unsigned int getIdxByFilename (WCSTR filename) const;
	unsigned int registerFilename (WCSTR filename);
	void cleanupAll (); // Cleanup all handles, arrays ...

	// Status vars of interpreter
	int programMode; // Current program mode
	int exitCode; // Exit code set with exit() function
	bool fAutoNL; // Make auto new line after print
	bool fShowWarnings; // Show warnings, e.g. "file not found"
	bool fOutputNoScript; // Output of non script code outside <?v ?>
	bool fOnlySyntaxCheck; // No interpretation, only check syntax
	bool fHeadersSent; // Header was sent in Web/CGI-Mode
	static WSCRIPT_THREAD_SAFE unsigned int loopDeep; // Current loop depth (while, for)
	static WSCRIPT_THREAD_SAFE unsigned int switchDeep; // Current switch depth
	static WSCRIPT_THREAD_SAFE unsigned int funcDeep; // Current function depth
	static WSCRIPT_THREAD_SAFE unsigned int flag; // Current flag with basic commands for interpreter
	static WSCRIPT_THREAD_SAFE unsigned int line; // Combined (fileIdx<<24)|line
	static InterpreterContext* ctx; // Pointer to interpreter context in current thread
	// static WSCRIPT_THREAD_SAFE const char* nativeFunctionName; // Name of current native function
	WKeyValueHT headerHT; // HTTP-Headers, set with function header() in Web/CGI-Mode
	WString errText; // Last interpreter error text
	int fAbort; // Interpreter aborted
	WScriptUserData userData; // User defined data
	// static WSCRIPT_THREAD_SAFE SymbolTableList* symbols; // 0 = constants, system vars, 1 = global vars, 2 ... n = local vars of functions  ...
	static WSCRIPT_THREAD_SAFE SymbolTable** symbols; // 0 = constants, system vars, 1 = global vars, 2 ... n = local vars of functions  ...
	static WSCRIPT_THREAD_SAFE unsigned int symbolTblCnt; // Count of symbol tables
	FunctionDictionary functions; // User functions
	FunctionLocationHT locationHT; // File locations of the functions
	File2IdxHT file2IdxHT; // Filename to Index
	Idx2FileHT idx2FileHT; // Index to Filename
	HandleHT handleHT; // Handles
	WStringList tmpFileList; // Temporary files
	WSCRIPT_NATIVE_FUNCTIONHT nativeFunctionHT; // Native functions
	static WSCRIPT_THREAD_SAFE WScriptThread* currThread; // Current thread object
	WMUTEX mutex; // Mutext for thread synchronization
	DLLModuleHT dllModuleHT; // List of loaded DLL modules

	// Static pointers to methods for DLL, because DLL cannot call class methods directly
	WSCRIPT_WARN_INTERPRETE _warnInterprete;
	WSCRIPT_ABORT_INTERPRETE _abortInterprete;
	WSCRIPT_CALLUSERFUNC _callUserFunc;
	WSCRIPT_SETCONSTANTFUNC _setConstant;
	WSCRIPT_UNSETCONSTANTFUNC _unsetConstant;
	WSCRIPT_BINARYPRINTOUTFUNC _binaryPrintOut;
  
	int securityMode; // Current security mode
};

// Universal Handle class
class Handle {

public:
	enum {
		HANDLETYPE_NONE = 0,
		HANDLETYPE_FILE = 1,
		HANDLETYPE_SOCKET = 2,
		HANDLETYPE_DIRECTORY = 3,
		HANDLETYPE_SERVERSOCKET = 4,
		HANDLETYPE_THREAD = 5,
	};

	Handle ()
	{
		handle = NULL;
		handletype = 0;
		freeFunction = NULL;
		param = NULL;
	}

	~Handle ()
	{
		if (handle && freeFunction) {
			freeFunction (handle, handletype);
		}
	}

	void* handle; // Pointer to file, socket ...
	int handletype; // Type of handle
	void* param; // Optional parameter
	WSCRIPT_FREE_HANDLE_FUNCTION freeFunction; // Function to free the handle

	operator WFile* ()
	{
		return (WFile*) handle;
	};

	operator WSocket* ()
	{
		return (WSocket*) handle;
	};
	operator WServerSocket* ()
	{
		return (WServerSocket*) handle;
	};

	operator WDirectoryEntries* ()
	{
		return (WDirectoryEntries*) handle;
	}

	operator WScriptThread* ()
	{
		return (WScriptThread*) handle;
	}
};

// Universal DataValue class
class DataValue {

public:

	enum {
		DATATYPE_NULL      = 0,  // NULL
		DATATYPE_NO_PARAM  = 1,  // No parameter
		DATATYPE_NUM       = 2,  // Number, int and double
		DATATYPE_BOOL      = 3,  // Boolean
		DATATYPE_STR       = 4,  // String
		DATATYPE_HANDLE    = 5,  // Files, sockets ...
		DATATYPE_MATRIX    = 9,  // Matrix
		DATATYPE_ARRAY     = 10, // Array

		/* Special datatypes */
		DATATYPE_MATRIXDEF = 0x7FFFFFFB, // Definition of matrix
		DATATYPE_ARRAYDEF  = 0x7FFFFFFC, // Definition of arrays
		DATATYPE_ARRAYKEY  = 0x7FFFFFFD, // Array key => value
		DATATYPE_REFERENCE = 0x7FFFFFFE, // Only used by parameters
		DATATYPE_OUTPUT    = 0x7FFFFFFF, // Outside <?v1 ... ?>

		/* Flags */
		FLAGS_USEREF        = 0x01, // Use reference pointer
		FLAGS_NOWRITE       = 0x10, // Only readable
		FLAGS_DUPARG        = 0x20, // Duplicate complete argument to function stack
		FLAGS_SYNCHRONIZE   = 0x40  // Variable is synchronized for threads
	};


	void initialize ()
	{
		datatype=0;
		numvalue=0;
		hvalue=0;
		refPnt = NULL;
		fNumberString = false;
		flags = 0;
	}

	DataValue ()
	{
		initialize ();
	}

	// Set type automatically from string
	void setAutoDataType (int fOnlyNum=0)
	{
		char* end;

		if (fOnlyNum) {
			// Only convert string to number
			if (datatype==DATATYPE_STR) {
				if (value.length ()==0) {
					numvalue=0;
					fNumberString=true;
				}
				else {
					numvalue = strtod(value.c_str(), &end);
					if (end == value.c_str() || *end != '\0') {
						fNumberString = false; // DATATYPE_STR
					}
					else {
						fNumberString = true; //  DATATYPE_NUM;
					}
				}
			}
			return;
		}

		// Find datatype automatically
		numvalue = strtod (value.c_str(), &end);
		if (value.length ()==0 || end == value.c_str() || *end != '\0') {
			datatype = DATATYPE_STR;
		}
		else {
			datatype = DATATYPE_NUM;
		}
		if (value=="true") {
			numvalue=1;
			value="1";
			datatype = DATATYPE_BOOL;
		}
		else if (value=="false") {
			numvalue=0;
			value="";
			datatype = DATATYPE_BOOL;
		}
	}

	DataValue (string s)
	{
		initialize ();
		value = s;
		datatype = DATATYPE_STR;
	}
	DataValue (WCSTR s)
	{
		initialize ();
		value = s;
		datatype = DATATYPE_STR;
	}
	DataValue (bool b)
	{
		initialize ();
		datatype = DATATYPE_BOOL;
		numvalue = b ? 1 : 0;
	}

	DataValue (int d)
	{
		initialize ();
		datatype=DATATYPE_NUM;
		numvalue = d;
	}

	DataValue (W32 d)
	{
		initialize ();
		datatype=DATATYPE_NUM;
		numvalue = d;
	}

	DataValue (double d)
	{
		initialize ();
		datatype=DATATYPE_NUM;
		numvalue = d;
	}

	DataValue (Handle* h)
	{
		initialize ();
		datatype = DATATYPE_HANDLE;
		// Store a handle also in number value for comparations!
		numvalue = (double) (uint64_t) h;
		hvalue = h;
	}

	mutable string value; // string
	double numvalue; // number
	void* hvalue; // handle
	ArrayHT arrayList; // Array values

	unsigned int datatype;
	DataValue* refPnt; // reference pointer
	bool fNumberString; // Number with datatype string
	unsigned int flags; // flags

	inline operator const string () const
	{
		toString ();
		return value;
	};
	inline operator const WCSTR () const
	{
		toString ();
		return value.c_str();
	};
	inline operator double () const
	{
		return numvalue;
	};
	inline operator int () const
	{
		return (int) numvalue;
	};
	inline operator unsigned int () const
	{
		return (unsigned int) numvalue;
	};
	inline operator bool () const
	{
		if (datatype==DataValue::DATATYPE_STR) {
			if (!fNumberString)
				return value.length ()>0; // Real strings always true if not empty!
		}
		else if (datatype>=DATATYPE_ARRAY)
			return arrayList.size ()>0; // Array are always true if not empty
		return numvalue!=0;
	};

	inline bool bool2 () const
	{
		if (datatype==DataValue::DATATYPE_STR) {
			return value.length ()>0; // Strings always true if not empty!
		}
		else if (datatype>=DATATYPE_ARRAY)
			return arrayList.size ()>0; // Array are always true if not empty
		return numvalue!=0;
	};

	inline operator Handle* () const
	{
		return (Handle*) hvalue;
	};
	inline operator void* () const
	{
		return hvalue;
	};

	const char* describeDataType (int ownType=-1) const
	{
		switch (ownType!=-1 ? ownType : datatype) {
			case DATATYPE_NUM:
				return "number";
				break;
			case DATATYPE_STR:
				return "string";
				break;
			case DATATYPE_BOOL:
				return "boolean";
				break;
			case DATATYPE_HANDLE:
				return "handle";
				break;
			case DATATYPE_NULL:
			case DATATYPE_NO_PARAM:
				return "null";
				break;
			case DATATYPE_MATRIX:
				return "matrix";
				break;
			default:
				if ((ownType!=-1 ? ownType : datatype)>=DATATYPE_ARRAY)
					return "array";
				break;
		}
		return "unknown type";
	}

	/* Operators */
	inline DataValue operator +(const DataValue& o2) const
	{

		// Array addition
		if (datatype>=DATATYPE_ARRAY && o2.datatype>=DATATYPE_ARRAY) {
			DataValue dataValue;
			dataValue.arrayList = arrayList;
			dataValue.datatype = DataValue::DATATYPE_ARRAY;
			while (o2.arrayList.hasMoreElements ()) {
				if (!dataValue.arrayList.isKey (o2.arrayList.nextKey ()))
					dataValue.arrayList.put (o2.arrayList.nextKey (), o2.arrayList.nextValue ());
			}
			return dataValue;
		}
		return numvalue+o2.numvalue;
	}


	inline DataValue& operator +=(const DataValue& o2)
	{

		// Array addition
		if (datatype>=DATATYPE_ARRAY && o2.datatype>=DATATYPE_ARRAY) {
			while (o2.arrayList.hasMoreElements ()) {
				if (!this->arrayList.isKey (o2.arrayList.nextKey ()))
					this->arrayList.put (o2.arrayList.nextKey (), o2.arrayList.nextValue ());
			}
			return *this;
		}
		this->numvalue+=o2.numvalue;
		this->datatype = DATATYPE_NUM;
		return *this;
	}


	typedef struct {
		unsigned int cnt, cntEqual;
		bool fStrict;
		int operation;
	} SArrayCmp;

	static void compareArrays (ArrayHT& array1, ArrayHT& array2, SArrayCmp& cmp)
	{
		bool ret = false;
		if (array1.size ()==0)
			cmp.cnt = array2.size ();
		else if (array2.size ()==0)
			cmp.cnt = array1.size ();
		array1.reset ();
		while (array1.hasMoreElements ()) {

			DataValue* value1 = array1.nextValuePnt ();
			DataValue* value2 = NULL;
			if (array2.hasMoreElements ()) {
				value2 = array2.nextValuePnt ();
			}
			else {
				break;
			}
			// Check value
			if (value1->datatype<DataValue::DATATYPE_ARRAY) {
				cmp.cnt++;
				if (!cmp.fStrict) {
					value1->setAutoDataType (true);
					value2->setAutoDataType (true);
					if (cmp.operation==0)
						ret = *value1==*value2;
					else if (cmp.operation==1)
						ret = *value1>*value2;
					else if (cmp.operation==2)
						ret = *value1<*value2;
					else if (cmp.operation==3)
						ret = *value1>=*value2;
					else if (cmp.operation==4)
						ret = *value1<=*value2;
					if (ret)
						cmp.cntEqual++;
				}
				else if (value1->datatype==value2->datatype) {
					if (value1->datatype!=DataValue::DATATYPE_STR) {
						if (cmp.operation==0)
							ret = value1->numvalue==value2->numvalue;
						else if (cmp.operation==1)
							ret = value1->numvalue>value2->numvalue;
						else if (cmp.operation==2)
							ret = value1->numvalue<value2->numvalue;
						else if (cmp.operation==3)
							ret = value1->numvalue>=value2->numvalue;
						else if (cmp.operation==4)
							ret = value1->numvalue<=value2->numvalue;
					}
					else {
						if (cmp.operation==0)
							ret = value1->value==value2->value;
						else if (cmp.operation==1)
							ret = value1->value>value2->value;
						else if (cmp.operation==2)
							ret = value1->value<value2->value;
						else if (cmp.operation==3)
							ret = value1->value>=value2->value;
						else if (cmp.operation==4)
							ret = value1->value<=value2->value;
					}
					if (ret)
						cmp.cntEqual++;
				}
				else {
					// Datatype mismatch

				}
			}
			else if (value2->datatype>=DataValue::DATATYPE_ARRAY) {
				// Compare arrays
				compareArrays (value1->arrayList, value2->arrayList, cmp);
			}
			else {
				break;
			}
		}
	}

	inline DataValue operator ==(const DataValue& o2) const
	{
		if (datatype!=o2.datatype) {
			int check = datatype + o2.datatype;
			if (check==6) {
				// number == string
				if (!fNumberString && !o2.fNumberString)
					return false;
			}
			else if (check==DATATYPE_STR)
				return this->bool2 () == o2.bool2 (); // string == null
			else
				return (bool) *this == (bool) o2;
		}
		else if (datatype==DATATYPE_STR) {
			if (!fNumberString && !o2.fNumberString)
				// Compare real strings which are both no numbers
				return (bool) !strcmp (value.c_str(), o2.value.c_str());
			else if (!(fNumberString && o2.fNumberString)) {
				return false; // Compare number with no number always false
			}
		}
		else if (datatype==DATATYPE_ARRAY) {
			// Compare arrays
			DataValue::SArrayCmp cmp;
			memset (&cmp, 0, sizeof (cmp));
			DataValue::compareArrays ((ArrayHT&) this->arrayList, (ArrayHT&) o2.arrayList, cmp);
			return cmp.cnt == cmp.cntEqual;
		}

		return (bool) (numvalue==o2.numvalue);
	}

	inline DataValue operator >(const DataValue& o2) const
	{
		if (datatype!=o2.datatype) {
			int check = datatype + o2.datatype;

			if (check==6) {
				// number > string | string > number
				return (bool) (numvalue>o2.numvalue);
			}
			else if (check==DATATYPE_STR)
				return this->bool2 () > o2.bool2 (); // string > null | null > string
			else
				return (bool) *this > (bool) o2;
		}
		else if (datatype==DATATYPE_STR) {
			if (!(fNumberString && o2.fNumberString)) {
				// Not two numbers, compare as string
				return (bool) (strcmp (value.c_str(), o2.value.c_str())>0);
			}
		}
		else if (datatype==DATATYPE_ARRAY) {
			// Compare arrays
			DataValue::SArrayCmp cmp;
			memset (&cmp, 0, sizeof (cmp));
			cmp.operation = 1;
			DataValue::compareArrays ((ArrayHT&) this->arrayList, (ArrayHT&) o2.arrayList, cmp);
			return cmp.cntEqual>0;
		}
		return (bool) (numvalue>o2.numvalue);
	}

	inline DataValue operator <(const DataValue& o2) const
	{
		if (datatype!=o2.datatype) {
			int check = datatype + o2.datatype;

			if (check==6) {
				// number < string | string < number
				return (bool) (numvalue<o2.numvalue);
			}
			else if (check==DATATYPE_STR)
				return this->bool2 () < o2.bool2 (); // null < string | string < null
			else
				return (bool) *this < (bool) o2;
		}
		else if (datatype==DATATYPE_STR) {
			if (!(fNumberString && o2.fNumberString)) {
				// Not two numbers, compare as string
				return (bool) (strcmp (value.c_str(), o2.value.c_str())<0);
			}
		}
		else if (datatype==DATATYPE_ARRAY) {
			// Compare arrays
			DataValue::SArrayCmp cmp;
			memset (&cmp, 0, sizeof (cmp));
			cmp.operation = 2;
			DataValue::compareArrays ((ArrayHT&) this->arrayList, (ArrayHT&) o2.arrayList, cmp);
			return cmp.cntEqual>0;
		}
		return (bool) (numvalue<o2.numvalue);
	}

	inline DataValue operator >=(const DataValue& o2) const
	{
		if (datatype!=o2.datatype) {
			int check = datatype + o2.datatype;
			if (check==6)
				// number >= string | string >= number
				return (bool) (numvalue>=o2.numvalue);
			else if (check==DATATYPE_STR)
				return this->bool2 () >= o2.bool2 (); // null >= string | null >= number
			else
				return (bool) *this >= (bool) o2;
		}
		else if (datatype==DATATYPE_STR) {
			if (!(fNumberString && o2.fNumberString)) {
				// Not two numbers, compare as string
				return (bool) (strcmp (value.c_str(), o2.value.c_str())>=0);
			}
		}
		else if (datatype==DATATYPE_ARRAY) {
			// Compare arrays
			DataValue::SArrayCmp cmp;
			memset (&cmp, 0, sizeof (cmp));
			cmp.operation = 3;
			DataValue::compareArrays ((ArrayHT&) this->arrayList, (ArrayHT&) o2.arrayList, cmp);
			return cmp.cntEqual>0;
		}
		return (bool) (numvalue>=o2.numvalue);
	}

	inline DataValue operator <=(const DataValue& o2) const
	{

		if (datatype!=o2.datatype) {
			int check = datatype + o2.datatype;

			if (check==6)
				// number <= string | string <= number
				return (bool) (numvalue<=o2.numvalue);
			else if (check==DATATYPE_STR)
				return this->bool2 () <= o2.bool2 (); // // null <= string | null <= number
			else
				return (bool) *this <= (bool) o2;
		}
		else if (datatype==DATATYPE_STR) {
			if (!(fNumberString && o2.fNumberString)) {
				// Not two numbers, compare as string
				return (bool) (strcmp (value.c_str(), o2.value.c_str())<=0);
			}
		}
		else if (datatype==DATATYPE_ARRAY) {
			// Compare arrays
			DataValue::SArrayCmp cmp;
			memset (&cmp, 0, sizeof (cmp));
			cmp.operation = 4;
			DataValue::compareArrays ((ArrayHT&) this->arrayList, (ArrayHT&) o2.arrayList, cmp);
			return cmp.cntEqual>0;
		}
		return (bool) (numvalue<=o2.numvalue);
	}

	inline DataValue operator !=(const DataValue& o2) const
	{
		return !(this->operator==(o2));
	}



	inline WCSTR toString (bool fAll=false) const
	{
		/*
		if (value.length ()>0)
		    return value.c_str ();
		*/
		if (datatype==DATATYPE_NUM) {
			char cstr[40];
			sprintf(cstr, "%.14g", numvalue);
			value=string(cstr);
		}
		else if (datatype==DATATYPE_BOOL) {
			value = numvalue ? "1" : "";
		}
		return value.c_str ();
	}
};

// The Expression
#define OPCODE_EQUAL_OP     0x0100
#define OPCODE_SYNTAX_OP    0x0200
#define OPCODE_FUNCTION     0x0400
#define OPCODE_MAGIC_FUNC   0x0800
#define OPCODE_QUICK_FUNC   0x1000

#define OPCODE_EQUAL             1
#define OPCODE_LOOP              2
#define OPCODE_IF1               3
#define OPCODE_IF2               4
#define OPCODE_WHEN1             5
#define OPCODE_WHEN2             6
#define OPCODE_WHEN3             7
#define OPCODE_GLOBAL            8
#define OPCODE_UNSET             9
#define OPCODE_ISSET            10
#define OPCODE_ASSIGN           11
#define OPCODE_CASE             12
#define OPCODE_DEFAULT          13
#define OPCODE_WHILE            14
#define OPCODE_SWITCH           15
#define OPCODE_DO_WHILE         16
#define OPCODE_FOR              17
#define OPCODE_FOREACH          18
#define OPCODE_FUNCDEF          19
#define OPCODE_RETURN           20
#define OPCODE_BREAK            21
#define OPCODE_CONTINUE         22
#define OPCODE_BOOL_AND         23
#define OPCODE_BOOL_OR          24

#define OPCODE_CONSTANT        100
#define OPCODE_VARIABLE        101
#define OPCODE_ARRAY           102

#define EXPRESSION_FLAG_NO_STORE 0
#define EXPRESSION_FLAG_STORE    1

#define IS_OPCODE(x) (opCode&0xFF)==x


class ExpressionList {
public:
	friend  class ExpressionList;

	ExpressionList ()
	{
		m_list = NULL;
		m_size = 0;
		m_maxSize = 0;
	}

	~ExpressionList ()
	{

	}

	void freeExpressions (InterpreterContext* ctx=NULL)
	{
		_freeExpressions (*this, ctx);
	}

	Expression& push_back (Expression& expression)
	{
		if (m_size+1>=m_maxSize) {
			m_maxSize++;
			if (!m_list)
				m_list = (Expression**) malloc (m_maxSize * sizeof (Expression*));
			else
				m_list = (Expression**) realloc (m_list, m_maxSize * sizeof (Expression*));
		}
		m_list[m_size]=&expression;
		m_size++;
		return expression;
	}

	void append (ExpressionList& list)
	{
		if (m_size+list.size ()>=m_maxSize) {
			m_maxSize+=list.size ();
			if (!m_list)
				m_list = (Expression**) malloc (m_maxSize * sizeof (Expression*));
			else
				m_list = (Expression**) realloc (m_list, m_maxSize * sizeof (Expression*));
		}
		memcpy ((void*) &m_list[m_size], (void*) list.m_list, list.size ()*sizeof (Expression*));
		m_size+=list.size ();
	}

	void reserve (unsigned int size)
	{
		m_maxSize = size;
		if (m_list)
			m_list = (Expression**) realloc (m_list, m_maxSize * sizeof (Expression*));
		else
			m_list = (Expression**) malloc (m_maxSize * sizeof (Expression*));
		for (unsigned int i=m_size; i<size; i++)
			m_list[m_size+i]= NULL;
		if (m_size>size)
			m_size=size;
	}

	inline unsigned int size ()
	{
		return m_size;
	}

	inline Expression& operator [] (unsigned int i)
	{
		return *m_list[i];
	}

private:
	Expression** m_list;
	unsigned int m_size;
	unsigned int m_maxSize;
	void _freeExpressions (ExpressionList& list, InterpreterContext* ctx=NULL);
};


class Expression {
public:
	Expression();
	// Expression(const W8* byteBuf, unsigned int readOffset, unsigned int maxReadOffset);
	Expression(string, ExpressionList, int type, unsigned int line, unsigned int opCode=0);
	Expression(string, ExpressionList, std::string* constvalue, int type, unsigned int line, unsigned int opCode=0);
	~Expression ()
	{
		if (constDataValue) delete constDataValue;
		if (constvalue) delete constvalue;
	}
	DataValue* calc (InterpreterContext& ctx, DataValue& retvalue, unsigned int flags=EXPRESSION_FLAG_NO_STORE);  // defined in interpret.cpp
	// void dumpByteCode (WString& str) const;
	// int loadByteCode (const W8* byteBuf, unsigned int readOffset, unsigned int maxReadOffset);

	string operation;
	string* constvalue;
	ExpressionList arguments;
	int datatype;
	unsigned int line;
	unsigned int opCode;
	void* userFunc;
private:
	DataValue* constDataValue;
};

#endif
