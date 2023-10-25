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
	char lev_data[][B]={"11111111111111111111111",
							  "112  1     1     1   11",
							  "1  3    1  3  1 1  3  1",
							  "1 1 1   1 1 1 1 1 1 1 1",
							  "1 1    1  1 1 1 1 1 1 1",
							  "1  3   1  1 1 1 1 1 1 1",
							  "1   1 1   151 1 1 151 1",
							  "1 1 5 1 1 1 1 1 1 1 1 1",
							  "1  1  151 1 1  5  1 1 1",
							  "1    4   4   4   4    1",
							  "11   1   1   1   1   11",
							  "11111111111111111111111"};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev19.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




