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
							  "      04000400040      ",
							  "      1  B  S   2      ",
							  "      0 004S400 0      ",
							  "      0K01GY 20R0      ",
							  "      1 KB 5YBR 2      ",
							  "      0 01GY 20 0      ",
							  "      0 003G300 0      ",
							  "      1  R K  S 2      ",
							  "      03000300030      ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev11.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




