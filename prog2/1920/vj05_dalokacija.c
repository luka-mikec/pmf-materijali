#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 5. Napišite funkciju imena tablica koja radi sljedeće.
    Učitajte tablicu **x sastavljenu od prirodnih brojeva.
    Tablica ima m > 0 redaka (korisnik unosi m na početku), svaki redak ima n_i + 1 ćelija,
    prva ćelija je sam broj n_i, nakon kojeg slijedi još n_i brojeva (n_i >= 0).
    Primjer ulaza:
      4
      3 1 3 2
      2 4 5
      0
      2 7 6
    Sortirajte retke unutar tablice uzlazno po sumi elemenata retka (osim prvog elementa, prvi element je broj n_i),
    a unutar pojedinog retka, stupce (osim prvog stupca) sortirajte uzlazno.
    Dakle, tablica bi nakon sortiranja (naravno, prije dealokacije) trebala izgledati ovako:
      0
      3 1 2 3
      2 4 5
      2 6 7
    Ispišite tablicu na način da sadržaj pojedinog retka tablice ispišete u zasebnom retku (kao u ovom tekstu zadatka).
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
        scanf("%d", &n); /* prvo učitamo n_i */
        x[i] = (int*)malloc((n + 1) * sizeof(int));
        x[i][0] = n;
        for (j = 1; j < n + 1; ++j)
            scanf("%d", &x[i][j]);
    }

    /* sortiranje unutar redaka, tj. pojedinih ćelija: */
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
    ZADATAK: na papiru skicirajte što se događa u memoriji prilikom izvođenja gornjeg koda.
*/

/*
Ponekad je korisno promijeniti veličinu alocirane memorije.

Realokacija dinamički alocirane memorije M čija je adresa u pokazivaču nazvanom pok uključuje sljedeće:
 - alokaciju nove memorije M2
 - kopiranje sadržaja memorije M u prostor od M2 (možda samo početnog dijela, ovisno o tome koliko podataka stane u M2)
 - dealokacija memorije M
 - spremanje adrese memorije M2 u pokazivač pok, jer općenito M2 i M nisu na istoj memorijskoj adresi

Sve to radi naredba sljedećeg oblika:
    pok = (tip*) realloc(pok, nova_velicina_u_bajtovima);
ako je _prije_ poziva pok == 0 (ili nul-pokazivač, tj. nullptr), realloc se ponaša kao malloc.

Moguće je da realloc optimizira svoj posao (npr. ako je nova veličina jednaka staroj, nije potrebno ništa napraviti),
no C ne garantira čak niti "očito" izvedive optimizacije.
Zbog toga je kod poziva realloc uvijek bitno spremiti novu adresu!
*/


/*
 6. Implementirajte funkciju void* realok(void* ulaz, size_t n, size_t m) koja radi isto što i funkcija
    realloc (do na optimizacije), gdje je n stara veličina u bajtovima, m nova veličina u bajtovima.
    Kopiranje izvedite sami, bajt-po-bajt.
    Tip size_t je cjelobrojni tip definiran u stdlib.h (najčešće samo drugo ime za unsigned long).

    Napomena: funkcija realloc (iz jezika C), za razliku od funkcije koju pišemo u ovom zadatku,
    ne treba naš parametar n.
    To je stoga što postoji sistemska evidencija veličine dinamički alocirane memorije koja je sakrivena od korisnika.
    Mi toj evidenciji ne možemo pristupiti na jednostavan način,
    pa je za ovu funkciju potrebno voditi vlastitu evidenciju.
*/
void* realok(void* ulaz, size_t n, size_t m)
{
    int i;
    void *nova = malloc(m); /* void* pokazivač je generički pokazivač, može spremiti adresu bilo kojeg tipa */
    for (i = 0; i < (n < m ? n : m); ++i)
        /* operator pridruživanja mora znati tip podataka kako bi znao koliko bajtova kopirati (u ovom slučaju 1): */
        ((char*)nova)[i] = ((char*)ulaz)[i];
    free(ulaz);
    return nova;
}

/*
 7. Napišite funkciju koja učitava nenegativne realne brojeve dok ne učita nulu.
    Možete pretpostaviti da će biti učitan barem jedan nenegativan realan broj.
    Funkcija treba ispisati one učitane brojeve koji su strogo veći od geometrijske sredine učitanih brojeva.
    Koristite funkciju pow iz math.h.

    Uočite: ne možemo unaprijed znati niti koliko brojeva će korisnik unijeti,
    niti koji brojevi ispunjavaju kriterij za ispis, pa ih treba sve pamtiti, i po potrebi malo-po-malo širiti polje.
*/
void sredina()
{
    int n = 0;
    double sredina; int i; /* za racun sredine */
    double ulaz;
    /* bitno je da je na početku brojevi == 0,
       kako bismo iskoristili činjenicu da se u prvom pozivu realloc ponaša kao malloc */
    double *brojevi = 0;
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
 8. Napišite funkciju tablica2 koja učitava cjelobrojnu "tablicu" čiji reci ne moraju imati jednak broj ćelija.
    Svaki je redak neprazan.
    Pojedini se redak učitava dok korisnik ne upiše 0.
    Taj broj 0 ne smatramo dijelom same "tablice", ali ga (u ovom zadatku) svejedno zapisujemo u memoriju
    kako bismo znali gdje je kraj pojedinog retka (slično kao kod stringova).
    Pokušaj unosa praznog retka, tj. unos broja 0 na početku učitavanja retka, označava kraj učitavanja tablice.
    Ako tablica ima (ne)paran broj redaka, izbacite sve retke s (ne)parnim indeksom (ako indekse brojimo od nule).

    Ispišite tablicu.

    (Korisnik ni u jednom trenutku ne unosi očekivan broj redaka i/ili stupaca.)
*/
void tablica2()
{
    int **t = 0;
    int n, m, i, j, p, ulaz;

    m = 0;
    do /* učitavanje redaka */
    {
        n = 0;
        do /* učitavanje stupaca */
        {
            scanf("%d", &ulaz);
            if (n == 0 && ulaz != 0) /* prvi broj nije 0: ovo je novi redak */
            {
                ++m;
                t = (int**) realloc(t, m * sizeof(int*));
                t[m - 1] = 0; /* novi redak je na početku prazan, pa je pokazivač na taj redak 0 (nul-pokazivač) */
            }
            if (n > 0 || ulaz != 0) /* treba li unijeti upravo unesen broj (ulaz) kao novi stupac trenutnog retka?
                                       treba u svi slučajevima osim kad smo u pokušaju unosa praznog retka */
            {
                ++n;
                t[m - 1] = (int*) realloc(t[m - 1], n * sizeof(int));
                t[m - 1][n - 1] = ulaz;
            }
        } while (ulaz != 0); /* prekidamo učitavanje stupaca kad je upisana 0 */
    } while (n > 0); /* prekidamo učitavanje redaka kad se pokuša unijeti prazan redak */

    p = m % 2; /* parnost */
    for (i = m - 1; i >= 0; --i) /* ZADATAK: probajte izvesti ovu petlju bez grananja (if) koda */
        if (i % 2 == p)
        {
            free(t[i]); /* dealociramo prostor za redak koji brišemo */
            for (j = i; j < m - 1; ++j) /* naknadne retke premještamo jedno mjesto bliže početku, zbog nastale 'rupe' */
                t[j] = t[j + 1];
            --m; /* pamtimo da imamo jedan redak manje nego ranije */
        }

    t = (int**) realloc(t, m * sizeof(int*));

    for (i = 0; i < m; ++i)
    {
        printf("\n");
        for (j = 0; t[i][j] != 0 ; ++j)
            printf("%d ", t[i][j]); /* pritom ne ispisujemo i broj 0 jer ju ne smatramo dijelom tablice */
        free(t[i]);
    }
    free(t);
}

/* ZADATAK (probajte prvo sami)

   U gornjem zadatku još iz svakog retka tablice s (ne)parnim brojem članova izbacite (ne)parne brojeve.

   Jedno rješenje (koje je moguće umetnuti na kraj gornje vanjske petlje za učitavanje (između linija 208 i 209),
   tj. u trenutku kad znamo da je učitan cijeli redak, a indeks trenutnog retka je m - 1) je ispod u komentarima.
 */


/*

if (n > 0)
{
  --n; // lakše će nam biti računati indekse ako ne moramo misliti na nulu koja se nalazi na kraju retka
  p = n % 2;
  for (i = n - 1; i >= 0; --i) // prolazimo stupcima u trenutnom retku (signalne nule "nema", pa uobičajeno krećemo od n - 1)
    if (t[m - 1][i] % 2 == p)
    {
      for (j = i; j < n - 2; ++j)
        t[m - 1][j] = t[m - 1][j + 1];
      --n;
    }
  t[m - 1][n] = 0; // upisujemo nulu na kraj retka; alternativno mogli smo i unutarnju petlju proširiti do n - 1
  // pa bi ta nula već bila kopirana

  // konačno, reduciramo veličinu polja; ispod piše n + 1 zbog nule na kraju retka
  // (ovaj n je najviše (stari n) - 1, tako da donja realokacija sigurno ne povećava polje)
  t[m - 1] = (int*) realloc(t[m - 1], (n + 1) * sizeof(int));
}

*/


int main()
{

    return 0;
}