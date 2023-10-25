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
							  "            1111       ",
							  "    111111111  11      ",
							  "   11  4      4 11111  ",
							  "   1   11 11   113331  ",
							  "   1 144 4 4414113331  ",
							  "   1 1    2  1   3331  ",
							  "   1  41 11144   3331  ",
							  "   1 4  44  4 1133331  ",
							  "   1114       1111111  ",
							  "     1  1111111        ",
							  "     1111              "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev10.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




