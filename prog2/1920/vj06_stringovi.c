#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/*
  Termin "string" u jeziku C označava (pod)niz znakova koji završava (prvim) znakom '\0', tj. nul-znakom.
  Taj je niz znakova (string i nul-znak koji signalizira kraj stringa) možda podniz nekog duljeg niza znakova.
  Taj dulji niz znakova može sadržavati bilo kakve podatke nakon našeg stringa, npr. možda sadrži druge stringove:
    char veliko_polje[] = ['a', 'b', '\0', 'c', '\0', 'd', 'e', '\0', 'f' ]
  Polje veliko_polje sadrži stringove "ab", "c" i "de", i potom znak 'f' (nije string: nema nul-znaka poslije 'f')
  Znak '\0' je u memoriji spremljen kao bajt 0 (svi bitovi su 0) i nema grafičku reprezentaciju.

  Primjer: char a[] = "abc";  (ekvivalentno: a = {'a', 'b', 'c', '\0'); )

  Duljina od a _kao niza_ je 4, ali _kao stringa_ je 3!
  (Duljina stringa a je zapravo indeks prvog znaka '\0' u nizu a)

  Primjer: b = {'a', 'b', 'c', '\0', 'x', '\0'}

  Duljina od b kao niza je 6, ali kao stringa 3 (broj ne-nul znakova do prvog nul-znaka)
  Niz b "sadrži" više stringova, no fraza "string b" referira na prvi.
  Slično, duljina od (b + 4) je 1: na adresi (b + 4) nalazi se string "x".

  Zbog znaka '\0' treba paziti koliko se memorije alocira (uvijek znak više od duljine stringa).

  #include <ctype.h>   korisne funkcije za rad sa znakovima
  #include <string.h>  korisne funkcije za rad sa stringovima (npr. strlen)
*/

/*
1. (5.1.1.) Napišite dio programa koji učitava jednu riječ duljine najviše 17 znakova, te ispisuje:
    a) tu riječ,
    b) tu rijeć bez prvog znaka,
    c) treći znak te riječi.
*/
void z511()
{
    char rijec[17 + 1];
    scanf("%17s", rijec); /*  "rijec" jest adresa memorije u koju želimo pisati, pa nema & */

    printf("%s\n", rijec);
    printf("%s\n", &rijec[1]); /* ili rijec + 1 */
    printf("%c\n", rijec[2]); /* ili *(rijec + 2) */
}

/*
  Za složenije scenarije poput učitavanja riječi i redaka pogledajte skriptu i predavanja.
*/


/*
2. (5.3.1.) Napišite funkciju void b(char *a) koja iz stringa a "briše" sve samoglasnike.
   Pritom funkcija ne smanjuje samo polje koje sadrži string, jer možda nije dinamički alocirano,
   a i možda čuva još nešto osim stringa.

   Možete koristiti ctype.h.

Npr. ako su memorijske ćelije izgledale ovako: [a][b][c][d][e][f][0]...
Nakon poziva trebaju izgledati ovako: [b][c][d][f][0]...
I pritom ne bi trebalo promijeniti sadržaj memorije nakon stare pozicije prvog nul-znaka.
*/
void b_1(char *a) /* jedno rješenje */
{
    int i, j;

    for (i = 0; a[i] != '\0'; ) /* ++i želimo samo kad trenutni znak nije samoglasnik! */
    {
        char c = tolower(a[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
            /* ako trenutni znak jest samoglasnik, premještamo ostatak stringa jedno mjesto unatrag */
            for (j = i; a[j] != '\0'; ++j) /* uočite - ovime kopiramo i završnu nulu */
                a[j] = a[j + 1];
        else
            /* inače trenutni znak nije samoglasnik, ne treba ništa premještati i prelazimo na sljedeći indeks */
            ++i;
    }
}
/*
    Gornja funkcija prima polje koje se mora moći mijenjati!
    za kakve argumente a funkcionira poziv b_1(a)?
    char a[] = "x";
        Spremanje znakova 'x', '\0' u novi (promjenjiv) niz a.
        Niz je promjenjiv, pa funkcija radi ispravno.
    char  *a = "x";
        Niz "x" je ovdje konstantan (kompajler smije optimizirati na način da se "x" niti ne nalazi u memoriji
        prilikom pokretanja). Pokazivač a moguće je preusmjeriti na drugi niz, ali sadržaj na trenutnoj adresi
        nije dopušteno mijenjati (jer se, dakle, taj sadržaj možda niti ne nalazi u memoriji).
        Zbog toga, funkcija neće raditi ispravno s ovakvim argumentom a.
    char str[] = "x"; char  *a = str;
        Ovo je u redu: iako je tip char*, ne pridružujemo mu konstantan niz "x", već promjenjiv niz "x".
        (On je promjenjiv jer je varijabla char str[] inicijalizirana njime, a u takvom slučaju C
         osigurava promjenjivost niza)
    char  *a = (char*) malloc((1 + 1) * sizeof(char)); a[0] = 'x'; a[1] = '\0';
        Ovo je također u redu jer dinamički alociran niz nije konstantan.
*/

/*
    Isti zadatak mogli smo riješiti i tako da brišemo "od kraja".
    Sada ne moramo paziti na to kada inkrementiramo/dekrementiramo varijablu i,
    ali moramo izračunati početnu duljinu stringa na početku.
*/

void b_2(char *a)
{
    int i, j;
    int n = strlen(a); /* duljina stringa a, tj. indeks prvog '\0' u nizu a */

    for (i = n - 1; i >= 0; --i)
    {
        char c = tolower(a[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        {
            for (j = i; j < n; ++j) /* opet, ne idemo samo do n - 1 jer zelimo i zavrsnu nulu */
                a[j] = a[j + 1];
            --n;
        }
    }
}

/*
    Pogledajte i rješenje iz skripte (5.3.1); općenito, puno je odluka oko implementacije ove funkcije,
    što rezultira s vrlo mnogo mogućih rješenja.
*/


/*
    3. (5.3.3.) Napišite funkciju koja kao argument prima jedan string, te iz njega briše svako treće slovo.

    Napomena: pod "slovo" podrazumijevamo samo znakove a--z te A--Z.
*/

void s(char *a)
{
    int i, j, slova = 0;

    for (i = 0; a[i] != '\0'; )
    {
        char c = tolower(a[i]);
        if (isalpha(c))
        {
            ++slova;
            if (slova % 3 == 0)
                for (j = i; a[j] != '\0'; ++j) /* uočite - ovime kopiramo i završnu nulu */
                    a[j] = a[j + 1];
            else
                ++i;
        }
        else
            ++i;
    }
}


/*
    4. Napišite funkciju char* bin(char *x) koja kopira x u novi, dinamički alociran, string,
    ali tako da sve prirodne brojeve zapisane unutar stringa (pretpostavljamo da su zapisani u dekadskom zapisu)
    zapisuje u bazi 2.
    Npr. "2 + 2 = 4" |-> "10 + 10 = 100".

Dva načina za realokaciju: prvo saznati finalan broj znakova
(= broj znakova - broj dekadskih (starih) znamenaka + broj binarnih (novih) znamenaka).

Drugi način (manje efikasan, ali s manje računanja): proširivati polje po potrebi.

Postupak: čitati znak-po-znak. Ako znak nije znamenka, kopira se.
          Inače, pročita se ostatak broja, zapisuje se u bazi 2 od posljednje prema prvoj znamenci,
          invertira se poredak tako zapisanih znamenki, i potom nastavlja se čitati početni niz.
*/

/* prima pokazivač na dinamički alociran string, produljuje ga za jedan novi znak i umeće znak c na kraj */
char* dodaj(char *x, char c)
{
    int n = strlen(x);
    x = (char*)realloc(x, (n + 1 + 1) * sizeof(char)); /* stara duljina niza znakova x je n + 1, pa je nova n + 2 */
    x[n] = c;
    x[n + 1] = '\0';
    return x;
}

/* prima pokazivač na string x, invertira segment koji počinje na indeksu poc i čija je duljina jednaka d */
void poredak(char *x, int poc, int d)
{
    int i;
    for (i = 0; i < d/2; ++i)
    {
        char t = x[poc + i]; /* ove tri naredbe su klasicna swap operacija, kao u sortiranju */
        x[poc + i] = x[poc + d - 1 - i];
        x[poc + d - 1 - i] = t;
    }
}

char* bin(char *x)
{
    int i, poc, d, broj;

    /* x (argument) je ulazni string, a y je izlazni string: */
    char *y = (char*)malloc(sizeof(char));
    y[0] = '\0'; /* inicijaliziramo y na prazan string, tj. nema niti jednog znaka ispred nul-znaka */

    for (i = 0; i < strlen(x); )
    {
        if (!isdigit(x[i]))
        {
            dodaj(y, x[i]);
            ++i;
            continue;
        }

        /* ako se nalazimo na ovom mjestu u izvršavanju funkcije, nalazimo se na broju (x[i] je prva znamenka) */

        for (broj = 0; isdigit(x[i]); ++i)
            broj = 10 * broj + (x[i] - '0');
        /* zbog gornje petlje i njenog ++i, sada se nalazimo na prvom
           znaku nakon broja u dekadskom zapisu u početnom stringu; zbog toga u nastavku nema ++i */

        if (broj == 0)
            dodaj(y, '0');
        else
        {
            poc = strlen(y); /* pamtimo indeks na kojem počinje binarno zapisan broj kojeg dodajemo u y */
            d = 0; /* koliko je dugačak spomenuti bianrni zapis? */
            while (broj > 0)
            {
                /* broj % 2 je znamenka koja nam treba, dok + '0' daje ascii kod znamenke */
                dodaj(y, (broj % 2) + '0');
                broj = broj / 2;
                ++d;
            }

            /* u ovom trenutku smo dodali binarni zapis na kraj stringa y, ali u obratnom poretku znamenaka; stoga: */
            poredak(y, poc, d);
        }
    }

    return y;
}


/*
    ZADATAK: napišite funkciju koja provjerava jesu li zagrade '(' i ')' u (char *) stringu ispravno sparene,
    tj. svaka ima svoj par. Npr. "()", "()()", "((((()))))" su neka ispravna sparivanja, a "(()", ")(" i "()())" nisu.
*/


int main()
{
    return 0;
}