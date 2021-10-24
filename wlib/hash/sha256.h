#ifndef _SHA256_H
#define _SHA256_H

#include <wlib.h>

extern bool sha256 (WCSTR input, WCSTR salt, WString& output);
extern bool sha256_check (WCSTR input, WCSTR enc);


extern void* sha256_init ();
extern void sha256_update (void* ctx, void* buf, unsigned int len);
extern void sha256_finish (void* ctx, void* hash);



#endif //_SHA256_H
