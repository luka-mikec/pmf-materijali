#include <stdio.h>
#include <stdlib.h>

// u komentarima iznad nekih zadataka su sa zvjezdicom označene varijacije
// zadataka (za samostalno rješavanje) koje nisu spomenute na vježbama

/*
2. Napišite rekurzivnu funkciju koja računa broj načina na koje se cijeli broj n
   može zapisati kao suma pribrojnika 2, 3 i 5 (ne nužno svih).

   Razlikujemo poredak, tj. 3 + 5 nije isto što i 5 + 3.

   Smatramo da se nula može zapisati na jedan način (kao prazna suma).

Ideja: kao i obično, treba pronaći način za svesti danu instancu ovog problema na
jednostavniju.
Traženi broj načina jednak je (primijetite da nema preklapanja):
 broj načina za n - 2 prikazati kao sumu koja započinje s 2 +
 + broj načina za n - 3 prikazati kao sumu koja započinje s 3 +
 + broj načina za n - 5 prikazati kao sumu koja započinje s 5.
*/
/*
int f(int n)
{
  if (n < 0) // negativan broj se ne može prikazati kao suma od 2, 3 i 5.
    return 0;
  if (n == 0) // vidi napomenu u zadatku
    return 1;

  return f(n - 2) + f(n - 3) + f(n - 5);
}
*/


/*
3. Prethodni zadatak, bez razlikovanja sumi koje se razlikuju samo u poretku.
   (tj. sada brojimo particije broja: https://en.wikipedia.org/wiki/Partition_(number_theory))

Ideja: na vježbama smo nacrtali stablo poziva u prethodnom zadatku.
"Skretanje" lijevo se moglo tumačiti kao "+ 2", dolje kao "+ 3", desno kao "+ 5".
Kako bismo osigurali uvjet iz zadatka, možemo postaviti uvjet da jednom kad "skrenemo"
dolje, više ne skrećemo lijevo; te jednom kad skrenemo desno, više ne skrećemo ni lijevo ni dolje.
Možemo razmišljati o ovom problemu kao problemu brojanja sumi s nekim fiksiranim poretkom (npr. uzlazan) pribrojnika.

SAMI: implementirajte ovu funkciju koristeći operator ?: i bez varijable s.

SAMI: implementirajte ovu funkciju bez rekurzije (i, kao i obično, bez pomoćnih nizova).

SAMI*: precizno formulirajte problem kojeg rješava donja funkcija, te izrazite naš problem preko njega.
*/
/*
int f(int n, int max)
{
  int s = 0;
  if (n < 0)
    return 0;
  if (n == 0)
    return 1;
  if (max <= 2)
    s += f(n - 2, 2);
  if (max <= 3)
    s += f(n - 3, 3);
  if (max <= 5)
    s += f(n - 5, 5);
  return s;
}
*/


/*
4. Prethodni zadatak, ali umjesto brojanja podnizova, ispišite ih.
   Pretpostavimo da je najviše 100 pribrojnika.
   Možete koristiti niz.

SAMI: promijenite ispis tako da za n = 5 dobijete:
2 + 3
5
(svaka particija u svom retku, brojevi odvojeni plusom)

SAMI*: nacrtajte stablo poziva za mali n i pokušajte precizno formulirati zašto
nije bilo potrebno kopirati niz (odnosno, zašto smo mogli stalno koristiti isti niz).

SAMI*: implementirajte funkciju iz 3. zadatka bez velikog pomoćnog niza i bez nestandardnih biblioteka.
SAMI* [teško]: implementirajte funkciju iz 2. zadatka bez velikog pomoćnog niza i bez nestandardnih biblioteka.
*/
/*
void f(int n, int max, int pom[100], int i) // može i int pom[]
{
  int j;
  if (n < 0)
    return;
  if (n == 0)
  {
    for (j = 0; j < i; ++j)
      printf("%d ", pom[j]);
    printf("\n");
    return;
  }

  if (max <= 2)
  {
    pom[i] = 2;
    f(n - 2, 2, pom, i + 1);
  }
  if (max <= 3)
  {
    pom[i] = 3;
    f(n - 3, 3, pom, i + 1);
  }
  if (max <= 5)
  {
    pom[i] = 5;
    f(n - 5, 5, pom, i + 1);
  }
}
*/


/*
 5. Napišite rekurzivnu funkciju koja prima niz cijelih brojeva a duljine n, te radi sljedeće:
    - prvu, drugu i treću trećinu niza postavlja redom na 1, 0, 1
    - funkcija se poziva na prvoj i trećoj trećini, ako je n > 3
    Pretpostavlja se da se funkcija nikad ne poziva za n koji nije oblika 3^k za k >= 1.

SAMI: pogledajte malo drugačije rješenje u skripti, bez aritmetike pokazivača (pointera)

SAMI: neka poziv funkcije za n koji nije oblika 3^k za k >= 1 rezultira nepromijenjenim nizom.
*/
/*
void f(int a[], int n)
{
  int i;
  for (i = 0; i < n; ++i)
    if (i < n / 3)
      a[i] = 1;
    else if (i < 2 * n / 3)
      a[i] = 0;
    else
      a[i] = 1;
  if (n > 3)
  {
    f(a, n / 3);
    f(a + 2 * n / 3, n / 3);
  }
}
*/


/*
6. Napišite funkciju koja računa broj načina na koje se od broja x, 0 <= x < n, može doći do n,
   u zadanom broju koraka k. Dopušteni su pomaci +5 i -2, i nije dopušteno pomicati se izvan [0, n>.
   Za x = n smatramo da je jedinstven put do rješenja.

Uočite da ovdje baza ne ovisi o samo jednom argumentu.
*/
/*
int f(int x, int n, int k)
{
  if (x == n)
    return 1;
  if (k == 0 || x < 0 || x > n)
    return 0;
  return f(x + 5, n, k - 1) + f(x - 2, n, k - 1);
}
*/

/*
7. Neka je dan sljedeći jednostavan jezik. Sintaksa:
    - 0 je izraz;
    - 1 je izraz;
    - ZBROJ A B je izraz, ako su A i B izrazi;
    - ništa drugo nije izraz.
   Nad izrazima je definirana funkcija f na sljedeći način:
    - f(0) = 0, f(1) = 1
    - f(ZBROJ A B) = f(A) + f(B)
   Pretpostavimo da se neki izraz X nalazi na standardnom ulazu. Izračunajte f(X).
   Umjesto "ZBROJ" koristite broj 2 (kako bi olakšali čitanje).
   Primjer: f(2 0 2 1 0) = 0 + (1 + 0) = 1

(Ovaj zadatak je na vježbama bio ostavljen za samostalno rješavanje)

SAMI: objasnite zašto u rješenju nije moglo pisati 2 * f() umjesto f() + f().
Razmislite za kakve cjelobrojne funkcije g izraz g(...) + g(...) ima isto značenje kao 2 * g(...).

SAMI: bi li rješenje bilo drugačije strukture da je umjesto zbrajanja bilo oduzimanje ili množenje? Ako da, kako?

SAMI (nevezano uz ovaj kolegij): pokušajte dokazati da svaki izraz u gornjem jeziku ima jedinstveno čitanje,
tj. f(X) je dobro definirana funkcija.
Uputa: indukcijom po n, dokažite da svaki niz sastavljen od 0, 1 i ZBROJ započinje s najviše jednim izrazom,
ako započinje s izrazom duljine n.
*/
int f()
{
  int x; scanf("%d", &x);
  if (x < 2)
    return x;
  return f() + f();
}
