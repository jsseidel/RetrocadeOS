#include "retrocadeos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RC_MAX_ARG_LEN 256
#define RC_DELIM " \t"

void _usage(void);

int RCParseCmdLine(int argc, char **argv) {
	int i;
	int nRetVal;

	nRetVal = 1;

	for (i=1; i<argc; i++) {
		if (!strcmp(argv[i], "-dumpcmds")) { 
			RCDumpCommands();
			exit(0);
		}
		else if (!strcmp(argv[i], "-bmpx")) { 
			if (i+1 < argc) {
				i++;

				if (strlen(argv[i]) >= RC_MAX_ARG_LEN) {
					nRetVal = 0;
					goto ERR_OUT;
				}

				gRCGlob.m_nBmpX = atoi(argv[i]);
			}
			else {
				nRetVal = 0;
				goto ERR_OUT;
			}
		}
		else if (!strcmp(argv[i], "-bmpy")) {	      
			if (i+1 < argc) {
				i++;

				if (strlen(argv[i+1]) >= RC_MAX_ARG_LEN) {
					nRetVal = 0;
					goto ERR_OUT;
				}

				gRCGlob.m_nBmpY = atoi(argv[i]);
			}
			else {
				nRetVal = 0;
				goto ERR_OUT;
			}
		}
		else if (!strcmp(argv[i], "-cx")) {	      
			if (i+1 < argc) {
				i++;

				if (strlen(argv[i]) >= RC_MAX_ARG_LEN) {
					nRetVal = 0;
					goto ERR_OUT;
				}

				gRCGlob.m_nScreenX = atoi(argv[i]);
			}
			else {
				nRetVal = 0;
				goto ERR_OUT;
			}
		}
		else if (!strcmp(argv[i], "-cy")) {	      
			if (i+1 < argc) {
				i++;

				if (strlen(argv[i]) >= RC_MAX_ARG_LEN) {
					nRetVal = 0;
					goto ERR_OUT;
				}

				gRCGlob.m_nScreenY = atoi(argv[i]);
			}
			else {
				nRetVal = 0;
				goto ERR_OUT;
			}
		}  	
		else if (!strcmp(argv[i], "-ss")) {	      
			if (i+1 < argc) {
				i++;

				if (strlen(argv[i]) >= RC_MAX_ARG_LEN) {
					nRetVal = 0;
					goto ERR_OUT;
				}

				gRCGlob.m_scrSave.m_nLag = atoi(argv[i]);
			}
			else {
				nRetVal = 0;
				goto ERR_OUT;
			}
		}
		else if (!strcmp(argv[i], "-cfg")) {	      
			if (i+1 < argc) {
				i++;

				if (strlen(argv[i]) >= RC_MAX_ARG_LEN) {
					nRetVal = 0;
					goto ERR_OUT;
				}

				sprintf(gRCGlob.m_szCfg, "%s", argv[i]);
			}
			else {
				nRetVal = 0;
				goto ERR_OUT;
			}
		}
		else if (!strcmp(argv[i], "-pic")) {	      
			if (i+1 < argc) {
				i++;

				if (strlen(argv[i]) >= RC_MAX_ARG_LEN) {
					nRetVal = 0;
					goto ERR_OUT;
				}

				sprintf(gRCGlob.m_szPic, "%s", argv[i]);
			}
			else {
				nRetVal = 0;
				goto ERR_OUT;
			}
		}
		else if (!strcmp(argv[i], "-ror")) {
			gRCGlob.m_bRor = TRUE;
		}
		else {
			fprintf(stderr, "Unknown arg: %s\n", argv[i]);
			_usage();
			nRetVal = 0;
			goto ERR_OUT;
		}
	}

ERR_OUT:
	return nRetVal;
}

// Spits out usage string
void _usage(void) {
  fprintf(stderr, "Usage: retrocadeos [-cx screen_width] [-cy screen_height] [-bmpx gamebmp_width] [-bmpy gamebmp_height] [-ss screen_saver_lag_seconds] [-ror] [-cfg config_file] [-pic pics_dir]\n");
}
