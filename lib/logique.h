/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *       Projet INF241 - Squaro
 *   ---------------------------------
 *
 *  logique.h
 *  definition des éléments de logique propositionelle
 *
 *  - forme normale disjonctive (Fnd)
 *  - forme normale conjonctive (Fnc)
 *  - clauses
 *  - assignation
 *  
 *
 */


#ifndef DEF_LOGIQUE
#define DEF_LOGIQUE

#include <string.h>
#include <math.h>

/*  STRUCTURES  */

/*   Structure Clause :
     ------------------

   défini une clause par
   - le nombre de littéraux
   - les coef associés
   
   Le litéral numéro 0 est par
   convention FAUX

   ex.
   Clause c = {3, {-1, 3, 5}}
   correspond à :
   c = non(x_1) + x_3 + x_5
*/

typedef struct{
  int nb_lit;
  int* coef;
}Clause;


/*   Conjonction de clauses :
     ------------------------

   la structure Fnc représente une forme normale
   conjonctive par la liste des clauses la composant.

   - nombre de clauses
   - nombre de littéraux
   - tableau de littéraux
   - tableau de clauses
*/
typedef struct{
  int nb_clse;
  int nb_lit;
  int* lit;
  Clause* clauses;
}Fnc;


/*   CNF en liste chainées :
     -----------------------

     Liste chainée de clauses, pour rendre les 
     manipulations (suppression, ajout...) plus 
     souples.
*/
typedef struct _Lstc{
  Clause c;
  struct _Lstc* nxt;
  struct _Lstc* prv;
}Fnc_l;


/*   Assignation :
     -------------

   un tableau de valeurs booléennes
   réalisant une assignation d'un ensemble de variables

   - nombre de variables
   - tableau de booléens :
     tab[i] vaut la valeur de la variable no i
*/
typedef struct{
  int nb_var;
  int* val;
}Assign;



/* 
   -----------------
   *   FONCTIONS   *
   -----------------
*/

/* ---------- Initialisations --------------*/

void init_clause(Clause*, int);
void init_fnc(Fnc*, int, int);
void init_fnc_nl(Fnc*, int, int, int);
void init_assign(Assign*, int);

void del_clause(Clause*);
void del_fnc(Fnc*);
void del_assign(Assign*);


/* ----------- Manipulations ----------------*/

/*
  Ajoute une clause décrite sous forme de chaine
  de caractère à une Fnc
*/
//void add_clause(Fnc*, char*);


/* --------- Utilitaires de conversions ---- */

void clse2str(Clause cls, char* dest);
Fnc conjonction(Fnc, Fnc, int);
//Fnc fnd2fnc(Fnc formedv);


/* --------- Utilitaires d'affichage ------- */
void print_fnc(Fnc);


/* --------- Utilitaires de contrôle ------- */
int est_lit(Fnc, int);
int est_lit_clause(Clause, int);
int est_valide(Clause);

int vide(Fnc);


/* --------- Manipulations de listes --------*/

Fnc lst2fnc(Fnc_l* f);
Fnc_l* fnc2lst(Fnc f);
Fnc_l* add_clause(Fnc_l* pos, Clause c);
Fnc_l* rm_clause(Fnc_l** lst, Fnc_l* pos);

void print_fnc_l(Fnc_l*);

void del_fnc_l(Fnc_l* f);

#endif
