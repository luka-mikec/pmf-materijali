#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  2.Napišite program koji učitava formatiranu tekstualnu datoteku čija je adresa u stringu argv[1].

     U datoteci je zapisan program sastavljen od najviše 100 definicija procedura odvojenih bjelinom.

     Definicija procedure: def ime naredbe end, gdje je:
      - ime: neprazna riječ (ne sadrži razmake) duljine najviše 20 znakova;
      - naredbe: možda prazan niz naredbi odvojenih bjelinama, koji sadrži najviše 100 naredbi.

     Naredba ima jedan od sljedećih oblika:
      - ispis n, gdje je n prirodan broj,
      - poziv n ime, gdje je n prirodan broj, a ime je ime neke procedure (vidi gore).

     Nakon učitavanja (možete koristiti pomoćni niz) izvršite proceduru main. Izlaz zapišite u istu datoteku.
     Pretpostavite da je datoteka sintaksno ispravna.
     Možete koristiti string.h.

     Primjer:

     def f
       ispis 1
     end

     def g
       ispis 2
       poziv 3 f
       ispis 2
     end

     def main
       poziv 2 g
     end

     ispisuje: 2 1 1 1 2 2 1 1 1 2
*/

typedef struct _naredba {
  char tip[6]; /* "ispis" ili "poziv"  */
  int n;
  char ime[21]; /* ako se radi o naredbi poziv */
} naredba;

typedef struct _procedura {
  char ime[21];
  naredba naredbe[100];
  int n;
} procedura;

/* otkomentirajte donje komentare na 4 mjesta za pregledniji ispis s uvlacenjem */
void izvrsi(FILE *f, procedura a[], int n, char *ime  /* , int dubina */)
{
  int i;
  for (i = 0; i < n; ++i)
    if (strcmp(a[i].ime, ime) == 0)
    {
      int j;
      for (j = 0; j < a[i].n; ++j)
      {
        if (strcmp(a[i].naredbe[j].tip, "ispis") == 0)
          fprintf (f, /*"%*s" */    "%d\n",    /* dubina, "", */   a[i].naredbe[j].n);
        else /* if (strcmp(a[i].naredbe[j].tip, "poziv") == 0) */
        {
          int k;
          for (k = 0; k < a[i].naredbe[j].n; ++k)
            izvrsi(f, a, n, a[i].naredbe[j].ime /*,  dubina + 1 */);
        }
      }
      break;
    }
}

int main_2(int argc, char **argv)
{
  procedura a[100];
  int i, j; /* brojaci za procedure i naredbe */
  FILE *f = fopen(argv[1], "r+");
  if (f == 0)
  {
    printf("Greska.\n");
    return 1;
  }

  /* kroz sve procedure */
  for (i = 0; i < 100; ++i)
  {
    fscanf(f, " def %s", a[i].ime); /* razmak na pocetku je bitan! */
    if (feof(f)) break;
    a[i].n = 0;

    /* kroz sve naredbe u proceduri */
    for (j = 0; j < 100; ++j)
    {
      /*
        ovdje treba probati pročitati naredbu: ili ćemo pročitati početak naredbe ("ispis" ili "poziv"), ili "end";
        u prvom slučaju treba pohraniti informacije o naredbi u a[i].naredbe[j], a inače prekinuti petlju
      */

      fscanf(f, "%s", a[i].naredbe[j].tip);
      if (strcmp(a[i].naredbe[j].tip, "end") == 0)
        break;

      fscanf(f, "%d", &a[i].naredbe[j].n);

      if (strcmp(a[i].naredbe[j].tip, "poziv") == 0)
        fscanf(f, "%s", a[i].naredbe[j].ime);

      ++ a[i].n;
    }
  }

  izvrsi(f, a, i, "main" /* , 0 */);
  fclose(f);
}



/* 13/14, 2. zadatak
   http://degiorgi.math.hr/prog2/kolokviji/p2-kolokvij-1314-2.pdf

*/

/* #include ... int main() { */
void rjesenje_1314()
{
  int i;
  FILE *izlaz = fopen("secret.message", "w");
  if (izlaz == 0)
    exit(1);

  for (i = 1; i < 10; ++i)
  {
    FILE *ulaz;
    int znak;
    char adresa[] = "?.part";
    adresa[0] = '0' + i;

    if ((ulaz = fopen(adresa, "r")) == 0)
      break;

    while ((znak = fgetc(ulaz)) != EOF)
      if (i == 1 || znak != ' ')
        fputc(znak, izlaz);
      else
        fseek(izlaz, 1, SEEK_CUR);
    fclose(ulaz);
    rewind(izlaz);
  }

  fclose(izlaz);
}




/* 15/16, 1. zadatak a)

   http://degiorgi.math.hr/prog2/kolokviji/p2-kolokvij-1516-2.pdf

   pretpostavke:
     - sve su osobe sadrzane u polju predavaonica i samo tamo, tj.
       za svaki OIB u memoriji je tocno jedna adresa strukture s tim OIB-om, i to negdje u polju predavaonica
       [zadatak ne tvrdi da to nuzno vrijedi, ali sluzbeno rjesenje pretpostavlja takvu situaciju]
     - polja prijatelja su dinamicki alocirana (razumna pretpostavka; alternativno bi trebao postojati mehanizam javljanja pozivatelju
       da zelimo obrisati neku osobu, pa bi pozivatelj odlucio sto napraviti s njenim nizom prijatelja)
     - konacno, u bilo kakvoj stvarnoj primjeni ove funkcije, ona bi trebala vratiti novi broj osoba u polju predavaonica (zbog toga ovdje funkcija vraća int)
     - b) dio zadatka (pogledajte sluzbeno rjesenje) pretpostavlja da "par ima 80% zajedničkih prijatelja" znači
       da je 80% ukupnog broja ljudi koji su prijatelji barem jednoj osobi u paru, prijatelj obje osobe u paru
       (dakle, situacija kad X ima prijatelje A, B, C, D, E; a Y prijatelje A, B, C, D, F nije takva situacija, iako je 80% prijatelja od X
        ujedno prijatelj od Y, a 80% prijatelja od Y ujedno prijatelj od X)

*/
typedef struct _osoba {
  char ime[51];
  char prezime[51];
  char spol;
  int godina;
  long long oib;

  int *pri; /* polje indeksa osoba (prijatelja) unutar polja predavaonica */
  int n;

  int brisati;
} osoba;


int brisi(osoba* pred, int n)
{
  int i, j, k;

  /* 1. dio: odrediti koje osobe (ne) treba brisati */
  for (i = 0; i < n; ++i)
  {
    /* ima li ova osoba vise od 3 prijatelja gdje spol == 'm'? */
    int m = 0;
    for (j = 0; j < pred[i].n; ++j)
      if (pred[pred[i].pri[j]].spol == 'm')
        ++m;
    pred[i].brisati = m > 3; /* pamtimo treba li brisati i-tu osobu; ne brisemo odmah jer je mozda ta osoba neciji muski prijatelj */
  }

  /* 2. dio: za svaku osobu ...  */
  for (i = 0; i < n; ++i)
  {
    /* ... iz njenog niza prijatelja brisemo indekse svih osoba koje treba brisati
       dakle, za svakog prijatelja (j-tog) osobe (i-te): */
    for (j = 0; j < pred[i].n;  )
    {
      /* treba li brisati j-tog prijatelja iz niza prijatelja? */
      if (pred[pred[i].pri[j]].brisati)
      {
        /* ako da, brisemo (premjestanjem) */
        for (k = j + 1; k < pred[i].n; ++k)
          pred[i].pri[k - 1] = pred[i].pri[k];
        -- pred[i].n;
      }
      else
        ++j;
    }
  }

  /* 3. dio: konacno, brisemo i same osobe; ovo je zakomplicirano cinjenicom da se kod pomicanja indeksa u polju predavaonica
             moraju osvjeziti i odgovarajuci indeksi u nizovima prijatelja (u sluzbenom rjesenju nedostaje ovaj postupak);
             npr. ako 1 ima prijatelja 3, a obrišemo 2 (i pomaknemo kasnije indekse prema dolje), tada 1 za prijatelja ima bivšeg 3, sadašnjeg 2
  */
  for (i = 0; i < n; )
  {
    if (pred[i].brisati)
    {
      free(pred[i].pri); /* jer smo pretpostavili da je ovo dinam. aloc., i da je osoba samo u memoriji samo u ovom nizu pred */

      for (j = i + 1; j < n; ++j)
        pred[j - 1] = pred[j];
      -- n;

      /* osvjezavanje indeksa u nizovima prijatelja */
      for (j = 0; j < n; ++j)
        for (k = 0; k < pred[j].n; ++k)
          if (pred[j].pri[k] > i) /* jer i-ta je osoba obrisana, a sve nakon nje pomaknuto za jedan indeks unatrag */
            -- pred[j].pri[k];
    }
    else
      ++i;
  }

  return n;
}


/* 15/16, 3. zadatak
   http://degiorgi.math.hr/prog2/kolokviji/p2-kolokvij-1516-2.pdf
*/

typedef struct _dionicar {
  char ime[31];
  int dio;
  struct _dionicar *sljed;
} dionicar;

/* donji kod manje-vise ispunjava obrazac koda za brisanje elemenata iz liste (s dvije faze),
   razlike su što ovdje prvo treba locirati dioničara 1; te što brišemo samo jednog dioničara (dioničara 2)
*/
dionicar* trgovina(dionicar* first, char* ime1, char* ime2)
{
  dionicar *e, *d1; /* pomocni pokazivac za iteriranje, te mjesto dionicara 1 */

  /* prvo trazimo dionicara 1 */
  for (d1 = first; d1 != 0; d1 = d1->sljed)
    if (strcmp(d1->ime, ime1) == 0)
      break;

  if (d1 == 0)
    return first;

  /* potom trazimo dionicara 2, pribrajamo njegove dionice dionicaru 1, te ga (dionicara 2) brisemo */

  /* prva faza - je li ime2 na početku liste? */
  if (strcmp(first->ime, ime2) == 0)
  {
    dionicar *t = first; /* t je drugi dionicar  */
    first = first->sljed;

    /* prije dealokacije, uzimamo dionice */
    d1->dio += t->dio;
    free(t);

    /* pronasli smo trazene dionicare itd. - mozemo izaci */
    return first;
  }

  /* u ovom trenutku bismo inace (kad nesto brisemo u listi) provjerili je li cijela lista obrisana
     - no u nasoj situaciji sigurno nije (nismo usli u gornji if, dakle prvi element postoji) */

  /* druga faza - je li ime2 negdje kasnije u listi? */
  for (e = first; e->sljed != 0; )
    if (strcmp(e->sljed->ime, ime2) == 0)
    {
      dionicar *t = e->sljed; /* t je drugi dionicar */
      e->sljed = e->sljed->sljed;

      /* prije dealokacije, uzimamo dionice  */
      d1->dio += t->dio;

      free(t);

      /* pronasli smo trazene dionicare - mozemo izaci */
      return first;
    }
    else
      e = e->sljed;

  /* prvi dionicar pronaden, drugi nije, stoga ne radimo nista i vracamo se iz funkcije: */
  return first;
}


dionicar* najbogatiji(dionicar** first)
{
  dionicar *e, *nova = 0, *nova_poslj = 0; /* pomocni pokazivac za iteriranje; pocetak nove liste; posljednji element nove liste */
  int dionicara = 0, bogatih;

  /* nema dionicara: nema najbogatijih, i ne mijenjamo pocetnu listu */
  if (*first == 0)
    return 0;

  /* koliko je dioničara? */
  for (e = *first; e != 0; e = e->sljed)
    ++ dionicara;

  /* broj najbogatijih koje izdvajamo */
  bogatih = dionicara / 20;
  if (bogatih == 0)
    bogatih = 1;

  /* u svakom koraku: pronađemo najbogatijeg (novi) u staroj listi (first), premjestimo ga u novu listu */
  while (bogatih --)
  {
    int max_dio = (*first)->dio; /* klasicno trazenje maksimalnog: pretpostavimo da je prvi najbogatiji i trazimo protuprimjer */
    for (e = *first; e != 0; e = e->sljed)
      if (e->dio > max_dio)
        max_dio = e->dio;


    /* sada (prvog ako ih je vise) dionicara s max_dio dionica premjestamo iz liste first u listu nova;
       slicno kao u funkciji "odvoji" ovdje: https://github.com/luka-mikec/pmf-materijali/blob/master/vj11_liste.c
    */

    if ((*first)->dio == max_dio) /* iz gornjih uvjeta znamo da *first != 0, inace bi i to trebalo provjeriti */
    {
      /* umetanje ispod se vrsi na kraj (nove) liste, pa ne moramo pamtiti *first->sljed prije umetanja */
      if (nova == 0)
        nova = nova_poslj = *first;
      else
        nova_poslj = nova_poslj->sljed = *first;

      *first = (*first)->sljed;

      nova_poslj->sljed = 0;
      continue; /* idemo na iduću iteraciju (koja će izvući sljedećeg najbogatijeg) */
    }

    /* u ovom trenutku bismo inace (kad nesto brisemo u listi) provjerili je li cijela lista obrisana
       - no u nasoj situaciji sigurno nije (nismo usli u gornji if, dakle prvi element postoji) */

    /* druga faza brisanja: */
    for (e = *first; e->sljed != 0; )
      if (e->sljed->dio == max_dio)
      {
        if (nova == 0)
          nova = nova_poslj = e->sljed;
        else
          nova_poslj = nova_poslj->sljed = e->sljed;

        e->sljed = e->sljed->sljed;

        nova_poslj->sljed = 0;
        break; /* prekidamo for petlju druge faze */
      }
      else
        e = e->sljed;
  }

  return nova;
}

int main(int argc, char **argv)
{
  return 0;
}
