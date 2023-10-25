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
							  "         40404         ",
							  "        0     0        ",
							  "       1 GR KB 2       ",
							  "       0 R5K5K 0       ",
							  "       1  RGY  2       ",
							  "       0 B5B5Y 0       ",
							  "       1  B YG 2       ",
							  "        0     0        ",
							  "         30303         ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev9.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




