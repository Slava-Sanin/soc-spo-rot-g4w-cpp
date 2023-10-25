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
                       "      0444444440       ",
							  "     05        2       ",
							  "     1         540     ",
							  "     1      0B   2     ",
							  "     1  RYB0Y0B  2     ",
							  "     1    Y RY0  2     ",
							  "     1     G 0   2     ",
							  "     0335   G    2     ",
							  "        1   BG   2     ",
							  "        1       50     ",
							  "        033333330      "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev4.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




