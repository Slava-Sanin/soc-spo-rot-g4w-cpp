#include <stdio.h>
#include <io.h>
#include <iostream.h>
#include <conio.h>
#define A 7
#define B 15
// 0 - stena
// ' ' - place for ball
// 2 - blue spot
// 3 - red blue
main()
{
	char lev_data[][B]={"000000000000000",
							  "02           30",
							  "0             0",
							  "0             0",
							  "0             0",
							  "03           20",
							  "000000000000000"};
  FILE *fp;
  fp=fopen("d:\\g4w\\spot\\lev1.dat","wt");
  fwrite(&lev_data, A*B,1,fp); /* write struct lev_data to file */
  fclose(fp);
  return 0;
}




