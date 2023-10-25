class cl_socoban
{
    public:
    HWND hwnd1;
    HDC hdc1,memdc1;
    HBITMAP hmap,hBit1,bkgrnd;
    HGDIOBJ hBrush;
    
    int level;
    char data_level[A][B], data_lev_gr[A][B], data_undo[A][B];
    time_t starttime, curtime;
    double htime;
    int moves;
    char background[255];
    char filename[255];
    int curX, curY;
    int error;

    public:
    cl_socoban();
    ~cl_socoban();
    int init();
    void NewGame();
    void Undo();
    void member_last_move();
    void bild_ground();
    void change_level();
    int SaveGame(char *);
    int LoadGame(char *);
    void movetop(char);
    void redraw();
    void putthis(int, int, char);
    double retime();
    void check_end();
    void My_Scrolling(WPARAM, LPARAM);
    void change_background(char *);
};


//////////////////////////////////////////////////////////////////////////
// Constructor bilds a window, background and fills a map of level.
//////////////////////////////////////////////////////////////////////////
cl_socoban::cl_socoban()
{
    hwnd1=CreateWindow("STATIC", NULL, WS_CHILD | WS_DLGFRAME,
    W1-4, W2-4, W3+35, W4+6, hTabWnd, NULL, hInst, NULL);
    strcpy(background, "");
    change_background(path1);
    hmap=LoadBitmap(hInst, "SOCMAP");
    level=1;
    change_level();
    init();
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Destructor destroys created objects.
//////////////////////////////////////////////////////////////////////////
cl_socoban::~cl_socoban()
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
int cl_socoban::init()
{
    int handle;
    error=0;

    if ((handle=open(filename, O_RDONLY | O_TEXT, S_IREAD)) == -1)
    {
        error=1;
        MessageBox(hwnd, "Can't open file!", "ERROR!!!", MB_OK | MB_ICONERROR);
        return -1;
    }
    if (read(handle, &data_level,A*B) == -1)
    {
        close(handle);
        error=1;
        MessageBox(hwnd, "Can't read file!", "ERROR!!!", MB_OK | MB_ICONERROR);
        return -1;
    }
    close(handle);

    starttime=time(NULL);
    moves=0;
    bild_ground();
    member_last_move();
    return 0;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Starts current game from the begining.
//////////////////////////////////////////////////////////////////////////
void cl_socoban::NewGame()
{
    if (moves)
    {
        if (glob_sound) PlayMySound("changepage.wav");
        init();
        EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_GRAYED);
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Do moving back.
//////////////////////////////////////////////////////////////////////////
void cl_socoban::Undo()
{
    if (GetMenuState(GetMenu(hwnd), IDM_Undo, MF_BYCOMMAND) == MF_GRAYED)
    return;
    for(int x=0; x<A; x++)
    {
        for(int y=0; y<B; y++)
        {
            data_level[x][y] = data_undo[x][y];
            if (data_level[x][y] == '2') {curX=x; curY=y;}
            putthis(x, y, data_level[x][y]);
        }
    }
    moves--;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Save last moving.
//////////////////////////////////////////////////////////////////////////
void cl_socoban::member_last_move()
{
    for(int x=0; x<A; x++)
        for(int y=0; y<B; y++)
            data_undo[x][y] = data_level[x][y];
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Draws a map of level in the window.
//////////////////////////////////////////////////////////////////////////
void cl_socoban::bild_ground()
{
    // find cursor and bild the ground array
    for(int x=0; x<A; x++)
        for(int y=0; y<B; y++)
        {
            switch (data_level[x][y])
            {
                case '1':
                case '3': data_lev_gr[x][y] = data_level[x][y]; break;
                case '2': curX=x; curY=y;
                case '4':
                case ' ': data_lev_gr[x][y] = ' '; break;
                case '5': data_lev_gr[x][y] = '3';
            }
            putthis(x, y, data_level[x][y]);
        }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Changes level.
//////////////////////////////////////////////////////////////////////////
void cl_socoban::change_level()
{
    char str[8];
    strcpy(filename, CurPath);
    strcat(filename, "\\socoban\\lev");
    sprintf(str, "%d.soc", level);
    strcat(filename, str);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Saves current game un the disk.
//////////////////////////////////////////////////////////////////////////
int cl_socoban::SaveGame(char *socfilename)
{
    int handle;
    int length=A*B;
    char filename[255];

    if (Save_as_Flag) strcpy(filename, socfilename);
    else sprintf(filename, "%s\\%s\\%s", CurPath, "Save", socfilename);

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
int cl_socoban::LoadGame(char *socfilename)
{
    strcpy(filename, socfilename);
    return init();
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Moves the object un the display by key or mouse pressing.
//////////////////////////////////////////////////////////////////////////
void cl_socoban::movetop(char key)
{
    switch (key)
    {
        case 75: // Moving left.
            if (data_level[curX][curY-1]==' ' || data_level[curX][curY-1]=='3')
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move1.wav");
                putthis(curX, curY-1, '2');
                putthis(curX, curY, data_lev_gr[curX][curY]);
                curY--;
                moves++;
                check_end();
                break;
            }
            if ((data_level[curX][curY-1]=='4' || data_level[curX][curY-1]=='5')
            && (data_level[curX][curY-2]==' ' || data_level[curX][curY-2]=='3'))
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move_push.wav");
                if (data_level[curX][curY-2] == ' ')
                    putthis(curX, curY-2, '4');
                else
                    putthis(curX, curY-2, '5');
                putthis(curX, curY-1, '2');
                putthis(curX, curY, data_lev_gr[curX][curY]);
                curY--;
                moves++;
                check_end();
                break;
            }
            break;
        case 77: // Moving right.
            if (data_level[curX][curY+1]==' ' || data_level[curX][curY+1]=='3')
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move1.wav");
                putthis(curX, curY+1, '2');
                putthis(curX, curY, data_lev_gr[curX][curY]);
                curY++;
                moves++;
                check_end();
                break;
            }
            if ((data_level[curX][curY+1]=='4' || data_level[curX][curY+1]=='5')
            && (data_level[curX][curY+2]==' ' || data_level[curX][curY+2]=='3'))
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move_push.wav");
                if (data_level[curX][curY+2] == ' ')
                    putthis(curX, curY+2, '4');
                else
                    putthis(curX, curY+2, '5');
                putthis(curX, curY+1, '2');
                putthis(curX, curY, data_lev_gr[curX][curY]);
                curY++;
                moves++;
                check_end();
                break;
            }
            break;
        case 72: // Moving up.
            if (data_level[curX-1][curY]==' ' || data_level[curX-1][curY]=='3')
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move1.wav");
                putthis(curX-1, curY, '2');
                putthis(curX, curY, data_lev_gr[curX][curY]);
                curX--;
                moves++;
                check_end();
                break;
            }
            if ((data_level[curX-1][curY]=='4' || data_level[curX-1][curY]=='5')
            && (data_level[curX-2][curY]==' ' || data_level[curX-2][curY]=='3'))
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move_push.wav");
                if (data_level[curX-2][curY] == ' ')
                    putthis(curX-2, curY, '4');
                else
                    putthis(curX-2, curY, '5');
                putthis(curX-1, curY, '2');
                putthis(curX, curY, data_lev_gr[curX][curY]);
                curX--;
                moves++;
                check_end();
                break;
            }
            break;
        case 80: // Moving down.
            if (data_level[curX+1][curY]==' ' || data_level[curX+1][curY]=='3')
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move1.wav");
                putthis(curX+1, curY, '2');
                putthis(curX, curY, data_lev_gr[curX][curY]);
                curX++;
                moves++;
                check_end();
                break;
            }
            if ((data_level[curX+1][curY]=='4' || data_level[curX+1][curY]=='5')
            && (data_level[curX+2][curY]==' ' || data_level[curX+2][curY]=='3'))
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move_push.wav");
                if (data_level[curX+2][curY] == ' ')
                    putthis(curX+2, curY, '4');
                else
                    putthis(curX+2, curY, '5');
                putthis(curX+1, curY, '2');
                putthis(curX, curY, data_lev_gr[curX][curY]);
                curX++;
                moves++;
                check_end();
                break;
            }
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Refreshing of board by drawing current position of the game.
//////////////////////////////////////////////////////////////////////////
void cl_socoban::redraw()
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
    Rectangle(memdctemp,6, 6, W3-6, W4-6);
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
void cl_socoban::putthis(int x, int y, char kode)
{
    int kode_x, kode_y;
    HDC memdctemp;
    data_level[x][y] = kode;

    hdc1=GetDC(hwnd1);
    memdctemp=CreateCompatibleDC(hdc1);

    if (kode == ' ') // Draws empty place.
    {
        SelectObject(memdctemp, bkgrnd);
        BitBlt(hdc1, otstup+y*25, otstup+x*25, 25, 25, memdctemp,
        otstup+y*25, otstup+x*25, SRCCOPY);
    }
    else // Draws other objects of level map.
    {
        kode = kode-48;
        kode_x = kode*25;
        kode_y=0;
        SelectObject(memdctemp, bkgrnd);
        BitBlt(hdc1, otstup+y*25, otstup+x*25, 25, 25, memdctemp,
        otstup + y*25, otstup + x*25, SRCCOPY);

        SelectObject(memdctemp, hmap);
        BitBlt(hdc1, otstup+y*25, otstup + x*25, 25, 25, memdctemp,
        kode_x, kode_y+25, SRCAND);
        BitBlt(hdc1, otstup + y*25, otstup + x*25, 25, 25, memdctemp,
        kode_x, kode_y, SRCINVERT);
    }

    DeleteDC(memdctemp);
    ReleaseDC(hwnd1, hdc1);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Time of current game.
//////////////////////////////////////////////////////////////////////////
double cl_socoban::retime()
{
    curtime=time(NULL);
    htime=difftime(curtime, starttime);
    return htime;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Checks if the game is complete.
//////////////////////////////////////////////////////////////////////////
void cl_socoban::check_end()
{
    for(int x=0; x<A; x++)
    {
        for(int y=0; y<B; y++)
        {
            if (data_lev_gr[x][y]=='3' && data_level[x][y]!='5')
                return;
        }
    }
    PlayMySound("winer1.wav");
    if (MessageBox(hwnd, "   Next level?", "Level complete.",
    MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        if (level==20) // If this is a last level.
        MessageBox(hwnd,"No more levels!","Level complete.",MB_OK|MB_ICONSTOP);
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
void cl_socoban::My_Scrolling(WPARAM wParam, LPARAM lParam)
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
    if (prevlevel==level) return;
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
void cl_socoban::change_background(char *str)
{
    makeBackGround(hwnd1, this, str);
}
//////////////////////////////////////////////////////////////////////////

