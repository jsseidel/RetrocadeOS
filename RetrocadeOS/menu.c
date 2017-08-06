#include "retrocadeos.h"
#include <stdio.h>
#include <stdlib.h>

int _nRecX = 0, _nRecY = 0;

void _adjust_scroll(void);
void _drawMoreBmps(int);

void _msg(char *p) {
	fprintf(stderr, "%s", p);
	fflush(stderr);
}

// Initializes the graphics needed to present the games in menu form
int RCInitMenu() {
	char szPath[RC_MAX_PATH];
	int i;
	char *p;
	int w, h;

	// Create room for the bitmap pointers
	gRCGlob.m_menuDisp.m_hbmpGames = (BITMAP **)malloc(sizeof(BITMAP) * gRCGlob.m_nNumGames);

	// Attempt to load a bitmap for each game we want to display
	for (i=0; i<gRCGlob.m_nNumGames; i++) {
		// Create a path
		sprintf(szPath, "%s/%s.bmp", gRCGlob.m_szPic, gRCGlob.m_szRoms[i]);

		// Try to load the bmp
		if ((gRCGlob.m_menuDisp.m_hbmpGames[i] = RCLoadBmp(szPath, gRCGlob.m_nBmpX, gRCGlob.m_nBmpY))==NULL) {
			fprintf(stderr, "Warning: %s not readable\n", szPath);
			// No bmp available, let's use the one we know is there
			if ((gRCGlob.m_menuDisp.m_hbmpGames[i] = RCLoadBmp(RC_404_BMP, gRCGlob.m_nBmpX, gRCGlob.m_nBmpY))==NULL) {
				return 0;
			}
			else {
				textout_ex(gRCGlob.m_menuDisp.m_hbmpGames[i], font, gRCGlob.m_szGames[i], 0, 20, makecol32(255,255,255), -1);
			}
		}
	}

	// Load the "more" bitmap
	p = RC_MORE_NORM_BMP;
	w = RC_MORE_BMP_W;
	h = RC_MORE_BMP_H;

	if (gRCGlob.m_bRor) {
		p = RC_MORE_ROR_BMP;
		w = RC_MORE_BMP_H;
		h = RC_MORE_BMP_W;
	}

	if ((gRCGlob.m_menuDisp.m_hbmpMore = RCLoadBmp(p, w, h))==NULL) {
		return 0;
	}

	p = RC_MORE_BLANK_BMP;
	if ((gRCGlob.m_menuDisp.m_hbmpMoreBlank = RCLoadBmp(p, w, h))==NULL) {
		return 0;
	}

	// Figure out how many games we can fit on the current screen (rotated)
	gRCGlob.m_menuDisp.m_nNumGamesX = gRCGlob.m_nScreenX/gRCGlob.m_nBmpX;
	gRCGlob.m_menuDisp.m_nNumGamesY = gRCGlob.m_nScreenY/gRCGlob.m_nBmpY;
	gRCGlob.m_menuDisp.m_nGameIdx1 = 0;
	gRCGlob.m_menuDisp.m_nGameIdx2 = gRCGlob.m_menuDisp.m_nNumGamesX * gRCGlob.m_menuDisp.m_nNumGamesY;

	gRCGlob.m_menuDisp.m_nCurrSel = 0;
	gRCGlob.m_menuDisp.m_nScrollStartIdx = 0;

	return 1;
}

// Frees up resources associated with the menu
void RCDestroyMenu(void) {
	int i;

	for (i=0; i<gRCGlob.m_nNumGames; i++) {
		if (gRCGlob.m_menuDisp.m_hbmpGames[i] != NULL) {
			destroy_bitmap(gRCGlob.m_menuDisp.m_hbmpGames[i]);
		}
	}

	destroy_bitmap(gRCGlob.m_menuDisp.m_hbmpMore);
	destroy_bitmap(gRCGlob.m_menuDisp.m_hbmpMoreBlank);

	free(gRCGlob.m_menuDisp.m_hbmpGames);
}

// Draws the current menu screen
void RCDrawMenuRor() {
	int col, row;
	int nCellWidth, nCellHeight;
	int x, y;
	int nXOffset, nYOffset;
	int color;
	int nCnt;
	int i;

	nCellWidth = gRCGlob.m_nScreenX / gRCGlob.m_menuDisp.m_nNumGamesX;
	nCellHeight = gRCGlob.m_nScreenY / gRCGlob.m_menuDisp.m_nNumGamesY;

	nXOffset = (nCellWidth - gRCGlob.m_nBmpX)/2;
	nYOffset = (nCellHeight - gRCGlob.m_nBmpY)/2;

	nCnt = gRCGlob.m_menuDisp.m_nScrollStartIdx;

	for (col=gRCGlob.m_menuDisp.m_nNumGamesX-1; col>=0; col--) {
		for (row=0; row<gRCGlob.m_menuDisp.m_nNumGamesY; row++) {
			x = col * nCellWidth + nXOffset;
			y = row * nCellHeight + nYOffset;

			// Draw a black rectangle
			if (nCnt >= gRCGlob.m_nNumGames) {
				rectfill(screen, x, y, x + gRCGlob.m_nBmpX, y + gRCGlob.m_nBmpY, makecol(0x00, 0x00, 0x00));
			}
			else {
				// Draw the bitmap
				RCDrawBmp(gRCGlob.m_menuDisp.m_hbmpGames[nCnt], x, y);

				// Save the position of the current game selection
				if (nCnt == gRCGlob.m_menuDisp.m_nCurrSel) {
					_nRecX = x;
					_nRecY = y;
				}

			}

			nCnt++;
		}
	}

	RCUpdateSelectionRect();

	_drawMoreBmps(nCnt);
}

void RCDrawMenuNorm(void) {
	int col, row;
	int nCellWidth, nCellHeight;
	int x, y;
	int nXOffset, nYOffset;
	int color;
	int nCnt;
	int i;

	nCellWidth = gRCGlob.m_nScreenX / gRCGlob.m_menuDisp.m_nNumGamesX;
	nCellHeight = gRCGlob.m_nScreenY / gRCGlob.m_menuDisp.m_nNumGamesY;

	nXOffset = (nCellWidth - gRCGlob.m_nBmpX)/2;
	nYOffset = (nCellHeight - gRCGlob.m_nBmpY)/2;

	nCnt = gRCGlob.m_menuDisp.m_nScrollStartIdx;

	for (row=0; row<gRCGlob.m_menuDisp.m_nNumGamesY; row++) {
		for (col=0; col<gRCGlob.m_menuDisp.m_nNumGamesX; col++) {
			x = col * nCellWidth + nXOffset;
			y = row * nCellHeight + nYOffset;

			// Draw a black rectangle
			if (nCnt >= gRCGlob.m_nNumGames) {
				rectfill(screen, x, y, x + gRCGlob.m_nBmpX, y + gRCGlob.m_nBmpY, makecol(0x00, 0x00, 0x00));
			}
			else {
				// Draw the bitmap
				RCDrawBmp(gRCGlob.m_menuDisp.m_hbmpGames[nCnt], x, y);

				// Save the position of the current game selection
				if (nCnt == gRCGlob.m_menuDisp.m_nCurrSel) {
					_nRecX = x;
					_nRecY = y;
				}
			}

			nCnt++;
		}
	}

	RCUpdateSelectionRect();

	_drawMoreBmps(nCnt);
}

void RCUpdateSelectionRect(void) {
	static unsigned long int cnt = 0L;
	int i;

	for (i=0; i<RC_SELECTION_WIDTH; i++) {
		int col;

		if (cnt % 2 == 0) {
			col = (i%2==0 ? makecol(0xff, 0xff, 0x00) : makecol(0xff, 0x00, 0x00)); 
		}
		else {
			col = (i%2==0 ? makecol(0xff, 0x00, 0x00) : makecol(0xff, 0xff, 0x00)); 
		}

		rect(screen, 
				_nRecX+i, _nRecY+i, 
				_nRecX + gRCGlob.m_nBmpX - i - 1, _nRecY + gRCGlob.m_nBmpY - i - 1, 
				col);
	}

	cnt++;
}

// Dumps key combinations to stderr
void RCDumpCommands() {
	fprintf(stderr, "Esc + 5 = shutdown\n");
	fprintf(stderr, "Esc + 1 = quit\n");
	fprintf(stderr, "Esc + 2 = rotate\n");
	fprintf(stderr, "Esc + Enter = switch\n");
	fprintf(stderr, "Right/g = right\n");
	fprintf(stderr, "Left/d = left\n");
	fprintf(stderr, "Down/f = down\n");
	fprintf(stderr, "Up/r = up\n");
}

// Handles user input
int RCHandleMenu() {
	// User quitting
	if (key[KEY_ESC] && key[KEY_5]) {
		return RC_HM_SHUTDOWN;
	}
	else if (key[KEY_ESC] && key[KEY_1]) {
		return RC_HM_QUIT;
	}
	else if (key[KEY_ESC] && key[KEY_2]) {
		return RC_HM_ROTATE;
	}
	else if (key[KEY_ESC] && key[KEY_ENTER]) {
		return RC_HM_SWITCH;
	}
	else if (key[KEY_RIGHT] || key[KEY_G]) {
		if (gRCGlob.m_menuDisp.m_nNumGamesY > 1 && gRCGlob.m_menuDisp.m_nCurrSel < (gRCGlob.m_nNumGames - 1)) {
			gRCGlob.m_menuDisp.m_nCurrSel++;

			_adjust_scroll();

			return RC_HM_REDRAW;
		}
	}
	else if (key[KEY_LEFT] || key[KEY_D]) {
		if (gRCGlob.m_menuDisp.m_nNumGamesY > 1 && gRCGlob.m_menuDisp.m_nCurrSel > 0) {
			gRCGlob.m_menuDisp.m_nCurrSel--;

			_adjust_scroll();

			return RC_HM_REDRAW;
		}
	}
	else if (key[KEY_DOWN] || key[KEY_F]) {
		if (gRCGlob.m_menuDisp.m_nCurrSel < gRCGlob.m_nNumGames - gRCGlob.m_menuDisp.m_nNumGamesX) {
			if (gRCGlob.m_bRor) {
				gRCGlob.m_menuDisp.m_nCurrSel += gRCGlob.m_menuDisp.m_nNumGamesY;
			}
			else {
				gRCGlob.m_menuDisp.m_nCurrSel += gRCGlob.m_menuDisp.m_nNumGamesX;
			}

			_adjust_scroll();

			return RC_HM_REDRAW;
		}
		else {
			gRCGlob.m_menuDisp.m_nCurrSel = gRCGlob.m_nNumGames - 1;

			_adjust_scroll();

			return RC_HM_REDRAW;
		}
	}
	else if (key[KEY_UP] || key[KEY_R]) {
		if (gRCGlob.m_menuDisp.m_nCurrSel >= gRCGlob.m_menuDisp.m_nNumGamesY) {
			if (gRCGlob.m_bRor) {
				gRCGlob.m_menuDisp.m_nCurrSel -= gRCGlob.m_menuDisp.m_nNumGamesY;
			}
			else {
				gRCGlob.m_menuDisp.m_nCurrSel -= gRCGlob.m_menuDisp.m_nNumGamesX;
			}

			_adjust_scroll();

			return RC_HM_REDRAW;
		}
	}
	else if (key[KEY_ENTER]) {
		return RC_HM_PLAY;
	}

	return 0;

}

// Moves the scroll "window" up or down as needed
void _adjust_scroll(void)
{
	int nMaxSel, nMinSel;

	// What is the maximum selection we could pick
	nMaxSel = gRCGlob.m_menuDisp.m_nScrollStartIdx + (gRCGlob.m_menuDisp.m_nNumGamesY * gRCGlob.m_menuDisp.m_nNumGamesX) - 1;

	// What is the minimum selection we could pick
	nMinSel = gRCGlob.m_menuDisp.m_nScrollStartIdx;

	// Maybe up the scroll window
	if ((gRCGlob.m_menuDisp.m_nCurrSel > nMaxSel)) {
		if (gRCGlob.m_bRor) {
			gRCGlob.m_menuDisp.m_nScrollStartIdx += gRCGlob.m_menuDisp.m_nNumGamesY*gRCGlob.m_menuDisp.m_nNumGamesX;
		}
		else {
			gRCGlob.m_menuDisp.m_nScrollStartIdx += gRCGlob.m_menuDisp.m_nNumGamesX*gRCGlob.m_menuDisp.m_nNumGamesY;
		}
	}
	// Maybe down the scroll window
	else if (gRCGlob.m_menuDisp.m_nCurrSel < nMinSel) {
		if (gRCGlob.m_bRor) {
			gRCGlob.m_menuDisp.m_nScrollStartIdx -= gRCGlob.m_menuDisp.m_nNumGamesY*gRCGlob.m_menuDisp.m_nNumGamesX;
		}
		else {
			gRCGlob.m_menuDisp.m_nScrollStartIdx -= gRCGlob.m_menuDisp.m_nNumGamesX*gRCGlob.m_menuDisp.m_nNumGamesY;
		}
	}
}

void _drawMoreBmps(int nCnt)
{
	int x, y;

	// Draw the more bmps if necessary
	if (gRCGlob.m_bRor) {
		x = 0;
		y = (gRCGlob.m_nScreenY - RC_MORE_BMP_W)/2;
	}
	else {
		x = (gRCGlob.m_nScreenX - RC_MORE_BMP_W)/2;
		y = gRCGlob.m_nScreenY - RC_MORE_BMP_H;
	}

	if (nCnt < gRCGlob.m_nNumGames) {
		RCDrawBmp(gRCGlob.m_menuDisp.m_hbmpMore, x, y);
	}
	else {
		RCDrawBmp(gRCGlob.m_menuDisp.m_hbmpMoreBlank, x, y);
	}

	// Draw the more bmps if necessary
	if (gRCGlob.m_bRor) {
		x = gRCGlob.m_nScreenX - RC_MORE_BMP_H;
	}
	else {
		y = 0;
	}

	if (nCnt > gRCGlob.m_menuDisp.m_nNumGamesX*gRCGlob.m_menuDisp.m_nNumGamesY) {
		RCDrawBmp(gRCGlob.m_menuDisp.m_hbmpMore, x, y);
	}
	else {
		RCDrawBmp(gRCGlob.m_menuDisp.m_hbmpMoreBlank, x, y);
	}
}
