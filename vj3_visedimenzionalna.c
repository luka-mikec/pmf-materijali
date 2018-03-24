#include <stdio.h>
#include <stdlib.h>

/*
 U komentarima iznad nekih zadataka su sa zvjezdicom označene varijacije
 zadataka (za samostalno rješavanje) koje nisu spomenute na vježbama.
*/


/* 1. Napišite funkciju suma koja prima polje realnih brojeva dimenzije m x n,
   1 <= m, n <= 10, te vraća sumu svih elemenata. */
double suma(double a[10][10], int m, int n)
{
  int i, j;
  double s = 0;
  for (i = 0; i < m; ++i)   /* kroz svaki redak */
    for (j = 0; j < n; ++j) /* kroz svaki stupac */
      s += a[i][j];
  return s;
}
/*
  SAMI:  implementirajte funkciju koja ispisuje danu matricu, 1 <= m, n <= 10
         (uočite da funkcija ispravno radi samo za matrice deklarirane s 10 stupaca)
  SAMI*: koristeći pokazivače, implementirajte
*/


/* 2. Napišite sljedeće funkcije koje sve primaju _kvadratnu_ matricu realnih brojeva
      dimenzija m x m, 1 <= m <= 10, te: */

/* a) vraća trag matrice. */
double trag(double a[10][10], int m)
{
  int i;
  double s = 0;
  for (i = 0; i < m; ++i)
      s += a[i][i];
  return s;
}
/*
  SAMI: napišite rješenje u formi rješenja prvog zadatka
*/

/* b) provjerava je li matrica donje-trokutasta (sve iznad glavne dijagonale mora biti 0) */
int dt(double a[10][10], int m)
{
  int i, j;
  for (i = 0; i < m; ++i) /* kroz svaki redak, ali može i "< m - 1"  */
    for (j = i + 1; j < m; ++j) /* kroz svaki stupac koji nas zanima; ponovno "m" jer je kvadratna matrica */
      if (a[i][j] != 0)
        return 0;
  return 1;
}

/* c) upisuje 0 na sporednu dijagonalu */
void sporedna(double a[10][10], int m)
{
  int i;
  for (i = 0; i < m; ++i)
      a[i][m - i - 1] = 0;
}

/* d) prima indekse i0, j0, te upisuje 1 u i0-ti redak, j0-ti stupac, a drugdje 0 */
void ispuna(double a[10][10], int m, int i0, int j0)
{
  int i, j;
  for (i = 0; i < m; ++i) /* kroz svaki redak */
    for (j = 0; j < m; ++j) /* kroz svaki stupac, kao i ranije, "m" jer je kvadratna matrica */
      a[i][j] = i == i0 || j == j0 ? 1 : 0;
}
/*
  SAMI: slicno, ali umjesto retka i stupca, dijagonale (razmislite koji uvjeti mora vrijediti, umjesto i = i0 v j = j0)
  npr. za i0 = 1, j0 = 1,
  x x x       1 0 1
  x x x  |->  0 1 0
  x x x       1 0 1
*/


/* e) vraca indeks _stupca_ s najvecom sumom (ako ih je vise, onda prvi takav indeks). */
int max_stupac(double a[10][10], int m)
{
  int i, j, max_ind = 0;
  double max_s, s;
  for (j = 0; j < m; ++j) /* kroz svaki STUPAC -
                             korisno promijeniti ime varijable prve petlje u j da to ne zaboravimo;
                             uz to, da matrica nije bila kvadratna, pisalo bi < n, ne m. */
  {
    s = 0;
    for (i = 0; i < m; ++i) /* kroz sve celije unutar stupca - tj. kroz sve retke */
      s += a[i][j]; /* uocite, kod indeksiranja ostaje [i][j] poredak - jer 'i' nam i dalje oznacava redak, j stupac */
    if (j == 0 || s > max_s)
    {
      max_ind = j;
      max_s = s;
    }
  }
  return max_ind;
}
/* SAMI: umjesto vracanja jednog indeksa, neka funkcija primi i niz b,
   te postavi vrijednost b[j] na 1 ako j-ti stupac ima najvecu sumu, inace 0, za sve j */


int main()
{

  return 0;
}
