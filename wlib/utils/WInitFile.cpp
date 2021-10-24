#include "WInitFile.h"

WInitFile::WInitFile ()
{
}

WInitFile::~WInitFile ()
{
}

void WInitFile::open (WCSTR filename, WCSTR sep)
{
	WFile file;
	file.open (filename);
	read (file, m_ht, sep);
	file.close ();
}

void WInitFile::save (WCSTR filename, WCSTR sep)
{
	WFile file;
	file.create  (filename, false);
	m_ht.reset ();
	while (m_ht.hasMoreElements ()) {
		file.write   (m_ht.nextKey   ());
		file.write   (sep);
		file.write   (m_ht.nextValue ());
		file.write   ("\r\n");
	}
	file.flush   ();
}

void WInitFile::close ()
{
	m_ht.removeAll ();
}

void WInitFile::putValue (WCSTR key, WCSTR value)
{
	m_ht.put (key, WString(value));
}

void WInitFile::putValue (WCSTR key, int value)
{
	putValue (key, WFormattedString ("%i", value));
}

void WInitFile::putValue (WCSTR key, W32 value)
{
	putValue (key, WFormattedString ("%u", value));
}

void WInitFile::putValue (WCSTR key, bool value)
{
	if (value)
		putValue (key, "true");
	else
		putValue (key, "false");
}

void  WInitFile::putValue (WCSTR key, double  value, int digits)
{
	WString format = WFormattedString ("%%.%uf", (int) digits);
	putValue (key, WFormattedString (format, (double) value));
}

void WInitFile::read (WFile& file, WInitFile::HT& ht, WCSTR sep)
{
	WString line;
	while (file.readln (line)) {
		line.trim ();
		if (!line.startsWith ('#')) {
			// parse key-value
			int pos =  line.indexOf (sep);
			if (pos!=-1) {
				WString k, v;
				line.substring (0,pos,k);
				line.substring (pos+1,v);
				k.trim ();
				v.trim ();
				// store in hashtable
				ht.put (k, v);
			}
		}
	}
}

WCSTR WInitFile::getValue (WCSTR key, WCSTR def) const
{
	if (m_ht.isKey (key))
		return m_ht.get (key);
	return def;
}

int WInitFile::getIntValue (WCSTR key, int def) const
{
	if (m_ht.isKey (key))
		return atoi (m_ht.get (key));
	return def;
}


bool WInitFile::getFlagValue (WCSTR key, bool def) const
{
	if (m_ht.isKey (key)) {
		WString str = m_ht.get (key);
		str.toLowerCase ();
		return str.equals ("true") || str.equals ("on") || str.equals ("yes");
	}
	return def;
}

bool WInitFile::isParam (WCSTR key) const
{
	return m_ht.isKey (key);
}