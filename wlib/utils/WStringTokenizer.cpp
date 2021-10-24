/**
 $Author: mwiede $
 $Revision: 1.1 $

 (C) Matthias Wiede <Matthias.Wiede@web.de> '2004

 THIS FILE IS PART OF WLIB2.
 WLIB2 IS A C++ LIBRARY WRITTEN BY MATTHIAS WIEDE.
*/


#include "WStringTokenizer.h"

bool nextToken (W8** pnt, WCSTR delim, WString& token)
{
	W8* c, *c0, *c1;


	c = c0 = *pnt;
	while (*c!=0) {
		if (*c==delim[0]) {
			W8* c2 = (W8*) delim;
			c1 = c;
			while (*c!=0 && *c==*c2 && *c2!=0) {
				c++;
				c2++;
			}
			if (*c2==0) {
				token.assign (*pnt, c1-*pnt);
				*pnt = c;
				return true;
			}
		}
		c++;
	}
	token.assign (*pnt, c-*pnt);
	*pnt = c;
	return false;


}

WStringTokenizer::WStringTokenizer ()
{

}

WStringTokenizer::WStringTokenizer (WCSTR str, WCSTR delim, bool fTrim)
{
	parse (str, delim, fTrim);
}

WStringTokenizer::~WStringTokenizer ()
{

}

W32 WStringTokenizer::parse (WCSTR str, WCSTR delim, bool fTrim)
{

	W8* c = (W8*) str;

	WString token;

	removeAll ();
	while (nextToken (&c, delim, token)) {
		if (fTrim)
			token.trim ();
		add (token);
	}
	if (size ()>0) {
		if (fTrim)
			token.trim ();
		add (token);
	}
	return size ();
}
