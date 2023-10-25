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
	char lev_data[][B]={"             11111     ",
							  "       1111111   11    ",
							  "      11 1 211 44 1    ",
							  "      1    4      1    ",
							  "      1  4  111   1    ",
							  "      111 111114111    ",
							  "      1 4  111 331     ",
							  "      1 4 4 4 3331     ",
							  "      1    1113331     ",
							  "      1 44 1 13331     ",
							  "      1  111 11111     ",
							  "      1111             "};
  FILE *fp;
  fp=fopen("d:\\g4w\\socoban\\lev5.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




