#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/*
  1. Definirajte pomoćnu strukturu i funkcije za učitavanje i ispis podataka o osobama.
     Učitajte osobu i izradite njenu kopiju. Kopiji prvo slovo imena postavite na 'D'.
     O osobi je potrebno pamtiti ime (rijec do 30 znakova) i starost (int).
*/
typedef struct _osoba {
  char ime[31];
  int godina;
} osoba;

/* nekoliko verzija učitavanja i ispisa: */

osoba ulaz_v1() /* najprirodniji nacin za male strukture: povratna vrijednost funkcije */
{
  osoba x;
  scanf("%s %d", x.ime, &x.godina); /* naravno, mogli smo i 2 zasebna poziva funkcije scanf */
  return x;
}

void ulaz_v2(osoba *x) /* koristeci varijabilni argument */
{
  scanf("%s %d", (*x).ime, &(*x).godina);
}

/* operator . uzima objekt tipa strukture i identifikator (varijabla iz definicije strukture)
   operator -> uzima adresu objekta koji je tipa strukture i identifikator
   ako je x objekt tipa strukture (npr. kao u definiciji s x;), x.ime i (&x)->ime su smisleni izrazi,
       dok x->ime te (&x)->ime nisu
   ako je p adresa strukture (npr. kao u definiciji s *x;), (*p).ime i p->ime su smisleni izrazi,
       dok p.ime te (*p)->ime nisu

   nema posebne pokrate za adrese adresa struktura; npr. za s **x, pišemo (*x)->ime ili (**x).ime */

void ulaz_v3(osoba *x)   /* koristeci varijabilni argument,  */
{                        /* i operator -> umjesto * i . */
  scanf("%s %d", x->ime, &x->godina);
}

void ispis_v1(osoba x)
{
  printf("%s %d\n", x.ime, x.godina);
}

void ispis_v2(osoba *x)
{
  printf("%s %d\n", (*x).ime, (*x).godina);
}

void ispis_v3(osoba *x)
{
  printf("%s %d\n", x->ime, x->godina);
}

void rjesenje1()
{
  osoba x, y;

  /* x = ulaz_v1();
         ili:
     ulaz_v2(&x);
         ili:       */
  ulaz_v3(&x);

  y = x; /* ovdje operator pridruzivanja kopira sadržaj strukture, uključno sa elementima polja x.ime!
            pritom, da je ime bilo deklarirano kao char *ime;, onda bi se ovdje kopirao samo pokazivač,
            što je i prirodno */

  y.ime[0] = 'D'; /* da smo imali char *ime, ovime bismo promijenili i x.ime[0] u 'D'! */

  /* ako bismo htjeli ispisati: */
  ispis_v1(x); /* ili ispis_v2(&x) itd. */
  ispis_v1(y);
}

/*
  2. Definirajte pomoćne strukture točku (dvije koordinate) i kružnicu (središte i polumjer).
     Učitajte broj n >= 0, te potom n kružnica.
      i)  Ispišite sumu središta svih učitanih kružnica (točke sumirajte kao vektore).
      ii) Sortirajte kružnice uzlazno s obzirom na udaljenost od ishodišta, te ih ispišite.
*/

typedef struct _tocka {
  double x, y;
} tocka;

typedef struct _kruznica {
  tocka  s;
  double r;
} kruznica;

kruznica ucitaj()
{
  kruznica k;
  scanf("%lf %lf %lf", &k.s.x, &k.s.y, &k.r);
  return k;
}

void ispisk(kruznica k)
{
  printf("%lf %lf %lf\n", k.s.x, k.s.y, k.r);
}

/* za i) */
tocka plus(tocka a, tocka b) /* SAMI: void plus_jednako(tocka *a, tocka b) koja pribraja b na *a */
{
  tocka rez = { a.x + b.x, a.y + b.y };
  return rez;
}

/* za ii) */
double d(tocka a) /* postoji i funkcija hypot u math.h; pogledajte i biblioteku complex.h te funkciju cabs */
{
  return sqrt(a.x * a.x + a.y * a.y);
}

void rjesenje2()
{
  int i, j, n;
  kruznica *k; /* niz kruznica - ne znamo unaprijed koliki je */
  tocka suma;  /* u ovoj smo naredbi mogli dopisati = {0} ili = {0, 0} */

  scanf("%d", &n);

  k = (kruznica *)malloc(n * sizeof(kruznica));
  for (i = 0; i < n; ++i)
    k[i] = ucitaj();

  /* i) dio */
  suma.x = 0; /* da je n nuzno bio pozitivan, mogli smo napisati  suma = k[0]; */
  suma.y = 0; /* a petlju zapoceti s i = 1; no ovako ne znamo postoji li k[0] */
  for (i = 0; i < n; ++i)
    suma = plus(suma, k[i].s);
  printf("%lf %lf\n", suma.x, suma.y);

  /* ii) dio */
  for (i = 0; i < n; ++i)
    for (j = i + 1; j < n; ++j)
      if (d(k[i].s) > d(k[j].s))
      {
        kruznica t = k[i];
        k[i] = k[j];
        k[j] = t;
      }

  for (i = 0; i < n; ++i)
    ispisk(k[i]);

  free(k);
}



/*
  3. Definirajte strukturu destinacija, koja sadrži naziv (spremljen kao char naziv[31]), te cijenu (int).
     Definirajte strukturu student koja sadrži ime (spremljeno kao char ime[31]), pozitivan cijeli broj n,
     te pokazivač na niz od n destinacija (destinacija*)

     Pretpostavljamo da su destinacije jednakog naziva iste destinacije,
     te da u nizu destinacija pojedinog studenta nema ponavljanja.

     Napišite funkciju "naj" koja prima pozitivan broj m, te niz od m studenata.
     Funkcija vraća cijenu najpopularnije (jedne ako ih je više) destinacije.
     Možete koristiti funkcije iz string.h
*/

typedef struct _destinacija {
    char naziv[31];
    int  cijena;
} destinacija;

typedef struct _student {
    char ime[31];
    int n;
    destinacija *d;
} student;

int naj(int m, student* s)
{
  int i, j;
  destinacija naj_d;
  int         naj_studenata = 0;

  /* prolazimo svim studentima i svim njihovim destinacijama */
  /* ovo nije ucinkovit pristup */
  for (i = 0; i < m; ++i)
    for (j = 0; j < s[i].n; ++j)
    {
      int studenata = 0;
      int k, l;
      for (k = 0; k < m; ++k)
        for (l = 0; l < s[k].n; ++l)
          if (strcmp(s[i].d[j].naziv, s[k].d[l].naziv) == 0)
            ++studenata;
      if (studenata > naj_studenata)
      {
        naj_d = s[i].d[j];
        naj_studenata = studenata;
      }
    }

  return naj_d.cijena;
}

int main(void)
{
  return 0;
}
