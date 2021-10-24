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
