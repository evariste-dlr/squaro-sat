#include <stdio.h>
#include <stdlib.h>

#include "../lib/plateau.h"
#include "../lib/lire.h"

int main(int argc, char *argv[]){
  Plateau p = {0, NULL, NULL};
  FILE* f = NULL;
  int i, j, n, pos;

  char *line = NULL;
  size_t len = 0;
  size_t read;

  if (argc < 2){
    printf("  [E] < %s > : Programme appelé avec trop peu d'argument \n", argv[0]);
    printf("      Arguments attendus :  \n");
    printf("           plt, string : nom du fichier de plateau \n");
    printf("           ass, string : nom du fichier de d'assignation \n \n");
    
    exit(EXIT_FAILURE);
  }

  f = fopen(argv[2], "r");
  p = ouvrir(argv[1]);

  while (( read = getline(&line, &len, f)) != -1){
    if (line[0] == 'S'){
      printf("Le plateau est résoluble : \n");
    }
    else if (line[0] == 'U'){
      printf("Le plateau n'est pas résoluble. \n");
    }
    else{
      pos = 0;
      for (i=0; i<=p.n; i++){
	for (j=0; j<=p.n; j++){
	  n = lire_int(line, &pos);
	  if (n > 0){
	    set_pixel(&p, j, i, 1);
	  }else{
	    set_pixel(&p, j, i, 0);
	  }
	}
      }

    }
  }

  print(p);

  destruction_plateau(&p);
  fclose(f);
  return EXIT_SUCCESS;
}
