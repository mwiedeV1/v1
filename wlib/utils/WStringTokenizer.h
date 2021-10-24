/**
 $Author: mwiede $
 $Revision: 1.1 $

 (C) Matthias Wiede <Matthias.Wiede@web.de> '2004

 THIS FILE IS PART OF WLIB2.
 WLIB2 IS A C++ LIBRARY WRITTEN BY MATTHIAS WIEDE.
*/



#ifndef _WSTRINGTOKENIZER_H
#define _WSTRINGTOKENIZER_H

#include "wlib.h"

/**
 WStringTokenizer teilt einen String
 anhand eines Trennzeichens in mehrere Teilbereiche auf.
*/

class _WLIBAPI WStringTokenizer : public WStringList {
public:
	WStringTokenizer ();

	/**
	 Konstruktor teilt einen String.
	 @see parse
	*/
	WStringTokenizer (WCSTR str, WCSTR delim, bool fTrim=false);

	virtual ~WStringTokenizer ();


	/**
	 Teilt einen String anhand eines Trennzeichens.

	 @param str String, der aufgeteilt werden soll.
	 @param delim Trennzeichen String (z.B "=")
	 @param fTrim true, wenn links- und rechtsbündige Leerzeichen
	 des Teilbereichs entfernt werden sollen.
	 @return Anzahl der ermittelten Teilbereiche oder 0, wenn
	 Trennzeichen nicht gefunden wurde.

	*/
	W32 parse (WCSTR str, WCSTR delim, bool fTrim=false);

};

#endif /* _WSTRINGTOKENIZER_H */

