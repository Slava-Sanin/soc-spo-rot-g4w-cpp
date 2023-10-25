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
							  "            1111       ",
							  "      1111111 21       ",
							  "      1     4  1       ",
							  "      1   411 41       ",
							  "      11413331 1       ",
							  "       1 4333  1       ",
							  "       1 13 31 11      ",
							  "       1   1 14 1      ",
							  "       14  4    1      ",
							  "       1  1111111      ",
							  "       1111            "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev8.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




