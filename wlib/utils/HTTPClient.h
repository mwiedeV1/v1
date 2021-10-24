/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef _HTTPCLIENT_H
#define _HTTPCLIENT_H

#include "../WSocket.h"

#include "URL.h"

#ifdef _USE_SSL
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif



class HTTPClientException : public WException {
public:
	HTTPClientException (WCSTR msg, int id) : WException ()
	{
		if (msg==NULL)
			msg = "Unknown HTTP-Error";
		set ("HTTPClientException", msg, id);
	};
};

class HTTPClient {


public:
	HTTPClient ();
	~HTTPClient ();

	// hashtable width http-headers
	typedef WHashtable <WString, WString, WCSTR, const WString&> HeaderHT;

	void setTimeout (W32 ms)
	{
		m_timeout = ms;
	}
	int requestPage (
	    WCSTR          url,
	    WCSTR          optHeader,
	    WString&       resultContent,
	    HeaderHT&      resultHeaderHT,
	    const void*    postData,
	    W32            postSize,
	    bool           fSSL = false);


private:
#ifdef _USE_SSL
	SSL_CTX* m_ctxPnt; // ssl-context
	SSL*     m_sslPnt; // ssl-session
#else
	typedef struct {
		int dummy;
	} SSL;
#endif
	W32 m_timeout;

	typedef struct {
		WString m_version;
		WString m_statusText;
		int     m_statusCode;
	} HTTPInfo;

	void create ();
	void close  ();

	// request page
	static int requestPage (
	    WSocket&       socket,
	    const WString& httpHeader,
	    WString&       result,
	    HeaderHT&      headerHT,
	    const void*    postData,
	    W32            postSize,
	    SSL*           sslPnt);

	// filter the http-header and return status-code
	static int filterHeader (
	    WString&       result,
	    HTTPInfo&      httpInfo,
	    HeaderHT&      headerHT);

	static void createHTTPHeader (
	    WString&       httpHeader,
	    WCSTR          filename,
	    WCSTR          host,
	    int            port,
	    const WString& optHeader,
	    WCSTR          method);

};

#endif /* _HTTPCLIENT_H */
