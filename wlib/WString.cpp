/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/


#include "WString.h"
#include "WSystem.h"

#include <stdio.h>
#include <stdarg.h>

static char g_zeroStr[1] = {0};

void WString::initialize (W32 size)
{
	m_str = m_str0 = g_zeroStr;
	resize (size);
}

bool WString::startsWith (char* src, WCSTR str)
{
	while ((*src!=0) && (*str!=0)) {
		if (*src!=*str)
			return false;
		str++;
		src++;
	}
	if ((*src==0) && (*str!=0))
		return false;
	return true;
}

/*
 * append a string to the private buffer
 * @param str string
 **/

void WString::_append (WCSTR strAdd, W32 lengthAdd)
{
	W32 l1, l2 = m_strLength + (l1  = lengthAdd + 1);
	if (l2>m_bufLength) {
		if (m_str==m_str0)
			m_str = (char*) WMemory::allocMemory  (l2);
		else
			m_str = (char*) WMemory::reallocMemory (m_str, l2);
		m_bufLength=l2;
	}
	memcpy (m_str + m_strLength, strAdd, lengthAdd);
	m_strLength = l2-1;
	m_str[m_strLength] = 0; // terminate C-Strings
}


/*
 * load a string into the private buffer
 * @param str string
 **/
void WString::_assign (WCSTR strNew, W32 lengthNew)
{
	lengthNew++;
	if (lengthNew>m_bufLength) {
		if (m_str==m_str0)
			m_str = (char*) WMemory::allocMemory  (lengthNew);
		else
			m_str = (char*) WMemory::reallocMemory (m_str, lengthNew);
		m_bufLength=lengthNew;
	}
	memcpy (m_str, strNew, lengthNew);
	m_strLength = lengthNew-1;
	m_str[m_strLength] = 0; // terminate C-Strings
}

void WString::replace (char* str, W32 length, char oldChar, char newChar)
{
	while (length>0) {
		if (*str==oldChar)
			*str=newChar;
		str++;
		length--;
	}
}

WString& WString::resize (W32 size)
{
	if (size==0) {
		if (m_str!=m_str0) {
			WMemory::freeMemory (m_str);
		}
		m_str       = m_str0;
		m_strLength = 0;
		m_bufLength = 1;
	}
	else if (m_str==m_str0) {
		m_str       = (char*) WMemory::allocMemory (size+1);
		m_str[0]    = 0;
		m_str[size] = 0;
		m_strLength = 0;
		m_bufLength = size+1;
	}
	else {
		m_str       = (char*) WMemory::reallocMemory (m_str, size+1);
		m_str[size] = 0;
		m_bufLength = size+1;
		if (size<m_strLength)
			m_strLength = size;
	}
	return *this;
}

WString::~WString ()
{
	if (m_str!=m_str0) {
		 WMemory::freeMemory (m_str);
	}
};


W32 WString::trim (char* str, W32 length, char extraTrimChar)
{
	char* pnt1 = str;
	// search from left-side
	while (((*str==32) ||
	        (*str==9)  ||
	        (*str==extraTrimChar)  ||
	        (*str=='\r')) &&
	        (*str!=0))
		str++;
	// check if string is zero
	if (*str==0) {
		*pnt1=0;
		return 0;
	}
	char* pnt2 = str;
	// search from right side
	str  = pnt1 + length - 1;

	while (str>pnt1) {
		if ((*str!=32)   &&
		        (*str!=9)    &&
		        (*str!=extraTrimChar)    &&
		        (*str!='\r') &&
		        (*str!=0))
			break;
		str--;
	}
	length = 0;
	while (pnt2<=str) {
		*pnt1++ = *pnt2++;
		length++;
	}
	// terminate
	*pnt1=0;
	// return new size
	return length;
}

int WString::indexOf (char* str, WCSTR findStr)
{
	char* pnt = strstr (str, findStr);
	if (pnt==NULL)
		return -1;
	return (int) (pnt - str);
}

int WString::lastIndexOf (char* str, W32 length, char value)
{
	char* pnt = &str[length];
	while (length>0) {
		if (*pnt--==value)
			return length;
		length--;
	}
	return -1;
}

void WString::substring (W32 pos0, W32 pos1, WString& str) const
{
	if ((pos0<m_strLength) && (pos1<=m_strLength))
		str.assign (m_str, pos0, pos1);
	else
		str.truncate (0);
}

void WString::substring (W32 pos0, WString& str) const
{
	if (pos0<m_strLength)
		str.assign (m_str, pos0, m_strLength);
	else
		str.truncate (0);
}


bool WString::split (WCSTR splitStr, WString& key, WString& value, bool fTrim) const
{
	int pos = indexOf (splitStr);
	if (pos!=-1) {
		substring (0,pos,key);
		substring (pos+strlen(splitStr),value);
		if (fTrim) {
			key.trim ();
			value.trim ();
		}
		return true;
	}
	return false;
}



WString operator + (WCSTR str1, const WString& str2)
{
	return WString (str1) += str2;
}

WString operator + (const WString& str1, const WString &str2)
{
	return WString (str1) += str2;
}

WString operator + (const WString& str1, WCSTR str2)
{
	return WString (str1) += str2;
}

bool operator == (const WString& str1, const WString& str2)
{
	return str1.equals (str2);
};


bool operator == (WCSTR str1, const WString& str2)
{
	return str2.equals (str1);
};


bool operator == (const WString& str1, WCSTR str2)
{
	return str1.equals (str2);
};


//////////////////////////////////////////////
// WFormattedString
//////////////////////////////////////////////

WFormattedString::WFormattedString () : WString ()
{

}

WFormattedString::WFormattedString (W32 length)  : WString (length)
{

}

WFormattedString::WFormattedString (WCSTR format, ...) : WString (DEFAULT_BUF_SIZE)
{
	if (getBufLength ()<=1)
		resize (DEFAULT_BUF_SIZE);

	va_list args;
	va_start (args, format);
	int len;
	if ((len = vsnprintf (m_str,
	                      m_bufLength-1, // under windows no 0 character will be inserted
	                      format,
	                      args))!=-1)
		m_strLength = len;
	else
		m_strLength = m_bufLength-1;
	va_end (args);
}


WString& WFormattedString::format (WCSTR format, ...)
{
	if (getBufLength ()<=1)
		resize (DEFAULT_BUF_SIZE);

	va_list args;
	va_start (args, format);
	int len;
	if ((len = vsnprintf (m_str,
	                      m_bufLength-1, // under windows no 0 character will be inserted
	                      format,
	                      args))!=-1)
		m_strLength = len;
	else
		m_strLength = m_bufLength-1;
	va_end (args);
	return *this;
}

