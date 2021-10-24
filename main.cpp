/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: V1 Script
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#include "wscript.h"
#include "ast.h"
#include <sys/resource.h>

using namespace std;

extern int g_programMode;

#ifdef _LOG_PERFORMANCE
W64 g_timerCounterStart;
W64 g_timerCounter;
W64 g_timerFreq;
WFile g_performanceLogFile;
#endif

int main(int argc, char *argv[], char *envp[])
{
	int ret = 0;
	try {
		// dup2(1, 2); // Redirect stderr to stdout
		int argIdx = 0;
		bool fHelp = true;
		bool fEval = false;
		bool fFork = false;
		bool fRedirectOutput = false;
		// int priority = 0;
		WString eval, outputFilename;
		WCSTR filename = NULL;
		InterpreterContext ctx;
		ctx.ctx = &ctx;

		// Startup
		wScriptStartup(ctx);

		// Init arguments
		DataValue datavalue;
		datavalue.datatype = DataValue::DATATYPE_ARRAY;
		datavalue.flags = DataValue::FLAGS_NOWRITE;
		ctx.symbols[0]->put("argv", datavalue);

		// Read params
		for (int i = 0; i < argc; i++) {

			const char *currArg = argv[i];

			if (i == 0 || filename != NULL) {
				// Save arg
				DataValue datavalue = (WCSTR)currArg;
				ctx.symbols[0]->get("argv").arrayList.put(WFormattedString("%u", argIdx++), datavalue)->m_value.flags |= DataValue::FLAGS_NOWRITE;
				continue;
			}

			if (fEval) {
				if (eval.length() > 0)
					eval.append(" ");
				eval.append((WCSTR)currArg);
				continue;
			}

			if (fRedirectOutput) {
				if (outputFilename.length() == 0) {
					outputFilename = (WCSTR)currArg;
					continue;
				}
			}

			if (currArg[0] == '-') {
				switch (currArg[1]) {
					/*
					case 'b':
					    fFork = true; fHelp = false;
					    break;
					*/
					case 'h':
						fHelp = true;
						break;
					case 'v':
						fHelp = false;
						printf("%g\r\n", WSCRIPT_VERSION);
						break;
					case 'l':
						fHelp = false;
						ctx.fOnlySyntaxCheck = true;
						break;
					/*
					    case 'p':
					        if ((i+1)<argc) {
					            i++;
					            priority = _wtoi (argv[i]);
					            switch (priority) {
					                case -2: priority = IDLE_PRIORITY_CLASS; break;
					                case -1: priority = BELOW_NORMAL_PRIORITY_CLASS; break;
					                case  0: priority = NORMAL_PRIORITY_CLASS; break;
					                case  1: priority = ABOVE_NORMAL_PRIORITY_CLASS; break;
					                case  2: priority = HIGH_PRIORITY_CLASS; break;
					                case  3: priority = REALTIME_PRIORITY_CLASS; break;
					                default:
					                    printf ("Illegal process priority: %i\r\n", priority);
					                    exit (1);
					                    break;
					            }
					            SetPriorityClass (GetCurrentProcess(), priority);
					        }
					        else {
					            printf ("Process priority required.\r\n");
					            exit (1);
					        }
					        break;
					    */
					case 'r':
						fHelp = false;
						fEval = true;
						break;
					case 'o':
						fHelp = false;
						fRedirectOutput = true;
						break;
					case 'w':
						fHelp = false;
						ctx.programMode = g_programMode = InterpreterContext::PROGRAM_MODE_WEB;
						ctx.fAutoNL = false;
						wScriptInitCGIVariables(ctx);
						break;
					case '-':
						if (strcmp(currArg, "--help") == 0) {
							fHelp = true;
							break;
						}
					default:
						printf("Unknown option: %s\r\n", currArg);
						exit(1);
						break;
				}
			}
			else if (filename == NULL) {
				// Save arg
				DataValue datavalue = DataValue((WCSTR)currArg);
				ArrayHT::WHash *hash = ctx.symbols[0]->get("argv").arrayList.put(WFormattedString("%u", argIdx++), datavalue);
				filename = hash->m_value.value.c_str();
				hash->m_value.flags |= DataValue::FLAGS_NOWRITE;
				fHelp = false;
			}
		}

		if (fHelp) {
			printf(
			    "V1 Script Language %g\r\n"
			    "https://v1-script.net\n\r\n"
			    "Usage:\r\n\r\nv1 [options] <filename>\r\n\r\n"
			    "Options:\r\n\r\n" /*  "-b\t\tRun in background (hide window)\r\n"\ */
			    "-l\t\tSyntax check only (lint)\r\n"
			    "-o <file>\tPrint and append output to file\r\n" /* "-p <priority>\tProcess priority: -2=IDLE .. 0=Normal .. 3=Realtime\r\n"\ */
			    "-r <code>\tRun code without <?v1 ?> tags\r\n"
			    "-v\t\tShow version\r\n"
			    "-w\t\tWeb mode (CGI)\r\n\r\n",
			    WSCRIPT_VERSION);
			exit(0);
		}

		if (fRedirectOutput) {
			if (!std::freopen((WCSTR)outputFilename, "a+", stdout)) {
				printf("Cannot create output file: %s\r\n", (WCSTR)outputFilename);
				exit(1);
			}
			std::freopen((WCSTR)outputFilename, "a+", stderr);
		}

		if (filename != NULL) {
			ret = ctx.execute(filename);
		}
		else if (fEval) {
			DataValue retvalue;
			ret = ctx.eval(eval, eval.length(), retvalue);
		}
		// Print Headers in Web/CGI Mode finally!
		ctx.printHeaders();

		wScriptShutdown(ctx);
	}
	catch (WException &e) {
		fprintf(stdout, "Fatal error: %s\r\n", (WCSTR)e.toString());
		ret = -1;
	}
	fflush(stdout);
#ifdef _LOG_PERFORMANCE
	W64 counter;
	WSystem::getPerformanceCounter(counter);
	double tDStart = (double)(counter - g_timerCounterStart) / (double)g_timerFreq;
	g_performanceLogFile.writeln(WFormattedString("Complete script time: %.6f sec", tDStart));
#endif
	return ret;
}
