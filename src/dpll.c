/*
  Implémentation DPLL pour 
  des problèmes 3-SAT
*/

#include <stdio.h>
#include <stdlib.h>

#include "../lib/logique.h"
#include "../lib/dimacs.h"


/* -------------- Prototypes des fonctions ------------------ */

int red(Fnc_l** lst);
int li(Fnc_l** lst, Assign* a);
int ru(Fnc_l** lst);

// assigne une valeur à var
Fnc_l* assign(Fnc_l* lst, int var, int val);

// Fonction d'appel
int dpll(Fnc f, Assign* a);

// détermine la prochaine variable
int heuristique(Fnc_l* lst);

// algo
int algo_dpll(Fnc_l** lst, Assign* a);



/* --------------------- utilitaires -------------------------*/



int red(Fnc_l** lst){
  Fnc_l* i = NULL;
  Fnc_l* j = NULL;
  int res = 0;
  int k;

  i = *lst;
  while (i != NULL){
    j = *lst;
    while (j != NULL){
      if (i != j){
	k = 0;
	while (k < (i->c.nb_lit) 
	       && est_lit_clause(j->c, i->c.coef[k])){
	  k++;
	}
	if (k == (i->c.nb_lit)){
	  j = rm_clause(lst, j);
	  res = 1;
	}
	else{ j = j->nxt; }
      }
      else{ j = j->nxt; }
    }
    i = i->nxt;
  }
  return res;
}


int li(Fnc_l** lst, Assign* a){
  Fnc_l* ec = NULL;
  Fnc_l* es = NULL;
  int i, n;
  int res = 0;
  int b;


  ec = *lst;
  while(ec != NULL){
    b = 0;
    for (i=0; i<(ec->c.nb_lit); i++){
      n = ec->c.coef[i];
      es = *lst;
      while (es != NULL && !est_lit_clause(es->c, -n)){
	es = es->nxt;
      }
      
      if (es == NULL){
	// littéral isolé <-- 1
	a->val[abs(n)-1] = (n > 0);
	// arrêt de la boucle avec code d'arrêt
	//i = ec->c.nb_lit +1; 
	res = 1;	
	b = 1;
      }
    }
    if (b){
      ec = rm_clause(lst, ec);
    }
    else
      ec = ec->nxt;
  }
  return res;
}


int ru(Fnc_l** lst){
  Fnc_l* i = NULL;
  Fnc_l* j = NULL;

  /* Recherche de clauses unitaires complémentaires */
  i = *lst;
  while (i != NULL){
    j = i;
    while (j != NULL && i->c.nb_lit == 1){
      if (j->c.nb_lit == 1){
	if (j->c.coef[0] + i->c.coef[0] == 0){
	  /* on retourne la clause Fausse */
	  del_fnc_l(*lst);
	  *lst = (Fnc_l*)malloc(sizeof(Fnc_l*));
	  init_clause(&((*lst)->c), 1);
	  (*lst)->c.coef[0] = 0;
	  return 1;
	}
      }
      j = j->nxt;
    }
    i = i->nxt;
  }

  /* Sinon réduction unitaire  :
     - la réduction est déjà effectuée,
     - enlever les complémentaires dans les clauses restantes
  */
  
  int k;
  int res = 0;
  i = *lst;
  while (i != NULL){
    j = *lst;
    while (j != NULL && i->c.nb_lit == 1){

      if (j != i && j->c.nb_lit > 1){
	k = 0;
	while (k < (j->c.nb_lit) && 
	       j->c.coef[k] != -(i->c.coef[0])){ k++; }
	
	if (k < j->c.nb_lit){	  
	  // suppression du littéral
	  (j->c.nb_lit) -= 1;
	  j->c.coef[k] = j->c.coef[j->c.nb_lit];
	  res=1;
	}
      }
      j = j->nxt;

    }
    i = i->nxt;
  }

  return res;
}


Fnc_l* assign(Fnc_l* lst, int var, int val){
  Fnc_l* res = NULL;
  Fnc_l* ec = NULL;
  Fnc_l* es = NULL;
  int i, k;
  
  /* copie dans une nouvelle liste */
  
  // premier élément :
  res = (Fnc_l*)malloc(sizeof(Fnc_l));
  init_clause(&(res->c), lst->c.nb_lit);
  for (i=0; i<(lst->c.nb_lit); i++){
    res->c.coef[i] = lst->c.coef[i];
  }

  ec = lst->nxt;
  es = res;
  while (ec != NULL){
    es->nxt = (Fnc_l*)malloc(sizeof(Fnc_l));
    es->nxt->prv = es;
    es = es->nxt;

    init_clause(&(es->c), ec->c.nb_lit);
    for (i=0; i<(ec->c.nb_lit); i++){
      es->c.coef[i] = ec->c.coef[i];
    }
    ec = ec->nxt;
  }
  
  //* -----  réduction de la liste  -----

  // val = 1 => suppression des clauses de var
  // val = 0 => suppression de var dans les clauses

  // si var = 0, on recherche -var (=1)
  if (!val){
    var = -var;
  }

  // est_lit_clause(C, var)  => suppression de C
  // est_lit_clause(C, -var) => suppression de -var dans C
  ec = res;
  while (ec != NULL){
    if (est_lit_clause(ec->c, var)){
      ec = rm_clause(&res, ec);
    }
    else{
      k = 0;
      while (k < (ec->c.nb_lit) && 
	     ec->c.coef[k] != -var){ k++; }
      
      if (k < ec->c.nb_lit){
	
	// suppression du littéral
	if (ec->c.nb_lit == 1){
	  
	  // on a G = ... .Ci.0.Cj ... 
	  //      G <=> {0}
	  /* on retourne la clause Fausse */
	  del_fnc_l(res);
	  res = (Fnc_l*)malloc(sizeof(Fnc_l*));
	  init_clause(&((res)->c), 1);
	  res->c.coef[0] = 0;
	  return res;
	}else{
	  ec->c.nb_lit--;
	  ec->c.coef[k] = ec->c.coef[ec->c.nb_lit];
	}
      }
    
      ec = ec->nxt;
    }
  }

  //*/

  return res;
}



/* -------------------------------------------------
   -------             DPLL                 --------
   -------------------------------------------------
*/

int dpll(Fnc f, Assign* a){
  Fnc_l* lst = NULL;
  Fnc_l* cur = NULL;
  int res = 0;

  /* Conversion en liste chainée */
  lst = fnc2lst(f);


  /* Suppression des clauses valides */
  cur = lst;
  while (cur != NULL){
    if (est_valide(cur->c))
      cur = rm_clause(&lst, cur);
    else
      cur = cur->nxt;
  }

  if (lst == NULL){
    res = 1;
  }else{
    res = algo_dpll(&lst, a);
  }

  del_fnc_l(lst);

  return res;
}



int heuristique(Fnc_l* lst){
  /* par définition, lst n'est pas vide
     et ne contient pas 0 */
  
  return abs(lst->c.coef[0]);
}


int algo_dpll(Fnc_l** lst, Assign* a){
  int c = 1;
  int b1, b2;
  int var;

  Fnc_l* fg = NULL;
  Fnc_l* fd = NULL;

  printf(" -------------------- Nouvelle branche ----------------- \n");
  print_fnc_l(*lst);
  printf("  -------------- \n");

  while(c){
    c = 0;

    /* 1 */
    if (*lst == NULL)
      return 1;
    else if ((*lst)->c.coef[0] == 0)
      return 0;
    
    /* 2 */
    c = red(lst);
    printf(" Red : \n");
    print_fnc_l(*lst);


    /* 3 */
    c = c | li(lst, a);
    printf(" li : \n");
    print_fnc_l(*lst);


    /* 4 */
    if (!c){
      c = ru(lst);
      printf(" ru : \n");
      print_fnc_l(*lst);
    }

    /* 5 */
    if (!c){
      c = 0;
      var = heuristique(*lst);
      
      /* Fils droit de l'arbre */
      fd = assign(*lst, var, 0);
      fg = assign(*lst, var, 1);

      b1 = algo_dpll(&fg, a);
      del_fnc_l(fg);

      if (b1){
	a->val[var-1] = 1;
	return b1;
      }

      /* Fils gauche de l'arbre */
      else{
	print_fnc_l(*lst);
	b2 = algo_dpll(&fd, a);
	
	a->val[var-1] = 0;
	return b2;
      }

      del_fnc_l(fd);
    }
  }
}




/* --------------------------------------------------
   -------             main()               ---------
   --------------------------------------------------
*/

int main(int argc, char *argv[]){
  Fnc f;
  Assign a;
  int b;
  int i;

  FILE* out;

  if (argc < 2){
    printf("  [E] < %s > : Programme appelé avec trop peu d'argument \n", argv[0]);
    printf("      Arguments attendus :  \n");
    printf("           in, string : nom du fichier de DIMACS \n");
    printf("          out, string : nom du fichier de sortie \n");
        
    exit(EXIT_FAILURE);
  }


  f = lire_dimacs(argv[1]);
  init_assign(&a, f.nb_lit);


  b = dpll(f, &a);
  
  out = fopen(argv[2], "w+");
  
  if (b){
    fputs("SAT \n", out);
    for (i=0; i<f.nb_lit; i++){
      if (!(a.val[i]))
	fputs("-", out);
      fprintf(out, "%d  ", i+1);
    }
    fprintf(out, "\n");
  }
  else{
    fputs("UNSAT \n", out);
  }
  
  fclose(out);
  del_assign(&a);
  del_fnc(&f);
  
  return EXIT_SUCCESS;
}
