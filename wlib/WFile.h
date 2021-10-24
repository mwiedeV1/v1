/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/


#ifndef WFILE_H
#define WFILE_H


#include "WSystem.h"

#include <sys/stat.h>
#include <dirent.h>

/**
 WFileException ist die Ausnahme von WFile
 und wird bei generellen Fehlern beim Dateizuguiff geworfen.
 @see WFile
*/
class _WLIBAPI WFileException: public WException {
public:
	enum {
		ERR_ID_CREATE = -1,
		ERR_ID_OPEN   = -2,
		ERR_ID_WRITE  = -3,
		ERR_ID_READ   = -4,
		ERR_ID_ACCESS = -5
	};
	WFileException (WCSTR msg, int id) : WException ()
	{
		set ("WFileException", msg, id, errno);
	};
};

/**
 WEOFException wird geworfen, wenn bei einem Lesezugriff
 das Ende der Datei erreicht wurde.
 Die noch gelesenen Bytes können mit der Methode "getEOFBytes"
 in der WFileIO oder WFile Klasse ermittelt werden.
 @see WFileIO
 @see WFile
*/
class _WLIBAPI WEOFException : public WException {
public:
	WEOFException (WCSTR msg) : WException ()
	{
		set ("WEOFException", msg, -1, errno);
	};
};

/**
 WFileNotFoundException wird geworfen, wenn
 eine Datei nicht gefunden wurde, d.h. nicht existiert.
*/
class _WLIBAPI WFileNotFoundException : public WException {
public:
	WFileNotFoundException (WCSTR msg) : WException ()
	{
		set ("WFileNotFoundException", msg, -1, errno);
	};
};


// predefinitions
class WFileInfo;

class _WLIBAPI WFile {
public:
	enum {
		FILE_READ           = 1,
		FILE_WRITE          = 2,
		FILE_CREATE         = 4,
		FILE_APPEND         = 8,
		READLN_BLOCKSIZE    = 128,
		POS_MODE_BEGIN      = SEEK_SET,
		POS_MODE_CURRENT    = SEEK_CUR,
		POS_MODE_END        = SEEK_END
	};

	WFile ();
	WFile (WCSTR filename);
	WFile (FILE* hFile, bool fBlockDevice=true, bool fClosable=true);
	~WFile ();

	void open               (FILE* hFile, bool fBlockDevice=true, bool fClosable=true);
	void open               (WCSTR filename, W32 flag=FILE_READ);
	void create             (WCSTR filename, bool fAppend = false);
	void close              ();

	W32  write              (const void* pnt, W32 size);
	W32  write              (WCSTR str);
	W32  writeln            (WCSTR str);
	void flush              ();

	W32  read               (void* pnt, W32 size);
	bool readln             (WString& str, bool fRemoveLF=true);
	int readByte            ();

	/** Dateizeiger setzen **/
	void mark               ();
	void reset              ();
	void setPosMode         (int mode);
	void setPos             (int pos);
	void skip               (int distance);

	/* Verschiedenes */
	void setClosable        (bool fClosable);

	/** Dateiattribute ermitteln **/
	W32 getPos              () const;
	W32 getSize             () const;
	W32 getEOFBytes         () const;
	bool isEOF              () const;
	WCSTR getFilename       () const;
	bool getRealFilename    (WString& filename) const;
	bool isOpen             () const;
	bool getFileInfo        (WFileInfo& fileInfo) const;
	FILE* getFileHandle () const;


	/** Cast-Operator für Windows-Funktionen */
	inline operator FILE* () const
	{
		return getFileHandle ();
	};



private:
	FILE*   m_hFile;          // filehandle
	WString m_filename;       // filename
	int     m_posMode;        // pos mode of filepointer
	bool    m_fEOF;           // mark end of file
	W32     m_markPos;        // marked filepointer
	W32     m_eofBytes;       // number of bytes read/wrote before WEOFException was throwed
	bool    m_fBlockDevice;   // true, if its a block device and setPos, getSize, getEOFBytes not work
	bool    m_fNoFilepointer; // File disallow file pointer operations
	W32     m_ownFilePos;     // File pos for streams with no filepointer
	bool    m_fClosable;      // true, if file handle can be closed
	W8      m_readlnBuf[READLN_BLOCKSIZE]; // Internal buffer for readln
	int     m_readlnBufCnt;   // Bytes in readln buffer
	int     m_readlnBufPos;   // Current pos in readln buffer


	// helpers
	void initialize ();
};


/**
 WFileInfo stellt Informationen über eine Datei
 oder ein Verzeichnis zur Verfügung.
*/
class WDirectoryEntries;

class _WLIBAPI WFileInfo {
public:
	/**
	 Dateityp
	 Diese Konstanten können OR verknüpft sein
	*/
	enum {
		FILE_TYPE_DIRECTORY  = S_IFDIR,
		FILE_TYPE_NORMAL     = S_IFREG,
	};

	WFileInfo ();
	~WFileInfo ();

	/** Dateiname inkl. Path **/
	WCCSTR getFilename () const
	{
		return m_filename;
	};

	/**
	 Dateityp, d.h. ein oder mehrere
	 OR verknüpfte Konstanten aus FILE_TYPE_XXX **/
	W32 getFileType () const
	{
		return m_fileType;
	};

	/** Größe der Datei in Bytes **/
	W64 getFileSize () const
	{
		return m_fileSize;
	};

	/** Zeit (C-Time) als die Datei erzeugt wurde **/
	WTIME getTimeCreation () const
	{
		return m_timeCreation;
	};

	/** Zeit (C-Time) des letzten Zugriffes auf die Datei **/
	WTIME getTimeLastAccess () const
	{
		return m_timeLastAccess;
	};

	/** Zeit (C-Time) der letzten Modifikation der Datei **/
	WTIME getTimeLastModified () const
	{
		return m_timeLastModified;
	};

private:
	WString  m_filename;
	W32      m_fileType;
	W64      m_fileSize;
	WTIME    m_timeCreation;
	WTIME    m_timeLastAccess;
	WTIME    m_timeLastModified;

	friend class WFile;
	friend class WDirectoryEntries;
};



/**
 WDirectoryEntries ermöglicht das Auslesen eines
 Verzeichnisses.

 @see WFileInfo
*/
class _WLIBAPI WDirectoryEntries {
public:
	/**
	 Öffnet ein Verzeichnis, um den Inhalt zu lesen.
	 @param path Verzeichnis
	 @param ext Datei-Erweiterungen, die gelesen werden
	 sollen, z.B. "bmp". Es sind auch Wildchars möglich,
	 z.B. "A*.gif".
	 */
	WDirectoryEntries (WCSTR path, WCSTR ext="");
	~WDirectoryEntries ();

	/**
	 Liest den nächsten Eintrag aus dem Verzeichnis.
	 @param fileInfo Informationen über den gelesenen Eintrag
	 @return true, wenn weitere Einträge folgen
	*/
	bool nextEntry (WFileInfo& fileInfo);

private:


	DIR* m_dir;
	WString m_path, m_ext;
};




#endif /* WFILE_H */
