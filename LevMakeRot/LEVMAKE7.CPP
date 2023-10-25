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
							  "       1 Y     2       ",
							  "       1  GSY  2       ",
							  "       1 YSG0R 2       ",
							  "       1 0GSK0 2       ",
							  "       1 KSGR  2       ",
							  "       1  GSKR 2       ",
							  "       033333330       ",
							  "                       ",
                       "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev7.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




