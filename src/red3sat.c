/*
  Programme de transformation d'un
  problème sat en problème 3-SAT
*/

#include <stdio.h>
#include <stdlib.h>

#include "../lib/logique.h"
#include "../lib/dimacs.h"


/* liste chainée de clause */
typedef struct _Lc{
  Clause c;
  struct _Lc* nxt;
}Lst_clauses;

/* Prototypes des fonctions */
Fnc red(Fnc);
void add_clause_3(Lst_clauses**, int, int, int);


Fnc red(Fnc f){
  int n=0;
  int i, j;
  int nz;
  Lst_clauses* lst = NULL;
  Lst_clauses* courant = NULL;
  Fnc res;

  nz = f.nb_lit + 1;

  /* clause valide pour commencer */
  lst = (Lst_clauses*)malloc(sizeof(Lst_clauses));
  init_clause(&(lst->c), 3);
  lst->c.coef[0] = nz;
  lst->c.coef[1] = nz;
  lst->c.coef[2] = -nz;

  courant = lst;

  for (i=0; i<f.nb_clse; i++){
    switch (f.clauses[i].nb_lit){
    case 1:
      add_clause_3(&courant, 
		 f.clauses[i].coef[0],
		 nz, nz+1);

      add_clause_3(&courant,
		 f.clauses[i].coef[0],
		 nz, -(nz+1));

      add_clause_3(&courant,
		 f.clauses[i].coef[0],
		 -nz, nz+1);

      add_clause_3(&courant,
		 f.clauses[i].coef[0],
		 -nz, -(nz+1));
      nz += 2;
      n += 4;
      break;


    case 2:
      add_clause_3(&courant,
		 f.clauses[i].coef[0],
		 f.clauses[i].coef[1],
		 nz);

      add_clause_3(&courant,
		 f.clauses[i].coef[0],
		 f.clauses[i].coef[1],
		 -nz);
      nz ++;
      n += 2;
      break;

    case 3:
      add_clause_3(&courant,
		 f.clauses[i].coef[0],
		 f.clauses[i].coef[1],
		 f.clauses[i].coef[2]);
      n++;
      break;

    case 4:
      add_clause_3(&courant,
		 f.clauses[i].coef[0],
		 f.clauses[i].coef[1],
		 nz);

      add_clause_3(&courant,
		 -nz,
		 f.clauses[i].coef[2],
		 f.clauses[i].coef[3]);
      n+=2;
      nz++;

    } // end switch
  } // end for
  
    /* On créé une Fnc à partir de la liste chainée */
  courant = lst->nxt;
  del_clause(&(lst->c));
  free(lst);
  lst = courant;
  
  // initialisation des clauses à 3 lit
  init_fnc_nl(&res, n-1, nz-1, 3);
  for (i=0; i<n-1; i++){
    for (j=0; j<3; j++){
      res.clauses[i].coef[j] = courant->c.coef[j];
    }
    courant = courant->nxt;

    //*---- libération mémoire -----
    del_clause(&(lst->c));
    free(lst);
    lst = courant;
    //-----------------------------
    //*/
  }
  print_fnc(res);
  
  return res;
}


/* -------------------- add_clause -------------------*/

void add_clause_3(Lst_clauses** lst, int z1, int z2, int z3){
  Lst_clauses* tmp;

  tmp = (Lst_clauses*)malloc(sizeof(Lst_clauses));
  init_clause(&(tmp->c), 3);
  
  tmp->c.coef[0] = z1;
  tmp->c.coef[1] = z2;
  tmp->c.coef[2] = z3;

  
  if (*lst == NULL){
    *lst = tmp;
  }else{
    (*lst)->nxt = tmp;
    *lst = tmp;  
  }
}


/* ----------------------------------------------------------
   ----------------------- main() ---------------------------
   ---------------------------------------------------------- */

int main(int argc, char* argv[]){
  Fnc res, f;

  if (argc < 2){
    printf("  [E] < %s > : Programme appelé avec trop peu d'argument \n", argv[0]);
    printf("      Arguments attendus : \n");
    printf("            in, string : nom du fichier sat à transformer \n");
    printf("           out, string : nom du fichier de sortie \n \n");
    
    exit(EXIT_FAILURE);
  }

  f = lire_dimacs(argv[1]);
  res = red(f);

  ecrire_dimacs(argv[2], res);
  
  del_fnc(&f);
  del_fnc(&res);
  return EXIT_SUCCESS;
}

