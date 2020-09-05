/*
  Programme de transformation d'un
  plateau en fichier CNF DIMACS
*/

#include <stdio.h>
#include <stdlib.h>

#include "../lib/plateau.h"
#include "../lib/logique.h"
#include "../lib/dimacs.h"


/* Structure de liste chainée 
   pour stocker le résultat
   au fur et à mesure */
typedef struct _lst{
  Fnc f;
  struct _lst* nxt;
}Lst_Fnc;



/* Prototypes */

Fnc modelisation(Plateau);
Fnc fnc_case(Plateau, int, int);
void del_lst(Lst_Fnc*);

Fnc modelisation(Plateau p){
  int i, j, n;
  Lst_Fnc* res = NULL;
  Lst_Fnc* courant = NULL;
  Lst_Fnc* tmp = NULL;

  Fnc f;

  /* Pour chaque case, on récupère l'ensemble
     des clauses correspondant au chiffre */

  res = (Lst_Fnc*)malloc(sizeof(Lst_Fnc));
  res->f = fnc_case(p, 0, 0);
  res->nxt = NULL;

  courant = res;
  

  /* Récupération des CNF dans une liste chainée */

  n = 2;
  for (i=0; i<p.n; i++){
    for (j=0; j<p.n; j++){

      if (i != 0){
	n = 1;
      }
      
      if ( i != 0 || j != 0){
	tmp = (Lst_Fnc*)malloc(sizeof(Lst_Fnc));
	tmp->f = fnc_case(p, j, i);
	
	courant->nxt = tmp;
	courant = tmp;
	courant->nxt = NULL;
      }
      //print_fnc(courant->f);
    }
  }
  
  
  
  /* Transformation en CNF */

  n=0;
  courant = res;
  while (courant != NULL){
    n += courant->f.nb_clse;
    courant = courant->nxt;
  }

  
  init_fnc(&f, n, (p.n+1)*(p.n+1));
  courant = res;
  n = 0;
  while (courant != NULL){
    for (i=0; i<courant->f.nb_clse; i++){
      f.clauses[n].nb_lit = courant->f.clauses[i].nb_lit;
      for (j=0; j<(courant->f.clauses[i].nb_lit); j++){
	f.clauses[n].coef[j] = courant->f.clauses[i].coef[j];
      }
      n++;
    }
    courant = courant->nxt;
  }

  // màj des variables de f
  for (i=0; i<((p.n+1)*(p.n+1)); i++){
    f.lit[i] = i+1;
  }

  /* Nettoyage de la mémoire */
  
  courant = res;
  while (courant != NULL){
    tmp = courant->nxt;
    del_fnc(&(courant->f));

    free(courant);
    courant = tmp;
  }

  return f;
}


/*--------------------------------------------------------*/


/* retourne la forme normale conjonctive associée à une case 
   Par exemple pour une case numérotée 0 :
     G = {
          -(X_NW)
	  -(X_NE)
	  -(X_SW)
	  -(X_SE)
	 }
*/
Fnc fnc_case(Plateau p, int x, int y){
  Fnc res;
  int i, j;
  int a;
  
  switch (p.val[x][y]){

    /* SI VAL = 0 */

  case 0:
    res = lire_dimacs("src/case0.sat");
    break;

    /* SI VAL = 1 */
    
  case 1:
    res = lire_dimacs("src/case1.sat");
    break;

  case 2:
    res = lire_dimacs("src/case2.sat");
    break;

  case 3:
    res = lire_dimacs("src/case3.sat");
    break;

  case 4:
    res = lire_dimacs("src/case4.sat");
  }    

    /* Recherche et remplacement des coeficients :
       1 par get_no_pixel(p, x, y, NW);
       2 par get_no_pixel(p, x, y, NE);
       ...
    */

  for (i=0; i<res.nb_clse; i++){
    for (j=0; j<res.clauses[i].nb_lit; j++){
      a = abs(res.clauses[i].coef[j]);
      switch (a){
      case 1:
	res.clauses[i].coef[j] /= a; // r.c[i].c[j] = 1 ou -1
	res.clauses[i].coef[j] *= get_no_pixel(p, x, y, NW);
	break;
	
      case 2:
	res.clauses[i].coef[j] /= a; 
	res.clauses[i].coef[j] *= get_no_pixel(p, x, y, NE);
	break;
	
      case 3:
	res.clauses[i].coef[j] /= a; 
	res.clauses[i].coef[j] *= get_no_pixel(p, x, y, SE);
	break;
	
      case 4:
	res.clauses[i].coef[j] /= a; 
	res.clauses[i].coef[j] *= get_no_pixel(p, x, y, SW);
	break;
      }
    }
  }
  

  /* mise à jour des variables */

  res.lit[0] = get_no_pixel(p, x, y, NW);
  res.lit[1] = get_no_pixel(p, x, y, NE);
  res.lit[2] = get_no_pixel(p, x, y, SW);
  res.lit[3] = get_no_pixel(p, x, y, SE);
  
  //print_fnc(res);

  return res;
}


/************************** Programme *************************/

int main(int argc, char* argv[]){
  
  Plateau p = {0, NULL, NULL};
  Fnc res;

  if (argc < 2){
    printf("  [E] < %s > : Programme appelé avec trop peu d'argument \n", argv[0]);
    printf("      Arguments attendus : n ; out \n");
    printf("            in, string : nom du fichier de plateau à modéliser \n");
    printf("           out, string : nom du fichier de sortie \n \n");
    
    exit(EXIT_FAILURE);
  }

  p = ouvrir(argv[1]);
  //print(p);

  res = modelisation(p);
  ecrire_dimacs(argv[2], res);

  /* Affichage du résultat */
  if (res.nb_clse < 150){
    printf("  [I] Modélisation avec %d clauses : \n \n", res.nb_clse);
    print_fnc(res);
  }
  else{
    printf("  [I] La modélisation ne peut être affichée (trop de clauses) \n");
    printf("  [I] Pour information : %d clauses \n \n", res.nb_clse);
  }

  del_fnc(&res);
  return EXIT_SUCCESS;
}
