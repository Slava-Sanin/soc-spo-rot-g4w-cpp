class cl_rotms
{
    public:
    HWND hwnd1;
    HDC hdc1, memdc1;
    HBITMAP hmap, hBit1, bkgrnd;
    HGDIOBJ hBrush;

    int level;
    char data_level[A][B], data_lev_gr[A][B], data_undo[A][B];
    time_t starttime, curtime;
    double htime;
    int moves;
    long score, score_undo;
    char background[255];
    char filename[255];
    int curX, curY;
    char flag_push;
    int error;

    public:
    cl_rotms();
    ~cl_rotms();
    int init();
    void NewGame();
    void Undo();
    void member_last_move();
    void bild_ground();
    void change_level();
    int SaveGame(char *);
    int LoadGame(char *);
    void pushbutton(int ,int);
    void movetop(char);
    void fire_all_on_pushing(int, int);
    void redraw();
    void putthis(int ,int ,char);
    double retime();
    void check_end();
    void My_Scrolling(WPARAM, LPARAM);
    void change_background(char *);
};


//////////////////////////////////////////////////////////////////////////
// Constructor bilds a window, background and fills a map of level.
//////////////////////////////////////////////////////////////////////////
cl_rotms::cl_rotms()
{
    hwnd1=CreateWindow("STATIC", NULL, WS_CHILD | WS_DLGFRAME,
    W1-4, W2-4, W3+35, W4+6, hTabWnd, NULL, hInst, NULL);
    strcpy(background, "");
    change_background(path3);
    hmap=LoadBitmap(hInst, "ROTMAP");
    level=1;
    flag_push=0;
    change_level();
    init();
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Destructor destroys created objects.
//////////////////////////////////////////////////////////////////////////
cl_rotms::~cl_rotms()
{
    if (memdc1) DeleteDC(memdc1);
    if (hBrush) DeleteObject(hBrush);
    if (hBit1) DeleteObject(hBit1);
    if (hmap) DeleteObject(hmap);
    if (bkgrnd) DeleteObject(bkgrnd);
    DestroyWindow(hwnd1);
}
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
// Starts a game.
// Loads file of level, initializes timer, bilds background,
// member current position.
//////////////////////////////////////////////////////////////////////////
int cl_rotms::init()
{
    int handle;
    error=0;

    if ((handle=open(filename, O_RDONLY | O_TEXT, S_IREAD)) == -1)
    {
        error=-1;
        MessageBox(hwnd, "Can't open file!", "ERROR!!!", MB_OK | MB_ICONERROR);
        return -1;
    }
    if (read(handle, &data_level, A*B) == -1)
    {
        close(handle);
        error=-1;
        MessageBox(hwnd, "Can't read file!", "ERROR!!!", MB_OK | MB_ICONERROR);
        return -1;
    }
    close(handle);

    starttime=time(NULL);
    score=0;
    score_undo=0;
    moves=0;
    bild_ground();
    member_last_move();
    return 0;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Starts current game from the begining.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::NewGame()
{
    if (moves)
    {
        PlayMySound("changepage.wav");
        init();
        EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Do moving back.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::Undo()
{
    if (GetMenuState(GetMenu(hwnd), IDM_Undo, MF_BYCOMMAND) == MF_GRAYED)
    return;
    for(int x=0; x<A; x++)
    {
        for(int y=0; y<B; y++)
            putthis(x, y, data_undo[x][y]);
    }
    score=score_undo;
    moves--;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Save last moving.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::member_last_move()
{
    for(int x=0; x<A; x++)
        for(int y=0; y<B; y++)
            data_undo[x][y] = data_level[x][y];
    score_undo = score;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Draws a map of level in the window.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::bild_ground()
{
    // find cursor and bild the ground array
    for(int x=0; x<A; x++)
        for(int y=0; y<B; y++)
        {
            switch (data_level[x][y])
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5': data_lev_gr[x][y] = data_level[x][y]; break;
                default:  data_lev_gr[x][y] = ' ';
            }
            putthis(x, y, data_level[x][y]);
        }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Changes level.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::change_level()
{
    char str[8];
    strcpy(filename, CurPath);
    strcat(filename, "\\rotms\\lev");
    sprintf(str, "%d.rot", level);
    strcat(filename, str);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Saves current game un the disk.
//////////////////////////////////////////////////////////////////////////
int cl_rotms::SaveGame(char *rotfilename)
{
    int handle;
    int length=A*B;
    char filename[255];

    if (Save_as_Flag)
        strcpy(filename, rotfilename);
    else
        sprintf(filename, "%s\\%s\\%s", CurPath, "Save", rotfilename);

    if ((handle = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
    S_IREAD | S_IWRITE)) == -1)
    {
        MessageBox(hwnd, "Can't create file!","ERROR!!!", MB_OK | MB_ICONERROR);
        return -1;
    }

    if (write(handle, &data_level, A*B) != length)
    {
        MessageBox(hwnd, "Can't write file!", "ERROR!!!", MB_OK | MB_ICONERROR);
        close(handle);
        return -1;
    }

    close(handle);
    return 0;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Loads a game from the disk.
//////////////////////////////////////////////////////////////////////////
int cl_rotms::LoadGame(char *rotfilename)
{
    strcpy(filename, rotfilename);
    return init();
}
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
// Converting mouse pressing un button to key pressing.
/////////////////////////////////////////////////////////////////////////
void cl_rotms::pushbutton(int x, int y)
{
    if (data_level[x][y]>'0' && data_level[x][y]<'6')
    {
        flag_push=1;
        putthis(x, y, data_level[x][y]);
        curX=x;
        curY=y;
    }
    else return;
    switch (data_level[x][y])
    {
        case '1': member_last_move(); movetop('1'); break; // Left.
        case '2': member_last_move(); movetop('2'); break; // Right.
        case '3': member_last_move(); movetop('3'); break; // Up.
        case '4': member_last_move(); movetop('4'); break; // Down.
        case '5': member_last_move();
            movetop('1'); // Left, Right, Up and Down (All).
            movetop('2');
            movetop('3');
            movetop('4');
    }
    Sleep(200);
    fire_all_on_pushing(x, y); // Fires the rotms.
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Moves the rotms un the board by mouse pressing.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::movetop(char key)
{
    int Xtemp, Ytemp;
    switch (key)
    {
        case '2': // Moving left.
            PlayMySound("move1.wav");
            for (Ytemp=curY-1;((data_level[curX][Ytemp]<'0')
            ||(data_level[curX][Ytemp]>'5'))&&(Ytemp>0); Ytemp--);
            while(Ytemp != curY-1)
            {
                if (data_level[curX][Ytemp] == ' ')
                {
                    putthis(curX, Ytemp, data_level[curX][Ytemp+1]);
                    putthis(curX, Ytemp+1, ' ');
                }
                Ytemp++;
            }
            EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
            break;

        case '1': // Moving right.
            PlayMySound("move1.wav");
            for (Ytemp=curY+1; ((data_level[curX][Ytemp]<'0')
            ||(data_level[curX][Ytemp]>'5'))&&(Ytemp<B-1); Ytemp++);
            while(Ytemp != curY+1)
            {
                if (data_level[curX][Ytemp] == ' ')
                {
                    putthis(curX, Ytemp, data_level[curX][Ytemp-1]);
                    putthis(curX, Ytemp-1, ' ');
                }
                Ytemp--;
            }
            EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
            break;

        case '3': // Moving up.
            PlayMySound("move1.wav");
            for (Xtemp=curX-1; ((data_level[Xtemp][curY]<'0')
            ||(data_level[Xtemp][curY]>'5'))&&(Xtemp>0); Xtemp--);
            while(Xtemp != curX-1)
            {
                if (data_level[Xtemp][curY] == ' ')
                {
                    putthis(Xtemp, curY, data_level[Xtemp+1][curY]);
                    putthis(Xtemp+1, curY, ' ');
                }
                Xtemp++;
            }
            EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
            break;

        case '4': // Moving down.
            PlayMySound("move1.wav");
            for (Xtemp=curX+1; ((data_level[Xtemp][curY]<'0')
            ||(data_level[Xtemp][curY]>'5'))&&(Xtemp<A-1); Xtemp++);
            while(Xtemp != curX+1)
            {
                if (data_level[Xtemp][curY] == ' ')
                {
                    putthis(Xtemp, curY, data_level[Xtemp-1][curY]);
                    putthis(Xtemp-1, curY, ' ');
                }
                Xtemp--;
            }
            EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
            break;
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Refreshing of board by drawing current position of the game.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::redraw()
{
    HDC memdctemp;
    hdc1=GetDC(hwnd1);
    memdctemp=CreateCompatibleDC(hdc1);
    SelectObject(memdctemp, bkgrnd);
    //-------Ramka-------
    HBRUSH hOldBrush=(HBRUSH)SelectObject(memdctemp,
    GetStockObject(HOLLOW_BRUSH));

    HPEN hPen=CreatePen(PS_SOLID, 1, RGB(0,0,0));
    HPEN hOldPen=(HPEN)SelectObject(memdctemp, hPen);
    // Drawing border of window.
    //-------1------
    Rectangle(memdctemp, 0, 0, W3, W4);
    Rectangle(memdctemp, 3, 3, W3-3, W4-3);
    Rectangle(memdctemp, 6, 6, W3-6, W4-6);
    //-------2------
    SelectObject(memdctemp, GetStockObject(HOLLOW_BRUSH));
    //--------------
    SelectObject(memdctemp, hOldPen);
    DeleteObject(hPen);
    for(int i=1; i<6; i++)
    {
        if (i==3) continue;
        hPen=CreatePen(PS_SOLID, 1, RGB((i+1)*40, 255, (i+1)*40));
        SelectObject(memdctemp, hPen);
        Rectangle(memdctemp, i, i, W3-i, W4-i);
        SelectObject(memdctemp, hOldPen);
        DeleteObject(hPen);
    }
    SelectObject(memdctemp, hOldBrush);
    //------------------
    // Drawing background.
    BitBlt(hdc1, 0, 0, W3, W4, memdctemp, 0, 0, SRCCOPY);
    // Drawing a map of current level.
    for(int x=0; x<A; x++)
    {
        for(int y=0; y<B; y++)
        {
            putthis(x, y, data_level[x][y]);
        }
    }
    DeleteDC(memdctemp);
    ReleaseDC(hwnd1, hdc1);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Draws needed sprite in window.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::putthis(int x, int y, char kode)
{
    int kode_x, kode_y;
    HDC memdctemp;
    data_level[x][y] = kode;

    hdc1=GetDC(hwnd1);
    memdctemp=CreateCompatibleDC(hdc1);

    if (kode == ' ') // Draws empty place.
    {
        SelectObject(memdctemp, bkgrnd);
        BitBlt(hdc1, otstup + y*25, otstup + x*25, 25, 25, memdctemp,
        otstup + y*25, otstup + x*25, SRCCOPY);
    }
    else
    {
        switch (kode) // Draws other objects of level map.
        {
            case 'B': kode=6; break;
            case 'G': kode=7; break;
            case 'K': kode=8; break;
            case 'R': kode=9; break;
            case 'S': kode=10; break;
            case 'Y': kode=11; break;
            default: kode=kode-48;
        }
        kode_x=kode*25;
        kode_y=0;
        if (kode>5)
        {
            SelectObject(memdctemp, bkgrnd);
            BitBlt(hdc1, otstup + y*25, otstup + x*25, 25, 25, memdctemp,
            otstup + y*25, otstup + x*25, SRCCOPY);
            SelectObject(memdctemp, hmap);
            BitBlt(hdc1, otstup + y*25, otstup + x*25, 25, 25, memdctemp,
            kode_x, kode_y+25, SRCAND);
            BitBlt(hdc1, otstup + y*25, otstup + x*25, 25, 25, memdctemp,
            kode_x, kode_y, SRCINVERT);
        }
        else
        {
            SelectObject(memdctemp, hmap);
            if (flag_push)
            {
                BitBlt(hdc1, otstup + y*25, otstup + x*25, 25, 25, memdctemp,
                kode_x, kode_y+25, SRCCOPY);
                Sleep(60);
                flag_push=0;
            }
            BitBlt(hdc1, otstup + y*25, otstup + x*25, 25, 25, memdctemp,
            kode_x, kode_y, SRCCOPY);
        }
    }
    DeleteDC(memdctemp);
    ReleaseDC(hwnd1, hdc1);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Time of current game.
//////////////////////////////////////////////////////////////////////////
double cl_rotms::retime()
{
    curtime=time(NULL);
    htime=difftime(curtime, starttime);
    return htime;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Checks if the game is complete.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::check_end()
{
    for(int x=0; x<A; x++)
    {
        for(int y=0; y<B; y++)
        {
            if ((data_level[x][y]=='B') || (data_level[x][y]=='G')
             || (data_level[x][y]=='K') || (data_level[x][y]=='R')
             || (data_level[x][y]=='S') || (data_level[x][y]=='Y'))
                return;
        }
    }
    PlayMySound("end1.wav");
    if (MessageBox(hwnd, "   Next level?","Level complete.",
    MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        if (level==20) // If this is a last level.
            MessageBox(hwnd, "No more levels!", "Level complete.",
            MB_OK | MB_ICONSTOP);
        else
        {
            level++;
            SetScrollPos(hScroll, SB_CTL, level, 1);
            UpdateWindow(hScroll);
            change_level();  // Load next level.
        }
    }
    NewGame(); // Play again.
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Changing level by scrolling bar.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::My_Scrolling(WPARAM wParam, LPARAM lParam)
{
    int prevlevel=level;
    switch (LOWORD(wParam))
    {
        case SB_LINEUP:
            if (level==1)	return;
            else level--;
            break;
        case SB_LINEDOWN:
            if (level==20) return;
            else level++;
            break;
        case SB_THUMBTRACK:
            level=HIWORD(wParam);
            break;
        case SB_PAGEUP:
            level-=1;
            if (level<1) level=1;
            break;
        case SB_PAGEDOWN:
            level+=1;
            if (level>20) level=20;
            break;
        default:
            return;
    }
    if (prevlevel == level) return;
    SetScrollRange((HWND)lParam, SB_CTL, 1, 20, TRUE);
    SetScrollPos((HWND)lParam, SB_CTL, level, 1);
    PlayMySound("move.wav");
    change_level();               // Change and load level.
    init();                       // New game.
    EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Change current background.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::change_background(char *str)
{
    makeBackGround(hwnd1, this, str);
}
//////////////////////////////////////////////////////////////////////////


extern cl_rotms* p3;
#include "fire.h"  // Defining class fire
//////////////////////////////////////////////////////////////////////////
// Searchs the board and fires all rotms that must to fire.
//////////////////////////////////////////////////////////////////////////
void cl_rotms::fire_all_on_pushing(int x, int y)
{
    int Xtemp, Ytemp;
    switch (data_level[x][y])
    {
        case '5': moves-=3;
        case '2': // Search left.
            for (Ytemp=curY-1; ((data_level[curX][Ytemp] < '0')
            || (data_level[curX][Ytemp] > '5')) && (Ytemp>-1); Ytemp--);
            Ytemp++;
            while(Ytemp != curY)
            {
                if (data_level[x][Ytemp] != ' ')
                    fire tempfire(x, Ytemp);
                Ytemp++;
            }
            moves++;
            check_end();
            if (data_level[x][y] == '2') break;

        case '1': // Search up.
            for (Ytemp=curY+1; ((data_level[curX][Ytemp] < '0')
            || (data_level[curX][Ytemp] > '5')) && (Ytemp<B); Ytemp++);
            Ytemp--;
            while(Ytemp != curY)
            {
                if (data_level[x][Ytemp] != ' ')
                    fire temp(x, Ytemp);
                Ytemp--;
            }
            moves++;
            check_end();
            if (data_level[x][y] == '1') break;

        case '3': // Search left.
            for (Xtemp=curX-1; ((data_level[Xtemp][curY] < '0')
            || (data_level[Xtemp][curY] > '5')) && (Xtemp>-1); Xtemp--);
            Xtemp++;
            while(Xtemp != curX)
            {
                if (data_level[Xtemp][y] != ' ')
                    fire temp(Xtemp, y);
                Xtemp++;
            }
            moves++;
            check_end();
            if (data_level[x][y] == '3') break;

        case '4': // Search down
            for (Xtemp=curX+1; ((data_level[Xtemp][curY] < '0')
            || (data_level[Xtemp][curY] > '5')) && (Xtemp<A); Xtemp++);
            Xtemp--;
            while(Xtemp != curX)
            {
                if (data_level[Xtemp][y] != ' ')
                    fire tempfire(Xtemp, y);
                Xtemp--;
            }
            moves++;
            check_end();
            break;
    }
}
//////////////////////////////////////////////////////////////////////////


