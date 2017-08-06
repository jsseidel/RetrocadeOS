#include "retrocadeos.h"
#include <stdio.h>

//
// Loads a bitmap from disk
BITMAP *RCLoadBmp(char *pszPath, int cx, int cy) {
	BITMAP *pbmp, *prsBmp;

	if (!(pbmp = load_bitmap(pszPath, NULL))) {
		return NULL;
	}

	// Resize if necessary
	if (pbmp->w != cx || pbmp->h != cy) {
		prsBmp = create_bitmap(cx, cy);
		stretch_blit(pbmp, prsBmp, 0, 0, pbmp->w, pbmp->h, 0, 0, cx, cy);
		destroy_bitmap(pbmp);
		pbmp = prsBmp;
	}

	return pbmp;
}

//
// Draws a scaled bitmap to an hdc
void RCDrawBmp(BITMAP *bmp, int x, int y) {
	blit(bmp, screen, 0, 0, x, y, bmp->w, bmp->h);
}

//
// Draws part of a bitmap to an hdc
void RCDrawBmpPartial(BITMAP *bmp, int xdst, int ydst, int xsrc, int ysrc, int cx, int cy) {
	blit(bmp, screen, xsrc, ysrc, xdst, ydst, cx, cy);
}
