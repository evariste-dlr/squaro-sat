/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *       Projet INF241 - Squaro
 *   ---------------------------------
 *
 *  dimacs.h
 *  Utilisation du format de fichier DIMACS
 *
 *  - Lecture/Ecriture d'un fichier
 *  - représentation en C
 *  - fonctions utilitaires
 *
 *  
 *
 */
 
 
#ifndef DEF_DIMACS
#define DEF_DIMACS

#include "logique.h"
#include "lire.h"

/* STRUCTURES */



/* FONCTIONS */

/* lit un fichier dimacs.
   retourne l'ensemble de clause décrit dans le fichier
   retourne une structure vide en cas d'échec
*/
Fnc lire_dimacs(char* nf);


/* Enregistre un fichier dimacs.
   retourne vrai ssi réussite
*/
int ecrire_dimacs(char* nf, Fnc g);


   /* Utilitaires */

/* Insert un nouveau littéral dans le 
   tableau trié de littéraux de g 
*/
void insert_lit(Fnc* g, int a, int* n); 
int compareints (const void * a, const void * b);
#endif
