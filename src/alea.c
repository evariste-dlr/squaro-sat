#include <stdio.h>
#include <stdlib.h>

#include "../lib/plateau.h"

int main(int argc, char *argv[]){
  Plateau p = {0, NULL, NULL};  
  int n;
  char s[100] = {0};

  if (argc < 2){
    printf("  [E] < %s > : Programme appelé avec trop peu d'argument \n", argv[0]);
    printf("      Arguments attendus : n ; out \n");
    printf("             n, int    : taille du plateau \n");
    printf("           out, string : nom du fichier de sortie \n \n");
    
    return 1;
  }

  /* récupération de la taille du plateau */
  sscanf(argv[1], "%d", &n);
    
  initialisation_plateau(&p, n);
  rempli_alea(&p);
  
  print(p);
    
  ecrire(p, argv[2]);
    
  destruction_plateau(&p); 

  return 0;
}
