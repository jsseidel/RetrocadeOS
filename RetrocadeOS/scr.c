#include "retrocadeos.h"
#include <stdlib.h>
#include <stdio.h>

// Fade functions
void _fade(void);
void _vlines(void);
void _hlines(void);
void _vshade(void);
void _hshade(void);
#define RC_NUM_FADE_FUNCS 5

void _loadRandBmp(void);

typedef void (*FADEFUNC)(void);

// local global
FADEFUNC lgFadeFuncs[RC_NUM_FADE_FUNCS];

//
// Initialize the screen saver
void RCInitScrSave(void) {
	RCResetScrSave();  

	lgFadeFuncs[0] = _fade;
	lgFadeFuncs[1] = _vlines;
	lgFadeFuncs[2] = _hlines;
	lgFadeFuncs[3] = _vshade;
	lgFadeFuncs[4] = _hshade;
}

//
// Resets the timing on the screen saver
void RCResetScrSave(void) {
	gRCGlob.m_scrSave.m_nState = RC_STATE_MENU;
	gRCGlob.m_scrSave.m_uTime = 0L;
	gRCGlob.m_scrSave.m_uNextTime = gRCGlob.m_scrSave.m_nLag;
	if (gRCGlob.m_scrSave.m_hbmpCurr != NULL) {
		destroy_bitmap(gRCGlob.m_scrSave.m_hbmpCurr);
		gRCGlob.m_scrSave.m_hbmpCurr = NULL;
	}
}

//
// Update the screen saver
int RCUpdateScrSave(void) {
	if ((++gRCGlob.m_scrSave.m_uTime >= gRCGlob.m_scrSave.m_uNextTime)) {
		if (gRCGlob.m_scrSave.m_nState == RC_STATE_MENU) {
			gRCGlob.m_scrSave.m_nState = RC_STATE_SCRSAVE;
		}

		gRCGlob.m_scrSave.m_uNextTime = gRCGlob.m_scrSave.m_uTime + gRCGlob.m_scrSave.m_nLag;  

		_loadRandBmp();

		return 1;
	}

	return 0;
}

//
// Draws the current bmp to the screen
void RCDrawScrSave() {
	int i;
	int x, y;
	int nRandFuncIdx;

	if ((gRCGlob.m_scrSave.m_hbmpCurr != NULL)) 
	{
		nRandFuncIdx = rand() % RC_NUM_FADE_FUNCS;
		(*lgFadeFuncs[nRandFuncIdx])();
	}
}

//
// Loads a random bitmap
void _loadRandBmp(void) {
	int nRandIdx;
	char szPath[RC_MAX_PATH];
	int bmpW, bmpH;
	BITMAP *pbmp;

	// Kill off the old one if we have it
	if (gRCGlob.m_scrSave.m_hbmpCurr != NULL) {
		destroy_bitmap(gRCGlob.m_scrSave.m_hbmpCurr);
		gRCGlob.m_scrSave.m_hbmpCurr = NULL;
	}

	// Pick a random bitmap to display from the game list
	nRandIdx = rand() % gRCGlob.m_nNumGames;

	// Create the path
	sprintf(szPath, "%s/%s.bmp",gRCGlob.m_szPic, gRCGlob.m_szRoms[nRandIdx]);

	// Make a proportional size
	bmpH = gRCGlob.m_nScreenY;
	bmpW = (int)(((double)gRCGlob.m_nBmpX*(double)bmpH) / (double)gRCGlob.m_nBmpY);

	// Load bmp
	if (!(pbmp = RCLoadBmp(szPath, bmpW, bmpH))) {
		// Load the default
		pbmp = RCLoadBmp(RC_404_BMP, bmpW, bmpH);
	}

	gRCGlob.m_scrSave.m_hbmpCurr = create_bitmap(gRCGlob.m_nScreenX, gRCGlob.m_nScreenY);
	clear_bitmap(gRCGlob.m_scrSave.m_hbmpCurr);
	blit(pbmp, gRCGlob.m_scrSave.m_hbmpCurr, 0, 0, (gRCGlob.m_nScreenX - pbmp->w)/2, 0, pbmp->w, pbmp->h);
	destroy_bitmap(pbmp);
}

//
// A fade screen transition
void _fade(void) {
	int i, x, y;
	int dim;

	for (i=0; i<8000; i++) {
		x = rand() % gRCGlob.m_nScreenX;
		y = rand() % gRCGlob.m_nScreenY;
		dim = rand()%48 + 16;

		RCDrawBmpPartial(gRCGlob.m_scrSave.m_hbmpCurr, x, y, x, y, dim, dim);
	}

	RCDrawBmp(gRCGlob.m_scrSave.m_hbmpCurr, 0, 0);
}

//
// A lines screen transition
void _vlines(void) {
	int nNumBlocksX, nNumBlocksY, x, y, i;

	nNumBlocksX = (gRCGlob.m_nScreenX/8);
	nNumBlocksY = (gRCGlob.m_nScreenY/8);

	for (y=0; y<nNumBlocksY; y++) {
		if (y%2 == 0) {
			for (x=0; x<nNumBlocksX; x++) {
				for (i=0; i<2; i++) {
					RCDrawBmpPartial(gRCGlob.m_scrSave.m_hbmpCurr, x*8, y*8, x*8, y*8, 8, 8);
				}
			}
		}
	}

	for (y=0; y<nNumBlocksY; y++) {
		if (y%2 == 1) {
			for (x=0; x<nNumBlocksX; x++) {
				for (i=0; i<2; i++) {
					RCDrawBmpPartial(gRCGlob.m_scrSave.m_hbmpCurr, x*8, y*8, x*8, y*8, 8, 8);
				}
			}
		}
	}

	RCDrawBmp(gRCGlob.m_scrSave.m_hbmpCurr, 0, 0);
}

//
// A lines screen transition
void _hlines(void) {
	int nNumBlocksX, nNumBlocksY, x, y, i;

	nNumBlocksX = (gRCGlob.m_nScreenX/8);
	nNumBlocksY = (gRCGlob.m_nScreenY/8);

	for (x=0; x<nNumBlocksX; x++) {
		if (x%2 == 0) {
			for (y=0; y<nNumBlocksY; y++) {
				for (i=0; i<2; i++) {
					RCDrawBmpPartial(gRCGlob.m_scrSave.m_hbmpCurr, x*8, y*8, x*8, y*8, 8, 8);
				}
			}
		}
	}

	for (x=0; x<nNumBlocksX; x++) {
		if (x%2 == 1) {
			for (y=0; y<nNumBlocksY; y++) {
				for (i=0; i<2; i++) {
					RCDrawBmpPartial(gRCGlob.m_scrSave.m_hbmpCurr, x*8, y*8, x*8, y*8, 8, 8);
				}
			}
		}
	}

	RCDrawBmp(gRCGlob.m_scrSave.m_hbmpCurr, 0, 0);
}

//
// A shade screen transition
void _vshade(void) {
	int nNumBlocksX, x, i, j;

	nNumBlocksX = (gRCGlob.m_nScreenX/16);

	for (i=0; i<16; i++) {
		for (x=0; x<nNumBlocksX; x++) {
			RCDrawBmpPartial(gRCGlob.m_scrSave.m_hbmpCurr, i+x*16, 0, i+x*16, 0, 1, gRCGlob.m_nScreenY);
		}
	}
}

//
// A shade screen transition
void _hshade(void) {
	int nNumBlocksY, y, i, j;

	nNumBlocksY = (gRCGlob.m_nScreenY/16);

	for (i=0; i<16; i++) {
		for (y=0; y<nNumBlocksY; y++) {
			RCDrawBmpPartial(gRCGlob.m_scrSave.m_hbmpCurr, 0, i+y*16, 0, i+y*16, gRCGlob.m_nScreenX, 1);
		}
	}
}
