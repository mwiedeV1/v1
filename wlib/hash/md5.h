#ifndef _MD5_H
#define _MD5_H

#include <wlib.h>

extern bool md5 (WCSTR input, WCSTR salt, WString& output);
extern bool md5_check (WCSTR input, WCSTR enc);

extern void* md5_init ();
extern void md5_update (void* ctx, void* buf, unsigned int len);
extern void md5_finish (void* ctx, void* hash);
extern void md5_raw (void* buf, unsigned int len, void* hash);


#endif //_MD5_H
