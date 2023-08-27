/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: V1 Script
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "ast.h"

#include <ostream>
#include <dlfcn.h>

extern unsigned int yyLine;
extern unsigned int yyInputFileIdx;

// Thread safe static variables
WSCRIPT_THREAD_SAFE unsigned int InterpreterContext::loopDeep = 0;
WSCRIPT_THREAD_SAFE unsigned int InterpreterContext::switchDeep = 0;
WSCRIPT_THREAD_SAFE unsigned int InterpreterContext::funcDeep = 0;
WSCRIPT_THREAD_SAFE unsigned int InterpreterContext::flag = 0;
WSCRIPT_THREAD_SAFE unsigned int InterpreterContext::line = 0;
WSCRIPT_THREAD_SAFE WScriptThread *InterpreterContext::currThread = NULL;
WSCRIPT_THREAD_SAFE SymbolTable **InterpreterContext::symbols = NULL;
WSCRIPT_THREAD_SAFE unsigned int InterpreterContext::symbolTblCnt = 0;
// WSCRIPT_THREAD_SAFE const char* InterpreterContext::nativeFunctionName = "";
InterpreterContext *InterpreterContext::ctx = NULL;

bool g_fHeadersSent = false;
int g_programMode = 0;

Expression::Expression()
{
	line = 0;
	datatype = 0;
	userFunc = NULL;
	constvalue = NULL;
	constDataValue = NULL;
	opCode = 0;
}

Expression::Expression(string op, ExpressionList args, int type, unsigned int line, unsigned int opCode)
{
	this->operation = op;
	this->arguments = args;
	this->constvalue = NULL;
	this->line = line;
	this->datatype = type;
	this->constDataValue = NULL;
	this->userFunc = NULL;
	this->opCode = opCode;
}

Expression::Expression(string op, ExpressionList args, std::string *constval, int type, unsigned int line, unsigned int opCode)
{
	this->operation = op;
	this->arguments = args;
	this->constvalue = constval;
	this->line = line;
	this->datatype = type;
	this->constDataValue = NULL;
	this->userFunc = NULL;
	this->opCode = opCode;
}

WScriptThread::WScriptThread() : WRunable()
{
	this->ctx = NULL;
	this->fFinish = false;
	this->loopDeep = 0;
	this->switchDeep = 0;
	this->funcDeep = 0;
	this->flag = 0;
	this->line = 0;
	this->threadSymbols = NULL;
}

WScriptThread::~WScriptThread()
{
	kill();
}

void WScriptThread::finish()
{
	this->fFinish = true;
}

void WScriptThread::create(vector<DataValue *> &argvalues, InterpreterContext &ctx)
{
	if (!m_thread.isActive()) {
		// Duplicate argvalues
		for (W32 i = 0; i < argvalues.size(); i++) {
			this->argvalues2.push_back(*argvalues[i]);
		}
		for (W32 i = 0; i < argvalues.size(); i++) {
			this->argvalues.push_back(&this->argvalues2[i]);
		}
		this->ctx = &ctx;
		this->loopDeep = ctx.loopDeep;
		this->switchDeep = ctx.switchDeep;
		this->funcDeep = ctx.funcDeep;
		this->flag = ctx.flag;
		this->line = ctx.line;
		this->parentSymbols = ctx.symbols;
	}
}

bool WScriptThread::start(bool fWaitStarted)
{
	if (!m_thread.isActive()) {
		fFinish = false;
		if (fWaitStarted) {
			semaphore.close();
			semaphore.create();
			if (m_thread.start(this))
				semaphore.wait(); // Wait until thread function was started
		}
		else {
			m_thread.start(this);
		}
		return true;
	}
	return false;
}

bool WScriptThread::join(unsigned int ms)
{
	return m_thread.join(ms);
}

bool WScriptThread::wait(unsigned int ms)
{
	return m_thread.wait(ms);
}

void WScriptThread::signal()
{
	return m_thread.signal();
}

void WScriptThread::suspend()
{
	return m_thread.suspend();
}

void WScriptThread::resume()
{
	return m_thread.resume();
}

void WScriptThread::kill()
{
	m_thread.kill();
	if (this->threadSymbols) {
		delete this->threadSymbols;
		this->threadSymbols = NULL;
	}
}

bool WScriptThread::isActive() const
{
	return m_thread.isActive();
}

bool WScriptThread::isFinish() const
{
	return this->fFinish;
}

int staticWarnInterprete(const WException &e, InterpreterContext &ctx)
{
	ctx.warnInterprete(e);
	return 0;
}

int staticAbortInterprete(const char *text, InterpreterContext &ctx)
{
	ctx.abortInterprete(text);
	return 0;
}

int staticBinaryPrintOut(void *buf, unsigned int size, const char *contentType, InterpreterContext &ctx)
{
	if (contentType != NULL) {
		ctx.headerHT.put("CONTENT-TYPE", contentType);
	}
	ctx.printHeaders();
	fflush(stdout);
	int nBytesWritten = fwrite(buf, size, 1, stdout);
	return 0;
}

int staticSetConstant(const char *name, double numval, const char *textval, InterpreterContext &ctx)
{

	if (textval != NULL) {
		DataValue datavalue = DataValue(textval);
		ctx.symbols[0]->put(name, datavalue)->m_value.flags |= DataValue::FLAGS_NOWRITE;
	}
	else {
		DataValue datavalue = DataValue(numval);
		ctx.symbols[0]->put(name, datavalue)->m_value.flags |= DataValue::FLAGS_NOWRITE;
	}
	return 0;
}

int staticUnsetConstant(const char *name, InterpreterContext &ctx)
{
	ctx.symbols[0]->remove(name);
	return 0;
}

InterpreterContext::InterpreterContext()
{
	loopDeep = 0;
	switchDeep = 0;
	funcDeep = 1; // 1 = Global variables, 0 = Constants, system variables
	flag = 0;
	currThread = NULL;
	fAbort = 0;
	programMode = 0;
  securityMode = 0;
	exitCode = 0;
	line = 0;
	userData = NULL;
	fShowWarnings = true;
	fAutoNL = true;
	fOutputNoScript = true;
	fOnlySyntaxCheck = false;
	fHeadersSent = false;
	symbolTblCnt = 100;
	symbols = (SymbolTable **)malloc(symbolTblCnt * sizeof(SymbolTable *));
	// memset (symbols, 0, symbolTblCnt * sizeof (SymbolTable*));
	for (unsigned int i = 0; i < symbolTblCnt; i++)
		symbols[i] = new SymbolTable();
	WSystem::initMutex(mutex);
	_warnInterprete = staticWarnInterprete;
	_abortInterprete = staticAbortInterprete;
	_callUserFunc = NULL; // Initialized with dl()
	_setConstant = staticSetConstant;
	_unsetConstant = staticUnsetConstant;
	_binaryPrintOut = staticBinaryPrintOut;
}

InterpreterContext::~InterpreterContext()
{
	cleanupAll();
	for (unsigned int i = 0; i < symbolTblCnt; i++) {
		if (symbols[i])
			delete symbols[i];
	}
	free(symbols);
	symbols = NULL;
	WSystem::deleteMutex(mutex);
}

void InterpreterContext::cleanupAll()
{

	// Ping all blocking server sockets so that threads will finish
	handleHT.setLast();
	while (handleHT.hasPrevElements()) {
		Handle *h = handleHT.prevValue();
		if (h->handletype == Handle::HANDLETYPE_SERVERSOCKET) {
			try {
				WSocketUtils::ping(*((WSocket *)h->handle));
			}
			catch (...) {
			}
		}
	}
	// Delete all handles in back order
	handleHT.setLast();
	while (handleHT.hasPrevElements()) {
		Handle *h = handleHT.prevValue();
		// Join threads
		if (h->handletype == Handle::HANDLETYPE_THREAD) {
			((WScriptThread *)h->handle)->join();
		}
		delete h;
	}
	handleHT.removeAll();

	// Delete all temporary files
	WString tmpDir;
	WSystem::getTempDirectory(tmpDir);
	tmpFileList.reset();
	while (tmpFileList.hasMoreElements()) {
		WSystem::removeFile(WFormattedString("%s%s", (WCSTR)tmpDir, (WCSTR)tmpFileList.nextValue()));
	}
	tmpFileList.removeAll();

	// Free all libraries in back order
	dllModuleHT.setLast();
	while (dllModuleHT.hasPrevElements()) {
		void *hDLL = dllModuleHT.prevValue();

		WSCRIPT_DLL_CLEANUPALL wScriptDLLCleanupAll = (WSCRIPT_DLL_CLEANUPALL)dlsym(hDLL, "wScriptDLLCleanupAll");
		if (wScriptDLLCleanupAll)
			wScriptDLLCleanupAll(*this);

		WSCRIPT_DLL_CLEANUPALL wScriptDLLShutdown = (WSCRIPT_DLL_SHUTDOWN)dlsym(hDLL, "wScriptDLLShutdown");
		if (wScriptDLLShutdown)
			wScriptDLLShutdown(*this);

		dlclose(hDLL); // Let the main program free libraries
	}

	dllModuleHT.removeAll();

	// Free expressions of functions
	while (functions.hasMoreElements()) {
		functions.nextValuePnt()->freeExpressions();
	}
	functions.removeAll();
}

// Interpreter error
extern WSCRIPT_ERROR_FUNCTION wScriptErrorFunction;
void InterpreterContext::abortInterprete(WCSTR str, bool fPreInterpretate)
{
	this->fAbort = 1;
	this->exitCode = -2;
	WCSTR filename = this->getFilenameByIdx(WSCRIPT_FILE(this->line));
	unsigned int line = WSCRIPT_LINE(this->line);
	this->errText = WFormattedString("Error in %s (%u): %s", filename, line, (WCSTR)str);

	if (fPreInterpretate) {
		this->errText = str;
		if (!wScriptErrorFunction) {
			fprintf(stdout, "Fatal error: %s\r\n", (WCSTR)str);
		}
		else {
			wScriptErrorFunction("", 0, this->errText);
		}
	}
	else {
		if (this->programMode == InterpreterContext::PROGRAM_MODE_WEB) {
			printHeaders();
			fprintf(stdout, "<br>");
			fprintf(stdout, "%s\r\n", (WCSTR)this->errText);
			fprintf(stdout, "<br>");
		}
		else if (!wScriptErrorFunction)
			fprintf(stdout, "%s\r\n", (WCSTR)this->errText);
		else
			wScriptErrorFunction(filename, line, this->errText);
	}
	throw AbortException("Aborted", -2);
}

// Interpreter warning
void InterpreterContext::warnInterprete(WCSTR str)
{
	if (this->fShowWarnings) {
		if (this->programMode == InterpreterContext::PROGRAM_MODE_WEB) {
			printHeaders();
			fprintf(stdout, "<br>");
		}
		fprintf(stdout, "Warning in %s (%u): %s\r\n", this->getFilenameByIdx(WSCRIPT_FILE(this->line)), WSCRIPT_LINE(this->line), (WCSTR)str);
		if (this->programMode == InterpreterContext::PROGRAM_MODE_WEB)
			fprintf(stdout, "<br>");
	}
}

void InterpreterContext::warnInterprete(const WException &e)
{
	if (this->fShowWarnings) {
		if (this->programMode == InterpreterContext::PROGRAM_MODE_WEB) {
			printHeaders();
			fprintf(stdout, "<br>");
		}
		unsigned int sysErr = e.getSysErrCode();
		fprintf(stdout, "Warning in %s (%u): %s", this->getFilenameByIdx(WSCRIPT_FILE(this->line)), WSCRIPT_LINE(this->line), (WCSTR)e.getMessage());
		if (sysErr != 0)
			fprintf(stdout, " (ErrorId: %u)", sysErr);
		if (this->programMode == InterpreterContext::PROGRAM_MODE_WEB)
			fprintf(stdout, "<br>");
		else
			fprintf(stdout, "\r\n");
	}
}

void InterpreterContext::printHeaders()
{
	if (this->programMode == InterpreterContext::PROGRAM_MODE_WEB && !g_fHeadersSent) {
		this->headerHT.reset();
		while (this->headerHT.hasMoreElements()) {
			printf("%s\n", (WCSTR)this->headerHT.nextValue());
		}
		printf("\n");
		g_fHeadersSent = this->fHeadersSent = true;
	}
}

WCSTR InterpreterContext::getFilenameByIdx(unsigned int idx) const
{
	WString *pnt = this->idx2FileHT.getPnt(idx);
	if (pnt)
		return *pnt;
	return "";
}

unsigned int InterpreterContext::getIdxByFilename(WCSTR filename) const
{
	unsigned int *pnt = this->file2IdxHT.getPnt(filename);
	if (pnt)
		return *pnt;
	return 0;
}

unsigned int InterpreterContext::registerFilename(WCSTR filename)
{
	if (this->file2IdxHT.isKey(filename)) {
		return this->file2IdxHT.get(filename);
	}
	unsigned int idx = this->file2IdxHT.size() + 1;
	this->file2IdxHT.put(filename, idx);
	this->idx2FileHT.put(idx, filename);
	return idx;
}

/*
WCSTR Handle::describeHandleType (int ownType) const  {
    switch (ownType!=-1 ? ownType : handletype) {
        case HANDLETYPE_FILE:
            return "file";
            break;
        case HANDLETYPE_SOCKET:
            return "clientsocket";
            break;
        case HANDLETYPE_SERVERSOCKET:
            return "serversocket";
            break;
        case HANDLETYPE_DIRECTORY:
            return "directory";
            break;
        case HANDLETYPE_THREAD:
            return "thread";
            break;
        case HANDLETYPE_NONE:
            return "none";
            break;
    }
    return "handle";
}
*/

void ExpressionList::_freeExpressions(ExpressionList &list, InterpreterContext *ctx)
{

	for (unsigned int i = 0; i < list.size(); i++) {
		if (ctx && (list.m_list[i]->opCode & 0xFF) == OPCODE_FUNCDEF) {
			// if (list.m_list[i]->arguments.size ()>0 && ctx->functions.isKey (list.m_list[i]->arguments[0].operation.c_str ())) {
			if (list.m_list[i]->datatype == 1) {
				// function was registered
				delete list.m_list[i];
				continue;
			}
		}
		_freeExpressions(list.m_list[i]->arguments, ctx);
		delete list.m_list[i];
	}
	if (list.m_list) {
		free(list.m_list);
		list.m_list = NULL;
		list.m_size = 0;
	}
}
