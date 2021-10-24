/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

/**

 ------------------------------------------------------
 Hinweis!!!
 Die Umrechnung in eine lokale Zeitzone ist
 derzeit mit Fehlern behaftet, aufgrund der Sommer/Winterzeit
 einiger Zeitzonen, das Problem muss noch gelöst werden.
 Es handelt sich um Umrechnungfehler im Bereich der Umschaltung,
 also z.B. +- 1h am 05.03. 2:00 Uhr.
 ------------------------------------------------------

*/

#ifndef _WDATE_H
#define _WDATE_H

#include "WException.h"
#include <time.h>

/**
 WDateException ist die Ausnahme von WDate.
 @see WDate
*/
class _WLIBAPI WDateException : public WException {
public:
	inline WDateException (WCSTR msg, int id) : WException ()
	{
		set ("WDateException", msg, id, errno);
	};
	enum {
		ERR_ID_INVALID_TIME = -1
	};
};

/** WTimeZone enthält Informationen
 über eine Zeitzone **/
class WDate;
class _WLIBAPI WTimeZone {
public:
	class _WLIBAPI Date {
	public:
		Date ();
		Date (int month, int dayOfWeek, int weekCode, int hour, int minute);
		int month, dayOfWeek, weekCode, hour, minute;
	};

	WTimeZone ();

	/** Konstruktor initialisiert eine Zeitzone. **/
	WTimeZone (
	    WCSTR       name,
	    int         bias=0);

	/** Konstruktor initialisiert eine Zeitzone. **/
	WTimeZone (
	    WCSTR       name,
	    WCSTR       daylightName,
	    int         bias,
	    const Date& standardDate,
	    int         standardBias,
	    const Date& daylightDate,
	    int         daylightBias);

	/** Berechnet die korrekte GMT Distance für das übergebene Datum. */
	int calcBias (int year, int month, int day, int hour,
	              int minute, bool& isDayLight) const;


	int          getBias         () const
	{
		return m_bias;
	};
	WCCSTR       getName         () const
	{
		return m_name;
	};
	WCCSTR       getDaylightName () const
	{
		return m_daylightName;
	};
	const  Date& getStandardDate () const
	{
		return m_standardDate;
	};
	int          getStandardBias () const
	{
		return m_standardBias;
	};
	const  Date& getDaylightDate () const
	{
		return m_daylightDate;
	};
	int          getDaylightBias () const
	{
		return m_daylightBias;
	};
	bool         isDaylightZone  () const
	{
		return m_isDaylightZone;
	};

	/* Berechnet den Tag des Monats für die entsprechende Zeitzone */
	static W32 getCurrentTimeZoneDay (W32 tzYear, W32 tzMonth,
	                                  W32 tzDayOfWeek, W32 tzWeekCode);

private:

	void initialize ();

	int     m_bias;           // GMT Distance in Minuten zum 0-Meridian
	WString m_name;           // Name der Zeitzone
	WString m_daylightName;   // Name der Zeitzone bei Sommerzeit
	Date    m_standardDate;   // Datum an dem die Normalzeit beginnt
	int     m_standardBias;   // GMT Offset in Minuten bei Normalzeit
	Date    m_daylightDate;   // Datum an dem die Sommerzeit beginnt
	int     m_daylightBias;   // GMT Offset in Minuten bei Sommerzeit
	bool    m_isDaylightZone; // Hat die Zeitzone Sommerzeit?
};



/**
 WDate kapselt Zeit- und Datums-Funktionen.
 Der Zeitwert innerhalb des Objekts ist ein 64 Bit Wert, der je nach
 eingestelltem "CounterFormat" interpretiert wird.
 Die Umrechnung der Zeit kann in lokale Zeitzonen erfolgen.
 Siehe "WTimeZone".

 \begin{verbatim}
 WDate date;
 WString str = date.format ("%d.%m.%Y (%H:%M:%S)");

 \end{verbatim}

 \Label{Formatcodes}
 Formatcodes
 \begin{itemize}
 \item %Bezeichner wie bei PHP date
 \item %% Prozentzeichen
 \end{itemize}

*/


class _WLIBAPI WDate {
public:

	/**
	 Standardisierte lesbare Zeitformate, die mit der "format" Funktion
	 erzeugt bzw. mit der "parse" Funktion gelesen werden können.
	 Der Standardwert ist TIME_FORMAT_ASCII
	 @see format
	 @see parse
	*/

	/** wkday Mon DD HH:MM:SS YYYY */
	static const WCSTR TIME_FORMAT_ASCII;


	/**
	 Zeigt an, wie der Wert des 64 Bit Zeitzählers (WTIME)
	 interpretiert werden muss. Der Standardwert ist COUNTER_TYPE_C
	 @see setTime
	 @see getTime
	*/
	enum CounterType {
		/* Zähler in Sekunden seit 1.1.1970 */
		COUNTER_TYPE_C = 1,

		// Noch nicht implementiert!
		/* Zähler in Sekunden seit 1.1.1900 */
		COUNTER_TYPE_UNIX = 2,

		// Noch nicht implementiert!
		/* Zähler in 100-Nanosekunden seit 1.1.1601 */
		COUNTER_TYPE_WINDOWS = 3
	};

	/**
	 Platzhalter für aktuell eingestellte Zeitzone im Objekt.
	 @see setDefaultTimeZone
	 @see WSystemInfo
	**/
	static const WTimeZone& TIME_ZONE_DEFAULT;

	/**
	 Konstruktor.
	 Initialisiert das Objekt mit aktueller Zeit.
	*/
	WDate ();
	WDate (const WTimeZone& timeZone);


	/**
	 Konstruktor.
	 Initialisiert das Objekt mit einem Zeitwert.
	*/
	WDate (WTIME time, const WTimeZone& timeZone=TIME_ZONE_DEFAULT);

	/**
	 Konstruktor.
	 Initialisiert das Objekt mit einem Datum.
	*/
	WDate (int year, int month, int day, int hour=0, int minute=0, int second=0,
	       const WTimeZone& timeZone=TIME_ZONE_DEFAULT);


	/** Destruktor. */
	virtual ~WDate ();

	/* Stellt die Standard-Zeitzone für das Datums-Objekt ein. */
	void setDefaultTimeZone (const WTimeZone& timeZone);


	/**
	 Setzt eine bestimmte Zeit.
	 @param time Zeitwert (UTC)
	 @see getTime
	 @see CounterType
	 @see TimeZone
	*/
	void setTime (WTIME time);


	/**
	  Gibt die Zeit vom Objekt zurück.
	  @return Zeitwert (UTC)
	  @see loadLocalTime
	  @see setTime
	  @see CounterType
	*/
	WTIME getTime () const;


	/**
	 Lädt die aktuelle Zeit der eingestellte Zeitzone ins Objekt.
	 @see setTime
	 @see setDefaultTimeZone
	 */
	void loadTime ();


	/**
	 Formatiert die Zeit zu einem String.
	 @param timeFormat Format in C Notation (siehe Formatcodes)
	 @param str String-Objekt
	*/
	const WString&  format (WCSTR timeFormat) const;


	// const WString& format (TimeFormat timeFormat) const;

	/**
	 Initialisiert die Zeit.
	 @param year Jahr z.B. 2001.
	 @param month Monat (1-12).
	 @param day Tag des Monats (1-31).
	 @param hour Stunde (0-23).
	 @param minute Minute (0-59).
	 @param second Sekunde (0-59).
	*/
	void parse (int year, int month, int day, int hour=0, int minute=0, int second=0,
	            const WTimeZone& timeZone=TIME_ZONE_DEFAULT);

	/** Gibt den Tag des Monats (1-31) zurück. */
	inline int getDay       () const
	{
		return m_localtime.tm_mday;
	}
	/** Gibt den Tag der Woche (0 - 6; Sonntag = 0) zurück. */
	inline int getWeekday   () const
	{
		return m_localtime.tm_wday;
	}
	/** Gibt den Monat des Jahres (1-12) zurück. */
	inline int getMonth     () const
	{
		return m_localtime.tm_mon  + 1;
	}
	/** Gibt das Jahr mit Jahrtausend (z.B: 2001) zurück. */
	inline int getYear      () const
	{
		return m_localtime.tm_year + 1900;
	}
	/** Gibt die Stunde des Tages (0-23) zurück. */
	inline int getHour      () const
	{
		return m_localtime.tm_hour;
	}
	/** Gibt die Minute der Stunde (0-59) zurück. */
	inline int getMinute    () const
	{
		return m_localtime.tm_min;
	}
	/** Gibt die Sekunde (0-59) der Minute zurück. */
	inline int getSecond    () const
	{
		return m_localtime.tm_sec;
	}
	/** Gibt true bei Sommerzeit und false bei Normalzeit zurück. **/
	inline bool isDaylight  () const
	{
		return m_localtime.tm_isdst;
	}
	/** Gibt die Sekunden zur UTC-GMT-Zeit zurück. **/
	int  getGMTOffset () const
	{
		return m_localtime.tm_gmtoff;
	};

	/**
	 Gibt einen String der Zeit vom Objekt zurück.
	 Die Zeit wird gemäß der Konstante WLIB_DEFAULT_DATE_TIME_FORMAT formatiert.
	 @return String z.B. '31.08.2004 / 02:56:05'
	*/
	const WString& toString () const;

	/* Operatoren, zum Vergleich zweier Datumsobjekte. */
	friend bool operator == (const WDate& date1, const WDate& date2);
	friend bool operator >  (const WDate& date1, const WDate& date2);
	friend bool operator >= (const WDate& date1, const WDate& date2);
	friend bool operator <  (const WDate& date1, const WDate& date2);
	friend bool operator <= (const WDate& date1, const WDate& date2);

	/* Ist das Jahr ein Schaltjahr? */
	static bool isLeapYear (W32 year);

	/* Maximale Tage des Monaths zurückgeben */
	static W32 getMaxDaysOfMonth (W32 year, W32 month);

	/* Tag des Jahres zurückgeben */
	static W32 getDayOfYear (W32 year, W32 month, W32 day);

	/* Wochennummer zurückgeben */
	static W32 getWeekOfYear (W32 year, W32 month, W32 day);

	/* Osterdatum Timestamp für ein Jahr ermitteln */
	static WTIME getEasterDate (W32 year);

protected:

	// everything is in local time
	WTIME m_time;         // seconds since 1.1.1970 (UTC)
	struct tm m_localtime; // time struct
	mutable WString m_dateStr; // formatted date
	WTimeZone m_defaultTimeZone; // default time zone
	void format (WCSTR timeFormat, WString &str) const;
	void initialize ();
};





#endif /* _WDATE_H */
