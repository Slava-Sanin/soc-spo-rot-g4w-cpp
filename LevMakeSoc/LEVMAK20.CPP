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
	char lev_data[][B]={" 1111 111 111 111 1111 ",
							  "1    1   1   1   1    1",
							  "1  1   3  1  3 5 1  1 1",
							  "1 3 3 1 1 3  1   3  1 1",
							  "1 1   1 1 11 1 5 51 1 1",
							  " 1 3  1 1 1 11 1 5 11 1",
							  " 1  1 131 1  3 1 5  3 1",
							  "1 3 3 1 1 12 1 1 5  1 1",
							  "1  1 414141  141454 1 1",
							  "1   4  4  4  4  4  4  1",
							  "1    1   1   1   1    1",
							  " 1111 111 111 111 1111 "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev20.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




