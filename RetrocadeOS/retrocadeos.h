#include <allegro.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Defines                                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RC_MAX_PATH             1024
#define RC_MAX_GAMES            1024
#define RC_MAX_ROM_NAME_SIZE    64
#define RC_MAX_GAME_NAME_SIZE   256
#define RC_MAX_PREF_SIZE        1024
#define RC_INI                  "./retrocadeos.ini"
#define RC_LOG                  "./retrocadeos.log"
#define RC_SNAP                 "./mpic"
#define RC_404_BMP              "./notfound.bmp"
#define RC_LOGO_BMP             "./logo.bmp"
#define RC_MORE_NORM_BMP        "./more_norm.bmp"
#define RC_MORE_ROR_BMP         "./more_ror.bmp"
#define RC_MORE_BLANK_BMP       "./more_blank.bmp"
#define RC_MORE_BMP_W           100
#define RC_MORE_BMP_H           25
#define RC_HM_QUIT              -1
#define RC_HM_NOP               0
#define RC_HM_REDRAW            1
#define RC_HM_PLAY              2
#define RC_HM_SHUTDOWN          3
#define RC_HM_ROTATE            4
#define RC_HM_SWITCH            5
#define RC_TIMER_ID             100
#define RC_TIMER_FREQ           1000 /*ms*/
#define RC_STATE_MENU           0
#define RC_STATE_SCRSAVE        1
#define RC_SCRSAVE_TIME         15 /*s between trans*/
#define RC_LOGO_X               65
#define RC_LOGO_Y               256
#define RC_DEF_BMP_X            50
#define RC_DEF_BMP_Y            50
#define RC_VK_1                 0x31
#define RC_VK_5                 0x35
#define RC_SELECTION_WIDTH			10

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Data Structures                                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef UINT
#define UINT unsigned long int
#endif

#ifndef BOOK
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct _RCMENUDISP {
  int m_nGameIdx1;
  int m_nGameIdx2;
  int m_nNumGamesX;
  int m_nNumGamesY;
  int m_nCurrSel;
  int m_nScrollStartIdx;
  BITMAP **m_hbmpGames;
  BITMAP *m_hbmpMore;
  BITMAP *m_hbmpMoreBlank;
} RCMENUDISP;

typedef struct _SCRNSVDAT {
  UINT m_uTime;
  UINT m_uNextTime;
  int m_nState;
  BITMAP *m_hbmpCurr;
  UINT m_nLag;
} SCRNSVDAT;

typedef struct _GLOB {
  char       m_szRoms[RC_MAX_GAMES][RC_MAX_ROM_NAME_SIZE];
  char       m_szGames[RC_MAX_GAMES][RC_MAX_GAME_NAME_SIZE];
  char       m_szPrefs[RC_MAX_GAMES][RC_MAX_PREF_SIZE];
  int        m_nNumGames;
  int        m_nScreenX, m_nScreenY;
  int        m_nBmpX, m_nBmpY;
  RCMENUDISP m_menuDisp;
  UINT       m_uTime;
  SCRNSVDAT  m_scrSave;
  BOOL       m_bRor;
  char       m_szCfg[RC_MAX_PREF_SIZE];
  char       m_szPic[RC_MAX_PREF_SIZE];
} RCGLOB;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Globals  (Declared in retrocade.c)                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern RCGLOB gRCGlob;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions                                                                                                       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// prefs.c
int RCGetGamePreferences(void);

// cmdline.c
int RCParseCmdLine(int, char **);

// menu.c
int RCInitMenu(void);
void RCDestroyMenu(void);
void RCDrawMenuRor();
void RCDrawMenuNorm();
void RCDumpCommands();
int RCHandleMenu();
void RCUpdateSelectionRect(void);

// bmp.c
BITMAP* RCLoadBmp(char *, int, int);
void RCDrawBmp(BITMAP *, int, int);
void RCDrawBmpPartial(BITMAP *, int, int, int, int, int, int);

// scr.c
void RCInitScrSave(void);
void RCResetScrSave(void);
int RCUpdateScrSave(void);
void RCDrawScrSave(void);

// retrocade.c
void RCPaintScreen(void);
