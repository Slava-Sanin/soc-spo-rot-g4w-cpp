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
							  "     04    4    40     ",
							  "     1           2     ",
							  "                       ",
							  "          GYG          ",
							  "         R0R0R         ",
							  "     1    G5G    2     ",
							  "         R0R0R         ",
							  "          GYG          ",
							  "                       ",
							  "     1     Y     2     ",
							  "     03    3    30     "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev5.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




