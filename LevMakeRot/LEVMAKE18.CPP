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
							  "       040000040       ",
							  "      00B0 0 0R00      ",
							  "      00R04040Y00      ",
							  "      01G  5 RS20      ",
							  "      0051   2500      ",
							  "      01BY 5  B20      ",
							  "      00G03030G00      ",
							  "      00S0 0 0Y00      ",
							  "       030000030       ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev18.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




