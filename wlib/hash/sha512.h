#ifndef _SHA512_H
#define _SHA512_H

#include <wlib.h>

extern bool sha512 (WCSTR input, WCSTR salt, WString& output);
extern bool sha512_check (WCSTR input, WCSTR enc);


extern void* sha512_init ();
extern void sha512_update (void* ctx, void* buf, unsigned int len);
extern void sha512_finish (void* ctx, void* hash);



#endif //_SHA512_H
