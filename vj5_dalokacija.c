#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
   (zadatak s prethodnih vjezbi)
   Učitajte tablicu **x sastavljenu od prirodnih brojeva.
   Tablica ima m > 0 redaka (korisnik unosi na početku), svaki redak ima n_i + 1 stupaca,
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
  int **x; /* tablica */
  int m;   /* redaka */
  int i, j, k; /* brojaci */

  scanf("%d", &m);
  x = (int**) malloc(m * sizeof(int*));
  for (i = 0; i < m; ++i)
  {
    int n;
    scanf("%d", &n);
    x[i] = (int*)malloc((n + 1) * sizeof(int));
    x[i][0] = n;
    for (j = 1; j < n + 1; ++j)
      scanf("%d", &x[i][j]);
  }

  /* sortiranje unutar redaka: */
  for (i = 0; i < m; ++i)
  {
    int n = x[i][0]; /* ovaj n je n_i iz zadatka - to znači da ovaj redak ima n + 1 stupac! */
    for (j = 1; j < n + 1; ++j)
      for (k = j + 1; k < n + 1; ++k)
        if (x[i][j] > x[i][k])
        {
          int t = x[i][j];
          x[i][j] = x[i][k];
          x[i][k] = t;
        }
  }

  /* sortiranje tablice, tj. pojedinih redaka: */
  for (i = 0; i < m; ++i)
    for (j = i + 1; j < m; ++j)
    {
      int suma_i = 0, suma_j = 0;
      for (k = 1; k < x[i][0] + 1; ++k)
        suma_i += x[i][k];
      for (k = 1; k < x[j][0] + 1; ++k)
        suma_j += x[j][k];
      if (suma_i > suma_j)
      {
        int *t = x[i];
        x[i] = x[j];
        x[j] = t;
      }
    }

  /* ispis i dealokacija */
  for (i = 0; i < m; ++i)
  {
    for (j = 0; j < x[i][0] + 1; ++j)
      printf("%d ", x[i][j]);
    printf("\n");
    free(x[i]);
  }
  free(x);
}


/*
 SAMI - pogledajte zadatak 4.3.5 (skripta) i minesweeper (materijali za prethodne vježbe)
*/



/*
Ponekad je korisno promijeniti veličinu alocirane memorije.

Realokacija D. A. memorije M čija je adresa u (pokazivaču) a uključuje sljedeće:
 - alokacije nove memorije M2
 - kopiranje M u M2 (samo početnog dijela, ako M2 zauzima manje prostora)
 - dealokacija M
 - spremanje adrese od M2 u pokazivač a

Sve to radi naredba:
 a = (tip*) realloc(a, nova_velicina_u_bajtovima);
ako je _prije_ poziva a == 0, realloc se ponaša kao malloc.

Moguće je da realloc neke korake preskoči (npr. ako je nova veličina ista kao stara, nije potrebno ništa napraviti), 
no C ne garantira niti "očito" izvedive optimizacije
  - zato je uvijek bitno spremiti novu adresu!

1. Implementirajte funkciju void* realok(void* ulaz, size_t n, size_t m) koja simulira funkciju realloc,
   gdje je n stara veličina, m nova.
   Kopiranje izvedite sami, bajt-po-bajt.
   Tip size_t je cjelobrojni tip definiran u stdlib.h (najčešće samo drugo ime za unsigned long).

   napomena: realloc ne treba naš parametar n - postoji evidencija veličina sakrivena od korisnika.
*/
void* realok(void* ulaz, size_t n, size_t m)
{
  int i;
  void *nova = malloc(m);
  for (i = 0; i < (n < m ? n : m); ++i)
    ((char*)nova)[i] = ((char*)ulaz)[i];
  free(ulaz);
  return nova;
}

/*
2. Napišite funkciju koja učitava nenegativne realne brojeve dok ne učita nulu.
Funkcija treba ispisati one učitane brojeve koji su strogo veći od geometrijske sredine učitanih brojeva.
Koristite pow iz math.h.

Uočite: ne možemo unaprijed znati koji brojevi ispunjavaju kriterij za ispis, pa ih treba pamtiti.
*/
void sredina()
{
  double *brojevi = 0; /* bitno - realloc će se na poziv s 0 ponašati kao malloc */
  int n = 0;
  double sredina; int i; /* za racun sredine */
  double ulaz;
  do
  {
    scanf("%lf", &ulaz);
    if (ulaz > 0)
    {
      ++n;
      brojevi = (double*) realloc(brojevi, n * sizeof(double));
      brojevi[n - 1] = ulaz;
    }
  } while (ulaz != 0);

  sredina = 1;
  for (i = 0; i < n; ++i)
    sredina *= brojevi[i]; /* mogli smo i kod ucitavanja */
  sredina = pow(sredina, 1. / n);

  for (i = 0; i < n; ++i)
    if (brojevi[i] > sredina)
      printf("%lf ", brojevi[i]);

  free(brojevi);
}

/*
 3. Napišite funkciju koja učitava (int) "tablicu".
    Svaki je redak neprazan.
    Pojedini redak se učitava dok korisnik ne upiše 0 (spremite i nju u redak, da znate gdje redak staje).
    Unos praznog retka, tj. 0 na početku učitavanja retka, označava kraj učitavanja tablice.
    Ako tablica ima (ne)paran broj redaka, izbacite sve retke s (ne)parnim indeksom.

    Ispišite tablicu.

    (Korisnik ni u jednom trenutku ne unosi očekivan broj redaka i/ili stupaca!)
*/
void tablica2() /* (u ovom fileu već imamo funkciju "tablica") */
{
  int **t = 0;
  int n, m,  i, j,  p,  ulaz;

  m = 0;
  do /* ucitavanje redaka */
  {
    n = 0;
    do /* ucitavanje stupaca */
    {
      scanf("%d", &ulaz);
      if (n == 0 && ulaz != 0) /* novi redak - tj., novi član niza t pokazivača na retke? */
      {
        ++m;
        t = (int**) realloc(t, m * sizeof(int*));
        t[m - 1] = 0;
      }
      if (n > 0 || ulaz != 0) /* novi stupac? */
      {
        ++n;
        t[m - 1] = (int*) realloc(t[m - 1], n * sizeof(int));
        t[m - 1][n - 1] = ulaz;
      }
    } while (ulaz != 0);


  } while (n > 0);

  p = m % 2;
  for (i = m - 1; i >= 0; --i) /* SAMI - izvedite ovo brisanje u jednoj for-petlji */
    if (i % 2 == p)
    {
      free(t[i]);
      for (j = i; j < m - 1; ++j)
        t[j] = t[j + 1];
      --m;
    }

  t = (int**) realloc(t, m * sizeof(int*));

  for (i = 0; i < m; ++i)
  {
    printf("\n");
    for (j = 0; t[i][j] > 0 ; ++j)
      printf("%d ", t[i][j]);
    free(t[i]);
    free(t);
  }
}

/* Dodatan zadatak (probajte prvo sami):

   Modificirajte gornje rješenje, tako da još iz svakog retka s (ne)parnim brojem članova
   (ne računajte 0 kao član) izbacite (ne)parne brojeve.

   Jedno rješenje (koje se umeće na kraj prve petlje za učitavanje,
   tj. kad znamo da je učitan cijeli redak, a indeks trenutnog retka je m - 1) je u komentarima.
 */


/*

if (n > 0)
{
  --n; // unutar ovog if-a, korisnije je o n razmišljati kao o broju stupaca bez završne signalne nule
  p = n % 2;
  for (i = n - 1; i >= 0; --i) // prolazimo stupcima u trenutnom retku (signalne nule "nema", pa uobičajeno krećemo od n - 1)
    if (t[m - 1][i] % 2 == p)
    {
      for (j = i; j < n - 1; ++j)
        t[m - 1][j] = t[m - 1][j + 1];
      --n;
    }
  t[m - 1][n] = 0; // upisujemo nulu na kraj retka

  // konačno, reduciramo veličinu polja; ispod piše n + 1 zbog nule na kraju retka
  // (ovaj n je najviše (stari n) - 1, tako da donja realokacija sigurno ne povećava polje)
  t[m - 1] = (int*) realloc(t[m - 1], (n + 1) * sizeof(int));
}

*/


int main()
{

  return 0;
}
