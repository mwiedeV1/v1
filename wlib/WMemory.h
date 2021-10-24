/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef _WMEMORY_H
#define _WMEMORY_H

#include "WException.h"

/**
 WMemoryException ist die Ausnahme von WMemory.
*/

class WMemoryException : public WException {
public:
	enum {
		ERR_ID_GENERAL= -1,
		ERR_ID_OUT_OF_MEMORY = -2
	};

	inline WMemoryException (WCSTR msg) : WException ()
	{
		set ("WMemoryException", msg, ERR_ID_GENERAL, errno);
	};

	inline WMemoryException (WCSTR msg, int id) : WException ()
	{
		set ("WMemoryException", msg, id, errno);
	};
};

/**
 WMemory stellt statische Methoden zur
 Speicherverwaltung zur Verfügung.
*/

class WMemory {
public:
	/** Speicherverwaltung **/
	static void* allocMemory    (W32 size);
	static void* reallocMemory  (void* pnt, W32 size);
	static void  freeMemory     (void* pnt);
};


#endif /* _WMEMORY_H */
