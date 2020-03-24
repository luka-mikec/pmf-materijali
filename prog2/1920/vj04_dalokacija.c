#include <stdio.h>
#include <stdlib.h>

/*
 DINAMIČKA ALOKACIJA

  - dinamička alokacija je, okvirno, alokacija (rezervacija) radne memorije u količini koja ne mora biti poznata prije kompilacije programa
  - kod dinamičke alokacije sami se brinemo o alokaciji, korištenju i dealokaciji memorije
  - kada nam je potrebna dinamička alokacija, tj. kada se koristi:
    - ako prije kompilacije nije poznata veličina podataka s kojima program treba raditi
    - ako je potrebno s nekim podacima raditi i nakon prvog bloka u kojem se koriste,
      a ne možemo ili ne želimo koristiti varijable deklarirane izvan tog bloka (npr. globalne)
      (npr. ako imamo kod poput  if (x > 0) { int i = 10; },  varijabla i više nije dostupna nakon simbola "}" za kraj bloka)
    - ako je potreban rad s podacima koji su toliko veliki da ne bi mogli biti smješteni u dio radne memorije
      predviđen za ne-dinamički alociranu memoriju (taj je dio u pravilu puno manji)
    - ako nam neka funkcionalnost programa nije neophodna, pa odlučimo dinamički alocirati
      memoriju samo ako to neće uzeti previše resursa računalu na kojem se program trenutno izvodi
  - što se tiče potreba ovog kolegija, dinamička alokacija nam treba zbog prve, i ponekad zbog druge, situacije
  - memoriju dinamički alociramo koristeći funkciju malloc; poziv malloc(broj_bajtova) radi sljedeće:
     1. alocira (= rezervira) neisprekidan blok memorije u radnoj memoriji veličine broj_bajtova
     2. vraća __pokazivač__ na početak spomenutog bloka, tj. memorijsku adresu njegovog početka
  - izraz sizeof(tip) vraća broj bajtova kojeg zauzima jedna varijabla tipa tip (sve varijable istog tipa
    zauzimaju isti broj bajtova); koliki je to točno broj bajtova ovisi o platformi za koju je program kompajlan
  - malloc vraća vrijednost tipa  void*,  to je tip koji može pohraniti memorijsku adresu bilo kojeg tipa.
    No, takvu adresu ne možemo odmah dereferencirati (pročitati što se nalazi na toj adresi),
    jer uz takvu adresu nije vezan podatak o tipu podatka koji je pohranjen na toj adresi;
    tip nam je potreban kako bismo znali koliko bajtova treba pročitati i kako ih interpretirati.
    Zbog toga, povratnu vrijednost funkcije malloc, tj. vrijednost tipa void*, prvo pretvaramo u pokazivač na pravi tip, npr.:
        int *x = (int*) malloc(sizeof(int));
    U svim ili gotovo svim situacijama na ovom kolegiju kod bi funkcionirao i bez (int *) dijela,
    jer ce se void* vrijednost automatski pretvoriti u int* vrijednost prilikom spremanja u varijablu tipa int*,
    no uvijek ćemo pisati tu konverziju jer je to dobra praksa.
  - ako malloc ne uspije rezervirati memoriju (npr. jer je na računalu slobodna manja količina memorije od zatražene),
    vratit će vrijednost 0. Vrijednost 0 nikad nije adresa (uspješno alociranog) objekta, pa možemo prepoznati
    je li poziv funkcije malloc bio uspješan po tome je li vraćena vrijednost 0.
  - tipično je korištenje dinamičke alokacije prilikom alokacije niza objekata, npr. za niz od n instanci tipa int:
        int *pok = (int*)malloc(n * sizeof(int))
  - dinamički alociranoj memoriji pristupamo kao memoriji na koju pokazuje bilo kakav pokazivač:
    pomoću operatora dereferenciranja, npr. kao *pok (ako ste alocirali prostor za jedan objekt, npr. *pok = 5) ili
    pok[...] (ako ste alocirali prostor za niz, npr. pok[12] = 34;)
  - u gornjoj natuknici, "pok" je obična automatska varijabla (koja sprema adresu bloka memorije),
    i ta (obična, automatska) varijabla sadrži adresu dinamički alocirane memorije
  - dinamički alociranu memoriju potrebno je dealocirati kad nam više nije potrebna: free(pok);
  - korištenje dinamički alocirane memorije kompliciranije je nego inače jer moramo paziti da:
    - ne koristimo prostor prije alokacije ili nakon dealokacije
    - alociramo ispravnu količinu memorije, tj. barem onoliko koliko je potrebno
    - prilikom pristupanja memoriji koristimo ispravne operatore
    - dealociramo memoriju nakon korištenja: free(pok);
*/



/* 1. Napišite dio programa koji učitava dva cijela broja, prvi sprema u uobičajen
      (automatski alociran) prostor, a drugi u dinamički alociran prostor.
      Ispisuje se veći od dva broja.
*/

void dio_programa()
{
    int x; /* ova varijabla je automatski alocirana: C sam rezervira memoriju za x, te ju sam oslobađa naknadno */
    int *y  = (int*) malloc(sizeof(int)); /* sam y (pokazivač) je automatski alocirana varijabla, ali sadrži adresu d. a. memorije */
    scanf("%d", &x); /* adresa na koju želimo da scanf zapiše ulazni broj je &x, zbog toga šaljemo &x */
    scanf("%d", y);  /* ovdje nema operatora &, jer je u y pohranjena adresa (dinamički alocirane) memorije na koju želimo pisati */
    printf("Veci je %d\n", x > *y ? x : *y); /* ne smijemo zaboraviti koristiti operator * prilikom pristupa d. a. memoriji (inače pristupamo adresi, a ne samoj memoriji) */
    free(y); /* prostor za x će biti automatski dealociran, a prostor za *y, jer je dinamički alociran, moramo sami dealocirati */
}

/* 2. Napišite funkciju koja prima pozitivan cijeli broj n, učitava n cijelih brojeva, te ih ispisuje unatrag.
      Koristite dinamički alocirano polje.
*/

void unatrag(int n)
{
    int i;
    int *brojevi = (int*) malloc(n * sizeof(int));

    /* Je li alokacija uspjela?
       Ovo bismo trebali provjeriti kad god je broj bajtova potencijalno velik (ovdje n može biti vrlo velik) */
    if (brojevi == 0)
        return;

    for (i = 0; i < n; ++i)
        scanf("%d", &brojevi[i]); /*   može i "brojevi + i" (zašto?) umjesto "&brojevi[i]"   */
    for (i = n - 1; i >= 0; --i)
        printf("%d ", brojevi[i]);

    free(brojevi);
}

/*
    ZADATAK: probajte riješiti isti zadatak bez korištenja nizova, koristeći rekurziju
        (takvo će rješenje biti sporije, i za malo veće vrijednosti n neće niti raditi)
*/



/*
 VIŠEDIMENZIONALNA POLJA

 Ranije smo vidjeli jednu specifičnu strukturu (niz nizova) koju smo zvali dvodimenzionalnim poljem.
 Još neke strukture možemo smatrati dvodimenzionalnim poljima. Vidi skriptu (poglavlje 4.3).

 Ono što nam od tih mogućnosti najčešće treba jest d. a. niz d. a. nizova (četvrta situacija u skripti).
 Ako se radi o cijelim brojevima, korištenje izgleda otprilike ovako:
        int **x;
        int i; --- umjesto ove i prethodne linije možemo kratko:  int **x, i;   ili   int i, **x;
                   jer se zvjezdice odnose samo na (neposredno) sljedeću varijablu
        x = (int**)malloc(m * sizeof(int*));  --- alociramo niz koji čuva sve adrese redaka
        for (i = 0; i < m; ++i)
          x[i] = (int*)malloc(n * sizeof(int));  ---  alociramo svaki redak i njegovu adresu spremamo u gornji niz
        (... ovdje dolazi korištenje dvodimenzionalnog polja ...)
        for (i = 0; i < m; ++i) --- dealociramo svaki redak
          free(x[i]);
        free(x); --- dealociramo niz adresa redaka: uočite, prvo dealociramo same retke, a onda dealociramo ovaj niz
*/



/*
  3. Napišite sljedeće funkcije:
        alokacija, koja prima dimenzije m i n, te vraća dinamički alociranu matricu realnih brojeva; ne morate provjeriti uspješnost alokacije
        dealokacija, koja prima dinamički alociranu matricu dimenzija m x n, te ju dealocira
        ucitavanje, koja prima dinamički alociranu matricu dimenzija m x n, te ju popunjava koristeći scanf
        ispis, koja prima dinamički alociranu matricu dimenzija m x n, te ju ispisuje
        umnozak, koja prima dinamički alociranu matrice: X dimenzija m x n  i  Y dimenzija n x k, te vraća njihov umnožak
*/

double **alokacija(int m, int n)
{
    int i;
    double **a = (double**) malloc(m * sizeof(double*));
    for (i = 0; i < m; ++i)
        a[i] = (double*)malloc(n * sizeof(double));
    return a;
}

void dealokacija(double **a, int m) /* parametar n nam nije potreban za dealokaciju  */
{
    int i;
    for (i = 0; i < m; ++i)
        free(a[i]);
    free(a);
}

void ucitavanje(double **a, int m, int n)
{
    int i, j;
    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            scanf("%lf", &a[i][j]);
}

void ispis(double **a, int m, int n)
{
    int i, j;
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j)
            printf("%lf ", a[i][j]);
        printf("\n");
    }
}

/* Napomena:
    Nekada funkcije kojima je rezultat niz same trebaju alocirati memoriju za rezultat kao u donjem podzadatku s umnoškom,
    a nekada očekuju da će to napraviti pozivatelj kao u gornjoj funkciji ucitavanje(double **a, int m, int n).
    U oba se slučaja pozivatelj brine o dealokaciji te memorije.
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
 Primjer poziva:
     double **X = alokacija(m, n);
     double **Y = alokacija(n, k);
     ucitavanje(X, m, n);
     ucitavanje(Y, n, k);
     double **Z = umnozak(X, Y, m, n, k);
     ispis(Z, m, k);
     dealokacija(X, m, n);
     dealokacija(Y, n, k);
     dealokacija(Z, m, k);
*/

/*
    ZADATAK: napišite funkciju double **ucitavanje2()
             koja koristeći scanf učita dimenzije m i n, alocira i ispuni matricu takvih dimenzija, te vrati tu matricu.
*/

/*
    ZADATAK (nije relevantan za kolegij): potražite što radi "comma operator" (operator , ) i razmislite
        kako ga iskoristiti na što prirodniji način, tako da u funkciji 'ispis' više nisu potrebne
        vitičaste zagrade (osim onih koje omeđuju tijelo funkcije).
        Iako kraći, tako dobiven kod nikako nije bolji od gore napisanog rješenja: čitljivost je bitnija od kratkoće koda.
        Možete razmisliti i što bi bili drugi načini za riješiti se vitičastih zagrada, osim korištenja operatora zareza.
        Postoji još barem dva načina.
*/




/*
 2. U ovom zadatku cilj je implementirati igru minesweeper.

    Igra se igra na ploči dimenzija m * n; m, n > 1.

    - na početku igra odredi da neke ćelije sadrže minu (-1), a ostale su prazne (-2)
    - igra je petlja sa sljedećim koracima:
       1. igrač odabire proizvoljnu ćeliju (i, j);
       2. ako je na (i, j) mina, petlja se prekida;
       3. inače, igra u ćeliju (i, j) upisuje koliko susjednih ćelija sadrži minu (dakle, upisuje broj iz skupa {0, ..., 8}).
          Ako je taj broj 0, izvršava se 3. korak i za sve prazne (-2) susjedne ćelije.

    a) Napišite funkciju koja alocira ploču dimenzija m x n (ne morate provjeriti uspješnost), te ispunjava ćelije na sljedeći način:
          broj -1 s vjerojatnošću 1/4 (okvirno),
          broj -2, inače
       Uputa: funkcija rand() iz stdlib.h vraća pseudo-slučajan cijeli broj između 0 i RAND_MAX.
*/

int **slucajna(int m, int n)
{
    int i, j;
    int **a = (int**) malloc(m * sizeof(int*));
    for (i = 0; i < m; ++i)
        a[i] = (int*)malloc(n * sizeof(int));

    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            a[i][j] = rand() % 4 == 0 ? -1 : -2; /* za rand() nam treba #include "stdlib.h" */

    return a;
}
/*
    ZADATAK: Umjesto ovakve implementacije slučajne ploče, funkciji dodajte argument k.
             Matrica mora na točno k slučajno odabranih mjesta sadržavati minu.
             ("slučajno" ovdje ne mora biti "pametno", dovoljno je da se svaki mogući odabir polja u principu može ostvariti)
*/

/*
 b) Napravite ispis stanja ploce: -2 (prazno polje) ispisite kao . (točka),
    -1 (mina) kao * (zvjezdica), a nenegativne brojeve ispišite kao brojeve.
*/
void ispis_ploce(int **a, int m, int n)
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
    ZADATAK: Napravite verziju ispisa koja osobi koja igra ne otkriva pozicije mine.
             Osim toga, neka ta verzija pokraj same ploče ispiše indekse redaka i stupaca (lijevo i iznad).
             Prvo napišite verziju koja funkcionira za jednoznamenkaste dimenzije, a potom za sve.
*/

/*
 c) Implementirajte pomoćnu funkciju int sadrzan(int m, int n, int i, int j),
    koja provjerava je li (i, j) u granicama polja dimenzije m x n
*/
int sadrzan(int m, int n, int i, int j)
{
    return i >= 0 && i < m && j >= 0 && j < n ? 1 : 0;
}

/*
 d) implementirajte funkciju int otvaranje(int **a, int m, int n, int r, int s)
    koja radi sljedeće:
     - ako je (r, s) izvan granica, vraća -1
     - ako je na (r, s) mina, vraća 1
     - inače, smatramo polje (r, s) otvorenim i funkcija upisuje broj mina susjednih polju (r, s)
       u ćeliju a[r][s]
     - ako nema mina u susjedstvu, ponavljamo pozive ove funkcije za polja susjedna polju (r, s) koja još nisu otvorena
     - neka funkcija sada vrati 0, što će nam biti oznaka da se igra nastavlja
*/
int otvaranje(int **a, int m, int n, int r, int s)
{
    int i, j, br = 0;

    /* je li odabrano polje koje postoji na ploči? */
    if (sadrzan(m, n, r, s) == 0)
        return -1;

    /* je li odabrano polje na kojem se nalazi mina? */
    if (a[r][s] == -1)
        return 1;

    /* prolazimo susjednim poljima i brojimo mine */
    for (i = r - 1; i <= r + 1; ++i)
        for (j = s - 1; j <= s + 1; ++j)
            if (sadrzan(m, n, i, j) && a[i][j] == -1)
                ++br; /* uočite da nismo morali posebno paziti na slučaj i == r, j == s,
                         unatoč tome što polje (r, s) ne smatramo susjednim sebi samom; zašto? */
    a[r][s] = br;

    /* ako nema mina na odabranom polju, rekurzivno otvaramo i susjedna polja (ako već nisu otvorena) */
    if (br == 0)
        for (i = r - 1; i <= r + 1; ++i)
            for (j = s - 1; j <= s + 1; ++j)
                if (sadrzan(m, n, i, j) && a[i][j] == -2)
                    otvaranje(a, m, n, i, j); /* zašto ovdje nismo morali posebno paziti na slučaj i == r, j == s?  */

    return 0;
}

/*
    e) Napišite funkciju igra(m, n) koja implementira igru Minesweeper na ploči dimenzija m x n.
*/

void igra(int m, int n) {
    int i, j, stanje;
    int **ploca = slucajna(m, n);
    do {
        ispis_ploce(ploca, m, n);
        printf("Upisite koordinate polja kojeg zelite otvoriti (odvojena razmakom): ");
        scanf("%d %d", &i, &j);
        stanje = otvaranje(ploca, m, n, i, j);
    } while (stanje == 0); /* petlja se izvršava dok god osoba uspješno otvara polja */

    for (i = 0; i < m; ++i)
        free(ploca[i]);
    free(ploca);
}


/*
    ZADATAK: pronađite što radi funkcija srand i dodajte je na odgovarajuće mjesto u kodu, kako bi različita pokretanja
        funkcije  slucajna  doista davala različite ploče.
    ZADATAK: u petlju u funkciji igra() dodajte provjeru je li igra završena, tj. jesu li otvorena točno sva polja
        osim onih u kojima se nalazi mina; u tom slučaju prekinite petlju i ispišite poruku o uspješno završenoj igri.
        Dodajte i odgovarajuć ispis za stanje == -1.
        Za stanje == 1, promijenite ponašanje gornje funkcije, tj. umjesto prekida igre, neka igra upozori korisnika
        da je unio nepostojeće koordinate, i ponovno ga upita da unese ispravne (dok god ne unese smislene koordinate).
    ZADATAK: potražite imena funkcija koje vraćaju trenutno vrijeme, i iskoristite ih za izračunati i ispisati
        vrijeme provedeno igrajući.
    ZADATAK (teže): potražite u skripti kako čitati i pisati u datoteke. Na kraju uspješne igre, pitajte osobu da unese ime.
        Dodajte rezultat u datoteku s rezultatima (highscores), po potrebi stvarajući tu datoteku ako već ne postoji.
        Potom ispišite sve rezultate (imena i vremena) iz te datoteke, sortirano od najboljih vremena prema lošijima.
        S datotekama ćemo ("službeno") raditi u drugom dijelu kolegija.
 */

int main()
{
    igra(6, 5);
    return 0;
}
