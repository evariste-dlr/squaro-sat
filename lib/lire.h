/*
  Projet INF242 - Paquetage LIRE
              -------

   Regroupe différentes fonctions pour
   lire des valeurs à partir d'une chaine.
   
   . lire_int(char* str, int* pos) --> int
   .
   .
*/

#ifndef DEF_LIRE
#define DEF_LIRE

/*
  lit le premier entier trouvé dans la chaine str
  à partir de la position *pos.
  à l'état final, *pos est la position qui suit 
  le dernier chiffre */
int lire_int(char* str, int* pos);

#endif
