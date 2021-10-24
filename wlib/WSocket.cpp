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
	m_port      = 0;
	m_ipAddress = 0;
}

WIPAddress::WIPAddress (const WIPAddress& ipAddress)
{
	*this = ipAddress;
}

WIPAddress::WIPAddress (WCSTR hostname, W32 port)
{
	// check if it is a ip-address
	if (hostname[0]==0)
		m_ipAddress = htonl (INADDR_ANY);
	else if (WSocketUtils::isIpAddress (hostname)) {
		m_ipAddress = WSocketUtils::ipAddr2NBO (hostname);
	}
	else
		// resolve hostname
		m_ipAddress = WSocketUtils::resolveHost (hostname);
	m_port = port;
	m_hostname.clear ();
}

WIPAddress::WIPAddress (W32 ipAddress, W32 port)
{
	m_ipAddress = ipAddress;
	m_port = port;
	m_hostname.clear ();
}

W32 WIPAddress::getIpAddress () const
{
	return m_ipAddress;
}

WCSTR WIPAddress::getIpAddressStr () const
{
	return WSocketUtils::ipAddr2String (m_ipAddress);
}

const WString& WIPAddress::getHostname () const
{
	if (m_hostname.length ()==0) {
		try {
			m_hostname = WSocketUtils::resolveIpAddress (m_ipAddress);
		}
		catch (WUnknownHostException) {
			m_hostname = "<unknown>";
		}
	}
	return m_hostname;
}

W32 WIPAddress::getPort () const
{
	return m_port;
};




//////////////////////////////////////////////////////////////
// WSocket
//////////////////////////////////////////////////////////////

void WSocket::initialize ()
{
	m_shutdownMode = 1;
	m_hSocket = -1;
	m_fConnected = false;
	m_bytesRead = 0;
	memset (&m_sockAddr, 0, sizeof (m_sockAddr));
	m_sockAddr.sin_family = AF_INET;
	init (INADDR_ANY, 0);
}

WSocket::WSocket ()
{
	initialize ();
}

WSocket::WSocket (const WSocket& socket)
{
	initialize ();
	init (socket);

}

WSocket::WSocket (const WIPAddress& ipAddr)
{
	initialize ();
	init (ipAddr);

}

WSocket::WSocket (int hSocket)
{
	initialize ();
	init (hSocket);
}

WSocket::WSocket (int hSocket, const sockaddr_in& sockAddr)
{
	initialize ();
	init (hSocket, sockAddr);

}

WSocket::~WSocket ()
{
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
}

void WSocket::init (const WIPAddress& ipAddress)
{
	init (ipAddress.getIpAddress (),
	      ipAddress.getPort ());
}

void WSocket::init (int hSocket)
{
	m_hSocket = hSocket;

	socklen_t length = sizeof (m_sockAddr);
	getsockname (m_hSocket, (struct sockaddr *)&m_sockAddr, &length);
}

void WSocket::init (int hSocket, const sockaddr_in& sockAddr)
{
	m_hSocket = hSocket;
	memcpy (&m_sockAddr, &sockAddr, sizeof (sockaddr_in));
}

void WSocket::init (W32 ipAddress, W32 port)
{
	m_sockAddr.sin_addr.s_addr = ipAddress;
	m_sockAddr.sin_port = htons((W16)port);
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
	int type = SOCK_STREAM;
	if (fUDP) type = SOCK_DGRAM;

	// create new socket
	if ((m_hSocket = socket (AF_INET, type, 0))==-1)
		throw WSocketException ("Cannot create socket.", WSocketException::ERR_ID_CREATE);
}

void WSocket::throwConnectException (W32 ipAddress, W32 port)
{
	WFormattedString text (512);
	text.format ("Cannot connect to %s:%u",
	             (WCSTR) WSocketUtils::ipAddr2String (ipAddress), port);
	throw WSocketException (text, WSocketException::ERR_ID_CONNECT);
}

void WSocket::throwBindException (W32 ipAddress, W32 port)
{
	WFormattedString text (512);
	text.format ("Cannot bind to %s:%u",
	             (WCSTR) WSocketUtils::ipAddr2String (ipAddress), port);
	throw WSocketException (text, WSocketException::ERR_ID_BIND);
}


void WSocket::connect  ()
{
	if (isConnected ())
		close ();
	if (::connect (m_hSocket, (sockaddr*) &m_sockAddr, sizeof (sockaddr_in))==-1)
		throwConnectException (getIpAddress (), getPort ());
	m_fConnected = true;
	m_bytesRead = 0;
}

W32 WSocket::write (const void* pnt, W32 size)
{
	return WSocketUtils::write (m_hSocket, pnt, size);
}

W32 WSocket::writeln  (const WString& str)
{
	W32 bytesWritten = 0;
	bytesWritten+=write ((void*) (WCSTR) str, str.length ());
	bytesWritten+=write ((void*) "\n", 1);
	return bytesWritten;
}

W32 WSocket::writeln (WCSTR str)
{
	W32 bytesWritten = 0;
	bytesWritten+=write ((void*) str, strlen (str));
	bytesWritten+=write ((void*) "\n", 1);
	return bytesWritten;
}

W32 WSocket::write (const WString& str)
{
	return write ((void*) (WCSTR) str, str.length ());
}

W32 WSocket::write (WCSTR str)
{
	return write ((void*) str, strlen (str));
}

W32 WSocket::read (void* pnt, W32 size, bool fPeek)
{
	return WSocketUtils::read (m_hSocket, pnt, size, m_bytesRead, fPeek);
}

bool WSocket::readln (WString& str)
{
	return WSocketUtils::readln (this, str);
}

bool WSocket::setReuseAddress (bool on)
{
	int i = on;
	return setsockopt (m_hSocket, SOL_SOCKET, SO_REUSEADDR, (WCSTR) &i, sizeof (i)) != -1;
}

bool WSocket::setRecvTimeout (W32 ms)
{
	return setsockopt (m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (WCSTR) &ms, sizeof (int)) != -1;
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

void WSocket::getLocalName (WIPAddress& ipAddress) const
{
	sockaddr_in addr;
	socklen_t length = sizeof (addr);
	getsockname (m_hSocket, (struct sockaddr *)&addr, &length);
	ipAddress = WIPAddress (addr.sin_addr.s_addr,
	                        ntohs (addr.sin_port));
}

void WSocket::getPeerName (WIPAddress& ipAddress) const
{
	sockaddr_in addr;
	socklen_t length = sizeof (addr);
	getpeername (m_hSocket, (struct sockaddr *)&addr, &length);
	ipAddress = WIPAddress (addr.sin_addr.s_addr,
	                        ntohs (addr.sin_port));
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

WServerSocket::WServerSocket (W32 port) : WSocket ()
{
	init (INADDR_ANY, port);
	m_backlog = 1;
}

WServerSocket::WServerSocket (const WIPAddress& ipAddr, int backlog) : WSocket ()
{
	init (ipAddr);
	m_backlog = backlog;
}

void WServerSocket::create ()
{
	WSocket::create ();
	setReuseAddress (true);
	// bind socket
	if (bind (m_hSocket, (sockaddr*) &m_sockAddr, sizeof(sockaddr_in))==-1)
		throwBindException (getIpAddress (), getPort ());
	socklen_t len = sizeof (sockaddr);
	getsockname (m_hSocket, (sockaddr*) &m_sockAddr, &len);
	// listen
	if (listen (m_hSocket, m_backlog)==-1)
		throw WSocketException ("Cannot listen socket.", WSocketException::ERR_ID_LISTEN);
}

bool WServerSocket::accept (WSocket& socket)
{
	sockaddr_in in;
	socklen_t len = sizeof (in);

	int hClientSocket;
	if ((hClientSocket = ::accept (m_hSocket, (sockaddr*) &in, &len))==-1) {
		if (errno==EWOULDBLOCK)
			return false;

		throw WSocketException ("Cannot accept socket.", WSocketException::ERR_ID_ACCEPT);
	}
	socket.init (hClientSocket, in);
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

W32 WSocketUtils::ipAddr2NBO (WCSTR ipAddr)
{
	return inet_addr (ipAddr);
}

// not MT-Safe
WCSTR WSocketUtils::ipAddr2String (W32 ipAddr)
{
	static in_addr in;
	in.s_addr = ipAddr;
	return inet_ntoa (in);
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
		text.format ("Cannot resolve host %s", (WCSTR) hostname);
		throw WUnknownHostException (text, -1);
	}
	return *(W32*) h->h_addr_list[0];
}

WCSTR WSocketUtils::resolveIpAddress (W32 ipAddr)
{
	hostent* h = gethostbyaddr ((WCSTR) &ipAddr, 4, 0);
	if (h==NULL) {
		// host not found
		WFormattedString text (512);
		WString str = ipAddr2String (ipAddr);
		text.format ("Cannot resolve IP %s", (WCSTR) str);
		throw WUnknownHostException (text, -1);
	}
	return h->h_name;
}

W32 WSocketUtils::getLocalIpAddress ()
{
	WString str;
	getLocalHost (str);
	return resolveHost (str);
}

void WSocketUtils::getLocalIpAddress (WString& ipAddr)
{
	ipAddr = ipAddr2String (getLocalIpAddress ());
}

void WSocketUtils::getLocalHost (WString& hostname)
{
	if (hostname.getBufLength ()<128)
		hostname.resize (128);
	gethostname ((char*) hostname.getBuf (),
	             hostname.getBufLength ()-1);
	hostname.truncate (strlen ((char*) hostname.getBuf ()));
}


// verry slow but good enought for FTP/SMTP/HTTP protocol
bool WSocketUtils::readln (WSocketIO* socket, WString& str)
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
					return false;
				}
				throw e;
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
	return true;
}


W32 WSocketUtils::read (int hSocket, void* pnt, W32 size, W32& bytesReadSum, bool fPeek)
{
	int bytesRead;
	if ((bytesRead = recv (hSocket, (char*) pnt, size, (fPeek ? MSG_PEEK : 0)))<=0) {
		if (errno==EWOULDBLOCK)
			bytesRead=0;
		else if (errno==EAGAIN)
			throw WSocketException ("Read timeout", WSocketException::ERR_ID_TIMEOUT);
		else if (bytesRead==0) //  || bytesRead==-1)
			throw WSocketException ("Connection was closed.", WSocketException::ERR_ID_CLOSED);
		else
			throw WSocketException ("Cannot read from socket.", WSocketException::ERR_ID_READ);
	}
	bytesReadSum+=bytesRead;
	return (W32) bytesRead;
}

W32 WSocketUtils::write (int hSocket, const void* pnt, W32 size)
{
	int bytesWritten = 0;
	if ((bytesWritten = ::send (hSocket, (WCSTR) pnt, size, MSG_NOSIGNAL))==-1) {
		throw WSocketException ("Cannot write into socket.", WSocketException::ERR_ID_WRITE);
	}
	return (W32) bytesWritten;
}

void WSocketUtils::close (int hSocket, int mode)
{
	::shutdown (hSocket, mode);
	::close  (hSocket);
}



