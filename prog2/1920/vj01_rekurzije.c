#include "stdio.h"

/* REKURZIJE
  - ideja rekurzije: kombiniranje rješenja jednostavnih instanci (slučajeva) nekog problema 
    u rješenje složenijih instanci istog problema
  - u C-u, rekurzije možemo prirodno implementirati koristeći rekurzivne funkcije
  - rekurzivne funkcije su funkcije koje u nekim situacijama pozivaju same sebe 
  - struktura jednostavne rekurzije:
    povratni_tip f(argumenti)
    {
      if (znamo izravno riješiti problem za dane argumente) {
        ... BAZA rekurzije: izravno računanje, bez novog poziva fje f 
        return ...;
      }
      ... KORAK rekurzije: računanje koje radi nove pozive f 
      return ...;
    }
  - uvjeti moraju biti takvi da svaki poziv u konačno mnogo koraka završi u baznom slučaju!
*/

/* primjer jednostavne rekurzije: potenciranje broja 2 */
int rek_p2(int n)
{
    if (n == 0)
        return 1;
    return 2 * rek_p2(n - 1);
}

int petlja_p2(int n) {
    /* gornja se rekurzija može "linearizirati" kao petlja: */
    int rezultat = 1, i;
    for (i = 0; i < n; i++) {
        rezultat *= 2;
    }
    return rezultat;
}

/*
  - petlje se mogu shvatiti kao jednostavne rekurzije,
    rješenje gradimo korak po korak
    
  * ZADATAK: probajte sami napisati "opći oblik" rekurzivne funkcije
    koja "simulira" uobičajenu for-petlju 

  - osnovno ograničenje petlji: rješenje gradimo "linearno":
    npr. kod računanja potencije broja 2, prvo izračunamo 2^1,
    pa koristeći taj rezultat 2^2, pa koristeći njega 2^3
  - rekurzija ne mora biti "linearna"; donja rek. funkcija fib
    ovisi ne samo o fib(n - 1), već i fib(n - 2)
*/

/* fib: 0  1  1  2  3  5  8  13  21  34 ... */
int fib(int n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return fib(n - 1) + fib(n - 2);
}

/*
  - rješenje od fib(n) ne gradimo samo iz fib(n - 1) već i iz fib(n - 2)
  - u ovom slučaju, unatoč "nelinearnosti" postoji jednostavna
    petlja koja radi isti posao 
*/

int petlja_fib(int n)
{
    int i, suma = 0, suma_p = 0, suma_pp = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            suma = 0;
        }
        if (i == 1) {
            suma = 1;
        }
        if (i > 1) {
            suma = suma_p + suma_pp;
        }

        suma_pp = suma_p;
        suma_p = suma;
    }
    return suma;
}

/*
  - međutim, mnoge se rekurzivne funkcije ne mogu jednostavno "linearizirati"
  - primjerice, zamislimo da je neka funkcija fab definirana na sljedeći način:
    fab(int n) = n za n <= 1
                 fab(n - 1) + fab(n / 2)
    (tako definirana funkcija, naravno, ne predstavlja Fibonaccijev niz)
  - kod Fibonaccijevog niza za račun sljedećeg člana dovoljno je pamtiti dva prethodna člana,
    dok ovdje pamćenje bilo kojeg konačnog broja prethodnih članova neće biti dovoljno
  - "linearizacija" je posebno često neprirodna kod funkcija s više argumenata
*/



/*
1. Napišite rekurzivnu funkciju 'zapisa' koja računa broj načina na koje se
   (int) n, n >= 0, može zapisati kao (možda prazna) suma brojeva 2, 3 i 5.
   Smatramo da je zapis 3 + 5 različit od 5 + 3.
   Smatramo da je prazna suma (bez pribrojnika) način, i to jedini način,
   za prikazati broj 0.

Kao i uvijek, pitamo se kako iz rješenja jednostavnijih slučajeva (instanci)
traženog problema konstruirati rješenje složenog problema.
U ovom zadatku, a često i inače, to nije sasvim očito.
Stoga, prvo pokušamo rastaviti traženi rezultat na (pod)slučajeve. 
Na taj način često prirodno dolazimo do rekurzivnog poziva.

Primjerice, rastavi broja 8 sastavljeni od 2, 3 i 5:
2 + 2 + 2 + 2
2 + 3 + 3
3 + 2 + 3
3 + 3 + 2
3 + 5
5 + 3

Možemo li nekako grupirati ove rastave u grupe koje je jednostavnije izbrojati?
Svaki rastav odgovara nekom nizu pribrojnika.
Jedan od prirodnih načina za odabrati grupe je grupirati po duljini niza.
Međutim, to nam daje broj slučajeva koji nije unaprijed poznat,
što bismo voljeli izbjeći (nekad je to dobra taktika, 
npr. rekurzija za Catalanove brojeve:
https://en.wikipedia.org/wiki/Catalan_number#Fifth_proof).
 
Stoga možemo grupirati zapise po (npr.) prvom ili zadnjem članu.

Ideja: rastavimo traženi broj na tri dijela:
 ->  broj načina za n prikazati kao odgovarajuću sumu koja započinje s 2
 ->  broj načina za n prikazati kao odgovarajuću sumu koja započinje s 3
 ->  broj načina za n prikazati kao odgovarajuću sumu koja započinje s 5
 
Uočite da je broj načina za n prikazati kao odgovarajuću sumu koja započinje s 2
jednak broju načina za broj n - 2 prikazati kao (neku) odgovarajuću sumu.
Općenito, broj načina za n prikazati kao odgovarajuću sumu koja započinje s x je: zapisa(n - x).
*/

int zapisa(int n)
{
    if (n < 0)    /* s obzirom na to da npr. zapisa(1) poziva npr. zapisa(-1), potreban nam je i ovaj slučaj */
        return 0; /* samo uz 2, 3 i 5 nema načina za zapisati brojeve manje od nule */
    if (n == 0)   /* jedan način za zapisati nulu (prazna suma) */
        return 1;
    return zapisa(n - 2) + zapisa(n - 3) + zapisa(n - 5); /* uočite disjunktnost slučajeva */
}

/* Uočite: n se strogo smanjuje, dok ne vrijedi baza: n = 0 ili n < 0 */

/*
  * ZADATAK: imajući u vidu što računa funkcija 'zapisa', što računa 'fib'?
*/



/*
2. Riješite prethodni zadatak, ali bez razlikovanja zapisa koji se
   razlikuju samo u poretku pribrojnika.

Ideja: ograničimo prethodno rješenje
tako da ono broji samo uzlazno sortirane poretke.

Objašnjenje: ovdje brojimo klase ekvivalencija rastava, gdje su zapisi 
ekvivalentni ako se razlikuju do na poredak.
Najjednostavniji način je izbrojati uniformno odabrane reprezentante tih klasa,
a tu je najjednostavniji odabir neki konkretan poredak: npr. uzlazan ili silazan.

Kako: pamtit ćemo najveću do sada dodanu vrijednost,
od koje manju ne smijemo dodati u daljnjem pribrajanju.
*/

int particije(int n, int max_do_sada)
{
    int suma = 0;
    if (n < 0)
        return 0;
    if (n == 0) 
        return 1;
    if (2 >= max_do_sada)
        suma += particije(n - 2, 2);
    if (3 >= max_do_sada)
        suma += particije(n - 3, 3);
    if (5 >= max_do_sada)
        suma += particije(n - 5, 5);
    return suma;
}

/* Uočite: n se strogo smanjuje, dok ne vrijedi baza: n = 0 ili n < 0 */



/*
3. Napišite funkciju 'putova' koja računa broj načina na koje se od broja x,
   0 <= x < n, može doći do n, u najviše k koraka.
   Dopušteni su pomaci +5 i -2, i nije dopušteno izaći iz [0, n>.
   
Ideja: slično kao i u ranijim zadacima, rješenje = 
   br. načina na koje se od broja x može doći do n u najviše k koraka, 
     ako je prvi korak "+5"
 + br. načina na koje se od broja x može doći do n u najviše k koraka,
     ako je prvi korak "-2"
 = br. načina na koje se od broja x + 5 može doći do n u najviše k - 1 korak
 + br. načina na koje se od broja x - 2 može doći do n u najviše k - 1 korak
*/
int putova(int x, int n, int k)
{
    if (x < 0 || x > n) /* izašli smo iz dopuštenog skupa pozicija */
        return 0;
    if (k < 1) /* potrošili smo sve korake---može i "k < 0", ovisno o tome što shvaćamo kao korak */
        return 0;
    if (x == n)    /* na cilju smo: završavamo put (na jedan način, */
        return 1;  /* tj. ignoriramo eventualno ostvarive cikluse) */
    return putova(x + 5, n, k - 1) + putova(x - 2, n, k - 1);
}

/* Uočite: k se strogo smanjuje, dok ne vrijedi baza: k = 0 (ili izademo iz [0, n>) */





int main(int argc, char const *argv[]) {
    printf("%d\n", putova(0, 20, 15));

    return 0;
}
