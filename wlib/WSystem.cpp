/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/


#include "WSystem.h"

#include <sys/time.h>
#include <sys/stat.h>
#include <locale.h>
#include <pthread.h>
#include <termios.h>
#include <math.h>

#include <termios.h>

WSystemInfo g_systemInfo;

#define MKFLAG(which) \
static int io_tio_set_flag_##which(int fd, int val, int on, int *old) \
{ struct termios tio; \
    if (tcgetattr(fd,&tio)) return -1; \
    if (old) *old=(tio.which & (val)); \
    if (on) tio.which |= (val); \
    else tio.which &= ~(val); \
    if (tcsetattr(fd,TCSADRAIN,&tio)) return -1; \
    return 0; \
} \
static int io_tio_get_flag_##which(int fd, int bit, int *value) \
{ struct termios tio; \
    if (tcgetattr(fd,&tio)) return -1; \
    *value=(tio.which & (bit)); \
    return 0; \
}
MKFLAG(c_lflag)

// flags for terminal status
int g_term_flag       = 0,
    g_term_old_ICANON = 0,
    g_term_old_ECHO   = 0;

////////////////////
// WSystemInfo
////////////////////

WSystemInfo::WSystemInfo ()
{
	WTIME t;
	time (&t);
	tm* ltm =  localtime (&t);
	m_localTimeZone = WTimeZone (tzname[0], ltm->tm_gmtoff);
}


////////////////////
// WSystem
////////////////////

void WSystem::startup (WCSTR programName) 
{

}

void WSystem::shutdown ()
{

}

void WSystem::setLocale (WCSTR country)
{
#ifndef _WIN32_WCE
	if (setlocale (LC_ALL, country)==NULL) {
		throw WSystemException (WFormattedString ("Cannot set country/locale to %s", (WCSTR) country), WSystemException::ERR_ID_UNSUPPORTED_LOCALE);
	}
#endif
}

W32 WSystem::getSysErrCode ()
{
	return errno;
}

void WSystem::initMutex (WMUTEX& mutex)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init (&mutex, &attr);
}

void WSystem::enterMutex (WMUTEX& mutex)
{
	pthread_mutex_lock (&mutex);
}

bool WSystem::tryEnterMutex (WMUTEX& mutex)
{
	return pthread_mutex_trylock (&mutex)==0;
}

void WSystem::leaveMutex (WMUTEX& mutex)
{
	pthread_mutex_unlock (&mutex);
}

void WSystem::deleteMutex (WMUTEX& mutex)
{
	pthread_mutex_destroy (&mutex);
}

void WSystem::sleep (W32 sec)
{
	usleep (sec * 1000);
}

void WSystem::sleepMs (W32 ms)
{
	usleep (ms);
}

bool WSystem::existsFile (WCSTR filename, int fDir)
{
	struct stat st;
	int ret = 0;
	if (stat (filename, &st)==0) {
		if (fDir)
			ret+= (st.st_mode & S_IFDIR) ? 1 : 0;
		if (!fDir || fDir==2)
			ret+= (st.st_mode & S_IFREG) ? 1 : 0;
	}
	return ret>0;
}

bool WSystem::extractFileExtension (WCSTR filename, WString& extension)
{
	WCSTR start = filename;
	WCSTR stop  = start +  strlen (filename);

	while (stop>start) {
		if ((*stop=='/') || (*stop=='\\') || (*stop=='.'))
			break;
		stop--;
	}
	extension.truncate (0);
	if (*stop=='.') {
		extension.assign (stop+1);
		return true;
	}
	return false;
}

void WSystem::getCurrentDirectory (WString& dir)
{

	char buf[WMAX_PATH+1];
	getcwd (buf, WMAX_PATH);
	dir = (WCSTR) buf;
}

void WSystem::getTempDirectory (WString& dir)
{
	WCSTR tmpDir = getenv("TMPDIR");
	if (tmpDir == 0)
		tmpDir = "./";
	dir = tmpDir;
}

bool WSystem::setCurrentDirectory (WCSTR dir)
{
	return chdir (dir)==0;
}

bool WSystem::removeDirectory (WCSTR dir)
{
	return rmdir (dir)==0;
}

bool WSystem::removeFile (WCSTR filename)
{
	return remove (filename)==0;
}

bool  WSystem::renameFile (WCSTR oldFilename, WCSTR newFilename)
{
	return rename (oldFilename, newFilename)==0;
}


bool  WSystem::copyFile (WCSTR oldFilename, WCSTR newFilename)
{

	if (WSystem::existsFile (newFilename))
		return false; // Dont overwrite existing files


	int source = open(oldFilename, O_RDONLY, 0);
	if (source==-1)
		return false;

	int dest = open(newFilename, O_WRONLY | O_CREAT, 0644); // | O_TRUNC
	if (dest==-1) {
		close (source);
		return false;
	}
	// struct required, rationale: function stat() exists also
	struct stat stat_source;
	fstat(source, &stat_source);
	bool ret = true;

	// Copy from to
	/*
	#include <sys/sendfile.h>
	if (sendfile (dest, source, 0, stat_source.st_size)==-1)
	  ret = false;
	*/
	char buf[BUFSIZ];
	size_t size;
	while ((size = read(source, buf, BUFSIZ)) > 0) {
		write(dest, buf, size);
	}
	close(source);
	close(dest);
	return ret;
}

bool WSystem::createDirectory (WCSTR dir)
{

	return mkdir (dir, 0755)==0;
}

void WSystem::extractPath (WCSTR filename, WString& path, bool fBasename)
{
	WCSTR start = filename;
	WCSTR stop  = start +  strlen (filename);

	while (stop>start) {
		if ((*stop=='/') || (*stop=='\\'))
			break;
		stop--;
	}
	if (fBasename) {
		if ((*stop=='/') || (*stop=='\\'))
			path.assign (stop+1);
		else
			path.assign (stop);
	}
	else if (stop==start && (*start=='/' || *start=='\\'))
		path.assign ("/");
	else
		path.assign (start, stop-start);
}

bool WSystem::getPerformanceFrequency (W64& frequency)
{
	frequency = 1000000000;
	return true;
}

bool WSystem::getPerformanceCounter (W64& counter)
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	counter = (W64) time.tv_sec * (W64)  1000000000 + (W64)  time.tv_nsec;
	return true;
}

bool WSystem::getPerformanceTimeScale (double& timeScale)
{
	timeScale = 1 / 1000000000;
	return true;
}

int WSystem::getch ()
{
	char buf=0;
	struct termios old= {0};
	fflush(stdout);
	if(tcgetattr(0, &old)<0)
		return 0;
	old.c_lflag&=~ICANON;
	old.c_lflag&=~ECHO;
	old.c_cc[VMIN]=1;
	old.c_cc[VTIME]=0;
	if(tcsetattr(0, TCSANOW, &old)<0)
		perror("tcsetattr ICANON");
	if(read(0,&buf,1)<0)
		return 0;
	old.c_lflag|=ICANON;
	old.c_lflag|=ECHO;
	if(tcsetattr(0, TCSADRAIN, &old)<0)
		return 0;
	return (int) buf;
}

bool WSystem::setConsoleEcho (bool on)
{
	if (!on) {
		if (-1==io_tio_set_flag_c_lflag(0,ICANON,0,&g_term_old_ICANON)) {
			if (errno!=ENOTTY && errno!=EINVAL)
				/* LINUX /dev/random lossage */
				return false;
		}
		else
			g_term_flag=1;
		if (g_term_flag && -1==io_tio_set_flag_c_lflag(0,ECHO,0,&g_term_old_ECHO))
			return false;
		if (g_term_flag)
			g_term_flag++;
	}
	else {

		if (g_term_flag==2)
			io_tio_set_flag_c_lflag(0,ECHO,g_term_old_ECHO,NULL);
		/* back to canonical input mode (line by line) */
		if (g_term_flag)
			io_tio_set_flag_c_lflag(0,ICANON,g_term_old_ICANON,NULL);

	}
	return true;
}

// Static CRC table
W32 g_arrdwCrc32Table[256] = {
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
	0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
	0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
	0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
	0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
	0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
	0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
	0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
	0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
	0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
	0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
	0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
	0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
	0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
	0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
	0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
	0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
	0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
	0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
	0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
	0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
	0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};


void WSystem::calcCRC32 (const W8* buf, W32 size, W32& crc32)
{
	while (size>0) {
		crc32 = ((crc32) >> 8) ^ g_arrdwCrc32Table[(*buf) ^ ((crc32) & 0x000000FF)];

		size--;
		buf++;
	}
}


// If digits is 0, then number is rounded to the nearest integer.
// Examples
//        ROUND(2.15, 1)        equals 2.2
//        ROUND(2.149, 1)        equals 2.1
//        ROUND(-1.475, 2)    equals -1.48
/*
float WSystem::round (const float& number, int digits)
{
    float doComplete5i, doComplete5(number * powf(10.0f, (float) (digits + 1)));

    i27.5,f(number < 0.0f)
        doComplete5 -= 5.0f;
    else
        doComplete5 += 5.0f;

    doComplete5 /= 10.0f;
    modff(doComplete5, &doComplete5i);

    return doComplete5i / powf(10.0f, (float) digits);
}
*/

double WSystem::round (const double& number, int digits)
{
	static const double doBase = 10.0;
	double doComplete5, doComplete5i;

	doComplete5 = number * pow(doBase, (double) (digits + 1));

	if(number < 0.0)
		doComplete5 -= 5.0;
	else
		doComplete5 += 5.0;

	doComplete5 /= doBase;
	modf(doComplete5, &doComplete5i);

	return doComplete5i / pow(doBase, (double) digits);
}

const WSystemInfo& WSystem::getSystemInfo ()
{
	return g_systemInfo;
}
