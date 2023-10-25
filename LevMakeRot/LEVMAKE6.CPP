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
							  "      044440           ",
							  "      0 B  0           ",
							  "      1B   0           ",
							  "      1 B0 444440      ",
							  "      0331      2      ",
							  "         1  BY  2      ",
							  "         1 BRKY 2      ",
							  "         1 KYBR 2      ",
							  "         1  KR  2      ",
							  "         1      2      ",
							  "         03333330      "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev6.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




