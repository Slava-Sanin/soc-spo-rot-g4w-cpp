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
	char lev_data[][B]={"    05   55 55   50    ",
							  "    0      5      0    ",
							  "    0  5R   K  5  0    ",
							  "    0   5S K  5Y  0    ",
							  "    05  B        50    ",
							  "    0 5BR  5G   5 0    ",
							  "    05   RY  G   50    ",
							  "    0  G5S Y  5B  0    ",
							  "    0  5  S K  5  0    ",
							  "    0      5      0    ",
							  "    05   55 55   50    ",
                       "    000000000000000    "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev16.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




