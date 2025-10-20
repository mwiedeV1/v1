/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "WSocket.h"


///////////////////////////////////////////////////////////////
// WIPAddress
///////////////////////////////////////////////////////////////

WIPAddress::WIPAddress ()
{
    m_port = 0;
    m_ipAddress = 0;
}

WIPAddress::WIPAddress (const WIPAddress& ipAddress) {
    *this = ipAddress;
}

WIPAddress::WIPAddress (WCSTR hostname, int port) {
    if (hostname[0]==0)
        m_ipAddress = htonl (INADDR_ANY);
    else
    if (WSocketUtils::isIpAddress (hostname)) 
        m_ipAddress = WSocketUtils::ipAddr2NBO (hostname);
    else
        m_ipAddress = WSocketUtils::resolveHost (hostname);
    m_port = port;
}

WIPAddress::WIPAddress (W32 ipAddress, int port)
{
	m_ipAddress = ipAddress;
    m_port = port;
}

W32 WIPAddress::getIpAddress () const
{
    return m_ipAddress;
}

WCSTR WIPAddress::getIpAddressStr () const {
	WSocketUtils::ipAddr2String (m_ipAddress, m_ipAddressStr);
	return m_ipAddressStr;
}

const WString& WIPAddress::getHostname () const
{
    if (m_hostname.length ()==0) {
        try {
			WSocketUtils::resolveIpAddress (m_ipAddress, m_hostname, m_port);
        }
        catch (WUnknownHostException) {
            m_hostname = "<unknown>";
        }
    }
    return m_hostname;
}

int WIPAddress::getPort () const {
    return m_port;
};


//////////////////////////////////////////////////////////////
// WIPAddressIPV6
//////////////////////////////////////////////////////////////

WIPAddressIPV6::WIPAddressIPV6 () {
	m_port = 0;
	memset (&m_ipAddress, 0, sizeof (m_ipAddress));
}

WIPAddressIPV6::WIPAddressIPV6 (in6_addr& ipAddress, int port) {
	m_port = port;
	m_ipAddress = ipAddress;
}

WIPAddressIPV6::WIPAddressIPV6 (const WIPAddressIPV6& ipAddress) {
	*this = ipAddress;
}

WIPAddressIPV6::WIPAddressIPV6 (WCSTR hostname, int port) {
	if (hostname[0]==0)
		m_ipAddress = in6addr_any;
	else
		WSocketUtils::resolveHost (hostname, m_ipAddress, (WCSTR) WFormattedString ("%u", port));
	m_port = port;
}

const in6_addr& WIPAddressIPV6::getIpAddress () const {
	return m_ipAddress;
}

WCSTR WIPAddressIPV6::getIpAddressStr () const {
	WSocketUtils::ipAddr2String (m_ipAddress, m_ipAddressStr);
	return m_ipAddressStr;
}

const WString& WIPAddressIPV6::getHostname () const {
    if (m_hostname.length ()==0) {
        try {
			WSocketUtils::resolveIpAddress (m_ipAddress, m_hostname, m_port);
        }
        catch (WUnknownHostException) {
            m_hostname = "<unknown>";
        }
    }
    return m_hostname;
}

int WIPAddressIPV6::getPort () const {
	return m_port;
}


//////////////////////////////////////////////////////////////
// WSocket
//////////////////////////////////////////////////////////////

void WSocket::initialize () {
	m_shutdownMode = 1;
	m_hSocket = INVALID_SOCKET;
	m_fConnected = false;
	m_bytesRead = 0;
	memset (&m_sockAddr, 0, sizeof (m_sockAddr));
}

WSocket::WSocket ()
{
	initialize ();
	init ((W32) INADDR_ANY, 0);
}

WSocket::WSocket (const WSocket& socket) 
{
	init (socket);
}

WSocket::WSocket (const WIPAddress& ipAddr)
{
	initialize ();
	init (ipAddr);
}

WSocket::WSocket (const WIPAddressIPV6& ipAddr)
{
	initialize ();
	init (ipAddr);
}

WSocket::WSocket (SOCKET hSocket) 
{
	initialize ();
	init (hSocket);
}

WSocket::WSocket (SOCKET hSocket, const sockaddr& sockAddr, int len) 
{
	initialize ();
	init (hSocket, sockAddr, len);
}

WSocket::WSocket (WCSTR hostname, int port,  bool fForceIPV4) {
	initialize ();
	init (hostname, port, fForceIPV4);
}


WSocket::~WSocket () {
    try {
        close ();
    }
    catch (WException) {
    }
}

void WSocket::init (const WSocket& socket)
{
	this->m_hSocket = socket.m_hSocket;
	this->m_sockAddr = socket.m_sockAddr;
	this->m_shutdownMode = socket.m_shutdownMode;
	this->m_bytesRead = socket.m_bytesRead;
	this->m_fConnected = socket.m_fConnected;
}

void WSocket::init (const WIPAddress& ipAddress)
{
    init (ipAddress.getIpAddress (), ipAddress.getPort ());
}

void WSocket::init (const WIPAddressIPV6& ipAddress)
{
    init (ipAddress.getIpAddress (), ipAddress.getPort ());
}

void WSocket::init (SOCKET hSocket)
{
    m_hSocket = hSocket;
    socklen_t len = sizeof (sockaddr_in);
    getsockname (m_hSocket, (sockaddr*) &m_sockAddr, &len);
}


void WSocket::init (SOCKET hSocket, const sockaddr& sockAddr, int len)
{
    m_hSocket = hSocket;
    memcpy (&m_sockAddr, &sockAddr, len);
}

void WSocket::init (W32 ipAddress, int port) {
  memset (&m_sockAddr, 0, sizeof (m_sockAddr));
  ((sockaddr_in*) &m_sockAddr)->sin_family = AF_INET;
  ((sockaddr_in*) &m_sockAddr)->sin_addr.s_addr = ipAddress;
  ((sockaddr_in*) &m_sockAddr)->sin_port = htons ((W16)port);
}

void WSocket::init (const in6_addr& ipAddress, int port) {
  memset (&m_sockAddr, 0, sizeof (m_sockAddr));
  ((sockaddr_in6*) &m_sockAddr)->sin6_family = AF_INET6;
  ((sockaddr_in6*) &m_sockAddr)->sin6_addr = ipAddress;
  ((sockaddr_in6*) &m_sockAddr)->sin6_port = htons ((W16)port);
}

void WSocket::init (WCSTR hostname, int port, bool fForceIPV4) {
	W32 ip4 = 0; in6_addr ip6 = in6addr_any;	
	if (hostname[0]==0 || inet_pton (AF_INET, hostname, &ip4)==1) {
		init (ip4, port);
	}
	else
	if (inet_pton (AF_INET6, hostname, &ip6)==1) {
		init (ip6, port);
	}
	else {
		struct addrinfo hints = { 0 };
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		addrinfo* addrs = NULL;
		if (getaddrinfo(hostname, WFormattedString ("%u", port), &hints, &addrs)!=0) {
			// host not found
			throw WUnknownHostException (WFormattedString ("Cannot resolve IP address for %s:%u", (WCSTR) hostname, port), -1);
		}	
		while (addrs) {
     
			switch (addrs->ai_family) {
				case AF_INET:			
					memcpy (&m_sockAddr, (sockaddr_in6 *) addrs->ai_addr, addrs->ai_addrlen);
					if (fForceIPV4) {
						freeaddrinfo(addrs);
						return;
					}
				case AF_INET6:
					if (fForceIPV4 && addrs->ai_next) {
						addrs = addrs->ai_next;
						continue;
					}                         
					memcpy (&m_sockAddr, (sockaddr_in6 *) addrs->ai_addr, addrs->ai_addrlen);
					break;
			}
			break;
		}
		freeaddrinfo(addrs);
	}
}

void WSocket::close ()
{

	if (m_hSocket!=0) {
		WSocketUtils::close (m_hSocket, m_shutdownMode);
		m_hSocket =-1;
	}
	m_fConnected= false;
}

bool WSocket::isConnected ()  const
{
	return m_fConnected;
}

void WSocket::create (bool fUDP)
{
	if ((m_hSocket = socket (((sockaddr*) &m_sockAddr)->sa_family, fUDP ? SOCK_DGRAM : SOCK_STREAM, 0))==SOCKET_ERROR)
        throw WSocketException ("Cannot create socket.", WSocketException::ERR_ID_CREATE);
}

void WSocket::throwException (WCSTR msg, const WSocket& socket)
{
    WFormattedString text (512), ipStr;
	if (socket.isIPV6())
		WSocketUtils::ipAddr2String (socket.getIpAddressIPV6 (), ipStr);
	else
		WSocketUtils::ipAddr2String (socket.getIpAddress(), ipStr);
		
	text.format (msg, (WCSTR) ipStr, socket.getPort ());
    throw WSocketException (text, WSocketException::ERR_ID_CONNECT);
}

void WSocket::connect  ()
{
	if (isConnected ())
		close ();
    if (::connect (m_hSocket, (sockaddr*) &m_sockAddr, sizeof (m_sockAddr))==SOCKET_ERROR)
        throwException ("Cannot connect to %s:%u", *this);
	m_fConnected = true;
	m_bytesRead = 0; 
}

int WSocket::write (const void* pnt, W32 size)
{
	return WSocketUtils::write (m_hSocket, pnt, size);
}

int WSocket::writeln  (const WString& str)
{
	W32 bytesWritten = 0;
	bytesWritten+=write ((void*) (WCSTR) str, str.length ());
	bytesWritten+=write ((void*) "\n", 1);
	return bytesWritten;
}

int WSocket::writeln (WCSTR str)
{
	W32 bytesWritten = 0;
	bytesWritten+=write ((void*) str, strlen (str));
	bytesWritten+=write ((void*) "\n", 1);
	return bytesWritten;
}

int WSocket::write (const WString& str)
{
	return write ((void*) (WCSTR) str, str.length ());
}

int WSocket::write (WCSTR str)
{
	return write ((void*) str, strlen (str));
}

int WSocket::read (void* pnt, W32 size, bool fPeek)
{
	return WSocketUtils::read (m_hSocket, pnt, size, m_bytesRead, fPeek);
}

bool WSocket::readln (WString& str, bool fZeroTmp)
{
	return WSocketUtils::readln (this, str, fZeroTmp);
}

bool WSocket::setReuseAddress (bool on)
{
	int i = on;
	return setsockopt (m_hSocket, SOL_SOCKET, SO_REUSEADDR, (WCSTR) &i, sizeof (i)) != -1;
}

bool WSocket::setRecvTimeout (W32 ms)
{
  struct timeval tv;
  tv.tv_sec = ms / 1000;
  tv.tv_usec = (ms % 1000) * 1000;
  return setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != -1;
	// return setsockopt (m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (WCSTR) &ms, sizeof (int)) != -1;
}

bool WSocket::setNonBlocking (bool on)
{
	// make noon blocking (we dont need it in time)
	int opts = fcntl (m_hSocket, F_GETFL);
	if (on)
		// set nonblock bit
		opts |=  O_NONBLOCK;
	else
		// clear nonblock bit
		opts &= ~O_NONBLOCK;
	return fcntl (m_hSocket, F_SETFL, opts) != -1;
}

bool WSocket::setRecvBufSize (W32 size)
{
	return setsockopt (m_hSocket, SOL_SOCKET, SO_RCVBUF, (WCSTR) &size, sizeof (size)) != -1;
}

bool WSocket::setSendBufSize (W32 size)
{
	return setsockopt (m_hSocket, SOL_SOCKET, SO_SNDBUF, (WCSTR) &size, sizeof (size)) != -1;
}

void WSocket::setShutdownMode (int mode)
{
	m_shutdownMode = mode;
}

void WSocket::getPeerName (WString& ipAddrStr, WString& portStr) const {
  sockaddr_in6 addr; socklen_t len = sizeof (addr);
	if (0==getpeername (m_hSocket, (sockaddr*) &addr, &len)) {
		int port = 0;
		if (((sockaddr*) &addr)->sa_family==AF_INET6) {
			port = ntohs (((sockaddr_in6*) &m_sockAddr)->sin6_port);
			WSocketUtils::ipAddr2String (((sockaddr_in6*) &m_sockAddr)->sin6_addr, ipAddrStr);
		}
		else {
			WSocketUtils::ipAddr2String (((sockaddr_in*) &m_sockAddr)->sin_addr.s_addr, ipAddrStr);		
			port = ntohs (((sockaddr_in*) &m_sockAddr)->sin_port);
		}
		portStr = WFormattedString ("%u", port);
	}
}

bool WSocket::isSamePeerIP (const WSocket& socket) const {
	bool fSame = false;
	sockaddr_in6 addr1; socklen_t len1 = sizeof(addr1);
	sockaddr_in6 addr2; socklen_t len2 = sizeof(addr2);
	if (0 == getpeername(m_hSocket, (sockaddr*)&addr1, &len1) && 0 == getpeername(socket, (sockaddr*)&addr2, &len2)) {
		if (((sockaddr*)&addr1)->sa_family == ((sockaddr*)&addr2)->sa_family) {
			if (((sockaddr*)&addr1)->sa_family == AF_INET6)
				fSame = memcmp(&((sockaddr_in6*)&addr1)->sin6_addr, &((sockaddr_in6*)&addr2)->sin6_addr, sizeof (in6_addr)) == 0;
			else
				fSame = ((sockaddr_in*)&addr1)->sin_addr.s_addr == ((sockaddr_in*)&addr2)->sin_addr.s_addr;
		}
	}
	return fSame;

}

W32 WSocket::getBytesRead () const
{
	return m_bytesRead;
}

/////////////////////////////////////////////////////////////
// WServerSocket
/////////////////////////////////////////////////////////////

WServerSocket::WServerSocket () : WSocket ()
{
  m_backlog = 1;
}

WServerSocket::WServerSocket (WCSTR hostname, int port, bool fForceIPV4) : WSocket (hostname, port, fForceIPV4) 
{
	m_backlog = 1;
}

void WServerSocket::bind ()
{
	if (::bind (m_hSocket, (sockaddr*) &m_sockAddr, sizeof (m_sockAddr))==SOCKET_ERROR)
		throwException ("Cannot bind to %s:%u", *this);

	socklen_t len = sizeof (m_sockAddr);
	getsockname (m_hSocket, (sockaddr*) &m_sockAddr, &len);

	if (listen (m_hSocket, m_backlog)==SOCKET_ERROR)
		throw WSocketException ("Cannot listen socket.", WSocketException::ERR_ID_LISTEN);
}

bool WServerSocket::accept (WSocket& socket)
{
  sockaddr_in6 in; 
	socklen_t len = sizeof (in);

	int hClientSocket;
	if ((hClientSocket = ::accept (m_hSocket, (sockaddr*) &in, &len))==-1) {
		if (errno==EWOULDBLOCK)
			return false;

		throw WSocketException ("Cannot accept socket.", WSocketException::ERR_ID_ACCEPT);
	}
	socket.init (hClientSocket, *(sockaddr*) &in, len);
	return true;
}

//////////////////////////////////////////////////////
// WSocketUtils
//////////////////////////////////////////////////////

void WSocketUtils::startup ()
{

}

void WSocketUtils::shutdown  ()
{


}

void WSocketUtils::ping (const WSocket& socket)
{
	WSocket s;
	s.init (socket.getIpAddress (),
	        socket.getPort ());
	s.create  ();
	s.connect ();
	s.close   ();
}

W32 WSocketUtils::ipAddr2NBO (WCSTR ipAddr) {
    return inet_addr (ipAddr);
}

bool WSocketUtils::ipAddr2INADDR (WCSTR ipAddr, in6_addr& in6) {
	return inet_pton (AF_INET6, ipAddr, &in6)==1;
}

bool WSocketUtils::ipAddr2INADDR (WCSTR ipAddr, in_addr& in) {
	return inet_pton (AF_INET, ipAddr, &in)==1;
}

void WSocketUtils::ipAddr2String (W32 ipAddr, WString& str) {
	str.resize(15);
	if (inet_ntop(AF_INET, (void*) &ipAddr, (char*) str.getBuf(), 15))
		str.truncate (strlen((WCSTR) str));
	else
		str.truncate(0);
}

void WSocketUtils::ipAddr2String (const in6_addr& ipAddr,  WString& str) {
	str.resize(39);
	if (inet_ntop(AF_INET6, (void*) &ipAddr, (char*) str.getBuf(), 39))
		str.truncate(strlen ((WCSTR) str));
	else
		str.truncate(0);
}

bool WSocketUtils::isIpAddress (WCSTR ipAddr)
{
    return inet_addr (ipAddr) != INADDR_NONE;
}

W32 WSocketUtils::resolveHost (WCSTR hostname)
{
    hostent* h = gethostbyname (hostname);
    if (h==NULL) {
        // host not found
        WFormattedString text (512);
        text.format ("Cannot resolve IP adress for host %s", (WCSTR) hostname);
        throw WUnknownHostException (text, -1);
    }
    return *(W32*) h->h_addr_list[0];
}

void WSocketUtils::resolveHost (WCSTR hostname, in6_addr& ipAddr, WCSTR scheme) {
	if (inet_pton (AF_INET6, hostname, &ipAddr)<1) {
		struct addrinfo hints = { 0 };
		hints.ai_flags = AI_V4MAPPED | AI_ALL;    
		hints.ai_family = AF_INET6;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		addrinfo* addrs = NULL;
    int s;
		if (s=getaddrinfo(hostname, scheme, &hints, &addrs)!=0) {
			// host not found
			throw WUnknownHostException (WFormattedString ("Cannot resolve IPV6 address for %s", (WCSTR) hostname), -1);
		}
 		memcpy(&ipAddr, (void*) &((sockaddr_in6*) addrs->ai_addr)->sin6_addr, sizeof (ipAddr));
		freeaddrinfo(addrs);
    
    /*
    bool fFound = false;
    for (struct addrinfo *ai = addrs; ai != NULL; ai = ai->ai_next) {
      if (ai->ai_family==AF_INET6) {
        fFound = true;
    		memcpy(&ipAddr, ai->ai_addr + 8, sizeof (ipAddr));
      }
    }
		freeaddrinfo(addrs);
    if (!fFound) {
  		throw WUnknownHostException (WFormattedString ("Cannot resolve IPV6 address for %s", (WCSTR) hostname), -1);
    }
    */
	}
}


void WSocketUtils::resolveIpAddress (const in6_addr& ipAddr, WString& hostname, int port) {
	struct sockaddr_in6  saGNI;
	saGNI.sin6_family = AF_INET6;
	saGNI.sin6_addr = ipAddr;
	saGNI.sin6_port = htons(port);
	char servInfo[NI_MAXSERV];
	hostname.resize(NI_MAXHOST);
	if (0 != getnameinfo ((const sockaddr*) &saGNI, sizeof(struct sockaddr_in6), (char*) hostname.getBuf(), NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV)) {
		WFormattedString text (512), ipStr;
		WSocketUtils::ipAddr2String (ipAddr, ipStr);
		text.format ("Cannot resolve IP address for %s", (WCSTR) ipStr);
		throw WUnknownHostException (text, -1);
	}
	hostname.truncate(strlen (hostname));
}

void WSocketUtils::resolveIpAddress (W32 ipAddr, WString& hostname, int port) {
	struct sockaddr_in  saGNI;
	saGNI.sin_family = AF_INET;
	saGNI.sin_addr.s_addr = ipAddr;
	saGNI.sin_port = htons(port);
	char servInfo[NI_MAXSERV];
	hostname.resize(NI_MAXHOST);
	if (0 != getnameinfo ((const sockaddr*) &saGNI, sizeof(struct sockaddr_in), (char*) hostname.getBuf(), NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV)) {
        WFormattedString text (512), ipStr;
		WSocketUtils::ipAddr2String (ipAddr, ipStr);
        text.format ("Cannot resolve IP address for %s", (WCSTR) ipStr);
        throw WUnknownHostException (text, -1);
	}
	hostname.truncate(strlen (hostname));
}


W32 WSocketUtils::getLocalIpAddress ()
{
    WString str;
    getLocalHost (str);
    return resolveHost (str);
}

void WSocketUtils::getLocalIpAddress (WString& ipAddr)
{
	ipAddr2String (getLocalIpAddress (), ipAddr);
}

void WSocketUtils::getLocalHost (WString& hostname)
{
    if (hostname.getBufLength ()<128)
        hostname.resize (128);
    gethostname ((char*) hostname.getBuf (), hostname.getBufLength ()-1);
    hostname.truncate (strlen ((char*) hostname.getBuf ()));
}


// verry slow but good enought for FTP/SMTP/HTTP protocol
bool WSocketUtils::readln (WSocketIO* socket, WString& str, bool fZeroTmp)
{
	str.truncate (0);

	char buf[256], *pnt = buf;
	int  idx = 0;
	buf[255] = 0;
	while (true) {
		do {
			try {
				socket->read (pnt, 1);
			}
			catch (WSocketException& e) {
        socket->close ();
        if (e.getId ()==WSocketException::ERR_ID_CLOSED) {
        	*pnt=0; str += buf;
        	return idx>0;
        }
        if (str.length ())
        	throw e;
        return false;
			}
		}
		while (*pnt==0x0D);
		if (*pnt==0x0A) // new line
			break;
		pnt++;
		idx++;
		if (idx==255) {
			str+= buf;
			pnt = buf;
			idx = 0;
		}
	};
	// append rest of line
	*pnt=0;
	str += buf;
	if (fZeroTmp)
		memset (buf, 0, sizeof (buf));
	return true;
}


int WSocketUtils::read (int hSocket, void* pnt, W32 size, W32& bytesReadSum, bool fPeek)
{
	int bytesRead;
	if ((bytesRead = recv (hSocket, (char*) pnt, size, (fPeek ? MSG_PEEK : 0)))<=0) {
    /*
		if (errno==EWOULDBLOCK)
			bytesRead=0;
		*/
    if (errno==EAGAIN)
			throw WSocketException ("Read timeout", WSocketException::ERR_ID_TIMEOUT);
		else if (bytesRead==0) //  || bytesRead==-1)
			throw WSocketException ("Connection was closed.", WSocketException::ERR_ID_CLOSED);
		else
			throw WSocketException ("Cannot read from socket.", WSocketException::ERR_ID_READ);
	}
	bytesReadSum+=bytesRead;
	return bytesRead;
}

int WSocketUtils::write (int hSocket, const void* pnt, W32 size)
{
	int bytesWritten = 0;
	if ((bytesWritten = ::send (hSocket, (WCSTR) pnt, size, MSG_NOSIGNAL))==-1) {
		throw WSocketException ("Cannot write into socket.", WSocketException::ERR_ID_WRITE);
	}
	return bytesWritten;
}

void WSocketUtils::close (int hSocket, int mode)
{
	::shutdown (hSocket, mode);
	::close  (hSocket);
}



