/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "Process.h"

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wordexp.h>

extern char **environ;

#define MAX_ARGS 30

Process::Process ()
{
	m_fBusy = false;
}


Process::Process (WCSTR cmdLine)
{
	m_fBusy = false;
	execute (cmdLine);
}

Process::~Process ()
{
	kill ();
}

Process::Handles::Handles ()
{
	memset (aStdinPipe, 0, sizeof (aStdinPipe));
	memset (aStdoutPipe, 0, sizeof (aStdoutPipe));
	hProcess = 0;
}

Process::Handles::~Handles ()
{
	this->free ();
}
void Process::Handles::free ()
{
	if (aStdinPipe[0]) {
		close(aStdinPipe[0]);
		aStdinPipe[0]=0;
	}

	if (aStdinPipe[1]) {
		close(aStdinPipe[1]);
		aStdinPipe[1]=0;
	}

	if (aStdoutPipe[0]) {
		close(aStdoutPipe[0]);
		aStdoutPipe[0]=0;
	}

	if (aStdoutPipe[1]) {
		close(aStdoutPipe[1]);
		aStdoutPipe[1]=0;
	}
}

void Process::error (WCSTR errText)
{
	throw WException (errText, -1);
}

void Process::setBusy (bool on)
{
	m_fBusy = on;
}

bool Process::isBusy () const
{
	return m_fBusy;
}

void Process::kill ()
{
	if (isRunning ()) {
		::kill(m_handles.hProcess, SIGKILL);
		m_handles.free ();
	}
}

void Process::startRead ()
{

	close(m_handles.aStdinPipe[1]);
	m_handles.aStdinPipe[1]=0;

	close(m_handles.aStdoutPipe[1]);
	m_handles.aStdoutPipe[1]=0;

}

void Process::join (W32 ms)
{
	if (isRunning ()) {
		waitpid(m_handles.hProcess, NULL, 0);
	}
}

bool Process::isRunning () const
{
	if (!m_handles.hProcess)
		return false;
	return 0 == ::kill(m_handles.hProcess, 0);
}

int Process::getExitCode () const
{
	int exitCode = -1;
	if (m_handles.hProcess) {
		int status;
		// WNOHANG
		if (waitpid(m_handles.hProcess, &status, 0)!=-1) {
			if (WIFEXITED(status)) {
				exitCode = WEXITSTATUS(status);
			}
		}
	}
	return exitCode;
}



bool Process::execute (WCSTR cmdLine, void* env)
{
	kill ();
	m_handles.free ();

	// Fill arguments from command line and check if its an executable
	int nResult=-1;
	wordexp_t result;
	/* Expand the string for the program to run.  */
	int wordexpRet = wordexp (cmdLine, &result, 0);
	bool fExecutable=false;
	if (!wordexpRet) {
		struct stat st;
		int ret = 0;
		if (stat (result.we_wordv[0], &st)==0 && st.st_mode & S_IXUSR) {
			fExecutable = true;
		}
	}
	if (!fExecutable) {
		if (!wordexpRet)
			wordfree (&result);
		throw WException (WFormattedString ("Cannot execute: %s", (WCSTR) cmdLine),-1);
	}

	if (pipe(m_handles.aStdinPipe) < 0) {
		error("Cannot open pipe for child input redirect.");
		return false;
	}
	if (pipe(m_handles.aStdoutPipe) < 0) {
		m_handles.free ();
		error("Cannot open pipe for child output redirect.");
		return false;
	}

	m_handles.hProcess = fork();
	// printf ("%s %i\r\n", cmdLine, m_handles.hProcess);
	if (0 == m_handles.hProcess) {
		// child continues here

		// redirect stdin
		if (dup2(m_handles.aStdinPipe[0], STDIN_FILENO) == -1) {
			exit(errno);
		}

		// redirect stdout
		if (dup2(m_handles.aStdoutPipe[1], STDOUT_FILENO) == -1) {
			exit(errno);
		}

		// redirect stderr
		if (dup2(m_handles.aStdoutPipe[1], STDERR_FILENO) == -1) {
			exit(errno);
		}

		// all these are for use by parent only
		m_handles.free();

		// Fill environment variables
		char* envList[MAX_ARGS+1];
		if (env!=NULL) {
			unsigned int i=1, i2=0;

			char* s = *environ;
			char* s2 =  (char*) env;
			do {
				envList[i2++]=s2;
				while (*s2!=0) {
					s2++;
				}
				s2++;
			}
			while (*s2!=0 && (i2+1)<MAX_ARGS);

			while (s && (i2+1)<MAX_ARGS) {
				envList[i2++]=s;
				s = *(environ+i);
				i++;
			}

			envList[i2]=NULL;
		}

		// Execute
		nResult = execve(result.we_wordv[0], result.we_wordv, env!=NULL ? envList : environ);

		// if we get here at all, an error occurred, but we are in the child
		// process, so just exit
		exit(nResult);
	}
	else if (m_handles.hProcess > 0) {

		// parent continues here

		// close unused file descriptors, these are for child only
		/*
		close(m_handles.aStdinPipe[0]);
		m_handles.aStdinPipe[0]=0;
		close(m_handles.aStdoutPipe[1]);
		m_handles.aStdoutPipe[1]=0;
		*/

		/*
		// Include error check here
		if (NULL != szMessage) {
		  write(aStdinPipe[PIPE_WRITE], szMessage, strlen(szMessage));
		}

		// Just a char by char read here, you can change it accordingly
		while (read(aStdoutPipe[PIPE_READ], &nChar, 1) == 1) {
		  write(STDOUT_FILENO, &nChar, 1);
		}
		*/

		// done with these in this example program, you would normally keep these
		// open of course as long as you want to talk to the child
		/*
		close(aStdinPipe[PIPE_WRITE]);
		close(aStdoutPipe[PIPE_READ]);
		*/
		wordfree (&result);
		return true;
	}
	else {
		// Failed to create child
		wordfree (&result);
		m_handles.free ();
		throw WException (WFormattedString ("Cannot execute: %s", (WCSTR) cmdLine),-1);
		return false;
	}
	return true;

}


/*
 pid_t intermediate_pid = fork();
if (intermediate_pid == 0) {
    pid_t worker_pid = fork();
    if (worker_pid == 0) {
        do_work();
        _exit(0);
    }

    pid_t timeout_pid = fork();
    if (timeout_pid == 0) {
        sleep(timeout_time);
        _exit(0);
    }

    pid_t exited_pid = wait(NULL);
    if (exited_pid == worker_pid) {
        kill(timeout_pid, SIGKILL);
    } else {
        kill(worker_pid, SIGKILL); // Or something less violent if you prefer
    }
    wait(NULL); // Collect the other process
    _exit(0); // Or some more informative status
}
waitpid(intermediate_pid, 0, 0);
*/
// https://stackoverflow.com/questions/282176/waitpid-equivalent-with-timeout
