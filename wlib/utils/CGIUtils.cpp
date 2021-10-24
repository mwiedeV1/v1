/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "CGIUtils.h"

bool CGIUtils::char2Hex (W8& c)
{
	if ((c>='0') && (c<='9'))
		c-='0';
	else if ((c>='A') && (c<='F')) {
		c-='A';
		c+=10;
	}
	else
		return false; // bad format
	return true;
}

bool CGIUtils::parseURIParams (WCSTR params, CGIUtils::CGIParamList& paramList, bool fDecodeCGI, bool fIgnoreCR)
{

	bool fKey = true;

	CGIUtils::CGIParam param;
	param.key.resize (128);
	param.value.resize (128);

	W8* c = (W8*) params;
	W8 v = 0, v2 = 0;
	while (*c!=0) {
		switch (*c) {
			case '=':
				c++;
				fKey = false;
				break;
			case '&':
				c++;
				if (param.key.length ()>0) {
					paramList.add (param);
					param.key.truncate (0);
				}
				param.value.truncate (0);
				fKey = true;
				break;
			case '%':
				if (!fDecodeCGI) {
					if (fKey)
						param.key.append (*c);
					else
						param.value.append (*c);
					c++;
				}
				else {
					// CGI-Decoding
					c++;
					if (*c==0)
						break;
					v  = 0;
					v2 = *c++;
					if (!char2Hex (v2))
						return false;
					v|=(v2<<4);
					if (*c==0)
						break;
					v2 = *c++;
					if (!char2Hex (v2))
						return false;
					v|=v2;
					if (v==0x0d && fIgnoreCR)
						break;
					if (fKey)
						param.key.append (v);
					else
						param.value.append (v);
					if (*c==0)
						break;

				}
				break;
			case '+':
				c++;
				if (fKey)
					param.key.append (" ");
				else
					param.value.append (" ");
				break;
			default:
				if (fKey)
					param.key.append (*c);
				else
					param.value.append (*c);
				c++;
				break;

		};

	};
	if (param.key.length ()>0)
		paramList.add (param);
	return true;
}
