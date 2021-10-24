/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef _CGIUtils_H
#define _CGIUtils_H

#include <wlib.h>


/**
  WWebserverUtils
*/

class CGIUtils {
public:
	typedef struct {
		WString key;
		WString value;
	} CGIParam;
	typedef WLinkedList<CGIParam, const CGIParam&> CGIParamList;

	static bool char2Hex (W8& c);
	static bool parseURIParams (WCSTR params, CGIUtils::CGIParamList& paramList, bool fDecodeURI=true, bool fIgnoreCR=false);

};


#endif /* _CGIUtils_H */
