/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "WThread.h"

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

WRunable::WRunable ()
{
	m_fThreadStarted = false;
}

WRunable::~WRunable ()
{

}

void* wlibThreadProc (void* param)
{
	WRunable* runable = (WRunable*) param;
	int oldCancelState, oldCancelType;
	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, &oldCancelState);
	pthread_setcanceltype  (PTHREAD_CANCEL_ASYNCHRONOUS, &oldCancelType);

	try {
		runable->run ();
	}
	catch (WException) { };
	runable->m_fThreadStarted = false;
	// runable->m_thread.signal (); // Signal a wait () function that thread is finished
	// RUN
	pthread_exit ((void*) 0);
	return (void*) 0;
}

void WThread::initializeAttributes (pthread_attr_t& attr)
{
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy (&attr, SCHED_OTHER);
}

void WThread::setDetach (bool on)
{
	pthread_attr_setdetachstate  (&m_attr, on ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);
}

void WThread::setPriority (int priority)
{
	if (pthread_attr_setscope (&m_attr, priority)==-1)
		throw WThreadException ("Cannot set the priority of thread.", WThreadException::ERR_ID_THREAD_SET_PRIORITY);
}

void WThread::destroyAttributes (pthread_attr_t& attr)
{
	pthread_attr_destroy (&attr);
}

bool WThread::wait (W32 ms)
{
	return m_semaphore.wait (ms);
}

void WThread::signal ()
{
	m_semaphore.signal ();
}

void WThread::suspend ()
{
	WSystem::enterMutex (m_mutex);
	if (!m_fSuspended) {
		pthread_kill (m_id, SIGSTOP);
		m_fSuspended = true;
	}
	WSystem::leaveMutex (m_mutex);
}

void WThread::resume  ()
{
	WSystem::enterMutex (m_mutex);
	if (m_fSuspended) {
		pthread_kill(m_id, SIGCONT);
		m_fSuspended = false;
	}
	WSystem::leaveMutex (m_mutex);
}

void WThread::close ()
{
  join ();
	WSystem::deleteMutex (m_mutex);
	destroyAttributes (m_attr);
	m_semaphore.close();
	m_id = 0;
	m_runable = NULL;
	m_fSuspended = false;
}

void WThread::initialize ()
{
	m_id = 0;
	m_runable = NULL;
	m_fSuspended = false;
	initializeAttributes (m_attr);
	WSystem::initMutex (m_mutex);
}

WThread::WThread ()
{
	initialize ();
}

WThread::~WThread ()
{
	try {
		kill ();
	}
	catch (WException) {
	}
	close ();
}

bool WThread::join (W32 ms)
{
	void* ret;
	if (ms>0) {
		struct timespec ts;
		struct timeval  tp;

		gettimeofday(&tp, NULL);

		ts.tv_sec   = tp.tv_sec;
		ts.tv_nsec  = tp.tv_usec * 1000;
		ts.tv_sec  += ms / 1000;
		ts.tv_nsec += ((W64) ms * 1000000) % 1000000000;
		return pthread_timedjoin_np (m_id, &ret, &ts)==0;
	}
	else {
		return pthread_join (m_id, &ret)==0;
	}
	// return wait (ms);
	return false;
}

bool WThread::kill ()
{
	if (isActive ()) {
		if (pthread_cancel (m_id)==-1)
			throw WThreadException ("Cannot kill thread.", WThreadException::ERR_ID_THREAD_KILL);
		m_runable->m_fThreadStarted=false;
		return true;
	}
	return false;
}

bool WThread::start (WRunable* runable)
{
	if (!isActive ()) {
		setDetach (false);
		runable->m_fThreadStarted = false;
		m_semaphore.create ();
		if (pthread_create(&m_id, &m_attr, wlibThreadProc,(void*) runable)!=0)
			throw WThreadException ("Cannot create new thread.", WThreadException::ERR_ID_THREAD_CREATE);
		m_runable = runable;
		m_runable->m_fThreadStarted = true;
		return true;
	}
	return false;
}

bool WThread::isActive () const
{
	return (m_runable && (m_runable->m_fThreadStarted));
}


///////////////////////////////////////////////////
// WSemaphore
///////////////////////////////////////////////////

void WSemaphore::initialize ()
{
	m_fActive = false;
	m_cnt = SEMAPHORE_LOCKED;
}

WSemaphore::WSemaphore ()
{
	initialize ();
}

WSemaphore::WSemaphore (int cnt)
{
	initialize ();
	create (cnt);
}

WSemaphore::~WSemaphore()
{
	close ();
}

void WSemaphore::create (int cnt)
{
	if (!m_fActive) {
		WSystem::initMutex          (m_mutex);
		pthread_condattr_init       (&m_attr);
		//pthread_condattr_setpshared (&m_attr, PTHREAD_PROCESS_SHARED);
		pthread_cond_init           (&m_cond, &m_attr);
		m_cnt = cnt;
		m_fActive = true;
	}
}

void WSemaphore::close ()
{
	if (m_fActive) {
		pthread_cond_destroy (&m_cond);
		pthread_condattr_destroy (&m_attr);
		WSystem::deleteMutex (m_mutex);
		m_fActive = false;
	}
}

bool WSemaphore::wait (W32 ms)
{
	if (m_fActive) {
		if (ms > 0)
			return timedWait(ms);
		WSystem::enterMutex (m_mutex);
		m_cnt--;
		while (m_cnt<=0)
			pthread_cond_wait (&m_cond,&m_mutex);
		WSystem::leaveMutex (m_mutex);
		return true;
	}
	return false;
}

bool WSemaphore::timedWait(W32 ms)
{
	struct timespec ts;
	struct timeval  tp;

	gettimeofday(&tp, NULL);

	ts.tv_sec   = tp.tv_sec;
	ts.tv_nsec  = tp.tv_usec * 1000;
	ts.tv_sec  += ms / 1000;
	ts.tv_nsec += ((W64) ms * 1000000) % 1000000000;

	bool ret = true;
	WSystem::enterMutex (m_mutex);
	m_cnt--;
	while (m_cnt <=0) {
		if (pthread_cond_timedwait(&m_cond,&m_mutex, &ts) == ETIMEDOUT) {
			m_cnt++;
			ret = false;
			break;
		}
	}
	WSystem::leaveMutex (m_mutex);
	return ret;
}

void WSemaphore::signal()
{
	if (m_fActive) {
		WSystem::enterMutex (m_mutex);
		m_cnt++;
		WSystem::leaveMutex (m_mutex);
		pthread_cond_signal (&m_cond);
	}
}
