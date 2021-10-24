#ifndef URL_H
#define URL_H

#include "../WString.h"

/**********************************************************************
 * URL
 *
 * plattform: WIN32
 * author: Matthias Wiede <mwiede@mailexcite.com>
 *
 **********************************************************************/

class URL {
	typedef struct {
		int     m_port;
		WString m_protocol;
		WString m_filename;
		WString m_host;
	} SURL;
	static void parse (WCSTR url, SURL* urlStruct);

	////////////////////
	SURL m_url;

public:

	inline URL (WCSTR url)
	{
		parse (url, &m_url);
	}

	inline void operator = (WCSTR url)
	{
		parse (url, &m_url);
	};

	inline int getPort () const
	{
		return m_url.m_port;
	}

	inline void setPort (int port)
	{
		m_url.m_port = port;
	}

	inline WCSTR getHost () const
	{
		return m_url.m_host;
	}

	inline WCSTR getProtocol () const
	{
		return m_url.m_protocol;
	}

	inline const WString& getFilename () const
	{
		return m_url.m_filename;
	}

};

#endif /* URL_H */
