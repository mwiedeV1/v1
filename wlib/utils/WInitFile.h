/**
 $Author: mwiede $
 $Revision: 1.1 $

 (C) Matthias Wiede <Matthias.Wiede@web.de> '2004

 THIS FILE IS PART OF WLIB2.
 WLIB2 IS A C++ LIBRARY WRITTEN BY MATTHIAS WIEDE.
*/


#ifndef _WINITFILE_H
#define _WINITFILE_H

#include "WFile.h"

/*
 * Class for a simple Init-file with Key-Value pairs.
 * ----------------------
 * # comment
 * key1 = value1
 * key2 = value2
 * ----------------------
 *
 * plattform: WIN32 / UNIX
 * author: Matthias Wiede <Matthias.Wiede@web.de>
 *
 **/

class _WLIBAPI WInitFile {
public:
	WInitFile ();
	virtual ~WInitFile ();
	void  open         (WCSTR filename, WCSTR sep = "=");
	void  save         (WCSTR filename, WCSTR sep = "=");
	void  close        ();
	// get value for key
	WCSTR getValue     (WCSTR key, WCSTR def="") const; // default return = ""
	int   getIntValue  (WCSTR key, int def=0) const; // default return = 0
	bool  getFlagValue (WCSTR key, bool def=false) const; // default return = false
	bool  isParam      (WCSTR key) const;
	void  putValue     (WCSTR key, WCSTR   value);
	void  putValue     (WCSTR key, int     value);
	void  putValue     (WCSTR key, W32     value);
	void  putValue     (WCSTR key, bool    value);
	void  putValue     (WCSTR key, double  value, int digits=2);
private:
	// hashtable with key->value pairs
	typedef WHashtable <WString, WString, WCSTR, const WString&> HT;
	HT m_ht;
	// static methods do the work
	void  read  (WFile& file, WInitFile::HT& ht, WCSTR sep);
};

#endif // WINITFILE_H