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
							  "          1111111      ",
							  "      11111  1  1111   ",
							  "      1   1   4    1   ",
							  "   1111 144 11 11  1   ",
							  "  11      1 1  11 111  ",
							  "  1  111 414  4  4  1  ",
							  "  1333    1 11  1   1  ",
							  "  13331    2 1 111 11  ",
							  "  13331  111  4  4  1  ",
							  "  11111111 11   1   1  ",
							  "            111111111  "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev9.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




