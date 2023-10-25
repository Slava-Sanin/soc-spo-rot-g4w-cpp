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
	char lev_data[][B]={"   111111111111111111  ",
							  "   1   31      13   1  ",
							  "   1 1  31    13  1 1  ",
							  "   1 1   314 13   1 1  ",
							  "   1   1  1 414 1   1  ",
							  "    1111 4 4 4  1111   ",
							  "    1111  424 4 1111   ",
							  "   1   1 414 1  1   1  ",
							  "   1 1   31 413   1 1  ",
							  "   1 1  31    13  1 1  ",
							  "   1   31      13   1  ",
							  "   111111111111111111  "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev18.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




