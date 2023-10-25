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
	char lev_data[][B]={"     1111111           ",
							  "     1  1  11111       ",
							  "    11  1  1333111     ",
							  "    1  41  1333  1     ",
							  "    1 4 144 333  1     ",
							  "    1  41  1333 31     ",
							  "    1   1 411111111    ",
							  "    114       4 4 1    ",
							  "    11  1  44 1   1    ",
							  "     111111  114421    ",
							  "          1      11    ",
							  "          11111111     "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev6.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




