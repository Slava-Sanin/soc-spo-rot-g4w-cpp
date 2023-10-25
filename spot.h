BOOL CALLBACK TableDlgProc(HWND, UINT, WPARAM, LPARAM);

struct PLACE // For finding the best place to put spot.
{
    int x;
    int y;
    int num;
    PLACE* next;
};

struct st_Player // For Player and Computer data.
{
    int spots;
    int color;
    int is; //first or second moving
};

extern st_Player PlayerDlg, ComputerDlg;

class cl_spot
{
    public:
    HWND hwnd1, but_Table, hwndLV;
    HDC hdc1, memdc2;
    HBITMAP hmap, hBit, bkgrnd;
    HGDIOBJ hBrush;

    int level;
    char data_level[Asp][Bsp], data_lev_gr[Asp][Bsp], data_undo[Asp][Bsp];
    time_t starttime, curtime;
    double htime;
    int moves;
    char background[255];
    char filename[255];
    int curX, curY;
    st_Player Player, Computer;
    int who_is_now;
    int table_was_changed;
    int error;

    public:
    cl_spot();
    ~cl_spot();
    void call_TableDlg();
    void draw_2_spots();
    int init();
    void NewGame();
    void Undo();
    void member_last_move();
    void bild_ground();
    void change_level();
    int SaveGame(char *);
    int LoadGame(char *);
    void movetop(char);
    void create_hBit();
    void redraw();
    void fast_redraw();
    void putthis(int, int, int, char);
    double retime();
    int check_end();
    void My_Scrolling(WPARAM, LPARAM);
    void change_background(char *);
    void player_move(int, int);
    BOOL player_cant_move();
    void computer_move();
    void draw_computer_moving(int, int, PLACE);
    PLACE check_around(int, int);
    int calculate_Players_spots_to_draw(int, int);
    BOOL check_the_place(int, int, int, int);
    void fill_around(int, int, int);
    void check_spots_number();
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Constructor bilds a window, background and fills a map of level.
//////////////////////////////////////////////////////////////////////////
cl_spot::cl_spot()
{
    hwnd1=CreateWindow("STATIC", NULL, WS_CHILD | WS_DLGFRAME,
    W1-4, W2-4, W3+35, W4+6, hTabWnd, NULL, hInst, NULL);

    strcpy(background, "");
    change_background(path2);
    hmap=LoadBitmap(hInst, "SPOTMAP"); // Loads all sprites.

    create_hBit();  // Creating board im memory(bitmap).

    table_was_changed=0; // Background changed "table"/pictures.
    level=1;

    Player.color=2;      // Color of player by default.

    Computer.color=3;    // Color of computer by default.

    change_level();      // Create filename by level number.
    if (init() == -1) return; // If problem whith file loading.
    call_TableDlg();     // Call and show addited options for game Spot.
}
/////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Destructor destroys created objects.
//////////////////////////////////////////////////////////////////////////
cl_spot::~cl_spot()
{
    if (memdc2) DeleteDC(memdc2);
    if (hBrush) DeleteObject(hBrush);
    if (hBit) DeleteObject(hBit);
    if (hmap) DeleteObject(hmap);
    if (bkgrnd) DeleteObject(bkgrnd);
    if (but_Table) DestroyWindow(but_Table);
    DestroyWindow(hwnd1);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Adds expended options dialog box.
//////////////////////////////////////////////////////////////////////////
void cl_spot::call_TableDlg()
{
    but_Table=CreateDialog(hInst, "TableDlg", htoolswnd,
    (DLGPROC)TableDlgProc);
    draw_2_spots();
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Draws spots in dialog window.
//////////////////////////////////////////////////////////////////////////
void cl_spot::draw_2_spots()
{
    HDC memdc_dlg, hdc_dlg;

    UpdateWindow(but_Table);
    hdc_dlg=GetDC(but_Table);
    memdc_dlg=CreateCompatibleDC(hdc_dlg);
    DeleteObject(SelectObject(memdc_dlg, hmap));
    StretchBlt(hdc_dlg, 16, 14, 32, 32,
    memdc_dlg, table*40, 40*PlayerDlg.color, 40, 40, SRCCOPY);
    StretchBlt(hdc_dlg, 83, 14, 32, 32,
    memdc_dlg, table*40, 40*ComputerDlg.color, 40, 40, SRCCOPY);

    DeleteDC(hdc_dlg);
    DeleteDC(memdc_dlg);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Starts a game.
// Loads level, initializes timer, bilds background,
// member current position.
//////////////////////////////////////////////////////////////////////////
int cl_spot::init()
{
    int handle;
    error=0;
    if ((handle=open(filename, O_RDONLY | O_TEXT, S_IREAD)) == -1)
    {
        error=-1;
        MessageBox(hwnd, "Can't open file!", "ERROR!!!", MB_OK | MB_ICONERROR);
        return -1;
    }
    if (read(handle, &data_level, Asp*Bsp) == -1)
    {
        close(handle);
        error=-1;
        MessageBox(hwnd, "Can't read file!", "ERROR!!!", MB_OK | MB_ICONERROR);
        return -1;
    }
    close(handle);

    starttime = time(NULL); // Init. timer.
    moves=0;
    Player.is = PlayerDlg.is;        // First or second?
    Computer.is = ComputerDlg.is;    // First or second?
    check_spots_number();            // Init. spots number.
    InitStatus();                    // Init. status line.
    bild_ground();                   // Bild map of spots un board.
    member_last_move();              // Save last moving.
    redraw();                        // Draw all.
    return 0;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Starts current game from the begining.
//////////////////////////////////////////////////////////////////////////
void cl_spot::NewGame()
{
    if (moves)
    {
        PlayMySound("changepage.wav");
        init();
        if (ComputerDlg.is == 1)
        {
            Sleep(700);
            computer_move();
            check_spots_number();
            InitStatus();
        }
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Do moving back.
//////////////////////////////////////////////////////////////////////////
void cl_spot::Undo()
{
    if (GetMenuState(GetMenu(hwnd), IDM_Undo, MF_BYCOMMAND) == MF_GRAYED)
    return;
    for(int x=0; x<Asp; x++)
    {
        for(int y=0; y<Bsp; y++)
        {
            data_level[x][y] = data_undo[x][y];
            if (data_level[x][y] == '2') {curX=x; curY=y;}
            putthis(2, x, y, data_level[x][y]);
        }
    }
    moves--;
    InvalidateRect(hwnd, NULL, TRUE);
    check_spots_number();
    InitStatus();
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Save last moving.
//////////////////////////////////////////////////////////////////////////
void cl_spot::member_last_move()
{
    for(int x=0; x<Asp; x++)
        for(int y=0; y<Bsp; y++)
            data_undo[x][y] = data_level[x][y];
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Draws a board in the memory.
//////////////////////////////////////////////////////////////////////////
void cl_spot::bild_ground()
{
    // Bilds the ground array
    for(int x=0; x<Asp; x++)
        for(int y=0; y<Bsp; y++)
        {
            switch (data_level[x][y])
            {
                case ' ':
                case '1': data_lev_gr[x][y] = data_level[x][y]; break;
                case '2':
                case '3': curX=x; curY=y;
            }
            putthis(2, x, y, data_level[x][y]);
        }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Changes level.
//////////////////////////////////////////////////////////////////////////
void cl_spot::change_level()
{
    char str[8];
    strcpy(filename, CurPath);
    strcat(filename, "\\spot\\lev");
    sprintf(str, "%d.spo", level);
    strcat(filename, str);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Saves current game un the disk.
//////////////////////////////////////////////////////////////////////////
int cl_spot::SaveGame(char *spofilename)
{
    int handle;
    int length = Asp*Bsp;
    char filename[255];

    if (Save_as_Flag)
        strcpy(filename, spofilename);
    else
        sprintf(filename, "%s\\%s\\%s", CurPath, "Save", spofilename);

    if ((handle = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
    S_IREAD | S_IWRITE)) == -1)
    {
        MessageBox(hwnd, "Can't create file!","ERROR!!!", MB_OK | MB_ICONERROR);
        return -1;
    }

    if (write(handle, &data_level, Asp*Bsp) != length)
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
int cl_spot::LoadGame(char *spofilename)
{
    strcpy(filename, spofilename);
    return init();
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Moves the spots un the board by mouse pressing.
//////////////////////////////////////////////////////////////////////////
void cl_spot::movetop(char key)
{
    switch (key)
    {
        case 75: // Left.
            if (data_level[curX][curY-1]==' ' || data_level[curX][curY-1]=='3')
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move1.wav");
                putthis(1, curX, curY-1, '2');
                putthis(1, curX, curY, data_lev_gr[curX][curY]);
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
                    putthis(1, curX, curY-2, '4');
                else
                    putthis(1, curX, curY-2, '5');
                putthis(1, curX, curY-1, '2');
                putthis(1, curX, curY, data_lev_gr[curX][curY]);
                curY--;
                moves++;
                check_end();
                break;
            }
            break;
        case 77: // Right.
            if (data_level[curX][curY+1]==' ' || data_level[curX][curY+1]=='3')
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move1.wav");
                putthis(1, curX, curY+1, '2');
                putthis(1, curX, curY, data_lev_gr[curX][curY]);
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
                    putthis(1, curX, curY+2, '4');
                else
                    putthis(1, curX, curY+2, '5');
                putthis(1, curX, curY+1, '2');
                putthis(1, curX, curY, data_lev_gr[curX][curY]);
                curY++;
                moves++;
                check_end();
                break;
            }
            break;
        case 72: // Up.
            if (data_level[curX-1][curY]==' ' || data_level[curX-1][curY]=='3')
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move1.wav");
                putthis(1, curX-1, curY, '2');
                putthis(1, curX, curY, data_lev_gr[curX][curY]);
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
                    putthis(1, curX-2, curY, '4');
                else
                    putthis(1, curX-2, curY, '5');
                putthis(1, curX-1, curY, '2');
                putthis(1, curX, curY, data_lev_gr[curX][curY]);
                curX--;
                moves++;
                check_end();
                break;
            }
            break;
        case 80: // Down.
            if (data_level[curX+1][curY]==' ' || data_level[curX+1][curY]=='3')
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);
                PlayMySound("move1.wav");
                putthis(1, curX+1, curY, '2');
                putthis(1, curX, curY, data_lev_gr[curX][curY]);
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
                    putthis(1, curX+2, curY, '4');
                else
                    putthis(1, curX+2, curY, '5');
                putthis(1, curX+1, curY, '2');
                putthis(1, curX, curY, data_lev_gr[curX][curY]);
                curX++;
                moves++;
                check_end();
                break;
            }
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Create bitmap(board) in memory.
//////////////////////////////////////////////////////////////////////////
void cl_spot::create_hBit()
{
    HBRUSH hOldBrush;
    HDC memdc1;

    hdc1=GetDC(hwnd1);
    memdc1=CreateCompatibleDC(hdc1);
    SelectObject(memdc1, bkgrnd);

    memdc2=CreateCompatibleDC(hdc1);
    hBit=CreateCompatibleBitmap(hdc1, W3+28, W4);
    SelectObject(memdc2, hBit);

    hBrush=GetStockObject(BLACK_BRUSH);

    hOldBrush=(HBRUSH)SelectObject(memdc2, hBrush);
    PatBlt(memdc2, 0, 0, W3+28, W4, PATCOPY);
    ReleaseDC(hwnd1, hdc1);
    DeleteDC(memdc1);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Refreshing of board by drawing current position of the game.
//////////////////////////////////////////////////////////////////////////
void cl_spot::redraw()
{
    HDC memdc1;
    HBRUSH hOldBrush;
    HPEN hPen;
    HPEN hOldPen;

    hdc1=GetDC(hwnd1);
    memdc1=CreateCompatibleDC(hdc1);
    SelectObject(memdc1, bkgrnd);

    //-------Borders-------
    hOldBrush=(HBRUSH)SelectObject(memdc2, GetStockObject(HOLLOW_BRUSH));

    hPen=CreatePen(PS_SOLID, 1, RGB(0,0,0));
    hOldPen=(HPEN)SelectObject(memdc2, hPen);
    //-------1------
    BitBlt(memdc2, 0, 0, W3+28, W4, memdc1, 0, 0, SRCCOPY);
    //-------2------
    Rectangle(memdc2, 0, 0, W3+28, W4);
    Rectangle(memdc2, 3, 3, W3-3+28, W4-3);
    Rectangle(memdc2, 6, 6, W3-6+28, W4-6);
    for(int i=1; i<6; i++)
    {
        if (i==3) continue;
        hPen=CreatePen(PS_SOLID, 1, RGB((i+1)*40, 255, (i+1)*40));
        DeleteObject(SelectObject(memdc2, hPen));
        Rectangle(memdc2, i, i, W3-i+28, W4-i);
    }
    DeleteObject(SelectObject(memdc2, hOldPen));
    SelectObject(memdc2, hOldBrush);
    //--------3---------
    for(int x=0; x<Asp; x++)
        for(int y=0; y<Bsp; y++)
            putthis(2, x, y, data_level[x][y]);

    //---copy to display----
    BitBlt(hdc1, 0, 0, W3+28, W4, memdc2, 0, 0, SRCCOPY);
    //----------------------
    ReleaseDC(hwnd1, hdc1);
    DeleteDC(memdc1);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Quick refreshing of board by drawing current position of the game.
//////////////////////////////////////////////////////////////////////////
void cl_spot::fast_redraw()
{
    HDC memdc1;
    hdc1=GetDC(hwnd1);
    memdc1=CreateCompatibleDC(hdc1);
    SelectObject(memdc1, bkgrnd);
    //----------------------
    BitBlt(hdc1, 0, 0, W3+28, W4, memdc2, 0, 0, SRCCOPY);
    //----------------------
    ReleaseDC(hwnd1, hdc1);
    DeleteDC(memdc1);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Draws needed sprite in window.
//////////////////////////////////////////////////////////////////////////
void cl_spot::putthis(int where, int x, int y, char kode)
{
    int kode_x, kode_y;
    HDC memdc1, memdctemp, where_dc;

    hdc1=GetDC(hwnd1);
    memdctemp=CreateCompatibleDC(hdc1);
    memdc1=CreateCompatibleDC(hdc1);
    SelectObject(memdc1, bkgrnd);

    if (where==1) where_dc = hdc1;
    else where_dc = memdc2;

    data_level[x][y] = kode;

    switch (kode)
    {
        case '0': // Stown(border).
            SelectObject(memdctemp, hmap);
            BitBlt(where_dc, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
            memdctemp, 0, 0, SRCCOPY);
            break;
        case ' ': // Empty place.
            if (table==2)
            {
                BitBlt(where_dc, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
                memdc1, otstup_sp+y*40, 9+otstup_sp+x*40, SRCCOPY);

                SelectObject(memdctemp, hmap);
                BitBlt(where_dc, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
                memdctemp, table*40, 40, SRCAND);
                BitBlt(where_dc, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
                memdctemp, table*40, 0, SRCINVERT);
            }
            else
            {
                SelectObject(memdctemp, hmap);
                BitBlt(where_dc, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
                memdctemp, table*40, 0, SRCCOPY);
            }
            break;
        default:  // Player's or computer's spot.
            kode-=48;
            if (kode==2) kode = PlayerDlg.color;
            else kode = ComputerDlg.color;

            BitBlt(where_dc, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
            memdc1, otstup_sp+y*40, 9+otstup_sp+x*40, SRCCOPY);

            SelectObject(memdctemp, hmap);
            BitBlt(where_dc, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
            memdctemp, table*40, 7*40, SRCAND);
            BitBlt(where_dc, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
            memdctemp, table*40, kode*40, SRCINVERT);
            break;
    }

    ReleaseDC(hwnd1, hdc1);
    DeleteDC(memdctemp);
    DeleteDC(memdc1);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Time of current game.
//////////////////////////////////////////////////////////////////////////
double cl_spot::retime()
{
    curtime=time(NULL);
    htime=difftime(curtime, starttime);
    return htime;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Checks if the game is complete.
//////////////////////////////////////////////////////////////////////////
int cl_spot::check_end()
{
    char result[100];
    if (Player.spots!=0 && Computer.spots!=0)
    {
        for(int x=1; x<(Asp-1); x++)
            for(int y=1; y<(Bsp-1); y++)
            {
                if (data_level[x][y] == ' ') return 0;
            }
    }

    // Checking for a winner.
    if (Player.spots == Computer.spots) strcpy(result,"Teko!!!");
    else if (Player.spots < Computer.spots) strcpy(result,"Computer won!!!");
         else if (Player.spots > Computer.spots)
                  {
                      PlayMySound("winer1.wav");
                      strcpy(result,"You are winner!!!");
                  }

    if (MessageBox(hwnd, result, "Party complete.",
    MB_OK | MB_ICONINFORMATION) == IDYES)
    {
        if (level==20)
            MessageBox(hwnd, "No more levels!", "Level complete.",
            MB_OK | MB_ICONSTOP);
        else
        {
            SetScrollPos(hScroll, SB_CTL, level, 1);
            UpdateWindow(hScroll);
        }
    }
    NewGame(); // Start new party.
    return 1;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Changing level by scrolling bar.
//////////////////////////////////////////////////////////////////////////
void cl_spot::My_Scrolling(WPARAM wParam, LPARAM lParam)
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
    init();
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Change current background.
//////////////////////////////////////////////////////////////////////////
void cl_spot::change_background(char *str)
{
    makeBackGround(hwnd1, this, str);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Player moves and computer begins to think.
//////////////////////////////////////////////////////////////////////////
void cl_spot::player_move(int x, int y)
{
    int kode_x, kode_y;
    static BOOL first_step=TRUE;
    static first_X, first_Y;
    HDC memdctemp;

    if (first_step || table_was_changed)
    {
        first_X=x;
        first_Y=y;
        if (data_level[x][y]-48 != Player.color) return;
        PlayMySound("poper.wav");

        hdc1=GetDC(hwnd1);
        memdctemp=CreateCompatibleDC(hdc1);
        SelectObject(memdctemp, hmap);
        BitBlt(hdc1, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
        memdctemp, 0, 7*40, SRCAND);
        BitBlt(hdc1, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
        memdctemp, 0, PlayerDlg.color*40, SRCINVERT);
        //-------gibuy for fast_draw-------
        BitBlt(memdc2, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
        hdc1, otstup_sp+y*40, 9+otstup_sp+x*40, SRCCOPY);
        //---------------------------------
        ReleaseDC(hwnd, hdc1);
        DeleteDC(memdctemp);

        first_step=FALSE;
        table_was_changed=0;
        return;
    }
    else
    {
        if ((x==first_X) && (y==first_Y)) // Same place. Drop down the spot.
        {
            putthis(1, x, y, 48+Player.color);
            //-------gibuy for fast_draw-------
            putthis(2, x, y, 48+Player.color);
            //---------------------------------
            first_step=TRUE;
            return;
        }
        else
        {
            if (check_the_place(x, y, first_X, first_Y)) // If place is cleared.
            {
                member_last_move();
                EnableMenuItem(GetMenu(hwnd), IDM_Undo, MF_ENABLED);

                if (abs(first_X-x)==2 || abs(first_Y-y)==2) // If spot jumps.
                {
                    putthis(1, first_X, first_Y, ' ');
                    //-------gibuy for fast_draw-------
                    putthis(2, first_X, first_Y, ' ');
                    //---------------------------------
                }
                else // Draw new spot.
                {
                    putthis(1, first_X, first_Y, 48+Player.color);
                    //-------gibuy for fast_draw-------
                    putthis(2, first_X, first_Y, 48+Player.color);
                    //---------------------------------
                }

                putthis(1, x, y, 48+Player.color);
                //-------gibuy for fast_draw-------
                putthis(2, x, y, 48+Player.color);
                //---------------------------------
                fill_around(x, y, Computer.color); // Paint around all enemy.
                first_step=TRUE;
                check_spots_number();
                InitStatus();
                check_end();
                Sleep(1000);
                //----computer is begining now----
                do
                {
                    computer_move();
                    moves++;
                    check_spots_number();
                    InitStatus();
                    if (check_end()) return;
                }
                while(player_cant_move());
                return;
            }

            putthis(1, first_X, first_Y, 48+Player.color);
            //-------gibuy for fast_draw-------
            putthis(2, first_X, first_Y, 48+Player.color);
            //---------------------------------
            first_step=TRUE;
        }
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Check if Player can to move.
// If not - computer is moving again.
//////////////////////////////////////////////////////////////////////////
BOOL cl_spot::player_cant_move()
{
    int x, y;
    for (x=1; x<(Asp-1); x++)
        for (y=1; y<(Bsp-1); y++)
        {
            if (data_level[x][y] == 48+Player.color)
            {
                int i, j;
                for (i=x-2; i<=(x+2); i++)
                    for (j=y-2; j<=(y+2); j++)
                    {
                        if (i<1 || i>=(Asp-1) || j<1 || j>=(Bsp-1))
                            continue;
                        if ((x==i) && (y==j)) continue;
                        if (data_level[i][j] == ' ') return FALSE;
                    }
            }
        }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Scan spots of Computer in all matrix and find the best place to move.
//////////////////////////////////////////////////////////////////////////
void cl_spot::computer_move()
{
    int X_from, Y_from;
    PLACE best={0,0,-1,NULL};
    PLACE choyse={0,0,-1,NULL};
    int i,j;
    for (i=1; i<(Asp-1); i++)
        for (j=1; j<(Bsp-1); j++)
        {
            if (data_level[i][j] == 48+Computer.color)
            {
                choyse=check_around(i, j);
                if (best.num < choyse.num)
                {
                    best=choyse;
                    X_from=i;
                    Y_from=j;
                }
            }
        }

    if (best.num != -1) // If found place.
        draw_computer_moving(X_from, Y_from, best); // Computer moves.
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Draws the computer's moving.
//////////////////////////////////////////////////////////////////////////
void cl_spot::draw_computer_moving(int x, int y, PLACE best)
{
    HDC memdctemp;

    PlayMySound("poper.wav");

    hdc1=GetDC(hwnd1);
    memdctemp=CreateCompatibleDC(hdc1);
    SelectObject(memdctemp, hmap);
    BitBlt(hdc1, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
    memdctemp, 0, 7*40, SRCAND);
    BitBlt(hdc1, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
    memdctemp, 0, ComputerDlg.color*40, SRCINVERT);
    //-------gibuy for fast_draw-------
    BitBlt(memdc2, otstup_sp+y*40, 9+otstup_sp+x*40, 40, 40,
    hdc1, otstup_sp+y*40, 9+otstup_sp+x*40, SRCCOPY);
    //---------------------------------
    ReleaseDC(hwnd, hdc1);
    DeleteDC(memdctemp);

    //-----------------------
    Sleep(1000);
    //-----------------------

    if (abs(best.x-x) == 2 || abs(best.y-y) == 2)
    {
        putthis(1, x, y, ' ');
        //-------gibuy for fast_draw-------
        putthis(2, x, y, ' ');
    }
    else
    {
        putthis(1, x, y, 48+Computer.color);
        //-------gibuy for fast_draw-------
        putthis(2, x, y, 48+Computer.color);
        //---------------------------------
    }
    //-----------------------
    Sleep(500);
    //-----------------------

    putthis(1, best.x, best.y, 48+Computer.color);
    //-------gibuy for fast_draw-------
    putthis(2, best.x, best.y, 48+Computer.color);
    //-----------------------
    Sleep(500);
    //-----------------------

    fill_around(best.x, best.y, Player.color);
    PlayMySound("move1.wav");
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Find destination places. (Recursia)
//////////////////////////////////////////////////////////////////////////
PLACE cl_spot::check_around(int x, int y)
{
    int i, j, bonus=0, num=0;
    int scolko_odinakovux=0;
    static int kakoe_iz_odinakovux_vubrat=0;
    static BOOL first_time=TRUE;
    static PLACE best={0,0,-1,NULL};

    if (first_time) best.num=-1;

    for (i=x-2; i<=x+2; i++)
        for (j=y-2; j<=y+2; j++)
        {
            if (i<1 || i>=(Asp-1) || j<1 || j>=(Bsp-1)) continue;
            if ((x==i) && (y==j)) continue;
            if ((abs(x-i) <= 1) && (abs(y-j) <= 1)) bonus=1;
            else bonus=0;

            if (data_level[i][j] == ' ')
            {
                num=calculate_Players_spots_to_draw(i, j);
                num+=bonus;
                if (num >= best.num)
                {
                    if (first_time)
                    {
                        if (num > best.num) scolko_odinakovux=1;
                        else scolko_odinakovux++;
                        best.num=num;
                    }
                    else
                    {
                        if (num == best.num)
                        {
                            scolko_odinakovux++;
                            if (scolko_odinakovux == kakoe_iz_odinakovux_vubrat)
                            {
                                best.x=i;
                                best.y=j;
                                best.num=num;
                                best.next=NULL;
                                first_time=TRUE;
                                return best;
                            }
                        }
                    }
                }
            }
        }
    if (best.num != -1)
    {
        randomize();
        kakoe_iz_odinakovux_vubrat=1 + (rand() % scolko_odinakovux);
        first_time=FALSE;
        check_around(x, y);
    }
    first_time=TRUE;
    return best;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Calculate number of Player's spots to draw.
//////////////////////////////////////////////////////////////////////////
int cl_spot::calculate_Players_spots_to_draw(int x, int y)
{
    int i, j, num=0;
    for (i=x-1; i<=x+1; i++)
        for (j=y-1; j<=y+1; j++)
        {
            if (i<1 || i>=(Asp-1) || j<1 || j>=(Bsp-1)) continue;
            if ((x==i) && (y==j)) continue;
            if (data_level[i][j] == 48+Player.color) num++;
        }
    return num;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Check if able to put here spot.
//////////////////////////////////////////////////////////////////////////
BOOL cl_spot::check_the_place(int x, int y, int first_X, int first_Y)
{
    if (data_level[x][y] != ' ') return FALSE;
    if (abs(first_X-x)>2 || abs(first_Y-y)>2) return FALSE;
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Paint all the enemy's spots.
//////////////////////////////////////////////////////////////////////////
void cl_spot::fill_around(int x, int y, int color)
{
    int i, j;
    for (i=x-1; i<=x+1; i++)
        for (j=y-1; j<=y+1; j++)
            if (data_level[i][j]-48 == color)
            {
                Sleep(300);
                putthis(1, i, j, data_level[x][y]);
                //-------gibuy for fast_draw-------
                putthis(2, i, j, data_level[x][y]);
                //---------------------------------
            }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Calculate number of spots for Player and Computer.
//////////////////////////////////////////////////////////////////////////
void cl_spot::check_spots_number()
{
    Player.spots=0;
    Computer.spots=0;
    for(int x=1; x<Asp; x++)
        for(int y=1; y<Bsp; y++)
        {
            if (data_level[x][y]-48 == Player.color) Player.spots++;
            if (data_level[x][y]-48 == Computer.color) Computer.spots++;
        }
}
//////////////////////////////////////////////////////////////////////////


extern cl_spot* p2;

//////////////////////////////////////////////////////////////////////////
// Change table Dialog.
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK TableDlgProc(HWND hdwnd, UINT message,
WPARAM wParam, LPARAM lParam)
{
    static HWND hwndTT;    // handle of tooltip
    static TOOLINFO ti;    // tool information
    static char *szTips="Table changer";

    switch(message)
    {
        case WM_INITDIALOG :
            hwndTT = CreateWindow(TOOLTIPS_CLASS, (LPSTR) NULL, TTS_ALWAYSTIP,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, (HMENU) NULL, hInst, NULL);

            if (hwndTT == (HWND) NULL) return 0;

            ti.cbSize = sizeof(TOOLINFO);
            ti.uFlags = TTF_IDISHWND | TTF_CENTERTIP | TTF_SUBCLASS;
            ti.hwnd = hdwnd;
            ti.hinst = 0;//hInst;
            ti.uId = (UINT) hdwnd;
            ti.lpszText = (LPSTR) szTips;
            SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);

            ti.uId = (UINT)GetDlgItem(hdwnd, IDC_BUTTON1);
            SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
            return 1;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_BUTTON1:
                    if (HIWORD(wParam) == BN_CLICKED)
                        SendMessage(hwnd, WM_COMMAND, IDM_Table, 0);
                    return 1;
            }
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////

