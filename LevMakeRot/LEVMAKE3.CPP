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
                       "       044444440       ",
							  "       1       2       ",
							  "       1     Y 2       ",
							  "       1    KGY2       ",
							  "       1   KGY 2       ",
							  "       1  RGK  2       ",
							  "       1 BGR   2       ",
							  "       1BGR    2       ",
							  "       1 B     2       ",
							  "       1       2       ",
							  "       033333330       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev3.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




