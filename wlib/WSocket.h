/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef _WSOCKET_H
#define _WSOCKET_H


#include "WSystem.h"

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>

// WSocketException
class _WLIBAPI WSocketException : public WException {
public:

	enum {
		ERR_ID_STARTUP      = -1,
		ERR_ID_SHUTDOWN     = -2,
		ERR_ID_CREATE       = -3,
		ERR_ID_CONNECT      = -2,
		ERR_ID_WRITE        = -3,
		ERR_ID_READ         = -4,
		ERR_ID_TIMEOUT      = -5,
		ERR_ID_CLOSED       = -6,
		ERR_ID_BIND         = -7,
		ERR_ID_LISTEN       = -8,
		ERR_ID_ACCEPT       = -9,
	};

	inline WSocketException (WCSTR msg, int id) : WException ()
	{
		set ("WSocketException", msg, id, errno);
	};
};

// WUnknownHostException
class _WLIBAPI WUnknownHostException : public WException {
public:
	inline WUnknownHostException (WCSTR msg, int id) : WException ()
	{
		set ("WUnknownHostException", msg, id, errno);
	};
};


/*
 WSocketIO

 Abstract class for input/output operations.
*/
class _WLIBAPI WSocketIO {
public:
	// input / output
	virtual void connect  () = 0;
	virtual W32  write    (const void* pnt, W32 size) = 0;
	virtual W32  writeln  (const WString& str) = 0;
	virtual W32  writeln  (WCSTR str) = 0;
	virtual W32  write    (const WString& str) = 0;
	virtual W32  write    (WCSTR str) = 0;
	virtual W32  read     (void* pnt, W32 size, bool fPeek=false) = 0;
	virtual bool readln   (WString& str) = 0;
	virtual void close    () = 0;
	virtual bool isConnected  () const = 0;
};

/*
  WSocketUtils

  Some utils for sockets.
*/

class WSocket;
class _WLIBAPI WSocketUtils {
public:
	static void  startup           ();
	static void  shutdown          ();
	static bool  isIpAddress       (WCSTR ipAddr);
	static W32   ipAddr2NBO        (WCSTR ipAddr);
	static WCSTR ipAddr2String     (W32 ipAddr);
	static W32   resolveHost       (WCSTR hostname);
	static WCSTR resolveIpAddress  (W32 ipAddr);
	static W32   getLocalIpAddress ();
	static void  getLocalIpAddress (WString& ipAddr);
	static void  getLocalHost      (WString& hostname);
	static void  ping              (const WSocket& socket);
	static bool  readln            (WSocketIO* socket, WString& str);
	static W32   read              (int hSocket, void* pnt, W32 size, W32& eofBytes, bool fPeek=false);
	static W32   write             (int hSocket, const void* pnt, W32 size);
	static void  close             (int hSocket, int mode=1);
};

/*
 WIPAddress

 Represents an IP address and port.
*/

class _WLIBAPI WIPAddress {
	W32 m_port;
	W32 m_ipAddress;
	mutable WString m_hostname;
public:
	WIPAddress ();
	WIPAddress (const WIPAddress& ipAddress);
	WIPAddress (WCSTR hostname, W32 port);
	WIPAddress (W32 ipAddress, W32 port);
	W32 getIpAddress () const;
	WCSTR getIpAddressStr () const;
	const WString& getHostname () const;
	W32 getPort () const;
};


/*
 WSocket

 Represents a socket.

*/

class _WLIBAPI WSocket : public WSocketIO {

protected:


	int         m_hSocket;       // socket-handle
	sockaddr_in m_sockAddr;      // ip-address structure
	W32         m_bytesRead;     // sum of all bytes read from socket
	int         m_shutdownMode;  // shutdown mode for closing
	bool        m_fConnected;    // true if socket is connected

	// helpers
	static void throwBindException    (W32 ipAddress, W32 port);
	static void throwConnectException (W32 ipAddress, W32 port);

	void initialize ();

public:

	WSocket                     ();
	WSocket                     (const WSocket& socket);
	WSocket                     (const WIPAddress& ipAddr);
	WSocket                     (int hSocket);
	WSocket                     (int hSocket, const sockaddr_in& sockAddr);
	virtual ~WSocket            ();

	// init / create / close
	void init                   (const WSocket& socket);
	void init                   (const WIPAddress& ipAddr);
	void init                   (W32 ipAddr, W32 port);
	void init                   (int hSocket);
	void init                   (int hSocket, const sockaddr_in& sockAddr);
	void create                 (bool fUDP = false);

	// socket options
	bool setReuseAddress        (bool on);
	bool setNonBlocking         (bool on);
	bool setRecvTimeout         (W32 ms);
	bool setRecvBufSize         (W32 size);
	bool setSendBufSize         (W32 size);
	void setShutdownMode        (int mode);

	// input / output
	virtual void connect        ();
	virtual W32  write          (const void* pnt, W32 size);
	virtual W32  writeln        (const WString& str);
	virtual W32  writeln        (WCSTR str);
	virtual W32  write          (const WString& str);
	virtual W32  write          (WCSTR str);
	virtual W32  read           (void* pnt, W32 size, bool fPeek=false);
	virtual bool readln         (WString& str);
	virtual void close          ();
	virtual bool isConnected    () const;

	// getter
	inline W32    getIpAddress  () const
	{
		return m_sockAddr.sin_addr.s_addr;
	}
	inline W32    getPort       () const
	{
		return ntohs (m_sockAddr.sin_port);
	}
	inline int    getHandle     () const
	{
		return m_hSocket;
	}
	void          getLocalName  (WIPAddress& ipAddress) const;
	void          getPeerName   (WIPAddress& ipAddress) const;
	W32           getBytesRead  () const;

	/** Cast-Operator. */
	inline operator int () const
	{
		return m_hSocket;
	};
};


/*
 WDatagramSocket

 Represents a UDP socket.

*/
/*
class _WLIBAPI WDatagramSocket : public WSocket
{
public:
    WDatagramSocket ();
    WDatagramSocket (W32 port);
    WDatagramSocket (const WIPAddress& ipAddr);
    void create     (bool  fBind=true);
    void connect    (const WIPAddress& ipAddr);
    bool joinGroup  (WCSTR group);
// IP_MULTICAST_TTL = 3
    inline bool setTTL (int ttl) {
        return setsockopt (m_hSocket, IPPROTO_IP, 3, (WCSTR) &ttl, sizeof (int)) != -1;
    }
};
*/

/*
 WServerSocket

 Represents a socket bound to a specific ip address.
*/
class _WLIBAPI WServerSocket : public WSocket {
	int m_backlog;
public:
	WServerSocket   ();
	WServerSocket   (W32 port);
	WServerSocket   (const WIPAddress& ipAddr, int backlog=1);
	void create     ();
	bool accept     (WSocket& socket);

	inline void setBacklog (int backlog)
	{
		m_backlog = backlog;
	}
};

#endif /* _WSOCKET_H */
