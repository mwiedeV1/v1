/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "WException.h"

const WString& WException::toString () const
{
	m_toString.format (
	    "%s: %s (id=%i, syserr=%.8Xh)",
	    (WCSTR) m_name,
	    (WCSTR) m_msg,
	    (int)   m_id,
	    (W32)   m_sysErrCode);
	return m_toString;
}
