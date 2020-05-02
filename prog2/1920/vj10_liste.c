#include <stdio.h>
#include <stdlib.h>

/*
   Niz (npr. int niz[100]) je slijed objekata u memoriji, tj. ima sljedeće svojstvo:
      (*) svaki je element niza u memoriji smješten neposredno uz njemu susjedne elemente niza.
   
   Dakle, nema "praznog prostora" između susjednih elemenata niza.

   Osnovna korist od svojstva (*) je što možemo brzo locirati član niza u memoriji kojem znamo indeks.
   
   Ali svojstvo (*) donosi i neke probleme:
      1. Umetanje ili brisanje elementa niza zahtijeva premještanje, u prosjeku, polovice preostalih članova niza.
      2. Umetanje novog elementa ponekad zahtijeva realokaciju, ili je čak nemoguće (ako ne možemo realocirati memoriju).
      3. U radu s nizovima često alociramo više memorije no što je potrebno za niz u trenutku alokacije.
   Još neki, možda manje bitni, nedostaci koje donosi svojstvo (*):
      4. U memoriji mora postojati _neprekinut_ slobodan blok za cijeli niz,
         pa alokacija niza može zakazati čak i kad je količina slobodne memorije puno veća od zatražene veličine niza.
      5. Pojedini element niza član je jedinstvenog niza: 
         jedan objekt u memoriji općenito ne može biti član više (neovisnih) nizova (osim ako se preklapaju).
   
   Vezane liste su najjednostavniji pristup pohrani kolekcije objekata koji nema svojstvo (*). 
   Kod vezanih listi dopuštamo da se pojedini elementi nalaze bilo gdje u memoriji.
   
   Kod vezanih listi općenito ne možemo izračunati memorijsku poziciju člana kolekcije samo na temelju njegovog indeksa,
   jer ta pozicija može biti bilo gdje u memoriji.
   
   Stoga vezane liste neposredno uz svaki element kolekcije spremaju adresu sljedećeg elementa.
      (u nekim varijantama i adrese prethodnog, prvog, posljednjeg, ... elementa);
   S takvim pristupom očito ne možemo izračunati (u konstantnom vremenu) adresu i-tog elementa,
      ali možemo doputovati do njega: nakon i skokova po elementima, nalazimo se na traženom elementu.

   Napomena: u stvarnom softveru, nedostaci svojstva (*) moraju biti vrlo izraženi da bi se skakanje po listi 
      doista isplatilo. Skakanje je iznimno spora operacija u odnosu.
      To vrijedi čak i ako samo prolazimo listom element-po-element, dakle kad nas prelazak na sljedeći element
      "stoji" samo jedno čitanje adrese u pokazivaču i potom čitanje sadržaja na toj adresi.
      Glavni razlog za to je što se kod čitanja radne memorije u cache sprema i bliska okolina zatražene memorije.
      Kod nizova je to vrlo korisno jer se dohvaćanjem i-tog člana, odmah dohvaćaju i susjedni članovi.
      Za usporedbu, na jednom prosječnom računalu u trenutku pisanja ovog teksta (2020.), 
      prolazak listom je čak 42 puta sporiji od prolaska nizom.
*/

typedef struct _element
{
  int x;
  struct _element *sljed; 
} element;

/*
   Vidi i slideove s predavanja, gdje se umjesto "element*" uvodi ime "lista".
   
   Ako želimo i unutar definicije strukture koristiti identifikator "element" (umjesto nezgrapnog struct _element):
   
   typedef struct _element element;
   struct _element
   {
     int x;
     element* sljed;
   };
   
   U programima na listu referiramo pamćenjem adrese prvog člana liste.
   
   "Prazna lista" znači da je ta adresa 0 (nul-pokazivač).
   
   Do preostalih elemenata dolazimo putovanjem (skakanjem) pokazivačima u elementima liste (pokazivač "sljed").
   Posljednji element kao adresu sljedećeg elementa ima 0, na taj način signaliziramo kraj liste.
   Svaki objekt tipa element ćemo dinamički alocirati.
   Smatramo da je svaki element naših listi dio najviše jedne liste.
   Funkcije stoga smiju (i moraju) dealocirati takve strukture kad god ih izbacuju iz liste 
   (iznimka su situacije kad izbačeni element nastavlja "živjeti": npr. kad se prebacuje u drugu listu).
   
   BITNO: paziti da svaki dio koda koji radi s listama uzima u obzir mogućnost da je lista prazna (nul-pokazivač).
*/


/*
  1. Učitavajte cijele brojeve x, x != 0, u listu u bilo kojem poretku, dok korisnik ne unese broj 0.
     Iz liste izbacite brojeve x t.d. |x| < n, gdje je n broj učitanih elemenata.
     Ispišite listu.
*/

/* 
Napravit ćemo funkciju za umetanje na početak liste (na početak umjesto na kraj jer je tako efikasnije).
Umetanje na početak liste mijenja što se smatra početkom liste.
Stoga ova funkcija mora vratiti novi početak, ili koristiti varijabilni argument (element **prvi)
   
Primjer poziva naše verzije (bez var. arg.): prvi = umetni_na_pocetak(prvi, broj); 

Probajte sami napisati alternativu: void umetni_na_pocetak(element **prvi, int broj).
Tada je poziv: umetni_na_pocetak(&prvi, broj);
*/

element* umetni_na_pocetak(element *prvi, int broj)
{
  element *novi = (element *)malloc(sizeof(element));
  novi->x = broj;
  novi->sljed = prvi;
  return novi;
}

/* Pomoćna funkcija za ispis čitave liste */
void ispis(element *prvi)
{
  element *e;
  for (e = prvi; e != 0; e = e->sljed)
    printf("%d ", e->x);

  /* alternativan kod; ovdje "prvi" mijenamo unutar funkcije, pa se promjene neće odraziti na "prvi" iz glavnog programa */
  for ( ; prvi != 0; prvi = prvi->sljed) 
    printf("%d ", prvi->x);              
}

/* 
   Pomoćna funkcija za dealokaciju.
   Funkcije koje možda mijenjaju početak liste uvijek trebaju nekako informirati pozivatelja o novom početku.
   Međutim, nakon dealokacije bi povratna vrijednost uvijek bila nula (nakon dealokacije, lista je prazna),
   pa nema potrebe da vraćamo tu nulu.
   
   Za dealocirati cijelu listu, treba napraviti dvije stvari za svaki element:
    (1) dealocirati taj element,
    (2) pomaknuti se na sljedeći element (kako bismo mogli dealocirati njega i ostatak liste).
   Poredak (1) pa (2) nije dobar: ako prvo dealociramo objekt, više ne možemo pristupiti pokazivaču na
     sljedeći objekt.
   Poredak (2) pa (1) _također_ nije dobar: ako prvo pomaknemo pokazivač trenutnog elementa na sljedeći,
     više ne znamo na kojoj se adresi nalazio prethodni element (kojeg želimo dealocirati)
   Oba poretka su ok ako iskoristimo pomoćnu varijablu u koju spremamo onu adresu koja bi se inače izgubila, npr.:
*/
void dealokacija(element *prvi)
{
  element *e;
  for (e = prvi; e != 0; )
  {
    element *t = e;
    e = e->sljed;
    free(t);
  }
}

/* Kako izbaciti element e iz liste?
   Ako je na početku, treba (u nekom poretku, možda uz pomoćne varijable):
    - dealocirati ga,
    - zapamtiti što je novi prvi element: prvi = prvi->sljed
   Ako je negdje kasnije, treba (u nekom poretku):
    - znati što je prethodnik ee od e,
    - postaviti ee->sljed na e->sljed,
    - dealocirati e.
    
   Npr. u sljedećoj listi želimo izbaciti podcrtane elemente (pogledajte uvjet iz zadatka)
     _1_ | _2_ | 8 | _4_ | 5
   Izgled liste nakon prve faze, tj. brisanja svih elemenata na početku:
     8 | _4_ | 5
   Izgled liste nakon druge faze:
     8 | 5
   
   Možete pogledati i 7.1.19 u skripti za još jedno (vrlo slično) rješenje za brisanje elemenata iz liste.
*/
element* izbacivanje(element *prvi)
{
  element *e;
  int n = 0; /* n nam treba samo zbog uvjeta o brisanju iz zadatka, ne jer nam inače za brisanje treba duljina liste */
  for (e = prvi; e != 0; e = e->sljed)
    ++n;
  /* Sad varijabla n sadrži broj elemenata liste. */

  /* Prva faza: dok je lista neprazna, brišemo sve elemente na početku liste koji ispunjavaju traženi uvjet. */
  while (prvi != 0  &&  abs(prvi->x) < n) /* poredak je bitan! */
  {
    /* kao i kod dealokacije, iz identičnog razloga nam treba pomoćni pokazivač */
    element *t = prvi;
    prvi = prvi->sljed;
    free(t);
  }

  /* Je li možda cijela lista obrisana? Moramo provjeriti ovo,
     jer u nastavku je bitno da postoje prethodnici promatranih elemenata. */
  if (prvi == 0)
    return prvi;

  /* Druga faza (u kojoj brišemo elemente koji nisu na samom početku).
     Kod ovih elemenata trebamo prilikom brisanja znati njihove prethodnike.
     Stoga u donjoj petlji zapravo iteriramo po _prethodnicima_ elemenata koje ćemo (možda) brisati, 
     a ne po samim elementima koje (možda) brišemo. */
  for (e = prvi; e->sljed != 0; ) /* Što je prvi element bez sljedbenika u listi? Element e takav da e->sljed == 0 */
    if (abs(e->sljed->x) < n)
    {
      element *t = e->sljed;
      e->sljed = e->sljed->sljed; /* Uočite da ovdje pridružujemo pokazivač pokazivaču, ne element elementu */
      free(t);
    }
    else
      e = e->sljed;

  return prvi;
}

void rjesenje_1()
{
  element *prvi = 0; /* Lista je na početku prazna */
  int broj;
  do
  {
    scanf("%d", &broj);
    if (broj != 0)
      prvi = umetni_na_pocetak(prvi, broj);
    /* bilo bi efikasnije ovdje računati n, ali za vježbu ćemo ga izračunati kasnije ručno, skakanjem po listi */
  } while (broj != 0);

  prvi = izbacivanje(prvi); /* izbacivanje je funkcija koja potencijalno mijenja početak liste: zato prvi = ... */
  ispis(prvi);
  dealokacija(prvi);
}

int main()
{
    return 0;
}
