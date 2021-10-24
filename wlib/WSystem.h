/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef _WSYSTEM_H
#define _WSYSTEM_H


#include "WMemory.h"
#include "WHashtable.h"
#include "WDate.h"

#include <pthread.h>

#define WAIT_FOREVER 0
#define WMUTEX pthread_mutex_t

// predefinition
class WSystem;

/**
 WMUTEX ist ein einfacher Mutex für Thread-Synchronisation.
 Unter WIN32 ist es ein "Critcal-Section"-Objekt, unter POSIX ein
 normaler Mutex.
 Alle Bearbeitungs-Routinen sind unter WSystem und WThread verfügbar.

 WMUTEX mutex;
 WSystem::initMutex (m_mutex);
 // ...

 WSystem::enterMutex (m_mutex);
 // this section is MT-Safe
 WSystem::leaveMutex (m_mutex);

 // ...
 WSystem::deleteMutex (m_mutex);



 @see WSystem
 @see WThread
 @see WMutex
 @see WMutexLocker
 @author Matthias Wiede <Matthias.Wiede@web.de>


*/


/**
 WSystemInfo enthält Informationen über das aktuelle System.
 Eine globale Instanz "g_systemInfo" wird in der "startup" Methode
 von WSystem initialisiert.

 @see WSystem
*/
class _WLIBAPI WSystemInfo {
public:
	WSystemInfo ();

	/** Informationen über lokale Zeitzone **/
	const WTimeZone& getLocalTimeZone () const
	{
		return m_localTimeZone;
	};

private:
	WTimeZone m_localTimeZone;
};

/** Globale Systeminformationen */
extern _WLIBAPI WSystemInfo g_systemInfo;

/** Globaler Mutex **/
extern _WLIBAPI WMUTEX g_mutex;


/**
 WSystemException ist die Ausnahme von WSystem.
 Diese Exception kann auch von anderen Klassen geworfen
 werden.

 @see WSystem
*/

class _WLIBAPI WSystemException : public WException {
public:
	enum {
		ERR_ID_GENERAL              = -1,
		ERR_ID_UNSUPPORTED_LOCALE   = -2,
	};

	inline WSystemException (WCSTR msg) : WException ()
	{
		set ("WSystemException", msg, ERR_ID_GENERAL, errno);
	};

	inline WSystemException (WCSTR msg, int id) : WException ()
	{
		set ("WSystemException", msg, id, errno);
	};
};


/**
 WSystem stellt systemnahe statische Funktionen zur Verfügung,
 z.B. für Speicher-Allokation, Mutexe, Programm-Initialisierung usw.
 Die Klasse stellt auch Startup-Methoden für Windows-Anwendugen bereit.

 @author Matthias Wiede <Matthias.Wiede@web.de>
*/

class _WLIBAPI WSystem : public WMemory {
public:

	/** Startup für Konsolen-Programme **/
	static void             startup                 (WCSTR programName="");

	/** Schließt alle Handles und Ressourcen der WLib **/
	static void             shutdown                ();

	/** Anwendung schließen - Windows-Funktion "PostQuitMessage" **/
	static void             exit                    (int exitCode);

	/** Gibt die globalen System-Informationen zurück **/
	static const WSystemInfo&  getSystemInfo        ();

	/** Setzt die Ländereinstellungen für ein bestimmtes Land (in Arbeit!!!) **/
	static void             setLocale               (WCSTR country);

	/** Mutexe und Synchronisation **/
	static void             initMutex               (WMUTEX& mutex);
	static void             enterMutex              (WMUTEX& mutex);
	static bool             tryEnterMutex           (WMUTEX& mutex);
	static void             leaveMutex              (WMUTEX& mutex);
	static void             deleteMutex             (WMUTEX& mutex);
	static void             sleep                   (W32 sec);
	static void             sleepMs                 (W32 ms);


	/** Prüft ob eine Datei oder Verzeichnis existiert, fDir: 0 = Datei, 1 = Verzeichnis, 2 = Datei oder Verzeichnis **/
	static bool             existsFile              (WCSTR filename, int fDir=0);

	/** Extrahiert die Erweiterung aus einem Dateinamen **/
	static bool             extractFileExtension    (WCSTR filename, WString& extension);
	/** Extrahiert den Pfad bzw. Dateiname **/
	static void             extractPath             (WCSTR filename, WString& path, bool fBasename=false);

	/** Hochauflösenden Zeitzähler ermitteln **/
	static bool             getPerformanceFrequency (W64& frequency);
	static bool             getPerformanceCounter   (W64& counter);
	static bool             getPerformanceTimeScale (double& timeScale);

	/** Verschiedenes **/
	static void             getCurrentDirectory     (WString& dir);
	static void             getTempDirectory        (WString& dir);
	static bool             setCurrentDirectory     (WCSTR dir);
	static bool             createDirectory         (WCSTR dir);
	static bool             removeDirectory         (WCSTR dir);
	static bool             removeFile              (WCSTR filename);
	static bool             renameFile              (WCSTR oldFilename, WCSTR newFilename);
	static bool             copyFile                (WCSTR oldFilename, WCSTR newFilename);
	static W32              getSysErrCode           ();

	static int              getch                   ();
	static bool             setConsoleEcho          (bool on);

	/** 32 Bit CRC berechnen **/
	static void             calcCRC32               (const W8* buf, W32 size, W32& crc32);

	/**
	  Double Zahlen runden
	  Beispiele:
	    round(2.15, 1)      = 2.2
	    round(2.149, 1)     = 2.1
	    round(-1.475, 2)    = -1.48

	**/
	static double           round                   (const double& number, int digits = 0);
};






#endif /* _WSYSTEM_H */
