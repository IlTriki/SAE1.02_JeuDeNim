#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* variables des lignes et colonnes max et min du tableau: */
#define VMIN 5
#define VMAX 30

/* type qui represente une case de la grille : */
struct T_Case {
  int n_lig, n_col;
};
/* declaration des fonctions qu'on utilisera: */
/* fonction qui affiche le tableau du jeu : */
void AfficheTab(int n_elem, int n_col, struct T_Case Pion, struct T_Case *Tab);
/* fonction qui donne les coordonées à chaque element du tableau: */
void RempCoord(int n_col, int n_lig, struct T_Case *Tab);
/* fonction qui verifie si les coordonnées d'une case sont valide (dans le
 * tableau ou pas): */
int DansTab(int n_col, int n_lig, int c, int l);
/* fonction qui construit la table des cases voisines d’une case donnée: */
int Voisines(int n_col, int n_lig, struct T_Case *Tab, struct T_Case Case);
/* fonction qui calcule le nimber d’une case donnée: */
int Nimber(int n_col, int n_lig, struct T_Case Pion);
/* fonction qui verifie si le nombre de colonnes et lignes est valide (c'est à
 * dire entre 5 et 30): */
int Lire_Entier(int inf, int sup);
/* fonction qui demande et renvoie la difficulté choisie par le joueur : */
void Difficulte(int *diff);
/* Fonction pour voir qui commence: */
void Next(int *next);
/* fonction qui retourne un nombre au hasard compris entre deux valeurs données:
 */
int Hasard(int imin, int imax);
/* une fonction qui gère le coup d’un joueur (cette fonction propose au joueur
 * de choisir un coup parmi la liste des coups possibles): */
struct T_Case Coup_joueur(int n_elem, int nb_vois, int ncol, struct T_Case Pion, struct T_Case *Tab, struct T_Case *vois);
/* une fonction qui gère le coup de l'ordi si le coup est un coup au hasard: */
struct T_Case Coup_Ordi_Hasard(int n_elem, int nb_vois, int ncol, struct T_Case Pion, struct T_Case *Tab, struct T_Case *vois);
/* une fonction qui gère le coup de l'ordi si le coup doit etre si possible
 * gagnant: */
struct T_Case Coup_Ordi_Gagnant(int n_elem,int nlig, int nb_vois, int ncol, struct T_Case Pion, struct T_Case *Tab, struct T_Case *vois);

int main() {
  /* declaration des variables que nous allons utiliser : */
  /*dans l'ordre : iterateur , nombre de lignes et de colonnes saisie par le
   * joueur, nombre d'elements de la grille, nombre des cases voisines: */
  int i, nlig, ncol, n_elem = 0, nb_vois;
  /* choix du joueur pour deplacer le pion: */
  int choix;
  /* Niveau de difficulté du jeu (1 ou 2 ou 3 ou 4): */
  int niveau;
  /* 1 si c'est l'ordinateur qui commence, 2 si c'est le joueur: */
  int next;
  /* un tableau à 1 dimension de T_Case: */
  struct T_Case T_Tab_Case[VMAX * VMAX];
  /* position du pion */
  struct T_Case Pion;
  /* tableau des cases voisines */
  struct T_Case vois[4];

  srand((unsigned int)time(NULL));
  printf("Paramètres du jeu :\n");
  printf("nombre de lignes(entre 5 et 30)   : ");
  nlig = Lire_Entier(VMIN, VMAX);
  printf("nombre de colonnes(entre 5 et 30) : ");
  ncol = Lire_Entier(VMIN, VMAX);
  n_elem = ncol * nlig;
  Pion.n_lig = 1;
  Pion.n_col = 1;
  Difficulte(&niveau);
  printf("qui commence ?Saisir 1 si c'est \n\033[0;31ml’ordinateur\033[0m ou 2 si c'est \033[0;34mvous\033[0m : ");
  Next(&next);
  RempCoord(ncol, nlig, T_Tab_Case);
  printf("\nC'est parti !\n\n");
  AfficheTab(n_elem, ncol, Pion, T_Tab_Case);
  while ((Pion.n_col != T_Tab_Case[n_elem - 1].n_col) || (Pion.n_lig != T_Tab_Case[n_elem - 1].n_lig)) {
    nb_vois = Voisines(ncol, nlig, vois, Pion);
    if (next == 1) {
      switch (niveau) {
      case 1: /* L'ordinateur joue au hasard */
        Pion = Coup_Ordi_Hasard(n_elem, nb_vois, ncol, Pion, T_Tab_Case, vois);
        break;
      case 2: /* L'ordinateur joue 1/3 fois gagnant */
        choix = Hasard(1, 3);
        if (choix == 1)
          Pion = Coup_Ordi_Gagnant(n_elem, nlig, nb_vois, ncol, Pion, T_Tab_Case, vois);
        else
          Pion = Coup_Ordi_Hasard(n_elem, nb_vois, ncol, Pion, T_Tab_Case, vois);
        break;
      case 3: /* L'ordinateur joue 2/3 fois gagnant */
        choix = Hasard(1, 3);
        if (choix == 1)
          Pion = Coup_Ordi_Hasard(n_elem, nb_vois, ncol, Pion, T_Tab_Case, vois);
        else
          Pion = Coup_Ordi_Gagnant(n_elem, nlig, nb_vois, ncol, Pion, T_Tab_Case, vois);
        break;
      default: /* L'ordinateur joue joujours gagnant */
        Pion = Coup_Ordi_Gagnant(n_elem, nlig, nb_vois, ncol, Pion, T_Tab_Case, vois);
        break;
      }
      next++;
    } else {
      Pion = Coup_joueur(n_elem, nb_vois, ncol, Pion, T_Tab_Case, vois);
      next--;
    }
  }
  if (next == 1)
    printf("\nC’est terminé. \033[0;34mBRAVO TU AS GAGNE !\033[0m\n");
  else
    printf("\nHAHA! Tu as perdu contre un \033[0;31mordinateur\033[0m\n");
}

void AfficheTab(int n_elem, int n_col, struct T_Case Pion, struct T_Case *Tab) {
  int i, n_lignes = 1;
  for (i = 0; i < n_elem; i++) {
    if ((Pion.n_lig == Tab[i].n_lig) && (Pion.n_col == Tab[i].n_col))
      printf("|0");
    else
      printf("|-");
    if ((n_col * n_lignes) == (i + 1)) {
      printf("|\n");
      n_lignes++;
    }
  }
}

void RempCoord(int n_col, int n_lig, struct T_Case *Tab) {
  int x, y, elem = 0;
  for (y = 1; y <= n_lig; y++) {
    for (x = 1; x <= n_col; x++) {
      Tab[elem].n_lig = y;
      Tab[elem].n_col = x;
      elem++;
    }
  }
}

int DansTab(int n_col, int n_lig, int c, int l) {
  if (c <= n_col && l <= n_lig)
    return 1;
  else
    return 0;
}

int Voisines(int n_col, int n_lig, struct T_Case *Tab, struct T_Case Case) /*voir explication dans la declaration de la fonction*/
{
  int nb_vois = 0;
  if (DansTab(n_col, n_lig, Case.n_col, Case.n_lig + 1)) {
    Tab[nb_vois].n_col = Case.n_col;
    Tab[nb_vois].n_lig = Case.n_lig + 1;
    nb_vois++;
    if (DansTab(n_col, n_lig, Case.n_col, Case.n_lig + 2)) /* on regarde si la case +2 est valide seulement si la case +1 est valide*/
    {
      Tab[nb_vois].n_col = Case.n_col;
      Tab[nb_vois].n_lig = Case.n_lig + 2;
      nb_vois++;
    }
  }
  if (DansTab(n_col, n_lig, Case.n_col + 1, Case.n_lig)) {
    Tab[nb_vois].n_col = Case.n_col + 1;
    Tab[nb_vois].n_lig = Case.n_lig;
    nb_vois++;
    if (DansTab(n_col, n_lig, Case.n_col + 2, Case.n_lig)) /* meme chose qu'avant */
    {
      Tab[nb_vois].n_col = Case.n_col + 2;
      Tab[nb_vois].n_lig = Case.n_lig;
      nb_vois++;
    }
  }
  return nb_vois;
}

int Nimber(int n_col, int n_lig, struct T_Case Pion) /*voir explication dans la declaration de la fonction*/
{
  int nimb;
  int ecartB, ecartC;
  ecartB = (n_lig % 3) - (n_col % 3);
  ecartC = (Pion.n_lig % 3) - (Pion.n_col % 3);
  if (ecartB != ecartC) nimb = 1;
  else nimb = 0;
  return nimb;
}

int Lire_Entier(int inf, int sup) /*voir explication dans la declaration de la fonction*/
{
  int n, nok;
  do {
    scanf("%i", &n);
    nok = (n >= inf) && (n <= sup);
    if (!nok) {
      printf("Error ! Repeter la saisie !\n");
    }
  } while (!nok);
  return n;
}

void Difficulte(int *diff) /*voir explication dans la declaration de la fonction*/
{
  printf("niveau de difficulté de 1 à 4 : \n");
  printf("\033[0;31m(1) niveau débutant\033[0m : l’ordinateur joue toujours au hasard.\n");
  printf("\033[0;32m(2) niveau moyen\033[0m    : l’ordinateur joue un coup au hasard avec la probabilité 2/3 ou un coup gagnant avec la probabilité 1/3.\n");
  printf("\033[0;33m(3) niveau expert\033[0m   : l’ordinateur joue un coup au hasard avec la probabilité 1/3 ou un coup gagnant avec la probabilité 2/3.\n");
  printf("\033[0;34m(4) niveau virtuose\033[0m : l’ordinateur joue toujours gagnant.\n");
  printf("Saisir le niveau de difficulté que vous voulez : \n");
  *diff = Lire_Entier(1, 4);
  printf("Vous avez choisie le ");
  if (*diff == 1)
    printf("\033[0;31m(1) niveau débutant\033[0m\n");
  else if (*diff == 2)
    printf("\033[0;32m(2) niveau moyen\033[0m\n");
  else if (*diff == 3)
    printf("\033[0;33m(3) niveau expert\033[0m\n");
  else
    printf("\033[0;34m(4) niveau virtuose\033[0m\n");
}

void Next(int *next) /*voir explication dans la declaration de la fonction*/
{
  do {
    scanf("%i", next);
    if (*next != 1 && *next != 2)
      printf("Vous n'avez pas saisie un numero entre 1 et 2.\nRepeter la saisie !\n");
  } while (*next != 1 && *next != 2);
}

int Hasard(int imin, int imax) /*voir explication dans la declaration de la fonction*/
{
  return (rand() % (imax - imin + 1) + imin);
}

struct T_Case Coup_joueur(int n_elem, int nb_vois, int ncol, struct T_Case Pion, struct T_Case *Tab, struct T_Case *vois) /*voir explication dans la declaration de la fonction*/
{
  int i, choix;
  if (nb_vois == 1) {
    printf("\n\033[0;34mA toi de jouer !\033[0m\nSeule possibilité : (%d,%d)\nvalider --->", vois[0].n_lig, vois[0].n_col);
    getchar();
    Pion.n_lig = vois[0].n_lig;
    Pion.n_col = vois[0].n_col;
  } else {
    printf("\n\033[0;34mA toi de jouer !\033[0m\nchoisir la destination");
    for (i = 1; i <= nb_vois; i++)
      printf(" %d:(%d,%d)", i, vois[i - 1].n_lig, vois[i - 1].n_col);
    do {
      printf("\n-->");
      scanf("%d", &choix);
      if ((choix > nb_vois) || (choix < 1))
        printf("erreur !");
    } while ((choix > nb_vois) || (choix < 1));
    Pion.n_col = vois[choix - 1].n_col;
    Pion.n_lig = vois[choix - 1].n_lig; /* on met à jour la position du pion */
  }
  printf("\n");
  AfficheTab(n_elem, ncol, Pion, Tab);
  return Pion;
}

struct T_Case Coup_Ordi_Hasard( int n_elem, int nb_vois, int ncol, struct T_Case Pion, struct T_Case *Tab, struct T_Case *vois) /*voir explication dans la declaration de la fonction*/
{
  int choix;
  choix = Hasard(0, nb_vois - 1);
  Pion.n_col = vois[choix].n_col;
  Pion.n_lig = vois[choix].n_lig;
  printf("\nL’\033[0;31mordinateur\033[0m déplace le pion en (%d,%d)\n", Pion.n_lig, Pion.n_col);
  AfficheTab(n_elem, ncol, Pion, Tab);
  printf("\n");
  return Pion;
}

struct T_Case Coup_Ordi_Gagnant( int n_elem,int nlig, int nb_vois, int ncol, struct T_Case Pion, struct T_Case *Tab, struct T_Case *vois) /*voir explication dans la declaration de la fonction*/
{
  int choix, i;
  for (i = 0; i < nb_vois; i++) {
    if (Nimber(ncol, nlig, vois[i]) == 0) {
      choix = i;
      Pion.n_col = vois[choix].n_col;
      Pion.n_lig = vois[choix].n_lig;
      printf("\nL’\033[0;31mordinateur\033[0m déplace le pion en (%d,%d)\n", Pion.n_lig, Pion.n_col);
      AfficheTab(n_elem, ncol, Pion, Tab);
      return Pion;
    }
  }
  choix = Hasard(0, nb_vois - 1);
  Pion.n_col = vois[choix].n_col;
  Pion.n_lig = vois[choix].n_lig;
  printf("\nL’\033[0;31mordinateur\033[0m déplace le pion en (%d,%d)\n", Pion.n_lig, Pion.n_col);
  AfficheTab(n_elem, ncol, Pion, Tab);
  return Pion;
}