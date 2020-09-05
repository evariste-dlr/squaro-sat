/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *       Projet INF241 - Squaro
 *   ---------------------------------
 *
 *  logique.c
 *  implémentation des fonctions de logique
 *
 *  - initialisations
 *  - conversions de chaines
 *  - utilitaires
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "logique.h"

void init_clause(Clause* c, int n){
  c->nb_lit = n;
  c->coef = (int*)malloc(n*sizeof(int));
}

void init_fnc(Fnc* f, int n, int b){
  int i;

  f->nb_clse = n;
  f->nb_lit = b;
  f->lit = (int*)calloc(b, sizeof(int));
  f->clauses = (Clause*)malloc(n*sizeof(Clause));

  for (i=0; i<n; i++){
    init_clause(&(f->clauses[i]), b);
  }
}

void init_fnc_nl(Fnc* f, int n, int b, int l){
  int i;

  f->nb_clse = n;
  f->nb_lit = b;
  f->lit = (int*)calloc(b, sizeof(int));
  f->clauses = (Clause*)malloc(n*sizeof(Clause));

  for (i=0; i<n; i++){
    init_clause(&(f->clauses[i]), l);
  }
} 

void init_assign(Assign* a, int n){
  a->nb_var = n;
  a->val = (int*)calloc(n,sizeof(int));
}

void del_clause(Clause* c){
  free (c->coef);
}

void del_fnc(Fnc* f){
  int i=0;
  for (i=0; i<f->nb_clse; i++){
    free(f->clauses[i].coef);
  }
  free(f->clauses);
}

void del_assign(Assign* a){
  free(a->val);
}

/* -------------------------------------- */

/* clause --> string
   La chaine fournie doit être assez longue
*/
void clse2str(Clause cls, char* dest){
  int i,j, n, p, k;
  j = 0;

  for(i=0; i<cls.nb_lit; i++){
    n = cls.coef[i];
    
    // n <- |n| dans le cas négatif
    if (n < 0){
      n = -n;
      dest[j] = '-';
      j++;
    }

    // dest[j...j+log(n)] = ascii(n)
    // le log nous permet de savoir
    // le nombre de case nessessaires
    if (n > 0){
      p = (int)log10(n);
      for(k=pow(10, p); k>=1; k/=10){
	dest[j] = '0' + (int)(n / k);
	n -= (dest[j] - '0') * k;
	j++;
      }
    }
    else{
      dest[j] = '0';
      j++;
    }

    //*/

    dest[j] = ' ';
    j++;
  }
}



/* ------------- Affichages ------------- */

void print_fnc(Fnc f){
  char* str = NULL;
  int i, j;

  // allocation de la chaine en fct de f.n
  // considérons chaque lit < 1000
  str = (char*)malloc(f.nb_lit*3*sizeof(char));
  
  printf(" Ensemble de clauses : [\n");
  printf("    Variables : [ ");
  for (i=0; i<f.nb_lit; i++){
    printf("%d ", f.lit[i]);
  }
  printf("] \n");
  for (i=0; i<f.nb_clse; i++){
    printf("  ");
    for (j=0; j<f.clauses[i].nb_lit; j++){
      printf("%d ", f.clauses[i].coef[j]); 
    }
    printf("; \n");
  }
  printf("] \n");
}

/* --------------------------------------
   --------     Utilitaires  ------------
   --------------------------------------
*/

int est_lit(Fnc f, int lit){
  int i, b;
  b = 0;

  for(i=0; i<f.nb_clse; i++){
    if (est_lit_clause(f.clauses[i], lit)){
      b = 1;
      i = f.nb_clse;
      // arrêt SALE ET BOURRIN de la boucle :D
    }
  }
  return b;
}

int est_lit_clause(Clause c, int lit){
  int i, b;
  b = 0;
  
  for(i=0; i<c.nb_lit; i++){
    if (c.coef[i] == lit){
      b = 1;
      i = c.nb_lit;
      // aussi sale mais fier !
    }
  }
  return b;
}

int vide(Fnc f){
  return f.nb_clse == 0;
}



int est_valide(Clause c){
  int i, j;
  
  for (i=0; i<c.nb_lit; i++){
    if (est_lit_clause(c, -(c.coef[i])))
      return 1;
  }
  return 0;
}


/* La conjonction agit comme une concaténation */
Fnc conjonction(Fnc g, Fnc h, int n){
  Fnc res;
  int i, j;
  
  init_fnc(&res, g.nb_clse + h.nb_clse, g.nb_lit + n);

  /* copie des clauses de g */
  for (i=0; i<g.nb_clse; i++){
    for(j=0; j<g.clauses[i].nb_lit; j++){
      res.clauses[i].coef[j] = g.clauses[i].coef[j];
    }
  }

  /* copie des clauses de h */
  for (i=0; i<h.nb_clse; i++){
    for (j=0; j<h.clauses[i].nb_lit; j++){
      res.clauses[g.nb_clse + i].coef[j] = h.clauses[i].coef[j];
    }
  }


  /* copie des Variables */
  for (i=0; i<g.nb_lit; i++){
    res.lit[i] = g.lit[i];
  }

  if (n == 2){
    res.lit[res.nb_lit-2] = h.lit[1];
  }
  res.lit[res.nb_lit-1] = h.lit[3];
  
  return res;

}



/* ------------------------------------------------ 
   ---          Manipulations de Listes        ----
   ------------------------------------------------ */

Fnc_l* fnc2lst(Fnc f){
  Fnc_l* res = NULL;
  Fnc_l* ec = NULL;
  Fnc_l* tmp = NULL;
  
  int i,j;

  //------- Création de l'ancre pour l'init -----
  res = (Fnc_l*)malloc(sizeof(Fnc_l));
  res->nxt = NULL;
  res->prv = NULL;
  init_clause(&(res->c), 0);
  ec = res;
  //--------------------------------------------

  for(i=0; i<f.nb_clse; i++){
    tmp = (Fnc_l*)malloc(sizeof(Fnc_l));
    tmp->prv = ec;
    tmp->nxt = NULL;
    ec->nxt = tmp;
    ec = tmp;
    init_clause(&(ec->c), f.clauses[i].nb_lit);
    for(j=0; j<f.clauses[i].nb_lit; j++){
      ec->c.coef[j] = f.clauses[i].coef[j];
    }
  }

  // on supprime la clause-ancre du début
  return rm_clause(&res, res);
  
}


Fnc lst2fnc(Fnc_l* f){
  int n, nl;
  int i, j;
  Fnc res;
  Fnc_l* ec;
  
  /* calcul de la taille 
     et du nombre max de littéraux
  */
  ec = f;
  n = 0;
  nl = 0;
  while (ec != NULL){
    n++;
    if (ec->c.nb_lit > nl)
      nl = ec->c.nb_lit;

    ec = ec->nxt;
  }

  
  /* init de res */
  init_fnc(&res, n, nl);
  
  for (i=0; i<n; i++){
    res.clauses[i].nb_lit = f->c.nb_lit;
    for (j=0; j<(f->c.nb_lit); j++){
      res.clauses[i].coef[j] = f->c.coef[j];
    }
    f = f->nxt;
  }

  return res;
}


Fnc_l* add_clause(Fnc_l* pos, Clause c){
  Fnc_l* res;
  int i;
  
  /* création de la nouvelle clause */
  res = (Fnc_l*)malloc(sizeof(Fnc_l));
  res->nxt = NULL;
  res->prv = NULL;
  init_clause(&(res->c), c.nb_lit);
  for(i=0; i<c.nb_lit; i++){
    res->c.coef[i] = c.coef[i];
  }
  

  /* Cas de liste vide */
  if(pos == NULL){
    pos = res;
    return pos;
  } 

  /* Cas normal */
  else{
    res->prv = pos;
    res->nxt = pos->nxt;
    pos->nxt = res;
    if (res->nxt != NULL)
      res->nxt->prv = res;
    
    return res;
  }
    
}


Fnc_l* rm_clause(Fnc_l** lst, Fnc_l* pos){
  Fnc_l* tmp = NULL;
  
  tmp = pos;

  /* Si liste non vide */
  if (tmp != NULL){
    
    /* cas d'un seul élément */
    if (tmp->prv == NULL && tmp->nxt == NULL){
      pos = NULL;
      *lst = NULL;
      del_clause(&(tmp->c));
      free(tmp);
    }
    /* cas de la tête */
    else if (tmp->prv == NULL){
      pos = tmp->nxt;
      pos->prv = NULL;
      tmp->nxt = NULL;
      del_clause(&(tmp->c));
      free(tmp);

      *lst = pos;
      
    }
    else{
      pos = tmp->prv;
      pos->nxt = tmp->nxt;
      if (tmp->nxt != NULL){
	tmp->nxt->prv = pos;
      }
      tmp->nxt = NULL;
      tmp->prv = NULL;
      del_clause(&(tmp->c));
      free(tmp);
      
      // positionnement au suivant
      pos = pos->nxt;
    }
  }
  return pos;
}


void print_fnc_l(Fnc_l* f){
  int i;

  printf("Ensemble de clauses [ \n");
  while (f != NULL){
    for (i=0; i<(f->c.nb_lit); i++){
      printf(" %d", f->c.coef[i]);
    }
    printf("\n");    
    f = f->nxt;
  }
  printf("] \n");
}


void del_fnc_l(Fnc_l* f){
  Fnc_l* tmp;

  while (f != NULL){
    tmp = f;
    f = f->nxt;
    
    del_clause(&(tmp->c));
    free(tmp);
  }
  
}

