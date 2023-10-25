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
							  "         40004         ",
							  "        1 R B 2        ",
							  "        1 G R 2        ",
							  "        1 B G 2        ",
							  "         30B03         ",
							  "          0G0          ",
							  "          0R0          ",
							  "          030          ",
							  "                       ",
                       "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev10.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




