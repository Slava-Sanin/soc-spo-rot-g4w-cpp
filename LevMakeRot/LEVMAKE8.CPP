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
							  "     0040404040040     ",
							  "     01  BGKY R  2     ",
							  "     00 0G0Y0S00 0     ",
							  "     01 GBYKR    2     ",
							  "     00 03030400 0     ",
							  "     00 0   0R00 0     ",
							  "     00 00000S00 0     ",
							  "     0           2     ",
							  "     0000000000000     ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev8.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




