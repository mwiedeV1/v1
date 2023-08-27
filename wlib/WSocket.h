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

typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1


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
class _WLIBAPI WSocketIO
{
public:
  // input / output
  virtual int  write    (const void* pnt, W32 size) = 0;
  virtual int  writeln  (const WString& str) = 0;
  virtual int  writeln  (WCSTR str) = 0;
  virtual int  write    (const WString& str) = 0;
  virtual int  write    (WCSTR str) = 0;
  virtual int  read     (void* pnt, W32 size, bool fPeek=false) = 0;
  virtual bool readln   (WString& str, bool fZeroTmp=false) = 0;
  virtual void close    () = 0;
  virtual bool isConnected  () const = 0;
};

/*
  WSocketUtils
  
  Some utils for sockets.
*/

class WSocket;
class _WLIBAPI WSocketUtils
{
public:
  static void  startup           ();
  static void  shutdown          ();
  static bool  isIpAddress       (WCSTR ipAddr);
  static W32   ipAddr2NBO        (WCSTR ipAddr);
  static bool  ipAddr2INADDR     (WCSTR ipAddr, in6_addr& in6);
  static bool  ipAddr2INADDR     (WCSTR ipAddr, in_addr& in);	 
  static void  ipAddr2String     (W32 ipAddr, WString& str);
  static void  ipAddr2String     (const in6_addr& ipAddr, WString& str);
  static W32   resolveHost       (WCSTR hostname);
  static void  resolveHost       (WCSTR hostname, in6_addr& ipAddr, WCSTR scheme="");
  static void  resolveIpAddress  (W32 ipAddr, WString& hostname, int port=0);
  static void  resolveIpAddress  (const in6_addr& ipAddr, WString& hostname, int port=0);
  static W32   getLocalIpAddress ();
  static void  getLocalIpAddress (WString& ipAddr);
  static void  getLocalHost      (WString& hostname);
  static void  ping              (const WSocket& socket);
  static bool  readln            (WSocketIO* socket, WString& str, bool fZeroTmp=false);
  static int   read              (SOCKET hSocket, void* pnt, W32 size, W32& eofBytes, bool fPeek=false);
  static int   write             (SOCKET hSocket, const void* pnt, W32 size);
  static void  close             (SOCKET hSocket, int mode=1);
};

/*
 WIPAddress
 
 Represents an IP V4 address and port.
*/

class _WLIBAPI WIPAddress
{
  int m_port;
  W32 m_ipAddress;
  mutable WString m_hostname, m_ipAddressStr;
public:
  WIPAddress ();
  WIPAddress (const WIPAddress& ipAddress);
  WIPAddress (WCSTR hostname, int port);
  WIPAddress (W32 ipAddress, int port);
  W32 getIpAddress () const;
  WCSTR getIpAddressStr () const;
  const WString& getHostname () const;
  int getPort () const;
};

/*
  WIPAddressIPV6

  Represents an IP V6 address and port.
*/

class _WLIBAPI WIPAddressIPV6
{
  int m_port;
  in6_addr m_ipAddress;
  mutable WString m_hostname, m_ipAddressStr;
  addrinfo* m_addrs;
public:
  WIPAddressIPV6 ();
  WIPAddressIPV6 (const WIPAddressIPV6& ipAddress);
  WIPAddressIPV6 (WCSTR hostname, int port);
  WIPAddressIPV6 (in6_addr& ipAddress, int port=0);
  const in6_addr& getIpAddress () const;
  WCSTR getIpAddressStr () const;
  const WString& getHostname () const;
  int getPort () const;
};


/*
 WSocket

 Represents a socket.

*/

class _WLIBAPI WSocket : public WSocketIO
{

protected:


  SOCKET        m_hSocket;       // socket-handle
  sockaddr_in6  m_sockAddr;      // socket structure (ipv4, ipv6)
  W32           m_bytesRead;     // sum of all bytes read from socket
  int           m_shutdownMode;  // shutdown mode for closing
  bool          m_fConnected;    // true if socket is connected

	void initialize ();

  // helpers
  static void throwException (WCSTR msg, const WSocket& socket);

public:

  WSocket                     ();
  WSocket                     (const WSocket& socket);
  WSocket                     (const WIPAddress& ipAddr);
  WSocket                     (const WIPAddressIPV6& ipAddr);
  WSocket                     (SOCKET hSocket);
  WSocket                     (SOCKET hSocket, const sockaddr& sockAddr, int len);
  WSocket                     (WCSTR hostname, int port, bool fForceIPV4=false);
  virtual ~WSocket            ();

  // init / create / close
  void init                   (const WSocket& socket);
  void init                   (const WIPAddress& ipAddr);
  void init                   (const WIPAddressIPV6& ipAddr);
  void init                   (W32 ipAddress, int port);
  void init                   (const in6_addr& ipAddress, int port);
  void init                   (SOCKET hSocket);
  void init                   (SOCKET hSocket, const sockaddr& sockAddr, int len);
  void init                   (WCSTR hostname, int port, bool fForceIPV4=false);
  void create                 (bool fUDP = false);
  void connect				        ();

  // socket options
  bool setReuseAddress        (bool on);
  bool setNonBlocking         (bool on);
  bool setRecvTimeout         (W32 ms);
  bool setRecvBufSize         (W32 size);
  bool setSendBufSize         (W32 size);
  void setShutdownMode        (int mode);

  // input / output
  virtual int  write          (const void* pnt, W32 size);
  virtual int  writeln        (const WString& str);
  virtual int  writeln        (WCSTR str);
  virtual int  write          (const WString& str);
  virtual int  write          (WCSTR str);
  virtual int  read           (void* pnt, W32 size, bool fPeek=false);
  virtual bool readln         (WString& str, bool fZeroTmp=false);
  virtual void close          ();
  virtual bool isConnected    () const;
    
   // getter
  inline W32              getIpAddress     () const  { return ((sockaddr_in*) &m_sockAddr)->sin_addr.s_addr; }
  inline int              getPort          () const  { return ntohs (((sockaddr*) &m_sockAddr)->sa_family==AF_INET6 ? ((sockaddr_in6*) &m_sockAddr)->sin6_port : ((sockaddr_in*) &m_sockAddr)->sin_port); }
  inline const in6_addr&  getIpAddressIPV6 () const  { return ((sockaddr_in6*) &m_sockAddr)->sin6_addr; }
  inline SOCKET           getHandle        () const  { return m_hSocket; }
  inline bool             isIPV6           () const  { return ((sockaddr*) &m_sockAddr)->sa_family==AF_INET6; }			 
  bool                    isSamePeerIP     (const WSocket& socket) const;
  void                    getPeerName      (WString& ipAddrStr, WString& portStr) const;
  W32                     getBytesRead     () const;

  // cast operators
  inline operator SOCKET () const {
      return m_hSocket;
  };
};


/*
 WServerSocket
 
 Represents a socket bound and listen to a specific ip address.
*/
class _WLIBAPI WServerSocket : public WSocket
{
  int m_backlog;  
public:
  WServerSocket   ();
  WServerSocket	  (WCSTR hostname, int port, bool fForceIPV4=false);

  void bind       ();
  bool accept     (WSocket& socket);

  inline void setBacklog (int backlog) {
      m_backlog = backlog;
  }
};


#endif /* _WSOCKET_H */
