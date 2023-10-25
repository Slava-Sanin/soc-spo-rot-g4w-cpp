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
							  "       044444440       ",
							  "       1  Y S  2       ",
							  "       1  GKR  2       ",
							  "       1G 000 R2       ",
							  "       1 SR   Y2       ",
							  "       1 K YG  2       ",
							  "       033333330       ",
							  "                       ",
							  "                       ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev0.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




