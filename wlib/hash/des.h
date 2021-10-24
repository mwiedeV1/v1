#ifndef _DES_H
#define _DES_H

#include <wlib.h>

extern bool des (WCSTR input, WCSTR salt, WString& output);
extern bool des_check (WCSTR input, WCSTR enc);

#endif //_DES_H