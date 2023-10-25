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
							  "           11111111    ",
							  "           1     21    ",
							  "           1 414 11    ",
							  "           1 4  41     ",
							  "           114 4 1     ",
							  "   111111111 4 1 111   ",
							  "   13333  11 4  4  1   ",
							  "   11333    4  4   1   ",
							  "   13333  1111111111   ",
							  "   11111111            ",
							  "                       "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev3.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




