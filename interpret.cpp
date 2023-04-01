/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: V1 Script
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "ast.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include <libgen.h>
#include <dlfcn.h>
#include <utime.h>
#include <sys/utsname.h>

#ifdef _HAVE_LIBC_VERSION_H
#include <gnu/libc-version.h>
#endif

#include "WSocket.h"
#include "utils/WStringTokenizer.h"
#include "utils/CGIUtils.h"
#include "utils/Process.h"
#include "hash/base64.h"

using namespace std;

#define _V1_THREADSAFE

#ifdef _LOG_PERFORMANCE
extern W64 g_timerFreq, g_timerCounter, g_timerCounterStart;
extern WFile g_performanceLogFile;
#endif

int freeHandle(void *handle, int handletype)
{
	if (handletype == Handle::HANDLETYPE_FILE) {
		delete (WFile *)handle;
	}
	else if (handletype == Handle::HANDLETYPE_DIRECTORY) {
		delete (WDirectoryEntries *)handle;
	}
	else if (handletype == Handle::HANDLETYPE_SOCKET) {
		delete (WSocket *)handle;
	}
	else if (handletype == Handle::HANDLETYPE_SERVERSOCKET) {
		delete (WServerSocket *)handle;
	}
	else if (handletype == Handle::HANDLETYPE_THREAD) {
		delete (WScriptThread *)handle;
	}
	return 0;
}

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#ifdef ENVIRONMENT64
#define SYS_UINT uint64_t
#define SYS_INT int64_t
#else
#define SYS_UINT uint32_t
#define SYS_INT int32_t
#endif

int callbackA1(void *arg1)
{
	DataValue ret;
	vector<DataValue *> argvalues;
	DataValue param1 = (double)*(SYS_UINT *)&arg1;
	argvalues.push_back(&param1);
	InterpreterContext::ctx->_callUserFunc("callbackA1", argvalues, ret, *InterpreterContext::ctx, 0);
	return (int)ret;
}

int callbackA2(void *arg1, void *arg2)
{
	DataValue ret;
	vector<DataValue *> argvalues;
	DataValue param1 = (double)*(SYS_UINT *)&arg1;
	argvalues.push_back(&param1);
	DataValue param2 = (double)*(SYS_UINT *)&arg2;
	argvalues.push_back(&param2);
	InterpreterContext::ctx->_callUserFunc("callbackA2", argvalues, ret, *InterpreterContext::ctx, 0);
	return (int)ret;
}

int callbackA3(void *arg1, void *arg2, void *arg3)
{
	DataValue ret;
	vector<DataValue *> argvalues;
	DataValue param1 = (double)*(SYS_UINT *)&arg1;
	argvalues.push_back(&param1);
	DataValue param2 = (double)*(SYS_UINT *)&arg2;
	argvalues.push_back(&param2);
	DataValue param3 = (double)*(SYS_UINT *)&arg3;
	argvalues.push_back(&param3);
	InterpreterContext::ctx->_callUserFunc("callbackA3", argvalues, ret, *InterpreterContext::ctx, 0);
	return (int)ret;
}

int callbackA4(void *arg1, void *arg2, void *arg3, void *arg4)
{
	DataValue ret;
	vector<DataValue *> argvalues;
	DataValue param1 = (double)*(SYS_UINT *)&arg1;
	argvalues.push_back(&param1);
	DataValue param2 = (double)*(SYS_UINT *)&arg2;
	argvalues.push_back(&param2);
	DataValue param3 = (double)*(SYS_UINT *)&arg3;
	argvalues.push_back(&param3);
	DataValue param4 = (double)*(SYS_UINT *)&arg4;
	argvalues.push_back(&param4);
	InterpreterContext::ctx->_callUserFunc("callbackA4", argvalues, ret, *InterpreterContext::ctx, 0);
	return (int)ret;
}

int callbackB1(void *arg1)
{
	DataValue ret;
	vector<DataValue *> argvalues;
	DataValue param1 = (double)*(SYS_UINT *)&arg1;
	argvalues.push_back(&param1);
	InterpreterContext::ctx->_callUserFunc("callbackB1", argvalues, ret, *InterpreterContext::ctx, 0);
	return (int)ret;
}

int callbackB2(void *arg1, void *arg2)
{
	DataValue ret;
	vector<DataValue *> argvalues;
	DataValue param1 = (double)*(SYS_UINT *)&arg1;
	argvalues.push_back(&param1);
	DataValue param2 = (double)*(SYS_UINT *)&arg2;
	argvalues.push_back(&param2);
	InterpreterContext::ctx->_callUserFunc("callbackB2", argvalues, ret, *InterpreterContext::ctx, 0);
	return (int)ret;
}

int callbackB3(void *arg1, void *arg2, void *arg3)
{
	DataValue ret;
	vector<DataValue *> argvalues;
	DataValue param1 = (double)*(SYS_UINT *)&arg1;
	argvalues.push_back(&param1);
	DataValue param2 = (double)*(SYS_UINT *)&arg2;
	argvalues.push_back(&param2);
	DataValue param3 = (double)*(SYS_UINT *)&arg3;
	argvalues.push_back(&param3);
	InterpreterContext::ctx->_callUserFunc("callbackB3", argvalues, ret, *InterpreterContext::ctx, 0);
	return (int)ret;
}

int callbackB4(void *arg1, void *arg2, void *arg3, void *arg4)
{
	DataValue ret;
	vector<DataValue *> argvalues;
	DataValue param1 = (double)*(SYS_UINT *)&arg1;
	argvalues.push_back(&param1);
	DataValue param2 = (double)*(SYS_UINT *)&arg2;
	argvalues.push_back(&param2);
	DataValue param3 = (double)*(SYS_UINT *)&arg3;
	argvalues.push_back(&param3);
	DataValue param4 = (double)*(SYS_UINT *)&arg4;
	argvalues.push_back(&param4);
	InterpreterContext::ctx->_callUserFunc("callbackB4", argvalues, ret, *InterpreterContext::ctx, 0);
	return (int)ret;
}

typedef struct {
	WCSTR name;
	SYS_UINT procPtr;
} SCallbackProc;

SCallbackProc g_callbackList[] = {
	{"callbackA1", (SYS_UINT)callbackA1},
	{"callbackA2", (SYS_UINT)callbackA2},
	{"callbackA3", (SYS_UINT)callbackA3},
	{"callbackA4", (SYS_UINT)callbackA4},
	{"callbackB1", (SYS_UINT)callbackB1},
	{"callbackB2", (SYS_UINT)callbackB2},
	{"callbackB3", (SYS_UINT)callbackB3},
	{"callbackB4", (SYS_UINT)callbackB4}
};

// ** START NATIVE AND NUMBER OPERATIONS ***

int func_sin(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = sin(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_asin(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = asin(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_cos(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = cos(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_acos(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = acos(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_tan(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = tan(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_atan(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = atan(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_atan2(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = atan2(argvalues[0]->numvalue, argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_ceil(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = ceil(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_floor(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = floor(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_sqrt(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = sqrt(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_log(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = log(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_log10(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = log10(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_exp(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = exp(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_fabs(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = fabs(argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_fmod(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = fmod(argvalues[0]->numvalue, argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_round(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	int precision = 0;

	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		precision = (int)*argvalues[1];
	}

	ret.numvalue = WSystem::round(argvalues[0]->numvalue, precision);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_rand(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	ret.numvalue = (double)rand();
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_srand(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	srand((int)argvalues[0]->numvalue);
	ret = true;
	return 0;
}

int func_plus(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	/*
	if (argvalues[0]->datatype!=argvalues[1]->datatype)
	{
	    ctx.warnInterprete (WFormattedString ("Types mismatch %s + %s ", (WCSTR) argvalues[0]->describeDataType(), (WCSTR) argvalues[1]->describeDataType()), ctx);
	}
	*/
	ret = *argvalues[0] + *argvalues[1];
	return 0;
}

int func_plus_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = ret + *argvalues[0];
	return 0;
}

int func_plusstr(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	argvalues[1]->toString();
	ret = *argvalues[0];
	ret.toString();
	ret.value.append(argvalues[1]->value.c_str(), argvalues[1]->value.size());
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_plusstr_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret.toString();
	argvalues[0]->toString();
	ret.value.append(argvalues[0]->value.c_str(), argvalues[0]->value.size());
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_minus(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = argvalues[0]->numvalue - argvalues[1]->numvalue;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_minus_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = ret.numvalue - argvalues[0]->numvalue;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_uminus(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = -argvalues[0]->numvalue;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_mul(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = argvalues[0]->numvalue * argvalues[1]->numvalue;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_mul_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = ret.numvalue * argvalues[0]->numvalue;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_pow(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = pow(argvalues[0]->numvalue, argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_pow_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = pow(ret.numvalue, argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_div(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues[1]->numvalue == 0) {
		ctx.abortInterprete("Division by zero");
		return WSCRIPT_RET_ABORT;
	}
	ret.numvalue = argvalues[0]->numvalue / argvalues[1]->numvalue;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_div_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues[0]->numvalue == 0) {
		ctx.abortInterprete("Division by zero");
		return WSCRIPT_RET_ABORT;
	}
	ret.numvalue = ret.numvalue / argvalues[0]->numvalue;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_mod(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((int)argvalues[0]->numvalue % (int)argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_mod_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((int)ret.numvalue % (int)argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_inc(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue++;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_dec(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue--;
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_not(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = !(bool)*argvalues[0];
	return 0;
}

int func_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (*argvalues[0] == *argvalues[1]);
	return 0;
}

int func_equal2(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (argvalues[0]->datatype != argvalues[1]->datatype) {
		ret = false;
		return 0;
	}

	ret = (*argvalues[0] == *argvalues[1]);
	return 0;
}

int func_greater(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = *argvalues[0] > *argvalues[1];
	return 0;
}

int func_less(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = *argvalues[0] < *argvalues[1];
	return 0;
}

int func_cmp_ge(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = *argvalues[0] >= *argvalues[1];
	return 0;
}

int func_cmp_le(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = *argvalues[0] <= *argvalues[1];
	return 0;
}

int func_cmp_ne(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = *argvalues[0] != *argvalues[1];
	return 0;
}

int func_cmp_ne2(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != argvalues[1]->datatype) {
		ret = true;
		return 0;
	}

	ret = *argvalues[0] != *argvalues[1];
	return 0;
}

int func_bool_or(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (bool)*argvalues[0] || (bool)*argvalues[1];
	return 0;
}

int func_bool_and(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (bool)*argvalues[0] && (bool)*argvalues[1];
	return 0;
}

int func_bin_and(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)argvalues[0]->numvalue & (unsigned int)argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_or(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)argvalues[0]->numvalue | (unsigned int)argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_xor(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)argvalues[0]->numvalue ^ (unsigned int)argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_shiftl(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)argvalues[0]->numvalue << (unsigned int)argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_shiftl_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)ret.numvalue << (unsigned int)argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_shiftr_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)ret.numvalue >> (unsigned int)argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_shiftr(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)argvalues[0]->numvalue >> (unsigned int)argvalues[1]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_and_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)ret.numvalue & (unsigned int)argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_or_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)ret.numvalue | (unsigned int)argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_xor_equal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ret.datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.numvalue = (double)((unsigned int)ret.numvalue ^ (unsigned int)argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_bin_neg(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	ret.numvalue = (double)(~(unsigned int)argvalues[0]->numvalue);
	ret.datatype = DataValue::DATATYPE_NUM;
	return 0;
}

int func_return(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ctx.funcDeep > 1) {
		ret = *argvalues[0];
		ctx.flag = InterpreterContext::FLAG_RETURN;
		return 0;
	}
	ctx.abortInterprete("Illegal return");
	return WSCRIPT_RET_ABORT;
}

int func_break(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ctx.loopDeep > 0 || ctx.switchDeep > 0) {
		ctx.flag = InterpreterContext::FLAG_BREAK;
		return 0;
	}
	ctx.abortInterprete("Illegal break");
	return WSCRIPT_RET_ABORT;
}

int func_continue(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (ctx.loopDeep > 0) {
		ctx.flag = InterpreterContext::FLAG_CONTINUE;
		return 0;
	}
	ctx.abortInterprete("Illegal continue");
	return WSCRIPT_RET_ABORT;
}

// *** END NATIVE OPERATIONS ***

// *** START HIGHER LEVEL FUNCTIONS ***

int func_time(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	WDate date;
	ret = (double)date.getTime();
	return 0;
}

int func_microtime(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	W64 counter = 0;
	bool fFloat = false;
	if (argvalues.size() > 0) {
		if (argvalues[0]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_BOOL_REQUIRED;
		fFloat = (bool)*argvalues[0];
	}
	WSystem::getPerformanceCounter(counter);
	if (fFloat) {
		W64 freq = 0;
		WSystem::getPerformanceFrequency(freq);
		ret = (double)counter / (double)freq;
	}
	else {
		ret = (double)counter;
	}
	return 0;
}

int func_sleep(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	WSystem::sleep((W32)argvalues[0]->numvalue);
	ret = true;
	return 0;
}

int func_date(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	WCSTR format = WLIB_DEFAULT_DATE_TIME_FORMAT;
	WDate date;
	if (argvalues.size() > 0) {
		if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
			return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
		}
		format = *argvalues[0];
	}
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		date.setTime((WTIME)argvalues[1]->numvalue);
	}
	ret = (WCSTR)date.format(format);
	ret.datatype = DataValue::DATATYPE_STR; // Interpretate as string
	return 0;
}

int func_easter_date(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret = (double)WDate::getEasterDate((W32)argvalues[0]->numvalue);
	return 0;
}

int func_gmdate(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	WCSTR format = WLIB_DEFAULT_DATE_TIME_FORMAT;
	WDate date(WTimeZone("GMT", 0));

	if (argvalues.size() > 0) {
		if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
			return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
		}
		format = *argvalues[0];
	}
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype != DataValue::DATATYPE_NUM)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		date.setTime((WTIME)argvalues[1]->numvalue);
	}
	ret = (WCSTR)date.format(format);
	ret.datatype = DataValue::DATATYPE_STR; // Interpretate as string
	return 0;
}

int func_mktime(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	WDate date;
	int parts[6];
	parts[0] = date.getHour();
	parts[1] = date.getMinute();
	parts[2] = date.getSecond();
	parts[3] = date.getMonth();
	parts[4] = date.getDay();
	parts[5] = date.getYear();

	for (unsigned int i = 0; i < 6; i++) {
		if (argvalues.size() > i) {
			if (argvalues[i]->datatype > DataValue::DATATYPE_STR) {
				return ((i + 1) << 4) | WSCRIPT_RET_NUM_REQUIRED;
			}
			parts[i] = (int)argvalues[i]->numvalue;
		}
	};

	try {
		date.parse(parts[5], parts[3], parts[4], parts[0], parts[1], parts[2]);
		ret = (double)date.getTime();
	}
	catch (WDateException &e) {
		ctx.warnInterprete(WFormattedString("%s %.2i-%.2i-%.2i %.2i:%.2i:%.2i", (WCSTR)e.getMessage(), parts[5], parts[3], parts[4], parts[0], parts[1], parts[2]));
		ret = (bool)false;
	}
	return 0;
}

int func_print(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
#ifdef _V1_THREADSAFE
	WMutexLocker locker(ctx.mutex);
#endif
	ctx.printHeaders();

	for (unsigned int j = 0; j < argvalues.size(); j++) {
		argvalues[j]->toString();
		// printf ("%s", argvalues[j]->value.c_str ());
		cout << (string)argvalues[j]->value;
	}
	if (ctx.fAutoNL)
		printf("\r\n");
	else if (ctx.programMode != InterpreterContext::PROGRAM_MODE_WEB)
		fflush(stdout);
	ret = 1;
	return 0;
}

int func_setlocale(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (argvalues[0]->datatype > DataValue::DATATYPE_STR)
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;

	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}

	argvalues[1]->toString();
	const char *r = setlocale((int)argvalues[0]->numvalue, argvalues[1]->value.c_str());
	if (r)
		ret = r;
	else
		ret = false;
	return 0;
}

int func_input_echo(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = true;
	WSystem::setConsoleEcho((bool)*argvalues[0]);
	return 0;
}

int func_input(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	char line[257];
	memset(line, 0, sizeof(line));
	if (argvalues[0]->datatype != 0) {
		argvalues[0]->toString();
		cout << (string)argvalues[0]->value;
		// cout << " ";
	}

	WCSTR defValue = NULL;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
		argvalues[1]->toString();
		defValue = argvalues[1]->value.c_str();
	}

	fflush(stdout);
	if (fgets(line, sizeof(line), stdin) != line) {
		// Ignore error
	}
	line[strlen(line) - 1] = '\0';
	ret.datatype = DataValue::DATATYPE_STR;
	if (line[0] == 0 && defValue)
		ret.value = defValue;
	else
		ret.value = line;
	return 0;
}

int func_getch(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if ((bool)*argvalues[0]) {
		// As Ascii Number
		ret = (int)WSystem::getch();
	}
	else {
		// As String
		char line[2];
		line[0] = (char)WSystem::getch();
		line[1] = '\0';
		ret = string(line);
	}
	return 0;
}

int func_sprintf(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.resize(0);

	const char *c = argvalues[0]->value.c_str();
	string format;
	char result[4096];
	unsigned int argIdx = 1;
	char stopChars[] = {'d', 'i', 'u', 'o', 'x', 'X', 'f', 'F', 'e', 'E', 'g', 'G', 'a', 'A', 'c', 's', '*'}; // n, p is ignored
	bool fFormat = false;
	while (*c != 0) {
		if (*c == '%') {
			fFormat = true;
			format = "";
			format.append(c, 1);
			c++;
			if (*c == 0)
				break;
			if (*c == '%') {
				fFormat = false;
			}
		}

		if (fFormat) {

			if (*c != '*')
				format.append(c, 1);

			for (unsigned int i = 0; i < sizeof(stopChars); i++) {
				if (*c == stopChars[i]) {
					fFormat = false;
					break;
				}
			}
			if (!fFormat) {

				int size = 0;
				if (argvalues.size() > argIdx) {
					switch (*c) {
						case '*': {
							format.append(WFormattedString("%u", (int)*argvalues[argIdx]));
							argIdx++;
							fFormat = true;
							c++;
							continue;
						}
						break;

						case 's': {
							argvalues[argIdx]->toString();
							size = snprintf(result, sizeof(result) - 1, format.c_str(), argvalues[argIdx]->value.c_str());
						}
						break;

						case 'i':
						case 'd': {
							int value = (int)(double)*argvalues[argIdx];
							size = snprintf(result, sizeof(result) - 1, format.c_str(), value);
						}
						break;

						case 'u':
						case 'o':
						case 'x':
						case 'X': {
							SYS_UINT value = (SYS_UINT)(double)*argvalues[argIdx];
							size = snprintf(result, sizeof(result) - 1, format.c_str(), value);
						}
						break;
						case 'c': {
							char value;
							argvalues[argIdx]->toString();
							if (argvalues[argIdx]->datatype == DataValue::DATATYPE_STR)
								value = (char)argvalues[argIdx]->value[0];
							else
								value = (char)(int)*argvalues[argIdx];

							size = snprintf(result, sizeof(result) - 1, format.c_str(), value);
						}
						break;
						default: {
							double value = (double)*argvalues[argIdx];
							size = snprintf(result, sizeof(result) - 1, format.c_str(), value);
						}
						break;
					}
				}
				if (size < 0)
					size = sizeof(result) - 1;
				result[size] = 0;
				ret.value.append(result, size);
				argIdx++;
			}
		}
		else {
			ret.value.append(c, 1);
		}
		c++;
	}
	return 0;
}

int func_sscanf(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}

	argvalues[0]->toString();
	argvalues[1]->toString();

	ret = 0;

	const char *c0 = argvalues[0]->value.c_str();
	const char *c = argvalues[1]->value.c_str();
	const char *cPrev = c;
	const char *cPrevPrev = cPrev;
	const char *cMax = c0 + argvalues[0]->value.length();
	string format;
	char result[4096];
	int matchCnt = 0;
	unsigned int argIdx = 2;
	char stopChars[] = {'d', 'i', 'u', 'o', 'x', 'X', 'f', 'F', 'e', 'E', 'g', 'G', 'a', 'A', 'c', 's'}; // n, p, * is ignored
	bool fFormat = false;
	DataValue tmp;

	while (*c != 0 && *c0 != 0) {
		if (*c == '%') {
			fFormat = true;
			format = "";
			format.append(c, 1);
			c++;
			if (*c == 0)
				break;
			if (*c == '%')
				fFormat = false;
		}

		if (fFormat) {
			format.append(c, 1);
			for (unsigned int i = 0; i < sizeof(stopChars); i++) {
				if (*c == stopChars[i]) {
					fFormat = false;
					break;
				}
			}
			if (!fFormat) {

				DataValue *arg = NULL;
				if (argvalues.size() > argIdx && argvalues[argIdx]->refPnt != NULL)
					arg = argvalues[argIdx]->refPnt;
				else
					arg = &tmp;

				// int size = 0;

				{
					switch (*c) {

						case 's':
						case 'c': {
							// Get length and make check
							const char *c3 = format.c_str() + 1;
							while (*c3 != 0 && (*c3 <= '0' || *c3 > '9'))
								c3++;
							unsigned int len = atoi(c3);
							if (len > sizeof(result) - 1) {
								ctx.warnInterprete(WFormattedString("Length of formatter %i is to long. Maximum %i bytes allowed.", argIdx - 1, sizeof(result) - 1));
								break;
							}
							if (*c != 'c') {
								// Overread spacers!
								while (*c0 == ' ' && *c0 != 0)
									c0++;
							}

							if (sscanf(c0, format.c_str(), result)) {
								arg->datatype = DataValue::DATATYPE_STR;

								if (*c == 'c') {
									if (len == 0)
										len = 1;
									if (c0 + len >= cMax)
										len = cMax - c0;
									arg->value.assign(result, len);
									c0 += len;
								}
								else {
									arg->value.assign(result);
									c0 += arg->value.length();
								}
								matchCnt++;
							}
						}
						break;

						case 'i':
						case 'd':
						case 'u':
						case 'o':
						case 'x':
						case 'X': {
							int value = 0;
							if (sscanf(c0, format.c_str(), &value)) {
								arg->datatype = DataValue::DATATYPE_NUM;
								if (*c == 'i' || *c == 'd') {
									// Signed
									if (*cPrev == 'L' || *cPrev == 'l')
										arg->numvalue = (double)value;
									else if (*cPrev == 'h' && *cPrevPrev != 'h')
										arg->numvalue = (double)(short)value;
									else if (*cPrev == 'h' && *cPrevPrev == 'h')
										arg->numvalue = (double)(char)value;
									else
										arg->numvalue = (double)(int)value;
								}
								else {
									// Unsigned
									if (*cPrev == 'h' && *cPrevPrev != 'h')
										arg->numvalue = (double)(unsigned short)value;
									else if (*cPrev == 'h' && *cPrevPrev == 'h')
										arg->numvalue = (double)(unsigned char)value;
									else
										arg->numvalue = (double)(unsigned int)value;
								}
								arg->value.clear();

								// Reverse check to get true length of string
								c0 += snprintf(result, sizeof(result) - 1, format.c_str(), value);
								// printf ("%c %s -> %s\r\n", *c, (WCSTR) format.c_str (), (WCSTR) result);
								matchCnt++;
							}
						}
						break;
						default: {
							long double value = 0;
							if (sscanf(c0, format.c_str(), &value)) {
								arg->datatype = DataValue::DATATYPE_NUM;

								arg->numvalue = (*cPrev == 'L' ? value : (*cPrev == 'l' ? *((double *)&value) : *((float *)&value)));
								arg->value.clear();

								// Reverse check to get true length of string
								char *endptr = NULL;
								strtod(c0, &endptr);
								if (endptr)
									c0 = (const char *)endptr;
								matchCnt++;
							}
						}
						break;
					}
				}
				if (arg != &tmp)
					arg->datatype = arg->refPnt->datatype;
				else {
					ret.datatype = DataValue::DATATYPE_ARRAY;
					ret.arrayList.push(*arg);
				}
				argIdx++;
			}
		}
		else {
			if (*c != *c0) {
				break;
			}
			c0++;
		}
		cPrevPrev = cPrev;
		cPrev = c;
		c++;
	}
	if (ret.datatype != DataValue::DATATYPE_ARRAY)
		ret = matchCnt;
	return 0;
}

int func_printf(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
#ifdef _V1_THREADSAFE
	WMutexLocker locker(ctx.mutex);
#endif

	int retCode = 0;
	if (!(retCode = func_sprintf(argvalues, ret, ctx))) {
		ctx.printHeaders();
		printf("%s", ret.value.c_str());
		if (ctx.fAutoNL)
			printf("\r\n");
		else if (ctx.programMode != InterpreterContext::PROGRAM_MODE_WEB)
			fflush(stdout);
	}
	ret = 1;
	return retCode;
}

int func_chr(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.datatype = DataValue::DATATYPE_STR;
	ret.value = "";
	ret.value += ((char)(int)argvalues[0]->numvalue);
	return 0;
}

int func_bin2hex(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.resize(0);
	W32 len = argvalues[0]->value.size();
	char *c = (char *)argvalues[0]->value.c_str();
	ret.value.reserve(len * 2);

	while (len > 0) {
		int v = (*c & 0xF0);
		v >>= 4;
		if (v >= 10)
			ret.value += (char)(87 + v);
		else
			ret.value += (char)(48 + v);
		v = (*c & 0x0F);
		if (v >= 10)
			ret.value += (char)(87 + v);
		else
			ret.value += (char)(48 + v);
		len--;
		c++;
	}
	return 0;
}

bool char2Hex(W8 &c)
{
	if ((c >= '0') && (c <= '9'))
		c -= '0';
	else if ((c >= 'A') && (c <= 'F')) {
		c -= 'A';
		c += 10;
	}
	else if ((c >= 'a') && (c <= 'f')) {
		c -= 'a';
		c += 10;
	}
	else
		return false; // bad format
	return true;
}

int func_hex2bin(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	int len = argvalues[0]->value.size();
	if ((len % 2) != 0) {
		ctx.warnInterprete("Hexadecimal input string must have an even length.");
		ret = false;
		return 0;
	}
	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.resize(0);
	ret.value.reserve(len / 2);
	char *c = (char *)argvalues[0]->value.c_str();
	W8 c2 = 0;
	while (*c != 0) {
		W8 v = 0;
		c2 = (W8)*c++;
		if (!char2Hex(c2)) {
			ret = false;
			return 0;
		}
		v |= (c2 << 4);

		c2 = (W8)*c++;
		if (!char2Hex(c2)) {
			ret = false;
			return 0;
		}
		v |= c2;
		ret.value += (char)v;
	}
	return 0;
}

int func_binparse(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	int type = 7;

	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		type = (int)argvalues[1]->numvalue;
	}

	unsigned int pos = 0;
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		pos = (unsigned int)argvalues[2]->numvalue;
	}
	bool fSigned = false;
	if (argvalues.size() > 3) {
		if (argvalues[3]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_BOOL_REQUIRED;
		fSigned = (bool)*argvalues[3];
	}
	unsigned int nbo = 0;
	if (argvalues.size() > 4) {
		if (argvalues[4]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM4 | WSCRIPT_RET_NUM_REQUIRED;
		nbo = (unsigned int)argvalues[4]->numvalue;
	}

	argvalues[0]->toString();

	int warnLen = 0;
	switch (type) {

		case 2:
			// 16 Bit short
			if (argvalues[0]->value.length() >= (2 + pos)) {
				uint16_t bin = *((uint16_t *)(argvalues[0]->value.c_str() + pos));
				if (nbo == 1) {
					bin = ntohs(bin);
				}
				else if (nbo == 2) {
					bin = htons(bin);
				}
				ret.datatype = DataValue::DATATYPE_NUM;
				ret.numvalue = (double)(!fSigned ? bin : (int16_t)bin);
			}
			else {
				warnLen = 2;
			}
			break;
		case 3:
			// 32 Bit int
			if (argvalues[0]->value.length() >= (4 + pos)) {
				uint32_t bin = *((uint32_t *)(argvalues[0]->value.c_str() + pos));
				if (nbo == 1) {
					bin = ntohl(bin);
				}
				else if (nbo == 2) {
					bin = htonl(bin);
				}
				ret.datatype = DataValue::DATATYPE_NUM;
				ret.numvalue = (double)(!fSigned ? bin : (int32_t)bin);
			}
			else {
				warnLen = 4;
			}
			break;
		case 4:
			// 64 Bit int
			if (argvalues[0]->value.length() >= (8 + pos)) {
				uint64_t bin = *((uint64_t *)(argvalues[0]->value.c_str() + pos));
				ret.datatype = DataValue::DATATYPE_NUM;
				ret.numvalue = (double)(!fSigned ? bin : (int64_t)bin);
			}
			else {
				warnLen = 8;
			}
			break;

		case 5:
			// 32 Bit float
			if (argvalues[0]->value.length() >= (4 + pos)) {
				float bin = *((float *)(argvalues[0]->value.c_str() + pos));
				ret.datatype = DataValue::DATATYPE_NUM;
				ret.numvalue = (double)bin;
			}
			else {
				warnLen = 4;
			}
			break;

		case 6:
			// 64 Bit double
			if (argvalues[0]->value.length() >= (8 + pos)) {
				ret.numvalue = *((double *)(argvalues[0]->value.c_str() + pos));
				ret.datatype = DataValue::DATATYPE_NUM;
			}
			else {
				warnLen = 8;
			}
			break;

		case 7:
			// Pointer / System default
			if (argvalues[0]->value.length() >= (sizeof(unsigned int) + pos)) {
				if (fSigned)
					ret.numvalue = (double)*((int *)(argvalues[0]->value.c_str() + pos));
				else
					ret.numvalue = (double)*((unsigned int *)(argvalues[0]->value.c_str() + pos));
				ret.datatype = DataValue::DATATYPE_NUM;
			}
			else {
				warnLen = sizeof(unsigned int);
			}
			break;

		default:
			// 8 Bit char
			if (argvalues[0]->value.length() >= (1 + pos)) {
				W8 bin = *((W8 *)(argvalues[0]->value.c_str() + pos));
				ret.datatype = DataValue::DATATYPE_NUM;
				ret.numvalue = (double)(!fSigned ? bin : (char)bin);
			}
			else {
				warnLen = 1;
			}
			break;
	}
	if (warnLen) {
		ret = false;
		ctx.warnInterprete(WFormattedString("Binary string must be %i bytes long", (int)warnLen));
	}
	return 0;
}

int func_binformat(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	int type = 7;

	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		type = (int)argvalues[1]->numvalue;
	}

	unsigned int nbo = 0;
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		nbo = (unsigned int)argvalues[2]->numvalue;
	}

	ret.datatype = DataValue::DATATYPE_STR;
	switch (type) {

		case 2: {
			// 16 Bit short
			uint16_t bin = (uint16_t)argvalues[0]->numvalue;
			if (nbo == 1) {
				bin = ntohs(bin);
			}
			else if (nbo == 2) {
				bin = htons(bin);
			}
			ret.value.assign((const char *)&bin, 2);
		}
		break;
		case 3: {
			// 32 Bit int
			uint32_t bin = (uint32_t)argvalues[0]->numvalue;
			if (nbo == 1) {
				bin = ntohl(bin);
			}
			else if (nbo == 2) {
				bin = htonl(bin);
			}
			ret.value.assign((const char *)&bin, 4);
		}
		break;
		case 4: {
			// 64 Bit int
			uint64_t bin = (uint64_t)argvalues[0]->numvalue;
			ret.value.assign((const char *)&bin, 8);
		}
		break;

		break;

		case 5: {
			// 32 Bit float
			float bin = (float)argvalues[0]->numvalue;
			ret.value.assign((const char *)&bin, 4);
		}
		break;

		case 6: {
			// 64 Bit double
			double bin = (float)argvalues[0]->numvalue;
			ret.value.assign((const char *)&bin, 8);
		}
		break;
		case 7: {
			// Pointer / System default
			unsigned int bin = (double)argvalues[0]->numvalue;
			ret.value.assign((const char *)&bin, sizeof(unsigned int));
		}
		break;

		default: {
			// 8 Bit char
			W8 bin = (W8)argvalues[0]->numvalue;
			ret.value.assign((const char *)&bin, 1);
		}
		break;
	}
	return 0;
}

int func_strlen(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR)
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	argvalues[0]->toString();
	ret = (int)argvalues[0]->value.length();
	return 0;
}

int func_strcmp(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	argvalues[1]->toString();

	ret = (int)strcmp(argvalues[0]->value.c_str(), argvalues[1]->value.c_str());
	return 0;
}

int func_substr(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	argvalues[0]->toString();

	int len = (int)argvalues[0]->value.size();
	int from = (int)*argvalues[1];
	int to = len;
	if (from < 0)
		from = len - (abs(from) % len);

	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
		int offset = (int)*argvalues[2];
		if (offset < 0)
			to = len - (abs(offset) % len);
		else
			to = from + offset;
	}

	if (to > len)
		to = len;
	if (to <= from) {
		ret = "";
	}
	else {
		ret = argvalues[0]->value.substr(from, to - from);
		ret.datatype = DataValue::DATATYPE_STR;
	}
	return 0;
}

int func_strpos(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	unsigned int offs = 0;
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
		offs = (int)*argvalues[2];
		if (offs >= argvalues[0]->value.size())
			offs = 0;
	}

	const char *str1 = argvalues[0]->value.c_str() + offs;
	const char *str2 = argvalues[1]->value.c_str();
	if (str2[0] == 0) {
		ret = false;
	}
	else {
		const char *pos = strstr(str1, str2);
		if (!pos)
			ret = false;
		else
			ret = (int)(pos - str1);
	}
	return 0;
}

int func_strtoupper(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	ret = argvalues[0]->value;
	transform(ret.value.begin(), ret.value.end(), ret.value.begin(), ::toupper);
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_strtolower(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	ret = argvalues[0]->value;
	transform(ret.value.begin(), ret.value.end(), ret.value.begin(), ::tolower);
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

bool findTrimChar(char c, const char *trimChars)
{
	while (*trimChars != 0) {
		if (*trimChars == c)
			return true;
		trimChars++;
	}
	return false;
}

int func_trim_universal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx, int direction = 0)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	const char *trimChars = " \t\n\r\0\x0B";

	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype != DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
		argvalues[1]->toString();
		trimChars = argvalues[1]->value.c_str();
	}

	const char *c0 = argvalues[0]->value.c_str();
	const char *c1 = argvalues[0]->value.c_str() + argvalues[0]->value.size();
	if (c1 > c0)
		c1--; // Zero char

	// Search from right
	if (!direction || direction == 1) {
		while (c1 >= c0) {
			if (!findTrimChar(*c1, trimChars)) {
				break;
			}
			c1--;
		}
	}
	// Search from left
	if (!direction || direction == 2) {
		while (c0 <= c1) {
			if (!findTrimChar(*c0, trimChars)) {
				break;
			}
			c0++;
		}
	}
	int l = (c1 - c0) + 1;
	if (*c0 == 0)
		l = 0;

	ret.value.assign(c0, l);
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_trim(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_trim_universal(argvalues, ret, ctx, 0);
}

int func_rtrim(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_trim_universal(argvalues, ret, ctx, 1);
}

int func_ltrim(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_trim_universal(argvalues, ret, ctx, 2);
}

int func_str_replace(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR && argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || (argvalues[1]->datatype > DataValue::DATATYPE_STR && argvalues[1]->datatype < DataValue::DATATYPE_ARRAY)) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 3 || argvalues[2]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	argvalues[1]->toString();
	argvalues[2]->toString();

	ret.value = argvalues[2]->value;
	ret.datatype = DataValue::DATATYPE_STR;

	string empty;
	string *find, *replace = &empty;

	W32 cnt = 1;
	if (argvalues[0]->datatype >= DataValue::DATATYPE_ARRAY) {
		cnt = argvalues[0]->arrayList.size();
		argvalues[0]->arrayList.reset();
	}
	do {

		if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
			find = &argvalues[0]->value;
		}
		else {
			if (argvalues[0]->arrayList.hasMoreElements())
				find = &argvalues[0]->arrayList.nextValuePnt()->value;
			else
				break;
		}

		if (argvalues[1]->datatype < DataValue::DATATYPE_ARRAY) {
			replace = &argvalues[1]->value;
		}
		else {
			if (argvalues[1]->arrayList.hasMoreElements())
				replace = &argvalues[1]->arrayList.nextValuePnt()->value;
		}
		std::string::size_type start_pos = 0;
		std::string::size_type search_length = find->length();
		if (search_length > 0) {
			std::string::size_type replace_length = replace->length();
			std::string::size_type found_pos = ret.value.find(*find, start_pos);
			while (std::string::npos != found_pos) {
				ret.value = ret.value.replace(found_pos, search_length, *replace);
				start_pos = found_pos + replace_length;
				found_pos = ret.value.find(*find, start_pos);
			}
		}
		cnt--;
	}
	while (cnt > 0);
	return 0;
}

int func_exit(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_NO_PARAM) {
		// Set exit code
		if (PARAM1_NOSTR) {
			return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
		}
		else {
			ctx.exitCode = (int)argvalues[0]->numvalue;
		}
	}
	ctx.fAbort = 2;
	throw AbortException("Exited", ctx.exitCode);
	ret = true;
	return 0;
}

int func_include_internal(WCSTR filename, DataValue &ret, InterpreterContext &ctx, bool fRequireOnce)
{
	WString path = filename;
	if (path.indexOf(":") != -1 || path.charAt(0) == '/' || path.charAt(0) == '\\') {
		// Absolute path
	}
	else {
		// Relative path
		WSystem::extractPath(ctx.getFilenameByIdx(WSCRIPT_FILE(ctx.line)), path);
		path = WFormattedString("%s/%s", (WCSTR)path, (WCSTR)filename);
	}

	if (!WSystem::existsFile(path)) {
		ctx.abortInterprete(WFormattedString("Include file %s not found", (WCSTR)filename));
		ret = false;
		return WSCRIPT_RET_ABORT;
	}

	// Reset status vars
	int loopDeep = ctx.loopDeep;
	int switchDeep = ctx.switchDeep;
	int funcDeep = ctx.funcDeep;
	int flag = ctx.flag;
	ctx.loopDeep = 0;
	ctx.switchDeep = 0;
	ctx.funcDeep = 1;
	ctx.flag = 0;

	ctx.exitCode = ctx.execute(path, fRequireOnce);
	if (ctx.fAbort == 1) {
		throw AbortException("Aborted", -2);
	}
	if (ctx.fAbort == 2) {
		throw AbortException("Exited", ctx.exitCode);
	}

	// Restore status vars
	ctx.loopDeep = loopDeep;
	ctx.switchDeep = switchDeep;
	ctx.funcDeep = funcDeep;
	ctx.flag = flag;
	ret = true;
	return 0;
}

int func_include(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	return func_include_internal(argvalues[0]->value.c_str(), ret, ctx, false);
}

int func_require_once(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	return func_include_internal(argvalues[0]->value.c_str(), ret, ctx, true);
}

int func_eval(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	ctx.exitCode = ctx.eval(argvalues[0]->value.c_str(), argvalues[0]->value.size(), ret);
	if (ctx.fAbort == 1) {
		throw AbortException("Aborted", -2);
	}
	if (ctx.fAbort == 2) {
		throw AbortException("Exited", ctx.exitCode);
	}
	return 0;
}

int func_fread(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx);

int func_exec(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() > 1) {
		argvalues[1]->refPnt->datatype = DataValue::DATATYPE_STR;
	}
	if (argvalues.size() > 2) {
		argvalues[2]->refPnt->datatype = DataValue::DATATYPE_NUM;
	}

	int contentLength = 0;
	if (argvalues.size() > 4) {
		if (argvalues[4]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM4 | WSCRIPT_RET_NUM_REQUIRED;
		contentLength = (int)argvalues[4]->numvalue;
	}

	WCSTR envParamStr = NULL;
	if (argvalues.size() > 5) {
		if (argvalues[5]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM5 | WSCRIPT_RET_STR_REQUIRED;
		argvalues[5]->toString();
		envParamStr = argvalues[5]->value.c_str();
	}

	bool retCode = false;
	if (argvalues.size() > 1) {
		argvalues[1]->refPnt->value.resize(0);
	}
	try {
		Process process;
		WString envAllStr(1024);

		if (envParamStr) {
			envAllStr.append((W8 *)argvalues[5]->value.c_str(), argvalues[5]->value.size());
			// Ending 0
			envAllStr.append(0);
		}

		if (process.execute(argvalues[0]->value.c_str(), (void *)(envAllStr.length() > 0 ? (WCSTR)envAllStr : NULL))) {
			const Process::Handles *handles = process.getHandles();

			W8 buf[0xFFFF];
			W32 bytesRead = 0;
			W32 bytesWritten = 0;
			buf[sizeof(buf) - 1] = 0;

			if (argvalues.size() > 3) {
				// Write input to process
				if (argvalues[3]->datatype <= DataValue::DATATYPE_STR) {
					argvalues[3]->toString();
					if (write(handles->aStdinPipe[1], argvalues[3]->value.c_str(), argvalues[3]->value.size()) == 0)
						bytesWritten = argvalues[3]->value.size();
				}
				else if (argvalues[3]->datatype == DataValue::DATATYPE_HANDLE) {
					vector<DataValue *> tmpArgvalues;
					DataValue tmpRet, tmpRead;
					tmpRead.datatype = DataValue::DATATYPE_STR;
					tmpRead.refPnt = &tmpRead;
					tmpArgvalues.push_back(argvalues[3]);
					tmpArgvalues.push_back(&tmpRead);
					if (contentLength > 0) {
						tmpArgvalues.push_back(argvalues[4]);
					}

					WSCRIPT_NATIVE_FUNCTION readFunc = func_fread;

					if (((Handle *)argvalues[3]->hvalue)->handletype == 13 && ctx.nativeFunctionHT.isKey("SSL_read")) {
						readFunc = ctx.nativeFunctionHT.get("SSL_read");
					}

					while (!readFunc(tmpArgvalues, tmpRet, ctx) && tmpRet.datatype == DataValue::DATATYPE_NUM && (bytesRead = (W32)tmpRet.numvalue) > 0) {
						bool fSuccess = (write(handles->aStdinPipe[1], tmpRead.value.c_str(), bytesRead) == 0);
						if (!fSuccess)
							break;
						bytesWritten = bytesRead;
						if (contentLength > 0) {
							contentLength -= bytesRead;
							tmpArgvalues[2]->numvalue = (double)contentLength;
							if (contentLength <= 0)
								break;
						}
					}
				}
			}

			// Close the pipe handles so the child process stops reading
			process.startRead();
			// Read output from process
			do {
				bool fSuccess = (bytesRead = read(handles->aStdoutPipe[0], buf, sizeof(buf) - 1)) != -1;
				if (!fSuccess || bytesRead == 0)
					break;
				// Store result
				if (argvalues.size() > 1) {
					argvalues[1]->refPnt->value.append((const char *)buf, bytesRead);
				}
				// process.join (100);
			}
			while (process.isRunning());

			// Get exit code
			if (argvalues.size() > 2) {
				*(argvalues[2]->refPnt) = (double)process.getExitCode();
			}

			retCode = true;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
	}
	ret = retCode;
	return 0;
}

int func_shell_exec(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	/*
	if (argvalues[0]->datatype!=DataValue::DATATYPE_STR) {
	    return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
	}
	string operation = "open";
	if (argvalues.size ()>1) {
	    if (argvalues[1]->datatype>DataValue::DATATYPE_STR)
	        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	    argvalues[1]->toString ();
	    operation = argvalues[1]->value;
	}
	bool fHide = false;
	if (argvalues.size ()>2) {
	    if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
	        return WSCRIPT_RET_PARAM3|WSCRIPT_RET_BOOL_REQUIRED;
	    fHide = (bool) *argvalues[2];
	}

	ret = false;
	ret = (bool) ( (W32) ShellExecute(
	    NULL,
	    operation.c_str (),
	    argvalues[0]->value.c_str (),
	    NULL,
	    NULL,
	    fHide ? SW_HIDE : SW_SHOW
	) > 32);
	if (!ret) {
	    ctx.warnInterprete (WFormattedString ("ShellExecute %s %s failed.", operation.c_str (), argvalues[0]->value.c_str () ));

	}
	*/
	ctx.warnInterprete("ShellExecute is not available on Linux.");
	ret = false;
	return 0;
}

int func_system(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	try {
		Process process;
		if (process.execute(argvalues[0]->value.c_str())) {
			const Process::Handles *handles = process.getHandles();
			// Close the pipe handles so the child process stops reading
			process.startRead();

			// Read output from process
			W32 bytesRead;
			char buf[4096 + 1];
			buf[sizeof(buf) - 1] = 0;

			ret.datatype = DataValue::DATATYPE_STR;
			ret.value.resize(0);
			// Read output from process
			do {
				bool fSuccess = (bytesRead = read(handles->aStdoutPipe[0], buf, sizeof(buf) - 1)) != -1;
				if (!fSuccess || bytesRead == 0)
					break;
				ret.value.append(buf, bytesRead);
				// process.join (100);
			}
			while (process.isRunning());
		}
		else
			ret = false;
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_version(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (argvalues[0]->datatype > DataValue::DATATYPE_STR)
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;

	int type = (int)*argvalues[0];

	utsname unameBuf;
	uname(&unameBuf);

	switch (type) {
		case 1:
			ret = (WCSTR)unameBuf.sysname;
			break;

		case 2:
			ret = (WCSTR)unameBuf.release;
			break;

		case 3:
			ret = strstr(unameBuf.machine, "_64") != NULL ? 1 : 0;

		case 4:

			if (type == 4) {
				ret.datatype = DataValue::DATATYPE_ARRAY;
				ret.arrayList.removeAll();
				DataValue tmp, tmp2;
				tmp.datatype = DataValue::DATATYPE_ARRAY;
				tmp.arrayList.setOwnKeys(true);
				tmp2.datatype = DataValue::DATATYPE_ARRAY;

				DataValue v;
				tmp.arrayList.put("type", v = DataValue((WCSTR)unameBuf.sysname));
				tmp.arrayList.put("version", v = DataValue((WCSTR)unameBuf.release));
				tmp.arrayList.put("is64Bit", v = DataValue(strstr(unameBuf.machine, "_64") != NULL ? 1 : 0));

				tmp2.arrayList.removeAll();
				tmp2.arrayList.setOwnKeys(true);
				tmp2.arrayList.put("sysname", v = DataValue((WCSTR)unameBuf.sysname));
				tmp2.arrayList.put("nodename", v = DataValue((WCSTR)unameBuf.nodename));
				tmp2.arrayList.put("release", v = DataValue((WCSTR)unameBuf.release));
				tmp2.arrayList.put("version", v = DataValue((WCSTR)unameBuf.version));
				tmp2.arrayList.put("machine", v = DataValue((WCSTR)unameBuf.machine));
#if _GNU_SOURCE
				tmp2.arrayList.put("domainname", v = DataValue((WCSTR)unameBuf.domainname));
#endif
#if _HAVE_LIBC_VERSION_H
				tmp2.arrayList.put("libc Version", v = DataValue(gnu_get_libc_version()));
#endif
				tmp.arrayList.put("details", tmp2);
				ret.arrayList.put("osData", tmp);

				// GEO data
				tmp.arrayList.removeAll();
				tmp.arrayList.setOwnKeys(true);
				/*
				char buf[128];
				memset (buf,0,sizeof (buf));
				setlocale(LC_ALL, "");
				tmp.arrayList.put ("locale", v=DataValue ((WCSTR) setlocale(LC_CTYPE, NULL)));
				*/

				WDate date;
				tmp2.arrayList.removeAll();
				tmp2.arrayList.setOwnKeys(true);
				tmp2.arrayList.put("timezone", v = DataValue(date.format("%e")));
				tmp2.arrayList.put("gmtOffset", v = DataValue(atoi(date.format("%Z"))));
				tmp.arrayList.put("timezoneData", tmp2);
				ret.arrayList.put("geoData", tmp);

				tmp.arrayList.removeAll();
				tmp.arrayList.setOwnKeys(true);
				tmp.arrayList.put("version", v = DataValue(WSCRIPT_VERSION));
				tmp.arrayList.put("buildDate", v = DataValue(__TIMESTAMP__));
				tmp.arrayList.put("compilerName", v = DataValue("GCC"));
				tmp.arrayList.put("compilerVersion", v = DataValue(__VERSION__));

				ret.arrayList.put("v1Data", tmp);
				ret.arrayList.setOwnKeys(true);
			}
			break;

		default:
			ret = (double)WSCRIPT_VERSION;
			break;
	}
	return 0;
}

int func_opendir(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	if (!WSystem::existsFile(argvalues[0]->value.c_str(), true)) {
		ret = false;
		return 0;
	}

	Handle *h = new Handle;
	try {
		h->handle = (void *)new WDirectoryEntries(argvalues[0]->value.c_str());
		// Register handle
		h->handletype = Handle::HANDLETYPE_DIRECTORY;
		h->freeFunction = freeHandle;
		ret = ctx.handleHT.put((void *)h, h)->m_value;
	}
	catch (WException) {
		if (h->handle) {
			delete (WDirectoryEntries *)h->handle;
			h->handle = NULL;
		}
		delete h;
		h = NULL;
		ret = false;
	}
	return 0;
}

int func_closedir(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	Handle *h = (Handle *)*argvalues[0];
	if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_DIRECTORY) {
		// WDirectoryEntries* dir = (WDirectoryEntries*) h->handle;
		ret = true;
		// Remove handle
		ctx.handleHT.remove(h);
		delete h;
	}
	else {
		ret = false;
	}
	return 0;
}

int func_readdir(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	Handle *h = (Handle *)*argvalues[0];
	if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_DIRECTORY) {
		WDirectoryEntries *dir = (WDirectoryEntries *)h->handle;
		try {
			WFileInfo fi;
			if (!dir->nextEntry(fi))
				ret = false;
			else {
				ret = (WCSTR)fi.getFilename();
			}
		}
		catch (WException) {
			ret = false;
		}
	}
	else {
		ret = false;
	}
	return 0;
}

int func_fopen(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	string mode = "r";
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype <= DataValue::DATATYPE_STR)
			mode = argvalues[1]->value;
		else
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	Handle *h = new Handle;

	try {
		h->handle = (void *)new WFile;
		bool fCheck = false;
		W32 flag = 0;

		if (mode == "r")
			flag = WFile::FILE_READ;
		else if (mode == "r+")
			flag = WFile::FILE_READ | WFile::FILE_WRITE;
		else if (mode == "w")
			flag = WFile::FILE_CREATE | WFile::FILE_WRITE;
		else if (mode == "w+")
			flag = WFile::FILE_CREATE | WFile::FILE_READ | WFile::FILE_WRITE;
		else if (mode == "a")
			flag = WFile::FILE_CREATE | WFile::FILE_APPEND | WFile::FILE_WRITE;
		else if (mode == "a+")
			flag = WFile::FILE_CREATE | WFile::FILE_APPEND | WFile::FILE_READ | WFile::FILE_WRITE;
		else if (mode == "x") {
			fCheck = true;
			flag = WFile::FILE_CREATE | WFile::FILE_WRITE;
		}
		else if (mode == "x+") {
			fCheck = true;
			flag = WFile::FILE_CREATE | WFile::FILE_READ | WFile::FILE_WRITE;
		}
		// Modes "c", "c+" not implemented
		if (argvalues[0]->value == "v1://stdin")
			((WFile *)*h)->open(stdin, false, false);
		else if (argvalues[0]->value == "v1://stdout")
			((WFile *)*h)->open(stdout, false, false);
		else if (argvalues[0]->value == "v1://stderr")
			((WFile *)*h)->open(stderr, false, false);
		else if (fCheck && !WSystem::existsFile(argvalues[0]->value.c_str())) {
			// File does not exists
		}
		else {
			((WFile *)*h)->open(argvalues[0]->value.c_str(), flag);
		}

		if ((WFile *)*h) {
			// Register handle
			h->handletype = Handle::HANDLETYPE_FILE;
			h->freeFunction = freeHandle;
			ret = ctx.handleHT.put((void *)h, h)->m_value;
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		if (h->handle) {
			delete (WFile *)h->handle;
			h->handle = NULL;
		}
		delete h;
		h = NULL;
		ret = false;
	}
	return 0;
}

int func_fsockopen(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	int port = 80;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype <= DataValue::DATATYPE_STR)
			port = (int)*argvalues[1];
		else
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	int rcvTimeout = 0;
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype <= DataValue::DATATYPE_STR)
			rcvTimeout = (int)*argvalues[2];
		else
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
	}
	Handle *h = new Handle;

	try {
		h->handle = (void *)new WSocket;
		WIPAddress ip(argvalues[0]->value.c_str(), port);
		((WSocket *)*h)->init(ip);
		((WSocket *)*h)->create();
		((WSocket *)*h)->connect();
		if (rcvTimeout)
			((WSocket *)*h)->setRecvTimeout(rcvTimeout);
		// Register handle
		h->handletype = Handle::HANDLETYPE_SOCKET;
		h->freeFunction = freeHandle;
		ret = ctx.handleHT.put((void *)h, h)->m_value;
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		if (h->handle) {
			delete (WSocket *)h->handle;
			h->handle = NULL;
		}
		delete h;
		h = NULL;
		ret = false;
	}
	return 0;
}

int func_fsockserver(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	int port = 80;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype <= DataValue::DATATYPE_STR)
			port = (int)*argvalues[1];
		else
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	int backlog = 10;
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype <= DataValue::DATATYPE_STR)
			backlog = (int)*argvalues[2];
		else
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
	}
	Handle *h = new Handle;

	try {

		WIPAddress ip(argvalues[0]->value.c_str(), port);
		h->handle = (void *)new WServerSocket(ip, backlog);
		((WServerSocket *)*h)->create();
		((WServerSocket *)*h)->setReuseAddress(true);

		// Register handle
		h->handletype = Handle::HANDLETYPE_SERVERSOCKET;
		h->freeFunction = freeHandle;
		ret = ctx.handleHT.put((void *)h, h)->m_value;
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		if (h->handle) {
			delete (WServerSocket *)h->handle;
			h->handle = NULL;
		}
		delete h;
		h = NULL;
		ret = false;
	}
	return 0;
}

int func_fsockaccept(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	int rcvTimeout = 0;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype <= DataValue::DATATYPE_STR)
			rcvTimeout = (int)*argvalues[1];
		else
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
	}
	Handle *h = (Handle *)*argvalues[0];

	if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_SERVERSOCKET) {
		Handle *h2 = NULL;
		WSocket *socket = NULL;
		try {
			// Accept socket
			WSocket *socket = new WSocket;
			((WServerSocket *)h->handle)->accept(*socket);
			if (rcvTimeout)
				socket->setRecvTimeout(rcvTimeout);

			// Create new socket handle
			h2 = new Handle;
			h2->handletype = Handle::HANDLETYPE_SOCKET;
			h2->handle = (void *)socket;
			h2->freeFunction = freeHandle;
			ret = ctx.handleHT.put((void *)h2, h2)->m_value;
		}
		catch (WException &e) {
			ctx.warnInterprete(e);
			if (socket) {
				delete socket;
				socket = NULL;
			}
			ret = false;
		}
	}
	else {
		ret = false;
	}
	return 0;
}

int func_fsockip(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	Handle *h = (Handle *)*argvalues[0];

	if (h && ctx.handleHT.isKey(h) && (h->handletype == Handle::HANDLETYPE_SOCKET || h->handletype == Handle::HANDLETYPE_SERVERSOCKET)) {
		ret.value.assign(WSocketUtils::ipAddr2String(((WSocket *)h->handle)->getIpAddress()));
		ret.datatype = DataValue::DATATYPE_STR;
	}
	else {
		ret = false;
	}
	return 0;
}

int func_fsockport(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	Handle *h = (Handle *)*argvalues[0];
	if (h && ctx.handleHT.isKey(h) && (h->handletype == Handle::HANDLETYPE_SOCKET || h->handletype == Handle::HANDLETYPE_SERVERSOCKET)) {
		ret = (int)((WSocket *)h->handle)->getPort();
	}
	else {
		ret = false;
	}
	return 0;
}

int func_gethostbyaddr(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	try {
		W32 ip = WSocketUtils::ipAddr2NBO(argvalues[0]->value.c_str());
		ret.value.assign(WSocketUtils::resolveIpAddress(ip));
		ret.datatype = DataValue::DATATYPE_STR;
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_gethostbyname(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	try {
		W32 ip = WSocketUtils::resolveHost(argvalues[0]->value.c_str());
		ret.value.assign(WSocketUtils::ipAddr2String(ip));
		ret.datatype = DataValue::DATATYPE_STR;
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_fclose(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	Handle *h = (Handle *)*argvalues[0];

	if (h && ctx.handleHT.isKey(h) && (h->handletype == Handle::HANDLETYPE_FILE || h->handletype == Handle::HANDLETYPE_SOCKET || h->handletype == Handle::HANDLETYPE_SERVERSOCKET)) {
		try {
			delete h;
			ret = true;
		}
		catch (WException) {
		}
		*(argvalues[0]->refPnt) = (Handle *)NULL;
		ctx.handleHT.remove(h);
		ret = true;
	}
	else {
		ret = false;
	}
	return 0;
}

int func_fflush(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	Handle *h = (Handle *)*argvalues[0];
	if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_FILE) {
		WFile *file = (WFile *)h->handle;
		try {
			file->flush();
			ret = true;
		}
		catch (WException) {
			ret = false;
		}
	}
	else {
		ret = false;
	}
	return 0;
}

int func_fwrite(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}

	int size = -1;
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
		size = (int)argvalues[2]->numvalue;
	}

	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h)) {
			if (argvalues[1]->datatype != DataValue::DATATYPE_STR)
				argvalues[1]->toString();
			if (h->handletype == Handle::HANDLETYPE_FILE) {
				WFile *file = (WFile *)h->handle;
				if (size >= 0)
					ret = file->write(argvalues[1]->value.c_str(), size);
				else
					ret = file->write(argvalues[1]->value.c_str(), argvalues[1]->value.length());
			}
			else if (h->handletype == Handle::HANDLETYPE_SOCKET) {
				WSocket *socket = (WSocket *)h->handle;
				if (size >= 0) {
					ret = socket->write(argvalues[1]->value.c_str(), size);
				}
				else
					ret = socket->write(argvalues[1]->value.c_str(), argvalues[1]->value.length());
			}
			else if (h->handletype == 13) {
				WSCRIPT_NATIVE_FUNCTION *writeFunc = ctx.nativeFunctionHT.getPnt("SSL_write");
				if (writeFunc)
					return (*writeFunc)(argvalues, ret, ctx);
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_fread(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->refPnt == NULL) {
		ctx.abortInterprete("Parameter 2 must be a reference to string variable");
		return 0;
	}
	unsigned int sizeMax = 0xFFFF;

	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
		sizeMax = (unsigned int)argvalues[2]->numvalue;
	}

	Handle *h = (Handle *)*argvalues[0];
	if (h && ctx.handleHT.isKey(h)) {
		WString str(0xFFFF);
		int bufSize = 0xFFFF;
		int size = 0;
		int bytesRead = 0;

		if (h->handletype == Handle::HANDLETYPE_FILE) {
			WFile *file = (WFile *)h->handle;
			try {
				argvalues[1]->refPnt->value.resize(0);
				argvalues[1]->refPnt->datatype = DataValue::DATATYPE_STR;
				if (sizeMax < bufSize)
					bufSize = sizeMax;
				while (sizeMax > 0 && (size = file->read(str.getBuf(), bufSize))) {
					argvalues[1]->refPnt->value.append(str, size);
					sizeMax -= size;
					bytesRead += size;
					if (bufSize > sizeMax)
						bufSize = sizeMax;
				}
				ret = bytesRead;
			}
			catch (WEOFException) {
				if (argvalues[1]->refPnt)
					argvalues[1]->refPnt->value.append(str, file->getEOFBytes());
				bytesRead += file->getEOFBytes();
				ret = bytesRead;
			}
			catch (WException &e) {
				ctx.warnInterprete(e);
				ret = false;
			}
		}
		else if (h->handletype == Handle::HANDLETYPE_SOCKET) {
			WSocket *socket = (WSocket *)h->handle;
			try {
				argvalues[1]->refPnt->datatype = DataValue::DATATYPE_STR;
				argvalues[1]->refPnt->value.resize(0);

				if (sizeMax < bufSize)
					bufSize = sizeMax;
				while (sizeMax > 0 && (size = socket->read(str.getBuf(), bufSize))) {
					argvalues[1]->refPnt->value.append(str, size);
					sizeMax -= size;
					bytesRead += size;
					if (bufSize > sizeMax)
						bufSize = sizeMax;
				}
				ret = bytesRead;
			}
			catch (WSocketException &e) {
				if (bytesRead == 0)
					ret = false;
				else
					ret = bytesRead;
				if (e.getId() != WSocketException::ERR_ID_CLOSED) {
					ctx.warnInterprete(e);
				}
				else {
					socket->close();
				}
			}
			catch (WException &e) {
				ctx.warnInterprete(e);
				ret = false;
			}
		}
		else if (h->handletype == 13) {
			WSCRIPT_NATIVE_FUNCTION *readFunc = ctx.nativeFunctionHT.getPnt("SSL_read");
			if (readFunc)
				return (*readFunc)(argvalues, ret, ctx);
			else
				ret = false;
		}
		else {
			ret = false;
		}
	}
	else {
		ret = false;
	}
	return 0;
}

int func_freadln(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->refPnt == NULL) {
		ctx.abortInterprete("Parameter 2 must be a reference to string variable");
		return 0;
	}

	try {
		Handle *h = (Handle *)*argvalues[0];
		if (h && ctx.handleHT.isKey(h)) {

			WString str(4096);
			if (h->handletype == Handle::HANDLETYPE_FILE) {

				WFile *file = (WFile *)h->handle;
				if (ret = file->readln(str)) {
					argvalues[1]->refPnt->datatype = DataValue::DATATYPE_STR;
					argvalues[1]->refPnt->value.assign(str, str.length());
				}
			}
			else if (h->handletype == Handle::HANDLETYPE_SOCKET) {
				WSocket *socket = (WSocket *)h->handle;
				if (ret = socket->readln(str)) {
					argvalues[1]->refPnt->datatype = DataValue::DATATYPE_STR;
					argvalues[1]->refPnt->value.assign(str, str.length());
				}
			}
			else if (h->handletype == 13) {
				WSCRIPT_NATIVE_FUNCTION *readFunc = ctx.nativeFunctionHT.getPnt("SSL_readln");
				if (readFunc)
					return (*readFunc)(argvalues, ret, ctx);
				else
					ret = false;
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_fgets(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	try {
		Handle *h = (Handle *)*argvalues[0];
		if (h && ctx.handleHT.isKey(h)) {

			WString str(4096);
			if (h->handletype == Handle::HANDLETYPE_FILE) {
				WFile *file = (WFile *)h->handle;
				if (file->readln(str)) {
					ret.datatype = DataValue::DATATYPE_STR;
					ret.value.assign(str, str.length());
				}
			}
			else if (h->handletype == Handle::HANDLETYPE_SOCKET) {
				WSocket *socket = (WSocket *)h->handle;
				if (socket->readln(str)) {
					ret.datatype = DataValue::DATATYPE_STR;
					ret.value.assign(str, str.length());
				}
			}
			else if (h->handletype == 13) {
				WSCRIPT_NATIVE_FUNCTION *readFunc = ctx.nativeFunctionHT.getPnt("SSL_gets");
				if (readFunc)
					return (*readFunc)(argvalues, ret, ctx);
				else
					ret = false;
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_fseek(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype != DataValue::DATATYPE_NUM) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}

	Handle *h = (Handle *)*argvalues[0];
	if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_FILE) {
		WFile *file = (WFile *)h->handle;
		try {
			file->setPos((W32)argvalues[1]->numvalue);
			ret = true;
		}
		catch (WException &e) {
			ctx.warnInterprete(e);
			ret = false;
		}
	}
	else {
		ret = false;
	}

	return 0;
}

int func_ftell(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	try {

		Handle *h = (Handle *)*argvalues[0];
		if (h && ctx.handleHT.isKey(h)) {
			if (h->handletype == Handle::HANDLETYPE_FILE) {
				WFile *file = (WFile *)h->handle;
				ret = (double)file->getPos();
			}
			else if (h->handletype == Handle::HANDLETYPE_SOCKET) {
				WSocket *socket = (WSocket *)h->handle;
				ret = (double)socket->getBytesRead();
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_feof(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h)) {
			if (h->handletype == Handle::HANDLETYPE_FILE) {
				WFile *file = (WFile *)h->handle;
				ret = file->isEOF();
			}
			else if (h->handletype == Handle::HANDLETYPE_SOCKET) {
				WSocket *socket = (WSocket *)h->handle;
				ret = !socket->isConnected();
			}
			else {
				ret = false;
			}
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_fileattr(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx, int attr = 1)
{
	WFile *file = NULL;

	if (argvalues[0]->datatype == DataValue::DATATYPE_HANDLE) {
		// Get open file handle
		Handle *h = (Handle *)*argvalues[0];
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_FILE) {
			file = (WFile *)h->handle;
		}
	}
	else if (argvalues[0]->datatype == DataValue::DATATYPE_STR) {
		// Open new file temporary
		try {
			file = new WFile();
			file->open(argvalues[0]->value.c_str(), 0);
		}
		catch (WException &e) {
			ctx.warnInterprete(e);
			ret = false;
			return 0;
		}
	}
	else {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (file) {
		try {
			WFileInfo fi;
			file->getFileInfo(fi);
			switch (attr) {
				case 1:
					ret = (double)fi.getFileSize();
					break;
				case 2:
					ret = (double)fi.getTimeCreation();
					break;
				case 3:
					ret = (double)fi.getTimeLastModified();
					break;
				case 4:
					ret = (double)fi.getTimeLastAccess();
					break;
				default:
					ret = false;
					break;
			}
		}
		catch (WException &e) {
			ctx.warnInterprete(e);
			ret = false;
		}
		if (argvalues[0]->datatype == DataValue::DATATYPE_STR) {
			delete file;
			file = NULL;
		}
	}
	else {
		ret = false;
	}
	return 0;
}

int func_filesize(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_fileattr(argvalues, ret, ctx, 1);
}

int func_filectime(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_fileattr(argvalues, ret, ctx, 2);
}

int func_filemtime(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_fileattr(argvalues, ret, ctx, 3);
}

int func_fileatime(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_fileattr(argvalues, ret, ctx, 4);
}

int func_is_file(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	ret = (bool)WSystem::existsFile(argvalues[0]->value.c_str(), 0);
	return 0;
}

int func_file_exists(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	ret = (bool)WSystem::existsFile(argvalues[0]->value.c_str(), 2);
	return 0;
}

int func_is_dir(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	ret = (bool)WSystem::existsFile(argvalues[0]->value.c_str(), 1);
	return 0;
}

int func_filetype(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	if (WSystem::existsFile(argvalues[0]->value.c_str(), true)) {
		ret.datatype = DataValue::DATATYPE_STR;
		ret.value = "dir";
	}
	else if (WSystem::existsFile(argvalues[0]->value.c_str(), false)) {
		ret.datatype = DataValue::DATATYPE_STR;
		ret.value = "file";
	}
	else
		ret = false;
	return 0;
}

int func_touch(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	WTIME time, timeA = 0;

	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		time = (WTIME)argvalues[1]->numvalue;
	}
	else {
		WDate date;
		time = date.getTime();
	}

	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
		timeA = (WTIME)argvalues[2]->numvalue;
	}

	struct stat fs;

	if (stat(argvalues[0]->value.c_str(), &fs) == -1) {
		ctx.warnInterprete(WException(WFormattedString("Cannot open: %s", argvalues[0]->value.c_str()), -2));
		ret = false;
	}
	else {
		struct utimbuf new_times;
		new_times.actime = fs.st_atime; /* keep atime unchanged */
		if (argvalues.size() > 2)
			new_times.actime = timeA;
		new_times.modtime = time; /* set mtime to current time */
		if (utime(argvalues[0]->value.c_str(), &new_times) == 0) {
			ret = true;
		}
		else {
			ctx.warnInterprete(WException(WFormattedString("Cannot set time: %s", argvalues[0]->value.c_str()), -1));
			ret = false;
		}
	}
	return 0;
}

int func_pathinfo(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	ret.datatype = 14; // Array with 4 elements
	ret.arrayList.removeAll();
	ret.arrayList.setOwnKeys(true);

	WString str, ext;
	DataValue value;
	value.datatype = DataValue::DATATYPE_STR;

	WSystem::extractPath(argvalues[0]->value.c_str(), str, false);
	if (str.length() == 0)
		str = "."; // Default local DIR
	value.value = str;

	ret.arrayList.put("dirname", value);

	WSystem::extractPath(argvalues[0]->value.c_str(), str, true);
	value.value = str;
	ret.arrayList.put("basename", value);

	bool fExt = WSystem::extractFileExtension(argvalues[0]->value.c_str(), ext);
	if (fExt) {
		value.value = ext;
		ret.arrayList.put("extension", value);
	}

	value.value.resize(0);
	int len = str.length();
	if (fExt)
		len -= (ext.length() + 1);
	if (len < 0)
		len = 0;
	value.value.append(str, len);
	ret.arrayList.put("filename", value);
	return 0;
}

int func_explode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	argvalues[1]->toString();

	/*
	bool fTrim = false;
	if (argvalues.size ()>2) {
	    if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
	        return WSCRIPT_RET_PARAM3|WSCRIPT_RET_BOOL_REQUIRED;

	    fTrim = (bool) *argvalues[2];
	}
	*/
	if (argvalues[0]->value.length() == 0) {
		ret = false;
		ctx.warnInterprete("Empty delimiter");
		return 0;
	}

	ret.datatype = 10; // Array
	ret.arrayList.removeAll();

	const char *c0 = argvalues[1]->value.c_str();
	const char *c = c0;
	const char *f = argvalues[0]->value.c_str();
	unsigned int idx = 0;

	while (*c != 0) {

		const char *check = c;
		while (*f != 0 && *check == *f && *check != 0) {
			f++;
			check++;
		}

		if (*f == 0) {

			// Found!
			DataValue value;
			value.datatype = DataValue::DATATYPE_STR;
			value.value.append(c0, c - c0);
			ret.arrayList.put(WFormattedString("%u", idx), value);
			idx++;
			f = argvalues[0]->value.c_str();
			c = c0 = check;
		}
		else {
			c++;
		}
	}

	// Last
	DataValue value;
	value.datatype = DataValue::DATATYPE_STR;
	value.value.append(c0, c - c0);
	ret.arrayList.put(WFormattedString("%u", idx), value);
	idx++;

	ret.datatype += idx;
	return 0;
}

int func_basename(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	WString path;
	WSystem::extractPath(argvalues[0]->value.c_str(), path, true);
	ret.value = path;
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_dirname(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	WString path;
	WSystem::extractPath(argvalues[0]->value.c_str(), path, false);
	ret.value = path;
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_getcwd(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	WString path;
	WSystem::getCurrentDirectory(path);
	ret.value = path;
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_chdir(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	if (WSystem::setCurrentDirectory(argvalues[0]->value.c_str())) {
		ret = true;
	}
	else {
		ctx.warnInterprete(WFormattedString("Cannot change directory to %s", (WCSTR)argvalues[0]->value.c_str()));
		ret = false;
	}
	return 0;
}

int func_unlink(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	ret = WSystem::removeFile(argvalues[0]->value.c_str());
	return 0;
}

int func_rename(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	argvalues[1]->toString();
	if (WSystem::renameFile((WCSTR)argvalues[0]->value.c_str(), (WCSTR)argvalues[1]->value.c_str())) {
		ret = true;
	}
	else {
		ctx.warnInterprete(WFormattedString("Cannot rename %s to %s", (WCSTR)argvalues[0]->value.c_str(), (WCSTR)argvalues[1]->value.c_str()));
		ret = false;
	}
	return 0;
}

int func_copy(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	argvalues[1]->toString();
	if (WSystem::copyFile((WCSTR)argvalues[0]->value.c_str(), (WCSTR)argvalues[1]->value.c_str())) {
		ret = true;
	}
	else {
		ctx.warnInterprete(WFormattedString("Cannot copy file %s to %s", (WCSTR)argvalues[0]->value.c_str(), (WCSTR)argvalues[1]->value.c_str()));
		ret = false;
	}
	return 0;
}

int func_realpath(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	WString path = argvalues[0]->value.c_str();
	try {
		WFile file(argvalues[0]->value.c_str());
		file.getRealFilename(path);
		ret.value = path;
		ret.datatype = DataValue::DATATYPE_STR;
	}
	catch (WException) {
		ret = false;
	}
	return 0;
}

int func_mkdir(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	if (WSystem::createDirectory(argvalues[0]->value.c_str())) {
		ret = true;
	}
	else {
		ctx.warnInterprete(WFormattedString("Cannot create directoy %s", (WCSTR)argvalues[0]->value.c_str()));
		ret = false;
	}
	return 0;
}

int func_rmdir(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	if (WSystem::removeDirectory(argvalues[0]->value.c_str())) {
		ret = true;
	}
	else {
		ctx.warnInterprete(WFormattedString("Cannot remove directoy %s", (WCSTR)argvalues[0]->value.c_str()));
		ret = false;
	}
	return 0;
}

int func_gettype(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret.value = argvalues[0]->describeDataType();
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_intval(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (int)*argvalues[0];
	return 0;
}

int func_doubleval(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (double)*argvalues[0];
	return 0;
}

int func_strval(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (string)argvalues[0]->value;
	return 0;
}

int func_utf8_decode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	bool fIsUnicode = false;
	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.resize(0);

	char *c = (char *)argvalues[0]->value.c_str();
	unsigned int codepoint;
	while (*c != 0) {
		unsigned char ch = static_cast<unsigned char>(*c);
		if (ch <= 0x7f)
			codepoint = ch;
		else if (ch <= 0xbf)
			codepoint = (codepoint << 6) | (ch & 0x3f);
		else if (ch <= 0xdf)
			codepoint = ch & 0x1f;
		else if (ch <= 0xef)
			codepoint = ch & 0x0f;
		else
			codepoint = ch & 0x07;
		++c;
		if (((*c & 0xc0) != 0x80) && (codepoint <= 0x10ffff)) {
			if (codepoint <= 255) {
				ret.value.append(1, static_cast<char>(codepoint));
			}
			else {
				ret.value.append("?");
				fIsUnicode = true;
			}
		}
	}

	return 0;
}

int func_utf8_encode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.resize(0);

	bool fEncoded = false;
	const char *str = argvalues[0]->value.c_str();
	while (*str) {
		if ((unsigned char)*str < 128)
			ret.value.append(1, *str++);
		else {
			fEncoded = true;
			ret.value.append(1, (char)0xc2 + ((unsigned char)*str > 0xbf));
			ret.value.append(1, (char)(int)((unsigned char)*str++ & 0x3f) + 0x80);
		}
	}
	return 0;
}

int func_empty(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype == DataValue::DATATYPE_STR) {
		ret = (bool)(argvalues[0]->value.size() == 0);
	}
	else if (argvalues[0]->datatype >= DataValue::DATATYPE_ARRAY) {
		ret = (bool)(argvalues[0]->arrayList.size() == 0);
	}
	else {
		ret = (bool)(argvalues[0]->numvalue == 0);
	}
	return 0;
}

void encodeReadableString (string& src, string& dst) 
{
	unsigned int size = src.size ();
	if (size>512) {
		dst+=WFormattedString ("[Big String with %u bytes]", size);
		return;
	}
	char* c = (char*) src.c_str ();
	while (size>0) {
		if (*c>=32) {
			dst+=*c;
		}
		else {
			unsigned char tmp[3];
			tmp[2]=0;
			sprintf ((char*) tmp, "%02x", (unsigned) *c & 0xFF); 
			dst.append ("\\x");
			dst.append ((const char*) tmp, 2);
		}
		c++;
		size--;
	}
}

string dumpArray(ArrayHT &arrayList, WCSTR tab = "")
{
	string str2, str;
	arrayList.reset();
	string dst;
	dst.reserve(255);

	while (arrayList.hasMoreElements()) {
		ArrayHT::WHash *hash;
		DataValue *value = arrayList.nextValuePnt(&hash);
		/*
		if (value->datatype==0)
		    continue;
		*/

		value->toString(true);
		if (str.length() > 0)
			str.append("\r\n");
		str.append(tab);
		str.append("    ");
		str.append("[");
		str.append(hash->m_key);
		str.append("]");
		str.append(" => ");
		if (value->datatype >= DataValue::DATATYPE_ARRAY) {
			str.append(dumpArray(value->arrayList, (string(tab) + "    ").c_str()));
		}
		else {
			dst.clear();
			encodeReadableString(value->value, dst);
			str.append(dst);
		}
	}

	str2.append("Array\r\n");
	str2.append(tab);
	if (tab[0] != 0)
		str2.append("  ");

	str2.append("(\r\n");
	str2.append(str);
	str2.append("\r\n");
	str2.append(tab);
	if (tab[0] != 0)
		str2.append("  ");

	str2.append(")\r\n");
	return str2;
}

int func_print_r(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	WMutexLocker locker(ctx.mutex);

	string str;
	if (argvalues[0]->datatype >= DataValue::DATATYPE_ARRAY)
		str = dumpArray(argvalues[0]->arrayList);
	else {
		argvalues[0]->toString(true);
		encodeReadableString(argvalues[0]->value, str);
	}
	if (argvalues.size() > 1 && (bool)*argvalues[1])
		ret = str; // Store result in string
	else {
		ctx.printHeaders();
		cout << str.c_str();
		if (ctx.fAutoNL)
			printf("\r\n");
		else if (ctx.programMode != InterpreterContext::PROGRAM_MODE_WEB)
			fflush(stdout);
		ret = true;
	}
	return 0;
}

void encodeJSONString(WCSTR src, string &dst)
{
	char *c = (char *)src;
	while (*c != 0) {
		if (*c == '"')
			dst += "\\\"";
		else if (*c == '\\')
			dst += "\\\\";
		else if (*c == '\n')
			dst += "\\n";
		else if (*c == '\r')
			dst += "\\r";
		else if (*c == '\t')
			dst += "\\t";
		else if (*c == '\f')
			dst += "\\f";
		else if (*c == '\b')
			dst += "\\b";
		else
			dst += *c;
		c++;
	}
}

string dumpJSON(ArrayHT &arrayList)
{
	string str2, str;
	arrayList.reset();
	while (arrayList.hasMoreElements()) {
		ArrayHT::WHash *hash;
		DataValue *value = arrayList.nextValuePnt(&hash);
		/*
		if (value->datatype==0)
		    continue;
		*/
		bool fSep = str.size() > 0;

		if (fSep)
			str.append(",");

		if (arrayList.hasOwnKeys()) {
			str.append("\"");
			str.append(hash->m_key);
			str.append("\"");
			str.append(":");
		}

		if (value->datatype >= DataValue::DATATYPE_ARRAY) {
			str.append(dumpJSON(value->arrayList));
		}
		else {
			if (value->datatype != DataValue::DATATYPE_STR)
				value->toString(true);

			if (value->datatype == DataValue::DATATYPE_STR) {
				str.append("\"");
				string enc;
				enc.reserve(value->value.size());
				encodeJSONString(value->value.c_str(), enc);
				str.append(enc);
				str.append("\"");
			}
			else if (value->datatype == 0) {
				str.append("null");
			}
			else if (value->datatype == DataValue::DATATYPE_BOOL) {
				str.append(value->numvalue ? "true" : "false");
			}
			else {
				str.append(value->value);
			}
		}
	}

	if (arrayList.hasOwnKeys()) {
		str2.append("{");
		str2.append(str);
		str2.append("}");
	}
	else {
		str2.append("[");
		str2.append(str);
		str2.append("]");
	}
	return str2;
}

int func_json_encode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	ret = dumpJSON(argvalues[0]->arrayList);
	return 0;
}


#define LITTLE_ENDIAN 0
/**
 *UTF16LEToUTF8:
 *@out:  a pointer to an array of bytes to store the result
 *@outlen:  the length of @out
 *@inb:  a pointer to an array of UTF-16LE passwd as a byte array
 *@inlenb:  the length of @in in UTF-16LE chars
 *
 *Take a block of UTF-16LE ushorts in and try to convert it to an UTF-8
 *block of chars out. This function assume the endian properity
 *is the same between the native type of this machine and the
 *inputed one.
 *
 *Returns the number of byte written, or -1 by lack of space, or -2
 *    if the transcoding fails (for *in is not valid utf16 string)
 *    The value of *inlen after return is the number of octets consumed
 *    as the return value is positive, else unpredictiable.
 */
int UTF16LEToUTF8(unsigned char *out, int *outlen, const unsigned char *inb, int *inlenb)
{
	unsigned char *outstart = out;
	const unsigned char *processed = inb;
	unsigned char *outend = out + *outlen;
	unsigned short *in = (unsigned short *) inb;
	unsigned short * inend;
	unsigned int c, d, inlen;
	unsigned char *tmp;
	int bits;

	if ((*inlenb % 2) == 1)
		(*inlenb) --;
	inlen = *inlenb / 2;
	inend = in +inlen;
	while ((in < inend) && (out - outstart + 5<*outlen))
	{
		if (LITTLE_ENDIAN)
		{
			c = *in ++;
		}
		else
		{
			tmp = (unsigned char *) in ;
			c = *tmp++;
			c = c | (((unsigned int) *tmp) << 8); in ++;
		}

		if ((c & 0xFC00) == 0xD800)
		{ 
			/*surrogates */
			if (in >= inend)
			{ 
				/*(in > inend) shouldn't happens */
				break;
			}

			if (LITTLE_ENDIAN)
			{
				d = *in ++;
			}
			else
			{
				tmp = (unsigned char *) in ;
				d = *tmp++;
				d = d | (((unsigned int) *tmp) << 8); in ++;
			}

			if ((d & 0xFC00) == 0xDC00)
			{
				c &= 0x03FF;
				c <<= 10;
				c |= d &0x03FF;
				c += 0x10000;
			}
			else
			{ 	*outlen = out - outstart;
				*inlenb = processed - inb;
				return (-2);
			}
		}

		/*assertion: c is a single UTF-4 value */
		if (out >= outend)
			break;
		if (c < 0x80)
		{
			*out++ = c;
			bits = -6;
		}
		else if (c < 0x800)
		{
			*out++ = ((c >> 6) &0x1F) | 0xC0;
			bits = 0;
		}
		else if (c < 0x10000)
		{
			*out++ = ((c >> 12) &0x0F) | 0xE0;
			bits = 6;
		}
		else
		{
			*out++ = ((c >> 18) &0x07) | 0xF0;
			bits = 12;
		}

		for (; bits >= 0; bits -= 6)
		{
			if (out >= outend)
				break;
			*out++ = ((c >> bits) &0x3F) | 0x80;
		}
		processed = (const unsigned char *) in ;
	}

	*outlen = out - outstart;
	*inlenb = processed - inb;
	return (0);
}

void decodeJSONString (WCSTR src, int length, string& dst, bool fTrim=false) 
{
	char* c = (char*) src;
	dst.reserve (length);
	dst.resize (0);
	char escapeList[] = {'"', 'n', 'r', 't', 'f', 'b', '\\', '/'};
	char replaceList[] = {'"', '\n', '\r', '\t', '\f', '\b', '\\', '/'};
	int maxChars = sizeof (escapeList) / sizeof (char);
	bool fSlash = false;

	if (fTrim) {
		char* c2 = c + length-1;
		int length2 = length;
		while ((*c==' ' || *c=='\t' || *c=='\r' || *c=='\n') && length2>0) {
			c++; length2--;
		}
		while ((*c2==' ' || *c2=='\t' || *c2=='\r' || *c2=='\n') && length2>0) {
			c2--; length2--;
		}
		length = length2;
	}

	while (length>0) {
		
		if ( *c=='\t' || *c=='\r' || *c=='\n') {
			// Ignore
			length--;
			c++;
			continue;
		}

		if (fSlash) {
			int i;
			for (i=0;i<maxChars;i++) {
				if (*c==escapeList[i]) {
					dst+=replaceList[i];
					break;
				}
			}
			if (i==maxChars) {
				// No escaped char found
				if (*c=='u') {
					// Decode UTF16 to UTF8
					unsigned char in[5], out[6];
					in[4]=0;
					int idx = 0;
					unsigned char* c2 = (unsigned char*) c+1;
					while (*c2 && idx<4) {
						in[idx++]=*c2++;
					}
					if (idx==4) {
						sscanf ((const char*) in,   "%04X", in);
						int inlen = 2;
						int outlen = 6;
						UTF16LEToUTF8 (out, &outlen, in, &inlen);			
						dst.append ((const char*) out, outlen);
					}
					length-=idx;
					c+=idx; 	
				}
				else {
					dst+='\\';
					dst+=*c;
				}
			}
			fSlash = false;
		}
		else
		if (*c=='\\')
			fSlash = true;
		else
			dst+=*c;
		c++;
		length--;
	}
}

char *parseJSON(WCSTR jsonStr, ArrayHT &arrayList)
{
	char *c = (char *)jsonStr, *key = NULL, *value = NULL;
	int keyLength = 0, valueLength = 0;
	int flag = 0;

	// Parse
	while (*c != 0) {

		if (flag == 2 || flag == 5) {
			// Overread strings
			while (*c != '"' && *c != 0) {
				if (*c == '\\' && *(c + 1) == '"') {
					c++;
				}
				c++;
			}
			if (!*c)
				break;
		}
		else if (*c == ' ' || *c == '\n' || *c == '\t' || *c == '\r') {
			// Ignore default chars
			c++;
			continue;
		}

		if (*c == '{' || *c == '[') {
			if (value) {
				// Syntax error (value not closed)
				return NULL;
			}
			else if (key && keyLength) {
				if (flag == 3) {
					// New inner array
					string keyStr;
					decodeJSONString(key, keyLength, keyStr, true);
					DataValue emptyArray;
					emptyArray.datatype = DataValue::DATATYPE_ARRAY;
					ArrayHT *newArrayPnt = &arrayList.put(keyStr.c_str(), emptyArray)->m_value.arrayList;
					arrayList.setOwnKeys(true);
					if (!(c = parseJSON(c, *newArrayPnt)))
						return NULL;
				}
				else {
					// Syntax error (key separator : missed)
					return NULL;
				}
			}
			else if (c > jsonStr) {
				// Push new array without key
				DataValue emptyArray;
				emptyArray.datatype = DataValue::DATATYPE_ARRAY;
				ArrayHT *newArrayPnt = &arrayList.push(emptyArray)->m_value.arrayList;
				if (!(c = parseJSON(c, *newArrayPnt)))
					return NULL;
			}
			flag = 1;
			key = value = NULL;
		}
		else if (*c == '}' || *c == ']' || *c == ',') {

			if (flag == 6) {
				valueLength = c - value;
			}

			if (value && valueLength) {

				DataValue data;
				decodeJSONString(value, valueLength, data.value, flag == 6);
				data.datatype = DataValue::DATATYPE_STR;
				if (flag == 6) {
					if (data.value == "null")
						data.datatype = DataValue::DATATYPE_NULL;
					else
						data.setAutoDataType();
				}

				if (key && keyLength) {
					// Key => Value
					string keyStr;
					decodeJSONString(key, keyLength, keyStr, true);
					arrayList.put(keyStr.c_str(), data);
					arrayList.setOwnKeys(true);
				}
				else {
					// Value
					arrayList.push(data);
				}
			}
			else if (key && keyLength) {
				// Value
				DataValue data;
				data.datatype = DataValue::DATATYPE_STR;
				decodeJSONString(key, keyLength, data.value, flag == 6);
				if (flag == 6) {
					if (data.value == "null")
						data.datatype = DataValue::DATATYPE_NULL;
					else
						data.setAutoDataType();
				}
				arrayList.push(data);
			}
			if (*c == ',') {
				key = value = NULL;
				keyLength = valueLength = 0;
				flag = 4;
			}
			else {
				// Finished
				return c;
			}
		}
		else if (*c == ':') {
			if (flag != 8) {
				// Syntaxt error (key not closed)
				return NULL;
			}
			flag = 3;
		}
		else if (*c == '"') {

			if (flag == 1 || flag == 4) {
				// Key start
				key = c + 1;
				flag = 2;
			}
			else if (flag == 2) {
				// Key end
				keyLength = c - key;
				flag = 8;
			}
			else if (flag == 3) {
				// Value string start
				value = c + 1;
				flag = 5;
			}
			else if (flag == 5) {
				// Value string end
				valueLength = c - value;
				flag = 7;
			}
			else {
				// Syntaxt error (value without array)
				return NULL;
			}
		}
		else if (flag == 3 || flag == 4) {
			// Value start
			value = c;
			flag = 6;
		}
		else if (flag == 6) {
			// In value
		}
		c++;
	}
	return c;
}

int func_json_decode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	ret.datatype = DataValue::DATATYPE_ARRAY;
	ret.arrayList.removeAll();
	if (!parseJSON(argvalues[0]->value.c_str(), ret.arrayList)) {
		ctx.warnInterprete("Wrong JSON format.");
		ret = false;
	}
	return 0;
}

int func_count(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (int)argvalues[0]->arrayList.size();
	return 0;
}

int func_array_push(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	if (argvalues.size() < 2) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->arrayList.push(*argvalues[1]);
	ret = (int)argvalues[0]->arrayList.size();
	return 0;
}

int func_array_pop(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	if (!argvalues[0]->arrayList.pop(ret))
		ret = false;
	return 0;
}

int func_array_keys(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	ret.datatype = 10; // Array
	ret.arrayList.removeAll();
	argvalues[0]->arrayList.reset();
	while (argvalues[0]->arrayList.hasMoreElements()) {
		DataValue key = argvalues[0]->arrayList.nextKey();
		ret.arrayList.push(key);
	}
	return 0;
}

int func_array_values(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	ret.datatype = 10; // Array
	ret.arrayList.removeAll();
	argvalues[0]->arrayList.reset();
	while (argvalues[0]->arrayList.hasMoreElements()) {
		ret.arrayList.push(argvalues[0]->arrayList.nextValue());
	}
	return 0;
}

int func_array_key_exists(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype >= DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	ret = argvalues[1]->arrayList.isKey(argvalues[0]->value.c_str());
	return 0;
}

int func_is_array(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (int)argvalues[0]->datatype >= DataValue::DATATYPE_ARRAY && argvalues[0]->datatype < DataValue::DATATYPE_REFERENCE;
	return 0;
}

int func_in_array_universal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx, DataValue *key = NULL)
{

	if (argvalues.size() < 2 || argvalues[1]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	bool fStrict = false;
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_BOOL_REQUIRED;
		fStrict = (bool)*argvalues[2];
	}

	ret = false;
	argvalues[1]->arrayList.reset();
	while (argvalues[1]->arrayList.hasMoreElements()) {

		DataValue *value = argvalues[1]->arrayList.nextValuePnt();
		// Check value
		if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
			if (!fStrict) {
				value->setAutoDataType(true);
				if (*argvalues[0] == *value)
					ret = true;
			}
			else if (argvalues[0]->datatype == value->datatype) {
				if (argvalues[0]->datatype != DataValue::DATATYPE_STR)
					ret = argvalues[0]->numvalue == value->numvalue;
				else
					ret = argvalues[0]->value == value->value;
			}
			if (ret) {
				if (key)
					*key = argvalues[1]->arrayList.nextKey();
				break;
			}
		}
		else if (value->datatype >= DataValue::DATATYPE_ARRAY) {
			// Compare arrays
			DataValue::SArrayCmp cmp;
			memset(&cmp, 0, sizeof(cmp));
			cmp.fStrict = fStrict;
			DataValue::compareArrays(value->arrayList, argvalues[0]->arrayList, cmp);
			ret = cmp.cnt == cmp.cntEqual;
			if (ret) {
				if (key)
					*key = argvalues[1]->arrayList.nextKey();
				break;
			}
		}
	}
	return 0;
}

int func_in_array(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_in_array_universal(argvalues, ret, ctx);
}

int func_array_search(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_in_array_universal(argvalues, ret, ctx, &ret);
}

class WScriptSorter : public WSorter {
public:
	WScriptSorter(bool fAsc = true, int datatype = 0, bool fSwapKey = true, bool fSortKey = false)
	{
		m_fAsc = fAsc;
		m_datatype = datatype;
		m_fSwapKey = fSwapKey;
		m_fSortKey = fSortKey;
	}
	int compare(void *obj1, void *obj2) const
	{

		int ret = 0;

		if (m_fSortKey) {
			if (m_datatype == 1) {
				double v1, v2;
				v1 = atof((WCSTR)(*(WString *)obj1));
				v2 = atof((WCSTR)(*(WString *)obj2));

				ret = (v1 > v2) ? -1 : (v1 < v2 ? 1 : 0);
			}
			else {
				ret = -strcmp((WCSTR)(*(WString *)obj1), (WCSTR)(*(WString *)obj2));
			}
		}
		else {
			if (m_datatype == 0) {
				if (((((DataValue *)obj1)->datatype == DataValue::DATATYPE_STR) && (((DataValue *)obj2)->datatype == DataValue::DATATYPE_STR)) || (((DataValue *)obj1)->datatype != ((DataValue *)obj2)->datatype))
					ret = -strcmp(((DataValue *)obj1)->value.c_str(), ((DataValue *)obj2)->value.c_str());
				else
					ret = (((DataValue *)obj1)->numvalue > ((DataValue *)obj2)->numvalue) ? -1 : ((((DataValue *)obj1)->numvalue < ((DataValue *)obj2)->numvalue) ? 1 : 0);
			}
			else if (m_datatype == 1)
				ret = (((DataValue *)obj1)->numvalue > ((DataValue *)obj2)->numvalue) ? -1 : ((((DataValue *)obj1)->numvalue < ((DataValue *)obj2)->numvalue) ? 1 : 0);
			else
				ret = -strcmp(((DataValue *)obj1)->value.c_str(), ((DataValue *)obj2)->value.c_str());
		}

		return m_fAsc ? ret : -ret;
	}

	bool isSetAutoKeys() const
	{
		return m_fSwapKey;
	}

	bool isSortByKey() const
	{
		return m_fSortKey;
	}

private:
	bool m_fAsc, m_fSwapKey, m_fSortKey;
	int m_datatype;
};

class WScriptSorterByFunc : public WSorter {
public:
	WScriptSorterByFunc(bool fAsc, bool fSwapKey, bool fSortKey, string &func, InterpreterContext &ctx)
	{
		m_fAsc = fAsc;
		m_fSwapKey = fSwapKey;
		m_fSortKey = fSortKey;
		m_func = &func;
		m_ctx = &ctx;
	}

	int compare(void *obj1, void *obj2) const
	{
		vector<DataValue *> argvalues;
		argvalues.push_back((DataValue *)obj1);
		argvalues.push_back((DataValue *)obj2);
		DataValue ret;
		m_ctx->_callUserFunc(*m_func, argvalues, ret, *m_ctx, 0);
		return m_fAsc ? (int)ret : -(int)ret;
	}

	bool isSetAutoKeys() const
	{
		return m_fSwapKey;
	}

	bool isSortByKey() const
	{
		return m_fSortKey;
	}

private:
	bool m_fAsc, m_fSwapKey, m_fSortKey;
	string *m_func;
	InterpreterContext *m_ctx;
};

int func_sort_universal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx, bool fAsc = true, bool fSetAutoKeys = true, bool fSortByKey = false)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	int datatype = 0;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		datatype = (int)*argvalues[1];
	}
	WScriptSorter sorter(fAsc, datatype, fSetAutoKeys, fSortByKey);
	if (argvalues[0]->refPnt) {
		argvalues[0]->refPnt->arrayList.sort(sorter);
		ret = true;
	}
	else {
		ret = false;
	}

	return 0;
}

int func_sort_by_func_universal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx, bool fAsc = true, bool fSetAutoKeys = true, bool fSortByKey = false)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR)
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	argvalues[1]->toString();
	WScriptSorterByFunc sorter(fAsc, fSetAutoKeys, fSortByKey, argvalues[1]->value, ctx);
	if (argvalues[0]->refPnt) {
		argvalues[0]->refPnt->arrayList.sort(sorter);
		ret = true;
	}
	else {
		ret = false;
	}

	return 0;
}

int func_sort(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_sort_universal(argvalues, ret, ctx, true, true);
}

int func_rsort(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_sort_universal(argvalues, ret, ctx, false, true);
}

int func_asort(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_sort_universal(argvalues, ret, ctx, true, false);
}

int func_arsort(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_sort_universal(argvalues, ret, ctx, false, false);
}

int func_ksort(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_sort_universal(argvalues, ret, ctx, true, false, true);
}

int func_krsort(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_sort_universal(argvalues, ret, ctx, false, false, true);
}

int func_usort(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_sort_by_func_universal(argvalues, ret, ctx, true, true);
}

int func_uasort(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	return func_sort_by_func_universal(argvalues, ret, ctx, true, false);
}

int func_array_merge(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	W32 arrayIdx = 0;
	ret.arrayList.removeAll();
	for (W32 i = 0; i < argvalues.size(); i++) {
		if (argvalues[i]->datatype < DataValue::DATATYPE_ARRAY) {
			return ((i + 1) << 8) | WSCRIPT_RET_ARRAY_REQUIRED;
		}
		ArrayHT *arrayList = &argvalues[i]->arrayList;
		arrayList->reset();
		while (arrayList->hasMoreElements()) {
			WString key = arrayList->nextKey();
			// Check alphanum
			char *c = (char *)key.getBuf();
			bool fAlphaNum = false;
			while (*c != 0) {
				if (*c < '0' || *c > '9') {
					fAlphaNum = true;
					break;
				}
				c++;
			}
			if (fAlphaNum) {
				ret.arrayList.put(key, arrayList->nextValue());
				ret.arrayList.setOwnKeys(true);
			}
			else {
				ret.arrayList.put(WFormattedString("%u", arrayIdx++), arrayList->nextValue());
			}
		}
	}
	ret.datatype = DataValue::DATATYPE_ARRAY;
	return 0;
}

int func_array_splice(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}

	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	int offset = (int)*argvalues[1];
	int length = argvalues[0]->arrayList.size();
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
		length = (int)*argvalues[2];
	}

	ret.datatype = 10; // Array
	ret.arrayList.removeAll();

	ArrayHT newArrayList;

	argvalues[0]->refPnt->arrayList.reset();
	if (offset < 0) {
		offset = argvalues[0]->refPnt->arrayList.size() + offset;
	}
	int pos = 0;
	bool hasMoreElements = true;
	while (offset > 0 && (hasMoreElements = argvalues[0]->refPnt->arrayList.hasMoreElements())) {
		DataValue *valuePnt = argvalues[0]->refPnt->arrayList.nextValuePnt();
		newArrayList.push(*valuePnt);
		offset--;
		pos++;
	}
	if (length < 0) {
		length = argvalues[0]->refPnt->arrayList.size() - pos + length;
	}
	if (hasMoreElements) {
		while (length > 0 && (hasMoreElements = argvalues[0]->refPnt->arrayList.hasMoreElements())) {
			DataValue *valuePnt = argvalues[0]->refPnt->arrayList.nextValuePnt();
			ret.arrayList.push(*valuePnt);
			length--;
		}
	}

	if (argvalues.size() > 3) {
		// Replace ...
		if (argvalues[3]->datatype >= DataValue::DATATYPE_ARRAY) {
			argvalues[3]->arrayList.reset();
			while (argvalues[3]->arrayList.hasMoreElements()) {
				newArrayList.push(*argvalues[3]->arrayList.nextValuePnt());
			}
		}
		else {
			newArrayList.push(*argvalues[3]);
		}
	}
	if (hasMoreElements) {
		while (argvalues[0]->refPnt->arrayList.hasMoreElements()) {
			newArrayList.push(*argvalues[0]->refPnt->arrayList.nextValuePnt());
		}
	}
	argvalues[0]->refPnt->arrayList = newArrayList;

	return 0;
}

int func_array_slice(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}

	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	int offset = (int)*argvalues[1];
	int length = argvalues[0]->arrayList.size();
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
		length = (int)*argvalues[2];
	}

	ret.datatype = 10; // Array
	ret.arrayList.removeAll();

	argvalues[0]->arrayList.reset();
	if (offset < 0) {
		offset = argvalues[0]->arrayList.size() + offset;
	}
	int pos = 0;
	while (offset > 0 && argvalues[0]->arrayList.hasMoreElements()) {
		argvalues[0]->arrayList.nextValuePnt();
		offset--;
		pos++;
	}
	if (length < 0) {
		length = argvalues[0]->arrayList.size() - pos + length;
	}
	while (length > 0 && argvalues[0]->arrayList.hasMoreElements()) {
		DataValue *valuePnt = argvalues[0]->arrayList.nextValuePnt();
		ret.arrayList.push(*valuePnt);
		length--;
	}

	return 0;
}

int func_array_reverse(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}
	bool fPreserveKeys = false;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_BOOL_REQUIRED;
		fPreserveKeys = (bool)*argvalues[1];
	}

	ret.datatype = 10; // Array
	ret.arrayList.removeAll();
	if (fPreserveKeys)
		ret.arrayList.setOwnKeys(true);

	argvalues[0]->arrayList.reset();
	argvalues[0]->arrayList.setLast();
	while (argvalues[0]->arrayList.hasPrevElements()) {
		if (fPreserveKeys)
			ret.arrayList.put(argvalues[0]->arrayList.nextKey(), argvalues[0]->arrayList.prevValue());
		else
			ret.arrayList.push(argvalues[0]->arrayList.prevValue());
	}
	return 0;
}

int func_array_flip(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype < DataValue::DATATYPE_ARRAY) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_ARRAY_REQUIRED;
	}

	ret.datatype = 10; // Array
	ret.arrayList.removeAll();
	ret.arrayList.setOwnKeys(true);
	argvalues[0]->arrayList.reset();
	while (argvalues[0]->arrayList.hasMoreElements()) {
		ArrayHT::WHash *hash;
		DataValue *v = argvalues[0]->arrayList.nextValuePnt(&hash);
		if (v->datatype > DataValue::DATATYPE_STR || v->datatype < DataValue::DATATYPE_NUM) {
			ctx.warnInterprete("Can only flip string and number values!");
		}
		else {
			v->toString();

			DataValue tmp = argvalues[0]->arrayList.nextKey();
			ret.arrayList.put(v->value.c_str(), tmp);
		}
	}
	return 0;
}

int func_show_warnings(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues.size() < 1 || PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_BOOL_REQUIRED;
	}
	ret = true;
	ctx.fShowWarnings = (bool)*argvalues[0];
	return 0;
}

int func_autonl(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (argvalues.size() < 1 || PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_BOOL_REQUIRED;
	}
	ctx.fAutoNL = (bool)*argvalues[0];
	ret = true;
	return 0;
}

int func_getenv(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	WCSTR var = getenv(argvalues[0]->value.c_str());
	if (!var) {
		ret = false;
	}
	else {
		ret.datatype = DataValue::DATATYPE_STR;
		ret.value.assign(var);
	}
	return 0;
}

int func_putenv(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[1]->toString();
	putenv((char *)(WCSTR)WFormattedString("%s=%s", (WCSTR)argvalues[0]->value.c_str(), (WCSTR)argvalues[1]->value.c_str()));
	ret = true;
	return 0;
}

/*
HKEY getRegKeyByName (WCSTR name) {

    if (strstr (name, "HKEY_CLASSES_ROOT")==name)
        return HKEY_CLASSES_ROOT;
    else
    if (strstr (name, "HKEY_CURRENT_CONFIG")==name)
        return HKEY_CURRENT_CONFIG;
    else
    if (strstr (name, "HKEY_CURRENT_USER")==name)
        return HKEY_CURRENT_USER;
    else
    if (strstr (name, "HKEY_LOCAL_MACHINE")==name)
        return HKEY_LOCAL_MACHINE;
    else
    if (strstr (name, "HKEY_PERFORMANCE_DATA")==name)
        return HKEY_PERFORMANCE_DATA;
    else
    if (strstr (name, "HKEY_PERFORMANCE_NLSTEXT")==name)
        return HKEY_PERFORMANCE_NLSTEXT;
    else
    if (strstr (name, "HKEY_PERFORMANCE_TEXT")==name)
        return HKEY_PERFORMANCE_TEXT;
    else
    if (strstr (name, "HKEY_PERFORMANCE_TEXT")==name)
        return HKEY_USERS;
    return NULL;
}
*/

int func_putreg(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	/*
	if (PARAM1_NOSTR) {
	    return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
	    return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	}
	bool fValue=false;
	if (argvalues.size ()>2) {
	    if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
	        return WSCRIPT_RET_PARAM3|WSCRIPT_RET_STR_REQUIRED;
	    argvalues[2]->toString ();
	    fValue = true;
	}

	ret = false;

	argvalues[0]->toString ();
	argvalues[1]->toString ();


	HKEY hRegKeyCurrUser = NULL, hKey=NULL;
	WCSTR keyName = argvalues[0]->value.c_str ();
	WCSTR tmp = NULL;
	hRegKeyCurrUser = getRegKeyByName (keyName);
	if (!hRegKeyCurrUser) {
	    RegOpenCurrentUser (KEY_ALL_ACCESS, &hRegKeyCurrUser);
	}
	else
	if (tmp = strstr (keyName, "\\")) {
	    keyName = tmp+1;
	}
	else
	{
	    hKey = hRegKeyCurrUser;
	}

	if (!hRegKeyCurrUser) {
	    return 0;
	}
	if (!fValue) {
	    // Create sub key
	    W32 disposion;
	    string newKey = string (keyName)+"\\"+argvalues[1]->value;
	    if (ERROR_SUCCESS==RegCreateKeyEx (hRegKeyCurrUser, newKey.c_str (), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &disposion)) {
	        ret = true;
	    }

	}
	else {

	    // Create key and value
	    W32 datatype = REG_SZ;
	    LPBYTE valuePnt = (LPBYTE) argvalues[2]->value.c_str ();
	    W32 valueLength = argvalues[2]->value.length ();
	    W32 tmp = 0;
	    unsigned __int64 tmp2 = 0;

	    if (argvalues.size ()>3) {
	        if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
	            return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;

	        if (argvalues[3]->numvalue==1) {
	            datatype=REG_DWORD;
	            tmp = (W32) argvalues[2]->numvalue;
	            valuePnt = (LPBYTE)  &tmp;
	            valueLength = sizeof (tmp);
	        }
	        else
	        if (argvalues[3]->numvalue==2) {
	            datatype=REG_QWORD;
	            tmp2 = (unsigned __int64) argvalues[2]->numvalue;
	            valuePnt = (LPBYTE)  &tmp2;
	            valueLength = sizeof (tmp2);
	        }
	        else
	        if (argvalues[3]->numvalue==3) {
	            datatype=REG_BINARY;
	            valueLength = argvalues[2]->value.size ();
	        }
	    }
	    if (!hKey)
	        RegOpenKeyEx (hRegKeyCurrUser, keyName, 0, KEY_ALL_ACCESS, &hKey);
	    if (hKey!=NULL && ERROR_SUCCESS==RegSetValueEx (hKey,argvalues[1]->value.c_str (),0, datatype, valuePnt, valueLength)) {
	        ret = true;
	    }
	}


	if (hKey!=NULL) {
	    RegCloseKey (hKey);
	    hKey = NULL;
	}
	if (hRegKeyCurrUser!=NULL) {
	    RegCloseKey (hRegKeyCurrUser);
	    hRegKeyCurrUser = NULL;
	}
	*/
	ctx.warnInterprete("putreg is not available on Linux.");
	ret = false;
	return 0;
}

int func_getreg(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	/*
	if (PARAM1_NOSTR) {
	    return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
	}
	bool fValue = false;
	if (argvalues.size ()>1) {
	    fValue = true;
	    if (argvalues[1]->datatype>DataValue::DATATYPE_STR)
	        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	    argvalues[1]->toString ();
	}
	argvalues[0]->toString ();


	ret = false;
	HKEY hRegKeyCurrUser = NULL, hKey=NULL;
	WCSTR keyName = argvalues[0]->value.c_str ();
	WCSTR tmp = NULL;
	hRegKeyCurrUser = getRegKeyByName (keyName);
	if (!hRegKeyCurrUser) {
	    RegOpenCurrentUser (KEY_READ|KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS, &hRegKeyCurrUser);
	    if (keyName[0]==0)
	        hKey = hRegKeyCurrUser;
	}
	else
	if (tmp = strstr (keyName, "\\")) {
	    keyName = tmp+1;
	}
	else {
	    hKey = hRegKeyCurrUser;
	}

	if (!hRegKeyCurrUser) {
	    return 0;
	}
	if (!hKey)
	    RegOpenKeyEx (hRegKeyCurrUser, keyName, 0, KEY_READ|KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS, &hKey);
	char buf[4097];
	buf[sizeof(buf)-1]=0;
	W32 len = sizeof (buf)-1;
	W32 datatype = 0;
	if (fValue) {
	    // Get value
	    if (hKey!=NULL && ERROR_SUCCESS==RegQueryValueEx(hKey, argvalues[1]->value.c_str (), NULL, &datatype, (LPBYTE) buf,  &len)) {
	        switch (datatype) {
	            case REG_DWORD:
	            case REG_DWORD_BIG_ENDIAN:
	                ret = (double) *(W32*) buf;
	                break;
	            case REG_QWORD:
	                ret = (double) *(unsigned __int64*) buf;
	                break;
	            case REG_BINARY:
	                ret.datatype=DataValue::DATATYPE_STR;
	                ret.value.assign (buf, len);
	                break;
	            default:
	                ret.datatype=DataValue::DATATYPE_STR;
	                ret.value=buf;
	                break;
	        }
	    }
	}
	else
	if (hKey)
	{
	    // Iterarte all sub keys
	    ret.datatype=DataValue::DATATYPE_ARRAY;
	    ArrayHT* list = NULL;
	    DataValue tmp;
	    tmp.datatype = DataValue::DATATYPE_ARRAY;
	    list = &ret.arrayList.put ("SUBKEYS", tmp)->m_value.arrayList;

	    W32 index = 0;
	    LONG retCode = 0;
	    tmp.datatype = DataValue::DATATYPE_STR;
	    do {
	        len = sizeof (buf)-1;

	        retCode = RegEnumKeyEx(hKey, index, buf, &len, NULL, NULL, NULL, NULL);
	        if (retCode==ERROR_SUCCESS) {
	            tmp.value.assign (buf, len);
	            list->push (tmp);
	            index++;
	        }
	        else {
	            break;
	        }
	    } while (true);

	    // Iterate all values
	    tmp.datatype = DataValue::DATATYPE_ARRAY;
	    list = &ret.arrayList.put ("VALUES", tmp)->m_value.arrayList;

	    index = 0;
	    tmp.datatype = DataValue::DATATYPE_STR;
	    char buf2[256];
	    buf2[sizeof(buf2)-1]=0;
	    do {
	        len = sizeof (buf)-1;
	        W32 len2 = sizeof (buf2)-1;

	        retCode = RegEnumValue(hKey, index, buf2, &len2, NULL, &datatype, (LPBYTE) buf, &len);

	        if (retCode==ERROR_SUCCESS) {
	            switch (datatype) {
	                case REG_DWORD:
	                case REG_DWORD_BIG_ENDIAN:
	                    tmp= (double) *(W32*) buf;
	                    break;
	                case REG_QWORD:
	                    tmp= (double) *(unsigned __int64*) buf;
	                    break;
	                case REG_BINARY:
	                    tmp.datatype=DataValue::DATATYPE_STR;
	                    tmp.value.assign (buf, len);
	                    break;
	                default:
	                    tmp.datatype=DataValue::DATATYPE_STR;
	                    tmp.value = buf;
	                    break;
	            }
	            list->put (buf2, tmp);
	            index++;
	        }
	        else {
	            break;
	        }
	    } while (true);

	}
	if (hKey!=NULL) {
	    RegCloseKey (hKey);
	    hKey = NULL;
	}
	if (hRegKeyCurrUser!=NULL) {
	    RegCloseKey (hRegKeyCurrUser);
	    hRegKeyCurrUser = NULL;
	}
	*/
	ctx.warnInterprete("getreg is not available on Linux.");
	ret = false;
	return 0;
}

int func_delreg(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	/*
	if (PARAM1_NOSTR) {
	    return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
	    return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	}
	int mode = 0;
	if (argvalues.size ()>2) {
	    if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
	        return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
	    mode = (int) argvalues[2]->numvalue;
	}

	argvalues[0]->toString ();
	argvalues[1]->toString ();

	ret = false;
	HKEY hRegKeyCurrUser = NULL, hKey=NULL;
	WCSTR keyName = argvalues[0]->value.c_str ();
	WCSTR tmp = NULL;
	hRegKeyCurrUser = getRegKeyByName (keyName);
	if (!hRegKeyCurrUser) {
	    RegOpenCurrentUser (KEY_ALL_ACCESS, &hRegKeyCurrUser);
	}
	else
	if (tmp = strstr (keyName, "\\")) {
	    keyName = tmp+1;
	}
	else {
	    hKey = hRegKeyCurrUser;
	}
	if (!hRegKeyCurrUser) {
	    return 0;
	}
	if (!hKey)
	    RegOpenKeyEx (hRegKeyCurrUser, keyName, 0, KEY_ALL_ACCESS, &hKey);

	if (hKey!=NULL) {
	    if (mode==0 || mode==2) // Delete key and value
	        RegDeleteValue (hKey, argvalues[1]->value.c_str ());
	    if (mode==1 || mode==2) // Delete sub key and all values
	        RegDeleteKey (hKey, argvalues[1]->value.c_str ());
	#ifdef _NDEBUG
	    if (mode==4) // Delete sub key and all values and sub keys recursive
	        RegDeleteTree (hKey, argvalues[1]->value.c_str ());
	#endif
	    ret = true;
	}

	if (hKey!=NULL) {
	    RegCloseKey (hKey);
	    hKey = NULL;
	}
	if (hRegKeyCurrUser!=NULL) {
	    RegCloseKey (hRegKeyCurrUser);
	    hRegKeyCurrUser = NULL;
	}
	*/
	ctx.warnInterprete("delreg is not available on Linux.");
	ret = false;
	return 0;
}

int func_header(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	const char *header = argvalues[0]->value.c_str();
	const char *value = strstr(header, ":");
	if (ctx.fHeadersSent) {
		ret = false;
		ctx.warnInterprete("header() must set before first output.");
	}
	if (value) {
		ret = true;

		WString key;
		key.assign(header, value - header);
		key.trim();
		key.toUpperCase();

		WString *exists = NULL;
		if (key.equals("SET-COOKIE") && (exists = ctx.headerHT.getPnt("SET-COOKIE")) != NULL) {
			// Can have multiple Set-Cookie headers
			exists->append("\n");
			exists->append(header);
		}
		else {
			ctx.headerHT.put(key, header);
		}
	}
	return 0;
}

int func_move_uploaded_file(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	argvalues[1]->toString();

	WString tmpDir;
	WSystem::getTempDirectory(tmpDir);
	WString tmpFilename = WFormattedString("%s%s", (WCSTR)tmpDir, (WCSTR)argvalues[0]->value.c_str());

	ret = WSystem::renameFile(tmpFilename, (WCSTR)argvalues[1]->value.c_str());
	if (!ret)
		ctx.warnInterprete(WFormattedString("Cannot move uploaded file %s to %s", (WCSTR)tmpFilename, (WCSTR)argvalues[1]->value.c_str()));
	return 0;
}

int func_is_uploaded_file(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	WString tmpDir;
	WSystem::getTempDirectory(tmpDir);
	WString tmpFilename = WFormattedString("%s%s", (WCSTR)tmpDir, (WCSTR)argvalues[0]->value.c_str());
	ret = WSystem::existsFile(tmpFilename);
	return 0;
}

int func_url_decode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	W8 v = 0, v2 = 0;

	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.reserve(255);
	ret.value.resize(0);
	char *c = (char *)argvalues[0]->value.c_str();
	while (*c != 0) {
		if (*c == '%') {
			// CGI-Decoding
			c++;
			if (*c == 0)
				break;
			v = 0;
			v2 = *c++;
			if (!CGIUtils::char2Hex(v2)) {
				// "Wrong URI encoding"
			}
			v |= (v2 << 4);
			if (*c == 0)
				break;
			v2 = *c++;
			if (!CGIUtils::char2Hex(v2)) {
				// "Wrong URI encoding"
			}
			v |= v2;

			ret.value.append((const char *)&v, 1);

			if (*c == 0)
				break;
			continue;
		}
		else if (*c == '+') {
			ret.value.append(" ");
		}
		else {
			ret.value.append(c, 1);
		}
		c++;
	}
	return 0;
}

int func_url_encode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.reserve(255);
	ret.value.resize(0);
	char *c = (char *)argvalues[0]->value.c_str();
	char tmp[4];
	tmp[3] = 0;
	while (*c != 0) {
		if (*c == 45 ||                // hyphen
		        (*c >= 48 && *c <= 57) ||  // 0-9
		        (*c >= 65 && *c <= 90) ||  // A-Z
		        *c == 95 ||                // underscore
		        (*c >= 97 && *c <= 122) || // a-z
		        *c == 126                  // tilde
		   ) {
			ret.value.append(c, 1);
		}
		else {

			snprintf(tmp, 3, "%%%2X", *c & 0xFF);
			ret.value.append((const char *)tmp);
		}
		c++;
	}
	return 0;
}

int func_htmlspecialchars(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.reserve(255);
	ret.value.resize(0);
	char *c = (char *)argvalues[0]->value.c_str();

	while (*c != 0) {
		if (*c == '&') {
			ret.value.append("&amp;");
		}
		else if (*c == '<') {
			ret.value.append("&lt;");
		}
		else if (*c == '>') {
			ret.value.append("&gt;");
		}
		else if (*c == '"') {
			ret.value.append("&quot;");
		}
		else {
			ret.value.append(c, 1);
		}
		c++;
	}
	return 0;
}

int func_nl2br(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();

	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.resize(0);
	ret.value.reserve(255);
	char *c = (char *)argvalues[0]->value.c_str();

	while (*c != 0) {
		if (*c == '\n') {
			ret.value.append("<br>");
		}
		else {
			ret.value.append(c, 1);
		}
		c++;
	}
	return 0;
}

int callUserFunc(const string &operation, vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx, int argOffs = 0)
{

	int line = ctx.line;
	// User functions
	ExpressionList *funcList;
	if (funcList = ctx.functions.getPnt(operation.c_str())) {
		if (ctx.funcDeep + 1 >= ctx.symbolTblCnt) {
			/*
			ctx.abortInterprete (WFormattedString ("Stack Overflow. Maximal function recursion depth %u reached.", ctx.funcDeep+1));
			return 0;
			*/
			// Add 100 new symbol tables
			ctx.symbolTblCnt += 100;
			ctx.symbols = (SymbolTable **)realloc(ctx.symbols, (ctx.symbolTblCnt * sizeof(SymbolTable *)));
			for (unsigned int i = 0; i < 100; i++)
				ctx.symbols[ctx.funcDeep + i + 1] = new SymbolTable();
		}

		unsigned int count = 0; // position in the argvalue list

		// place the formal parameters in symbol table with their values
		ExpressionList *list = &(*funcList)[0].arguments;
		ctx.symbols[ctx.funcDeep + 1]->removeAll(); // Remove all old symbols

		for (unsigned int i = 0; i < list->size(); count++, i++) {
			Expression *e = &(*list)[i];
			const char *key = e->operation.c_str();
			if (e->constvalue != NULL) {
				// Set default parameter
				key = e->constvalue->c_str();
				ctx.funcDeep++;
				DataValue datavalue;
				DataValue *tmp = &ctx.symbols[ctx.funcDeep]->put(key, datavalue)->m_value;
				e->calc(ctx, *tmp, EXPRESSION_FLAG_STORE);
				ctx.funcDeep--;
			}

			if (argvalues.size() < (count + 1 + argOffs) || argvalues[count + argOffs]->datatype == DataValue::DATATYPE_NO_PARAM) {
				// No parameter given
				if (e->constvalue == NULL) {
					// No default parameter
					unsigned int *line2 = ctx.locationHT.getPnt(operation.c_str());
					if (!line2)
						ctx.abortInterprete(WFormattedString("%s() expects %u %s", operation.c_str(), list->size(), list->size() == 1 ? "parameter" : "parameters"));
					else
						ctx.abortInterprete(WFormattedString("%s() expects %u %s, see declarion in %s (%u)", operation.c_str(), list->size(), list->size() == 1 ? "parameter" : "parameters", ctx.getFilenameByIdx(WSCRIPT_FILE(*line2)), WSCRIPT_LINE(*line2)));
					return 0;
				}
			}
			else {
				DataValue *argValue = argvalues[count + argOffs];
				if (e->datatype == DataValue::DATATYPE_REFERENCE && !(argValue->flags & (DataValue::FLAGS_NOWRITE | DataValue::FLAGS_DUPARG))) {
					// Copy only reference
					DataValue datavalue;
					DataValue *newDataValue = &ctx.symbols[ctx.funcDeep + 1]->put(key, datavalue)->m_value;
					newDataValue->refPnt = argValue->refPnt;
					newDataValue->datatype = argValue->datatype;
					newDataValue->flags = argValue->flags | DataValue::FLAGS_USEREF;
				}
				else {
					//Copy complete variable to sstack
					DataValue *newDataValue = &ctx.symbols[ctx.funcDeep + 1]->put(key, *argValue)->m_value;
					newDataValue->flags &= (~(DataValue::FLAGS_USEREF | DataValue::FLAGS_DUPARG));
					newDataValue->refPnt = newDataValue;
				}
			}
		}
		ctx.funcDeep++;

		// Execute the function i.e. each expression in the function
		ExpressionList *funcExpr = &(*funcList)[1].arguments;

		for (unsigned int i = 0; i < funcExpr->size(); i++) {
			(*funcExpr)[i].calc(ctx, ret);

			if (ctx.flag == InterpreterContext::FLAG_RETURN) {
				ctx.flag = 0;
				ctx.funcDeep--;
				return 0;
			}
		}
		ctx.funcDeep--;
	}
	else {
		ctx.line = line; // Refresh the line
		ctx.abortInterprete(WFormattedString("Function %s() is not declared", operation.c_str()));
	}
	return 0;
}

void WScriptThread::run()
{
	try {
		DataValue ret;
		// We are in thread scope, init thread variables
		this->ctx->loopDeep = this->loopDeep;
		this->ctx->switchDeep = this->switchDeep;
		this->ctx->funcDeep = 1;
		this->ctx->flag = this->flag;
		this->ctx->line = this->line;
		this->ctx->currThread = this;
		this->ctx->symbolTblCnt = 100;
		this->ctx->symbols = (SymbolTable **)malloc(this->ctx->symbolTblCnt * sizeof(SymbolTable *));
		for (unsigned int i = 0; i < this->ctx->symbolTblCnt; i++)
			this->ctx->symbols[i] = new SymbolTable();

		this->threadSymbols = this->ctx->symbols;
		// Init global variable as reference pointers
		for (int i = 0; i < 2; i++) {
			WMutexLocker locker(this->ctx->mutex);
			while (this->parentSymbols[i]->hasMoreElements()) {
				const char *key = this->parentSymbols[i]->nextKey();
				DataValue v, *vOld = this->parentSymbols[i]->nextValuePnt();
				if (vOld->refPnt)
					v.refPnt = vOld->refPnt;
				else
					v.refPnt = vOld;
				v.flags = v.refPnt->flags | DataValue::FLAGS_USEREF;
				v.datatype = v.refPnt->datatype;
				this->ctx->symbols[i]->put(key, v);
			}
		}

		// Set argvalues to duplicate
		for (unsigned int i = 0; i < this->argvalues.size(); i++)
			this->argvalues[i]->flags |= DataValue::FLAGS_DUPARG;

		// Signal calling thread that we are ready
		semaphore.signal();

		callUserFunc(this->argvalues[0]->value, this->argvalues, ret, *this->ctx, 1);

		// Delete thread symbol table
		// WMutexLocker locker (this->ctx->mutex);
		for (unsigned int i = 0; i < this->ctx->symbolTblCnt; i++) {
			if (this->ctx->symbols[i])
				delete this->ctx->symbols[i];
		}
		free(this->ctx->symbols);
		this->ctx->symbols = NULL;
		this->threadSymbols = NULL;
	}
	catch (...) {
	}
}

#define CALL_TYPE_VOID 0
#define CALL_TYPE_BOOL 1
#define CALL_TYPE_INT 2
#define CALL_TYPE_FLOAT 3
#define CALL_TYPE_DOUBLE 4
#define CALL_TYPE_POINTER 5
#define CALL_TYPE_STRING 6
#define CALL_TYPE_REFERENCE 7
#define CALL_TYPE_INT_ARRAY 20
/*
double testfunc (int a, int b, double c, WCSTR d, float e) {

    printf ("%i, %i, %g, %s, %f\r\n", a, b, c, d, e);
    double z = 1;
    double t = 3*z;
    return  172;
}
*/

#ifdef _WITH_DYNCALL
#include <dyncall.h>
#endif

int func_universal_call(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx, void *func, const int *paramDef, unsigned int paramCnt = 0, int retDef = 0)
{
	// func = (void*) testfunc;
	// ctx.warnInterprete ("Native function calling not implemented on this architecture.");
	ret = false;
	int arrayParamIdx = -1;
	int x2 = 0;
	for (unsigned int i = 3; i < (paramCnt + 3); i++) {
		x2++;
		int type = paramDef[i - 3];
		if (type == CALL_TYPE_STRING || type == CALL_TYPE_REFERENCE) {
			if (argvalues.size() < (i + 1) || argvalues[i]->datatype > DataValue::DATATYPE_STR || argvalues[i]->datatype == DataValue::DATATYPE_NO_PARAM) {
				return ((i + 1) << 8) | WSCRIPT_RET_STR_REQUIRED;
			}
			if (type == CALL_TYPE_REFERENCE && argvalues[i]->refPnt == NULL) {
				ctx.warnInterprete("No reference pointer found.");
				return ((i + 1) << 8) | WSCRIPT_RET_STR_REQUIRED;
			}
			argvalues[i]->toString();
		}
		else if ((type >= CALL_TYPE_BOOL) && (type <= CALL_TYPE_POINTER)) {
			if (argvalues.size() < (i + 1) || argvalues[i]->datatype > DataValue::DATATYPE_STR || argvalues[i]->datatype == DataValue::DATATYPE_NO_PARAM) {
				return ((i + 1) << 8) | WSCRIPT_RET_NUM_REQUIRED;
			}
		}
		else if (argvalues.size() < (i + 1) || argvalues[i]->datatype < DataValue::DATATYPE_ARRAY) {
			return ((i + 1) << 8) | WSCRIPT_RET_ARRAY_REQUIRED;
		}
		else {
			arrayParamIdx = i;
			if (type == CALL_TYPE_INT_ARRAY)
				x2++;
		}
	}
#ifndef _WITH_DYNCALL
	void *v;
	double v2;
#else
	DCCallVM *vm = dcNewCallVM(4096);
	dcMode(vm, DC_CALL_C_DEFAULT);
	dcReset(vm);
#endif
	int *array1 = NULL;
	unsigned int arraySize = 0;
	if (arrayParamIdx != -1) {

		if (paramDef[arrayParamIdx] == CALL_TYPE_INT_ARRAY) {
			arraySize = argvalues[arrayParamIdx]->arrayList.size();
		}

		if (arraySize) {
			array1 = new int[arraySize];
			memset(array1, 0, sizeof(int) * arraySize);
			unsigned int i = 0;
			argvalues[arrayParamIdx]->arrayList.reset();
			while (argvalues[arrayParamIdx]->arrayList.hasMoreElements() && i < arraySize) {
				array1[i++] = (int)argvalues[arrayParamIdx]->arrayList.nextValue();
			}
		}
	}
#ifndef _WITH_DYNCALL
	void *voidList[20 + 1];
	memset(voidList, 0, sizeof(voidList));
	int mmxReg = 0;
	int x3 asm("r7") = x2;
#endif
	x2 = 0;
	for (int x = 0; x < paramCnt; x++ && x2 < 20) {

		int type = paramDef[x];
		if (type == CALL_TYPE_REFERENCE) {
#ifndef _WITH_DYNCALL
			v = (void *)argvalues[x + 3]->refPnt->value.c_str();
			voidList[x2++] = v;
#else
			dcArgPointer(vm, (DCpointer)argvalues[x + 3]->refPnt->value.c_str());
#endif
		}
		else if (type == CALL_TYPE_POINTER) {
#ifndef _WITH_DYNCALL
			v = (void *)(uint64_t)argvalues[x + 3]->numvalue;
			voidList[x2++] = v;
#else
			dcArgPointer(vm, (DCpointer)(unsigned int)argvalues[x + 3]->numvalue);
#endif
		}
		else if (type == CALL_TYPE_INT) {
#ifndef _WITH_DYNCALL
			v = (void *)(int)argvalues[x + 3]->numvalue;
			voidList[x2++] = v;
#else
			dcArgInt(vm, (DCint)(int)argvalues[x + 3]->numvalue);
#endif
		}
		else if (type == CALL_TYPE_FLOAT) {
#ifndef _WITH_DYNCALL
			switch (mmxReg) {
				case 0:
					__asm__ __volatile__("movsd %0,%%xmm0;cvtpd2ps %%xmm0, %%xmm0" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 1:
					__asm__ __volatile__("movsd %0,%%xmm1;cvtpd2ps %%xmm1, %%xmm1" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 2:
					__asm__ __volatile__("movsd %0,%%xmm2;cvtpd2ps %%xmm2, %%xmm2" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 3:
					__asm__ __volatile__("movsd %0,%%xmm3;cvtpd2ps %%xmm3, %%xmm3" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 4:
					__asm__ __volatile__("movsd %0,%%xmm4;cvtpd2ps %%xmm4, %%xmm4" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 5:
					__asm__ __volatile__("movsd %0,%%xmm5;cvtpd2ps %%xmm5, %%xmm5" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 6:
					__asm__ __volatile__("movsd %0,%%xmm6;cvtpd2ps %%xmm6, %%xmm6" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 7:
					__asm__ __volatile__("movsd %0,%%xmm7;cvtpd2ps %%xmm7, %%xmm7" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				default:
					ctx.abortInterprete("Too many floating point parameters (8 allowed)");
					return 0;
					break;
			}
			mmxReg++;
#else
			dcArgFloat(vm, (DCfloat)argvalues[x + 3]->numvalue);
#endif
		}
		else if (type == CALL_TYPE_DOUBLE) {
#ifndef _WITH_DYNCALL
			switch (mmxReg) {
				case 0:
					__asm__ __volatile__("movsd %0,%%xmm0" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 1:
					__asm__ __volatile__("movsd %0,%%xmm1" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 2:
					__asm__ __volatile__("movsd %0,%%xmm2" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 3:
					__asm__ __volatile__("movsd %0,%%xmm3" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 4:
					__asm__ __volatile__("movsd %0,%%xmm4" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 5:
					__asm__ __volatile__("movsd %0,%%xmm5" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 6:
					__asm__ __volatile__("movsd %0,%%xmm6" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				case 7:
					__asm__ __volatile__("movsd %0,%%xmm7" ::"m"(argvalues[x + 3]->numvalue)
					                     :);
					break;
				default:
					ctx.abortInterprete("Too many floating point parameters (8 allowed)");
					return 0;
					break;
			}
			mmxReg++;
#else
			dcArgDouble(vm, (DCdouble)argvalues[x + 3]->numvalue);
#endif
		}
		else if (type == CALL_TYPE_STRING) {
#ifndef _WITH_DYNCALL
			v = (void *)argvalues[x + 3]->value.c_str();
			voidList[x2++] = v;
#else
			dcArgPointer(vm, (DCpointer)argvalues[x + 3]->value.c_str());
#endif
		}
		else if (type >= CALL_TYPE_INT_ARRAY) {

#ifndef _WITH_DYNCALL
			voidList[x2++] = array1;
			if (type == CALL_TYPE_INT_ARRAY) {
				voidList[x2++] = (void *)arraySize;
			}
#else
			dcArgPointer(vm, (DCpointer)array1);
			if (type == CALL_TYPE_INT_ARRAY) {
				dcArgInt(vm, (DCint)arraySize);
			}
#endif
		}
	}
#ifndef _WITH_DYNCALL
	// Fill parameter registers and stack for X86 64 Bit
	__asm__ __volatile__("movq %0, %%rdi" ::"r"(voidList[0])
	                     : "rdi");
	__asm__ __volatile__("movq %0, %%rsi" ::"r"(voidList[1])
	                     : "rsi");
	__asm__ __volatile__("movq %0, %%rdx" ::"r"(voidList[2])
	                     : "rdx");
	__asm__ __volatile__("movq %0, %%rcx" ::"r"(voidList[3])
	                     : "rcx");
	__asm__ __volatile__("movq %0, %%r8" ::"r"(voidList[4])
	                     : "r8");
	__asm__ __volatile__("movq %0, %%r9" ::"r"(voidList[5])
	                     : "r9");

	if (x3 >= 7) {
		__asm__ __volatile__("movq %0,%%rax\r\nmovq %%rax, (%%rsp)" ::"r"(voidList[6])
		                     : "rdi", "rsi", "rdx", "rcx", "r8", "r9");
	}
	if (x3 >= 8) {
		__asm__ __volatile__("movq %0,%%rax\r\nmovq %%rax, 8(%%rsp)" ::"r"(voidList[7])
		                     : "rdi", "rsi", "rdx", "rcx", "r8", "r9");
	}
	if (x3 >= 9) {
		__asm__ __volatile__("movq %0,%%rax\r\nmovq %%rax, 16(%%rsp)" ::"r"(voidList[8])
		                     : "rdi", "rsi", "rdx", "rcx", "r8", "r9");
	}
	if (x3 >= 10) {
		__asm__ __volatile__("movq %0,%%rax\r\nmovq %%rax, 24(%%rsp)" ::"r"(voidList[9])
		                     : "rdi", "rsi", "rdx", "rcx", "r8", "r9");
	}
	if (x3 >= 11) {
		__asm__ __volatile__("movq %0,%%rax\r\nmovq %%rax, 32(%%rsp)" ::"r"(voidList[10])
		                     : "rdi", "rsi", "rdx", "rcx", "r8", "r9");
	}

	SYS_UINT v3;
	__asm__ __volatile__("call *%0"
	                     : "=a"(v3)
	                     : "0"(func)
	                     : "memory");
	if (retDef == CALL_TYPE_INT) {
		ret = (int)v3;
	}
	else if (retDef == CALL_TYPE_BOOL) {
		ret = (bool)(v3 != 0);
	}
	else if (retDef == CALL_TYPE_STRING) {
		ret = (WCSTR)v3;
	}
	else if (retDef == CALL_TYPE_POINTER) {
		ret = (double)v3;
	}
	else if (retDef == CALL_TYPE_FLOAT) {
		float f;
		__asm__ __volatile__("movss %%xmm0,%0"
		                     : "=m"(f)::);
		ret = (double)f;
	}
	else if (retDef == CALL_TYPE_DOUBLE) {
		__asm__ __volatile__("movq %%xmm0, %0"
		                     : "=r"(v2)
		                     :);
		ret = (double)v2;
	}
	else {
		ret = true;
	}
#else
	if (retDef == CALL_TYPE_INT) {
		ret = (int)dcCallInt(vm, (DCpointer)func);
	}
	else if (retDef == CALL_TYPE_BOOL) {
		ret = (bool)dcCallInt(vm, (DCpointer)func) != 0;
	}
	else if (retDef == CALL_TYPE_STRING) {
		ret = (WCSTR)dcCallPointer(vm, (DCpointer)func);
	}
	else if (retDef == CALL_TYPE_POINTER) {
		ret = (double)(SYS_UINT)dcCallPointer(vm, (DCpointer)func);
	}
	else if (retDef == CALL_TYPE_FLOAT) {
		ret = (double)dcCallFloat(vm, (DCpointer)func);
	}
	else if (retDef == CALL_TYPE_DOUBLE) {
		ret = (double)dcCallDouble(vm, (DCpointer)func);
		;
	}
	else {
		dcCallInt(vm, (DCpointer)func);
		ret = true;
	}
	dcFree(vm);
#endif
	if (array1) {
		delete[] array1;
		array1 = NULL;
	}
	return 0;
}

int func_dlsym(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	HMODULE hDLL = (void *)(SYS_UINT)argvalues[0]->numvalue;
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}

	ret = false;
	argvalues[1]->toString();

	if (hDLL == NULL) {
		// Internal callback proc
		for (int i = 0; i < sizeof(g_callbackList) / sizeof(SCallbackProc); i++) {
			if (argvalues[1]->value == g_callbackList[i].name) {
				ret = (double)*(SYS_UINT *)&g_callbackList[i].procPtr;
				break;
			}
		}
		if (!ret)
			ctx.warnInterprete(WFormattedString("%s not found.", argvalues[1]->value.c_str()));
		return 0;
	}

	void *procAddr = dlsym(hDLL, argvalues[1]->value.c_str());
	if (procAddr)
		ret = (double)*(SYS_UINT *)&procAddr;
	else
		ctx.warnInterprete(WFormattedString("%s not found.", argvalues[1]->value.c_str()));
	return 0;
}

int func_call(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	HMODULE hDLL = (void *)(unsigned int)argvalues[0]->numvalue;
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}

	ret = false;
	void *procAddr;
	if (argvalues[1]->datatype == 2) {
		procAddr = (void *)(unsigned int)argvalues[1]->numvalue;
		argvalues[1]->toString();
	}
	else {
		procAddr = dlsym(hDLL, argvalues[1]->value.c_str());
	}
	if (!procAddr) {
		ctx.warnInterprete(WFormattedString("Symbol %s not found.", argvalues[1]->value.c_str()));
		return 0;
	}

	int retDef = CALL_TYPE_VOID;
	int paramDef[20];
	int paramCnt = 0;
	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype <= DataValue::DATATYPE_STR) {
			argvalues[2]->toString();
			// Parameter definition as string
			const char *c = argvalues[2]->value.c_str();
			while (*c != 0 && paramCnt < (sizeof(paramDef) / sizeof(int))) {
				if (*c == ':') {
					switch (*(c + 1)) {
						case 'i':
							retDef = CALL_TYPE_INT;
							break;
						case 'b':
							retDef = CALL_TYPE_BOOL;
							break;
						case 'f':
							retDef = CALL_TYPE_FLOAT;
							break;
						case 'd':
							retDef = CALL_TYPE_DOUBLE;
							break;
						case 's':
							retDef = CALL_TYPE_STRING;
							break;
						case 'p':
							retDef = CALL_TYPE_POINTER;
							break;
					}
					break;
				}
				switch (*c) {
					case 'f':
						paramDef[paramCnt++] = CALL_TYPE_FLOAT;
						break;
					case 'd':
						paramDef[paramCnt++] = CALL_TYPE_DOUBLE;
						break;
					case 's':
						paramDef[paramCnt++] = CALL_TYPE_STRING;
						break;
					case 'p':
						paramDef[paramCnt++] = CALL_TYPE_POINTER;
						break;
					case 'r':
						paramDef[paramCnt++] = CALL_TYPE_REFERENCE;
						break;
					case 'a':
						paramDef[paramCnt++] = CALL_TYPE_INT_ARRAY;
						break;
					default:
						paramDef[paramCnt++] = CALL_TYPE_INT;
						break;
				}
				c++;
			}
		}
		else {

			if (argvalues[2]->datatype < DataValue::DATATYPE_ARRAY)
				return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_ARRAY_REQUIRED;
			if (argvalues[2]->arrayList.size() < 2) {
				ctx.warnInterprete("Parameter array need 2 entries (1=params, 2=return type)");
				return 0;
			}
			int i = 0;
			argvalues[2]->arrayList.reset();
			while (argvalues[2]->arrayList.hasMoreElements() && (paramCnt < sizeof(paramDef) / sizeof(int))) {
				DataValue *entry = argvalues[2]->arrayList.nextValuePnt();
				if (i == 0) {
					entry->arrayList.reset();

					while (entry->arrayList.hasMoreElements()) {
						DataValue *param = entry->arrayList.nextValuePnt();
						char c = param->value.c_str()[0];
						switch (c) {
							case 'f':
								paramDef[paramCnt++] = CALL_TYPE_FLOAT;
								break;
							case 'd':
								paramDef[paramCnt++] = CALL_TYPE_DOUBLE;
								break;
							case 's':
								paramDef[paramCnt++] = CALL_TYPE_STRING;
								break;
							case 'r':
								paramDef[paramCnt++] = CALL_TYPE_REFERENCE;
								break;
							case 'p':
								paramDef[paramCnt++] = CALL_TYPE_POINTER;
								break;
							case 'a':
								paramDef[paramCnt++] = CALL_TYPE_INT_ARRAY;
								break;
							default:
								paramDef[paramCnt++] = CALL_TYPE_INT;
								break;
						}
					}
				}
				else if (i == 1) {
					char c = entry->value.c_str()[0];
					switch (c) {
						case 'i':
							retDef = CALL_TYPE_INT;
							break;
						case 'b':
							retDef = CALL_TYPE_BOOL;
							break;
						case 'f':
							retDef = CALL_TYPE_FLOAT;
							break;
						case 'd':
							retDef = CALL_TYPE_DOUBLE;
							break;
						case 's':
							retDef = CALL_TYPE_STRING;
							break;
						case 'p':
							retDef = CALL_TYPE_POINTER;
							break;
					}
				}
				i++;
			}
		}
	}

	int retCode = func_universal_call(argvalues, ret, ctx, procAddr, paramDef, paramCnt, retDef);
	return retCode;
}

int func_dl(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	bool fNative = false;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR) {
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_BOOL_REQUIRED;
		}
		fNative = (bool)*argvalues[1];
	}

	if (ctx.currThread != NULL) {
		// Cannot load extensions in threads
		ctx.abortInterprete("Its not allowed to load libraries in threads.");
		return 0;
	}

	WMutexLocker locker(ctx.mutex);

	if (!fNative && ctx.dllModuleHT.isKey(argvalues[0]->value.c_str())) {
		ret = true;
		return 0;
	}

	// Load DLL library

	HMODULE hDLL = NULL;

	WString currDir;

	if (!fNative && argvalues[0]->value.c_str()[0] != '/') {
		
		// WSystem::getCurrentDirectory (currDir);
		char currExec[PATH_MAX + 1];
		ssize_t len = readlink("/proc/self/exe", currExec, PATH_MAX);
		if (len > 0) {
			currExec[len] = 0;
			currDir = (WCSTR) dirname(currExec);
			currDir += "/";
		}
		
		// currDir+="/ext/";
	}

	WString ext;
	WSystem::extractFileExtension(argvalues[0]->value.c_str(), ext);
	if (!ext.equals(""))
		ext = "";
	else
		ext = ".so";

	hDLL = dlopen((WCSTR) WFormattedString ("%s%s%s", (WCSTR) currDir, (WCSTR) argvalues[0]->value.c_str(), (WCSTR) ext), RTLD_LAZY);
	if (hDLL != NULL) {
		if (fNative) {
			ret = (double) (SYS_UINT) hDLL;
		}
		else {
			WSCRIPT_DLL_STARTUP wScriptDLLStartup = (WSCRIPT_DLL_STARTUP)dlsym(hDLL, "wScriptDLLStartup");
			if (wScriptDLLStartup != NULL) {
				ctx._callUserFunc = callUserFunc;
				wScriptDLLStartup(ctx);
				ctx.dllModuleHT.put (argvalues[0]->value.c_str(), hDLL);
				ret = true;
			}
			else {
				ctx.abortInterprete(WFormattedString("Invalid extension %s. wScriptDLLStartup not found.", argvalues[0]->value.c_str()));
				ret = false;
			}
		}
	}
	else {
		if (!fNative)
			ctx.abortInterprete(WFormattedString("Cannot load library %s. %s", argvalues[0]->value.c_str(), dlerror()));
		else
			ctx.warnInterprete(WFormattedString("Cannot load library %s. %s", argvalues[0]->value.c_str(), dlerror()));
		ret = false;
	}
	return 0;
}

int func_resize(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}
	ret = false;
	if (argvalues[0]->refPnt) {
		argvalues[0]->refPnt->value.resize((int)argvalues[1]->numvalue);
		argvalues[0]->refPnt->datatype = DataValue::DATATYPE_STR;
		ret = true;
	}
	return 0;
}

int func_memat(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	int size = 0;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		size = argvalues[1]->numvalue;
	}
	void *memory = (void *)(SYS_UINT)argvalues[0]->numvalue;
	// memcpy (&memory,  &argvalues[0]->numvalue, sizeof (memory));
	// memory = (void*) *((unsigned int*) &argvalues[0]->numvalue);
	if (size) {
		ret.value.resize(size);
		memcpy((void *)ret.value.c_str(), (void *)memory, size);
	}
	else
		ret.value = (WCSTR)memory;
	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_memref(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}
	if (argvalues[0]->refPnt)
		ret = (double)(SYS_UINT)argvalues[0]->refPnt->value.c_str();
	else
		ret = (double)(SYS_UINT)argvalues[0]->value.c_str();
	return 0;
}

int func_memset(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
	}

	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}

	int pos = 0;

	if (argvalues.size() > 2) {
		if (argvalues[2]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3 | WSCRIPT_RET_NUM_REQUIRED;
		pos = argvalues[2]->numvalue;
	}

	char *memPnt = (char *)(unsigned int)argvalues[0]->numvalue + pos;

	memcpy(memPnt, argvalues[1]->value.c_str(), argvalues[1]->value.size());
	ret = (int)argvalues[1]->value.size();
	return 0;
}

int func_extension_loaded(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	ret = ctx.dllModuleHT.isKey(argvalues[0]->value.c_str());
	return 0;
}

int func_function_exists(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	ret = (ctx.nativeFunctionHT.isKey(argvalues[0]->value.c_str()) || (ctx.functions.isKey(argvalues[0]->value.c_str())));
	return 0;
}

int func_call_user_func(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	callUserFunc(argvalues[0]->value, argvalues, ret, ctx, 1);
	return 0;
}

int func_thread_create(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	Handle *h = new Handle;
	ret = (Handle *)NULL;
	try {

		h->handle = (void *)new WScriptThread();
		((WScriptThread *)*h)->create(argvalues, ctx);

		// Register handle
		h->handletype = Handle::HANDLETYPE_THREAD;
		h->freeFunction = freeHandle;
		ret = ctx.handleHT.put((void *)h, h)->m_value;
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		if (h->handle) {
			delete (WScriptThread *)h->handle;
			h->handle = NULL;
		}
		delete h;
		h = NULL;
	}
	return 0;
}

int func_thread_start(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	bool fWaitStarted = ctx.currThread ? true : false;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_BOOL_REQUIRED;
		fWaitStarted = (bool)*argvalues[1];
	}

	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			ret = thread->start(fWaitStarted);
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_finish(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			thread->finish();
			ret = true;
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_join(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	int ms = -1;
	if (argvalues.size() > 1) {
		if (argvalues[1]->datatype > DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
		ms = (int)*argvalues[1];
	}
	ret = false;
	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			if (thread != ctx.currThread) {
				ret = thread->join((W32)ms);
			}
			else {
				// Cannot join myself
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_wait(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	int ms = 0;
	if (argvalues[0]->datatype != DataValue::DATATYPE_NO_PARAM) {
		if (PARAM1_NOSTR)
			return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_NUM_REQUIRED;
		ms = (int)*argvalues[0];
	}
	if (ctx.currThread) {
		ret = ctx.currThread->wait(ms);
	}
	else {
		ret = false;
	}
	return 0;
}

int func_thread_signal(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			if (thread != ctx.currThread) {
				thread->signal();
				ret = true;
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_suspend(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			if (thread != ctx.currThread) {
				thread->suspend();
				ret = true;
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_resume(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			if (thread != ctx.currThread) {
				thread->resume();
				ret = true;
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_kill(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}
	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			if (thread != ctx.currThread) {
				WMutexLocker locker(ctx.mutex);
				thread->kill();
				ret = true;
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_close(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			if (thread != ctx.currThread) {

				WMutexLocker locker(ctx.mutex);
				thread->kill();
				try {
					delete h;
				}
				catch (WException) {
				}
				*(argvalues[0]->refPnt) = (Handle *)NULL;
				ctx.handleHT.remove(h);
				ret = true;
			}
			else {
				ret = false;
			}
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_is_finish(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{

	if (ctx.currThread) {
		ret = ctx.currThread->isFinish();
	}
	else {
		ret = false;
	}
	return 0;
}

int func_thread_is_active(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (argvalues[0]->datatype != DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_HANDLE_REQUIRED;
	}

	Handle *h = (Handle *)*argvalues[0];
	try {
		if (h && ctx.handleHT.isKey(h) && h->handletype == Handle::HANDLETYPE_THREAD) {
			WScriptThread *thread = (WScriptThread *)h->handle;
			ret = thread->isActive();
		}
		else {
			ret = false;
		}
	}
	catch (WException &e) {
		ctx.warnInterprete(e);
		ret = false;
	}
	return 0;
}

int func_thread_sync_enter(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	WSystem::enterMutex(ctx.mutex);
	ret = true;

	return 0;
}

int func_thread_sync_leave(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	WSystem::leaveMutex(ctx.mutex);
	ret = true;
	return 0;
}

int func_crc32(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	W32 crc32 = 0;
	argvalues[0]->toString();
	WSystem::calcCRC32((W8 *)argvalues[0]->value.c_str(), argvalues[0]->value.size(), crc32);
	ret = (double)crc32;
	return 0;
}

int func_base64_encode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	argvalues[0]->toString();

	int len = argvalues[0]->value.size();
	int bufLen = Base64encode_len(len);

	WString str(bufLen);
	ret.datatype = DataValue::DATATYPE_STR;

	ret.value.assign((WCSTR)str, Base64encode((char *)str.getBuf(), argvalues[0]->value.c_str(), len) - 1);
	return 0;
}

int func_base64_decode(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[0]->toString();
	int len = Base64decode_len(argvalues[0]->value.c_str());
	WString str(Base64encode_len(len));
	ret.datatype = DataValue::DATATYPE_STR;
	ret.value.assign((WCSTR)str, Base64decode((char *)str.getBuf(), argvalues[0]->value.c_str()));
	return 0;
}

int chunk_split(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1 | WSCRIPT_RET_STR_REQUIRED;
	}

	if (argvalues.size() < 2 || argvalues[1]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_NUM_REQUIRED;
	}

	if (argvalues.size() < 3 || argvalues[2]->datatype > DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2 | WSCRIPT_RET_STR_REQUIRED;
	}

	argvalues[0]->toString();
	argvalues[2]->toString();

	int len = (int)argvalues[1]->numvalue;
	int max = argvalues[0]->value.size();
	int lenCheck = 0;
	int lenDelimiter = argvalues[2]->value.size();

	// Reserve buffer
	ret.value.resize(0);
	ret.value.reserve(max + (max / len) * lenDelimiter);

	const char *from = argvalues[0]->value.c_str();
	const char *from0 = from;
	while (*from != 0) {

		// Check delimiter
		const char *check = argvalues[2]->value.c_str();
		const char *from2 = from;

		while (*check != 0 && *from2 != 0 && *check == *from2) {
			check++;
			from2++;
		}

		if (*check == 0) {
			// Found
			lenCheck = 0;
			from = from2;
			continue;
		}

		if (lenCheck == len) {
			ret.value.append(from0, from - from0);
			ret.value.append(argvalues[2]->value);
			from0 = from;
			lenCheck = 0;
		}
		from++;
		lenCheck++;
	}
	// Append rest
	ret.value.append(from0, from - from0);

	ret.datatype = DataValue::DATATYPE_STR;
	return 0;
}

int func_current_file(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = ctx.getFilenameByIdx(WSCRIPT_FILE(ctx.line));
	return 0;
}

int func_current_line(vector<DataValue *> &argvalues, DataValue &ret, InterpreterContext &ctx)
{
	ret = (int)WSCRIPT_LINE(ctx.line);
	return 0;
}

void wScriptInitFunctions(InterpreterContext &ctx)
{

	WScriptFuncDef funcDefList[] = {

		// Native functions
		{"_cmp_ne", func_cmp_ne},
		{"_cmp_ne2", func_cmp_ne2},
		{"_cmp_le", func_cmp_le},
		{"_cmp_ge", func_cmp_ge},
		{"_bool_or", func_bool_or},
		{"_bool_and", func_bool_and},
		{"_bin_or", func_bin_or},
		{"=_bin_or", func_bin_or_equal},
		{"_bin_and", func_bin_and},
		{"=_bin_and", func_bin_and_equal},
		{"_bin_xor", func_bin_xor},
		{"=_bin_xor", func_bin_xor_equal},
		{"_bin_shiftl", func_bin_shiftl},
		{"=_bin_shiftl", func_bin_shiftl_equal},
		{"_bin_shiftr", func_bin_shiftr},
		{"=_bin_shiftr", func_bin_shiftr_equal},
		{"_bin_neg", func_bin_neg},

		{"_less", func_less},
		{"_greater", func_greater},
		{"_equal", func_equal},
		{"_equal2", func_equal2},
		{"_plus", func_plus},
		{"_plusstr", func_plusstr},
		{"=_plus", func_plus_equal},
		{"=_plusstr", func_plusstr_equal},
		{"_minus", func_minus},
		{"=_minus", func_minus_equal},
		{"_mul", func_mul},
		{"=_mul", func_mul_equal},
		{"_pow", func_pow},
		{"=_pow", func_pow_equal},
		{"_div", func_div},
		{"=_div", func_div_equal},

		{"_mod", func_mod},
		{"=_mod", func_mod_equal},
		{"_uminus", func_uminus},
		{"=_inc", func_inc},
		{"=_dec", func_dec},
		{"_not", func_not},

		{"__FILE__", func_current_file},
		{"__LINE__", func_current_line},

		{"return", func_return},
		{"break", func_break},
		{"continue", func_continue},

		// Array functions
		// isset, unset, assign are hard coded!
		{"count", func_count},
		{"in_array", func_in_array},
		{"array_search", func_array_search},
		{"array_key_exists", func_array_key_exists},
		{"array_keys", func_array_keys},
		{"array_values", func_array_values},
		{"array_push", func_array_push},
		{"array_pop", func_array_pop},
		{"array_merge", func_array_merge},
		{"array_slice", func_array_slice},
		{"array_splice", func_array_splice},
		{"array_flip", func_array_flip},
		{"array_reverse", func_array_reverse},
		{"sort", func_sort},
		{"rsort", func_rsort},
		{"asort", func_asort},
		{"arsort", func_arsort},
		{"ksort", func_ksort},
		{"krsort", func_krsort},
		{"usort", func_usort},
		{"uasort", func_uasort},
		//  { "uksort", func_array_uksort },

		// IO and program functions
		{"version", func_version},
		{"setlocale", func_setlocale},
		//  { "error_get_last",  func_error_get_last},
		{"print", func_print},
		{"sprintf", func_sprintf},
		{"printf", func_printf},
		{"sscanf", func_sscanf},
		{"console_echo", func_input_echo},
		{"input", func_input},
		{"getch", func_getch},
		{"exit", func_exit},
		{"include", func_include},
		{"require_once", func_require_once},
		{"eval", func_eval},
		{"call_user_func", func_call_user_func},
		{"exec", func_exec},
		{"shellexec", func_shell_exec},
		{"system", func_system},
		{"autonl", func_autonl},
		{"error_reporting", func_show_warnings},
		{"getreg", func_getreg},
		{"putreg", func_putreg},
		{"delreg", func_delreg},
		{"getenv", func_getenv},
		{"putenv", func_putenv},
		{"dl", func_dl},
		{"dlsym", func_dlsym},
		{"call", func_call},
		{"memat", func_memat},
		{"memref", func_memref},
		{"memset", func_memset},
		// { "extension_loaded",  func_extension_loaded},
		{"function_exists", func_function_exists},

		// Datatype functions
		{"gettype", func_gettype},
		{"intval", func_intval},
		{"doubleval", func_doubleval},
		{"strval", func_strval},
		{"empty", func_empty},
		{"is_array", func_is_array},
		{"print_r", func_print_r},

		// Date/Time functions
		{"time", func_time},
		{"date", func_date},
		{"easter_date", func_easter_date},
		{"gmdate", func_gmdate},
		{"mktime", func_mktime},
		//  { "strtotime",  func_strtotime},
		{"microtime", func_microtime},
		{"sleep", func_sleep},

		// Mathematical functions
		{"sin", func_sin},
		{"asin", func_asin},
		{"cos", func_cos},
		{"acos", func_acos},
		{"tan", func_tan},
		{"atan", func_atan},
		{"atan2", func_atan2},
		{"floor", func_floor},
		{"ceil", func_ceil},
		{"sqrt", func_sqrt},
		{"log", func_log},
		{"log10", func_log10},
		{"exp", func_exp},
		{"fabs", func_fabs},
		{"abs", func_fabs},
		{"fmod", func_fmod},
		{"mod", func_fmod},
		{"pow", func_pow},
		{"round", func_round},

		// Hash functions
		{"crc32", func_crc32},
		{"base64_encode", func_base64_encode},
		{"base64_decode", func_base64_decode},
		{"bin2hex", func_bin2hex},
		{"hex2bin", func_hex2bin},
		{"binparse", func_binparse},
		{"binformat", func_binformat},

		// Random functions
		{"rand", func_rand},
		{"srand", func_srand},

		// String functions
		{"chr", func_chr},
		//  { "pack",  func_pack},
		{"strlen", func_strlen},
		{"strcmp", func_strcmp},
		{"strpos", func_strpos},
		{"substr", func_substr},
		{"strtoupper", func_strtoupper},
		{"strtolower", func_strtolower},
		{"trim", func_trim},
		{"rtrim", func_rtrim},
		{"ltrim", func_ltrim},
		{"str_replace", func_str_replace},
		{"utf8_encode", func_utf8_encode},
		{"utf8_decode", func_utf8_decode},
		{"explode", func_explode},
		{"resize", func_resize},
		{"chunk_split", chunk_split},

		// File functions
		{"fopen", func_fopen},
		{"fwrite", func_fwrite},
		{"fflush", func_fflush},
		{"fread", func_fread},
		{"freadln", func_freadln},
		{"fgets", func_fgets},
		{"fseek", func_fseek},
		{"ftell", func_ftell},
		{"feof", func_feof},
		{"fclose", func_fclose},
		{"pathinfo", func_pathinfo},
		{"realpath", func_realpath}, // Does not work for files with length 0 and non readable files!
		{"filesize", func_filesize},
		{"filemtime", func_filemtime},
		{"filectime", func_filectime},
		{"fileatime", func_fileatime},
		{"basename", func_basename},
		{"dirname", func_dirname},
		{"file_exists", func_file_exists},
		{"is_file", func_is_file},
		{"is_dir", func_is_dir},
		{"filetype", func_filetype},
		{"unlink", func_unlink},
		{"rename", func_rename},
		{"copy", func_copy},
		//  { "stat",  func_stat},
		{"touch", func_touch},

		// Directory functions
		{"opendir", func_opendir},
		{"closedir", func_closedir},
		{"readdir", func_readdir},
		{"chdir", func_chdir},
		{"rmdir", func_rmdir},
		{"mkdir", func_mkdir},
		{"getcwd", func_getcwd},

		// Thread functions
		{"thread_create", func_thread_create},
		{"thread_start", func_thread_start},
		{"thread_join", func_thread_join},
		{"thread_wait", func_thread_wait},
		{"thread_signal", func_thread_signal},
		{"thread_suspend", func_thread_suspend},
		{"thread_resume", func_thread_resume},
		{"thread_kill", func_thread_kill},
		{"thread_close", func_thread_close},
		{"thread_set_finish", func_thread_finish},
		{"thread_is_finish", func_thread_is_finish},
		{"thread_is_active", func_thread_is_active},
		{"thread_sync_enter", func_thread_sync_enter},
		{"thread_sync_leave", func_thread_sync_leave},

		// Web functions
		{"header", func_header},
		{"move_uploaded_file", func_move_uploaded_file},
		{"is_uploaded_file", func_is_uploaded_file},
		{"htmlspecialchars", func_htmlspecialchars},
		{"nl2br", func_nl2br},
		{"url_decode", func_url_decode},
		{"url_encode", func_url_encode},
		{"json_encode", func_json_encode},
		{"json_decode", func_json_decode},

		// Socket functions, use fwrite, fread, fclose ...
		{"fsockopen", func_fsockopen},
		{"fsockserver", func_fsockserver},
		{"fsockaccept", func_fsockaccept},
		{"fsockip", func_fsockip},
		{"fsockport", func_fsockport},
		{"gethostbyaddr", func_gethostbyaddr},
		{"gethostbyname", func_gethostbyname},

	};

	for (unsigned int i = 0; i < sizeof(funcDefList) / sizeof(WScriptFuncDef); i++)
		ctx.nativeFunctionHT.put(funcDefList[i].name, (WSCRIPT_NATIVE_FUNCTION)funcDefList[i].func);

	// Contants
	typedef struct _SConstant {
		const char *name;
		double numval;
		const char *textval;
	} SConstant;

	SConstant constList[] = {
		{"_ALIGN", sizeof(void *), NULL},
	};

	for (int i = 0; i < sizeof(constList) / sizeof(SConstant); i++)
		ctx._setConstant(constList[i].name, constList[i].numval, constList[i].textval, ctx);

	ctx._callUserFunc = callUserFunc;
}

void wScriptCleanupFunctions(InterpreterContext &ctx)
{
	ctx.nativeFunctionHT.removeAll();
}

void freeArgValues(vector<DataValue *> &argvalues)
{

	for (unsigned int i = 0; i < argvalues.size(); i++) {
		delete argvalues[i];
	}
}

DataValue *getVariable(ExpressionList &arguments, InterpreterContext &ctx, bool fCreateNew, unsigned int &argOffs, unsigned int *flags = NULL)
{
	string name = arguments[argOffs].operation;
	DataValue *v = NULL, *v2 = NULL;
	int j = 0;
	for (j = ctx.funcDeep; j >= 0; j -= ctx.funcDeep) {
		v = ctx.symbols[j]->getPnt(name.c_str());
		if (v) {
			break;
		}
		if (j == 0)
			break;
	}

	if (!v) {
		if (fCreateNew) {
			DataValue datavalue;
			v = &ctx.symbols[ctx.funcDeep]->put(name.c_str(), datavalue)->m_value;
		}
	}
	else {
		if (fCreateNew && j == 0) {
			ctx.abortInterprete(WFormattedString("Illegal overwriting of %s", (WCSTR)name.c_str()));
			return NULL;
		}

		if (flags)
			*flags = v->flags;
		if ((v->flags & DataValue::FLAGS_USEREF) && v->refPnt)
			v = v->refPnt;
	}

	// Calc indexes
	if (v && arguments[argOffs].datatype > DataValue::DATATYPE_ARRAY) {

		unsigned int lastIndexPos = (arguments[argOffs].datatype - DataValue::DATATYPE_ARRAY) + argOffs + 1;
		argOffs++;
		for (argOffs; argOffs < lastIndexPos; argOffs++) {
			v2 = v;
			DataValue index;
			DataValue *indexPnt = arguments[argOffs].calc(ctx, index);
			indexPnt->toString();
			if (!v2->arrayList.isKey(indexPnt->value.c_str())) {
				// Index not found, make empty value
				DataValue datavalue;
				v2->arrayList.put(indexPnt->value.c_str(), datavalue);
			}
			// Parent is an array!
			v2->datatype = 10;
			v2->numvalue = 1;
			v = v2->arrayList.getPnt(indexPnt->value.c_str());
		}
	}
	else {
		argOffs++;
	}

	if (!v) {
		ctx.abortInterprete(WFormattedString("Variable %s is not defined", name.c_str()));
	}
	return v;
}

void assignVariables(ExpressionList &arguments, DataValue &retvalue, InterpreterContext &ctx)
{
	DataValue empty;
	unsigned int i = 0;
	retvalue.arrayList.reset();
	while (i < arguments.size()) {
		DataValue *value = NULL;
		if (retvalue.datatype >= DataValue::DATATYPE_ARRAY) {
			if (retvalue.arrayList.hasMoreElements()) {
				value = retvalue.arrayList.nextValuePnt();
			}
		}
		if (!value)
			value = &empty;

		if ((arguments[i].opCode & 0xFF) == OPCODE_ASSIGN) {
			// Inner assign operation
			assignVariables(arguments[i].arguments[0].arguments, *value, ctx);
			i++;
		}
		else {
			// Its a variable
			DataValue *v = getVariable(arguments, ctx, true, i);
			*v = *value;
		}
	}
}
///////////////////////
// QUICK FUNCTIONS
///////////////////////

DataValue *quickFuncEqual(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->datatype = DataValue::DATATYPE_BOOL;
	ret->numvalue = *a == *b;
	return ret;
}

DataValue *quickFuncNot(DataValue *a, DataValue *b, DataValue *ret)
{
	ret->datatype = DataValue::DATATYPE_BOOL;
	ret->numvalue = !*a;
	return ret;
}

DataValue *quickFuncNotEqual(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->datatype = DataValue::DATATYPE_BOOL;
	ret->numvalue = *a != *b;
	return ret;
}

DataValue *quickFuncUMinus(DataValue *a, DataValue *b, DataValue *ret)
{
	ret->datatype = DataValue::DATATYPE_NUM;
	ret->numvalue = -a->numvalue;
	return ret;
}

DataValue *quickFuncLess(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->datatype = DataValue::DATATYPE_BOOL;
	ret->numvalue = *a < *b;
	return ret;
}

DataValue *quickFuncGreater(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->datatype = DataValue::DATATYPE_BOOL;
	ret->numvalue = *a > *b;
	return ret;
}

DataValue *quickFuncLessEqual(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->datatype = DataValue::DATATYPE_BOOL;
	ret->numvalue = *a <= *b;
	return ret;
}

DataValue *quickFuncGreaterEqual(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->datatype = DataValue::DATATYPE_BOOL;
	ret->numvalue = *a >= *b;
	return ret;
}

DataValue *quickFuncInc(DataValue *a, DataValue *b, DataValue *ret)
{
	ret->numvalue = a->numvalue;
	ret->datatype = a->datatype = DataValue::DATATYPE_NUM;
	a->datatype = DataValue::DATATYPE_NUM;
	a->numvalue++;
	return ret; // Return old value!
}

DataValue *quickFuncDec(DataValue *a, DataValue *b, DataValue *ret)
{
	ret->numvalue = a->numvalue;
	ret->datatype = a->datatype = DataValue::DATATYPE_NUM;
	a->numvalue--;
	return ret; // Return old value!
}

DataValue *quickFuncPlusEqual(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	if (a->datatype == DataValue::DATATYPE_NUM && b->datatype == DataValue::DATATYPE_NUM)
		a->numvalue += b->numvalue;
	else {
		*a += *b;
	}
	return a;
}

DataValue *quickFuncPlus(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	*ret = *a + *b;
	return ret;
}

DataValue *quickFuncMinus(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->numvalue = a->numvalue - b->numvalue;
	ret->datatype = DataValue::DATATYPE_NUM;
	return ret;
}

DataValue *quickFuncMinusEqual(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	a->numvalue -= b->numvalue;
	a->datatype = DataValue::DATATYPE_NUM;
	return a;
}

DataValue *quickFuncMul(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->numvalue = a->numvalue * b->numvalue;
	ret->datatype = DataValue::DATATYPE_NUM;
	return ret;
}

DataValue *quickFuncMulEqual(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	a->numvalue *= b->numvalue;
	a->datatype = DataValue::DATATYPE_NUM;
	return a;
}

DataValue *quickFuncDiv(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	ret->numvalue = a->numvalue / b->numvalue;
	ret->datatype = DataValue::DATATYPE_NUM;
	return ret;
}

DataValue *quickFuncDivEqual(DataValue *a, DataValue *b, DataValue *ret)
{
	if (b->datatype == DataValue::DATATYPE_STR)
		b->setAutoDataType(true); // Maybe a number
	a->numvalue /= b->numvalue;
	a->datatype = DataValue::DATATYPE_NUM;
	return a;
}

DataValue *Expression::calc(InterpreterContext &ctx, DataValue &retvalue, unsigned int flags)
{
	if (ctx.fAbort == 1) {
		throw AbortException("Aborted", ctx.exitCode);
	}
#ifdef _LOG_PERFORMANCE
	W64 counter;
	WSystem::getPerformanceCounter(counter);
	double tD = (double)(counter - g_timerCounter) / (double)g_timerFreq;
	double tDStart = (double)(counter - g_timerCounterStart) / (double)g_timerFreq;
	g_performanceLogFile.writeln(WFormattedString("Line %3i (%.6f): | %-16s | %-16s | %8X | %u | %.6f", WSCRIPT_LINE(line), tD, (WCSTR)operation.c_str(), (WCSTR)constvalue ? constvalue->c_str() : "", datatype, arguments.size(), tDStart));
	g_timerCounter = counter;
#endif

	// Quick functions (arithmetic operations)
	if (opCode & OPCODE_QUICK_FUNC) {
		DataValue a, b, *aPnt, *bPnt;
#ifdef _DEBUG
		bPnt = NULL;
#endif

		unsigned int indexCnt = 0;
		for (unsigned int idx = 0; idx < arguments.size(); idx++) {
			Expression &e = arguments[idx];
			if (opCode & OPCODE_EQUAL_OP) {
				if (idx == 0) {
					if (e.datatype > DataValue::DATATYPE_ARRAY)
						indexCnt = e.datatype - DataValue::DATATYPE_ARRAY; // Indexes
					aPnt = ctx.symbols[ctx.funcDeep]->getPnt(e.operation.c_str());
					if (!aPnt && ctx.symbols[0]->getPnt(e.operation.c_str()) != NULL) {
						ctx.line = this->line; // Refresh the line
						ctx.abortInterprete(WFormattedString("Illegal overwriting of %s", (WCSTR)e.operation.c_str()));
						return &retvalue;
					}
					if (!aPnt || (indexCnt > 0 && aPnt->datatype < DataValue::DATATYPE_ARRAY)) {
						ctx.line = this->line; // Refresh the line
						ctx.abortInterprete(WFormattedString("%s %s is not defined", indexCnt > 0 ? "Array" : "Variable", e.operation.c_str()));
						return &retvalue;
					}
					else if ((aPnt->flags & DataValue::FLAGS_USEREF) && aPnt->refPnt)
						aPnt = aPnt->refPnt;
					continue;
				}
				if (idx <= indexCnt) {
					// Index value
					if (aPnt) {
						DataValue index, *indexPnt;
						indexPnt = e.calc(ctx, index);
						indexPnt->toString();
						if (!aPnt->arrayList.isKey(indexPnt->value.c_str()) || aPnt->datatype < DataValue::DATATYPE_ARRAY) {
							// Index not found
							aPnt->datatype = DataValue::DATATYPE_ARRAY + (indexCnt - idx) + 1;
							DataValue empty;
							aPnt = &aPnt->arrayList.put(indexPnt->value.c_str(), empty)->m_value;
							aPnt->refPnt = aPnt;
						}
						else
							aPnt = aPnt->arrayList.getPnt(indexPnt->value.c_str());
					}
					continue;
				}
			}

			// Evaluate the arguments for the function
			DataValue *tmp;
			if (e.constDataValue) {
				// Argument is a constant
				tmp = e.constDataValue;
			}
			else if (e.opCode == OPCODE_VARIABLE) {
				// Argument is a single variable
				tmp = ctx.symbols[ctx.funcDeep]->getPnt(e.operation.c_str());
				if (!tmp) {
					tmp = ctx.symbols[0]->getPnt(e.operation.c_str());
				}
				if (!tmp) {
					ctx.line = this->line; // Refresh the line
					ctx.abortInterprete(WFormattedString("Variable %s is not defined", e.operation.c_str()));
					return &retvalue;
				}
				if ((tmp->flags & DataValue::FLAGS_USEREF) && tmp->refPnt) {
					tmp = tmp->refPnt;
					tmp->flags |= DataValue::FLAGS_USEREF;
				}
				tmp->refPnt = tmp;
			}
			else {
				// Argument is a array value or expression
				tmp = e.calc(ctx, idx == 0 ? a : b);
			}
			if (idx == 0)
				aPnt = tmp;
			else
				bPnt = tmp;
		}

		if (aPnt->datatype == DataValue::DATATYPE_STR)
			aPnt->setAutoDataType(true); // Maybe a number

		return ((WSCRIPT_QUICK_FUNCTION)this->userFunc)(aPnt, bPnt, &retvalue);
	}

	ctx.line = this->line;

	DataValue *retValuePnt = &retvalue;

	unsigned int _opCode = opCode & 0xFF;
	if (opCode & OPCODE_FUNCTION)
		goto function;

	//////////////////////////////////
	// Constants
	//////////////////////////////////
	if (opCode == OPCODE_CONSTANT) {
		if (!constDataValue) {
			// Compile the constant
			if (constvalue)
				constDataValue = new DataValue(*constvalue);
			else
				constDataValue = new DataValue(); // NULL
			constDataValue->datatype = datatype;  // Set native datatype
			if (datatype == DataValue::DATATYPE_STR)
				constDataValue->setAutoDataType(true); // Only convert string to number
			else if (datatype > 0)
				constDataValue->setAutoDataType();
		}
		if (flags == EXPRESSION_FLAG_STORE) {
			retvalue = *constDataValue;
			return retValuePnt;
		}
		return constDataValue;
	}
	else if (datatype > 1) {
		//////////////////////////////////
		// Data and Arrays
		//////////////////////////////////

		if (datatype == DataValue::DATATYPE_OUTPUT) {
			// HTML data outside <?v1 ... ?>
			if (ctx.fOutputNoScript) {
				ctx.printHeaders();
				cout << constvalue->c_str();
			}
			return retValuePnt;
		}
		else if (datatype == DataValue::DATATYPE_ARRAYDEF) {
			// Array definition
			int idx = 0;
			bool fOwnKeys = false;
			DataValue newArray;
			DataValue *newArrayPnt = &retvalue;
			if (flags == EXPRESSION_FLAG_STORE) {
				newArrayPnt = &newArray;
			}
			newArrayPnt->arrayList.removeAll();
			newArrayPnt->datatype = DataValue::DATATYPE_ARRAY;
			for (unsigned int i = 0; i < arguments.size(); i++) {
				Expression &e = arguments[i];
				string key = std::to_string(idx); // default key
				if (e.datatype == DataValue::DATATYPE_ARRAYKEY) {
					DataValue arrayKey;
					DataValue *arrayKeyPnt = e.arguments[0].calc(ctx, arrayKey);
					if (arrayKeyPnt->datatype < DataValue::DATATYPE_STR || arrayKeyPnt->fNumberString) {
						int idx2 = (int)arrayKeyPnt->numvalue;
						if (idx2 >= idx) {
							arrayKeyPnt->numvalue = idx = idx2;
							newArrayPnt->arrayList.setMaxAutoIndex(idx);
							idx++;
						}
					}

					arrayKeyPnt->toString();
					DataValue *arrayValuePnt = &newArrayPnt->arrayList.put(arrayKeyPnt->value.c_str())->m_value;
					e.arguments[1].calc(ctx, *arrayValuePnt, EXPRESSION_FLAG_STORE);
					fOwnKeys = true;
				}
				else {
					DataValue *arrayValuePnt = &newArrayPnt->arrayList.put(key.c_str())->m_value;
					e.calc(ctx, *arrayValuePnt, EXPRESSION_FLAG_STORE);
					newArrayPnt->arrayList.setMaxAutoIndex(idx);
					idx++;
				}
			}
			newArrayPnt->arrayList.setOwnKeys(fOwnKeys);
			if (flags == EXPRESSION_FLAG_STORE) {
				retvalue = newArray;
			}
			return retValuePnt;
		}
		else if (datatype >= DataValue::DATATYPE_ARRAY) {

			if (IS_OPCODE(OPCODE_EQUAL)) {
				// array[x]=expression
				DataValue *arrayValue = NULL;
				int j = ctx.funcDeep;
				for (j = ctx.funcDeep; j >= 0; j -= ctx.funcDeep) {
					arrayValue = ctx.symbols[j]->getPnt(constvalue->c_str());
					if (arrayValue == NULL) {
						if (j == 0) {
							j = ctx.funcDeep;
							break;
						}
						continue;
					}
					if (arrayValue->refPnt && (arrayValue->flags & DataValue::FLAGS_USEREF)) {
						arrayValue = arrayValue->refPnt;
					}
					break;
				}

				int indexCnt = datatype - DataValue::DATATYPE_ARRAY;
				if (j == 0) {
					ctx.abortInterprete(WFormattedString("Illegal overwriting of %s", (WCSTR)constvalue->c_str()));
					return retValuePnt;
				}
				else {
					if (!arrayValue) {
						// New value
						arrayValue = &ctx.symbols[j]->put(constvalue->c_str())->m_value;
						arrayValue->refPnt = arrayValue;
					}
					arrayValue->datatype = DataValue::DATATYPE_ARRAY + indexCnt;
					DataValue index;
					for (unsigned int i = 0; i < arguments.size(); i++) {
						Expression &e = arguments[i];
						if (indexCnt > 0) {
							// Create index
							DataValue *indexPnt = e.calc(ctx, index);

							indexPnt->toString();
							indexCnt--;
							if (!arrayValue->arrayList.isKey(indexPnt->value.c_str()) || arrayValue->datatype < DataValue::DATATYPE_ARRAY) {
								DataValue empty;
								arrayValue->datatype = DataValue::DATATYPE_ARRAY + indexCnt + 1;

								if (indexPnt->datatype < DataValue::DATATYPE_STR || indexPnt->fNumberString) {
									// Set max auto index
									int idx = (int)index.numvalue;
									if (idx > arrayValue->arrayList.getMaxAutoIndex())
										arrayValue->arrayList.setMaxAutoIndex(idx);
								}

								arrayValue->arrayList.setOwnKeys(true);
								arrayValue = &arrayValue->arrayList.put(indexPnt->value.c_str(), empty)->m_value;
							}
							else {
								arrayValue = arrayValue->arrayList.getPnt(indexPnt->value.c_str());
							}
						}
						else {
							// Assign the expression
							e.calc(ctx, *arrayValue, EXPRESSION_FLAG_STORE);
							if (flags == EXPRESSION_FLAG_STORE) {
								if (&retvalue != arrayValue) {
									// Dont assign myself (problems with arrays!)
									if (retvalue.datatype >= DataValue::DATATYPE_ARRAY)
										retvalue = DataValue(*arrayValue); // Overwrite array, duplicate value before because value could be a child of destination array
									else
										retvalue = *arrayValue;
								}
							}
						}
					}
					arrayValue->refPnt = arrayValue;
					return arrayValue;
				}
				return retValuePnt;
			}
			else {
				// expression = array[x]
				int j = ctx.funcDeep;
				DataValue *arrayValue = NULL;
				unsigned int rootFlags = 0;
				for (j = ctx.funcDeep; j >= 0; j -= ctx.funcDeep) {
					arrayValue = ctx.symbols[j]->getPnt(operation.c_str());
					if (arrayValue == NULL) {
						if (j == 0)
							break;
						continue;
					}

					rootFlags = arrayValue->flags;
					if (arrayValue->refPnt && (arrayValue->flags & DataValue::FLAGS_USEREF)) {
						arrayValue = arrayValue->refPnt;
					}
					if (arrayValue->datatype < DataValue::DATATYPE_ARRAY) {
						arrayValue = NULL;
						break; // No array
					}
					break;
				}

				if (!arrayValue) {
					ctx.line = this->line; // Refresh the line
					ctx.abortInterprete(WFormattedString("Array %s is not defined", operation.c_str()));
					return retValuePnt;
				}

				unsigned int indexCnt = arguments.size();
				for (unsigned int i = 0; i < arguments.size(); i++) {
					DataValue index;
					DataValue *indexPnt = arguments[i].calc(ctx, index);
					indexPnt->toString();

					arrayValue = arrayValue->arrayList.getPnt(indexPnt->value.c_str());
					if (arrayValue == NULL) {
						// Index not found, return empty value
						retvalue = DataValue();
						return retValuePnt;
					}
				}
				arrayValue->refPnt = arrayValue;
				if (rootFlags & DataValue::FLAGS_DUPARG) {
					retvalue = *arrayValue;
					retvalue.flags |= DataValue::FLAGS_DUPARG;
					return &retvalue;
				}

				if (flags == EXPRESSION_FLAG_STORE) {
					if (&retvalue != arrayValue) {
						// Dont assign myself (problems with arrays!)
						if (retvalue.datatype >= DataValue::DATATYPE_ARRAY)
							retvalue = DataValue(*arrayValue); // Overwrite array, duplicate value before because value could be a child of destination array
						else
							retvalue = *arrayValue;
					}
				}
				return arrayValue;
			}
		}

		retvalue = DataValue();
		return retValuePnt;
	}

	//////////////////////////////////
	// Assignment
	//////////////////////////////////
	if (_opCode == OPCODE_EQUAL) {
		if (datatype == -1) {
			// Constant definition
			DataValue *constPnt = ctx.symbols[0]->getPnt(constvalue->c_str());
			if (!constPnt) {
				// New constant
				retValuePnt = &ctx.symbols[0]->put(constvalue->c_str())->m_value;
				arguments[0].calc(ctx, *retValuePnt, EXPRESSION_FLAG_STORE);
				retValuePnt->refPnt = retValuePnt;
				retValuePnt->flags |= DataValue::FLAGS_NOWRITE;
			}
			else {
				// Redefinition not allowed
				ctx.warnInterprete(WFormattedString("Redefinition of constant %s not allowed", (WCSTR)constvalue->c_str()));
			}
		}
		else if (!ctx.symbols[0]->isKey(constvalue->c_str())) {
			// Variable
			retValuePnt = ctx.symbols[ctx.funcDeep]->getPnt(constvalue->c_str());
			if (retValuePnt) {
				// Overwrite variable
				if (retValuePnt->refPnt && (retValuePnt->flags & DataValue::FLAGS_USEREF))
					retValuePnt = retValuePnt->refPnt;
				arguments[0].calc(ctx, *retValuePnt, EXPRESSION_FLAG_STORE);
			}
			else {
				// New variable
				retValuePnt = &ctx.symbols[ctx.funcDeep]->put(constvalue->c_str())->m_value;
				arguments[0].calc(ctx, *retValuePnt, EXPRESSION_FLAG_STORE);
			}
			retValuePnt->refPnt = retValuePnt;
			if (flags == EXPRESSION_FLAG_STORE) {
				if (&retvalue != retValuePnt) {
					// Dont assign myself (problems with arrays!)
					if (retvalue.datatype >= DataValue::DATATYPE_ARRAY)
						retvalue = DataValue(*retValuePnt); // Overwrite array, duplicate value before because value could be a child of destination array
					else
						retvalue = *retValuePnt;
				}
			}
		}
		else {
			ctx.abortInterprete(WFormattedString("Illegal overwriting of %s", (WCSTR)constvalue->c_str()));
		}
		return retValuePnt;
	}

	//////////////////////////////////
	// Get Variables
	//////////////////////////////////
	if (_opCode == OPCODE_VARIABLE) {
		retValuePnt = ctx.symbols[ctx.funcDeep]->getPnt(operation.c_str());
		if (!retValuePnt) {
			retValuePnt = ctx.symbols[0]->getPnt(operation.c_str());
		}

		if (!retValuePnt) {
			ctx.line = this->line; // Refresh the line
			ctx.abortInterprete(WFormattedString("Variable %s is not defined", operation.c_str()));
			return retValuePnt;
		}
		if (retValuePnt->refPnt && (retValuePnt->flags & DataValue::FLAGS_USEREF)) {
			retValuePnt = retValuePnt->refPnt;
			retValuePnt->flags |= DataValue::FLAGS_USEREF;
		}
		retValuePnt->refPnt = retValuePnt;
		if (flags == EXPRESSION_FLAG_STORE) {
			if (&retvalue != retValuePnt) {
				// Dont assign myself (problems with arrays!)
				if (retvalue.datatype >= DataValue::DATATYPE_ARRAY)
					retvalue = DataValue(*retValuePnt); // Overwrite array, duplicate value before because value could be a child of destination array
				else
					retvalue = *retValuePnt;
			}
		}
		return retValuePnt;
	}

	//////////////////////////////////
	// Syntax: if, while, switch ...
	//////////////////////////////////

	if (opCode & OPCODE_SYNTAX_OP) {
		switch (_opCode) {
			case OPCODE_IF1: {
				/***********************************/
				// Handle the if statement
				DataValue tmpvalue;
				DataValue *check = arguments[0].calc(ctx, tmpvalue);
				if ((bool)*check) {
					for (unsigned int i = 0; i < arguments[1].arguments.size(); i++) {
						retValuePnt = arguments[1].arguments[i].calc(ctx, retvalue);
						if (ctx.flag == InterpreterContext::FLAG_RETURN)
							return retValuePnt;
						if (ctx.flag == InterpreterContext::FLAG_BREAK ||
						        ctx.flag == InterpreterContext::FLAG_CONTINUE) {
							break;
						}
					}
				}
				return retValuePnt;
			}
			break;

			case OPCODE_IF2: {
				/***********************************/
				// Handle the if/else statement
				DataValue tmpvalue;
				DataValue *check = arguments[0].calc(ctx, tmpvalue);
				if ((bool)*check) {
					for (unsigned int i = 0; i < arguments[1].arguments.size(); i++) {
						retValuePnt = arguments[1].arguments[i].calc(ctx, retvalue, flags);
						if (ctx.flag == InterpreterContext::FLAG_RETURN)
							return retValuePnt;
						if (ctx.flag == InterpreterContext::FLAG_BREAK ||
						        ctx.flag == InterpreterContext::FLAG_CONTINUE) {
							break;
						}
					}
				}
				else {
					for (unsigned int i = 0; i < arguments[2].arguments.size(); i++) {
						retValuePnt = arguments[2].arguments[i].calc(ctx, retvalue, flags);
						if (ctx.flag == InterpreterContext::FLAG_RETURN)
							return retValuePnt;
						if (ctx.flag == InterpreterContext::FLAG_BREAK || ctx.flag == InterpreterContext::FLAG_CONTINUE) {
							break;
						}
					}
				}
				return retValuePnt;
			}
			break;

			case OPCODE_FOR: {
				/***********************************/
				// Handle the for loop
				ctx.loopDeep++;
				DataValue f0, f1, f2;
				arguments[0].calc(ctx, f0);

				while (true) {
					DataValue *f1Pnt = arguments[1].calc(ctx, f1);
					if (!((bool)*f1Pnt))
						break;

					for (unsigned int i = 0; i < arguments[3].arguments.size(); i++) {
						retValuePnt = arguments[3].arguments[i].calc(ctx, retvalue);

						if (ctx.flag == InterpreterContext::FLAG_RETURN) {
							ctx.loopDeep--;
							return retValuePnt;
						}

						if (ctx.flag == InterpreterContext::FLAG_BREAK ||
						        ctx.flag == InterpreterContext::FLAG_CONTINUE) {
							break;
						}
					}
					if (ctx.flag == InterpreterContext::FLAG_CONTINUE) {
						ctx.flag = 0;
						arguments[2].calc(ctx, f2);
						continue;
					}

					if (ctx.flag == InterpreterContext::FLAG_BREAK) {
						ctx.flag = 0;
						break;
					}
					arguments[2].calc(ctx, f2);
				}
				ctx.loopDeep--;
				return retValuePnt;
			}
			break;

			case OPCODE_FOREACH: {

				/***********************************/
				// Handle the foreach loop
				DataValue *f0 = NULL, *f1 = NULL, *f2 = NULL;

				unsigned int loopIdx = 3;
				unsigned int argOffs = 0;
				unsigned int flags0;

				// Copy the array list
				ArrayHT arrayDupList;
				// f0->arrayList
				{
					f0 = getVariable(arguments[0].arguments, ctx, false, argOffs, &flags0);

					if (arguments.size() == 3) {
						loopIdx = 2;
						argOffs = 0;
						f2 = getVariable(arguments[1].arguments, ctx, true, argOffs);
					}
					else {
						argOffs = 0;
						f1 = getVariable(arguments[1].arguments, ctx, true, argOffs);

						argOffs = 0;
						f2 = getVariable(arguments[2].arguments, ctx, true, argOffs);
					}

					if (f0 == f1 || f0 == f2) {
						ctx.line = this->line; // Refresh the line
						ctx.abortInterprete(WFormattedString("Source overwritten in foreach()"));
						return retValuePnt;
					}

#ifdef _V1_THREADSAFE
					if (flags0 & DataValue::FLAGS_SYNCHRONIZE)
						WSystem::enterMutex(ctx.mutex);
#endif
					// Copy foreach array thread safe
					f0->arrayList.reset();
					arrayDupList = f0->arrayList;
#ifdef _V1_THREADSAFE
					if (flags0 & DataValue::FLAGS_SYNCHRONIZE)
						WSystem::leaveMutex(ctx.mutex);
#endif
				}

				ctx.loopDeep++;

				while (true) {

					if (!arrayDupList.hasMoreElements())
						break;

					// Set Key
					if (f1) {
						*f1 = (string)arrayDupList.nextKey();
					}

					// Set value
					*f2 = arrayDupList.nextValue();

					for (unsigned int i = 0; i < arguments[loopIdx].arguments.size(); i++) {
						retValuePnt = arguments[loopIdx].arguments[i].calc(ctx, retvalue);

						if (ctx.flag == InterpreterContext::FLAG_RETURN) {
							ctx.loopDeep--;
							return retValuePnt;
						}

						if (ctx.flag == InterpreterContext::FLAG_BREAK ||
						        ctx.flag == InterpreterContext::FLAG_CONTINUE) {
							break;
						}
					}
					if (ctx.flag == InterpreterContext::FLAG_CONTINUE) {
						ctx.flag = 0;
						continue;
					}

					if (ctx.flag == InterpreterContext::FLAG_BREAK) {
						ctx.flag = 0;
						break;
					}
				}
				ctx.loopDeep--;
				return retValuePnt;
			}
			break;

			case OPCODE_ASSIGN: {
				retValuePnt = arguments[1].calc(ctx, retvalue);
				assignVariables(arguments[0].arguments, *retValuePnt, ctx);
				return retValuePnt;
			}
			break;

			case OPCODE_FUNCDEF: {
				/***********************************/
				// Check if we are looking at a function declaration
				string funcname = arguments[0].operation;
				if (ctx.funcDeep > 1) {
					ctx.line = this->line; // Refresh the line
					ctx.abortInterprete(WFormattedString("Cannot declare function %s() inside another function", funcname.c_str()));
				}
				if (ctx.nativeFunctionHT.getPnt(funcname.c_str())) {
					ctx.line = this->line; // Refresh the line
					ctx.abortInterprete(WFormattedString("Cannot redeclare internal function %s()", funcname.c_str()));
					return retValuePnt;
				}
				unsigned int *line2 = ctx.locationHT.getPnt(funcname.c_str());
				if (line2 != NULL) {
					arguments.freeExpressions(); // Delete expression here manually, because automatic cleaning is not possible because of same function name
					ctx.line = this->line;       // Refresh the line
					ctx.abortInterprete(WFormattedString("Cannot redeclare function %s() previously declared in %s (%u)", funcname.c_str(), ctx.getFilenameByIdx(WSCRIPT_FILE(*line2)), WSCRIPT_LINE(*line2)));
					return retValuePnt;
				}
				ctx.locationHT.put(funcname.c_str(), this->line);
				ctx.functions.put(funcname.c_str(), arguments);
				retvalue = DataValue(funcname);
				this->datatype = 1; // Mark function as registered for automatically cleaning
				return retValuePnt;
			}
			break;

			case OPCODE_WHILE: {
				/***********************************/
				// Handle the while loop
				ctx.loopDeep++;

				DataValue tmpvalue;
				do {
					// Calc the decision
					// DataValue* check = arguments[0].calc (ctx, tmpvalue);
					DataValue *check;
					for (unsigned int i = 0; i < arguments[0].arguments.size(); i++) {
						check = arguments[0].arguments[i].calc(ctx, tmpvalue);
					}
					if (check->datatype == DataValue::DATATYPE_STR)
						check->setAutoDataType(true); // Maybe a number

					if (!(bool)*check)
						break;

					for (unsigned int i = 0; i < arguments[1].arguments.size(); i++) {
						retValuePnt = arguments[1].arguments[i].calc(ctx, retvalue);

						if (ctx.flag == InterpreterContext::FLAG_RETURN) {
							ctx.loopDeep--;
							return retValuePnt;
						}

						if (ctx.flag == InterpreterContext::FLAG_BREAK ||
						        ctx.flag == InterpreterContext::FLAG_CONTINUE) {
							break;
						}
					}
					if (ctx.flag == InterpreterContext::FLAG_CONTINUE) {
						ctx.flag = 0;
						continue;
					}

					if (ctx.flag == InterpreterContext::FLAG_BREAK) {
						ctx.flag = 0;
						break;
					}

				}
				while (true);
				ctx.loopDeep--;
				return retValuePnt;
			}
			break;

			case OPCODE_DO_WHILE: {
				/***********************************/
				// Handle the do while loop
				ctx.loopDeep++;

				DataValue tmpvalue, *check;
				do {

					for (unsigned int i = 0; i < arguments[1].arguments.size(); i++) {
						retValuePnt = arguments[1].arguments[i].calc(ctx, retvalue);

						if (ctx.flag == InterpreterContext::FLAG_RETURN) {
							ctx.loopDeep--;
							return retValuePnt;
						}

						if (ctx.flag == InterpreterContext::FLAG_BREAK ||
						        ctx.flag == InterpreterContext::FLAG_CONTINUE) {
							break;
						}
					}

					if (ctx.flag == InterpreterContext::FLAG_CONTINUE) {
						ctx.flag = 0;
						goto doWhileCheck;
					}

					if (ctx.flag == InterpreterContext::FLAG_BREAK) {
						ctx.flag = 0;
						break;
					}
doWhileCheck:
					check = arguments[0].calc(ctx, tmpvalue);
				}
				while ((bool)*check);

				ctx.loopDeep--;
				return retValuePnt;
			}
			break;

			case OPCODE_SWITCH: {
				/***********************************/
				// Handle the switch case
				ctx.switchDeep++;
				DataValue checkvalue, checkvalue2, *check, *check2;
				check = arguments[0].calc(ctx, checkvalue);

				bool fFound = false;

				for (unsigned int i = 0; i < arguments[1].arguments.size(); i++) {
					Expression *e = &arguments[1].arguments[i];
					if (e->opCode == OPCODE_CASE) {
						check2 = e->arguments[0].calc(ctx, checkvalue2);
						if (*check == *check2) {
							fFound = true;
						}
					}

					if (fFound) {
						// Handle statements
						for (unsigned int j = 0; j < e->arguments[1].arguments.size(); j++) {
							Expression *f = &e->arguments[1].arguments[j];

							retValuePnt = f->calc(ctx, retvalue);

							if (ctx.flag == InterpreterContext::FLAG_RETURN) {
								ctx.switchDeep--;
								return retValuePnt;
							}

							if (ctx.flag == InterpreterContext::FLAG_BREAK) {
								break;
							}
						}
					}

					if (ctx.flag == InterpreterContext::FLAG_BREAK) {
						ctx.flag = 0;
						break;
					}
				}
				if (!fFound) {
					// Handle default
					for (unsigned int i = 0; i < arguments[1].arguments.size(); i++) {
						Expression *e = &arguments[1].arguments[i];
						if (e->opCode == OPCODE_DEFAULT) {
							fFound = true;
						}
						if (fFound) {
							// Handle statements

							for (unsigned int j = 0; j < e->arguments[1].arguments.size(); j++) {
								Expression *f = &e->arguments[1].arguments[j];

								retValuePnt = f->calc(ctx, retvalue);

								if (ctx.flag == InterpreterContext::FLAG_RETURN) {
									ctx.switchDeep--;
									return retValuePnt;
								}

								if (ctx.flag == InterpreterContext::FLAG_BREAK) {
									break;
								}
							}
						}

						if (ctx.flag == InterpreterContext::FLAG_BREAK) {
							ctx.flag = 0;
							break;
						}
					}
				}
				ctx.switchDeep--;
				return retValuePnt;
			}
			break;
		}
	}

	if (opCode == 0) {
		// Empty statement
		retvalue = DataValue();
		return retValuePnt;
	}

	//////////////////////////////////
	// Functions
	//////////////////////////////////

function:
#ifdef _LOG_PERFORMANCE
	W64 counterBefore;
	WSystem::getPerformanceCounter(counterBefore);
#endif

	if (opCode == (OPCODE_EQUAL_OP | OPCODE_LOOP | OPCODE_FUNCTION)) {
		// Simple loop

		for (unsigned int i = 0; i < arguments.size(); i++) {
			retValuePnt = arguments[i].calc(ctx, retvalue);
		}
		if (retValuePnt->datatype == DataValue::DATATYPE_STR)
			retValuePnt->setAutoDataType(true); // Maybe a number
		return retValuePnt;
	}

	DataValue *variablePnt = NULL;

	if (opCode & OPCODE_MAGIC_FUNC) {
		// Buildin magic functions
		if (_opCode == OPCODE_GLOBAL) {
			// Init global variables
			for (unsigned int i = 0; i < arguments.size(); i++) {
				const char *key = arguments[i].operation.c_str();
				DataValue *tmp;
				if (tmp = ctx.symbols[1]->getPnt(key)) {
					DataValue v;
					v.flags = tmp->flags | DataValue::FLAGS_USEREF;
					if (ctx.currThread)
						v.flags |= DataValue::FLAGS_DUPARG | DataValue::FLAGS_SYNCHRONIZE; // In threads always copy global variables to function stack and synchronize access
					v.datatype = tmp->datatype;
					v.refPnt = tmp->refPnt;
					ctx.symbols[ctx.funcDeep]->put(key, v);
				}
			}
			return retValuePnt;
		}
		else {
			if (_opCode == OPCODE_ISSET)
				retvalue.numvalue = arguments.size();

			for (unsigned int i = 0; i < arguments.size(); i++) {
				Expression &e = arguments[i];
				if (e.datatype >= DataValue::DATATYPE_ARRAY) {
					// Unset/Isset array
					for (int funcDeep = ctx.funcDeep; funcDeep >= 0; funcDeep -= ctx.funcDeep) {
						variablePnt = ctx.symbols[funcDeep]->getPnt(e.operation.c_str());
						unsigned int fSynchronize = 0;
						if (variablePnt) {
							fSynchronize = variablePnt->flags & DataValue::FLAGS_SYNCHRONIZE;
							DataValue index, *indexPnt;
							for (unsigned int j = 0; j < e.arguments.size(); j++) {
								if (j > 0) {
									DataValue *tmp = NULL;
									if ((variablePnt->flags & DataValue::FLAGS_USEREF) && variablePnt->refPnt)
										tmp = variablePnt->refPnt->arrayList.getPnt(indexPnt->value.c_str());
									else
										tmp = variablePnt->arrayList.getPnt(indexPnt->value.c_str());
									if (tmp && tmp->datatype >= DataValue::DATATYPE_ARRAY) {
										variablePnt = tmp;
									}
									else {
										// Not found!
										variablePnt = NULL;
										break;
									}
								}

								indexPnt = e.arguments[j].calc(ctx, index);
								indexPnt->toString();
							}
							if (variablePnt) {
								if (_opCode == OPCODE_UNSET) {
#ifdef _V1_THREADSAFE
									if (fSynchronize)
										WSystem::enterMutex(ctx.mutex);
#endif
									if (funcDeep >= 1) {
										variablePnt->arrayList.remove(indexPnt->value.c_str());

										if ((variablePnt->flags & DataValue::FLAGS_USEREF) && variablePnt->refPnt) {
											// Remove reference pointer from symbol tables
											DataValue *test;
											test = variablePnt->refPnt->arrayList.getPnt(indexPnt->value.c_str());
											if (test) {
												for (int k = ctx.funcDeep; k > 1; k--) {
													while (ctx.symbols[k]->hasMoreElements()) {
														DataValue *tmp = &ctx.symbols[k]->nextValue();
														if (tmp->refPnt == test) {
															*tmp = *test;
															tmp->refPnt = NULL;
														}
													}
												}
											}
											variablePnt->refPnt->arrayList.remove(indexPnt->value.c_str());
										}
									}
#ifdef _V1_THREADSAFE
									if (fSynchronize)
										WSystem::leaveMutex(ctx.mutex);
#endif
								}
								else {
									DataValue *test;
									if (variablePnt->refPnt && (variablePnt->flags & DataValue::FLAGS_USEREF))
										test = variablePnt->refPnt->arrayList.getPnt(indexPnt->value.c_str());
									else
										test = variablePnt->arrayList.getPnt(indexPnt->value.c_str());

									if (test && test->datatype != 0)
										retvalue.numvalue -= 1;
								}
							}
							break;
						}
					}
				}
				else {
					// Unset/Isset variable
					DataValue *test = NULL;
					test = ctx.symbols[ctx.funcDeep]->getPnt(e.operation.c_str());
					if (test) {
						if (_opCode == OPCODE_UNSET) {
#ifdef _V1_THREADSAFE
							if (test->flags & DataValue::FLAGS_SYNCHRONIZE)
								WSystem::enterMutex(ctx.mutex);
#endif

							ctx.symbols[ctx.funcDeep]->remove(e.operation.c_str());
#ifdef _V1_THREADSAFE
							if (test->flags & DataValue::FLAGS_SYNCHRONIZE)
								WSystem::leaveMutex(ctx.mutex);
#endif
						}
						else {
							if (test->datatype != 0)
								retvalue.numvalue -= 1;
						}
					}
					else if (_opCode == OPCODE_ISSET) {
						test = ctx.symbols[0]->getPnt(e.operation.c_str());
						if (test && test->datatype != 0)
							retvalue.numvalue -= 1;
					}
				}
			}
			retvalue = (bool)(retvalue.numvalue == 0);
			return retValuePnt;
		}
	}

	vector<DataValue *> argvalues;
	vector<DataValue *> freetmpvalues;
	bool fEqualOp = (opCode & OPCODE_EQUAL_OP) > 0;
	bool fNoSecondCheck = false;
	int idx = 0, indexCnt = 0;

	for (unsigned int i = 0; i < arguments.size(); i++) {
		Expression &e = arguments[i];
		if (fEqualOp) {
			if (idx == 0) {
				if (e.datatype > DataValue::DATATYPE_ARRAY)
					indexCnt = e.datatype - DataValue::DATATYPE_ARRAY; // Indexes
				variablePnt = ctx.symbols[ctx.funcDeep]->getPnt(e.operation.c_str());
				if (!variablePnt) {
					if (ctx.symbols[0]->getPnt(e.operation.c_str()) != NULL) {
						ctx.abortInterprete(WFormattedString("Illegal overwriting of %s", (WCSTR)e.operation.c_str()));
						return retValuePnt;
					}
				}
				if (!variablePnt || (indexCnt > 0 && variablePnt->datatype < DataValue::DATATYPE_ARRAY)) {
					ctx.line = this->line; // Refresh the line
					ctx.abortInterprete(WFormattedString("%s %s is not defined", indexCnt > 0 ? "Array" : "Variable", e.operation.c_str()));
					return retValuePnt;
				}
				else if ((variablePnt->flags & DataValue::FLAGS_USEREF) && variablePnt->refPnt)
					variablePnt = variablePnt->refPnt;
				idx++;
				continue;
			}
			if (idx <= indexCnt) {
				// Index value
				if (variablePnt) {
					DataValue index, *indexPnt;
					indexPnt = e.calc(ctx, index);
					indexPnt->toString();
					if (!variablePnt->arrayList.isKey(indexPnt->value.c_str()) || variablePnt->datatype < DataValue::DATATYPE_ARRAY) {
						// Index not found, new entry
						variablePnt->datatype = DataValue::DATATYPE_ARRAY + (indexCnt - idx) + 1;
						DataValue empty;
						variablePnt = &variablePnt->arrayList.put(indexPnt->value.c_str(), empty)->m_value;
						variablePnt->refPnt = variablePnt;
					}
					else
						variablePnt = variablePnt->arrayList.getPnt(indexPnt->value.c_str());
				}
				idx++;
				continue;
			}
		}

		// Evaluate the arguments for the function
		DataValue *tmp;
		bool v;
		if (e.datatype != 1 && !fNoSecondCheck) {

			if (e.constDataValue) {
				// Argument is a constant
				tmp = e.constDataValue;
			}
			else if (e.opCode == OPCODE_VARIABLE) {
				// Argument is a single variable
				tmp = ctx.symbols[ctx.funcDeep]->getPnt(e.operation.c_str());
				if (!tmp) {
					tmp = ctx.symbols[0]->getPnt(e.operation.c_str());
				}
				if (!tmp) {
					ctx.line = this->line; // Refresh the line
					ctx.abortInterprete(WFormattedString("Variable %s is not defined", e.operation.c_str()));
					return retValuePnt;
				}
				if (tmp == &retvalue) {
					// Return value is same as argument, duplicate argument
					tmp = new DataValue(*tmp);
					tmp->refPnt = tmp;
					freetmpvalues.push_back(tmp);
				}
				else {
					if (tmp->refPnt && (tmp->flags & DataValue::FLAGS_USEREF)) {
						tmp = tmp->refPnt;
						tmp->flags |= DataValue::FLAGS_USEREF;
					}
					tmp->refPnt = tmp;
				}
			}
			else {
				// Argument is a array value or expression
				tmp = new DataValue();
				DataValue *directvaluePnt = e.calc(ctx, *tmp);
				;
				if (directvaluePnt != tmp) {
					// Argument comes from array value
					if (directvaluePnt == &retvalue) {
						// Return value is same as argument, duplicate argument
						*tmp = *directvaluePnt;
						freetmpvalues.push_back(tmp);
					}
					else {
						delete tmp;
						tmp = directvaluePnt;
					}
				}
				else {
					// Argument is a expression value
					tmp->refPnt = tmp;
					// tmp->flags |= DataValue::FLAGS_DUPARG;
					freetmpvalues.push_back(tmp);
				}
			}
			if (tmp->datatype == DataValue::DATATYPE_STR)
				tmp->setAutoDataType(true); // Maybe a number
			v = (bool)*tmp;
		}
		else {
			// False
			tmp = new DataValue();
			tmp->datatype = e.datatype;
			tmp->refPnt = tmp;
			freetmpvalues.push_back(tmp);
			v = false;
		}
		if ((!v && _opCode == OPCODE_BOOL_AND) || (v && _opCode == OPCODE_BOOL_OR)) {
			// Dont check second parameter on logical AND, OR if first parameter is false or true!
			fNoSecondCheck = true;
		}
		argvalues.push_back(tmp);
		idx++;
	}
#ifdef _LOG_PERFORMANCE
	WSystem::getPerformanceCounter(counter);
	tD = (double)(counter - counterBefore) / (double)g_timerFreq;
	g_performanceLogFile.writeln(WFormattedString("After create parameters: %.6f", tD));
	counterBefore = counter;
#endif

	// Build in functions
	if (!this->userFunc) {
		this->userFunc = ctx.nativeFunctionHT.getPnt(operation.c_str());
	}
	if (this->userFunc) {

		ctx.line = this->line; // Refresh the line, because argument calculation may set own line

		// Call the function
		int ret;
		double oldNumValue;
		try {
			if (fEqualOp) {
				if (variablePnt->datatype == DataValue::DATATYPE_STR)
					variablePnt->setAutoDataType(true); // Maybe a number
				oldNumValue = variablePnt->numvalue;
				ret = (*(WSCRIPT_NATIVE_FUNCTION *)this->userFunc)(argvalues, *variablePnt, ctx);
			}
			else {
				ret = (*(WSCRIPT_NATIVE_FUNCTION *)this->userFunc)(argvalues, retvalue, ctx);
			}
		}
		catch (AbortException &e) {
			if (freetmpvalues.size() > 0)
				freeArgValues(freetmpvalues);
			throw e;
		}

		if (ret != 0) {
			if (freetmpvalues.size() > 0)
				freeArgValues(freetmpvalues);

			int paramIdx = (ret >> 8) & 0x000000FF;
			int requiredTypeId = (ret & 0x000000FF);
			WCSTR requiredType = retValuePnt->describeDataType(requiredTypeId);

			if (paramIdx)
				ctx.abortInterprete(WFormattedString("%s() expects parameter %u as %s", (operation.c_str()[0] == '=' ? operation.c_str() + 1 : operation.c_str()), paramIdx, requiredType));
			else
				ctx.abortInterprete("Aborted"); // Aborted with return code WSCRIPT_RET_ABORT
			return &retvalue;
		}
		else if (fEqualOp) {
			if (freetmpvalues.size() > 0)
				freeArgValues(freetmpvalues);

			if (datatype < 0) {
				retvalue.datatype = DataValue::DATATYPE_NUM;
				retvalue.numvalue = oldNumValue; // Return last value, var++
				return &retvalue;
			}
			if (flags == EXPRESSION_FLAG_STORE) {
				retvalue = *variablePnt;
				return &retvalue;
			}
			return variablePnt;
		}
	}
	else {
		callUserFunc(operation, argvalues, retvalue, ctx);
	}

	if (freetmpvalues.size() > 0)
		freeArgValues(freetmpvalues);

	return &retvalue;
}
