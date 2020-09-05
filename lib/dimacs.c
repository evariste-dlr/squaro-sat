/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *       Projet INF241 - Squaro
 *   ---------------------------------
 *
 *  dimacs.c
 *  Implémentation des méthodes sur le format DIMACS
 */
#include <stdio.h>
#include <stdlib.h>
#include "dimacs.h"

Fnc lire_dimacs(char* nf){
  
  Fnc dim;
  char *line = NULL;
  size_t len = 0;
  size_t read;
  FILE* f = NULL;
  int i,j;
  int coef = 1;
  int pos = 0;
  int nc, nl;
  int k = 0;

  f = fopen(nf, "r");
  if (f == NULL) {
    printf(" [E] erreur lors de l'ouverture du fichier DIMACS : ");
    printf(" %s \n", nf);
  }
  else{

    /* Lecture du fichier ligne par ligne... */

    i=0;
    while (( read = getline(&line, &len, f)) != -1){
      /* traittement de la ligne lue */
      /* commentaire : ligne ignorée */
      if (line[0] != 'c'){
	
	/* déclaration probleme */
	if (line[0] == 'p'){
	  pos = 0;
	  nl = lire_int(line, &pos);
	  nc = lire_int(line, &pos);

	  init_fnc(&dim, nc, nl);
	  for(j=0; j<nc; j++){
	    init_clause(&(dim.clauses[j]), nl);
	  }
	}
	else{
	  /* lecture d'une clause */
	  j=0;
	  pos = 0;
	  while ((coef = lire_int(line, &pos)) != 0){
	    dim.clauses[i].coef[j] = coef;
	    insert_lit(&dim, coef, &k);
	    j++;
	  }
	  dim.clauses[i].nb_lit = j;
	  i++;
	}
      }
    }
    
    free(line);
    fclose(f);
  }
  
  return dim;
  
  
}

int ecrire_dimacs(char* nf, Fnc g){
  
  int i, j;
  FILE* f = NULL;
  
  f = fopen(nf, "w+");
  if (f == NULL){
    printf(" [E] erreur lors de la création du fichier DIMACS : ");
    printf(" %s \n", nf);

    return 0;
  }

  fputs("c Fichier CNF généré automatiquement \n", f);
  fprintf(f, "p cnf %d %d \n", g.nb_lit, g.nb_clse);
  
  
  for (i=0; i<g.nb_clse; i++){
    for (j=0; j<g.clauses[i].nb_lit; j++){
      fprintf(f, " %d", g.clauses[i].coef[j]);
    }
    fputs(" 0 \n", f);
  }
  


  fclose(f);
  return 1;
}


void insert_lit(Fnc* g, int a, int* n){

  /* recherche de la valeur */

  int* pItem = NULL;
  int* pmItem = NULL;
  int key = -a;
  
  pItem = (int*) bsearch (&key, g->lit, *n, sizeof (int), compareints);
  pmItem= (int*) bsearch(&a, g->lit, *n, sizeof(int), compareints);

  /* insertion si elle n'existe pas */
  if (pItem == NULL && pmItem == NULL){
    g->lit[*n] = abs(a);
    *n = *n + 1;

    // tri du tableau
    qsort(g->lit, *n, sizeof(int), compareints);
  }

}

int compareints (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}
