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
	char lev_data[][B]={"         11111         ",
							  "        11 1 11        ",
							  "       113333311       ",
							  "       1 33133 1       ",
							  "       1 2 1   1       ",
							  "    11111 111 11111    ",
							  "    1   1 111 1   1    ",
							  "   11  4  4 4  4  11   ",
							  "   1  411 1 1 114  1   ",
							  "   11  4   4   4  11   ",
							  "    1   1  1  1   1    ",
							  "    111111111111111    "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev12.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




