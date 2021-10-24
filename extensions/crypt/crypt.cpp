/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: V1 Script
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifdef _WIN32
#include <windows.h>
#endif

#include "../../ast.h"
#include <math.h>

#include "hash/des.h"
#include "hash/sha256.h"
#include "hash/sha512.h"
#include "hash/md5.h"
#include "hash/sha1.h"
#include "hash/blowfish.h"

#ifdef _WIN32
#define DLL_EXPORT __declspec( dllexport)
#else
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#endif

// https://github.com/besser82/libxcrypt

WCSTR g_algoNames[] = { "md5", "sha1", "sha256", "sha512" };


#define ALGO_MD5     1
#define ALGO_SHA1    2
#define ALGO_SHA256  3
#define ALGO_SHA512  4

int g_handletype_HashContext = 10;

// Helper function convert  binary data to readable hex string
void bin2hex (W8* c, int len, string& ret)
{

    ret.reserve (len*2);
    ret.resize (0);

    while (len>0) {
        int v = (*c & 0xF0);
        v>>=4;
        if (v>=10)
            ret+= (char) (87+v);
        else
            ret+= (char) (48+v);
        v = (*c & 0x0F);
        if (v>=10)
            ret+= (char) (87+v);
        else
            ret+= (char) (48+v);
        len--;
        c++;
    }
}


class HashContext {

public:
    HashContext (int algo)
    {
        ctx = NULL;
        this->algo= algo;

        switch (this->algo) {
            case ALGO_MD5:
                this->ctx = md5_init ();
                break;
            case ALGO_SHA1:
                this->ctx = sha1_init ();
                break;
            case ALGO_SHA256:
                this->ctx = sha256_init ();
                break;
            case ALGO_SHA512:
                this->ctx = sha512_init ();
                break;
            default:
                throw WException ("Hash algorithm not supported", -1);
                break;
        };
        if (!this->ctx) {
            throw WException ("Initialization of hash algorithm failed", -1);
        }
    }

    ~HashContext ()
    {
        if (ctx) {
            free (ctx);
            ctx=NULL;
        }
    }
    void update (void* buf, unsigned int length)
    {
        switch (this->algo) {
            case ALGO_MD5:
                md5_update (this->ctx, buf, length);
                break;
            case ALGO_SHA1:
                sha1_update (this->ctx, buf, length);
                break;
            case ALGO_SHA256:
                sha256_update (this->ctx, buf, length);
                break;
            case ALGO_SHA512:
                sha512_update (this->ctx, buf, length);
                break;
        };

    }

    void finish (string& hash, bool fRaw=false)
    {
        hash="";
        int keyLen = 0;
        unsigned char bytes[64];
        memset (bytes, 0, sizeof (bytes));
        switch (this->algo) {
            case ALGO_MD5:
                md5_finish (this->ctx, bytes);
                this->ctx = NULL;
                keyLen = 16;
                break;
            case ALGO_SHA1:
                sha1_finish (this->ctx, bytes);
                this->ctx = NULL;
                keyLen = 20;
                break;
            case ALGO_SHA256:
                sha256_finish (this->ctx, bytes);
                this->ctx = NULL;
                keyLen = 32;
                break;
            case ALGO_SHA512:
                sha512_finish (this->ctx, bytes);
                this->ctx = NULL;
                keyLen = 64;
                break;
        };
        if (fRaw)
            hash.assign ((const char*) bytes, keyLen);
        else
            bin2hex (bytes, keyLen, hash);

    }

    void* ctx;
    int algo;
};

int freeCryptHandles (void* handle, int handletype)
{
    if (handletype==g_handletype_HashContext)
        delete (HashContext*) handle;
    return 0;
}


int func_crypt (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }
    WString salt (32);
    if (argvalues.size ()>1) {
        if (argvalues[1]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
        salt = (WCSTR) *argvalues[1];
    }

    if (salt.length ()==0) {
        // Create random salt for MD5 = default
        W64 counter;
#ifdef _WIN32
        QueryPerformanceCounter ((LARGE_INTEGER*) &counter);
#else
        WSystem::getPerformanceCounter (counter);
#endif
        srand (counter);

        salt.append ("$1$");
        for (int i=0; i<8; i++) {
            int start = (rand ()%3==1) ? 65 : 97;
            int max = 26;
            if (rand ()%3==2) {
                start = 48;
                max = 10;
            }
            salt.append (start+rand() % max);
        }
    }

    argvalues[0]->toString ();

    WString output;
    ret.datatype=DataValue::DATATYPE_STR;
    char method = 0;
    int saltLen = salt.length ();
    if (saltLen==1) {
        salt.append ("$");
    }
    else if (salt.charAt (0)=='$') {
        if (salt.charAt (2)=='$') {
            method = salt.charAt (1);
        }
        else if (saltLen<=2) {
            method = salt.charAt (1);
            salt.append ("$");
        }
        else if (salt.startsWith ("$2")) {
            // It may be blowfish
            if (saltLen>=6 && salt.charAt (3)=='$') {
                if (saltLen==6) {
                    salt.append ("$");
                    method = '2'; // Its blowfish!
                }
                else if (salt.charAt (6)=='$')
                    method = '2'; // Its blowfish!

                saltLen = salt.length ();
                if (method=='2' && saltLen<29) {
                    for (int i=0; i<=29-saltLen; i++)
                        salt.append ("$");
                }
            }
        }
    }

    bool retCode = false;
    if (method=='1') {
        // MD5
        retCode = md5 (argvalues[0]->value.c_str (), salt, output);
    }
    else if (method=='2') {
        // Blowfish
        retCode = blowfish (argvalues[0]->value.c_str (), salt, output);
    }
    else if (method=='5') {
        // SHA256
        retCode = sha256 (argvalues[0]->value.c_str (), salt, output);
    }
    else if (method=='6') {
        // SHA512
        retCode = sha512 (argvalues[0]->value.c_str (), salt, output);
    }
    else {
        // Standard DES
        retCode = des (argvalues[0]->value.c_str (), salt, output);
    }

    if (!retCode) {
        ctx._abortInterprete ("crypt() failed.", ctx);
    }

    ret.value.assign (output, output.length ());
    return 0;
}

int func_md5 (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }

    bool fBin = false;
    if (argvalues.size ()>1) {
        if (argvalues[1]->datatype>DataValue::DATATYPE_BOOL)
            return WSCRIPT_RET_PARAM2|WSCRIPT_RET_BOOL_REQUIRED;
        fBin = (bool) *argvalues[1];
    }
    W8 buf[16];
    md5_raw ((W8*) argvalues[0]->value.c_str (), argvalues[0]->value.length (), buf);
    ret.datatype=DataValue::DATATYPE_STR;
    if (!fBin) {
        bin2hex (buf, sizeof (buf), ret.value);
    }
    else {
        ret.value.assign ((const char*) buf, sizeof (buf));
    }
    return 0;
}


int func_sha1 (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }

    bool fBin = false;
    if (argvalues.size ()>1) {
        if (argvalues[1]->datatype>DataValue::DATATYPE_BOOL)
            return WSCRIPT_RET_PARAM2|WSCRIPT_RET_BOOL_REQUIRED;
        fBin = (bool) *argvalues[1];
    }
    W8 buf[20];
    sha1_raw ((W8*) argvalues[0]->value.c_str (), argvalues[0]->value.length (), buf);
    ret.datatype=DataValue::DATATYPE_STR;
    if (!fBin) {
        bin2hex (buf, sizeof (buf), ret.value);
    }
    else {
        ret.value.assign ((const char*) buf, sizeof (buf));
    }
    return 0;
}


int func_hash_init (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }
    argvalues[0]->toString ();

    Handle* h = new Handle;
    ret = (Handle*) NULL;
    try {
        int algo = 0;

        for (int i=0; i<sizeof (g_algoNames) / sizeof (WCSTR); i++) {
            if (argvalues[0]->value==g_algoNames[i]) {
                algo = i+1;
                break;
            }
        }
        if (algo==0)
            throw WException (WFormattedString ("Unsupported hash algorithm: %s", (WCSTR) argvalues[0]->value.c_str ()), -1);
        h->handle = (void*) new HashContext (algo);

        // Register handle
        h->handletype = g_handletype_HashContext;
        h->freeFunction = freeCryptHandles;
        ret = ctx.handleHT.put ((void*) h, h)->m_value;
    }
    catch (WException& e) {

        if (h->handle) {
            delete (HashContext*) h->handle;
            h->handle = NULL;
        }
        delete h;
        h = NULL;
        ctx._warnInterprete (e, ctx);
    }
    return 0;
}


int func_hash_update (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }
    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
    }

    argvalues[1]->toString ();


    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_HashContext) {
            HashContext* hashCtx = (HashContext*) h->handle;
            hashCtx->update ((void*) argvalues[1]->value.c_str (), argvalues[1]->value.size ());
            ret = true;
        }
        else {
            throw WException ("No valid hash context", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}

int func_hash_finish (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    bool fRaw = false;
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }
    if (argvalues.size ()>=2) {
        if (argvalues[1]->datatype>DataValue::DATATYPE_BOOL)
            return WSCRIPT_RET_PARAM2|WSCRIPT_RET_BOOL_REQUIRED;
        fRaw = (bool) *argvalues[1];
    }


    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_HashContext) {
            HashContext* hashCtx = (HashContext*) h->handle;

            hashCtx->finish (ret.value, fRaw);
            ret.datatype = DataValue::DATATYPE_STR;

            // Delete context
            try {
                delete hashCtx;
            }
            catch (WException) {
            }
            *(argvalues[0]->refPnt) = (Handle*) NULL;
            ctx.handleHT.remove (h);

        }
        else {
            throw WException ("No valid hash context", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}

int func_hash_file (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    bool fRaw = false;
    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }
    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
    }

    if (argvalues.size ()>=3) {
        if (argvalues[2]->datatype>DataValue::DATATYPE_BOOL)
            return WSCRIPT_RET_PARAM3|WSCRIPT_RET_BOOL_REQUIRED;
        fRaw = (bool) *argvalues[2];
    }
    argvalues[1]->toString ();

    ret = false;
    try {
        int algo = 0;

        for (int i=0; i<sizeof (g_algoNames) / sizeof (WCSTR); i++) {
            if (argvalues[0]->value==g_algoNames[i]) {
                algo = i+1;
                break;
            }
        }
        if (algo==0)
            throw WException (WFormattedString ("Unsupported hash algorithm: %s", (WCSTR) argvalues[0]->value.c_str ()), -1);
        HashContext h (algo);
        W8 buf[0xFFFF];


#ifdef _WIN32
        WCHAR wFilename[WMAX_PATH];
        int size = 0;
        if (!(size=MultiByteToWideChar(CP_UTF8, 0, (WCSTR)  argvalues[1]->value.c_str (),  argvalues[1]->value.length (), wFilename, WMAX_PATH-1)))
            throw WException ("Invalid UTF8 characters in filename", -1);
        wFilename[size]=0;

        HANDLE hFile;
        if (INVALID_HANDLE_VALUE == (hFile = CreateFileW (wFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL))) {
            throw WException (WFormattedString ("Cannot open file: %s", (WCSTR) argvalues[1]->value.c_str ()), -1);
        }
        DWORD bytesRead=0;
        while (ReadFile (hFile, buf, sizeof (buf),  &bytesRead, NULL) && bytesRead) {
            h.update (buf, bytesRead);
        }
        CloseHandle (hFile);
#else
        FILE *fp = fopen (argvalues[1]->value.c_str (), "rb");
        if (!fp) {
            throw WException (WFormattedString ("Cannot open file: %s", (WCSTR) argvalues[1]->value.c_str ()), -1);
        }
        unsigned int bytesRead;
        while (bytesRead = fread (buf,  1, sizeof (buf), fp)) {
            h.update (buf, bytesRead);
        }
        fclose (fp);
#endif
        h.finish (ret.value, fRaw);
        ret.datatype = DataValue::DATATYPE_STR;
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);
    }

    return 0;
}



int func_hash_algos (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{


    ret = false;
    ret.datatype=DataValue::DATATYPE_ARRAY;

    for (int i=0; i<sizeof (g_algoNames) / sizeof (WCSTR); i++) {
        DataValue v = DataValue (g_algoNames[i]);
        ret.arrayList.push (v);
    }
    return 0;
}

WScriptFuncDef g_funcDefList[] = {
    // Functions
    { "crypt",  func_crypt },
    { "md5",  func_md5 },
    { "sha1",  func_sha1 },
    { "hash_init",  func_hash_init },
    { "hash_update",  func_hash_update },
    { "hash_finish",  func_hash_finish },
    { "hash_file",  func_hash_file },
    { "hash_algos",  func_hash_algos }

};

extern "C" {
    int DLL_EXPORT wScriptDLLStartup (InterpreterContext& ctx)
    {
        for (int i=0; i<sizeof (g_funcDefList) / sizeof (WScriptFuncDef); i++)
            ctx.nativeFunctionHT.put (g_funcDefList[i].name, (WSCRIPT_NATIVE_FUNCTION) g_funcDefList[i].func);
        return 0;
    }

    int DLL_EXPORT wScriptDLLCleanupAll (InterpreterContext& ctx)
    {
        return 0;
    }

    int DLL_EXPORT wScriptDLLShutdown (InterpreterContext& ctx)
    {
        for (int i=0; i<sizeof (g_funcDefList) / sizeof (WScriptFuncDef); i++)
            ctx.nativeFunctionHT.remove (g_funcDefList[i].name);
        return 0;
    }

    int DLL_EXPORT wScriptDLLVersion ()
    {
        return (int) (WSCRIPT_VERSION * 100);
    }
}

#ifdef _WIN32
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  dwReason,
                       LPVOID lpReserved)
{
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
#endif
