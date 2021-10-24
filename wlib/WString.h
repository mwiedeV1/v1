/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/


#ifndef _WSTRING_H
#define _WSTRING_H

#include "wbase.h"
#include <string.h>

/**
 WString kapselt eine Zeichenkette oder ein Byte Array.

 \begin{verbatim}
 WString str = "I am a String";

 str += " Appended String";
 \end{verbatim}

*/
class WString {

public:

	/** Kostruktor erzeugt einen leeren String. */
	inline WString ()
	{
		initialize (0);
	};

	/**
	 Konstruktor erzeugt einen leeren String mit bestimmer Puffergröße.
	 Die Puffergröße ist die Größe des internen Puffers in Zeichen (Bytes).
	 Der eigentliche String hat bei der Erzeugung eine Länge von 0.
	 Die Angabe der Puffergröße ist bei folgenden Situationen nötig:

	 1. Bei der Methode format, da diese Funktion den String nicht automatisch vergrößert.
	 Die Größe des Puffers zeigt an, wie groß der formatierte String maximal sein darf.

	 2. Bei performanten Operationen und beim Hinzufügen von Strings.
	 Hier entfällt die automatische Vergrößerung des Puffers,
	 wenn der Anfangspuffer groß genug gewählt wurde.

	 Hinweis: Der interne Puffer wird immer um ein Zeichen (Byte) mit dem Wert 0 erweitert.
	 Wenn die Anfangsgröße z.B. mit 90 eingestellt wurde, dann
	 ist die wahre Puffergröße 91 Zeichen (Bytes). Dies muss beachtet werden, wenn der interne
	 Puffer direkt manipuliert wird.

	 @param length Länge des Anfangspuffers in Zeichen (Bytes).

	 @see getBuf ()
	 @see getBufLength ()
	 @see FormattedString
	*/
	inline WString (W32 length)
	{
		initialize (length);
	};

	/** Konstruktor weist einen neuen String zu. */
	inline WString (const WString& str)
	{
		initialize (str.length ());
		assign ((WCSTR) str, str.length ());
	};

	/** Konstruktor weist einen neuen String zu. */
	inline WString (WCSTR str)
	{
		W32 length;
		initialize (length = strlen (str));
		assign (str, length);
	};

	/** Konstruktor weist einen neuen String mit bestimmter Länge zu. */
	inline WString (WCSTR str, W32 length)
	{
		initialize (length);
		assign (str, length);
	};

	/** Konstruktor weist eine Byte-Array zu. */
	inline WString (const W8* array, W32 length)
	{
		initialize (length);
		assign ((WCSTR) array, length);
	};

	/** Zuweisungsoperator weist einen neuen String zu. */
	inline WString& operator = (WCSTR str)
	{
		_assign (str, strlen (str));
		return *this;
	};
	inline WString& operator = (const WString& str)
	{
		_assign (str, str.length ());
		return *this;
	};

	/** Destruktor. */
	virtual ~WString ();

	/**
	 Allokiert internen Puffer.
	 Der interne Puffer des String wird vergrößert oder verkleinert.
	 Die Länge des Strings wird angepasst, falls der Länge des neuen Puffers
	 kleiner als die Länge des aktuellen String ist.
	 @param length Puffergröße in Zeichen (Bytes)
	 @see getBuf ()
	 @see getBufLength ()
	 @see WString (W32 length)
	*/
	WString& resize (W32 length);

	/** Weist einen neuen String zu. */
	inline WString& assign (WCSTR str)
	{
		_assign (str, strlen (str));
		return *this;
	}
	inline WString& assign (const WString& str)
	{
		_assign (str, str.length ());
		return *this;
	}

	/** Weist einen neuen String mit bestimmter Länge zu. */
	inline WString& assign (WCSTR str, W32 length)
	{
		_assign (str, length);
		return *this;
	}

	/**
	 Weist einen neuen Sub-String zu.
	 Diese Methode setzt den Sub-String von Position pos0 bis pos1.
	 \begin{verbatim}
	 WString str ("I am a String", 0, 8);
	 prinft ("%s\n", (WCSTR) str);
	 \end{verbatim}
	 Zuweisung: "am a Str"
	*/
	inline WString& assign (WCSTR str, W32 pos0, W32 pos1)
	{
		_assign (&str[pos0], pos1-pos0);
		return *this;
	}

	/** Weisst ein neues Byte-Array zu. */
	inline WString& assign (const W8* array, W32 length)
	{
		_assign ((WCSTR) array, length);
		return *this;
	};

	/**
	 Schneidet den String ab einer bestimten Position ab.
	 Die Länge darf nicht größer als die Länge des existierenden Strings bzw.
	 des internen Puffers sein. Diese Methode wird verwendet, um einen String
	 zu verkürzen.
	 Für C-Strings wird standardmäßig das Terminierungszeichen (0) eingefügt.
	*/
	inline void truncate (W32 length)
	{
		if (length>m_bufLength)
			length=m_bufLength-1;
		m_str[length] = 0;
		m_strLength = length;
	}

	/** Fügt ein Byte-Array an den existierenden String an. */
	inline WString& append (const W8* array, W32 length)
	{
		_append ((WCSTR) array, length);
		return *this;
	};

	/** Fügt einen String an den existierenden String an. */
	inline WString& append (WCSTR str)
	{
		_append (str, strlen (str));
		return *this;
	};

	/** Fügt einen String an den existierenden String an. */
	inline WString& append (const WString& str)
	{
		_append (str, str.length ());
		return *this;
	};


	/** Fügt einen Zeichen an den existierenden String an. */
	inline WString& append (int c)
	{
		_append ( (WCSTR) &c, 1);
		return *this;
	};


	/**
	  Gibt die Länge des Strings zurück.
	  Die Länge des Strings ist die Anzahl der Zeichen bis zum Terminierungszeichen (0),
	  nicht die Größe des internen Puffers.
	*/
	inline W32 length () const
	{
		return m_strLength;
	};

	/** Vergleicht, ob zwei Strings identisch sind. */
	inline bool equals (WCSTR str) const
	{
		return compare (str) == 0;
	};

	/** Vergleicht, ob zwei Strings identisch sind. */
	inline bool equals (const WString& str) const
	{
		if (m_strLength==str.length ())
			return memcmp (m_str, str.getBuf (), m_strLength) == 0;
		return false;
	};

	/**
	 Vergleicht zwei Strings.
	 @return < 0 aktueller String ist kleiner als str
	           0 aktueller String ist identisch mit str
	         > 0 aktueller String ist größer als str
	*/
	inline int compare (const WString& str) const
	{
		return strcmp (m_str, str);
	};

	inline int compare (WCSTR str) const
	{
		return strcmp (m_str, str);
	};

	/**
	 Füllt den String mit einem bestimmten Zeichen auf.
	*/
	inline WString& fill (W8 fillByte=0)
	{
		memset (m_str, (int) fillByte, m_strLength);
		return* this;
	};

	/**
	 Löscht den String.
	 Der String wird deallokiert und auf die Länge 0 gesetzt.
	*/
	inline WString& clear ()
	{
		resize (0);
		return* this;
	};

	/**
	 Gibt den ersten Index eines zu suchenden Strings zurück.
	 Diese Methode durchsucht den existierenden String nach einem
	 String und gibt die Start-Position dieses Strings zurück.
	 @return Index oder -1, wenn der String nicht gefunden wurde.
	*/
	inline int indexOf (WCSTR str) const
	{
		return indexOf (m_str, str);
	}

	/**
	 Gibt den ersten Index eines zu suchenden Zeichens zurück.
	 Diese Methode durchsucht den existierenden String nach einem
	 Zeichen und gibt den Index des ersten gefundenen Zeichens zurück.
	 @return Index oder -1, wenn das Zeichen nicht gefunden wurde.
	*/
	inline int lastIndexOf (char value) const
	{
		return lastIndexOf (m_str, m_strLength, value);
	}

	/** Ersetzt ein bestimmtes Zeichen im gesamten String. */
	inline WString& replace (char oldChar, char newChar)
	{
		replace (m_str, m_strLength, oldChar, newChar);
		return *this;
	}

	/** Entfernt links- und rechts stehende Leerzeichen (0x20) und Tabulatoren (0x09). */
	inline WString& trim (char extraTrimChar=9)
	{
		m_strLength = trim (m_str, m_strLength, extraTrimChar);
		return *this;
	}

	/** Prüft, ob ein String mit einem bestimmten Zeichen beginnt. */
	inline bool startsWith (int c) const
	{
		return m_str[0]==c;
	}

	/** Prüft, ob ein String mit einem bestimmten String beginnt. */
	inline bool startsWith (WCSTR str) const
	{
		return startsWith (m_str, str);
	}

	/* Wandelt alle Buchstaben in Kleinbuchstaben um. */
	inline WString& toLowerCase ()
	{
		W8* str = (W8*) m_str;
		while (*str!=0) {
			*str = this->tolower (*str);
			str++;
		}
		return* this;
	}

	/* Wandelt alle Buchstaben in Großbuchstaben um.*/
	inline WString& toUpperCase ()
	{
		W8* str = (W8*) m_str;
		while (*str!=0) {
			*str = this->toupper (*str);
			str++;
		}
		return* this;
	}

	// emulation routines for string conversion
	static W8 toupper (W8 c)
	{
#ifdef _USE_LOCALE
		return (W8) ::toupper ((int) c);
#else
		if (c==228) return 196;
		if (c==246) return 214;
		if (c==252) return 220;
		if ((c>=97) && (c<=122))
			return c-32;
#endif
		return c;
	}

	static W8 tolower (W8 c)
	{
#ifdef _USE_LOCALE
		return (W8) ::tolower ((int) c);
#else
		if (c==196) return 228;
		if (c==214) return 246;
		if (c==220) return 252;
		if ((c>=65) && (c<=90))
			return c+32;
#endif
		return c;
	}
	/**
	 Gibt ein Zeichen von einem bestimmten Index zurück.
	@param idx Position von der das Zeichen gelesen wird
	@return Zeichen oder -1 wenn Index ausserhalb des Strings ist.
	*/
	inline int charAt (W32 idx) const
	{
		if (idx<m_strLength)
			return (int) m_str[idx];
		return -1;
	}

	/** Kopiert den Sub-String von Position pos0 bis pos1.
	Diese Methode kopiert einen Sub-String aus dem existierenden
	String von einschließlich Position pos0 bis pos1.
	\begin{verbatim}
	WString substr, str = "I am a String";
	str.substring (2, 10, substr);
	printf ("%s\n", (WCSTR) substr);
	\end{verbatim}
	Ausgabe: "am a Str"
	*/
	void substring (W32 pos0, W32 pos1, WString& str) const;

	/** Kopiert den Sub-String von Position pos0 bis zum Ende. */
	void substring (W32 pos0, WString& str) const;

	/**
	Spaltet den String in zwei separierte Teile.
	Nützlich z.B. zum Separieren von Schlüssel/Wert-Paaren.
	@param splitStr Trennzeichen, z.B. "="
	@param key Referenz auf einen String zum Speichern des linken Teils
	@param value Referenz auf einen String zum Speichern des rechten Teils
	@param fTrim true, wenn links- und rechts stehende Leerzeichen entfernt werden sollen
	@return true, wenn Trennzeichen gefunden, sonst false
	*/
	bool split (WCSTR splitStr, WString& key, WString& value, bool fTrim=true) const;


	/** Gibt einen Zeiger auf den internen Puffer zurück. */
	inline W8* getBuf () const
	{
		return (W8*) m_str;
	}

	/** Gibt die Länge des internen Puffers in Bytes zurück. */
	inline W32 getBufLength () const
	{
		return m_bufLength;
	}

	/** Operator hängt einen neuen String an den existierenden String an. */
	inline WString& operator += (WCSTR str)
	{
		return append (str);
	};

	/** Operator hängt einen neuen String an den existierenden String an. */
	inline WString& operator += (const WString& str)
	{
		return append (str);
	};


	/** Operator fügt zwei Strings zusammen */
	friend WString operator + (WCSTR str1, const WString& str2);
	friend WString operator + (const WString& str1, WCSTR str2);
	friend WString operator + (const WString& str1, const WString &str2);

	/** Vergleichsoperator prüft, ob zwei Strings identisch sind. */
	friend bool operator == (const WString& str1, const WString& str2);
	friend bool operator == (WCSTR str1, const WString& str2);
	friend bool operator == (const WString& str1, WCSTR str2);

	/** Array-Operator gibt ein Zeichen von einem bestimmten Index zurück
	 oder -1, wenn der Index ausserhalb des Strings ist.*/
	inline int operator[] (W32 idx) const
	{
		return charAt (idx);
	}

	/** Cast-Operator für WCSTR. */
	inline operator WCSTR () const
	{
		return m_str;
	};

protected:
	char* m_str;       // buffer with string
	char* m_str0;      // zero string
	W32   m_strLength; // length of string
	W32   m_bufLength; // length of the buffer

	void initialize (W32 length);

	// static methods do the work to minimize object size
	void _append  (WCSTR strAdd, W32 lengthAdd);
	void _assign  (WCSTR strNew, W32 lengthNew);
	static void  replace     (char* str, W32 length, char  oldChar, char newChar);
	static int   indexOf     (char* str, WCSTR findStr);
	static int   lastIndexOf (char* str, W32 length, char value);
	static bool  startsWith  (char* src, WCSTR str);
	static W32   trim        (char* str, W32 length, char extraTrimChar=9);

};


/**
 WFormattedString ermöglicht das Formatieren eines
 Strings mit variablen Argumenten.

 \begin{verbatim}
 WFormattedString str ("%s = %i", (WCSTR) "NAME", 10);
 \end{verbatim}

 Nützlich z.B. zum Generieren formatierter Ausnahmen.

 \begin{verbatim}
 throw WException (WFormattedString ("Error: %s = %i", (WCSTR) "NAME", 10));
 \end{verbatim}


 @see WString
*/
class WFormattedString : public WString {
public:
	enum {
		/** Standard Puffergröße in Zeichen **/
		DEFAULT_BUF_SIZE = 1024
	};

	/**
	 Konstruktor erzeugt einen leeren String.
	 Die Puffergröße wird bei der ersten Formatierung auf DEFAULT_BUF_SIZE
	 gesetzt, sofern sie vorher nicht durch die "resize"
	 Methode manuell gesetzt wurde.
	*/
	WFormattedString ();


	/**
	 Konstruktor erzeugt einen leeren String mit bestimmer Puffergröße.
	*/
	WFormattedString (W32 length);

	/**
	  Konstruktor formatiert einen String mit variablen Argumenten.
	  Die interne Puffergröße wird auf DEFAULT_BUF_SIZE gesetzt.
	  @see format
	 */

	WFormattedString (WCSTR format, ...);

	/**
	 Weist einen formatierten String zu.
	 Diese Methode ist äquivalent mit der Funktion snprintf aus
	 der C-Library. Zur Formatierung werden die standardisierten Formatcodes verwendet.
	 Wichtig: Der interne Puffer des Strings muss groß genug sein, um den
	 formatierten String aufzunehmen.
	 \begin{verbatim}
	 WFormattedString* strPnt = new WFormattedString (128);
	 strPnt->snprintf ("Number=%i, Str=%s", 15, "I am a string.");
	 \end{verbatim}
	 Zuweisung: "Number=15, Str=I am a string"
	 @return Referenz auf String.
	 @see resize
	*/
	WString& format (WCSTR format, ...);
};


class WNumberString : public WFormattedString {

public:
	enum NumberType {
		NUMBER_TYPE_INT    = 1,
		NUMBER_TYPE_W32    = 3,
		NUMBER_TYPE_W64    = 4,
		NUMBER_TYPE_FLOAT  = 5,
		NUMBER_TYPE_DOUBLE = 6
	};


	inline WNumberString (int value, WCSTR format = "%i")
		: WFormattedString (128)
	{
		m_numberType = NUMBER_TYPE_INT;
		WFormattedString::format (format, value);
	};

	inline WNumberString (W32 value, WCSTR format = "%u")
		: WFormattedString (128)
	{
		m_numberType = NUMBER_TYPE_W32;
		WFormattedString::format (format, value);
	};

	inline NumberType getNumberType () const
	{
		return m_numberType;
	}

private:
	NumberType m_numberType;
};



#ifndef WCCSTR
typedef const WString& WCCSTR;
#endif

const WString g_nullStr;

#endif /* _WSTRING_H */
