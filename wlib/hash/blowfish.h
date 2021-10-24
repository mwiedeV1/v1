#ifndef _BLOWFISH_H
#define _BLOWFISH_H

#include <wlib.h>

extern bool blowfish (WCSTR input, WCSTR salt, WString& output);
extern bool blowfish_check (WCSTR input, WCSTR enc);

#endif //_BLOWFISH_H