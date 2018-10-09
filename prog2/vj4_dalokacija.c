#include <stdio.h>
#include <stdlib.h>

/*
  DINAMIČKA ALOKACIJA
*/

/* 1. Napišite dio programa koji učitava dva cijela broja, jedan sprema u uobičajen
      (automatski alociran) prostor, drugi u dinamički alociran prostor.
      Ispisuje se veći od dva broja.
*/

void z1()
{
  int x;
  int *y;
  scanf("%d", &x); /* scanf zanima adresa na koju će upisati podatak: adresa na koju zelimo pisati je &x */
  y = (int*) malloc(sizeof(int));
  scanf("%d", y); /* nema &, jer y jest adresa na koju zelimo pisati */
  printf("Veci je %d\n", x > *y ? x : *y);

  free(y);
}

/* 2. Napišite funkciju koja prima broj n > 0, učitava n cijelih brojeva, te ih ispisuje unatrag.
      Koristite d. a. polje. (SAMI: bez polja, koristeći rekurziju)
*/
void unatrag(int n)
{
  int i;
  int *brojevi = (int*) malloc(n * sizeof(int));
  if (brojevi == 0) /* je li alokacija uspjela? */
    return;
  for (i = 0; i < n; ++i)
    scanf("%d", &brojevi[i]); /* moze i "brojevi + i" umjesto "&brojevi[i]" */
  for (i = n - 1; i >= 0; --i)
    printf("%d ", brojevi[i]);

  free(brojevi);
}

/*
  3. Napišite sljedeće funkcije:
    alokacija, koja prima dimenzije a i b, te vraća dinamički alociranu matricu real. br. (u uobičajenom smislu (4))
    dealokacija, koja prima d. a. m. dimenzija a x b, te ju dealocira
    ucitavanje, koja prima d. a. m. dimenzija a x b, te u nju ucitava sve brojeve
    ispis, koja prima d. a. m.  dimenzija a x b, te ispisuje sve brojeve - SAMI
    umnozak, koja prima d. a. m. X iz R^mxn, Y iz R^nxk, te kreira i vraća njihov umnožak
    napišite i primjer poziva
*/

double **alokacija(int a, int b)
{
  int i;
  double **x = (double**) malloc(a * sizeof(double*));
  for (i = 0; i < a; ++i)
    x[i] = (double*)malloc(b * sizeof(double));
  return x;
}

void dealokacija(double **x, int a, int b)
{
  int i;
  for (i = 0; i < a; ++i)
    free(x[i]);
  free(x);
}

void ucitavanje(double **x, int a, int b)
{
  int i, j;
  for (i = 0; i < a; ++i)
    for (j = 0; j < b; ++j)
      scanf("%lf", &x[i][j]);
}

/* SAMI: ispis */

/* Napomena:
    nekada funkcije koje vracaju nizove same trebaju alocirati memoriju za rezultat (kao u ovom zadatku),
    a nekada (npr. u nekim numeričkim bibliotekama) očekuju da će to napraviti pozivatelj

    u oba slučaja se pozivatelj brine o dealokaciji te memorije!
*/
double **umnozak(double **X, double **Y, int m, int n, int k)
{
  int i, j, l;
  double **Z = alokacija(m, k);
  for (i = 0; i < m; ++i)
    for (j = 0; j < k; ++j)
    {
      Z[i][j] = 0;
      for (l = 0; l < n; ++l)
        Z[i][j] += X[i][l] * Y[l][j];
    }
  return Z;
}
/*
 Primjer koristenja:

 double **X = alokacija(m, n);
 double **Y = alokacija(n, k);
 ucitavanje(X, m, n);
 ucitavanje(Y, n, k);
 double **Z = umnozak(X, Y, m, n, k);
 dealokacija(X, m, n);
 dealokacija(Y, n, k);
 dealokacija(Z, m, k);
*/

/*
   Učitajte tablicu **x sastavljenu od prirodnih brojeva.
   Tablica ima m > 0 redaka, svaki redak ima n_i + 1 stupaca,
   a sastoji se od prirodnog broja n_i,
   nakon kojeg slijedi još n_i brojeva (n_i >= 0).
   Primjer ulaza:
      4
      3 1 3 2
      2 4 5
      0
      2 7 6
   Sortirajte retke unutar tablice uzlazno po sumi svih elemenata retka osim prvog (n_i),
     a unutar pojedinog retka (osim prvog stupca), stupce sortirajte uzlazno.
*/
void tablica()
{
  // na sljedećim vježbama
}


/* SAMI: pogledajte zadatak 4.3.5 */


/*
 2. U ovom zadatku cilj je implementirati igru minesweeper

igra se igra na ploči dimenzija m * n; m, n > 1.

    - na početku igra odredi da neke ćelije sadrže minu (-1), a ostale su prazne (-2)
    - igra se sastoji od petlje sa sljedećim koracima:
       1. igrač odabire proizvoljnu ćeliju (i, j)
       2. ako je na (i, j) mina, petlja se prekida
       3. igra u ćeliju (i, j) upisuje broj iz {0, ..., 8},
          broj koliko susjednih ćelija sadrži minu
          ako je taj broj 0, izvršava se 3. korak i za sve prazne (-2) susjedne ćelije


    a) Napišite funkciju koja prima ploču, te ispunjava celije na sljedeci nacin:
          broj -1 s vjerojatnoscu 1/4, (-1 će predstavljati minu)
          broj -2 inače (-2 predsavlja praznu ćeliju)
       uputa: funkcija rand() iz stdlib.h vraća pseudo-slučajan broj između 0 i RAND_MAX
*/

void slucajna(int **a, int m, int n)
{
  int i, j;
  for (i = 0; i < m; ++i)
    for (j = 0; j < n; ++j)
        a[i][j] = rand() % 4 == 0 ? -1 : -2; /* stdlib.h */

}
/*
 * SAMI: Umjesto ovakve implementacije slučajne ploče, dodajte argument k.
 * Matrica a nakon pozivanja mora na tocno k mjesta imati minu.
*/

/*
 b) Napravite ispis stanja ploce: -2 (prazno polje) ispisite kao .,
    -1 (mine) kao *, a nenegativne brojeve ispišite
*/
void ispis(int **a, int m, int n)
{
  int i, j;
  for (i = 0; i < m; ++i)
  {
    for (j = 0; j < n; ++j)
      if (a[i][j] == -2)
        printf(".");
      else if (a[i][j] == -1)
        printf("*");
      else
        printf("%d", a[i][j]);
    printf("\n");
  }
}
/*
* SAMI: Napravite verziju ispisa koja ne otkriva mine. Koristite operator ?: umjesto grananja.
*       Osim toga, neka ta verzija pokraj same ploce ispise indekse redaka i stupaca:
 0123
0....
1....
2....
*/

/*
 c) implementirajte pomocnu funkciju int sadrzan(int m, int n, int i, int j),
    koja provjerava je li (i, j) u granicama polja dimenzije m x n
*/
int sadrzan(int m, int n, int i, int j)
{
  return i >= 0 && i < m && j >= 0 && j < n;
}


/*
 d) implementirajte funkciju int otvaranje(int **a, int m, int n, int r, int s)
    koja radi sljedece:
     - ako je (r, s) izvan granica, vraca -1
     - ako je na (r, s) mina, vraca 1
     - inace, simulira 3. korak iz igre, tj. upisuje broj susjednih mina
       u celiju a[r][s], te po potrebi to ponavlja za susjedna polja

    pripazite da se program korektno izvršava kada postoje dvije susjedne prazne (-2) ćelije a i b:
    ako pozovete funkciju za ćeliju a, ona prije pozivanja sebe za okolne ćelije
    mora promijeniti vrijednost ćelije a; u suprotnom će a pozvati funkciju za b,
    taj će poziv uočiti da je a prazna ćelija pa će pozvati funkciju za a itd.
*/
int otvaranje(int **a, int m, int n, int r, int s)
{
  int i, j, br = 0;

  /* jesmo li izvan ploče? */
  if (!sadrzan(m, n, r, s))
    return -1;

  /* jesmo li unutar ploče, ali stali na minu? */
  if (a[r][s] == -1)
    return 1;

  /* inače nismo stali na minu pa otkrivamo broj okolnih mina: prvo računamo taj broj */
  for (i = r - 1; i <= r + 1; ++i)
    for (j = s - 1; j <= s + 1; ++j)
      if (sadrzan(m, n, i, j) && a[i][j] == -1)
          ++br; /* uocite - nismo morali paziti na slucaj i = r, j = s, zašto? */
  a[r][s] = br; /* bitno je da ova naredba dolazi prije pozivanja funkcije za okolne ćelije! */

  /* potom, ako nije bilo mina u okolini, otvaramo susjedne ćelije */
  if (br == 0)
    for (i = r - 1; i <= r + 1; ++i)
      for (j = s - 1; j <= s + 1; ++j)
        if (sadrzan(m, n, i, j) && a[i][j] == -2)
            otvaranje(a, m, n, i, j);

  return 0;
}

/*
 SAMI: iskoristite gornje funkcije kako biste završili implementaciju igre.
*/



int main()
{

  return 0;
}
