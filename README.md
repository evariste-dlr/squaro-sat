# Résolution du jeu de Squaro

Projet INF242\
Année 2012 - 2013  -  Université Joseph Fourier\
Auteur : Évariste Daller\

Ce projet permet de générer et résoudre des plateaux du jeu Squaro (exemple simple [ici](https://jerevise.net/jeux/enigmath/jeu23.php) de taille variable, en s'appuyant sur une modélisation SAT et une résolution à l'aide de l'algorithme [DPLL](https://fr.wikipedia.org/wiki/Algorithme_DPLL).


## 0. Compilation

Pour compiler l'ensemble des programmes du projet :

```
$ make all
```


## I. Configuration de l'environnement


Le script principal est ``squaro.sh``. Vous pouvez le rendre exécutable comme ceci :

```
$ chmod u+x squaro.sh
```

Pour lancer le programme, exécutez le script (``./squaro.sh``) avec les options choisies. Ces options sont décrites dans le paragraphe suivant.


## II. Utilisation

L'utilisation du script se fait comme suit :

```
$ squaro.sh <commande> [opt] [fichier]
```

Les commandes possibles sont :

### gen :

```
$ squaro.sh gen <taille> fichier.plt
```

Génère aléatoirement un plateau de taille <taille>.  Le résultat est inscrit dans le fichier 'fichier.plt'

     Options :
```
        -v :   verbose mode 
               Affiche le plateau à l'écran
```


### res :

```
$ squaro.sh res [opt] fichier.plt
```

Résous le fichier plateau si une solution existe, et l'affiche à l'écran. Les options possibles sont :

```
     -m : Séléctionne MiniSat pour la résolution
     -v : Ecrit la trace de DPLL dans un fichier "trace.txt"
```

### help :

```
$ squaro.sh help
```

Affiche une aide à l'utilisation


## Exemple de sortie

```
evariste@buntu-studio:~/Bureau/squaro_sat$ ./squaro.sh gen 5 plateau.plt -v
O---O---O---O---O---O 
| 2 | 2 | 2 | 2 | 2 | 
O---O---O---O---O---O 
| 1 | 1 | 0 | 2 | 3 | 
O---O---O---O---O---O 
| 2 | 2 | 1 | 2 | 4 | 
O---O---O---O---O---O 
| 2 | 2 | 1 | 2 | 4 | 
O---O---O---O---O---O 
| 2 | 2 | 2 | 3 | 3 | 
O---O---O---O---O---O 


evariste@buntu-studio:~/Bureau/squaro_sat$ ./squaro.sh res plateau.plt 
  [I] La modélisation ne peut être affichée (trop de clauses) 
  [I] Pour information : 181 clauses 

Le plateau est résoluble :
@---@---@---@---O---@ 
| 2 | 2 | 2 | 2 | 2 | 
O---O---O---O---@---O 
| 1 | 1 | 0 | 2 | 3 | 
O---@---O---O---@---@ 
| 2 | 2 | 1 | 2 | 4 | 
@---O---@---O---@---@ 
| 2 | 2 | 1 | 2 | 4 | 
O---@---O---O---@---@ 
| 2 | 2 | 2 | 3 | 3 | 
@---O---@---@---@---O 
```


