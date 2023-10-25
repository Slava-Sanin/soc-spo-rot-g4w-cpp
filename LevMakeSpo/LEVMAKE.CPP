#include <stdio.h>
#include <io.h>
#include <iostream.h>
#include <conio.h>
#define A 7
#define B 15
// ' ' - space
// 0 - stena
// 1 - place for ball
// 2 - blue spot
// 3 - red blue
main()
{                                      ",
	char lev_data[][B]={"
							  "
							  "
							  "
							  "
							  "
							  "
  FILE *fp;
  fp=fopen("d:\\g4w\\spot\\lev0.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




