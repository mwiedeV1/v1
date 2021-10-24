/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "HTTPClient.h"

// SNI Infos
// http://stackoverflow.com/questions/5113333/how-to-implement-server-name-indication-sni

#include "WStringTokenizer.h"

#ifndef _WIN32
#include <syslog.h>
#endif


HTTPClient::HTTPClient ()
{
#ifdef _USE_SSL
	m_ctxPnt = NULL;
	m_sslPnt = NULL;
#endif
	m_timeout = 60*10*1000;
	create ();
}

HTTPClient::~HTTPClient ()
{
	try {
		close ();
	}
	catch (WException) {
	}
}


void HTTPClient::create ()
{
#ifdef _USE_SSL
//    SSL_METHOD *meth = ; // SSLv23_client_method();
	m_ctxPnt = SSL_CTX_new (TLSv1_2_client_method ());
	if (m_ctxPnt==NULL)
		throw HTTPClientException ("Cannot create SSL Context", -1);
	m_sslPnt = SSL_new (m_ctxPnt);
	if (m_sslPnt==NULL)
		throw HTTPClientException ("Cannot create SSL Session", -1);
#endif
}

void HTTPClient::close ()
{
#ifdef _USE_SSL
	if (m_sslPnt!=NULL) {
		SSL_free (m_sslPnt);
		m_sslPnt=NULL;
	}
	if (m_ctxPnt!=NULL) {
		SSL_CTX_free (m_ctxPnt);
		m_ctxPnt=NULL;
	}
#endif
}


int HTTPClient::filterHeader (
    WString&  result,
    HTTPInfo& httpInfo,
    HeaderHT& headerHT)
{
	WString header;


	int pos = result.indexOf ("\r\n\r\n");
	if (pos==-1)
		pos = result.indexOf ("\n\n");

	headerHT.removeAll ();
	if (pos!=-1) {
		result.substring (0,pos, header);
		result.substring (pos+4, result);
		WString key, value;
		WStringTokenizer tok (header,"\n");

		bool fFirst = true;
		while (tok.hasMoreElements ()) {
			WString line = tok.nextValue ();
			if (fFirst) {
				int pos = line.indexOf (" ");
				if (pos==-1)
					return -1;
				line.substring (0,pos, httpInfo.m_version);
				line.substring (pos+1, line);

				pos = line.indexOf (" ");
				if (pos==-1)
					return -1;
				httpInfo.m_statusCode = atoi (line);
				line.substring (pos+1, httpInfo.m_statusText);
				httpInfo.m_statusText.trim ();
				fFirst = false;
			}
			else {
				line.split (":", key, value);
				headerHT.put (key, value);
			}
		}
		return httpInfo.m_statusCode;
	}
	return -1;
}

int HTTPClient::requestPage (
    WSocket&       socket,
    const WString& httpHeader,
    WString&       result,
    HeaderHT&      headerHT,
    const void*    postData,
    W32            postSize,
    SSL*           sslPnt)
{

	result.clear ();

	W8 mybuf[1025];
	int err;
#ifdef _USE_SSL
	if (sslPnt!=NULL) {
		// set socket and connect
		SSL_set_fd (sslPnt, socket.getHandle ());
		if ((err = SSL_connect (sslPnt))==-1)
			throw HTTPClientException (ERR_lib_error_string(ERR_get_error ()), err);
		// write HTTP header
		if ( (err = SSL_write (sslPnt, (WCSTR) httpHeader, httpHeader.length ()))==-1)
			throw HTTPClientException (ERR_lib_error_string(ERR_get_error ()), err);
		// read data
		do {
			if ( (err = SSL_read (sslPnt, (char*) mybuf, 1024))==-1) {
				WCSTR errStr = ERR_lib_error_string(ERR_get_error ());
				if (errStr)
					throw HTTPClientException (errStr, err);
				else
					err=0;
			}
			result+=WString (mybuf, err);
		}
		while (err>0);

		// shutdown ssl-connection
		SSL_shutdown (sslPnt);
	}
	else {
#endif
		socket.write (httpHeader);
		if (postData!=NULL) {
			socket.write (postData, postSize);
		}
		bool fBreak = false;
		do {
			try  {
				err = (W32) socket.read ((void*) &mybuf[0], 512);
			}
			catch (WSocketException& e) {
				if (e.getId ()!=WSocketException::ERR_ID_CLOSED)
					throw e;
				fBreak = true;
				break;
			}
			catch (...) {
				fBreak = true;
				break;
			}
			if (!fBreak)
				result += WString (mybuf, err);
		}
		while (!fBreak);
#ifdef _USE_SSL
	}
#endif
	HTTPInfo info;
	int statusCode;
	if ((statusCode=filterHeader (result, info, headerHT))==-1)
		throw HTTPClientException ("No HTTP-Header available !", -1);
	return statusCode;
}

void HTTPClient::createHTTPHeader (
    WString&       httpHeader,
    WCSTR          filename,
    WCSTR          host,
    int            port,
    const WString& optHeader,
    WCSTR method)
{
	httpHeader = WFormattedString (
	                 "%s /%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\nCache-Control: max-age=0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.112 Safari/537.36",
	                 method,
	                 filename,
	                 host);

	if (optHeader.length ()>0) {
		httpHeader.append ("\r\n");
		httpHeader.append (optHeader);
	}
	httpHeader += "\r\n\r\n";
}


int HTTPClient::requestPage (
    WCSTR url,
    WCSTR optHeader,
    WString& resultContent,
    HeaderHT& resultHeaderHT,
    const void* postData,
    W32 postSize,
    bool fSSL)
{
	WString httpHeader (512);

	// Connect to server
	URL myURL (url);
	WIPAddress ipAddress (myURL.getHost (), myURL.getPort ());
	WSocket socket;
	socket.create ();
	socket.init (ipAddress);
	socket.connect ();
	socket.setRecvTimeout (m_timeout);

	// Create HTTP header
	WString myOptHeader = optHeader;
	if (postSize>0) {
		if (myOptHeader.length ()>0)
			myOptHeader.append ("\r\n");
		myOptHeader.append (WFormattedString ("Content-Length: %u", postSize));
	}
	createHTTPHeader (
	    httpHeader,
	    myURL.getFilename (),
	    myURL.getHost (),
	    myURL.getPort (),
	    myOptHeader,
	    postData!=NULL ? "POST" : "GET");
	WString protocol = myURL.getProtocol ();

	SSL* sslPnt = NULL;
#ifdef _USE_SSL
	if (protocol.equals ("https") || fSSL) {
		sslPnt = m_sslPnt;
		SSL_set_tlsext_host_name (sslPnt, myURL.getHost ());
	}
#endif
	return requestPage (socket, httpHeader, resultContent, resultHeaderHT, postData, postSize, sslPnt);
}

