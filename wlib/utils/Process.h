/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef _Process_h
#define _Process_h

#include "wlib.h"

/**
  Process
*/

class Process { //  public WRunable
public:

	class Handles {
	public:
		Handles ();
		~Handles ();
		void free ();
		int aStdinPipe[2];
		int aStdoutPipe[2];
		int hProcess;
	};

	Process ();
	Process (WCSTR cmdLine);
	~Process ();

	bool execute (WCSTR cmdLine, void* env=NULL, bool fSecure=false);
	void startRead ();
	void join (W32 ms = 0);

	/* Kill CGI process immediately */
	void kill ();

	bool isRunning () const;

	int getExitCode () const;

	/* Own status */
	void setBusy (bool on);
	bool isBusy () const;

	const Handles* getHandles () const
	{
		return &m_handles;
	}

private:
	void error (WCSTR errText);
	Handles m_handles;
	bool m_fBusy;
	WString m_cmdLine;

};

typedef WHashtable<WString,Process*,WCSTR,Process*> ProcessHT;

#endif /* _Process_h */
