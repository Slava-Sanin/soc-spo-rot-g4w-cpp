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
							  "      00040004000      ",
							  "      1    B    2      ",
							  "      0 0 0K0B0 0      ",
							  "      0 0G0R0K0B0      ",
							  "      0 0R0G0 0K0      ",
							  "      0R0 0Y0G0 0      ",
							  "      0 0Y0 0 0 0      ",
							  "      1Y        2      ",
							  "      03000300030      ",
							  "                       ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev17.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




