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
							  "       1111111111      ",
							  "       1     1  1      ",
							  "       14 4 414 1      ",
							  "      11 11  1  1      ",
							  "      1   1 414 1      ",
							  "      1   1  2  1      ",
							  "      13331     1      ",
							  "      13331111111      ",
							  "      11111            ",
							  "                       ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev16.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




