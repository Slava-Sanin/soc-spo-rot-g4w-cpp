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
	char lev_data[][B]={"         11111111      ",
							  "    1111113 11  11     ",
							  "    1    234     1     ",
							  "    1 111131111  1     ",
							  "    1 111131111 11     ",
							  "    1      1111 1      ",
							  "    1111 4 4 11 1      ",
							  "      11 111 11 1      ",
							  "      1  11  11 11     ",
							  "      1      4   1     ",
							  "      11111  1   1     ",
							  "          11111111     "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev14.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




