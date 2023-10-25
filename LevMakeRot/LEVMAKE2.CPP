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
							  "     0444444444440     ",
							  "     1           2     ",
							  "     1           2     ",
							  "     1           2     ",
							  "     1     YG    2     ",
							  "     1    RKYG   2     ",
							  "     1    GRKY   2     ",
							  "     1     GR    2     ",
							  "     1           2     ",
							  "     1           2     ",
							  "     0333333333330     "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev2.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




