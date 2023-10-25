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
	char lev_data[][B]={"  11111111111111 111   ",
							  "  1    1       111 1   ",
							  "  1  4   4  144    1   ",
							  "  11 1111 1     1  1   ",
							  "  1 44 1 4114 1    1   ",
							  "  1    4  4     1  1   ",
							  "  111 11 11 144 1  1   ",
							  "  133331 11 1  11111   ",
							  "  13332   14 4  1      ",
							  "  133331 4      1      ",
							  "  133331  1  1111      ",
							  "  111111111111         "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev17.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




