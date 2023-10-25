#include <stdio.h>
#include <io.h>
#include <iostream.h>
#include <conio.h>
#define A 12
#define B 23
// ' ' - pustota
// 0 - stena
// 1 - ->
// 2 - <-
// 3 - ^
// 4 - v
// 5 - <,>,^,v
// B - Blue rotm
// G - Green rotm
// K - Korich rotm
// R - Red rotm
// S - Seren rotm
// Y - Yellow rotm
main()
{
	char lev_data[][B]={"                       ",
							  "                       ",
							  "     040040040040      ",
							  "     1GSG    YSK2      ",
							  "     0B00 00 00R0      ",
							  "     0R00 00 00S00     ",
							  "     1SY       GB2     ",
							  "     03K00 00 00R0     ",
							  "      0R00 00 00Y0     ",
							  "      1BK    GSRG2     ",
							  "      030030030030     ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev19.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




