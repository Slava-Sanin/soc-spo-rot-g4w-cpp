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
							  "    111111111111       ",
							  "    133  1     111     ",
							  "    133  1 4  4  1     ",
							  "    133  141111  1     ",
							  "    133    2 11  1     ",
							  "    133  1 1  4 11     ",
							  "    111111 114 4 1     ",
							  "      1 4  4 4 4 1     ",
							  "      1    1     1     ",
							  "      111111111111     ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev2.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




