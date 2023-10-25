LRESULT CALLBACK WindowFunc (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK BkgrDlgProc (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SpotColorDlgProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK hwndIWindowFunc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK hwndColorWindowFunc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK hwndFirstProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK hwndSecondProc (HWND, UINT, WPARAM, LPARAM);
BOOL LoadGame(HANDLE hwnd);
BOOL SaveGame(HANDLE hwnd);

void InitToolBar();
int func1(),func2(),func3();
void funcNew(),funcFinish();
void InitStatus(),checkMenu(),Scroll_Switch(),load_pic_4_menu();
void write_config_file(),read_config_file();
void drawString(HWND, char *, int);
void PlayMySound(char *);

template<class whatever>
void makeBackGround(HWND, whatever *, char *);
//---------
char szAppName[]="G4W Window"; /*Name of window class*/
HINSTANCE hInst;
HWND hwnd, htoolswnd, hTabWnd=NULL, hStatusWnd, hScroll, hDlg;
HBITMAP hBit, hOldBit, hFon, hBitMenu;
char background0[255]="#1";
char background[255]="";
char path0[255]="#1", path1[255]="#2", path2[255]="#3", path3[255]="#4";
char CurPath[255];
HBRUSH hFonBrush;
TC_ITEM tci;
RECT WinDim;
TBBUTTON tbButtons[NUMBUTTONS];
HDC hdc, memdc;
PAINTSTRUCT paintstruct;
int parts[4];
char gamecode=0, loaded=0, glob_sound=0, status_line=0, table=1, Save_as_Flag;
HBITMAP hbmCheck[MENU_PIC];
#include "socoban.h"
#include "spot.h"
#include "rotms.h"

cl_socoban *p1=NULL;
cl_spot *p2=NULL;
cl_rotms *p3=NULL;
st_Player PlayerDlg, ComputerDlg;
