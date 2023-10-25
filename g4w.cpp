#if !defined( __WINDOWS_H )
#include <windows.h>
#endif
#include <stdio.h>
#include <commctrl.h>
#include <time.h>
#include <bwcc.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
//#pragma startup My_startup_func <priority>
//#pragma exit My_startup_func <priority>
#include "constants.h"
#include "dialogs.h"
#include "globals.h"
#include "filesfunc.h"

//////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst,
LPSTR lpszArgs, int nWinMode)
{
    MSG msg;
    WNDCLASSEX wcl;
    HACCEL hAccel;

    if (!hPrevInst) // If the game is launched, don't launch again.
    {
        hwnd = FindWindow(szAppName, NULL);
        if (hwnd)
        {
            if (IsIconic(hwnd)) ShowWindow(hwnd, SW_RESTORE);
            SetForegroundWindow (hwnd);
            return 0;
        }
    }
    // Filling parameters
    wcl.hInstance=hThisInst;
    wcl.lpszClassName=szAppName;
    wcl.lpfnWndProc=WindowFunc;
    wcl.style=0;
    wcl.cbSize=sizeof(WNDCLASSEX);
    wcl.hIcon=LoadIcon(NULL, IDI_APPLICATION);
    wcl.hIconSm=LoadIcon(hThisInst, "G4WICON");
    wcl.hCursor=LoadCursor(hThisInst, "NANA");
    wcl.lpszMenuName="G4WMENU";
    wcl.cbClsExtra=0;
    wcl.cbWndExtra=0;
    wcl.hbrBackground=(HBRUSH)NULL;

    if (!RegisterClassEx(&wcl)) return 0;
    //----------
    // Member current directory, where the game was loaded.
    GetCurrentDirectory(255, CurPath);
    read_config_file();

    // Creating main window.
    hwnd=CreateWindowEx(WS_EX_DLGMODALFRAME, szAppName, "Soc-Spo-Rot.",
    WS_CAPTION | WS_BORDER | WS_MINIMIZEBOX | WS_SYSMENU,
    0, 0, 640, 480-2, HWND_DESKTOP, NULL, hThisInst, NULL);

    //----------
    // Initialization of menu.
    checkMenu();
    hInst=hThisInst;
    //----------
    GetClientRect(hwnd, &WinDim);
    //----------
    // Toolbar creating and initialization.
    InitToolBar();
    InitCommonControls(); //COMCTL32.DLL
    htoolswnd=CreateToolbarEx(hwnd, WS_VISIBLE | WS_CHILD | WS_BORDER |
    TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS, ID_TOOLBAR, NUMBUTTONS, hThisInst,
    TOOLS_BMP, tbButtons, NUMBUTTONS, 0, 0, 40, 40, sizeof(TBBUTTON));
    //----------
    hAccel=LoadAccelerators(hThisInst, "G4WMENU");
    BWCCRegister(hThisInst);
    //----------
    // Inserting images into the menu.
    load_pic_4_menu();
    ShowWindow (hwnd, SW_SHOWNORMAL);
    UpdateWindow (hwnd);
    //----------
    // Main cicle.
    while (GetMessage (&msg, NULL, 0, 0))
    {
        if (!IsDialogMessage(hDlg, &msg))
        {
            if(!TranslateAccelerator(hwnd, hAccel, &msg))
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
    }
    // if ((UINT)hInstBWCC > 32) FreeLibrary(hInstBWCC);
    return msg.wParam;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Window procedire of main window.
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK
WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static LPTOOLTIPTEXT TText;
    static HMENU hSysMenu;

    switch (message)
    {
        case WM_CREATE:
            // Status bar creating.
            PlayMySound("startgame.wav");
            hStatusWnd=CreateWindow(STATUSCLASSNAME, "", WS_CHILD | WS_VISIBLE,
            0, 0, 0, 0, hwnd, NULL, hInst, NULL);
            // Create timer cicle.
            SetTimer(hwnd, 1, 1000, NULL);

            // System menu initialization.
            hSysMenu=GetSystemMenu(hwnd, FALSE);
            EnableMenuItem(hSysMenu,2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            EnableMenuItem(hSysMenu,4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
            AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL) ;
            AppendMenu(hSysMenu, MF_STRING, IDM_NormalSize, "Normal size") ;
            break;

        case WM_SYSCOMMAND:
            switch (LOWORD (wParam))
            {
                case IDM_NormalSize: // Moves a window to normal pozition.
                    if (status_line) MoveWindow(hwnd, 0, 0, 640, 480-2, TRUE);
                    else MoveWindow(hwnd, 0, 0, 640, 460-2, TRUE);
                    return 0;
            }
            break;

        case WM_DESTROY: // Destroying Timer cicle and created objects.
            KillTimer(hwnd, 1);
            delete p1;
            delete p2;
            delete p3;
            {
                int i;
                for (i=0; i<MENU_PIC; i++) DeleteObject(hbmCheck[i]);
            }
            if (hBit) DeleteObject(hBit);
            if (hFon) DeleteObject(hFon);
            if (hFonBrush) DeleteObject(hFonBrush);
            if (hScroll) DestroyWindow(hScroll);
            if (hTabWnd) DestroyWindow(hTabWnd);
            if (hStatusWnd) DestroyWindow(hStatusWnd);
            strcat(CurPath, "\\sound\\reminder.wav");
            if (glob_sound) PlaySound(CurPath, NULL, SND_FILENAME | SND_SYNC |
            SND_NOSTOP | SND_NOWAIT);
            PostQuitMessage(0);
            break;
        case WM_PAINT: // Drawing in window of choised game.
            hdc=BeginPaint(hwnd, &paintstruct);
            if (!loaded)// If none loaded games, draw background in main window.
            {
                BITMAP bm;

                memdc=CreateCompatibleDC(hdc);

                if (hBit) DeleteObject(hBit);
                if (background0[0]=='#')
                    hBit=LoadBitmap(hInst, background0);
                else
                    hBit=LoadImage(NULL, background0, IMAGE_BITMAP, 0, 0,
                    LR_LOADFROMFILE);

                hOldBit=SelectObject(memdc, hBit);
                GetObject(hBit, sizeof(BITMAP), &bm);
                GetClientRect(hwnd, &WinDim);
                StretchBlt(hdc, 0, 53, WinDim.right, WinDim.bottom-53,
                memdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
                SelectObject(memdc, hOldBit);

                DeleteObject(hBit);
                DeleteDC(memdc);
            }
            EndPaint(hwnd, &paintstruct);
            InitStatus();
            if (hTabWnd)// If (hTabWnd) - you have some loaded game.
            {
                UpdateWindow(hTabWnd);
                switch (gamecode)
                {
                    case 1: // Draw in Socoban window.
                        p1->redraw();
                        break;

                    case 2: // Draw in Spot window.
                        if (p2->error) break;
                        p2->redraw();
                        p2->draw_2_spots(); // Draw addition options in toolbar.
                        p2->table_was_changed=1;
                        break;

                    case 3: // Draw in Rotms window.
                        p3->redraw();
                        break;
                }
            }
            break;

        case WM_TIMER: // Init Status bar by timer.
            InitStatus();
            break;

        case WM_KEYDOWN: // Able to press buttons in game Socoban.
            if (gamecode==1) p1->movetop((char)HIWORD(lParam));
            InitStatus();
            break;

        case WM_LBUTTONDOWN: // Moving by mouse pressing.
            int Xmouse, Ymouse;

            switch (gamecode)
            {
                case 1: // Moving by mouse pressing in Socoban.
                    Xmouse=(HIWORD(lParam)-94) / 25;
                    Ymouse=(LOWORD(lParam)-18) / 25;
                    if ((Xmouse == p1->curX) && (Ymouse > p1->curY))
                        p1->movetop((char)77);
                    if ((Xmouse == p1->curX) && (Ymouse < p1->curY))
                        p1->movetop((char)75);
                    if ((Xmouse > p1->curX) && (Ymouse == p1->curY))
                        p1->movetop((char)80);
                    if ((Xmouse < p1->curX) && (Ymouse == p1->curY))
                        p1->movetop((char)72);
                    InitStatus();
                    break;

                case 2: // Moving by mouse pressing in Spot.
                    Xmouse=(HIWORD(lParam)-94-9) / 40;
                    Ymouse=(LOWORD(lParam)-18) / 40;
                    if (Xmouse<1 || Ymouse<1 || Xmouse>5 || Ymouse>13) break;
                    p2->player_move(Xmouse, Ymouse);
                    InitStatus();
                    break;

                case 3: // Moving by mouse pressing in Rotms.
                    Xmouse=(HIWORD(lParam)-94) / 25;
                    Ymouse=(LOWORD(lParam)-18) / 25;
                    p3->pushbutton(Xmouse, Ymouse);
                    InitStatus();
                    break;
            }
            break;

        case WM_VSCROLL: // Change level of game.
            switch (gamecode)
            {
                case 1: // Change level in Socoban.
                    p1->My_Scrolling(wParam, lParam);
                    break;
                case 3: // Change level in Rotms.
                    p3->My_Scrolling(wParam, lParam);
                    break;
            }
            InitStatus();
            break;

        case WM_NOTIFY: // Change game.
            SetFocus(hwnd);
            NMHDR *nmptr;
            nmptr=(LPNMHDR)lParam;
            if (nmptr->code == TCN_SELCHANGE)
            {
                PlayMySound("changepage.wav");
                int tab;
                tab=TabCtrl_GetCurSel((HWND)(nmptr->hwndFrom));
                TabCtrl_GetItem(hTabWnd, tab, &tci);
                gamecode=tci.lParam;

                checkMenu();
                Scroll_Switch();
                InitStatus();
                InvalidateRect(hwnd, NULL, 0);
                UpdateWindow(hwnd);
            }

            // Initialization of Tool Tips.
            TText=(LPTOOLTIPTEXT)lParam;
            if (TText->hdr.code == TTN_NEEDTEXT)
            {
                switch(TText->hdr.idFrom)
                {
                    case IDM_Socoban: TText->lpszText="Socoban"; break;
                    case IDM_Spot: TText->lpszText="Spot"; break;
                    case IDM_Rotms: TText->lpszText="Rotms"; break;
                    case IDM_Sound: TText->lpszText="Sound"; break;
                    case IDM_Save: TText->lpszText="Save"; break;
                    case IDM_New: TText->lpszText="New"; break;
                    case IDM_Finish: TText->lpszText="Finish"; break;
                    case IDM_Undo: TText->lpszText="Undo"; break;
                }
            }
            break;

        case WM_CTLCOLORSCROLLBAR: // Changing color or fon of Scrollbar.
            if (hFon) DeleteObject(hFon);
            if (hFonBrush) DeleteObject(hFonBrush);
            switch (gamecode)
            {
                case 1: // Fon for Scrollbar in Socoban.
                    hFon=LoadBitmap(hInst, "FON1");
                    break;
                case 2: // Fon for Scrollbar in Spot.
                    hFon=LoadBitmap(hInst,"FON2");
                    break;
                case 3: // Fon for Scrollbar in Rotms.
                    hFon=LoadBitmap(hInst, "FON3");
            }
            hFonBrush=CreatePatternBrush(hFon);
            return (LRESULT)hFonBrush;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDM_Socoban: // Load Socoban.
                    if (func1() == -1) break;
                    if (loaded > 1) InvalidateRect(hwnd, NULL, 0);
                    SetScrollRange(hScroll, SB_CTL, 1, 20, TRUE);
                    SetScrollPos(hScroll, SB_CTL, p1->level, 1);
                    ShowWindow(hScroll, SW_SHOW);
                    break;

                case IDM_Spot: // Load Spot.
                    if (func2() == -1) break;
                    SetScrollRange(hScroll, SB_CTL, 1, 20, TRUE);
                    SetScrollPos(hScroll, SB_CTL, p2->level, 1);
                    ShowWindow(hScroll, SW_HIDE);
                    break;

                case IDM_Rotms: // Load Rotms.
                    if (func3() == -1) break;
                    if (loaded > 1) InvalidateRect(hwnd, NULL, 0);
                    SetScrollRange(hScroll, SB_CTL, 1, 20, TRUE);
                    SetScrollPos(hScroll, SB_CTL, p3->level, 1);
                    ShowWindow(hScroll, SW_SHOW);
                    break;

                case IDM_Status_On: // Switch Status line in ON.
                    MoveWindow(hwnd, 0, 0, 640, 480-2, TRUE);
                    ShowWindow(hStatusWnd, SW_SHOW);
                    CheckMenuItem(GetMenu(hwnd), IDM_Status_On, MF_CHECKED);
                    CheckMenuItem(GetMenu(hwnd), IDM_Status_Off, MF_UNCHECKED);
                    UpdateWindow(hwnd);
                    status_line=1;
                    write_config_file();
                    break;

                case IDM_Status_Off: // Switch Status line in OFF.
                    ShowWindow(hStatusWnd, SW_HIDE);
                    MoveWindow(hwnd, 0, 0, 640, 460-2, TRUE);
                    CheckMenuItem(GetMenu(hwnd), IDM_Status_Off, MF_CHECKED);
                    CheckMenuItem(GetMenu(hwnd), IDM_Status_On, MF_UNCHECKED);
                    UpdateWindow(hwnd);
                    status_line=0;
                    write_config_file();
                    break;

                case IDM_Status: // Auto-Switch Status line in ON/OFF.
                    if (!status_line)
                        SendMessage(hwnd, WM_COMMAND, IDM_Status_On, 0);
                    else
                        SendMessage(hwnd, WM_COMMAND, IDM_Status_Off, 0);
                    break;

                case IDM_On: // Switch Sound in ON.
                    glob_sound=1;
                    CheckMenuItem(GetMenu(hwnd), IDM_On, MF_CHECKED);
                    CheckMenuItem(GetMenu(hwnd), IDM_Off, MF_UNCHECKED);
                    write_config_file();
                    break;

                case IDM_Off: // Switch Sound in OFF.
                    glob_sound=0;
                    CheckMenuItem(GetMenu(hwnd), IDM_Off, MF_CHECKED);
                    CheckMenuItem(GetMenu(hwnd), IDM_On, MF_UNCHECKED);
                    write_config_file();
                    break;

                case IDM_Sound: // Auto-Switch sound in ON/OFF.
                    if (!glob_sound) SendMessage(hwnd, WM_COMMAND, IDM_On, 0);
                    else SendMessage(hwnd, WM_COMMAND, IDM_Off, 0);
                    break;

                case IDM_New: // New game.
                    funcNew();
                    break;

                case IDM_Finish: // Leave current game.
                    funcFinish();
                    break;

                case IDM_Spot_Color: // Open Options of Spot.
                    DialogBox(hInst, "SpotColorDlg", hwnd,
                    (DLGPROC)SpotColorDlgProc);
                    break;

                case IDM_Table: // Switch background in Spot (Table/not Table).
                    table= (table==1)? 2:1;
                    if (table==1)
                        CheckMenuItem(GetMenu(hwnd), IDM_Table, MF_CHECKED);
                    else
                        CheckMenuItem(GetMenu(hwnd), IDM_Table, MF_UNCHECKED);

                    SendMessage(hwnd, WM_PAINT, 0, 0);
                    SetFocus(hwnd);
                    write_config_file();
                    break;

                case IDM_Exit: // Exit from the game.
                    PlayMySound("huh.wav");
                    if (MessageBox(hwnd, "Do you realy want to exit?",
                    "       Exit from the game!!!",
                    MB_YESNO | MB_ICONQUESTION) == IDYES)
                    SendMessage(hwnd, WM_DESTROY, 0, 0);
                    break;

                case IDM_Help: // Help.
                    WinHelp(hwnd, "g4w.hlp", HELP_CONTENTS, 0);
                    break;

                case IDM_About: // About Dialog Window.
                    PlayMySound("box.wav");
                    if (DialogBox(hInst,"AboutBox",hwnd,(DLGPROC)AboutDlgProc))
                        InvalidateRect (hwnd, NULL, TRUE) ;
                    return 0 ;

                case IDM_Undo: // Move back.
                    switch (gamecode)
                    {
                        case 1: // in Socoban.
                            p1->Undo();
                            break;
                        case 2: // in Spot.
                            p2->Undo();
                            break;
                        case 3: // in Rotms.
                            p3->Undo();
                            break;
                    }
                    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
                    break;

                case IDM_Background: // Changing backgrounds in all games.
                    DialogBox(hInst,"BackGroundDlg",hwnd,(DLGPROC)BkgrDlgProc);
                    break;

                case IDM_Load: // Load saved game.
                    LoadGame(hwnd);
                    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
                    break;

                case IDM_Save: // Save quickly current game to disk.
                    switch (gamecode)
                    {
                        case 1: p1->SaveGame("quicksave.soc");
                            break;
                        case 2: p2->SaveGame("quicksave.spo");
                            break;
                        case 3: p3->SaveGame("quicksave.rot");
                            break;
                    }
                    break;

                case IDM_Save_As: // Save current game to disk.
                    Save_as_Flag=1;
                    SaveGame(hwnd);
                    Save_as_Flag=0;
                    break;

                case IDM_Save_All: // Save quickly all current games to disk.
                    if (p1) p1->SaveGame("quicksave.soc");
                    if (p2) p2->SaveGame("quicksave.spo");
                    if (p3) p3->SaveGame("quicksave.rot");
                    break;
            }
            break;

        case WM_CLOSE : // If you trying to close the main window,
            // you will leave the game.
            SendMessage(hwnd, WM_COMMAND, IDM_Exit, 0);
            return 0 ;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Initialization of Tool bar.
//////////////////////////////////////////////////////////////////////////
void InitToolBar()
{
    tbButtons[0].iBitmap=0;
    tbButtons[0].idCommand=IDM_Socoban;
    tbButtons[0].fsState=TBSTATE_ENABLED;
    tbButtons[0].fsStyle=TBSTYLE_CHECK;
    tbButtons[0].dwData=0L;
    tbButtons[0].iString=0;

    tbButtons[1].iBitmap=1;
    tbButtons[1].idCommand=IDM_Spot;
    tbButtons[1].fsState=TBSTATE_ENABLED;
    tbButtons[1].fsStyle=TBSTYLE_CHECK;
    tbButtons[1].dwData=0L;
    tbButtons[1].iString=0;

    tbButtons[2].iBitmap=2;
    tbButtons[2].idCommand=IDM_Rotms;
    tbButtons[2].fsState=TBSTATE_ENABLED;
    tbButtons[2].fsStyle=TBSTYLE_CHECK;
    tbButtons[2].dwData=0L;
    tbButtons[2].iString=0;

    tbButtons[3].iBitmap=0;
    tbButtons[3].idCommand=0;
    tbButtons[3].fsState=TBSTATE_ENABLED;
    tbButtons[3].fsStyle=TBSTYLE_SEP;
    tbButtons[3].dwData=0L;
    tbButtons[3].iString=0;

    tbButtons[4].iBitmap=4;
    tbButtons[4].idCommand=IDM_Sound;
    tbButtons[4].fsState=TBSTATE_ENABLED;
    tbButtons[4].fsStyle=TBSTYLE_BUTTON;
    tbButtons[4].dwData=0L;
    tbButtons[4].iString=0;

    tbButtons[5].iBitmap=5;
    tbButtons[5].idCommand=IDM_Save;
    tbButtons[5].fsState=TBSTATE_ENABLED;
    tbButtons[5].fsStyle=TBSTYLE_BUTTON;
    tbButtons[5].dwData=0L;
    tbButtons[5].iString=0;

    tbButtons[6].iBitmap=6;
    tbButtons[6].idCommand=IDM_New;
    tbButtons[6].fsState=TBSTATE_ENABLED;
    tbButtons[6].fsStyle=TBSTYLE_BUTTON;
    tbButtons[6].dwData=0L;
    tbButtons[6].iString=0;

    tbButtons[7].iBitmap=0;
    tbButtons[7].idCommand=0;
    tbButtons[7].fsState=TBSTATE_ENABLED;
    tbButtons[7].fsStyle=TBSTYLE_SEP;
    tbButtons[7].dwData=0L;
    tbButtons[7].iString=0;

    tbButtons[8].iBitmap=8;
    tbButtons[8].idCommand=IDM_Finish;
    tbButtons[8].fsState=TBSTATE_ENABLED;
    tbButtons[8].fsStyle=TBSTYLE_BUTTON;
    tbButtons[8].dwData=0L;
    tbButtons[8].iString=0;

    tbButtons[9].iBitmap=9;
    tbButtons[9].idCommand=IDM_Undo;
    tbButtons[9].fsState=TBSTATE_ENABLED;
    tbButtons[9].fsStyle=TBSTYLE_BUTTON;
    tbButtons[9].dwData=0L;
    tbButtons[9].iString=0;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Start to play Socoban.
//////////////////////////////////////////////////////////////////////////
int func1()
{
    if (hTabWnd==NULL)
    {
        hTabWnd=CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_DLGFRAME,
        0, 53, 640+1, 361, hwnd, NULL, hInst, NULL);
        tci.mask= TCIF_TEXT | TCIF_PARAM;
        tci.iImage= -1;

        hScroll=CreateWindow("SCROLLBAR", "", WS_CHILD | SBS_VERT,
        599, 89, 28, 311, hwnd, NULL, hInst, NULL);
    }

    if (p1==NULL)
    {
        PlayMySound("load.wav");/*SND_ASYNC*/
        tci.pszText="Socoban";
        tci.lParam=1;
        TabCtrl_InsertItem(hTabWnd, loaded, &tci);
        TabCtrl_SetCurSel(hTabWnd, loaded);
        SendMessage(htoolswnd, TB_HIDEBUTTON, (WPARAM)IDM_Socoban, (LPARAM)1);
        p1=new cl_socoban;
        gamecode=1;
        loaded++;
        if (p1->error)
        {
            funcFinish();
            return -1;
        }
        ShowWindow(p1->hwnd1, SW_SHOW);
        ShowWindow(hTabWnd, SW_SHOW);
        InitStatus();
        checkMenu();
        if (p2) DestroyWindow(p2->but_Table);
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Start to play Spot.
//////////////////////////////////////////////////////////////////////////
int func2()
{
    if (hTabWnd == NULL)
    {
        hTabWnd=CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_DLGFRAME,
        0, 53, 640+1, 361, hwnd, NULL, hInst, NULL);
        tci.mask= TCIF_TEXT | TCIF_PARAM;
        tci.iImage= -1;

        hScroll=CreateWindow("SCROLLBAR", "", WS_CHILD | SBS_VERT,
        599, 89, 28, 311, hwnd, NULL, hInst, NULL);
    }

    if (p2 == NULL)
    {
        PlayMySound("load.wav");
        tci.pszText="Spot";
        tci.lParam=2;
        TabCtrl_InsertItem(hTabWnd, loaded, &tci);
        TabCtrl_SetCurSel(hTabWnd, loaded);
        SendMessage(htoolswnd, TB_HIDEBUTTON, (WPARAM)IDM_Spot, (LPARAM)1);
        p2=new cl_spot;
        gamecode=2;
        loaded++;
        if (p2->error)
        {
            funcFinish();
            return -1;
        }
        ShowWindow(p2->hwnd1, SW_SHOW);
        ShowWindow(hTabWnd, SW_SHOW);
        InitStatus();
        checkMenu();

        if (ComputerDlg.is == 1)
        {
            Sleep(700);
            p2->computer_move();
            p2->check_spots_number();
            InitStatus();
        }
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Start to play Rotms.
//////////////////////////////////////////////////////////////////////////
int func3()
{
    if (hTabWnd == NULL)
    {
        hTabWnd=CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_DLGFRAME,
        0, 53, 640+1, 361, hwnd, NULL, hInst, NULL);
        tci.mask= TCIF_TEXT | TCIF_PARAM;
        tci.iImage= -1;

        hScroll=CreateWindow("SCROLLBAR", "", WS_CHILD | SBS_VERT,
        599, 89, 28, 311, hwnd, NULL, hInst, NULL);
    }

    if (p3 == NULL)
    {
        PlayMySound("load.wav");
        tci.pszText="Rotms";
        tci.lParam=3;
        TabCtrl_InsertItem(hTabWnd, loaded, &tci);
        TabCtrl_SetCurSel(hTabWnd, loaded);
        SendMessage(htoolswnd, TB_HIDEBUTTON, (WPARAM)IDM_Rotms, (LPARAM)1);
        p3=new cl_rotms;
        gamecode=3;
        loaded++;
        if (p3->error)
        {
            funcFinish();
            return -1;
        }
        ShowWindow(p3->hwnd1, SW_SHOW);
        ShowWindow(hTabWnd, SW_SHOW);
        InitStatus();
        checkMenu();
        if (p2) DestroyWindow(p2->but_Table);
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Close current game.
//////////////////////////////////////////////////////////////////////////
void funcFinish()
{
    if (!loaded) return;
    int tab=TabCtrl_GetCurSel(hTabWnd);
    PlayMySound("down.wav");
    if (gamecode == 1) // Close Socoban.
    {
        delete p1;
        p1=NULL;
        loaded--;
        TabCtrl_DeleteItem(hTabWnd,tab);
        SendMessage(htoolswnd, TB_HIDEBUTTON, (WPARAM)IDM_Socoban, (LPARAM)0);
        SendMessage(htoolswnd, TB_CHECKBUTTON, (WPARAM)IDM_Socoban, (LPARAM)0);
    }
    if (gamecode == 2) // Close Spot.
    {
        delete p2;
        p2=NULL;
        loaded--;
        TabCtrl_DeleteItem(hTabWnd,tab);
        SendMessage(htoolswnd, TB_HIDEBUTTON, (WPARAM)IDM_Spot, (LPARAM)0);
        SendMessage(htoolswnd, TB_CHECKBUTTON, (WPARAM)IDM_Spot, (LPARAM)0);
    }
    if (gamecode == 3) // Close Rotms.
    {
        delete p3;
        p3=NULL;
        loaded--;
        TabCtrl_DeleteItem(hTabWnd, tab);
        SendMessage(htoolswnd, TB_HIDEBUTTON, (WPARAM)IDM_Rotms, (LPARAM)0);
        SendMessage(htoolswnd, TB_CHECKBUTTON, (WPARAM)IDM_Rotms, (LPARAM)0);
    }
    if (!p1 && !p2 && !p3) // If all games is closed, remove Status bar.
    {
        DestroyWindow(hTabWnd);
        DestroyWindow(hScroll);
        gamecode=0;
        hTabWnd=NULL;
        checkMenu();
        return ;
    }
    TabCtrl_SetCurSel(hTabWnd, loaded-1);
    TabCtrl_GetItem(hTabWnd, loaded-1, &tci);
    gamecode=tci.lParam;

    checkMenu();
    Scroll_Switch();
    InitStatus();
    InvalidateRect(hwnd, NULL, 0);
    UpdateWindow(hwnd);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Status bar initialization.
//////////////////////////////////////////////////////////////////////////
void InitStatus()
{
    char str1[30], str2[30], str3[30], str4[30];
    int sec;
    static char prev_game=0;
    if (MF_CHECKED == GetMenuState(GetMenu(hwnd), IDM_Status_Off, MF_BYCOMMAND))
        return;

    GetClientRect(hwnd, &WinDim);

    switch (gamecode)
    {
        case 0: // Empty Status bar.
            if (prev_game != 0)
            {
                DestroyWindow(hStatusWnd);
                hStatusWnd=CreateWindow(STATUSCLASSNAME, "", WS_CHILD |
                WS_VISIBLE, 0, 0, 0, 0, hwnd, NULL, hInst, NULL);
            }
            SendMessage(hStatusWnd, SB_SETTEXT, 0, (LPARAM)"Try to play!!!");
            prev_game=gamecode;
            return;

        case 1: // Socoban status.
            if (prev_game != 1)
            {
                for (int i=1; i<=3; i++) parts[i-1]=(WinDim.right) / 3*i;
                SendMessage(hStatusWnd, SB_SETPARTS, 3, (LPARAM)parts);
            }
            sec=p1->retime();
            sprintf(str1, "Time of game: %02d:%02d:%02d",
            sec/3600, sec/60%60, sec%60);
            sprintf(str2, "Level: %d", p1->level);
            sprintf(str3, "Moves maked: %d", p1->moves);
            break;

        case 2: // Spot status.
            if (prev_game != 2)
            {
                for (int i=1; i<=3; i++) parts[i-1]=(WinDim.right) / 3*i;
                SendMessage(hStatusWnd, SB_SETPARTS, 3, (LPARAM)parts);
            }
            sec=p2->retime();
            sprintf(str1, "Time of game: %02d:%02d:%02d",
            sec/3600, sec/60%60, sec%60);
            sprintf(str2, "Player(%d): %d", p2->Player.is, p2->Player.spots);
            sprintf(str3,"Computer(%d): %d",p2->Computer.is,p2->Computer.spots);
            break;

        case 3: // Rotms status.
            if (prev_game != 3)
            {
                for (int i=1; i<=4; i++) parts[i-1]=(WinDim.right) / 4*i;
                SendMessage(hStatusWnd, SB_SETPARTS, 4, (LPARAM)parts);
            }
            sec=p3->retime();
            sprintf(str1, "Time of game: %02d:%02d:%02d",
            sec/3600, sec/60%60, sec%60);
            sprintf(str2, "Level: %d", p3->level);
            sprintf(str3, "Moves: %d", p3->moves);
            sprintf(str4, "Score: %d", p3->score);
            SendMessage(hStatusWnd, SB_SETTEXT, 3, (LPARAM)str4);
            break;
    }

    SendMessage(hStatusWnd, SB_SETTEXT, 0, (LPARAM)str1);
    SendMessage(hStatusWnd, SB_SETTEXT, 1, (LPARAM)str2);
    SendMessage(hStatusWnd, SB_SETTEXT, 2, (LPARAM)str3);
    prev_game=gamecode;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// New game(Start to play from the begining).
//////////////////////////////////////////////////////////////////////////
void funcNew()
{
    switch (gamecode)
    {
        case 1: // New Socoban.
            p1->NewGame();
            break;
        case 2: // New Spot.
            p2->NewGame();
            break;
        case 3: // New Rotms.
            p3->NewGame();
            break;
    }
    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Write configuration to disk.
//////////////////////////////////////////////////////////////////////////
void write_config_file()
{
    struct
    {
        char sound;
        char status;
        char table;
        st_Player pl,comp;
        char lpath0[255], lpath1[255], lpath2[255], lpath3[255];
    }temp;

    int handle;
    int length=sizeof(temp);
    char filename[255];

    strcpy(filename, CurPath);
    strcat(filename, "\\g4w.cfg");

    ZeroMemory(&temp, length);

    temp.sound=glob_sound;
    temp.status=status_line;
    temp.table=table;

    temp.pl=PlayerDlg;
    temp.comp=ComputerDlg;
    strcpy(temp.lpath0, path0);
    strcpy(temp.lpath1, path1);
    strcpy(temp.lpath2, path2);
    strcpy(temp.lpath3, path3);

    if ((handle =
    open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE)) == -1)
    {
        MessageBox(hwnd, "Can't create configuration file!", "ERROR!!!", MB_OK |
        MB_ICONERROR);
        return;
    }

    if (write(handle, &temp, sizeof(temp)) != length)
    {
        MessageBox(hwnd, "Can't write configuration file!", "ERROR!!!", MB_OK |
        MB_ICONERROR);
    }

    close(handle);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Read configurations from the disk.
//////////////////////////////////////////////////////////////////////////
void read_config_file()
{
    struct
    {
        char sound;
        char status;
        char table;
        st_Player pl,comp;
        char lpath0[255], lpath1[255], lpath2[255], lpath3[255];
    }temp;

    int handle;
    char filename[255];

    strcpy(filename, CurPath);
    strcat(filename, "\\g4w.cfg");

    if ((handle =
    open(filename, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) != -1)
    {
        if (read(handle, &temp, sizeof(temp)) != -1)
        {
            glob_sound=temp.sound;
            status_line=temp.status;
            table=temp.table;
            
            PlayerDlg=temp.pl;
            ComputerDlg=temp.comp;
            
            strcpy(path0, temp.lpath0);
            strcpy(background0, path0);
            strcpy(path1, temp.lpath1);
            strcpy(path2, temp.lpath2);
            strcpy(path3, temp.lpath3);
        }
        else
            MessageBox(hwnd, "Can't read configuration file!", "ERROR!!!",
            MB_OK | MB_ICONERROR);
        close(handle);
    }
    else
    {
        MessageBox(hwnd, "Can't open configuration file!", "ERROR!!!", MB_OK |
        MB_ICONERROR);
        PlayerDlg.is=1;
        PlayerDlg.color=2;

        ComputerDlg.is=2;
        ComputerDlg.color=3;
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Menu initiakization.
//////////////////////////////////////////////////////////////////////////
void checkMenu()
{
    static char first_time;
    if (first_time == 0)
    {
        if (glob_sound) // Sound ON/OFF.
        {
            CheckMenuItem(GetMenu(hwnd), IDM_On, MF_CHECKED);
            CheckMenuItem(GetMenu(hwnd), IDM_Off, MF_UNCHECKED);
        }
        else
        {
            CheckMenuItem(GetMenu(hwnd), IDM_On, MF_UNCHECKED);
            CheckMenuItem(GetMenu(hwnd), IDM_Off, MF_CHECKED);
        }

        if (status_line) // Status line ON/OFF.
            SendMessage(hwnd, WM_COMMAND, IDM_Status_On, 0);
        else
            SendMessage(hwnd, WM_COMMAND, IDM_Status_Off, 0);

        if (table==1)
            CheckMenuItem(GetMenu(hwnd), IDM_Table, MF_CHECKED);
        else
            CheckMenuItem(GetMenu(hwnd), IDM_Table, MF_UNCHECKED);

        first_time==1;
    }

    if (!loaded)//If there is none loaded games, Hide/Show some options in menu.
    {
        EnableMenuItem(GetMenu(hwnd), IDM_Save, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), IDM_Save_As, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), IDM_Save_All, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), IDM_New, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), IDM_Finish, MF_GRAYED);
    }
    else
    {
        EnableMenuItem(GetMenu(hwnd), IDM_Save, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), IDM_Save_As, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), IDM_Save_All, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), IDM_New, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), IDM_Finish, MF_ENABLED);
        switch (gamecode)
        {
            case 1: // For Socoban.
                if (p1->moves)
                    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                else
                    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
                break;
            case 2: // For Spot.
                if (p2->moves)
                    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                else
                    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
                break;
            case 3: // For Rotms.
                if (p3->moves)
                    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                else
                    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
                break;
        }
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Switch scroll bar(changing level) ON/OFF.
//////////////////////////////////////////////////////////////////////////
void Scroll_Switch()
{
    switch (gamecode)
    {
        case 0: // None loaded games. None Scroll bar.
            return;

        case 1: // Scroll bar for Socoban.
            SetScrollRange(hScroll, SB_CTL, 1, 20, TRUE);
            SetScrollPos(hScroll, SB_CTL, p1->level, 1);
            ShowScrollBar(hScroll, SB_CTL, TRUE);

            if (p2) DestroyWindow(p2->but_Table); // Hide options for Spot.
            break;

        case 2: // Scroll bar for Spot.
            ShowScrollBar(hScroll, SB_CTL, FALSE);
            p2->call_TableDlg();
            break;

        case 3: // Scroll bar for Rotms.
            SetScrollRange(hScroll, SB_CTL, 1, 20, TRUE);
            SetScrollPos(hScroll, SB_CTL, p3->level, 1);
            ShowScrollBar(hScroll, SB_CTL, TRUE);

            if (p2) DestroyWindow(p2->but_Table); // Hide options for Spot.
            break;
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Dialog Window Procedure. Background changing for all games.
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK BkgrDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndI, hwndIl, hwnd1, hwnd2, hwnd3;
    static int i;
    static char str_drive[6]="";
    static char str_directory[255]="";
    static char str_fullpath[255]="";
    static char str_file[255]="";
    static int who_for; //Who to choose background for

    switch (iMsg)
    {
        case WM_INITDIALOG : // Fill forms in dialog window.
            hwnd1=GetDlgItem(hDlg, IDC_COMBOBOX1);
            hwnd2=GetDlgItem(hDlg, IDC_LISTBOX1);
            hwnd3=GetDlgItem(hDlg, IDC_LISTBOX2);
            SendDlgItemMessage(hDlg, IDC_COMBOBOX1, CB_DIR,
            0x4000 | 0x8000, (LPARAM) ((LPSTR) "*"));
            DlgDirList(hDlg, "*", IDC_LISTBOX1, NULL, DDL_DIRECTORY);
            SendDlgItemMessage(hDlg, IDC_LISTBOX2, LB_DIR, 0, (LPARAM)"*.BMP");
            switch(gamecode)
            {
                case 0: // Get background for Start Page.
                    strcpy(background, background0);
                    break;
                case 1: // Get background for Socoban.
                    strcpy(background, p1->background);
                    break;
                case 2: // Get background for Spot.
                    strcpy(background, p2->background);
                    break;
                case 3: // Get background for Rotms.
                    strcpy(background, p3->background);
            }
            hwndI=CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,"borshade",
            "", WS_CHILD | WS_VISIBLE, 15+247-4, 60-40, 110, 110/1.33, hDlg,
            NULL, hInst, NULL);
            hwndIl=CreateWindow("static", "", WS_CHILD | WS_VISIBLE | WS_BORDER
            | SS_GRAYFRAME | SS_GRAYRECT, 15+247, 60+4-40, 110-9, 110/1.33-9,
            hDlg, NULL, hInst, NULL);
            SetWindowLong(hwndIl, GWL_WNDPROC, (LONG)hwndIWindowFunc);
            GetCurrentDirectory(255, str_fullpath);
            SetWindowText(hDlg,"                                  BACKGROUND");
            //----------------------------------------------//
            CheckRadioButton(hDlg, 112, 116, 112+gamecode);

            SendDlgItemMessage(hDlg, IDC_DEFBOX, LB_ADDSTRING, 0, (LPARAM)"#1");
            SendDlgItemMessage(hDlg, IDC_DEFBOX, LB_ADDSTRING, 0, (LPARAM)"#2");
            SendDlgItemMessage(hDlg, IDC_DEFBOX, LB_ADDSTRING, 0, (LPARAM)"#3");
            SendDlgItemMessage(hDlg, IDC_DEFBOX, LB_ADDSTRING, 0, (LPARAM)"#4");

            SetWindowText(GetDlgItem(hDlg, IDC_PATH0), path0);
            SetWindowText(GetDlgItem(hDlg, IDC_PATH1), path1);
            SetWindowText(GetDlgItem(hDlg, IDC_PATH2), path2);
            SetWindowText(GetDlgItem(hDlg, IDC_PATH3), path3);

            who_for=gamecode; // Which game for(picture in window).
            return FALSE;

        case WM_COMMAND :
            switch (LOWORD (wParam))
            {
                case IDC_COMBOBOX1: // Get drive.
                    if (HIWORD(wParam) == LBN_SELCHANGE)
                    {
                        GetDlgItemText(hDlg, IDC_COMBOBOX1, str_drive, 6);
                        str_drive[0]=str_drive[2];
                        str_drive[1]=':';
                        str_drive[2]=NULL;
                        SetCurrentDirectory(str_drive);
                        GetCurrentDirectory(255, str_fullpath);
                        SendDlgItemMessage(hDlg, IDC_LISTBOX1, LB_RESETCONTENT,
                        0, 0);
                        SendDlgItemMessage(hDlg, IDC_LISTBOX1, LB_DIR, 0x37,
                        (LPARAM)"*");
                        SendDlgItemMessage(hDlg, IDC_LISTBOX2, LB_RESETCONTENT,
                        0, 0);
                        SendDlgItemMessage(hDlg, IDC_LISTBOX2, LB_DIR, 0,
                        (LPARAM)"*.BMP");
                    }
                    return TRUE ;
                case IDC_LISTBOX1: // Get directory.
                    if (HIWORD(wParam) == LBN_DBLCLK)
                    {
                        i=SendDlgItemMessage(hDlg, IDC_LISTBOX1, LB_GETCURSEL,
                        0, 0L);
                        SendMessage(hwnd2, LB_GETTEXT, i,(LPARAM)str_directory);
                        if (str_directory[1]=='.' && str_directory[2]=='.')
                        {
                            SetCurrentDirectory("..");
                            SendDlgItemMessage(hDlg, IDC_LISTBOX1,
                            LB_RESETCONTENT, 0, 0);
                            SendDlgItemMessage(hDlg, IDC_LISTBOX1, LB_DIR, 0x37,
                            (LPARAM)"*");
                            SendDlgItemMessage(hDlg, IDC_LISTBOX2,
                            LB_RESETCONTENT, 0, 0);
                            SendDlgItemMessage(hDlg, IDC_LISTBOX2, LB_DIR, 0,
                            (LPARAM)"*.BMP");
                        }
                        else
                        {
                            GetCurrentDirectory(255, str_fullpath);
                            if (str_fullpath[strlen(str_fullpath)-1] != '\\')
                                strcat(str_fullpath, "\\");
                            strcat(str_fullpath, &(str_directory[1]));
                            str_fullpath[strlen(str_fullpath)-1]=NULL;
                            SetCurrentDirectory(str_fullpath);
                            SendDlgItemMessage(hDlg, IDC_LISTBOX1,
                            LB_RESETCONTENT, 0, 0);
                            SendDlgItemMessage(hDlg, IDC_LISTBOX1, LB_DIR, 0x37,
                            (LPARAM)"*");
                            SendDlgItemMessage(hDlg, IDC_LISTBOX2,
                            LB_RESETCONTENT, 0, 0);
                            SendDlgItemMessage(hDlg, IDC_LISTBOX2, LB_DIR, 0,
                            (LPARAM)"*.BMP");
                        }
                        GetCurrentDirectory(255, str_fullpath);
                    }
                    return TRUE ;
                case IDC_LISTBOX2: // Get file(*.BMP)
                    if (HIWORD(wParam) == LBN_SELCHANGE)//LBN_DBLCLK)
                    {
                        GetCurrentDirectory(255, str_fullpath);
                        i=SendDlgItemMessage(hDlg, IDC_LISTBOX2, LB_GETCURSEL,
                        0, 0L);
                        SendMessage(hwnd3, LB_GETTEXT, i, (LPARAM)str_file);
                        if (str_fullpath[strlen(str_fullpath)-1] != '\\')
                            strcat(str_fullpath, "\\");
                        strcat(str_fullpath, str_file);
                        SetWindowText(GetDlgItem(hDlg, IDC_PATH0+who_for),
                        str_fullpath);
                        strcpy(background, str_fullpath);
                        InvalidateRect(hwndIl, NULL, TRUE);
                        UpdateWindow(hwndIl);
                    }
                    return TRUE ;

                case IDC_DEFBOX: // Get background from Pictures by default.
                    if (HIWORD(wParam) == LBN_SELCHANGE)//LBN_DBLCLK)
                    {
                        i=SendDlgItemMessage(hDlg,IDC_DEFBOX,LB_GETCURSEL,0,0L);
                        SendMessage(GetDlgItem(hDlg, IDC_DEFBOX), LB_GETTEXT, i,
                        (LPARAM)background);
                        SetWindowText(GetDlgItem(hDlg, IDC_PATH0+who_for),
                        background);
                        InvalidateRect(hwndIl, NULL, TRUE);
                        UpdateWindow(hwndIl);
                    }
                    return TRUE ;

                case IDC_R0: // Here is background for Start Page.
                    who_for=0;
                    GetWindowText(GetDlgItem(hDlg, IDC_PATH0), background, 255);
                    InvalidateRect(hwndIl, NULL, TRUE);
                    UpdateWindow(hwndIl);
                    return TRUE;

                case IDC_R1: // Here is background for Socoban.
                    who_for=1;
                    GetWindowText(GetDlgItem(hDlg, IDC_PATH1), background, 255);
                    InvalidateRect(hwndIl, NULL, TRUE);
                    UpdateWindow(hwndIl);
                    return TRUE;

                case IDC_R2: // Here is background for Spot.
                    who_for=2;
                    GetWindowText(GetDlgItem(hDlg, IDC_PATH2), background, 255);
                    InvalidateRect(hwndIl, NULL, TRUE);
                    UpdateWindow(hwndIl);
                    return TRUE;

                case IDC_R3: // Here is background for Rotms.
                    who_for=3;
                    GetWindowText(GetDlgItem(hDlg, IDC_PATH3), background, 255);
                    InvalidateRect(hwndIl, NULL, TRUE);
                    UpdateWindow(hwndIl);
                    return TRUE;

                case IDOK : // Aply changes and write to disk.
                    GetWindowText(GetDlgItem(hDlg, IDC_PATH0), path0, 254);
                    GetWindowText(GetDlgItem(hDlg, IDC_PATH1), path1, 254);
                    GetWindowText(GetDlgItem(hDlg, IDC_PATH2), path2, 254);
                    GetWindowText(GetDlgItem(hDlg, IDC_PATH3), path3, 254);

                    strcpy(background0, path0);
                    if (p1) p1->change_background(path1);
                    if (p2) p2->change_background(path2);
                    if (p3) p3->change_background(path3);

                    DestroyWindow(hwndI);
                    DestroyWindow(hwndIl);
                    write_config_file();
                    EndDialog(hDlg, TRUE) ;
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    return TRUE;

                case IDCANCEL : // Cancel from Dialog Box.
                    DestroyWindow(hwndI);
                    DestroyWindow(hwndIl);
                    read_config_file();
                    EndDialog(hDlg, FALSE);
                    return TRUE ;
            }
            break ;

        case WM_CLOSE: // Close Dialog Box.
            DestroyWindow(hwndI);
            DestroyWindow(hwndIl);
            read_config_file();
            EndDialog(hDlg, FALSE) ;
            return TRUE;
    }
    return FALSE ;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Dialog Box
// Options for Spot:
//        1. Changing color of spots
//        2. Moving First/Second
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK
SpotColorDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndColor;
    int cons=32;
    static HWND hwndFirst, hwndSecond;

    switch (iMsg)
    {
        case WM_INITDIALOG: // Draw colors and show current configuration.

            hwndColor=CreateWindow("static", "", WS_CHILD | WS_VISIBLE |
            WS_DLGFRAME | SS_GRAYFRAME | SS_GRAYRECT, 67, 79, 6+cons,6 + 5*cons,
            hDlg, NULL, hInst, NULL);
            SetWindowLong(hwndColor, GWL_WNDPROC, (LONG)hwndColorWindowFunc);
            SendDlgItemMessage(hDlg, PlayerDlg.color, BM_SETCHECK, 1, 0);
            SendDlgItemMessage(hDlg,5 + ComputerDlg.color, BM_SETCHECK, 1, 0);

            hwndFirst=CreateWindow("static", "", WS_CHILD | WS_VISIBLE,
            19, 262, 52, 19, hDlg, NULL, hInst, NULL);
            SetWindowLong(hwndFirst, GWL_WNDPROC, (LONG)hwndFirstProc);

            hwndSecond=CreateWindow("static", "", WS_CHILD | WS_VISIBLE,
            97, 262, 52, 19, hDlg, NULL, hInst, NULL);
            SetWindowLong(hwndSecond, GWL_WNDPROC, (LONG)hwndSecondProc);

            return FALSE;

        case WM_CLOSE: // Close Dialog box and save changes.
            DestroyWindow(hwndColor);
            DestroyWindow(hwndFirst);
            DestroyWindow(hwndSecond);
            write_config_file();
            EndDialog (hDlg, TRUE);
            return TRUE;

        case WM_COMMAND: // Changing colors.
            switch (LOWORD(wParam))
            {   // Variants for Player.
                case IDC_RADIOBUTTON1:
                case IDC_RADIOBUTTON2:
                case IDC_RADIOBUTTON3:
                case IDC_RADIOBUTTON4:
                case IDC_RADIOBUTTON5:
                    PlayerDlg.color=LOWORD(wParam);
                    if (PlayerDlg.color == ComputerDlg.color)
                    {
                        ComputerDlg.color++;
                        if (ComputerDlg.color > 6) ComputerDlg.color=2;
                        CheckRadioButton(hDlg, 7, 11, ComputerDlg.color + 5);
                    }
                    break;

                    // Variants for Computer.
                case IDC_RADIOBUTTON6:
                case IDC_RADIOBUTTON7:
                case IDC_RADIOBUTTON8:
                case IDC_RADIOBUTTON9:
                case IDC_RADIOBUTTON10:
                    ComputerDlg.color=LOWORD(wParam) - 5;
                    if (ComputerDlg.color == PlayerDlg.color)
                    {
                        PlayerDlg.color++;
                        if (PlayerDlg.color > 6) PlayerDlg.color=2;
                        CheckRadioButton(hDlg, 2, 6, PlayerDlg.color);
                    }
                    break;

                case IDC_BUTTON6: // Moving First/Second.
                    int temp;
                    temp=PlayerDlg.is;
                    PlayerDlg.is=ComputerDlg.is;
                    ComputerDlg.is=temp;
                    ShowWindow(hwndFirst, SW_HIDE);
                    ShowWindow(hwndSecond, SW_HIDE);
                    ShowWindow(hwndFirst, SW_SHOW);
                    ShowWindow(hwndSecond, SW_SHOW);
                    break;
            }
            return TRUE;
    }
    return FALSE;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Procedure checks which word ("First" or "Second") to draw in Player's window.
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK
hwndFirstProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static char str[7]="First";

    switch (iMsg)
    {
        case WM_PAINT:
            if (PlayerDlg.is==1) strcpy(str, "First");
            else strcpy(str, "Second");
            drawString(hwnd, str, 0);
            return 0;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//Procedure checks which word ("First" or "Second") to draw in Computer's window.
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK
hwndSecondProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static char str[7]="Second";

    switch (iMsg)
    {
        case WM_PAINT:
            if (ComputerDlg.is==1) strcpy(str, "First");
            else strcpy(str, "Second");
            drawString(hwnd, str, 0);
            return 0;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Draws word "First" and "Second" in dialog box.
//////////////////////////////////////////////////////////////////////////
void drawString(HWND hwndWhere, char *string, int color)
{
    static HDC hdc;
    static PAINTSTRUCT ps;

    HFONT hfnt, hOldFont;
    hdc=BeginPaint(hwndWhere, &ps);
    hfnt = GetStockObject(OEM_FIXED_FONT);
    if (hOldFont = SelectObject(hdc, hfnt))
    {
        GetClientRect(hwndWhere, &WinDim);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT + color));
        WinDim.top+=4;
        DrawText(hdc, string, strlen(string), &WinDim, DT_CENTER | DT_VCENTER);
        SelectObject(hdc, hOldFont);
    }
    EndPaint(hwndWhere, &ps);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// About Dialog Box Procedure.
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
        case WM_INITDIALOG:
            return FALSE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hDlg, 0);
                    return TRUE;
            }
            break;
    }
    return FALSE;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Function draws Pictures in Background Dialog Box.
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK
hwndIWindowFunc(HWND hwndIl, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdcIl, memdcIl;
    static PAINTSTRUCT ps;
    static HBITMAP hBitl, hOldBitl;
    static BITMAP bm;
    char size_string[30];

    switch (iMsg)
    {
        case WM_PAINT:
            hdcIl=BeginPaint(hwndIl, &ps);
            if (background[0] == '#')
                hBitl=LoadBitmap(hInst, background);
            else
               hBitl=LoadImage(NULL, background, IMAGE_BITMAP, 0, 0,
               LR_LOADFROMFILE);
            GetObject(hBitl, sizeof(BITMAP), &bm);
            memdcIl=CreateCompatibleDC(hdcIl);
            hOldBitl=SelectObject(memdcIl, hBitl);
            StretchBlt(hdcIl, 0, 0, 96+3, 91/1.33 + 3, memdcIl, 0, 0,
            bm.bmWidth, bm.bmHeight, SRCCOPY);
            SelectObject(memdcIl, hOldBitl);
            EndPaint(hwndIl, &ps);
            DeleteObject(hBitl);
            DeleteDC(memdcIl);

            sprintf(size_string, "%d x %d", bm.bmWidth, bm.bmHeight);
            SetWindowText(GetDlgItem(GetParent(hwndIl), IDC_SIZE), size_string);
            return 0;
    }
    return DefWindowProc(hwndIl, iMsg, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Function draws colors in "Spot Options" Dialog Box
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK
hwndColorWindowFunc(HWND hwndColor, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdcColor, memdcColor;
    static PAINTSTRUCT ps;
    static HBITMAP hBit, hOldBit;

    switch (iMsg)
    {
        case WM_PAINT:
            hdcColor=BeginPaint(hwndColor, &ps);
            hBit=LoadBitmap(hInst, "SPOTMAP");

            memdcColor=CreateCompatibleDC(hdcColor);
            hOldBit=SelectObject(memdcColor, hBit);

            int i, cons=32;
            for(i=2; i<7; i++)
                StretchBlt(hdcColor, 0, cons*(i-2), cons, cons,
                memdcColor, 80, i*40, 40, 40, SRCCOPY);

            SelectObject(memdcColor, hOldBit);
            EndPaint(hwndColor, &ps);
            DeleteObject(hBit);
            DeleteDC(memdcColor);
            return 0;
    }
    return DefWindowProc(hwndColor, iMsg, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Function loads and draws pictures in menu item.
//////////////////////////////////////////////////////////////////////////
void load_pic_4_menu()
{
    COLORREF crBackground;
    DWORD dwCheck;
    HBRUSH hbrBackground;
    HBRUSH hbrOld;
    HBITMAP hbmOld;
    HDC hdcScreen;
    HDC hdcBitmap;
    int cxCheckWidth;
    int cyCheckHeight;

    int i;

    dwCheck=GetMenuCheckMarkDimensions(); // Get size for picture in menu.
    cyCheckHeight=HIWORD(dwCheck);
    cxCheckWidth=LOWORD(dwCheck);

    crBackground=GetSysColor(COLOR_MENU);
    hbrBackground=CreateSolidBrush(crBackground);

    hdcScreen=CreateDC("DISPLAY", 0, 0, 0);
    hdcBitmap=CreateCompatibleDC(hdcScreen);
    hbrOld=SelectObject(hdcBitmap, hbrBackground);
    //-------
    HBITMAP hMenuMap, hOldMenuMap;
    hMenuMap=LoadBitmap(hInst, "MENUMAP");
    HDC dcMenuMap=CreateCompatibleDC(hdcScreen);
    hOldMenuMap=SelectObject(dcMenuMap, hMenuMap);
    //-------
    // Creating 10 pictures for menu items.
    for (i=0; i<MENU_PIC; i++)
    {
        hbmCheck[i]=CreateCompatibleBitmap(hdcScreen, cxCheckWidth,
        cyCheckHeight);
        if (i==0)
            hbmOld=SelectObject(hdcBitmap, hbmCheck[i]);
        else
            SelectObject(hdcBitmap, hbmCheck[i]);
        PatBlt(hdcBitmap, 0, 0, cxCheckWidth, cyCheckHeight, PATCOPY);

        SelectObject(hdcBitmap, hbmCheck[i]);
        StretchBlt(hdcBitmap, 0, 0, cxCheckWidth, cyCheckHeight, dcMenuMap,
        i*16, 0, 16, 13, SRCCOPY);

        switch (i)
        {
            case 0: // Pictures for item "Load".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_Load, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 1: // Pictures for item "Save".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_Save, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 2: // Pictures for item "Save All".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_Save_All, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 3: // Pictures for item "Exit".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_Exit, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 4: // Pictures for item "Undo".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_Undo, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 5: // Pictures for item "New".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_New, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 6: // Pictures for item "Finish".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_Finish, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 7: // Pictures for item "Spot Options -> Color".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_Spot_Color, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 8: // Pictures for item "Help".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_Help, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;

            case 9: // Pictures for item "About".
                SetMenuItemBitmaps(GetMenu(hwnd), IDM_About, MF_BYCOMMAND,
                hbmCheck[i], hbmCheck[i]);
                break;
        }
    }

    //--------
    SelectObject(dcMenuMap, hOldMenuMap);
    DeleteObject(dcMenuMap);
    DeleteObject(hMenuMap);
    //--------
    SelectObject(hdcBitmap, hbrOld);
    SelectObject(hdcBitmap, hbmOld);
    DeleteDC(hdcBitmap);
    DeleteObject(hbrBackground);
    DeleteDC(hdcScreen);

    DrawMenuBar(hwnd);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Playing needed sound in current time.
//////////////////////////////////////////////////////////////////////////
void PlayMySound(char *soundname)
{
    char path[255];
    sprintf(path, "%s\\sound\\%s", CurPath, soundname);
    if (glob_sound)
    {
        if (strcmp("load.wav", soundname) == 0) // Wait for end of music.
            PlaySound(path, NULL, SND_FILENAME|SND_SYNC|SND_NOSTOP|SND_NOWAIT);
        else
            PlaySound(path, NULL, SND_FILENAME|SND_ASYNC|SND_NOWAIT);
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Creating new backgrounds for Socoban, Spot and Rotms.
// Used for changing backgrounds in dialog box "Background".
//////////////////////////////////////////////////////////////////////////
template<class whatever>
void makeBackGround(HWND hwnd, whatever *P_this, char *str)
{
    if (!strcmp(P_this->background, str)) return; // If same background, exit.
    strcpy(P_this->background, str);
    if (P_this->bkgrnd) DeleteObject(P_this->bkgrnd);
    if (P_this->background[0] == '#') // If pictures is from default pictures,
        P_this->bkgrnd=LoadBitmap(hInst, P_this->background); //- quick loading.
    else
    {
        HBITMAP loc_bkgrnd, loc_Old1, loc_Old2;
        BITMAP loc_bm;
        HDC loc_hdc, loc_memdc1, loc_memdc2;

        loc_bkgrnd=LoadImage(NULL, P_this->background, IMAGE_BITMAP, 0, 0,
        LR_LOADFROMFILE);

        GetObject(loc_bkgrnd, sizeof(BITMAP), &loc_bm);

        loc_hdc=GetDC(hwnd);

        loc_memdc1=CreateCompatibleDC(loc_hdc);
        loc_memdc2=CreateCompatibleDC(loc_hdc);

        P_this->bkgrnd=CreateCompatibleBitmap(loc_hdc, W3, W4);
        loc_Old1=SelectObject(loc_memdc1, P_this->bkgrnd);
        loc_Old2=SelectObject(loc_memdc2, loc_bkgrnd);
        StretchBlt(loc_memdc1, 0, 0, W3, W4, loc_memdc2, 0, 0,
        loc_bm.bmWidth, loc_bm.bmHeight, SRCCOPY);

        SelectObject(loc_memdc1, loc_Old1);
        SelectObject(loc_memdc2, loc_Old2);
        DeleteObject(loc_bkgrnd);
        DeleteDC(loc_memdc1);
        DeleteDC(loc_memdc2);
        ReleaseDC(hwnd, loc_hdc);
    }
}
//////////////////////////////////////////////////////////////////////////


