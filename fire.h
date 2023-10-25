typedef struct
{
    int x;
    int y;
} rotm;

class fire   // Firing rotms class.
{
    public:
    rotm mat[4];
    int count;
    double points;

    public:
    fire(int, int);
    ~fire();
    void init();
    int find(int, int);
    void check_around(int, int);
};


//////////////////////////////////////////////////////////////////////////
// Searchs the board, finds rotms to fire and fires them, adds points if need.
//////////////////////////////////////////////////////////////////////////
fire::fire(int x, int y)
{
    init();
    check_around(x, y); // Find more rotms around this rotm.
    if (count>1)
    {
        if (glob_sound)
            PlaySound("sound\\fire.wav", NULL, SND_FILENAME | SND_ASYNC);
        points=count*count*10; // Points of player.
        p3->score += points;
        while(count) // Remove rotms from board.
        {
            p3->putthis(mat[count-1].x, mat[count-1].y, ' ');
            count--;
        }
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Initializes "matrix-data base" of rotms that must been fired.
//////////////////////////////////////////////////////////////////////////
void fire::init()
{
    points=0;
    count=0;
    for(int i=1; i<4; i++)
    {
        mat[i].x = -1;
        mat[i].y = -1;
    }
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Checks if this rotm already was founded and now in "matrix-data base".
//////////////////////////////////////////////////////////////////////////
int fire::find(int x, int y)
{
    for(int i=0; i<count; i++)
        if ((mat[i].x == x) && (mat[i].y == y)) return 1;
    return 0;
}
//////////////////////////////////////////////////////////////////////////


fire::~fire(){} // Destructor.


//////////////////////////////////////////////////////////////////////////
// Recursia for searching rotms around rotm.
//////////////////////////////////////////////////////////////////////////
void fire::check_around(int x, int y)
{
    if (!find(x, y)) // If yet not in "matrix-data base".
    {
        mat[count].x = x; //  Put coordinates of rotm in to
        mat[count].y = y; //  "matrix-data base".
        count++; // Counts rotms that is found and must been fired.
        if (x-1 > -1) // Check place from left of rotm.
        {
            if (p3->data_level[x-1][y] == p3->data_level[x][y])
                check_around(x-1, y);
        }

        if (y+1 < B)  // Check place from right of rotm.
        {
            if (p3->data_level[x][y+1] == p3->data_level[x][y])
                check_around(x, y+1);
        }

        if (x+1 < A)  // Check place from up of rotm.
        {
            if (p3->data_level[x+1][y] == p3->data_level[x][y])
                check_around(x+1, y);
        }

        if (y-1 > -1)  // Check place from down of rotm.
        {
            if (p3->data_level[x][y-1] == p3->data_level[x][y])
                check_around(x, y-1);
        }
    }
}
//////////////////////////////////////////////////////////////////////////

