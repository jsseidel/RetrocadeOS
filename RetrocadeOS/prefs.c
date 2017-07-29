#include "retrocadeos.h"
#include <stdio.h>
#include <string.h>

#define RC_MAX_LINE 1024

char *_readln(FILE *);

//
// Opens ini file and parses it to get game prefs - returns the number
// of games in the retrocadeos.ini file
int RCGetGamePreferences(void) {
	FILE *fp;
	int nNumGames;
	int nRetVal;
	int nLineNum;
	char *pszLine, *pszToken;

	nRetVal = 0;
	nLineNum = 1;

	if (!(fp = fopen(gRCGlob.m_szCfg, "r"))) {
		return nRetVal;
	}

	// Tokenize the CSV string into rom,name,prefs
	while ((pszLine = _readln(fp)) != NULL) {
		// rom 
		if ((pszToken = strtok(pszLine, ","))==NULL) {
			nRetVal = 0;
			goto ERR_OUT;
		}
		else {
			if (strlen(pszToken) > RC_MAX_ROM_NAME_SIZE - 1) {
				nRetVal = 0;
				goto ERR_OUT;
			}

			sprintf(gRCGlob.m_szRoms[nLineNum-1], "%s", pszToken);
		}

		// name 
		if ((pszToken = strtok(NULL, ","))==NULL) {
			nRetVal = 0;
			goto ERR_OUT;
		}
		else {
			if (strlen(pszToken) > RC_MAX_GAME_NAME_SIZE - 1) {
				nRetVal = 0;
				goto ERR_OUT;
			}

			sprintf(gRCGlob.m_szGames[nLineNum-1], "%s", pszToken);
		}

		// prefs  (can be null)
		if ((pszToken = strtok(NULL, ","))!=NULL) {
			if (strlen(pszToken) > RC_MAX_PREF_SIZE - 1) {
				nRetVal = 0;
				goto ERR_OUT;
			}

			sprintf(gRCGlob.m_szPrefs[nLineNum-1], "%s", pszToken);
		}
		else {
			gRCGlob.m_szPrefs[nLineNum-1][0] = '\0';
		}

		nLineNum++;
	}

	nRetVal = nLineNum-1;

ERR_OUT:
	fclose(fp);
	return nRetVal;
}

//
// Reads a line using the file pointer and returns a reference to a
// static buffer
char *_readln(FILE *fp) {
	static char szLineBuffer[RC_MAX_LINE];
	int nCharsRead;
	char c;

	c = fgetc(fp);
	nCharsRead = 0;
	szLineBuffer[0] = '\0';
	while (c != EOF && c != '\n') {
		szLineBuffer[nCharsRead] = c;
		if (nCharsRead + 1 > RC_MAX_LINE - 1) {
			szLineBuffer[nCharsRead] = '\0';

			// Fast forward till the newline char
			c = fgetc(fp);
			while (c != EOF && c != '\n') {
				c = fgetc(fp);
			}

			return szLineBuffer;
		}
		else {
			szLineBuffer[nCharsRead+1] = '\0';
		}

		c = fgetc(fp);
		nCharsRead++;
	}

	if (strlen(szLineBuffer)) {
		return szLineBuffer;
	}

	return NULL;
}
