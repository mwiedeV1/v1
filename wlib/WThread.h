/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef _WTHREAD_H
#define _WTHREAD_H

#include "WSystem.h"
#include <pthread.h>

#define WAIT_FOREVER 0

// mutext struct
#define WMUTEX pthread_mutex_t

// WThreadException
class WThreadException : public WException {
public:
	enum {
		ERR_ID_THREAD_CREATE = -1,
		ERR_ID_THREAD_SET_PRIORITY = -2,
		ERR_ID_THREAD_KILL = -3
	};

	WThreadException (WCSTR msg, int id) : WException ()
	{
		set ("WThreadException", msg, id, errno);
	};
};

/**
 WSemaphore wird f�r Z�hlfunktionen in Threads verwendet.
*/
class WSemaphore {
	bool m_fActive;
	volatile int m_cnt;
	pthread_condattr_t m_attr;
	WMUTEX m_mutex;
	pthread_cond_t m_cond;
	bool timedWait (W32 ms);
	void initialize ();
public:
	enum {
		SEMAPHORE_LOCKED   = 1,
		SEMAPHORE_UNLOCKED = 2
	};
	WSemaphore  ();
	WSemaphore  (int cnt);
	~WSemaphore  ();
	void create (int cnt=SEMAPHORE_LOCKED);
	void close  ();
	bool wait   (W32 ms  = WAIT_FOREVER);
	void signal ();
};

class WRunable;

/**
 WThread stellt Thread-Funktionen zur Verf�gung.
*/
class WThread {
private:

	pthread_t       m_id;            // thread-id
	pthread_attr_t  m_attr;          // attributes of thread
	WSemaphore      m_semaphore;     // universal semaphore
	WRunable*       m_runable;       // pointer to runable-object
	WMUTEX          m_mutex;         // mutex for thread-synchronization
	bool            m_fSuspended;    // marks if the thread is supended or not

	// attributes initialization / destroying
	void initialize ();
	static void initializeAttributes (pthread_attr_t& attr);
	static void destroyAttributes    (pthread_attr_t& attr);
	void setDetach (bool on);

	void close ();

public:

	WThread ();
	~WThread ();

	bool  start       (WRunable* runable);
	bool  wait        (W32 ms = WAIT_FOREVER);
	void  signal      ();
	bool  isActive    () const;
	bool  kill        ();
	bool  join        (W32 ms=WAIT_FOREVER);
	void  suspend     ();
	void  resume      ();
	void  setPriority (int priority);

};

// wrapper for a locked mutex
class WMutexLocker {
	WMUTEX* m_mutexPnt;
public:
	inline WMutexLocker (WMUTEX& mutex)
	{
		m_mutexPnt=&mutex;
		WSystem::enterMutex (mutex);
	}
	~WMutexLocker ()
	{
		WSystem::leaveMutex (*m_mutexPnt);
	}
};

/**
 WRunable ist die Basis f�r alle Thread-Klassen.
*/

class WRunable {
	// class WThread can access members of WRunable
	friend class WThread;
	// wlib thread-proc can access members of WRunable
	friend void* wlibThreadProc (void* param);
	/** Flag kennzeichnet, ob die run () Methode gestartet wurde. */
	bool m_fThreadStarted;

protected:
	WRunable ();
	virtual ~WRunable ();
	// thread-object
	WThread m_thread;
	virtual void run () = 0;

};

#endif /* _WTHREAD_H */
