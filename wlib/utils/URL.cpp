/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "URL.h"

void URL::parse (WCSTR url, SURL* urlStruct)
{
	// extract protocol
	char* str1 = strstr ((char*) url, "://");

	if (str1!=NULL) {
		urlStruct->m_protocol = WString (url, str1-url);
		str1+=3;
	}
	else  {
		// default protocol is http
		urlStruct->m_protocol = "http";
		str1 = (char*) url;
	}

	// default port for specific protocol

	if (urlStruct->m_protocol.equals ("http"))
		urlStruct->m_port = 80;
	else if (urlStruct->m_protocol.equals ("ftp"))
		urlStruct->m_port = 21;
	else if (urlStruct->m_protocol.equals ("https"))
		urlStruct->m_port = 443;

	// extract host / port
	char* str2 = strstr (str1, ":");

	if (str2!=NULL) {
		// host
		urlStruct->m_host = WString (str1, str2-str1);
		// port
		str2++;
		urlStruct->m_port = atoi (str2);

		// filename
		str1 = strstr (str2, "/");
	}
	else {
		str2  = str1;
		// host
		str1 = strstr (str2, "/");
		if (str1!=NULL) {
			// host found
			urlStruct->m_host = WString (str2, str1-str2);
		}
		else {
			urlStruct->m_host = str2;
		}
	}
	// filename
	if (str1!=NULL) {
		str1++;
		urlStruct->m_filename = str1;
	}
}
