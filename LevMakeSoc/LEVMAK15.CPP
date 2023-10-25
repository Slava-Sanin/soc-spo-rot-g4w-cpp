#include <stdio.h>
#include <io.h>
#include <iostream.h>
#include <conio.h>
#define A 12
#define B 23
// ' ' - pustota
// 1 - stena
// 2 - chelovek
// 3 - mesto naznacheniya
// 4 - yashik
main()
{
	char lev_data[][B]={"                       ",
							  "     111111111111      ",
							  "     1  11  1   1      ",
							  "     1   4 2  4 1      ",
							  "     1  11151 111      ",
							  "     11 11335  11      ",
							  "     11  3331  11      ",
							  "     1 41141114 1      ",
							  "     1          1      ",
							  "     1  1  111  1      ",
							  "     111111111111      ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev15.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




