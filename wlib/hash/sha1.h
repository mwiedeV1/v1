#ifndef _SHA1_H
#define _SHA1_H

#include <wlib.h>

extern void* sha1_init ();
extern void sha1_update (void* ctx, void* buf, unsigned int len);
extern void sha1_finish (void* ctx, void* hash);
extern void sha1_raw (void* buf, unsigned int len, void* hash);


#endif //_SHA1_H
