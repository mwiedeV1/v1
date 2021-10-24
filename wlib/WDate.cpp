/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "WDate.h"
#include "WSystem.h"


const WTimeZone& WDate::TIME_ZONE_DEFAULT = g_systemInfo.getLocalTimeZone ();

// RFC1123: wkday "," DD Mon YYYY HH:MM:SS "GMT"
// RFC850: weekday "," DD-Mon-YY HH:MM:SS "GMT"
// UTC: YYYYMMDD'T'HHMMSS'Z'
// ISO8601: ?
const WCSTR WDate::TIME_FORMAT_ASCII = "%D %M %d %H:%i:%s %Y";


#define TO_C_TIME(x) ((x-116444736000000000)/10000000)


void WDate::initialize ()
{
	m_dateStr.resize (32);
	memset (&m_localtime, 0, sizeof (m_localtime));
}

WDate::WDate ()
{
	initialize ();
	setDefaultTimeZone (TIME_ZONE_DEFAULT);
	loadTime ();
}

WDate::WDate (const WTimeZone& timeZone)
{

	initialize ();
	setDefaultTimeZone (timeZone);
	loadTime ();
}

WDate::WDate (WTIME time, const WTimeZone& timeZone)
{

	initialize ();
	setDefaultTimeZone (timeZone);
	setTime (time);
}

WDate::WDate (int year, int month, int day, int hour, int minute, int second, const WTimeZone& timeZone)
{
	initialize ();

	parse (year, month, day, hour, minute, second, timeZone);
}


WDate::~WDate ()
{

}


WTIME WDate::getTime () const
{
	return m_time;
}

void WDate::setTime (WTIME time)
{
	m_time = time;
	memcpy (&m_localtime, localtime (&m_time), sizeof (tm));
}

void WDate::loadTime ()
{
	time (&m_time);
	setTime (m_time);
}

const WString& WDate::format (WCSTR timeFormat) const
{
	format (timeFormat, m_dateStr);
	return m_dateStr;
}

void WDate::format (WCSTR timeFormat, WString& str) const
{
	static WCSTR wday_name[7] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static WCSTR wday_name2[7] = {
		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
	};
	static WCSTR mon_name[12] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static WCSTR mon_name2[12] = {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};
	str.truncate (0);
	bool flag = false;
	WCSTR c = timeFormat;
	int value=0;


	while (*c!=0) {

		if (flag) {
			switch (*c) {
				// Tag des Monats mit führender 0
				case 'd':
					str.append (WNumberString (m_localtime.tm_mday, "%.2u"));
					break;
				// Wochentag abgekürzt
				case 'D':
					str.append (wday_name[m_localtime.tm_wday]);
					break;
				// Tag des Monats ohne führende 0
				case 'j':
					str.append (WNumberString (m_localtime.tm_mday, "%u"));
					break;
				// Ausgeschriebender Wochentag
				case 'l':
					str.append (wday_name2[m_localtime.tm_wday]);
					break;
				// Numerischer Tag der Woche 0 = Sonntag, 6 = Samstag
				case 'w':
					str.append (WNumberString (m_localtime.tm_wday));
					break;
				// Tag des Jahres von 0 beginnend
				case 'z':
					str.append (WNumberString (getDayOfYear (m_localtime.tm_year+1900, m_localtime.tm_mon+1, m_localtime.tm_mday)));
					break;

				// Wochennummer
				case 'W':
					str.append (WNumberString (getWeekOfYear (m_localtime.tm_year+1900, m_localtime.tm_mon+1, m_localtime.tm_mday)));
					break;
				// Monat als ganzes Wort
				case 'F':
					str.append (mon_name2[m_localtime.tm_mon]);
					break;
				// Monatszahl mit führender 0
				case 'm':
					str.append (WNumberString (m_localtime.tm_mon+1, "%.2u"));
					break;
				// Monatsname kurz
				case 'M':
					str.append (mon_name[m_localtime.tm_mon]);
					break;
				// Monatszahl ohne führende 0
				case 'n':
					str.append (WNumberString (m_localtime.tm_mon+1, "%u"));
					break;
				// Anzahl Tage des Monats
				case 't':
					str.append (WNumberString (getMaxDaysOfMonth (m_localtime.tm_year+1900, m_localtime.tm_mon+1), "%u"));
					break;
				// Schaltjahr 1 oder 0
				case 'L':
					str.append (WNumberString (isLeapYear (m_localtime.tm_year+1900)));
					break;
				// Jahreszahl 4 stellig
				case 'Y':
					str.append (WNumberString (m_localtime.tm_year+1900));
					break;
				// Jahreszahl 2 stellig
				case 'y': {
					WNumberString yearStr (m_localtime.tm_year+1900, "%.4u");
					WString tmp;
					yearStr.substring (2, str);
					str.append (tmp);
				}
				break;
				// Kleingeschrieben: Ante meridiem (Vormittag) und Post meridiem (Nachmittag)
				case 'a':
					if (m_localtime.tm_hour>12)
						str.append ("pm");
					else
						str.append ("am");
					break;
				// Großgeschrieben: Ante meridiem (Vormittag) und Post meridiem (Nachmittag)
				case 'A':
					if (m_localtime.tm_hour>12)
						str.append ("PM");
					else
						str.append ("AM");
					break;
				// Stunde im 12-Stunden-Format, ohne führende Nullen
				case 'g':
					value = m_localtime.tm_hour;
					if (value>12)
						value-=12;
					str.append (WNumberString (value, "%u"));
					break;
				// Stunde im 24-Stunden-Format, ohne führende Nullen
				case 'G':
					str.append (WNumberString (m_localtime.tm_hour, "%u"));
					break;
				// Stunde im 12-Stunden-Format, mit führende Nullen
				case 'h':
					value = m_localtime.tm_hour;
					if (value>12)
						value-=12;
					str.append (WNumberString (value, "%.2u"));
					break;
				// Stunde im 24-Stunden-Format, mit führende Nullen
				case 'H':
					str.append (WNumberString (m_localtime.tm_hour, "%.2u"));
					break;
				// Minute mit führenden Nullen
				case 'i':
					str.append (WNumberString (m_localtime.tm_min, "%.2u"));
					break;
				// Sekunden, mit führenden Nullen
				case 's':
					str.append (WNumberString (m_localtime.tm_sec, "%.2u"));
					break;
				// Zeitunterschied zur Greenwich time (GMT) in Stunden
				case 'O':
				case 'P': {
					int h = (m_localtime.tm_gmtoff/60) / 60;
					int m = (m_localtime.tm_gmtoff/60) % 60;
					str.append (WFormattedString (*c=='O' ? "%s%.2u%.2u" : "%s%.2u:%.2u", h<0 ? "-" : "+", abs(h), abs(m)));
				}
				break;

				// Offset der Zeitzone von UTC in Sekunden
				case 'Z':
					str.append (WNumberString ((int) m_localtime.tm_gmtoff));
					break;
				// Fällt ein Datum in die Sommerzeit
				case 'I':
					str.append (WNumberString (m_localtime.tm_isdst));
					break;
				// Name der Zeitzone
				case 'e':
					// str.append (m_localtime.tm_isdst ? m_defaultTimeZone.getDaylightName () : m_defaultTimeZone.getName ());
					str.append (m_localtime.tm_zone);
					break;

				case '%':
					str.append ("%");
					break;

			};

			flag=false;
		}
		else if (*c=='%')
			flag=true;
		else
			str.append (*c);
		c++;
	};
}


void WDate::parse (int year, int month, int day, int hour, int minute, int second, const WTimeZone& timeZone)
{
	tm t;
	memset (&t, 0, sizeof (t));
	t.tm_year  = year  - 1900;
	t.tm_mon   = month - 1;
	t.tm_mday  = day;
	t.tm_hour  = hour;
	t.tm_min   = minute;
	t.tm_sec   = second;
	t.tm_isdst = -1;

	WTIME ts = mktime (&t);
	if (t.tm_year!=(year-1900) || t.tm_mon!=(month-1) || t.tm_mday!=day || t.tm_hour!=hour || t.tm_min!=minute || t.tm_sec!=second)
		throw WDateException ("Invalid date/time",
		                      WDateException::ERR_ID_INVALID_TIME);

	setTime (ts);
}


void WDate::setDefaultTimeZone (const WTimeZone& timeZone)
{

	if (timeZone.getName ().length ()>0) {
		setenv("TZ", timeZone.getName (), 1);
		tzset();
	}
}


bool WDate::isLeapYear (W32 year)
{
	if ((year % 400) == 0)
		return true;
	else if ((year % 100) == 0)
		return false;
	else if ((year % 4) == 0)
		return true;
	return false;
}

W32 WDate::getMaxDaysOfMonth (W32 year, W32 month)
{
	bool fLeapYear = isLeapYear (year);
	switch (month) {
		case 1:
			return 31;
			break;
		case 2:
			if (!fLeapYear) return 28;
			else return 29;
			break;
		case 3:
			return 31;
			break;
		case 4:
			return 30;
			break;
		case 5:
			return 31;
			break;
		case 6:
			return 30;
			break;
		case 7:
			return 31;
			break;
		case 8:
			return 31;
			break;
		case 9:
			return 30;
			break;
		case 10:
			return 31;
			break;
		case 11:
			return 30;
			break;
		case 12:
			return 31;
			break;
	};
	return 0;
}

W32 WDate::getDayOfYear (W32 year, W32 month, W32 day)
{
	if ((month == 0) || (month > 12)) {
		return 0;
	}
	W32 dayOfYear = day;
	W32 month2 = month;
	while (month2 > 1) {
		month2--;
		dayOfYear += getMaxDaysOfMonth(year, month2);
	}
	return dayOfYear;
}

W32 WDate::getWeekOfYear (W32 year, W32 month, W32 day)
{

	int dayOfYear = getDayOfYear (year, month, day);

	// Berechnen des Wochentags des 1. Januar:
	WDate date (year, 1, 1);
	int wDayFirstJan = date.getWeekday ();

	// Sonderfälle Freitag und Samstag
	if (wDayFirstJan >= 5)
		wDayFirstJan = wDayFirstJan - 7;

	// Sonderfälle "Jahresanfang mit KW - Nummer aus dem Vorjahr"
	if ( (dayOfYear + wDayFirstJan) <= 1) {
		return getWeekOfYear( year - 1, 12, 31);
	}

	int weekOfYear = ((dayOfYear + wDayFirstJan + 5) / 7);

	if (weekOfYear < 1 || weekOfYear>53) return 0;

	// 53 Kalenderwochen hat grundsätzlich nur ein Jahr,
	// welches mit einem Donnerstag anfängt !
	// In Schaltjahren ist es auch mit einem Mittwoch möglich, z.B. 1992
	// Andernfalls ist diese KW schon die KW1 des Folgejahres.
	if (weekOfYear == 53) {
		bool fLeapYear = WDate::isLeapYear(year);

		if (  (wDayFirstJan  ==  4) // Donnerstag
		        ||  (wDayFirstJan  == -3) // auch Donnerstag
		        ||  ((wDayFirstJan ==  3) && fLeapYear)
		        ||  ((wDayFirstJan == -4) && fLeapYear)
		   ) {
			; // Das ist korrekt und erlaubt
		}
		else
			weekOfYear = 1; // Korrektur des Wertes
	}

	return weekOfYear;
}


WTIME WDate::getEasterDate (W32 year)
{


	// Berechnet f�r ein beliebiges Jahr das Osterdatum.

	// Quelle des Gauss - Algorithmus: Stefan Gerth,
	// "Die Gaussche Osterregel", N�rnberg, Februar 2003.
	// http://krapfen.org/content/paper/Schule/Facharbeit/Berechnung_des_Osterfestes.pdf

	W32 day = 0;
	W32 month = 0;

	W32 a = year % 19;
	W32 b = year %  4;
	W32 c = year %  7;

	int k = year / 100;
	int q = k / 4;
	int p = ((8 * k) + 13) / 25;
	W32 Egz = (38 - (k - q) + p) % 30; // Die Jahrhundertepakte
	W32 M = (53 - Egz) % 30;
	W32 N = (4 + k - q) % 7;

	W32 d = ((19 * a) + M) % 30;
	W32 e = ((2 * b) + (4 * c) + (6 * d) + N) % 7;

	// Ausrechnen des Ostertermins:
	if ((22 + d + e) <= 31) {
		day = 22 + d + e;
		month = 3;
	}
	else {
		day = d + e - 9;
		month = 4;

		// Zwei Ausnahmen ber�cksichtigen:
		if (day == 26)
			day = 19;
		else if ((day == 25) && (d == 28) && (a > 10))
			day = 18;
	}

	WDate date (year, month, day);
	return date.getTime ();

	// Offsets für andere Feiertage:

	// Schwerdonnerstag / Weiberfastnacht -52
	// Rosenmontag -48
	// Fastnachtsdienstag -47
	// Aschermittwoch -46
	// Gr�ndonnerstag -3
	// Karfreitag -2
	// Ostersonntag 0
	// Ostermontag +1
	// Christi Himmelfahrt +39
	// Pfingstsonntag +49
	// Pfingstmontag +50
	// Fronleichnam +60
	// Maria Himmelfahrt ist stets am 15. August


}


const WString& WDate::toString () const
{
	return format (WLIB_DEFAULT_DATE_TIME_FORMAT);
}


bool operator == (const WDate& date1, const WDate& date2)
{
	return date1.getTime () == date2.getTime ();
};
bool operator >  (const WDate& date1, const WDate& date2)
{
	return date1.getTime () > date2.getTime ();
};
bool operator >=  (const WDate& date1, const WDate& date2)
{
	return date1.getTime () >= date2.getTime ();
};

bool operator < (const WDate& date1, const WDate& date2)
{
	return date1.getTime () < date2.getTime ();
};

bool operator <= (const WDate& date1, const WDate& date2)
{
	return date1.getTime () <= date2.getTime ();
};


////////////////////
// WTimeZone
////////////////////

WTimeZone::WTimeZone ()
{
	initialize ();
}

void WTimeZone::initialize ()
{
	m_bias = m_daylightBias = m_standardBias = 0;
	m_isDaylightZone = false;
	m_name = m_daylightName = ""; // Default
}

WTimeZone::WTimeZone (
    WCSTR       name,
    int         bias)
{

	initialize ();
	m_name = m_daylightName = name;
	m_bias = bias;

}

WTimeZone::WTimeZone (
    WCSTR       name,
    WCSTR       daylightName,
    int         bias,
    const Date& standardDate,
    int         standardBias,
    const Date& daylightDate,
    int         daylightBias)
{
	initialize ();
	m_name = name;
	m_daylightName = daylightName;
	m_bias = bias;
	m_standardDate = standardDate;
	m_standardBias = standardBias;
	m_daylightDate = daylightDate;
	m_daylightBias = daylightBias;
	if (standardDate.month!=0)
		m_isDaylightZone = true;
}



WTimeZone::Date::Date ()
{
	dayOfWeek = weekCode = month = hour = minute = 0;
}

WTimeZone::Date::Date (int month, int dayOfWeek, int weekCode, int hour, int minute)
{
	this->month = month;
	this->dayOfWeek = dayOfWeek;
	this->weekCode = weekCode;
	this->hour = hour;
	this->minute = minute;
}

int WTimeZone::calcBias (int year, int month, int day, int hour,
                         int minute, bool& isDayLight) const
{
	isDayLight =false;
	if (m_isDaylightZone) {
		if ((month<m_standardDate.month && month>=m_daylightDate.month)) {
			int tzDay = getCurrentTimeZoneDay (year, m_daylightDate.month,
			                                   m_daylightDate.dayOfWeek, m_daylightDate.weekCode);

			if (month>m_daylightDate.month)
				isDayLight = true;
			else if (day>tzDay)
				isDayLight=true;
			else if (day==tzDay) {
				if (hour>m_daylightDate.hour)
					isDayLight=true;
				else if (hour==m_daylightDate.hour &&
				         minute>=m_daylightDate.minute)
					isDayLight=true;
			}
		}
		else if (month==m_standardDate.month) {
			int tzDay = getCurrentTimeZoneDay (year, m_standardDate.month,
			                                   m_standardDate.dayOfWeek, m_standardDate.weekCode);

			if (day<tzDay)
				isDayLight = true;
			else if (hour<m_standardDate.hour)
				isDayLight = true;
			else if (minute<m_standardDate.minute)
				isDayLight = true;
		}

		if (isDayLight)
			return m_bias + m_daylightBias;
	}
	return m_bias + m_standardBias;
}

W32 WTimeZone::getCurrentTimeZoneDay (W32 tzYear, W32 tzMonth, W32 tzDayOfWeek, W32 tzWeekCode)
{


	W32 week=1, weekDay=0;

	int maxDays = WDate::getMaxDaysOfMonth
	              (tzYear, tzMonth);

	// Get first week day
	/*
	SYSTEMTIME st, lt;
	memset (&st, 0, sizeof (st));
	st.wDay    = 1;
	st.wMonth  = (WORD) tzMonth;
	st.wYear   = (WORD) tzYear;
	st.wHour   = 0;
	st.wMinute = 0;
	st.wSecond = 0;

	ULARGE_INTEGER src;
	if (!SystemTimeToFileTime (&st, (LPFILETIME) &src))
	    throw WDateException ("Invalid date/time",
	        WDateException::ERR_ID_INVALID_TIME);
	FileTimeToSystemTime ((LPFILETIME) &src, &lt);
	weekDay = lt.wDayOfWeek;
	*/

	if (weekDay==0)
		weekDay=7;
	if (tzDayOfWeek==0)
		tzDayOfWeek=7;

	// Count to correct time zone day
	for (int day=1; day<=maxDays; day++) {
		if (weekDay==tzDayOfWeek) {
			if (tzWeekCode>=1 &&
			        tzWeekCode<=4 &&
			        tzWeekCode==week) {
				return day;
			}
			else if (tzWeekCode==5 &&  // last week
			         (day+7)>maxDays)
				return day;
		}
		weekDay++;
		if (weekDay>7) {
			weekDay=1;
			week++; // next week
		}
	}
	return 0;
}

