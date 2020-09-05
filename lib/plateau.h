/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *       Projet INF241 - Squaro
 *   ---------------------------------
 *
 *  plateau.h :
 *  Description du plateau de jeu.
 *
 *  Le plateau est composé d'une "superposition" de 2 sous-plateaux : 
 *    - le plateau val contenant les valeurs de chaque cases  (taille n)
 *    - le plateau pixels contenant les coins à colorier      (taille n+1)
 *
 *  Ces sous plateaux sont stockés dans une structure.
 *
 */
 
 
 #ifndef DEF_PLATEAU
 #define DEF_PLATEAU

 #include <time.h>
 
 /* DEFINES */
 
 #define NOIR 1
 #define BLANC 0
 
 #define VAL 0
 #define NE 1
 #define NW 2
 #define SE 3
 #define SW 4
 
 // définition des autres directions
 #define N 5
 #define S 6
 #define E 7
 #define W 8
 
 
 /* STRUCTURES */
 
 // n est la taille du plateau de valeurs
 typedef struct {
     int n;
     int** val;
     int** pixels;
 }Plateau;
 
 typedef struct {
     int x;
     int y;
 }Case;
 
 
 /* FONCTIONS */

/* -------------------- Création, destruction et init ----------------- */
 
/* initialise un plateau vide */
void initialisation_plateau(Plateau *p, int n);

/* remplis un plateau rempli aléatoirement */
void rempli_alea(Plateau *p);

/* retourne un plateau créé à partir d'un fichier
   Le format est : 
   - première ligne : taille
   - lignes suivantes : le plateau
*/
Plateau ouvrir(char* fichier);


/* Enregistre un plateau dans le format plt 
   retourne Vrai ssi réussi
 */
int ecrire(Plateau p, char* fichier);

/* détruit un plateau */
void destruction_plateau(Plateau *p);

/* ------------------- Utilitaires --------------------------*/

/* retourne la valeur bool du pixel dir d'une case */
int get_pixel(Plateau p, int x, int y, int dir);

/* retourne le numéro unique d'un pixel dir */
int get_no_pixel(Plateau p, int x, int y, int dir);

/* met à jour le pixel (x,y) à la valeur val */
void set_pixel(Plateau* p, int x, int y, int val);
/* Affiche le plateau en console */
void print(Plateau p);
 #endif
 
