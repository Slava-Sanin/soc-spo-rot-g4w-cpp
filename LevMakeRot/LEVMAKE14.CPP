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
							  "      044440           ",
							  "      1K GY2           ",
							  "      1    50          ",
							  "      1YR KR5440       ",
							  "      0335G  R 2       ",
							  "         05 Y G2       ",
							  "          1 K  2       ",
							  "          033330       ",
							  "                       ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev14.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




