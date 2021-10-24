/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "WMemory.h"

#include <stdlib.h>

void* WMemory::allocMemory (W32 size)
{
	void* pnt = ::malloc (size);
	if (pnt==NULL) {
		WFormattedString str ("Out of memory. Cannot alloc %u bytes.", (W32) size);
		throw WMemoryException (str, WMemoryException::ERR_ID_OUT_OF_MEMORY);
	}
	return pnt;
}

void* WMemory::reallocMemory (void* ptr, W32 size)
{
	void* pnt = ::realloc (ptr, size);
	if (pnt==NULL) {
		WFormattedString str ("Out of memory. Cannot realloc %u bytes.", (W32) size);
		throw WMemoryException (str, WMemoryException::ERR_ID_OUT_OF_MEMORY);
	}
	return pnt;
}

void WMemory::freeMemory (void* pnt)
{
	::free (pnt);
}

