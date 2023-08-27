#ifdef _WIN32
#include <windows.h>
#endif 
#include "openssl-1.0.2u/include/openssl/conf.h"
#include "openssl-1.0.2u/include/openssl/crypto.h"
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include "../../ast.h"

using namespace std;

#ifdef _WIN32
#define DLL_EXPORT __declspec( dllexport)
#else
#include <signal.h>
#define DLL_EXPORT 
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#define SOCKET int
#endif

WMUTEX* g_mutexList = NULL;
int g_handletype_SSL_CTX = 12;
int g_handletype_SSL = 13;

typedef struct {
	WCSTR name;
	W32 flag;
} SFlagCfg;

SFlagCfg g_flagList[] = {
	{ "SSL_OP_LEGACY_SERVER_CONNECT", SSL_OP_LEGACY_SERVER_CONNECT },
#ifdef SSL_OP_TLSEXT_PADDING
	{ "SSL_OP_TLSEXT_PADDING", SSL_OP_TLSEXT_PADDING },
#endif
#ifdef SSL_OP_SAFARI_ECDHE_ECDSA_BUG
	{ "SSL_OP_SAFARI_ECDHE_ECDSA_BUG", SSL_OP_SAFARI_ECDHE_ECDSA_BUG },
#endif
	{ "SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS", SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS },
	{ "SSL_OP_NO_QUERY_MTU", SSL_OP_NO_QUERY_MTU },
	{ "SSL_OP_COOKIE_EXCHANGE", SSL_OP_COOKIE_EXCHANGE },
	{ "SSL_OP_NO_TICKET", SSL_OP_NO_TICKET },
	{ "SSL_OP_CISCO_ANYCONNECT", SSL_OP_CISCO_ANYCONNECT },
	{ "SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION", SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION },
	{ "SSL_OP_NO_COMPRESSION", SSL_OP_NO_COMPRESSION },
	{ "SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION", SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION },
	{ "SSL_OP_CIPHER_SERVER_PREFERENCE", SSL_OP_CIPHER_SERVER_PREFERENCE },
	{ "SSL_OP_TLS_ROLLBACK_BUG", SSL_OP_TLS_ROLLBACK_BUG },
	{ "SSL_OP_NO_SSLv3", SSL_OP_NO_SSLv3 },
	{ "SSL_OP_NO_TLSv1", SSL_OP_NO_TLSv1 },
	{ "SSL_OP_NO_TLSv1_2",  SSL_OP_NO_TLSv1_2 },
	{ "SSL_OP_NO_TLSv1_1", SSL_OP_NO_TLSv1_1 },
#ifdef SSL_OP_NO_DTLSv1
	{ "SSL_OP_NO_DTLSv1", SSL_OP_NO_DTLSv1 },
#endif
#ifdef SSL_OP_NO_DTLSv1_2
	{ "SSL_OP_NO_DTLSv1_2", SSL_OP_NO_DTLSv1_2 },
#endif
#ifdef SSL_OP_NO_SSL_MASK
	{ "SSL_OP_NO_SSL_MASK", SSL_OP_NO_SSL_MASK },
#endif
#ifdef SSL_OP_NO_DTLS_MASK    
	{ "SSL_OP_NO_DTLS_MASK", SSL_OP_NO_DTLS_MASK },
#endif
	{ "SSL_OP_CRYPTOPRO_TLSEXT_BUG", SSL_OP_CRYPTOPRO_TLSEXT_BUG },
	{ "SSL_OP_ALL",SSL_OP_ALL },
	{ "SSL_VERIFY_PEER", SSL_VERIFY_PEER },
	{ "SSL_VERIFY_FAIL_IF_NO_PEER_CERT", SSL_VERIFY_FAIL_IF_NO_PEER_CERT },
	{ "SSL_VERIFY_CLIENT_ONCE", SSL_VERIFY_CLIENT_ONCE },
};


class _SSL_CTX;
class _SSL;

int openssl_pem_password_cb(char *, int, int, void *);
int openssl_servername_cb(SSL *ssl, int *ad, void *arg);
void ERR_print_errors (WString& str);

#ifndef _WIN32
void My_X509_get0_signature(ASN1_BIT_STRING **psig, X509_ALGOR **palg, const X509 *x)
{
    if (psig)
        *psig = (ASN1_BIT_STRING *) &x->signature;
    if (palg)
        *palg = (X509_ALGOR *) &x->sig_alg;
}
#endif

void cleanupOpenSSL () {

    FIPS_mode_set(0);
    CRYPTO_set_locking_callback(NULL);
    CRYPTO_set_id_callback(NULL);
    ERR_remove_state (0);
    ERR_free_strings();
    EVP_cleanup();      
}


class _SSL_CTX 
{
public:

	_SSL_CTX (InterpreterContext& ctx) {
		initialize ();
		m_interpreterCtx = &ctx;
	}

	~_SSL_CTX () {
		close ();
	}

	void create (WCSTR method) {
		if (m_ctx)
			return;
		
		const SSL_METHOD *methodPnt = TLSv1_2_client_method ();
		
		if (!strcmp (method, "SSLv23_method"))
			methodPnt = SSLv23_method();
  	else
		if (!strcmp (method, "SSLv23_server_method")) 
			methodPnt = SSLv23_server_method();
    else
		if (!strcmp (method, "SSLv23_client_method"))
			methodPnt = SSLv23_client_method();
		else
		if (!strcmp (method, "TLSv1_1_method"))
			methodPnt = TLSv1_1_method();
    else
		if (!strcmp (method, "TLSv1_1_server_method"))
			methodPnt = TLSv1_1_server_method();
		else
		if (!strcmp (method, "TLSv1_1_client_method"))
			methodPnt = TLSv1_1_client_method();
		else
		if (!strcmp (method, "TLSv1_2_method"))
			methodPnt = TLSv1_2_method();
		else
		if (!strcmp (method, "TLSv1_2_server_method"))
			methodPnt = TLSv1_2_server_method();
    else
		if (!strcmp (method, "TLSv1_2_client_method"))
			methodPnt = TLSv1_2_client_method();
    else 
		if (method[0]!=0)
			throw WException ("SSL method unsupported", -1);

		m_ctx = SSL_CTX_new(methodPnt);  
		if ( m_ctx == NULL )
			throw WException ("Cannot create SSL Context", -1);
                                       
		SSL_CTX_set_default_passwd_cb (m_ctx, openssl_pem_password_cb);
		SSL_CTX_set_default_passwd_cb_userdata(m_ctx, this);
		
		SSL_CTX_set_tlsext_servername_callback (m_ctx, openssl_servername_cb);
		SSL_CTX_set_tlsext_servername_arg (m_ctx, this);	
	}
	
	void close () {
		if (m_ctx) {
			SSL_CTX_free(m_ctx);
			m_ctx = NULL;
		}
	}

	void loadCertificate (WCSTR pubCertFile, WCSTR keyFile, WCSTR keyPassword="") {
		if (!m_ctx)
			return;

		if ( SSL_CTX_use_certificate_file(m_ctx, pubCertFile, SSL_FILETYPE_PEM) !=1 ) {
			throw WException (WFormattedString ("Cannot read public certificate. %s", pubCertFile), ERR_get_error());
		}
		m_keyFile = keyFile;
		m_keyPassword = keyPassword;
		if ( SSL_CTX_use_PrivateKey_file(m_ctx, keyFile, SSL_FILETYPE_PEM) != 1 ) {
			throw WException (WFormattedString ("Cannot read private key. %s", keyFile), ERR_get_error());
		}
		if ( SSL_CTX_check_private_key(m_ctx)!=1 ) {
			throw WException (WFormattedString ("Private key %s does not match the public %s certificate.", keyFile, pubCertFile), ERR_get_error());
		}

	}

	bool setCipherList (WCSTR ciphers) {
		int err = 0; 
		if (m_ctx) {

			if ((err = SSL_CTX_set_cipher_list(m_ctx, ciphers))==0) {				
				WCSTR libErrStr = ERR_lib_error_string(ERR_get_error ());
				if (libErrStr) {					
					throw WException (WFormattedString ("Cannot set SSL ciphers. %s", libErrStr), ERR_get_error());
				}
				return false;
			}
			return true;
		}
		return false;
	}	
	
	void setServernameCallback (WCSTR callback) {
		m_servernameCallback = callback;
	}

	WCSTR getServernameCallback () const {
		return m_servernameCallback;
	}

	bool isServernameCallback () const {
		return m_servernameCallback.length ()>0;
	}

	void setVerifyCallback (WCSTR callback) {
		m_verifyCallback = callback;
	}

	void setKeyPassword (WCSTR password) {
		m_keyPassword = password;
	}

	WCSTR getVerifyCallback () const {
		return m_verifyCallback;
	}

	WCSTR getKeyPassword () const {
		return m_keyPassword;
	}

	InterpreterContext* getInterpreterContext () const {
		return m_interpreterCtx;
	}

    operator SSL_CTX* () const {
        return m_ctx;
    };

private:
	void initialize () {
		m_ctx = NULL;
		m_interpreterCtx = NULL;
	}
	SSL_CTX* m_ctx;
	WString m_keyFile;
	WString m_keyPassword;
	InterpreterContext* m_interpreterCtx;
	WString m_servernameCallback;
	WString m_verifyCallback;

};


class _SSL 
{
public:

	_SSL () {
		initialize ();
	}

	~_SSL () {
		close ();
	}

	void create (_SSL_CTX& ctx) {
		m_ctx = &ctx;
		if (m_ssl)
			return;
		m_ssl = SSL_new ((SSL_CTX*) ctx);

		if (!m_ssl) {
			WCSTR errStr = ERR_lib_error_string(ERR_get_error ());
			if (errStr)
				throw WException (WFormattedString ("Cannot create SSL handle. %s", errStr), ERR_get_error ());
		}

	}

	
	void close () {
		if (m_ssl) {
			SSL_set_shutdown (m_ssl, SSL_RECEIVED_SHUTDOWN); // SSL_SENT_SHUTDOWN
            SSL_shutdown (m_ssl); 
            SSL_clear (m_ssl);
			SSL_free (m_ssl);
			m_ssl = NULL;
		}
	}

	void setSocket (SOCKET hSocket, int mode=0) {
		if (!m_ssl)
			return;

		if (mode==1)
			SSL_set_accept_state (m_ssl);
		else
		if (mode==2)
			SSL_set_connect_state (m_ssl);
		SSL_set_fd (m_ssl, hSocket);
	}

	int connect (WCSTR hostname=NULL) {
		int err = 0;
		if (hostname)
			SSL_set_tlsext_host_name (m_ssl, hostname);
		if ((err = SSL_connect (m_ssl))==-1) {
			WCSTR errText = ERR_lib_error_string(ERR_get_error ());
			if (!errText)
				errText = "SSL connection failed";
			throw WException (errText, ERR_get_error ());
		}
		return err;
	}

	int read (void* pnt, unsigned int size) {
		int err = 0;
		if (m_ssl) {
			if ((err = SSL_read (m_ssl, pnt, size))==-1) {
				WCSTR errStr = ERR_lib_error_string(ERR_get_error ());
				if (errStr)
					throw WSocketException (WFormattedString ("Cannot read SSL data. %s", errStr), ERR_get_error ());
				else
					err=0;
			}
		}
		return err;
	}

	bool readln (string& str, int zeroStr=0)
	{
		str.clear ();

		char buf[1024], *pnt = buf;
		int  idx = 0;
		buf[1023] = 0; buf[0]= 0;
		int readCnt = 0;
		do 
		{
			readCnt = 0;
			do
			{
				readCnt= this->read (pnt, 1);
			} while (readCnt>0 && *pnt==0x0D);
			if (*pnt==0x0A) // new line
				break;

			pnt++; idx++;
			if (idx==1023) {
				str.append (buf, 1023);
				pnt = buf;
				idx = 0;
			}
		} while (readCnt>0);
		// append rest of line
		*pnt=0; str += buf;
		if (zeroStr)
			memset (buf, 0, sizeof (buf));
		return readCnt>0;
	}

	int write (const void* pnt, unsigned int size) {
		int err = 0;
		if (m_ssl) {
			if ( (err = SSL_write (m_ssl, pnt, size))==-1) {
				WCSTR errStr = ERR_lib_error_string(ERR_get_error ());
				if (errStr)
					throw WSocketException (WFormattedString ("Cannot write SSL data. %s", errStr), ERR_get_error ());
			}
		}
		return err;
	}
	
	bool accept () {
		int err = 0; 
		if (m_ssl) {
			if ((err = SSL_accept(m_ssl))==-1) {
				ERR_print_errors (m_openSSLWarning);
				WCSTR libErrStr = ERR_lib_error_string(ERR_get_error ());
        // printf ("SSL accept problems %s %x\r\n", libErrStr, ERR_get_error ());
				if (libErrStr)
					throw WException (WFormattedString ("Cannot accept SSL connection. %s %s", (libErrStr==NULL ? "" : libErrStr), (WCSTR) m_openSSLWarning), ERR_get_error ());			
				return false;
			}
			return true;
		}
		return false;
	}

	bool setCipherList (WCSTR ciphers) {
		int err = 0; 
		if (m_ssl) {

			if ((err = SSL_set_cipher_list(m_ssl, ciphers))==0) {				
				WCSTR libErrStr = ERR_lib_error_string(ERR_get_error ());
				if (libErrStr) {					
					throw WException (WFormattedString ("Cannot set SSL ciphers. %s", libErrStr), ERR_get_error());
				}
				return false;
			}
			return true;
		}
		return false;
	}

	operator SSL* () const {
	    return m_ssl;
	};

private:
	void initialize () {
		m_ssl = NULL;
		m_ctx = NULL;
	}
	SSL* m_ssl;
	_SSL_CTX* m_ctx;
	WString m_openSSLWarning;
};

// OpenSSL Callbacks ans special functions
int openssl_pem_password_cb(char *buf, int size, int rwflag, void *arg) {
	
	_SSL_CTX* ctx = (_SSL_CTX*) arg;
	if (ctx) {
		WString pwd = ctx->getKeyPassword ();
		strncpy (buf, (WCSTR) pwd, size);
		buf[size - 1] = '\0';
		return(strlen(buf));
	}
	return 0;
}


void openSSLLockFunction (int mode, int n, const char *file, int line)
{
	// printf ("openSSLLockFunction %i %s %i\r\n", mode, file, line);
    if (mode & CRYPTO_LOCK)
      WSystem::enterMutex (g_mutexList[n]);
    else
      WSystem::leaveMutex (g_mutexList[n]);
}

int openssl_servername_cb(SSL *ssl, int *ad, void *arg)
{
	if (ssl == NULL)
		return SSL_TLSEXT_ERR_NOACK;
	
	const char* servername = SSL_get_servername(ssl, TLSEXT_NAMETYPE_host_name);
	if (!servername || servername[0] == '\0')
		return SSL_TLSEXT_ERR_NOACK;

	_SSL_CTX* ctx = (_SSL_CTX*) arg;
	if (ctx) {
		
		// Get context for Servername
		InterpreterContext* interpreterCtx = ctx->getInterpreterContext ();
		if (interpreterCtx && ctx->isServernameCallback ()) {
			// Call user func
			DataValue ret, param;
			param = (WCSTR) servername;
			vector<DataValue*> argvalues;
			argvalues.push_back (&param);
			if (interpreterCtx->_callUserFunc (ctx->getServernameCallback (), argvalues, ret, *interpreterCtx, 0)==0) {				
				Handle* h = (Handle*) ret;
				if (h && interpreterCtx->handleHT.isKey (h)) {			
					if (h->handletype==g_handletype_SSL_CTX) {
						_SSL_CTX* ctx2 = (_SSL_CTX*) h->handle;		

						SSL_CTX* v = SSL_set_SSL_CTX(ssl,  (SSL_CTX*) *ctx2);
						if (v != (SSL_CTX*) *ctx2)   
							return SSL_TLSEXT_ERR_NOACK;
						return SSL_TLSEXT_ERR_OK;
					}
				}
			}
			
		}
		else {
			// Set default context
			SSL_CTX* v = SSL_set_SSL_CTX(ssl,  (SSL_CTX*) *ctx);
			if (v != (SSL_CTX*) *ctx)   
				return SSL_TLSEXT_ERR_NOACK;
			return SSL_TLSEXT_ERR_OK;
		}
	}
	return SSL_TLSEXT_ERR_NOACK;
}

int openssl_verify_cb (X509_STORE_CTX* x509_ctx, void *arg) {
	_SSL_CTX* ctx = (_SSL_CTX*) arg;
	int depth = X509_STORE_CTX_get_error_depth(x509_ctx);
	int err = X509_STORE_CTX_get_error(x509_ctx);
    
	X509* cert = X509_STORE_CTX_get_current_cert(x509_ctx);
	if (cert) {
		char buf[255];
		memset (buf, 0, sizeof (buf));
		char *subj = X509_NAME_oneline(X509_get_subject_name(cert), buf, sizeof(buf)-1);
		char *issuer = X509_NAME_oneline(X509_get_issuer_name(cert), buf, sizeof(buf)-1);
	}

	if (ctx) {

	}
	return 1; // 1 = verified
}

void ERR_print_errors (WString& str) {
	/*
	char path[MAX_PATH];
	GetCurrentDirectory (MAX_PATH-1, path);
	char szTempFileName[MAX_PATH];
	memset (szTempFileName, 0, sizeof (szTempFileName));
	if (!GetTempFileName(path, "ssl", 0, szTempFileName))
		return;

	freopen (szTempFileName,"w",stderr);
	ERR_print_errors_fp (stderr);
	fclose (stderr);

	WFile file;
	file.open (szTempFileName);
	file.readln (str);
	file.close ();
	DeleteFile (szTempFileName);
	*/
}


// Handle functions
int freeSSLHandle (void* handle, int handletype) {
	if (handletype==g_handletype_SSL_CTX)
		delete (_SSL_CTX*) handle;
	else
	if (handletype==g_handletype_SSL)
		delete (_SSL*) handle;
	return 0;
}

int func_SSL_CTX_new (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {	
	WCSTR method = "TLSv1_2_client_method";

	if (argvalues[0]->datatype!=DataValue::DATATYPE_NO_PARAM) {
		if (argvalues[0]->datatype!=DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;		
		method = argvalues[0]->value.c_str ();
	}

	Handle* h = new Handle;
	ret = (Handle*) NULL;
	try {

		h->handle = (void*) new _SSL_CTX (ctx);
		((_SSL_CTX*) h->handle)->create (method);

		// Register handle
		h->handletype = g_handletype_SSL_CTX;
		h->freeFunction = freeSSLHandle;
		ret = ctx.handleHT.put ((void*) h, h)->m_value;
	}
	catch (WException& e) {
		// ctx._warnInterprete (e, ctx);
		if (h->handle) {
			delete (_SSL_CTX*) h->handle;
			h->handle = NULL;
		}
		delete h;
		h = NULL;   
		ctx._abortInterprete (e.toString (), ctx);
	}
	return 0;
}

int func_SSL_CTX_free (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}
	
	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL_CTX) {			
			_SSL_CTX* obj = (_SSL_CTX*) h->handle;			
			obj->close ();
			try {
				delete h;
			}
			catch (WException) {
			}
			*(argvalues[0]->refPnt) = (Handle*) NULL;
			ctx.handleHT.remove (h);
			ret = true;
		}	
		else {
			throw WException ("No valid SSL context handle", -1);
		}
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
		ret = false;
	}
	return 0;
}


int func_SSL_CTX_load_cert (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}
	if (argvalues.size ()<2 || argvalues[1]->datatype!=DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;		
	}
	if (argvalues.size ()<3 || argvalues[2]->datatype!=DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM3|WSCRIPT_RET_STR_REQUIRED;		
	}
	WCSTR password="";
	if (argvalues.size ()>3) {
		if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM4|WSCRIPT_RET_STR_REQUIRED;		
		argvalues[3]->toString ();
		password = argvalues[3]->value.c_str ();
	}	

	ret = false;
	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL_CTX) {			
			_SSL_CTX* obj = (_SSL_CTX*) h->handle;			
				
			obj->loadCertificate (argvalues[1]->value.c_str (), argvalues[2]->value.c_str (), password);
			ret = true;				
		}
		else {
			throw WException ("No valid SSL context handle", -1);
		}
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
	}	
	return 0;
}



int func_SSL_new (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {	
	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}
	ret = (Handle*) NULL;
	_SSL_CTX* obj = NULL;

	Handle* h0 = (Handle*) *argvalues[0];
	if (h0 && ctx.handleHT.isKey (h0)) {			
		if (h0->handletype==g_handletype_SSL_CTX) {
			obj = (_SSL_CTX*) h0->handle;	
		}
	}
	if (!obj) {
		ctx._warnInterprete (WException ("No SSL Context given",-1), ctx);
    ret = false;
		return 0;
	}
	
	Handle* h = new Handle;

	try {

		h->handle = (void*) new _SSL ();		
		((_SSL*) h->handle)->create (*obj);
		// Register handle
		h->handletype = g_handletype_SSL;
		h->freeFunction = freeSSLHandle;
		ret = ctx.handleHT.put ((void*) h, h)->m_value;
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);

		if (h->handle) {
			delete (_SSL*) h->handle;
			h->handle = NULL;
		}
		delete h;
		h = NULL;
    ret = false;
	}
	return 0;
}

int func_SSL_free (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}
	ret = false;
	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h)) {			
			if (h->handletype==g_handletype_SSL) {
				_SSL* obj = (_SSL*) h->handle;			
				obj->close ();
				
				ret = true;
				try {
					delete h;
				}
				catch (WException) {
				}
				*(argvalues[0]->refPnt) = (Handle*) NULL;
				ctx.handleHT.remove (h);
			}			
		}		
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);

	}
	return 0;
}


int func_SSL_set_socket (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	if (argvalues[1]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	int mode = 0;
	if (argvalues.size ()>2) {
		if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;		
		mode = (int) *argvalues[0];
	}

	ret = false;
	SOCKET hSocket = -1; 

	Handle* h0 = (Handle*) *argvalues[1];
	if (h0 && ctx.handleHT.isKey (h0)) {			
		if (h0->handletype==Handle::HANDLETYPE_SOCKET ||
			h0->handletype==Handle::HANDLETYPE_SERVERSOCKET) {
			hSocket = (SOCKET) ((WSocket*) h0->handle)->getHandle ();
		}
	}
	int retCode = 0;

	if (hSocket==-1) {
		ctx._abortInterprete ("No valid socket", ctx);      
		return 0;
	}

	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;		
			obj->setSocket (hSocket, mode);				
            h->param = (void*) h0;
			h0->param = (void*) h;
			ret = true;				
		}
		else {
			throw WException ("No valid SSL handle", -1);
		}

	}
	catch (WException& e) {
		/*
		if (retCode==WSCRIPT_RET_ABORT)
			ctx._abortInterprete (e.getMessage (), ctx);
		*/
		ctx._warnInterprete (e, ctx);
		retCode=0;
	}	
	return retCode;
}


int func_SSL_read (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	if (argvalues.size ()<2|| argvalues[1]->datatype!=DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	}
	int sizeMax = 0xFFFF;
	if (argvalues.size ()>2) {
		if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
		sizeMax = argvalues[2]->numvalue;
	}

	WString str (4096);
	ret = false;
	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;							
			int bufSize = 4096;
			int bytesRead = 0;
			int size = 0;

			argvalues[1]->refPnt->datatype = DataValue::DATATYPE_STR;
			argvalues[1]->value.resize (0);
			if (sizeMax<bufSize)
				bufSize=sizeMax;
			while (sizeMax>0 && (size = obj->read (str.getBuf (), bufSize))) {
				argvalues[1]->refPnt->value.append (str, size);
				sizeMax-=size;
				bytesRead+=size;
				if (bufSize>sizeMax)
					bufSize=sizeMax;
			}
			ret = (double) bytesRead;			
		}
		else {
			throw WException ("No valid SSL handle", -1);
		}

	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
	}
	if (ctx.securityMode & 1) {
		memset (str.getBuf (), 0, str.getBufLength ());
	}  	
	return 0;
}



int func_SSL_readln (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	if (argvalues.size ()<2 || argvalues[1]->refPnt==NULL) {
		ctx._abortInterprete ("Parameter 2 must be a reference to string variable", ctx);
		return 0;
	}

	ret = false;
	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;			
			argvalues[1]->refPnt->datatype = DataValue::DATATYPE_STR;
			ret = obj->readln (argvalues[1]->refPnt->value, ctx.securityMode & 1);
		}
		else {
			throw WException ("No valid SSL handle", -1);
		}

	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
	}	
	return 0;
}


int func_SSL_gets (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	if (argvalues.size ()<2|| argvalues[1]->datatype!=DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	}

	ret = false;
	Handle* h = (Handle*) *argvalues[0];
	string tmp;
	tmp.reserve (256);
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;			
			argvalues[1]->refPnt->datatype = DataValue::DATATYPE_STR;
			if (obj->readln (tmp, ctx.securityMode & 1)) {
				ret.datatype = DataValue::DATATYPE_STR;
				ret.value=tmp;
			}
      else {
				ret = false;
      }
		}
		else {
			throw WException ("No valid SSL handle", -1);
		}
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
	}	
	if (ctx.securityMode & 1) {
		memset ((void*) tmp.c_str (), 0, tmp.capacity ());
	}
	return 0;
}


int func_SSL_write (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	}

	int size = -1;	
	if (argvalues.size ()>2) {
		if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
		size = (int) argvalues[2]->numvalue;
	}


	ret = false;

	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;							
			if (size>=0)
				obj->write (argvalues[1]->value.c_str (), size);			
			else
				obj->write (argvalues[1]->value.c_str (), argvalues[1]->value.length ());				
			ret = true;				
		}
		else {
			throw WException ("No valid SSL handle", -1);
		}

	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
	}	
	return 0;
}


int func_SSL_connect (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	WCSTR hostname = NULL;
	if (argvalues.size ()>1) {
		if (argvalues[1]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
		argvalues[1]->toString ();
		hostname = argvalues[1]->value.c_str ();
	}
	
	ret = false;

	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;											
			obj->connect (hostname);			
			ret = true;				
		}
		else {
			throw WException ("No valid SSL handle", -1);
		}
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
	}	
	return 0;
}

int func_SSL_accept (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	ret = false;

	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;																
			ret = obj->accept ();				
		}
		else {
			throw WException ("No valid SSL handle", -1);
		}
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
	}	

	return 0;
}




int func_SSL_CTX_universal (WCSTR function, vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	ret = false;
	int retCode = 0;
	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL_CTX) {			
			_SSL_CTX* obj = (_SSL_CTX*) h->handle;			
				
			if (!strcmp (function, "SSL_CTX_set_cipher_list")) {
				if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
				}
				argvalues[1]->toString ();
				retCode = WSCRIPT_RET_ABORT; // Abort interprete if function throw exception
				obj->setCipherList (argvalues[1]->value.c_str ());
				retCode = 0;
				ret = true;
			}
            else                
			if (!strcmp (function, "SSL_CTX_set_options")) {
				if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
				}	
				ret.datatype=1;
				ret.numvalue = (unsigned int) SSL_CTX_set_options ((SSL_CTX*) *obj, (unsigned int) argvalues[1]->numvalue);
			}
			else
			if (!strcmp (function, "SSL_CTX_clear_options")) {
				if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
				}	
				ret.datatype=1;
				ret.numvalue = (unsigned int) SSL_CTX_clear_options ((SSL_CTX*) *obj, (unsigned int) argvalues[1]->numvalue);
			}
			else
			if (!strcmp (function, "SSL_CTX_get_options")) {
					
				ret.datatype=1;
				ret.numvalue = (unsigned int) SSL_CTX_get_options ((SSL_CTX*) *obj);
			}
			else
			if (!strcmp (function, "SSL_CTX_set_tlsext_servername_callback")) {
				if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
				}	
				argvalues[1]->toString ();
				obj->setServernameCallback (argvalues[1]->value.c_str ());
				ret = true;
			}
			else
			if (!strcmp (function, "SSL_CTX_set_verify")) {
				if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
				}
				SSL_CTX_set_verify ((SSL_CTX*) *obj, (unsigned int) argvalues[1]->numvalue, NULL);
				/*
				// Experimental
				if (argvalues.size ()>2) {
					if ( argvalues[2]->datatype>DataValue::DATATYPE_STR)
						return WSCRIPT_RET_PARAM3|WSCRIPT_RET_STR_REQUIRED;
					argvalues[2]->toString ();
					obj->setVerifyCallback (argvalues[2]->value.c_str ());
					SSL_CTX_set_cert_verify_callback ((SSL_CTX*) *obj, openssl_verify_cb, obj);
				}
				*/
			}
			else
			if (!strcmp (function, "SSL_CTX_set_verify_depth")) {
				if (argvalues.size ()<1 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
				}	
				SSL_CTX_set_verify_depth ((SSL_CTX*) *obj, (int) argvalues[1]->numvalue);
				ret = true;
			}
			else
			if (!strcmp (function, "SSL_CTX_load_verify_locations")) {
				if (argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
				}	
				argvalues[1]->toString ();
				const char *CApath = NULL;
				if (argvalues.size ()>2) {
					if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
						return WSCRIPT_RET_PARAM3|WSCRIPT_RET_STR_REQUIRED;
					argvalues[2]->toString ();
					CApath = argvalues[2]->value.c_str ();
				}
				ret = (bool) (SSL_CTX_load_verify_locations((SSL_CTX*) *obj, argvalues[1]->value.c_str (), CApath)==1 ? true : false);
			}
			else
			if (!strcmp (function, "SSL_CTX_use_certificate_chain_file")) {
				if (argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
				}
				argvalues[1]->toString ();
				ret = (bool) (SSL_CTX_use_certificate_chain_file ((SSL_CTX*) *obj, argvalues[1]->value.c_str ()) == 1 ? true : false);
			}
			else
			if (!strcmp (function, "SSL_CTX_use_certificate_file")) {
				if (argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
				}
				int fileType  = SSL_FILETYPE_PEM; // X509_FILETYPE_ASN1 = 2
				argvalues[1]->toString ();
				if (argvalues.size ()>2) {
					if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
						return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
					fileType = (int) argvalues[2]->numvalue; 
				}
				ret = (bool) (SSL_CTX_use_certificate_file ((SSL_CTX*) *obj, argvalues[1]->value.c_str (), fileType) == 1 ? true : false);
			}
			else
			if (!strcmp (function, "SSL_CTX_use_PrivateKey_file")) {
				if (argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
				}
				int fileType  = SSL_FILETYPE_PEM; // X509_FILETYPE_ASN1 = 2
				argvalues[1]->toString ();
				if (argvalues.size ()>2) {
					if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
						return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
					fileType = (int) argvalues[2]->numvalue; 
				}

				if (argvalues.size ()>3) {
					if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
						return WSCRIPT_RET_PARAM4|WSCRIPT_RET_STR_REQUIRED;
					argvalues[3]->toString ();
					obj->setKeyPassword (argvalues[3]->value.c_str ());
				}

				ret = (bool) (SSL_CTX_use_PrivateKey_file ((SSL_CTX*) *obj, argvalues[1]->value.c_str (), fileType) == 1 ? true : false);
			}
			else
			if (!strcmp (function, "SSL_CTX_check_private_key")) {
				ret = (bool) (SSL_CTX_check_private_key ((SSL_CTX*) *obj) == 1 ? true : false);
			}
				
		}
		else {
			throw WException ("No valid SSL context handle", -1);
		}
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
	}	
	return retCode;
}


void setCertNameArray (ArrayHT* list, DataValue& tmp, WCSTR buf) {
	tmp.datatype=DataValue::DATATYPE_STR;						
	WString key;
	char* c= (char*) buf;
	char* c0 = c;
	while (*c!=0) {
		if (*c=='=') {
			key.assign (c0, c-c0);
			c0=c+1;
		}
		else 
		if (*c=='/') {
			if (key.length ()>0) {
				tmp.value.assign (c0, c-c0);
				list->put (key, tmp);
				key.truncate (0);
				tmp.value.clear ();
			}
			c0=c+1;
		}							
		c++;
	}
	if (key.length ()>0) {
		tmp.value.assign (c0, c-c0);
		list->put (key, tmp);
		key.truncate (0);
		tmp.value.clear ();
	}						
}


int func_SSL_universal (WCSTR function, vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	ret = false;
	int retCode = 0;
	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;																
				
			if (!strcmp (function, "SSL_set_cipher_list")) {
				if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
				}
				argvalues[1]->toString ();
				retCode = WSCRIPT_RET_ABORT; // Abort interprete if function throw exception
				obj->setCipherList (argvalues[1]->value.c_str ());
				retCode = 0;
				ret = true;
			}
			else
			if (!strcmp (function, "SSL_get_cipher_list")) {
				int priority = 0;
				if (argvalues.size ()>1) {
					if (argvalues[1]->datatype>DataValue::DATATYPE_STR)
						return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
					priority = (int) argvalues[1]->numvalue;
				}		
				ret.datatype=DataValue::DATATYPE_STR;				
				ret.value = SSL_get_cipher_list ((SSL*) *obj, priority); // Abort interprete if function throw exception
			}
			else					
			if (!strcmp (function, "SSL_get_peer_certificate")) {
				X509* cert = SSL_get_peer_certificate((SSL*) *obj);
				if(cert) {
					char buf[512];
					buf[sizeof(buf)-1]=0;

					ret.datatype=DataValue::DATATYPE_ARRAY;
					ret.arrayList.removeAll ();
					ret.arrayList.setOwnKeys (true);
					ArrayHT* list = NULL;
					DataValue tmp;

					tmp.datatype=DataValue::DATATYPE_NUM;
					ret.arrayList.put ("VERSION", tmp)->m_value.numvalue=X509_get_version (cert);
#ifdef _WIN32
					const X509_ALGOR* palg = NULL;
					const ASN1_BIT_STRING *psig = NULL;

					X509_get0_signature(&psig, &palg, cert);
					if (palg) {
						tmp.datatype=DataValue::DATATYPE_STR;		
						OBJ_obj2txt(buf, sizeof(buf)-1, palg->algorithm, 0);
						ret.arrayList.put ("SIG_TYPE", tmp)->m_value=(WCSTR) buf;
					}
#else
					X509_ALGOR* palg = NULL;
					ASN1_BIT_STRING *psig = NULL;

					My_X509_get0_signature(&psig, &palg, cert);
					if (palg) {
						tmp.datatype=DataValue::DATATYPE_STR;		
						int sig_nid = OBJ_obj2nid (cert->sig_alg->algorithm);
                        tmp.datatype =DataValue::DATATYPE_NUM;
						ret.arrayList.put ("SIG_TYPE_ID", tmp)->m_value.numvalue=(double) sig_nid;
                        tmp.datatype =DataValue::DATATYPE_STR;
                        ret.arrayList.put ("SIG_TYPE", tmp)->m_value=OBJ_nid2ln(sig_nid); 
					}
#endif

					ASN1_INTEGER* s = X509_get_serialNumber (cert);
					if (s) {
						char* c = buf;
						for (int i=0;i<s->length;i++) {	
							sprintf (c, "%.2X", s->data[i]);
							c+=2;
						}
						*c = 0;

						tmp.datatype = DataValue::DATATYPE_STR;
						ret.arrayList.put ("SERIAL", tmp)->m_value.value=(WCSTR) buf;
					}
												

					tmp.datatype=DataValue::DATATYPE_ARRAY;
					tmp.arrayList.setOwnKeys (true);
					list = &ret.arrayList.put ("SUBJECT", tmp)->m_value.arrayList;
					X509_NAME_oneline(X509_get_subject_name(cert), buf, sizeof(buf)-1);
					setCertNameArray (list, tmp, buf);

						
					X509_NAME_oneline(X509_get_issuer_name(cert), buf, sizeof(buf)-1);
					tmp.datatype=DataValue::DATATYPE_ARRAY;		
					tmp.arrayList.setOwnKeys (true);
					list = &ret.arrayList.put ("ISSUER", tmp)->m_value.arrayList;
					setCertNameArray (list, tmp, buf);
										
					tmp.datatype = DataValue::DATATYPE_ARRAY;
					tmp.arrayList.setOwnKeys (true);
					list = &ret.arrayList.put ("VALID", tmp)->m_value.arrayList;
						
					tmp.datatype =DataValue::DATATYPE_STR;
#ifdef _WIN32
					ASN1_TIME* t = X509_getm_notBefore (cert);
#else
					ASN1_TIME* t = cert->cert_info->validity->notBefore; 
#endif                     
					if (t) {
						tmp.value = (WCSTR) t->data;
						list->put ("FROM", tmp);
					}
#ifdef _WIN32
					t = X509_getm_notAfter (cert);
#else
					t = cert->cert_info->validity->notAfter;
#endif
                    if (t) {
						tmp.value =  (WCSTR) t->data;
						list->put ("TO", tmp);
					}

						
					
					X509_free(cert);						
				}					

			}
			else
			if (!strcmp (function, "SSL_set_verify_result")) {
				if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
					return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
				}
				SSL_set_verify_result ((SSL*) *obj, (int) argvalues[1]->numvalue); 
				ret = true;
			}
			else	
			if (!strcmp (function, "SSL_get_verify_result")) {
				ret = (int) SSL_get_verify_result ((SSL*) *obj); 
			}
		}		
		else {
			throw WException ("No valid SSL handle", -1);
		}
	}
	catch (WException& e) {
		if (retCode==WSCRIPT_RET_ABORT)
			ctx._abortInterprete (e.getMessage (), ctx);
		else
			ctx._warnInterprete (e, ctx);
		retCode=0;
	}	
	return retCode;
}


int func_SSL_CTX_set_tlsext_servername_callback (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_set_tlsext_servername_callback", argvalues, ret, ctx);
}


int func_SSL_CTX_set_options (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_set_options", argvalues, ret, ctx);
}

int func_SSL_CTX_clear_options (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_clear_options", argvalues, ret, ctx);
}


int func_SSL_CTX_get_options (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_get_options", argvalues, ret, ctx);
}

int func_SSL_CTX_set_verify (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_set_verify", argvalues, ret, ctx);
}

int func_SSL_CTX_set_cipher_list (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_set_cipher_list", argvalues, ret, ctx);
}

int func_SSL_CTX_set_verify_depth (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_set_verify_depth", argvalues, ret, ctx);
}


int func_SSL_CTX_load_verify_locations (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_load_verify_locations", argvalues, ret, ctx);
}

int func_SSL_CTX_use_certificate_chain_file (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_use_certificate_chain_file", argvalues, ret, ctx);
}

int func_SSL_CTX_use_certificate_file (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_use_certificate_file", argvalues, ret, ctx);
}

int func_SSL_CTX_use_PrivateKey_file (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_use_PrivateKey_file", argvalues, ret, ctx);
}

int func_SSL_CTX_check_private_key (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_CTX_universal ("SSL_CTX_check_private_key", argvalues, ret, ctx);
}


int func_SSL_set_cipher_list (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_universal ("SSL_set_cipher_list", argvalues, ret, ctx);
}

int func_SSL_get_cipher_list (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_universal ("SSL_get_cipher_list", argvalues, ret, ctx);
}

int func_SSL_get_peer_certificate (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_universal ("SSL_get_peer_certificate", argvalues, ret, ctx);
}

int func_SSL_set_verify_result (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_universal ("SSL_set_verify_result", argvalues, ret, ctx);
}

int func_SSL_get_verify_result (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	return func_SSL_universal ("SSL_get_verify_result", argvalues, ret, ctx);
}




int func_SSL_FLAG (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	
	if (argvalues[0]->datatype!=DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
	}

	unsigned int flag = 0;
	ret.datatype = 1;

	for (unsigned int i=0;i<sizeof (g_flagList) / sizeof (SFlagCfg);i++) {
		if (!strcmp (g_flagList[i].name, argvalues[0]->value.c_str ())) {
			flag = g_flagList[i].flag;
			break;
		}
	}

	ret.numvalue = (unsigned int) flag;

	return 0;
}

int func_ERR_get_error (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	ret.datatype=DataValue::DATATYPE_NUM;
	ret.numvalue = (unsigned int) ERR_get_error ();
	return 0;
}


int func_ERR_lib_error_string (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	if (PARAM1_NOSTR) { 
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_NUM_REQUIRED;
	}
	ret.datatype=DataValue::DATATYPE_STR;
	WCSTR errStr = ERR_lib_error_string ((unsigned int) argvalues[0]->numvalue);
	if (errStr)
		ret.value.assign (errStr);
	else
		ret.value.clear ();
	return 0;
}


int func_ERR_remove_state (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
    ERR_remove_state (0);
	return 0;
}


int func_SSL_create_selfsigned_cert  (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	if (PARAM1_NOSTR) { 
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size ()<1 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	}	
	if (argvalues.size ()<2 || argvalues[2]->datatype>DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM3|WSCRIPT_RET_STR_REQUIRED;
	}	
	argvalues[0]->toString ();
	argvalues[1]->toString ();
	argvalues[2]->toString ();

	ret = false;
	
	int bits = 2048;
	long long lifetime = 10*31536000;
	string sigTypeName = "sha256";
	if (argvalues.size ()>2) {
		if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
		lifetime = (long long) argvalues[3]->numvalue;
	}	
	if (argvalues.size ()>3) {
		if (argvalues[4]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM5|WSCRIPT_RET_STR_REQUIRED;
		argvalues[4]->toString ();
		sigTypeName = argvalues[4]->value;
	}	
	if (argvalues.size ()>4) {
		if (argvalues[5]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM5|WSCRIPT_RET_NUM_REQUIRED;
		bits = (int) argvalues[5]->numvalue;
	}	


	EVP_PKEY * pkey = EVP_PKEY_new();
	X509 * x509 = X509_new();
	RSA* rsa = RSA_generate_key(bits, RSA_F4, NULL, NULL);
	if (!pkey || !rsa || !x509) {
		WCSTR errStr = ERR_lib_error_string(ERR_get_error ());
		if (!errStr)
			errStr = "";
		ctx._warnInterprete (WException (WFormattedString ("Cannot create RSA key. %s", (WCSTR) errStr), ERR_get_error ()), ctx);
	}
	else {
		EVP_PKEY_assign_RSA(pkey, rsa);
		
		ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
		X509_gmtime_adj(X509_get_notBefore(x509), 0);
		X509_gmtime_adj(X509_get_notAfter(x509), lifetime);
		X509_set_pubkey(x509, pkey);
		X509_NAME * name;
		name = X509_get_subject_name(x509);	

#ifdef _WIN32
		char countryCode[8];
		GEOID myGEO = GetUserGeoID(GEOCLASS_NATION);
		memset (countryCode,0,sizeof (countryCode));
		GetGeoInfo(myGEO, GEO_ISO2, countryCode, sizeof(countryCode)-1, 0);
		if (countryCode[0]!=0)
			X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char*) countryCode, -1, -1, 0);
#else
		X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char*) "US", -1, -1, 0);  
#endif
		X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char*) (WCSTR) WFormattedString ("V1 Script %.2f", WSCRIPT_VERSION), -1, -1, 0);
		X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (const unsigned char*) argvalues[2]->value.c_str(), -1, -1, 0);
		X509_set_issuer_name(x509, name);

		const EVP_MD *sigType = EVP_sha256();

		if (sigTypeName=="md5")
			sigType = EVP_md5();
		else
		if (sigTypeName=="sha1")
			sigType = EVP_sha1();
		else
		if (sigTypeName=="mdc2")
			sigType = EVP_mdc2();
		else
		if (sigTypeName=="sha224")
			sigType = EVP_sha224();
		else
		if (sigTypeName=="sha256")
			sigType = EVP_sha256();
		else
		if (sigTypeName=="sha384")
			sigType = EVP_sha384();
		else
		if (sigTypeName=="sha512")
			sigType = EVP_sha512();
		else
			ctx._warnInterprete (WException (WFormattedString ("Signature type %s is not supported, sha256 is chosen instead.", (WCSTR) sigType), -1), ctx);

		if (X509_sign(x509, pkey, sigType)==0) {
			WCSTR errStr = ERR_lib_error_string(ERR_get_error ());
			if (!errStr)
				errStr = "";
			ctx._warnInterprete (WException (WFormattedString ("Cannot sign RSA key. %s", (WCSTR) errStr), ERR_get_error ()), ctx);
		}
		else {
			FILE *f = fopen((WCSTR) argvalues[1]->value.c_str(), "wb");
			if (!f)  {
				ctx._warnInterprete (WException (WFormattedString ("Cannot create key file: %s", (WCSTR) argvalues[1]->value.c_str()), -1), ctx);
			}
			else
			{
				PEM_write_PrivateKey(
					f,                  /* write the key to the file we've opened */
					pkey,               /* our key from earlier */
					NULL,               /* default cipher for encrypting the key on disk EVP_des_ede3_cbc() */
					NULL,				//(unsigned char*) "password",   /* passphrase required for decrypting the key on disk */
					0,                  /* length of the passphrase string */
					NULL,               /* callback for requesting a password */
					NULL                /* data to pass to the callback */
				);
				fclose (f);
				f = NULL;
				f = fopen((WCSTR) argvalues[0]->value.c_str(), "wb");
				if (!f) {
					ctx._warnInterprete (WException (WFormattedString ("Cannot create certificate file: %s", (WCSTR) argvalues[0]->value.c_str()), -1), ctx);		
				}
				else {					
					PEM_write_X509(f, x509);
					fclose (f);
					ret = true;
				}	
				f = NULL;
			}			
		}
	}	
	/*
	if (rsa) {
		RSA_free(rsa);
		rsa = NULL;
	}
	*/

	if (x509) {
		X509_free (x509);
		x509 = NULL;
	}

	if (pkey) {
		EVP_PKEY_free (pkey);
		pkey = NULL;
	}
	return 0;
}

int func_SSL_copy_session_id (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {

	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	if (argvalues.size ()<2 || argvalues[1]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_HANDLE_REQUIRED;
	}

	Handle* h = (Handle*) *argvalues[0];
	Handle* h2 = (Handle*) *argvalues[1];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_SSL) {			
			_SSL* obj = (_SSL*) h->handle;			
			if (h2 && ctx.handleHT.isKey (h2) && h2->handletype==g_handletype_SSL) {
				_SSL* obj2 = (_SSL*) h2->handle;					
				SSL_copy_session_id ((SSL*) *obj, (SSL*) *obj2);
        ret = true;
			}
			else {
				throw WException ("No valid SSL handle", -1);
			}
		}
		else {
			throw WException ("No valid SSL handle", -1);
		}

	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
		ret = false;
	}	
	return 0;
}

WScriptFuncDef g_funcDefList[] = {
	// Functions
	{ "SSL_CTX_new",  func_SSL_CTX_new },
	{ "SSL_CTX_free",  func_SSL_CTX_free },
	{ "SSL_CTX_load_cert",  func_SSL_CTX_load_cert },
	{ "SSL_CTX_set_tlsext_servername_callback", func_SSL_CTX_set_tlsext_servername_callback},
	{ "SSL_CTX_set_verify",  func_SSL_CTX_set_verify },
	{ "SSL_CTX_set_verify_depth",  func_SSL_CTX_set_verify_depth },	
	{ "SSL_CTX_set_options",  func_SSL_CTX_set_options },
	{ "SSL_CTX_clear_options",  func_SSL_CTX_clear_options },
	{ "SSL_CTX_get_options",  func_SSL_CTX_get_options },
	{ "SSL_CTX_set_cipher_list", func_SSL_CTX_set_cipher_list },

	{ "SSL_CTX_load_verify_locations",  func_SSL_CTX_load_verify_locations },
	{ "SSL_CTX_use_certificate_chain_file",  func_SSL_CTX_use_certificate_chain_file },
	{ "SSL_CTX_use_certificate_file",  func_SSL_CTX_use_certificate_file },
	{ "SSL_CTX_use_PrivateKey_file",  func_SSL_CTX_use_PrivateKey_file },
	{ "SSL_CTX_check_private_key",  func_SSL_CTX_check_private_key },
	
	{ "SSL_new",  func_SSL_new },
	{ "SSL_free",  func_SSL_free },
	{ "SSL_set_cipher_list", func_SSL_set_cipher_list },
	{ "SSL_get_cipher_list", func_SSL_get_cipher_list },
	{ "SSL_set_verify_result", func_SSL_set_verify_result },
	{ "SSL_get_verify_result", func_SSL_get_verify_result },	
	{ "SSL_set_fd",  func_SSL_set_socket },
	{ "SSL_accept",  func_SSL_accept },
	{ "SSL_connect",  func_SSL_connect },
	{ "SSL_read",  func_SSL_read },
	{ "SSL_readln",  func_SSL_readln },
	{ "SSL_gets",  func_SSL_gets },
	{ "SSL_write",  func_SSL_write },
	{ "SSL_get_peer_certificate", func_SSL_get_peer_certificate },
	{ "SSL_FLAG",  func_SSL_FLAG },
	{ "SSL_copy_session_id", func_SSL_copy_session_id },

	{ "SSL_create_selfsigned_cert", func_SSL_create_selfsigned_cert },

  { "ERR_remove_state",  func_ERR_remove_state },
	{ "ERR_get_error", func_ERR_get_error },    
	{ "ERR_lib_error_string", func_ERR_lib_error_string },

};

extern "C" {

int DLL_EXPORT wScriptDLLStartup (InterpreterContext& ctx) {

	// Delete SIGPIPE signal
	{struct sigaction act;
	act.sa_handler = SIG_IGN;
	sigemptyset (&act.sa_mask);
	act.sa_flags = 0;
	sigaction (SIGPIPE, &act, NULL);}

#if OPENSSL_VERSION_NUMBER < 0x10100000L
    SSL_library_init();
#else
    OPENSSL_init_ssl(0, NULL);
//  SSL_library_init();
#endif    

  // CRYPTO_malloc_init();
  // CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
  // OPENSSL_config(NULL);
  SSL_load_error_strings();
  ERR_load_crypto_strings ();
  // OpenSSL_add_ssl_algorithms ();
  OpenSSL_add_all_algorithms();

  
  // Create mutex table    
  int cnt = CRYPTO_num_locks();
  g_mutexList = (WMUTEX*) malloc (cnt * sizeof (WMUTEX));
  for (int i=0;i<cnt;i++)
  	WSystem::initMutex (g_mutexList[i]);
  CRYPTO_set_locking_callback (openSSLLockFunction);	

	for (unsigned int i=0;i<sizeof (g_funcDefList) / sizeof (WScriptFuncDef);i++)
		ctx.nativeFunctionHT.put (g_funcDefList[i].name, (WSCRIPT_NATIVE_FUNCTION) g_funcDefList[i].func);
	return 0;
}


int DLL_EXPORT wScriptDLLCleanupAll (InterpreterContext& ctx) {
	return 0;
}


int DLL_EXPORT wScriptDLLShutdown (InterpreterContext& ctx) {
	for (unsigned int i=0;i<sizeof (g_funcDefList) / sizeof (WScriptFuncDef);i++)
		ctx.nativeFunctionHT.remove (g_funcDefList[i].name);
    cleanupOpenSSL ();
    
    int cnt = CRYPTO_num_locks();
    for (int i=0;i<cnt;i++)
      WSystem::deleteMutex (g_mutexList[i]);
    
    if (g_mutexList)
        free ((void*) g_mutexList);
    return 0;
}


int DLL_EXPORT wScriptDLLVersion () {
	return (int) (WSCRIPT_VERSION * 100);
}

}

#ifdef _WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved)
{
    switch (dwReason)
		{
			case DLL_PROCESS_ATTACH:			
				break;
			case DLL_THREAD_ATTACH:
				break;
			case DLL_THREAD_DETACH:			
				ERR_remove_state(0);
				break;
			case DLL_PROCESS_DETACH:           
				break;
		}
    return TRUE;
}
#endif
