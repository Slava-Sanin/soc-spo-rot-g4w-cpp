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
	char lev_data[][B]={"    111111111111111    ",
							  "    11131      1111    ",
							  "    11331 4  4 1  1    ",
							  "    13331 11 4 1  1    ",
							  "    133333  144   1    ",
							  "    1133334    14 1    ",
							  "    1111 1111111  1    ",
							  "    1   4        11    ",
							  "    1  4 1  41 4 11    ",
							  "    1 4111 4 1 44 1    ",
							  "    1   21  11    1    ",
							  "    111111111111111    "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev7.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




