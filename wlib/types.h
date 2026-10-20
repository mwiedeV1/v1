/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef _WTYPES_H
#define _WTYPES_H

#include <inttypes.h>
#include <time.h>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__ || __aarch64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#ifdef ENVIRONMENT64
typedef SYS_UINT uint64_t
typedef SYS_INT int64_t
#else
typedef SYS_UINT uint32_t
typedef SYS_INT int32_t
#endif


#ifndef FALSE
#define FALSE false
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef WCSTR
typedef const char* WCSTR;
#endif

#ifndef W8
typedef unsigned char W8;
#endif

#ifndef W16
typedef unsigned short W16;
#endif

#ifndef W32
typedef unsigned long W32;
#endif

#define W32_MAX 0xFFFFFFFF

#ifndef W64
typedef uint64_t W64;
#endif

#ifndef WTIME
typedef time_t WTIME;
#endif


#endif /* _WTYPES_H */
