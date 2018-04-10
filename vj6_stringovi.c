#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/*
  Termin "string" u C-u označava niz znakova koji završava (prvim) znakom '\0'
  taj niz znakova je možda podniz nekog duljeg niza znakova
  '\0' je u memoriji spremljen kao bajt 0, nema grafičku reprezentaciju, značenje je "kraj stringa"

  npr. char a[] = "abc";  -->  a = {'a', 'b', 'c', '\0')

  duljina a _kao niza_ je 4, ali _kao stringa_ je 3!
  (duljina stringa a je zapravo indeks prvog znaka '\0' u nizu a)

  pretp. b = {'a', 'b', 'c', '\0', 'x', '\0'}

  duljina b kao niza je 5, ali kao stringa 3 (do prvog '\0'!)
  (niz b "sadrži" više stringova, no fraza "string b" referira na prvi)

  Zbog '\0' treba paziti koliko se memorije alocira (uvijek znak više od duljine stringa).

  #include <ctype.h>  - korisne funkcije za rad sa znakovima
  #include <string.h> - korisne funkcije za rad sa stringovima (npr. strlen)

  ctype.h, string.h - korisne funkcije
*/

/*
1. (5.1.1.) Napišite dio programa koji učitava jednu riječ duljine najviše 17 znakova, te ispisuje:
a) tu riječ
b) tu rijeć bez prvog znaka
c) treći znak te riječi
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
  SAMI - skripta i predavanja (i Prog 1) - učitavanje riječi, redaka itd.
*/

/*
2. (5.3.1.) Napišite funkciju void b(char *a) koja iz stringa a "briše" sve samoglasnike.
  Pritom funkcija ne smanjuje samo polje koje sadrži string, jer možda nije din. aloc., a i možda čuva još nešto osim stringa.

  Možete koristiti ctype.h.

  npr. [a][b][c][d][e][f][0][???]... => [b][c][d][f][0][???]...
*/
void b_1(char *a) /* jedno rješenje */
{
  int i, j;

  for (i = 0; a[i] != '\0'; ) /* ++i želimo samo kad trenutni znak nije samoglasnik! */
  {
    char c = tolower(a[i]);
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
      for (j = i; a[j] != '\0'; ++j) /* uočite - ovime kopiramo i završnu nulu */
        a[j] = a[j + 1];
    else
      ++i;
  }
}
/*
 Ova funkcija prima polje koje se mora moći mijenjati!
 za kakve a funkcionira b(a)?
 char a[] = "x"; - ok, spremanje znakova 'x', '\0' u novi (promjenjiv) niz a.
 char  *a = "x"; - nije ok,  ali ne zbog tipa (char*), već jer je a adresa konstantnog niza
 char str[] = "x"; char  *a = str;  - ok
 char  *a = (char*) malloc((1 + 1) * sizeof(char)); a[0] = 'x'; a[1] = '\0';  - ok
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
 SAMI - pogledajte rješenje iz skripte; općenito, puno je odluka oko implementacije 
        ove funkcije, što rezultira s vrlo mnogo mogućih rješenja
*/


/*
3. (5.3.3.) Napišite funkciju koja kao argument prima jedan string, te iz njega briše svako treće __slovo__.
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
ali tako da sve prirodne brojeve (pretpostavljamo da su u dekadskom zapisu) zapisuje u bazi 2.
Npr. "2 + 2 = 4" |-> "10 + 10 = 100".

Dva načina za realokaciju: prvo saznati finalan broj znakova (= broj znakova - broj znamenaka + broj znakova-binarnih-znamenaka).

Drugi: (manje efikasan, ali jednostavniji): proširivati polje po potrebi.

Postupak: čitati znak-po-znak. Ako znak nije znamenka, kopira se. 
          Inače, pročita se ostatak broja, zapisuje u bazi 2 od posljednje prema prvoj znamenci, 
          invertira se poredak znamenki, nastavlja se čitati početni niz.
*/

/* prima pokazivač na dinamički alociran string, produljuje ga i umeće znak c */
char* dodaj(char *x, char c)
{
  int n = strlen(x);
  x = (char*)realloc(x, (n + 1 + 1) * sizeof(char));
  x[n] = c;
  x[n + 1] = '\0';
  return x;
}

/* prima pokazivač na string x, invertira segment koji počinje na poc, duljine d */
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

  char *y = (char*)malloc(sizeof(char));
  y[0] = '\0';

  for (i = 0; i < strlen(x); )
  {
    if (!isdigit(x[i]))
    {
      dodaj(y, x[i]);
      ++i;
      continue;
    }

    for (broj = 0; isdigit(x[i]); ++i)
      broj = 10 * broj + (x[i] - '0');
    /* zbog gornje petlje i njenog ++i, sada se nalazimo na prvom
       znaku nakon broja u dekadskom zapisu u početnom stringu; zbog toga u nastavku nema ++i */

    if (broj == 0)
      dodaj(y, '0'); 
    else
    {
      poc = strlen(y);
      d = 0;
      while (broj > 0)
      {
        dodaj(y, (broj % 2) + '0'); /* broj % 2 je znamenka, + '0' daje ascii kod znamenke */
        broj = broj / 2;
        ++d;
      }
      poredak(y, poc, d); 
    }
  }

  return y;
}
/* SAMI - umjesto zapisa binarnih znamenaka u obrnutom poretku, pa inverzije, zapišite ih u ispravnom poretku
          (jedan elegantan način bi bio koristeći pomoćnu rekurzivnu funkciju koja izvlači binarne znamenke,
           a zapis radi nakon rekurzivnog poziva)  
*/


/*
Vježba
----
 1. napišite (možda rekurzivnu) funkciju koja provjerava jesu li zagrade '(' i ')' u (char *) stringu ispravno sparene,
    tj. svaka ima svoj par.
 
 2. napišite rekurzivnu funkciju koja prima string koji sadrži ispravno sparene zagrade (i ništa drugo).
    Funkcija alocira i vraća leksikografski najmanji niz koji možete proizvesti brisanjem pod-stringa "()" jedan ili više puta (sekvencijalno).
   Implementacija može koristiti proizvoljno mnogo pomoćnih dinamički alociranih nizova.
*/

int main()
{


  return 0;
}
