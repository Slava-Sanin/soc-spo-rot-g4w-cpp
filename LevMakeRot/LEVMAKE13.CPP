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
							  "      4004   4004      ",
							  "     1SYG 201  BY2     ",
							  "     0Y00 000 00K0     ",
							  "     1RBK 201  GR2     ",
							  "      301     203      ",
							  "      000  5  000      ",
							  "      401     204      ",
							  "     1RG  201  BK2     ",
							  "     0K00 000 00Y0     ",
							  "     1GBSY201KGBS2     ",
							  "      3003   3003      "};
  FILE *fp;
  fp=fopen("d:\\g4w\\rotms\\lev13.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




