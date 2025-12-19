/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: V1 Script
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "ast.h"
#include "utils/CGIUtils.h"
#include "utils/WStringTokenizer.h"

#include <string.h>

// Status vars of scanner and parser
typedef struct YYLTYPE {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} YYLTYPE;

WSCRIPT_THREAD_SAFE int yydebug = 0;
WSCRIPT_THREAD_SAFE char *yyInputBuf = NULL;
WSCRIPT_THREAD_SAFE const char *yyFilename = "";
WSCRIPT_THREAD_SAFE unsigned int yyReadMax = 0;
WSCRIPT_THREAD_SAFE unsigned int yyReadCurr = 0;
WSCRIPT_THREAD_SAFE unsigned int yyLine = 1;
WSCRIPT_THREAD_SAFE unsigned int yyFileIdx = 0;
WSCRIPT_ERROR_FUNCTION wScriptErrorFunction = NULL;

// Scanner
typedef void *yyscan_t;
extern int yylex_init(yyscan_t *scanner);
extern int yylex_destroy(yyscan_t scanner);

extern bool g_fHeadersSent;
extern int g_programMode;

extern "C"
{

	// Parser
	extern int yyparse(yyscan_t scanner, ExpressionList **pexplist);

	// Parser error
	WSCRIPT_THREAD_SAFE WString *lastParserErrText = NULL;
	void yyerror(YYLTYPE *location, yyscan_t scanner, ExpressionList **pexplist, const char *str)
	{
		if (!lastParserErrText)
			lastParserErrText = new WString();
		if (location)
			*lastParserErrText = WFormattedString("%s (%u:%u-%u): %s", yyFilename, location->last_line, location->first_column, location->last_column, str);
		else
			*lastParserErrText = WFormattedString("%s", str);

		if (g_programMode == InterpreterContext::PROGRAM_MODE_WEB && !g_fHeadersSent) {
			fprintf(g_programMode == InterpreterContext::PROGRAM_MODE_WEB ? stdout : stderr, "\n");
		}
		if (!wScriptErrorFunction)
			fprintf(g_programMode == InterpreterContext::PROGRAM_MODE_WEB ? stdout : stderr, "%s\n", (WCSTR)*lastParserErrText);
		else
			wScriptErrorFunction(yyFilename, yyLine, *lastParserErrText);
		g_fHeadersSent = true;
		// fflush (stdout);
	}
};

// Reader function
int yyScriptReader(char *buffer, size_t *numBytesRead, int maxBytesToRead)
{
	int numBytesToRead = maxBytesToRead;
	int bytesRemaining = yyReadMax - yyReadCurr;

	if (numBytesToRead > bytesRemaining) {
		numBytesToRead = bytesRemaining;
	}
	for (int i = 0; i < numBytesToRead; i++) {
		buffer[i] = yyInputBuf[yyReadCurr + i];
	}
	*numBytesRead = numBytesToRead;
	yyReadCurr += numBytesToRead;
	return 0;
}

// Helper function
bool yyStrnCaseCmp(const char *s1, const char *s2, int len)
{
	int oldLen = len;
	while (len > 0) {
		if (toupper(*s1) == toupper(*s2))
			oldLen--;
		len--;
		s1++;
		s2++;
	}
	return oldLen == len;
}

/* Higher level functions */

extern void wScriptInitFunctions(InterpreterContext &ctx);
extern void wScriptCleanupFunctions(InterpreterContext &ctx);

void wScriptStartup(InterpreterContext &ctx)
{
	WSystem::startup();
	WSocketUtils::startup();
	wScriptInitFunctions(ctx);
}

void wScriptShutdown(InterpreterContext &ctx)
{
	ctx.cleanupAll();
	wScriptCleanupFunctions(ctx);
	WSocketUtils::shutdown();
	WSystem::shutdown();
}

void setCGIParam2Array(ArrayHT &arrayList, WCSTR name, WCSTR value, WCSTR fileParam = NULL)
{
	ArrayHT *arrayListPnt = &arrayList;
	// Calc indexes
	const char *c = name, *c0 = name;
	WString baseName, key;
	WString indexList[10];
	int indexCnt = 0;
	while (*c != 0 && indexCnt < 9) {
		if (*c == '[') {
			c0 = c + 1;
			if (indexCnt == 0)
				baseName.assign((W8 *)name, c - name);
		}
		else if (*c == ']') {
			if (indexCnt == 0 && fileParam != NULL)
				indexList[indexCnt++] = fileParam; // Second index is file parameter
			indexList[indexCnt].assign((W8 *)c0, c - c0);
			indexCnt++;
		}
		c++;
	}
	if (fileParam != NULL && indexCnt == 0) {
		// Second index is file parameter
		baseName = name;
		indexList[indexCnt++] = fileParam;
	}

	if (indexCnt == 0) {
		// No indexes
		DataValue dataValue = DataValue(value);
		arrayListPnt->put(name, dataValue);
	}
	else {
		DataValue arrayValue;
		arrayValue.datatype = DataValue::DATATYPE_ARRAY;

		for (int i = -1; i < indexCnt; i++) {
			if (i == -1)
				key = baseName;
			else if (indexList[i].length() > 0)
				key = indexList[i];
			else
				key = WFormattedString("%u", arrayListPnt->size());

			if ((i + 1) < indexCnt) {
				// Sub array
				DataValue *existingArray = arrayListPnt->getPnt(key);
				if (existingArray)
					arrayListPnt = &existingArray->arrayList;
				else
					arrayListPnt = &arrayListPnt->put(key, arrayValue)->m_value.arrayList;
			}
			else {
				// The value
				DataValue datavalue = DataValue(value);
				arrayListPnt->put(key, datavalue)->m_value.flags |= DataValue::FLAGS_NOWRITE;
			}
		}
	}
}

void wScriptInitCGIVariables(InterpreterContext &ctx)
{

	try {

		char buf[4096 + 1];
		buf[0] = 0;

		DataValue datavalue;
		datavalue.datatype = DataValue::DATATYPE_ARRAY;
		datavalue.flags |= DataValue::FLAGS_NOWRITE;
		ctx.symbols[0]->put("_GET", datavalue);
		ctx.symbols[0]->put("_POST", datavalue);
		ctx.symbols[0]->put("_REQUEST", datavalue);
		ctx.symbols[0]->put("_FILES", datavalue);

		CGIUtils::CGIParamList paramList;
		WCSTR queryStr = getenv("QUERY_STRING");
		if (queryStr)
			CGIUtils::parseURIParams(queryStr, paramList, true);

		while (paramList.hasMoreElements()) {
			const CGIUtils::CGIParam *param = &paramList.nextValue();
			setCGIParam2Array(ctx.symbols[0]->get("_GET").arrayList, param->key, param->value);
			setCGIParam2Array(ctx.symbols[0]->get("_REQUEST").arrayList, param->key, param->value);
		}

		// Read post data
		/*
		GetEnvironmentVariable ("CONTENT_LENGTH", buf, sizeof (buf)-1);
		W32 contentLength = atoi (buf);
		*/

		int postFormat = 0;
		WCSTR envVar = getenv("CONTENT_TYPE");
		if (envVar != 0)
			strncpy(buf, envVar, sizeof(buf) - 1);

		char *boundary;
		WString boundaryStart, boundaryEnd, dispType, dispName, dispFilename, dispContentType, tmp;
		if ((boundary = strstr(buf, ";")) != NULL) {
			*boundary++ = 0;
			if ((boundary = strstr(boundary, "boundary=")) != NULL) {
				boundary += 9;
				boundaryStart = WFormattedString("--%s", (WCSTR)boundary);
				boundaryEnd = WFormattedString("--%s--", (WCSTR)boundary);
			}
		}
		if (strcasecmp(buf, "application/x-www-form-urlencoded") == 0) {
			postFormat = 1;
		}
		else if (strcasecmp(buf, "multipart/form-data") == 0) {
			postFormat = 2;
			if (boundaryStart.length() == 0)
				postFormat = 0; // No boundary found
		}
		bool fBoundaryFound = false;

		WFile file;
		file.open(stdin, false, false);

		WString line, contentType;
		while (file.readln(line)) {

			if (postFormat == 1) {
				CGIUtils::parseURIParams(line, paramList, true, true);
				while (paramList.hasMoreElements()) {
					const CGIUtils::CGIParam *param = &paramList.nextValue();
					setCGIParam2Array(ctx.symbols[0]->get("_POST").arrayList, param->key, param->value);
					setCGIParam2Array(ctx.symbols[0]->get("_REQUEST").arrayList, param->key, param->value);
				}
			}
			else if (postFormat == 2) {
checkboundary:
				if (line.startsWith(boundaryEnd)) {
					break;
				}
				else if (line.startsWith(boundaryStart)) {
					dispFilename.truncate(0);
					dispName.truncate(0);
					contentType.truncate(0);

					fBoundaryFound = true;
					while (file.readln(line)) {
						// file2.writeln (line);
						if (line.length() == 0)
							break;
						// Parse HTTP-Header
						WString key, value;
						if (line.split(":", key, value, true)) {
							key.toUpperCase();
							if (key.equals("CONTENT-TYPE")) {
								contentType = value;
							}
							else if (key.equals("CONTENT-DISPOSITION")) {
								WStringTokenizer tok(value, ";", true);
								while (tok.hasMoreElements()) {
									value = tok.nextValue();
									if (value.indexOf("=") == -1) {
										dispType = value;
									}
									else {
										value.split("=", key, tmp, true);
										tmp.trim('"');
										if (key.equals("filename")) {
											dispFilename = tmp;
										}
										else if (key.equals("name")) {
											dispName = tmp;
										}
									}
								}
							}
						}
					}
				}

				if (fBoundaryFound && dispName.length() > 0) {

					if (dispFilename.length() > 0) {
						// Remove path from filename
						WString basename;
						WSystem::extractPath(dispFilename, basename, true);

						// Create temporary file
						WFile file3;
						WDate date;
						WString tmpDir;
						WSystem::getTempDirectory(tmpDir);
						unsigned int r = (date.getTime() % 1000) * rand();
						WString tmpFilename = WFormattedString("v1_%u_%s", (W32)r, (WCSTR)basename);
						file3.create(WFormattedString("%s%s", (WCSTR)tmpDir, (WCSTR)tmpFilename));

						int lineCnt = 0;
						W32 contentSize = 0;
						bool fRead = false;
						while (fRead = file.readln(line, false)) {

							if (line.startsWith(boundaryStart) || line.startsWith(boundaryEnd)) {

								// Remove last \r
								file3.setPosMode(WFile::POS_MODE_CURRENT);
								file3.setPos(-1);

								file3.setPosMode(WFile::POS_MODE_BEGIN);
								ftruncate(fileno(file), file.getPos()); // Truncate

								contentSize = file3.getSize();

								break;
							}
							if (lineCnt > 0) {
								file3.write("\n");
							}
							file3.write(line.getBuf(), line.length());
							lineCnt++;
						}
						file3.write((WCSTR)WFormattedString("END\r\n"));

						setCGIParam2Array(ctx.symbols[0]->get("_FILES").arrayList, dispName, basename, "name");
						setCGIParam2Array(ctx.symbols[0]->get("_FILES").arrayList, dispName, tmpFilename, "tmp_name");
						setCGIParam2Array(ctx.symbols[0]->get("_FILES").arrayList, dispName, contentType, "type");
						setCGIParam2Array(ctx.symbols[0]->get("_FILES").arrayList, dispName, WFormattedString("%u", contentSize), "size");
						ctx.tmpFileList.add(tmpFilename);
						file3.close();
						if (fRead)
							goto checkboundary;
					}
					else {

						WString value;
						bool fRead = false;
						while (fRead = file.readln(line)) {
							if (line.startsWith(boundaryStart) || line.startsWith(boundaryEnd)) {
								if (value.length() > 0)
									value.truncate(value.length() - 1);
								break;
							}
							value += line;
							value += "\n";
						}
						// Content
						setCGIParam2Array(ctx.symbols[0]->get("_POST").arrayList, dispName, value);
						setCGIParam2Array(ctx.symbols[0]->get("_REQUEST").arrayList, dispName, value);

						if (fRead)
							goto checkboundary;
					}
				}
			}
		}

		// Default HTTP headers
		ctx.headerHT.put("CONTENT-TYPE", "Content-Type: text/html; charset=utf-8");
		//  ctx.headerHT.put ("X-POWERED-BY", "X-Powered-By: V1 Script");
	}
	catch (WException &e) {
		ctx.abortInterprete(e.getMessage(), true);
	}

	// Read the whole input empty, so that the server process finish writing!

	W32 emptyBuf[256];
	while (read(fileno(stdin), emptyBuf, sizeof(emptyBuf)) > 0) {
	}
}

int InterpreterContext::eval(WCSTR code, unsigned int size, DataValue &retvalue)
{
	int exitCode = 0;
	ExpressionList *ast = NULL;
	vector<Expression>::iterator i; // iterator to "walk" along vector
	yydebug = 0;                    // 1 - debug output, 0 - no debug output (in wscript.tab.cpp)

	const char *oldFilename = yyFilename;
	unsigned int oldLine = yyLine;
	char *oldInputBuf = yyInputBuf;

	try {
		// Parse
		unsigned int line = WSCRIPT_FILE(this->line);
		WString filename;
		if (line)
			filename = WFormattedString("%s (%u): eval()", getFilenameByIdx(WSCRIPT_FILE(this->line)), WSCRIPT_LINE(this->line));
		else
			filename = "eval()";

		yyFilename = filename;
		yyFileIdx = registerFilename(yyFilename);
		yyLine = 1;
		yyReadCurr = 0;
		yyReadMax = size + 8;
		yyInputBuf = (char *)malloc(yyReadMax);
		memcpy(yyInputBuf, "<?v1 ", 5);
		memcpy(&yyInputBuf[5], code, size);
		memcpy(&yyInputBuf[5 + size], " ?>", 3);

		// yyInputBuf = (char*) code;

		yyscan_t scanner;
		yylex_init(&scanner);
		exitCode = yyparse(scanner, &ast);
		yylex_destroy(scanner);

		if (!exitCode) {
			// Now interprete
			try {
				for (unsigned int i = 0; i < ast->size(); i++) {
					if (this->flag == InterpreterContext::FLAG_RETURN)
						break;
					(*ast)[i].calc(*this, retvalue);
				}
			}
			catch (AbortException &e) {
				exitCode = e.getId();
			}
		}
		else {
			// Parser error
			if (lastParserErrText)
				this->errText = *lastParserErrText;
			this->fAbort = 1;
		}
	}
	catch (WException &e) {
		this->errText = e.getMessage();
		this->fAbort = 1;
		exitCode = -1;
		yyerror(NULL, NULL, NULL, (WCSTR)e.getMessage());
	}

	if (yyInputBuf) {
		free(yyInputBuf);
		yyInputBuf = NULL;
	}

	yyFilename = oldFilename;
	yyLine = oldLine;
	yyInputBuf = oldInputBuf;
	if (ast) {
		// Free all expressions except user defined functions
		ast->freeExpressions(this);
		delete ast;
		ast = NULL;
	}
	return exitCode;
}

int InterpreterContext::execute(WCSTR filename, int fRequireOnce)
{

	int ret = 0;
	ExpressionList *ast = NULL;
	vector<Expression>::iterator i; // iterator to "walk" along vector
	yydebug = 0;                    // 1 - debug output, 0 - no debug output (in wscript.tab.cpp)

	const char *oldFilename = yyFilename;
	unsigned int oldLine = yyLine;
	bool fInterpreted = false;
	try {
		WFile file;
		file.open(filename);

		WString realFilename;
		file.getRealFilename(realFilename);
		if (fRequireOnce == 1 && getIdxByFilename(realFilename) > 0)
			return 0;

		yyFilename = (WCSTR)realFilename;
		yyFileIdx = registerFilename(realFilename);
		yyLine = 1;
		yyReadCurr = 0;

		// Parse
		yyReadMax = file.getSize();
		yyInputBuf = (char *) WMemory::allocMemory (yyReadMax);
		file.read(yyInputBuf, yyReadMax);
		file.close();

		yyscan_t scanner;
		yylex_init(&scanner);
		ret = yyparse(scanner, &ast);
		yylex_destroy(scanner);
    /*
		memset (yyInputBuf, 0, yyReadMax);
		WMemory::freeMemory (yyInputBuf);
		yyInputBuf=NULL;
    */
		if (!ret && !fOnlySyntaxCheck) {

			if (programMode & PROGRAM_MODE_WEB && fRequireOnce==2) {
				// Set working directory of main file in web mode
				WString path;
				WSystem::extractPath (realFilename, path, false);
				WSystem::setCurrentDirectory (path);
			}
			
			// Now interprete
			DataValue retvalue;
			try {
				for (unsigned int i = 0; i < ast->size(); i++) {
					(*ast)[i].calc(*this, retvalue);
				}
			}
			catch (AbortException &e) {
				ret = e.getId();
			}
		}
		else if (ret != 0) {
			// Parser error
			if (lastParserErrText)
				this->errText = *lastParserErrText;
			this->fAbort = 1;
		}
	}
	catch (WException &e) {
		this->errText = e.getMessage();
		ret = -1;
		this->fAbort = 1;
		yyerror(NULL, NULL, NULL, (WCSTR)e.getMessage());
	}

	yyFilename = oldFilename;
	yyLine = oldLine;

	if (yyInputBuf) {
		memset (yyInputBuf, 0, yyReadMax);
		WMemory::freeMemory (yyInputBuf);
		yyInputBuf=NULL;
	}
	if (ast) {
		// Free all expressions except user defined functions
		ast->freeExpressions(this);
		delete ast;
		ast = NULL;
	}
	return ret;
}
