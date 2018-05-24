#include <stdio.h>
#include <stdlib.h>

/*
   niz je slijed objekata u memoriji, tj. (*)
      svaki se element u memoriji nalazi neposredno nakon prethodnog člana,

   neki nedostaci svojstva (*):
    - 1. premještanja: umetanje i brisanje jednog objekta zahtijeva premještanje, u prosjeku, polovice članova niza,
    - 2. realokacije: umetanje ponekad zahtijeva realokaciju, ili je nemoguće (ako ne možemo realocirati memoriju),
    - 3. previše memorije: često radimo s više memorije no što je potrebno za sam niz;  
   još neki, možda manje bitni, nedostaci:
    - (4.) fragmentiranost radne memorije je problem: u memoriji mora postojati slobodan _neprekinut_ blok za cijeli niz
    - (5.) element uvijek član jedinstvenog niza: jedan objekt u memoriji općenito ne može biti član više (neovisnih) nizova;
   osnovna alternativa je odustati od zahtijeva da vrijedi (*),
     tj. dopustiti da se pojedini elementi nalaze bilo gdje u memoriji
   no, kako onda saznati/izračunati adresu i-tog elementa u listi?
   "vezana lista" je osnovno rješenje računanja adrese: u memoriji je neposredno uz svaki element
      spremljena adresa sljedećeg elementa
      (eventualno i adrese prethodnog, prvog, posljednjeg, ...);
      s takvim pristupom ne možemo "izračunati" (u konstantnom vremenu) adresu i-tog elementa,
      ali možemo doputovati do njega: nakon i skokova, na traženom smo elementu
   još neke mogućnosti (osim vezane liste), koje vjerojatno nećemo koristiti na ovom kolegiju:
    - pamtiti adrese naših elemenata A, B, C, ... u pomoćnom nizu [&A, &B, &C, ...]
      tj. i-ti element  čitamo kao *pom[i]
      niz pom (kojeg treba održavati) ima iste nedostatke kao niz [A, B, C, ...],
      ali elementi pomoćnog niza su obično puno manji od elemenata tog niza,
      pa su operacije nad pomoćnim nizom obično prihvatljive efikasnosti
    - čak i ako nam se ne sviđa neki nedostatak svojstva (*), npr. nešto od 1. - (5.),
      u praksi je (puno) bolje koristiti kombinaciju liste i niza (male nizove vezane listom), nego samo listu
      naime, nedostaci svojstva (*) moraju biti vrlo izraženi da bi se skakanje po listi doista isplatilo
      (dereferenciranje pointera je vrlo spora operacija, a uz to raspršenost liste po memoriji
      onemogućava optimiziran rad dohvaćanja memorije u procesoru)
*/

typedef struct _element
{
  int x;
  struct _element *sljed; /* jer kompajler još nije vidio identifikator "element"; vidi dolje za alternative */
} element;

/*
   v. slideove s predavanja, gdje se umjesto "element*" uvodi ime "lista"
   
   osim te dvije verzije, možemo i prebaciti typedef ispred definicije strukture; 
   tada mozemo i unutar definicije strukture koristiti identifikator "element", kako slijedi:
   
   typedef struct _element element;
   struct _element
   {
     int x;
     element* sljed;
   };
   
   -----
   
   U programu je potrebno pamtiti adresu prvog člana liste.
   
   "Prazna lista" znači da je ta adresa 0.
   
   Do preostalih elemenata dolazimo putovanjem (skakanjem) pokazivačima sljed u prethodnicima.
   Posljednji element kao adresu sljedećeg elementa ima 0.
   Svaki objekt tipa element će biti dinamički alociran i dio najviše jedne liste - funkcije ih stoga smiju (i moraju)
   dealocirati kad god ih izbacuju iz liste.
   BITNO: paziti da kod koji radi s listama uzima u obzir mogućnost da je lista prazna (tj. pokazivač je NULL-pokazivač)
*/


/*
  1. Učitavajte cijele brojeve x, x != 0, u listu, dok korisnik ne unese broj 0.
     Iz liste izbacite brojeve x t.d. |x| < n, gdje je n broj učitanih elemenata.
     Ispišite listu.
*/

/* umetanje na pocetak liste mijenja i to sto se smatra pocetkom liste
   - stoga moramo vratiti novi pocetak, ili koristiti varijabilni argument (element **prvi)
   primjer poziva naše verzije (bez var. arg.): prvi = umetni_na_pocetak(prvi, broj); */
element* umetni_na_pocetak(element *prvi, int broj)
/* probajte sami: void umetni_na_pocetak(element **prvi, int broj) gdje bi prvi bio varijabilan argument,
   tada je poziv: umetni_na_pocetak(&prvi, broj); */
{
  element *novi = (element *)malloc(sizeof(element));
  novi->x = broj;
  novi->sljed = prvi;
  return novi;
}

void ispis(element *prvi)
{
  for ( ; prvi != 0; prvi = prvi->sljed) /* ovdje "prvi" mijenamo unutar funkcije, pa se promjene */
    printf("%d ", prvi->x);              /* neće odraziti na varijablu prvi iz glavnog programa   */
    
  /*
    alternativno, koristeci pomocni pokazivac:
    element *e;
    for (e = prvi; e != 0; e = e->sljed)
      printf("%d ", e->x);
  */
}

/* nakon dealokacije, jer se radi o operaciji koja mijenja listu, bismo mogli vratiti novi pocetak (element*),
   no povratna bi vrijednost uvijek bila nula (nakon dealokacije, lista je prazna)
   
   za dealocirati cijelu listu, treba napraviti dvije stvari za svaki element:
    (1) dealocirati ga
    (2) saznati gdje je sljedeći element (kako bismo mogli dealocirati njega i ostatak liste)
   poredak (1) pa (2) nije dobar - ako prvo dealociramo objekt, više ne možemo pristupiti pokazivaču na
     sljedeći objekt
   poredak (2) pa (1) _također_ nije dobar - ako prvo pomaknemo pokazivač trenutnog elementa na sljedeći,
     više ne znamo na kojoj se adresi nalazio prethodni element (kojeg želimo dealocirati)
   oba poretka su ok ako iskoristimo pomoćnu varijablu u koju spremamo onu adresu koja bi se inače izgubila, npr.:
*/
void dealokacija(element *prvi)
{
  while (prvi != 0)
  {
    element *t = prvi;
    prvi = prvi->sljed; /* opet, prvi se mijenja samo lokalno */
    free(t);
  }
}

/* pogledajte 7.1.19 za jos jedno (vrlo slicno) rjesenje za sljedeću operaciju

   kako izbaciti element y iz liste?
   ako je na početku, treba (u nekom poretku):
    - dealocirati ga
    - pospremiti novi prvi: prvi = prvi->sljed
   ako je negdje kasnije, treba (u nekom poretku):
    - znati što je prethodnik x od y
    - postaviti x->sljed na y->sljed
    - dealocirati y
    
   npr. u sljedećoj listi želimo izbaciti podcrtane elemente (pogledajte uvjet iz zadatka)
   _1_ | _2_ | 8 | _4_ | 5
   cijela lista nakon prve faze:
   8 | _4_ | 5
   cijela lista nakon druge faze:
   8 | 5
*/
element* izbacivanje(element *prvi)
{
  element *e;
  int n = 0; /* n nam treba samo zbog uvjeta o brisanju iz zadatka, ne jer nam inače za brisanje treba duljina liste
                (npr. kod nizova, za razliku od listi, nam gotovo uvijek treba dulina za rad) */
  for (e = prvi; e != 0; e = e->sljed)
    ++n;

  /* prva faza - brišemo sve elemente koji ispunjavaju traženi uvjet, a na samom su početku liste */
  while (prvi != 0 && abs(prvi->x) < n) /* poredak je bitan! */
  {
    /* kao i kod dealokacije, iz identičnog razloga nam treba pomoćni pokazivač */
    element *t = prvi;
    prvi = prvi->sljed;
    free(t);
  }

  /* je li možda cijela lista obrisana? moramo provjeriti ovo,
     jer u nastavku je bitno da postoje prethodnici promatranih elemenata */
  if (prvi == 0)
    return prvi;

  /* druga faza (u kojoj brišemo elemente koji nisu na samom početku)
     kod ovih elemenata trebamo prilikom brisanja znati njihove prethodnike, da mozemo preusmjeriti pokazivač sljed;
     stoga u donjoj petlji zapravo iteriramo po _prethodnicima_ elemenata koje ćemo možda brisati, a ne po samim elementima koje možda brišemo; */
  for (e = prvi; e->sljed != 0; )
    if (abs(e->sljed->x) < n)
    {
      element *t = e->sljed;
      e->sljed = e->sljed->sljed; /* uočite da ovdje pridružujemo pokazivač pokazivaču, ne element (objekt) elementu */
      free(t);
    }
    else
      e = e->sljed;

  return prvi;
}

void rjesenje_1()
{
  element *prvi = 0;
  int broj;
  do
  {
    scanf("%d", &broj);
    if (broj != 0)
      prvi = umetni_na_pocetak(prvi, broj);
    /* bilo bi efikasnije ovdje izračunati n, ali za vježbu ćemo ga izračunati kasnije ručno, skakanjem po listi */
  } while (broj != 0);

  prvi = izbacivanje(prvi);
  ispis(prvi);
  dealokacija(prvi);
}

int main()
{
    return 0;
}
