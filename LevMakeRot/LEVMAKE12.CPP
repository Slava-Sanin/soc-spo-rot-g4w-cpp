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
							  "           040         ",
							  "       040 0G0 040     ",
							  "       0G0 0Y0 0R0     ",
							  "     000 000R000B0     ",
							  "     1KY         2     ",
							  "     000B000G000 0     ",
							  "       0R0 0B0 0K0     ",
							  "       0K0 030 0Y0     ",
							  "       030     030     ",
							  "                       ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev12.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




