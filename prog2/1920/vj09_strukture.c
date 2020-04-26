#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/*
  1. Definirajte pomoćnu strukturu i funkcije za učitavanje i ispis podataka o osobama.
     Učitajte jednu osobu i izradite njenu kopiju. Kopiji prvo slovo imena postavite na 'D'.
     O osobi je potrebno pamtiti ime (rijec do 30 znakova) i broj godina (int).
*/
typedef struct _osoba {
  char ime[31]; 
  int godina;
} osoba;

/* nekoliko verzija učitavanja i ispisa: */

osoba ulaz_v1() /* najprirodniji način za male strukture: povratna vrijednost funkcije */
{
  osoba x;
  scanf("%s %d", x.ime, &x.godina); /* naravno, u redu su i dva zasebna poziva funkcije scanf */
  return x;
}

void ulaz_v2(osoba *x) /* koristeci varijabilni argument */
{
  scanf("%s %d", (*x).ime, &(*x).godina);
}

/* Operator . uzima objekt tipa strukture i identifikator (varijabla iz definicije strukture).
   Operator -> uzima adresu objekta koji je tipa strukture i identifikator.
   Ako je x objekt tipa strukture (npr. kao u definiciji: str x;), 
   x.ime i (&x)->ime su smisleni izrazi, dok x->ime te (&x)->ime nisu.
   Ako je p adresa strukture (npr. kao u definiciji s *x;), (*p).ime i p->ime su smisleni izrazi,
   dok p.ime te (*p)->ime nisu.

   Nema posebne pokrate za adrese adresa struktura; npr. za s **x, pišemo (*x)->ime ili (**x).ime */

void ulaz_v3(osoba *x)   /* koristeći varijabilni argument i operator ->  */
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

  /* 
    Ovdje dolazi jedna od sljedećih linija:
      x = ulaz_v1();
          ili:
      ulaz_v2(&x);
          ili:       
      ulaz_v3(&x);
  */

  y = x; 
  /* U naredbi  y = x; operator pridruzivanja kopira sadržaj strukture, 
     uključno s elementima polja x.ime.
     To je vrlo različito ponašanje od naredbe "polje = polje2;" koja se 
     (ovisno o tome je li polje deklarirano npr. kao char[] ili char*)
     ili ne kompajla, ili ne radi kopiranje članova polja (već samo adrese polja).
       
     Da da je char ime[31] bilo deklarirano kao char *ime; 
     onda se u naredbi  y = x;  ne bi kopirao sadržaj polja x.ime u polje y.ime, već
     samo pokazivač x u pokazivač y (što je i očekivano).
     Da smo tada htjeli kopiranje i sadržaja (vidi "shallow copy" i "deep copy"),
     trebalo bi napisati npr. for petlju koja kopira sadržaj te po potrebi realocirati y.ime.
  */

  y.ime[0] = 'D'; /* Da smo imali char *ime i napisali samo  y = x;, 
                     ovime bismo promijenili i x.ime[0] u 'D'!      */

  /* Ako bismo htjeli ispisati: */
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
double d(tocka a) /* postoji i funkcija hypot u math.h */
{
  return sqrt(a.x * a.x + a.y * a.y);
}

void rjesenje2()
{
  int i, j, n;
  kruznica *k; /* niz kruznica - ne znamo unaprijed koliko je velik */
  tocka suma;  /* u ovoj smo naredbi mogli dopisati = {0} ili = {0, 0} */

  scanf("%d", &n);

  k = (kruznica *)malloc(n * sizeof(kruznica));
  for (i = 0; i < n; ++i)
    k[i] = ucitaj();

  /* i) dio */
  suma.x = 0; 
  suma.y = 0; 
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

  /* Prolazimo svim studentima i svim njihovim destinacijama (nimalo efikasno) */
  for (i = 0; i < m; ++i)
    for (j = 0; j < s[i].n; ++j)
    {
      int studenata = 0;
      int k, l;
      
      /* Ponovno prolazimo svim studentima i svim njihovim destinacijama, sada
         nas zanima samo destinacija s[i].d, brojimo koliko se puta javlja */
         
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
