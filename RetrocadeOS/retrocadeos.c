#include "retrocadeos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TIMER_FREQ 200 /*ms*/

// GLOBALS
RCGLOB gRCGlob;

// PROTOS
void _setGlobalDefaults(void);

//
// This is the variable that keeps track of time
volatile UINT _time = 0L;

//
// This is an internal function used to update our timer
void _update_timer(void) {
	_time++;
}
END_OF_FUNCTION(_update_timer);

//
// main
int main(int argc, char **argv) {
	char szCmdLine[1024];
	SAMPLE *moveSnd, *selSnd, *exitSnd;
	FILE *fp;
	BOOL bQuit = FALSE;

	// Get rid of an old RUNGAME, if any
	if (system("rm -f ./RUNGAME") != 0) {
		fprintf(stderr, "WARNING: rm -f ./RUNGAME did not return 0\n");	
	}

	// Seed generator
	srand(time(NULL));

	// Set the global defaults
	_setGlobalDefaults();

	// Parse the command line
	if (!RCParseCmdLine(argc, argv)) {
		allegro_message("Unable to parse command line");
		goto DIE;
	}

	// Initialize Allegro
	allegro_init();

	// Try to set the screen size
	if (set_gfx_mode(GFX_AUTODETECT, gRCGlob.m_nScreenX, gRCGlob.m_nScreenY, 0, 0) != 0) {
		allegro_message("Unable to set graphics mode: %s", allegro_error);
		goto DIE;
	}

	// Set up sound
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)==-1) {
		allegro_message("WARNING: Unable to install sound: %s", allegro_error);
	}

	// Get the active games from RC_INI
	if ((gRCGlob.m_nNumGames = RCGetGamePreferences()) == 0) {
		allegro_message("No games available\n");
		goto DIE;
	}

	// Initialize our game menu
	if (!RCInitMenu()) {
		allegro_message("Couldn't initialize game menu\n");
		goto DIE;
	}

	// Set up the timer
	LOCK_VARIABLE(_time);
	LOCK_FUNCTION(_update_timer);
	install_int(_update_timer, TIMER_FREQ);  

	// Install the keyboard handler
	install_keyboard();

	// Load the samples
	moveSnd = load_sample("./click.wav");
	selSnd = load_sample("./select.wav");
	exitSnd = load_sample("./quit.wav");

	// Initialize the screen saver
	RCInitScrSave();

	RCPaintScreen();

	// Enter loop
	UINT t = _time;
	while (!bQuit) {
		if (_time != t) {
			t = _time;

			if (gRCGlob.m_scrSave.m_nState == RC_STATE_MENU) {
				RCUpdateSelectionRect();
			}

			if (_time % (1000/TIMER_FREQ) == 0) {
				if (RCUpdateScrSave()) {
					RCDrawScrSave();
				}
			}

			if (keypressed()) {
				if (gRCGlob.m_scrSave.m_nState == RC_STATE_MENU) {
					RCResetScrSave();

					switch(RCHandleMenu()) {
						case RC_HM_QUIT:
							fp = fopen("./RUNGAME", "w");
							fprintf(fp, "STATE=SHELL");
							fflush(fp);
							fclose(fp);
							play_sample(exitSnd, 255, 255, 1000, 0);
							rest(1000);
							bQuit = TRUE;
							break;

						case RC_HM_SHUTDOWN:
							fp = fopen("./RUNGAME", "w");
							fprintf(fp, "STATE=SHUTDOWN");
							fflush(fp);
							fclose(fp);
							play_sample(exitSnd, 255, 255, 1000, 0);
							rest(1000);
							bQuit = TRUE;
							break;

						case RC_HM_REDRAW:
							play_sample(moveSnd, 255, 255, 1000, 0);
							RCPaintScreen();
							break;

						case RC_HM_PLAY:
							play_sample(selSnd, 255, 255, 1000, 0);

							fp = fopen("./RUNGAME", "w");

							// Rom 
							fprintf(fp, "STATE=%s\n", gRCGlob.m_szRoms[gRCGlob.m_menuDisp.m_nCurrSel]);

							// Game Name
							fprintf(fp, "GNAME='%s'\n", gRCGlob.m_szGames[gRCGlob.m_menuDisp.m_nCurrSel]);

							// Game specific prefs
							fprintf(fp, "PREFS=\"%s\"\n", gRCGlob.m_szPrefs[gRCGlob.m_menuDisp.m_nCurrSel]);

							fflush(fp);
							fclose(fp);
							bQuit = TRUE;
							rest(1000);
							break;

						case RC_HM_ROTATE:
							fp = fopen("./RUNGAME", "w");
							fprintf(fp, "STATE=ROTATE");
							fflush(fp);
							fclose(fp);
							rest(1000);
							bQuit = TRUE;
							break;

						case RC_HM_SWITCH:
							fp = fopen("./RUNGAME", "w");
							fprintf(fp, "STATE=SWITCH");
							fflush(fp);
							fclose(fp);
							rest(1000);
							bQuit = TRUE;
							break;

						case RC_HM_NOP:
						default: 
							break;
					}

					t = _time;
				}
				else {
					clear_bitmap(screen);
					RCResetScrSave();
					RCPaintScreen();
				}
			}
		}
	}

	destroy_sample(moveSnd);
	destroy_sample(selSnd);
	destroy_sample(exitSnd);

DIE:
	return 0;
}
END_OF_MAIN();

//
// Sets the glob's default values
void _setGlobalDefaults(void) {
	gRCGlob.m_nScreenX = 1024;
	gRCGlob.m_nScreenY = 768;
	gRCGlob.m_nBmpX = RC_DEF_BMP_X;
	gRCGlob.m_nBmpY = RC_DEF_BMP_Y;
	gRCGlob.m_uTime = 0L;
	gRCGlob.m_scrSave.m_nLag = RC_SCRSAVE_TIME;
	sprintf(gRCGlob.m_szCfg, "%s", RC_INI);
	sprintf(gRCGlob.m_szPic, "%s", RC_SNAP);
	sprintf(gRCGlob.m_szRunGame, "%s", RC_RUNGAME);
	gRCGlob.m_bRor = FALSE;
}

//
// Paints the screen
void RCPaintScreen(void) {
	switch (gRCGlob.m_scrSave.m_nState) {
		case RC_STATE_MENU:
			if (gRCGlob.m_bRor == TRUE) {
				RCDrawMenuRor();
			}
			else {
				RCDrawMenuNorm();
			}
			break;

		case RC_STATE_SCRSAVE:
			RCDrawScrSave();
			break;
	}
}
