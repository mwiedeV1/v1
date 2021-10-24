/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "WFile.h"

#define FILE_EXCEPTION(x, y) (throw WFileException(x, y))

void WFile::initialize()
{
	m_hFile = NULL;
	m_posMode = POS_MODE_BEGIN;
	m_markPos = 0;
	m_eofBytes = 0;
	m_fBlockDevice = true;
	m_fClosable = true;
	m_fNoFilepointer = false;
	m_fEOF = false;
	m_ownFilePos = 0;
	m_readlnBufCnt = 0;
	m_readlnBufPos = 0;
}

WFile::WFile()
{
	initialize();
}

WFile::WFile(WCSTR filename)
{
	initialize();
	open(filename);
}

WFile::WFile(FILE *hFile, bool fBlockDevice, bool fClosable)
{
	initialize();
	open(hFile, fBlockDevice, fClosable);
}

WFile::~WFile()
{
	try {
		close();
	}
	catch (...) {
	}
}

void WFile::setClosable(bool fClosable)
{
	m_fClosable = fClosable;
}

void WFile::create(WCSTR filename, bool fAppend)
{
	this->open(filename, FILE_CREATE | FILE_WRITE | FILE_READ | (fAppend ? FILE_APPEND : 0));
}

void WFile::open(FILE *hFile, bool fBlockDevice, bool fCloseable)
{
	m_hFile = hFile;
	m_fBlockDevice = fBlockDevice;
	m_fClosable = fCloseable;
	m_fNoFilepointer = false;

	if (!m_fBlockDevice) {
		m_fNoFilepointer = true;
	}
}

void WFile::open(WCSTR filename, W32 flag)
{
	if (isOpen())
		close();

	WString mode;

	if (flag & WFile::FILE_APPEND) {
		mode = "a";
		if (flag & WFile::FILE_READ)
			mode += "+";
	}
	else if (flag & WFile::FILE_WRITE) {
		mode = "w";
		if (flag & WFile::FILE_READ)
			mode += "+";
	}
	else if (flag & WFile::FILE_READ) {
		mode = "r";
		if (flag & WFile::FILE_WRITE)
			mode += "+";
	}
	else if (flag & WFile::FILE_CREATE)
		mode += "w";
	else
		mode = "r";

	if (!(m_hFile = fopen(filename, mode))) {
		if (errno == ENOENT)
			throw WFileNotFoundException(WFormattedString("File not found: %s", (WCSTR)filename));
		else
			FILE_EXCEPTION(WFormattedString("Cannot open file: %s", (WCSTR)filename), WFileException::ERR_ID_OPEN);
	}
	m_filename = filename;
}

void WFile::close()
{
	if (!m_fClosable)
		return;
	if (m_hFile != NULL)
		fclose(m_hFile);
	initialize();
}

W32 WFile::write(const void *array, W32 size)
{
	W32 nBytesWritten;
	if ((nBytesWritten = fwrite(array, 1, size, m_hFile)) == -1)
		FILE_EXCEPTION("Cannot write file.", WFileException::ERR_ID_WRITE);
	return nBytesWritten;
};

W32 WFile::write(WCSTR str)
{
	return this->write((void *)str, strlen(str));
};

W32 WFile::writeln(WCSTR str)
{
	W32 nBytesWritten = 0;
	nBytesWritten += this->write((void *)str, strlen(str));
	nBytesWritten += this->write("\r\n");
	return nBytesWritten;
};

void WFile::flush()
{
	if (fflush(m_hFile) != 0)
		FILE_EXCEPTION("Cannot flush file.", WFileException::ERR_ID_ACCESS);
}

W32 WFile::read(void *pnt, W32 size)
{

	if (m_readlnBufPos > 0) {
		// Empty line buffer
		int max = size;
		if ((m_readlnBufPos + max) >= m_readlnBufCnt) {
			max = m_readlnBufCnt - m_readlnBufPos;
			memcpy(pnt, &m_readlnBuf[m_readlnBufPos], max);
			m_readlnBufPos = 0;
		}
		else {
			memcpy(pnt, &m_readlnBuf[m_readlnBufPos], max);
			m_readlnBufPos += max;
		}
		size -= max;
		pnt = (W8 *)pnt + max;
	}

	W32 nBytesRead = 0;
	if (!m_fEOF && (nBytesRead = fread(pnt, 1, size, m_hFile)) < 0) {
		FILE_EXCEPTION("Cannot read file.", WFileException::ERR_ID_READ);
	}
	m_ownFilePos += (W32)nBytesRead;
	if (nBytesRead < size) {
		m_eofBytes = nBytesRead;
		m_fEOF = true;
		throw WEOFException("End of file reached.");
	}
	return nBytesRead;
};

int WFile::readByte()
{
	W8 c;
	read(&c, 1);
	return c;
}

bool WFile::readln(WString &str, bool fRemoveLF)
{
	m_eofBytes = 0;
	str.truncate(0);
	bool fFound = false;
	W32 len;
	do {
		// Check if read from existing buffer
		if (m_readlnBufPos == 0 || m_readlnBufPos >= m_readlnBufCnt) {
			m_readlnBufCnt = READLN_BLOCKSIZE;
			m_readlnBufPos = 0;
			try {
				read(m_readlnBuf, m_readlnBufCnt);
			}
			catch (WEOFException) {
				if ((m_readlnBufCnt = m_eofBytes) == 0) {
					// Remove '\r'
					len = str.length();
					if (fRemoveLF && len > 0 && str.charAt(len - 1) == '\r')
						str.truncate(len - 1);
					return len > 0;
				}
				else {
					// Give a last try to read!
					m_fEOF = false;
				}
			}
		}
		// Search new line
		W8 *start = &m_readlnBuf[m_readlnBufPos];
		len = 0;
		while (m_readlnBufPos < m_readlnBufCnt) {
			if (m_readlnBuf[m_readlnBufPos] == '\n') {
				fFound = true;
				break;
			}
			m_readlnBufPos++;
			len++;
		}
		// Append buffer to string
		str.append(start, len);
	}
	while (!fFound && !m_fEOF);
	if (fFound) {
		// Overread \n
		m_readlnBufPos++;
	}
	// Remove '\r'
	len = str.length();
	if (fRemoveLF && len > 0 && str.charAt(len - 1) == '\r')
		str.truncate(len - 1);

	// Reset filepointer only for block devices
	if (m_fBlockDevice) {
		if (fseek(m_hFile, -(m_readlnBufCnt - m_readlnBufPos), SEEK_CUR) == -1)
			FILE_EXCEPTION("Cannot set file pointer.", WFileException::ERR_ID_ACCESS);
		m_readlnBufPos = 0;
		m_fEOF = false;
	}
	return true;
}

void WFile::mark()
{
	m_markPos = getPos();
}

void WFile::reset()
{
	int oldPosMode = m_posMode;
	setPosMode(POS_MODE_BEGIN);
	setPos(m_markPos);
	setPosMode(oldPosMode);
}

void WFile::setPosMode(int mode)
{
	m_posMode = mode;
}

void WFile::setPos(int pos)
{
	if (m_fNoFilepointer)
		return;

	if (fseek(m_hFile, pos, m_posMode) == -1)
		FILE_EXCEPTION("Cannot set file pointer.", WFileException::ERR_ID_ACCESS);
}

void WFile::skip(int distance)
{
	if (m_fNoFilepointer)
		return;

	if (fseek(m_hFile, distance, SEEK_CUR) == -1)
		FILE_EXCEPTION("Cannot set file pointer.", WFileException::ERR_ID_ACCESS);
}

W32 WFile::getPos() const
{
	if (!m_fBlockDevice) {
		// Calc the real file pos if file pointer dont work (for streams)
		return m_ownFilePos - (m_readlnBufCnt - m_readlnBufPos);
	}

	int pos;
	if ((pos = ftell(m_hFile)) == -1)
		FILE_EXCEPTION("Cannot get file pointer.", WFileException::ERR_ID_ACCESS);
	return (W32)pos;
}

W32 WFile::getSize() const
{
	struct stat st;
	if (fstat(fileno(m_hFile), &st) == -1)
		return 0;
	return st.st_size;
}

bool WFile::isEOF() const
{

	if (m_fNoFilepointer)
		return !isOpen() || m_fEOF;

	return feof(m_hFile); // getPos ()==getSize ();
}

WCSTR WFile::getFilename() const
{
	return m_filename;
}

bool WFile::getRealFilename(WString &filename) const
{

	char buf[WMAX_PATH + 1];
	char *res = realpath(m_filename, buf);
	if (res) {
		filename = (WCSTR)buf;
		return true;
	}
	filename.truncate(0);
	return false;
}

FILE *WFile::getFileHandle() const
{
	return m_hFile;
}

bool WFile::isOpen() const
{
	return m_hFile != NULL;
}

W32 WFile::getEOFBytes() const
{
	return m_eofBytes;
}

bool WFile::getFileInfo(WFileInfo &fileInfo) const
{

	struct stat st;
	if (fstat(fileno(m_hFile), &st) != -1) {
		fileInfo.m_fileType = st.st_mode;
		fileInfo.m_fileSize = st.st_size;
		fileInfo.m_filename = m_filename;
		fileInfo.m_timeCreation = st.st_ctime;
		fileInfo.m_timeLastAccess = st.st_atime;
		fileInfo.m_timeLastModified = st.st_mtime;
		return true;
	}
	return false;
}

////////////////////////////////////////
// WFileInfo
////////////////////////////////////////

WFileInfo::WFileInfo()
{
}

WFileInfo::~WFileInfo()
{
	m_fileType = 0;
	m_fileSize = 0;
	m_timeCreation = 0;
	m_timeLastAccess = 0;
	m_timeLastModified = 0;
}

////////////////////////////////////////
// WDirectoryList
////////////////////////////////////////

WDirectoryEntries::WDirectoryEntries(WCSTR path, WCSTR ext)
{
	if ((m_dir = opendir(path)) != NULL) {
		m_ext = ext;
		m_path = path;
	}
}

bool WDirectoryEntries::nextEntry(WFileInfo &fileInfo)
{
	WString myFilename, myExt;

	dirent *dirent;
	while ((dirent = readdir(m_dir)) != NULL) {
		bool fFound = false;
		myFilename = (WCSTR)dirent->d_name;
		if (m_ext.length() > 0) {
			// check ext
			int pos = myFilename.lastIndexOf('.');
			if (pos != -1) {
				myFilename.substring(pos + 1, myExt);
				if (myExt.equals(m_ext)) {
					fFound = true;
				}
			}
		}
		else {
			fFound = true;
		}
		if (fFound) {
			struct stat st;
			WString path = m_path + "/" + (WCSTR)myFilename;

			if (stat((WCSTR)path, &st) != -1) {
				fileInfo.m_fileType = st.st_mode;
				fileInfo.m_fileSize = st.st_size;
				fileInfo.m_filename = myFilename;
				fileInfo.m_timeCreation = st.st_ctime;
				fileInfo.m_timeLastAccess = st.st_atime;
				fileInfo.m_timeLastModified = st.st_mtime;
				return true;
			}
		}
	}
	return false;
}

WDirectoryEntries::~WDirectoryEntries()
{
	if (m_dir != NULL)
		closedir(m_dir);
}
