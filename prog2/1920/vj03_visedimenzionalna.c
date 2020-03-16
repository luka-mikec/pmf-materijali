#include "stdio.h"

/*
  Višedimenzionalno polje je polje sastavljeno od drugih polja.
  Deklaracija n-dimenzionalnog polja:  tip ime[n_1][n_2]...[n_k]
  To je polje sastavljeno od n_1 manjih polja, gdje je svako manje polje polje od n_2 (još) manjih polja itd.

  Dvodimenzionalno polje  tip ime[m][n]  je polje sastavljeno od m manjih polja, 
  gdje je svako manje polje neko n-člano polje cijelih brojeva.

  Dvodimenzionalno  tip ime[m][n]  polje često shvaćamo kao matricu,
  vrijednost m tada obično interpretiramo kao broj redaka, a vrijednost n kao broj stupaca matrice.

  Primjerice  int A[2][5]  je polje od dva retka i pet stupaca, te je svaka ćelija cijeli broj.

  Jezik C promatra takvo polje A kao niz duljine 2, sastavljen od nizova cijelih brojeva duljine 5,
  npr. A[1] je drugi redak, A[0][4] je peti element u prvom retku (peti stupac u prvom retku)

  U memoriji, ćelije dvodimenzionalnog polja A ovako su poredane (bez memorijskog prostora između ćelija):
         A[0][0] A[0][1] A[0][2] A[0][3] A[0][4] A[1][0] A[1][1] A[1][2] A[1][3] A[1][4]

  Ovakav memorijski raspored je tzv. row-major konvencija; C i većina modernih jezika koristi takvu konvenciju.
  Iznimka je Fortran i jezici povijesno vezani uz Fortran ili Fortranove biblioteke (npr. MATLAB/Octave)

  Za niz  int A[n], izraz A[i] ekvivalentan je izrazu *((int*)A + i). Objašnjenje tog izraza:
    - A u jeziku C odjednom označava i samo polje, i memorijsku adresu na kojoj počinju elementi polja A
    - s (int*) specificiramo da tu adresu želimo promatrati kao adresu cijelog broja (int)
    - (int*)A + i  znači "memorijska adresa za i mjesta (intova) udesno od adrese A, 
                          ponovno promatrana kao adresa nekog cijelog broja"
                   (operator + u jeziku C u situaciju poput ove, kad se s lijeve strane operatora nalazi memorijska adresa, 
                    tj. pointer/pokazivač, ima takvo ponašanje koje je drugačije od uobičajenog zbrajanja brojeva)               
    - *((int*)A + i) znači "sadržaj na koji pokazuje (int*)A + i" (kako je tip izraza  (int*)A + i  pokazivač na cijeli broj,
      onda je "sadržaj" u ovom slučaju cijeli broj)
    - uočite da u ovom računu nema varijable n---zbog toga u deklaraciji funkcije koja prima niz nije potrebno pisati int A[n],
      C može sam izračunati lokaciju i-tog elementa u memoriji bez da zna gdje elementi završavaju 
      (zna gdje i-ti element počinje, i zna da su svi objekti tipa int duljine npr. 4 bajta, točan broj ovisi o platformi)
  Za matricu int A[m][n], izraz A[i][j] isto je što i *((int*)A + n * i + j)
    - potrebno je znati (samo) drugu dimenziju, tj. broj n
    - općenito, za k-dimenzionalno polje moramo znati sve dimenzije osim prve,
      pa je sve te dimenzije (barem posljednjih k - 1, alternativno svih k) 
      potrebno uključiti u tip polja u deklaraciji funkcije.
    - npr. šaljemo li trodimenzionalno polje dimenzija 6x5x4 funkciji, deklaracija bi izgledala ovako:
        povratni_tip f(..., tip A[6][5][4]), ili
        povratni_tip f(..., tip A[][5][4])

  Napomena: pod "višedimenzionalna polja" u C-u smo do sada podrazumijevali strukture oblika  tip ime[n_1][n_2]...[n_k].
  Ponekad (kasnije na kolegiju) mislimo i na drugačiju strukturu podataka od te.
  Za tu drugačiju strukturu neće vrijediti A[i][j] == *((int*)A + n * i + j), jer će tamo memorijski raspored biti drugačiji
  (stupci će slijediti jedan za drugim, ali bit će dopušten neiskorišten prostor između redova)

  Napomena: ako se u zadatku traži da je k-ta dimenzija (gdje k nije prva dimenzija) nekog polja __najviše__ x,
  onda x mora biti prisutan u tipovima svih varijabli koje će pamtiti to polje.
  Primjerice:
     void f(int A[][100], int m, int n) { ... }  // ako u zadatku pise da ce biti najvise 100 stupaca
     ...
     int main() {
       ...
       int p[2][2]   = {{0, 1}, {2, 3}}; --- pogresno, unatoc tome sto p "zapravo" ima 2 stupca
       int p[2][100] = {{0, 1}, {2, 3}}; --- ispravno 
       f(p, 2, 2);
       ...
     }
 */

/* 1. Napišite funkciju suma koja prima polje realnih brojeva dimenzije m x n, 1 <= m <= 10, 1 <= n <= 15, te vraća sumu svih elemenata. */
double suma(double a[10][15], int m, int n)
{
    int i, j;
    double s = 0;
    for (i = 0; i < m; ++i)   /* kroz svaki redak */
        for (j = 0; j < n; ++j) /* kroz svaki stupac */
            s += a[i][j];
    return s;
}
/*
  ZADATAK: implementirajte funkciju koja može poslužiti za ispis matrica s do 15 stupaca
  Donja dva zadatka su za slučaj da želite isprobati pristup memoriji pokazivačima (pointerima).
  U ovom trenutku na kolegiju to još nije bitno, ali uskoro će postati bitno.
  ZADATAK (teže): implemenetirajte funkciju koja može poslužiti za ispis matrica s do 15 stupaca,
                  ali na način da matricu primite kao double*, tj.  void ispis(double *matrica, int m, int n)
  ZADATAK (teže): implementirajte još jednom sličnu funkciju, ali ovaj put bez pretpostavke da je najviše 15 stupaca;
                  uočite da vam kao argument treba i stvaran (da znamo koliko stupaca ispisati)
                  i maksimalan broj stupaca (zbog računanja memorijske pozicije pojedine ćelije).
*/


/* 2. Napišite odgovarajuću funkciju koja prima matricu realnih brojeva dimenzija m x m, 1 <= m <= 10, te: */


/* a) vraća trag matrice. */
double trag(double a[10][10], int m)
{
    int i;
    double s = 0;
    for (i = 0; i < m; ++i)
        s += a[i][i];
    return s;
}

/* b) provjerava je li matrica donje-trokutasta (sve iznad glavne dijagonale mora biti 0) */
int dt(double a[10][10], int m)
{
    int i, j;
    for (i = 0; i < m; ++i) /* kroz svaki redak */
        for (j = i + 1; j < m; ++j) /* kroz svaki stupac s indeksom većim od indeksa retka, ponovno "m" jer je matrica kvadratna */
            if (a[i][j] != 0)
                return 0; /* 0: matrica nije kvadratna */
    return 1; /* 1: matrica je kvadratna */
}

/* c) upisuje 0 na sporednu dijagonalu */
void sporedna(double a[10][10], int m)
{
    int i;
    for (i = 0; i < m; ++i)
        a[i][m - i - 1] = 0;
}


/* d) prima indekse i0, j0, te upisuje broj 1 u i0-ti redak, j0-ti stupac, a drugdje 0 */
void ispuna(double a[10][10], int m, int i0, int j0)
{
    int i, j;
    for (i = 0; i < m; ++i) /* kroz svaki redak */
        for (j = 0; j < m; ++j) /* kroz svaki stupac, kao i ranije, "m" jer je kvadratna matrica */
            a[i][j] = i == i0 || j == j0 ? 1 : 0;
}
/*
  ZADATAK: riješite isti zadatak bez upisivanja broja 0, te petljama prođite samo kroz ćelije kroz koje morate proći

  ZADATAK: riješite zadatak d), ali umjesto retka i stupca, jedinicama ispunite dijagonale koje prolaze ćelijom (i0, j0)
  (razmislite koji uvjet vrijedi o ćelijama u koje treba doći 1, umjesto "i = i0 ili j = j0")
  npr. za i0 = 2, j0 = 1,
  x x x x       0 0 0 1
  x x x x  |->  1 0 1 0
  x x x x       0 1 0 0
  x x x x       1 0 1 0
*/



/* e) vraća indeks stupca s najvecom sumom (ako ih je više, onda prvi takav indeks). */
int max_stupac(double a[10][10], int m)
{
    int i, j, max_ind;
    double max_s;
    for (j = 0; j < m; ++j) /* prolazimo kroz svaki STUPAC,
                               pa je korisno promijeniti ime varijable prve petlje u j da to ne zaboravimo;
                               također, da ova matrica nije bila kvadratna, pisalo bi "< n", ne "< m". */
    {
        double s = 0;
        for (i = 0; i < m; ++i) /* kroz sve ćelije unutar stupca---tj. kroz sve retke */
            s += a[i][j];       /* uočite, kod indeksiranja, 'i' nam i dalje oznacava redak, a 'j' stupac */
        if (j == 0 || s > max_s)
        {
            max_ind = j;
            max_s = s;
        }
    }
    return max_ind;
}

/* ZADATAK: umjesto vraćanja jednog indeksa, neka funkcija primi i niz b,
   te postavi vrijednost b[j] na 1 ako i samo ako j-ti stupac ima najveću sumu, inace b[j] treba biti 0 */


/* f) transponira matricu. */
void transp(double a[10][10], int m)
{
    int i, j;
    for (i = 0; i < m; ++i) /* kroz svaki redak */
        for (j = 0; j < m; ++j) /* kroz svaki stupac */
        {
            double tmp = a[i][j];
            a[i][j] = a[j][i];
            a[j][i] = tmp;
        }
}



int main(int argc, char const *argv[]) {
    return 0;
}
