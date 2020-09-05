/* * * * * * * * * * * * * * * * * * * * * * *
 *        Projet INF241 - Squaro
 *   ---------------------------------
 *
 *  plateau.c :
 *  Implémentation des fonctions de gestion du plateau
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "plateau.h"
 
 void initialisation_plateau(Plateau *p, int n){
     int i, j;
     p->n = n;
     
     p->val = (int**)(malloc(n*sizeof(int*)));
     p->pixels = (int**)(malloc((n+1)*sizeof(int*)));
     
     for(i=0; i<=n; i++){
         if (i<n){
             p->val[i] = (int*)(malloc(n*sizeof(int)));
         }
         p->pixels[i] = (int*)(malloc((n+1)*sizeof(int)));
     }
}


void rempli_alea(Plateau *p){
  /* 
     on remli aléatoirement la grille
     de pixels (taille n+1)
     puis, en fonction de cette grille, 
     on rempli les case
     ensuite on vide la grille de pixels
  */
  int i,j, n;
  srand(time(NULL));

  // remplissage de la grille de pixels
  for(i=0; i<=p->n; i++){
    for(j=0; j<=p->n; j++){
      p->pixels[i][j] = rand() % 2;
    }
  }

  // remplissage des cases
  for(i=0; i<p->n; i++){
    for(j=0; j<p->n; j++){
      n =  get_pixel(*p, i, j, NE);
      n += get_pixel(*p, i, j, NW);
      n += get_pixel(*p, i, j, SE);
      n += get_pixel(*p, i, j, SW);
      p->val[i][j] = n;
    }
  }

  //* effaçage des pixels
  for(i=0; i<=p->n; i++){
    for(j=0; j<=p->n; j++){
      p->pixels[i][j] = 0;
    }
  }
  //*/

}



Plateau ouvrir(char* fichier){
  Plateau p = {0, NULL, NULL};
  int n, i, j;
  char c;
  FILE* f = NULL;

  f = fopen(fichier, "r");
  
  if (f == NULL){
    printf(" [E] Erreur lors de l'ouverture du fichier \"");
    printf("%s", fichier);
    printf("\" \n");
  }
  else{
    // lecture de la taille
    fscanf(f, "%d", &n);

    // initialisation du plateau (taille n)
    initialisation_plateau(&p, n);    

    //lecture carcatère par caractère de la grille de valeurs
    for(i=0; i<n; i++){
      for(j=0; j<n; j++){
	// tant que le char est illisible :
	do{
	  c = fgetc(f);
	}while(c > '4' || c < '0');
	p.val[j][i] = (int)(c - '1') +1;
      }
    }


    // lecture de la grille de pixels de la mm façon
    for(i=0; i<=p.n; i++){
      for(j=0; j<=p.n; j++){
	do{
	  c = fgetc(f);
	}while(c != '0' && c != '1');
	p.pixels[j][i] = (int)(c - '1') +1;
      }
    }

    fclose(f);
  }

  return p;
  
}


int ecrire(Plateau p, char* fichier){
  int i,j;
  FILE* f = NULL;

  f = fopen(fichier, "w");
  
  if (f == NULL){
    printf(" [E] Erreur lors de la création du fichier du fichier \"");
    printf("%s", fichier);
    printf("\" \n");
  }
  else{
    // écriture de la taille :
    fprintf(f, "%d \n", p.n);

    // écriture ligne par ligne de la grille de cases
    for(i=0; i<p.n; i++){
      for(j=0; j<p.n; j++){
	fprintf(f, "%d", p.val[j][i]);
      }
      fprintf(f, "\n");
    }

    // écriture ligne par ligne de la grille de pixels
    for(i=0; i<=p.n; i++){
      for(j=0; j<=p.n; j++){
	fprintf(f, "%d", p.pixels[j][i]);
      }
      fprintf(f, "\n");
    }

    fclose(f);
  }
}
 
 
 void destruction_plateau(Plateau *p){
   int i;
      
      for(i=0; i<=p->n; i++){
          if(i<p->n){
              free(p->val[i]);
          }
          free(p->pixels[i]);
      }
      
      free(p->val);
      free(p->pixels);
      //free(p);
 }
 


int get_pixel(Plateau p, int x, int y, int dir){
  int res;

  switch(dir){
  case NW: 
    res = p.pixels[x][y];
    break;

  case NE:
    res = p.pixels[x+1][y];
    break;

  case SW :
    res = p.pixels[x][y+1];
    break;

  case SE:
    res = p.pixels[x+1][y+1];
    break;

  default:
    res = 0;
  }

  return res;
}


int get_no_pixel(Plateau p, int x, int y, int dir){
  int res;
  int n = p.n;

  switch(dir){
  case NW: 
    res = y*(n+1)+x;
    break;

  case NE:
    res = y*(n+1)+x+1;
    break;

  case SW :
    res = (y+1)*(n+1)+x;
    break;

  case SE:
    res = (y+1)*(n+1)+x+1;
  }

  if(res >= (n+1)*(n+1))
    res = -1;

  /* la numérotation commence à 1 */
  return res+1;
}


void set_pixel(Plateau* p, int x, int y, int val){
  p->pixels[x][y] = val;
}

/* Affichage console */
void print(Plateau p){

  int i,j;
  
  /* affichage alternatif de :
     - une ligne du tableau pixels
     - une ligne du tableau val 
  */

  for(i=0; i<p.n; i++){
    for(j=0; j<p.n; j++){
      if (p.pixels[j][i])
	printf("@---");
      else
	printf("O---");
    }
    
    if (p.pixels[p.n][i])
      printf("@ \n");
    else
      printf("O \n");

    for(j=0; j<p.n; j++){
      printf("| %d ", p.val[j][i]);
    }

    printf("| \n");
  }

  /* affichage de la dernière ligne */
  for(j=0; j<p.n; j++){
    if(p.pixels[j][p.n])
      printf("@---");
    else
      printf("O---");
  }
  if(p.pixels[p.n][p.n])
    printf("@ \n");
  else
    printf("O \n");

  printf("\n");

}
