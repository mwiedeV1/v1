/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/


#ifndef _WEXCEPTION_H
#define _WEXCEPTION_H

#include "WString.h"

/**
 WException

*/

class WException {
public:
	inline WException()
	{
		m_id = 0;
		m_sysErrCode = 0;
		m_name = "WException";
	}

	inline WException(WCSTR msg)
	{
		set("WException", msg, 0);
	};

	inline WException(WCSTR msg, int id = 0)
	{
		set("WException", msg, id);
	};

	inline WException(const WException &e)
	{
		set(e.getName(), e.getMessage(), e.getId());
	}

	inline WException(const WException &e, WCSTR msg, int id = 0)
	{
		set("WException", e, msg, id);
	};

	inline WCSTR getName() const
	{
		return (m_name);
	};

	inline const WString &getMessage() const
	{
		return m_msg;
	}

	inline int getId() const
	{
		return m_id;
	};

	inline W32 getSysErrCode() const
	{
		return m_sysErrCode;
	}

	virtual const WString &toString() const;

protected:
	int m_id;         // id
	WCSTR m_name;     // name of exception
	WString m_msg;    // message text
	W32 m_sysErrCode; // system error-code

	inline void set(WCSTR name, WCSTR msg, int id = 0, W32 sysErrCode = 0)
	{
		m_name = name;
		m_msg = msg;
		m_id = id;
		m_sysErrCode = sysErrCode;
	};

	inline void set(WCSTR name, const WException &e, WCSTR msg, int id = 0)
	{
		set(name, e.toString() + WString("\r\n   ->") + msg, id, e.getSysErrCode());
	}

	mutable WFormattedString m_toString;
};
#endif /* _WEXCEPTION_H */
