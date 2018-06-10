#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Datoteke.

  Dvije ``vrste datoteka'':
    - tekstualne: niz linija odvojenih s \n ili \r ili \r\n (ovisno o platformi),
                  same linije sadrže uobičajene (i prikazive, osim u iznimnim okolnostima) ASCII znakove,
                  tekst editori na mjestima gdje se nalaze \n, \r ili \r\n prelamaju tekst.
    - binarne (ostale)
  Obje ``vrste datoteka'' su zapravo nizovi bajtova (slično kao C-ovski nizovi); razlika je u tome kako se čitaju i pišu.
  U praksi, jedina razlika je što će kod tekstualnih datoteka C čitati/pisati ispravne
  znakove umjesto '\n'; npr. fprintf(datoteka, "\n") će po potrebi umjesto "\n" zapisati "\r\n".


  Osnovne funkcije.

  FILE *fopen(adresa, tip)  i  void fclose(datoteka)

  Mogući stringovi tip: samo čitanje: r
                        samo pisanje: w
                        samo pisanje na kraj: a
                        č. i p. postojeće datoteke: r+ (ako ne postoji, greška)
                        č. i p. nove datoteke: w+ (ako postoji, bit će prebrisana)
                        č. i p. na kraj dat.: a+ (ako postoji, neće se prebrisati, ako ne postoji, stvorit će se)
  Uočite da ovo ne pokriva sve slučajeve - npr. ne postoji varijanta r+ koja će stvoriti datoteku ako ona ne postoji.

  Ako želimo binaran rad (tj. da C čita i piše \n, \r i \r\n doslovno), dodajemo još 'b' na kraj tipa.
  (Novi standard dopušta još i 'x', koji onemogućava pisanje preko postojeće datoteke, ali ni to ne pokriva sve slučajeve)


  Primjer korištenja:

  FILE *f = fopen("a.txt", "r");
  if (f == 0) {
    printf("greska\n");
    exit(1);
  }
  ... rad s datotekom f ...
  fclose(f);

  Struktura FILE bilježi neke informacije o datoteci; npr. jesmo li čitajući datoteku došli do kraja datoteke.
  No te informacije ne čitamo izravno, nego koristeći pomoćne funkcije; npr. feof govori jesmo li došli do kraja.


  int fprintf(datoteka, ...) radi točno isto što i printf(...), ali piše u datoteku umjesto na std. izlaz
    vraća broj uspješno ispisanih znakova

  int fscanf(datoteka, ...) radi točno isto što i scanf(...), ali učitava iz datoteke umjesto sa std. ulaza
    vraća EOF (negativna cjelobrojna konstantna koja označava end-of-file) ako se niti jedan traženi argument nije uspio pročitati,
    inače broj učitanih objekata

  int fflush(datoteka) - kod upisivanja u datoteku, prije fizičkog pisanja u datoteku, podaci koje želimo pisati prvo odlaze
    u međuspremnik u radnoj memoriji; ova funkcija prebacuje memoriju iz međuspremnika u datoteku
    (obično nije potrebno ručno pozivati; ali korisno npr. kad nakon pisanja u datoteku odmah želimo čitati iz nje, v. predavanja)

  int fgetc(datoteka) vraća EOF ili pročitani znak; npr. često je potrebno čitanje cijele datoteke znak-po-znak:
    int znak; [int jer za char nije garantirano da je signed, a EOF je negativna vrijednost]
    while ((znak = fgetc(f)) != EOF)
      printf("%c", znak);

  char* fgets(str, n, datoteka) učitava prvih n - 1 simbola linije (linija uključuje pripadni '\n') u polje str; polje ima barem n znakova
    funkcija vraća str (adresu prvog znaka) ako je uspjesna, inace 0 (NULL)
    char c[100];
    while ((fgets(c, 100, f)) != 0) [ovdje linija u datoteci, bez \n, smije imati do uključno 98 znakova]
      printf("%s", c);

  void rewind(datoteka) vraća "kursor" na početak datoteke, ako ju želimo ponovno čitati (efikasnije od zatvaranja i otvaranja)

  int feof(datoteka) provjerava jesmo li pokušali čitati izvan datoteke

  int fputs(str, datoteka)

  int fputc(znak, datoteka)

  v. u slideovima predavanja: fread, fwrite, fseek, ftell
*/

/*
  1. Napišite dio programa koji otvara datoteku ulaz.txt,
     i prepisuje ju u izlaz.txt.
     Nakon svakog retka treba se nalaziti broj znakova (osim \n) u retku:
      "abc def" |-> "abc def [7]"
*/
void rjesenje_1()
{
  int n = 0;
  char c;
  FILE *ulaz, *izlaz;
  if ((ulaz = fopen("ulaz.txt", "r")) == 0)
    exit(1);
  if ((izlaz = fopen("izlaz.txt", "w")) == 0)
    exit(1);


  while ((c = fgetc(ulaz)) != EOF)
  {
    if (c == '\n')
    {
      fprintf(izlaz, " [%d]\n", n);
      n = 0;
    }
    else
    {
      fprintf(izlaz, "%c", c);
      ++n;
    }
  }

  if (n > 0) /* nakon posljednje linije mozda nema znaka \n  */
    fprintf(izlaz, " [%d]", n);


  fclose(ulaz);
  fclose(izlaz);
}

/*
  2. Napišite program koji učitava formatiranu tekstualnu datoteku čija je adresa u stringu argv[1].

     U datoteci je zapisan program sastavljen od najviše 100 definicija procedura odvojenih bjelinom.

     Definicija procedure: def ime naredbe end, gdje je:
      - ime: neprazna riječ (ne sadrži razmake) duljine najviše 20 znakova;
      - naredbe: možda prazan niz naredbi odvojenih bjelinama, koji sadrži najviše 100 naredbi.

     Naredba ima jedan od sljedećih oblika:
      - ispis n, gdje je n pozitivan prirodan broj,
      - poziv n ime, gdje je n pozitivan prirodan broj, a ime je ime neke procedure (vidi gore).

     Nakon učitavanja izvršite proceduru main. Izlaz zapišite u istu datoteku.
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

/* tu će doći funkcija koja će simulirati izvršavanje programa */

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
    a[i].n = 0;
    fscanf(f, " def %s", a[i].ime); /* razmak na pocetku je bitan! */
    if (feof(f)) break;

    /* kroz sve naredbe u proceduri */
    for (j = 0; j < 100; ++j)
    {
      /*
        ovdje treba probati pročitati naredbu: ili ćemo pročitati naredbu ("ispis" ili "poziv"), ili "end";
        u prvom slučaju treba pohraniti informacije o naredbi u a[i].naredbe[j], a inače prekinuti petlju
      */
    }
  }

  /* ovdje treba pozvati funkciju koja će simulirati izvršavanje programa */

  fclose(f);
}




/*
 3. (ovaj zadatak se nije stigao riješiti na vježbama 12; na vježbama 13 će se dovršiti prethodni zadatak, te raditi zadaci koje su studenti predložili)

   Pohranjuju se zapisi o položenim ispitima, koji sadrže sljedeće podatke: student (string do 50 znakova, koji ne sadrži ':'), 
     kolegij (string do 100 znakova, koji ne sadrži '/') i ocjena (cijeli broj).
   Niz redova oblika
     student:kolegij/ocjena
   čuva se u __tekstualnoj__ datoteci ocjene.txt.

   Napišite dio programa koji ispisuje sve studente i njihove prosjeke u datoteku studenti.txt, sortirane silazno po prosjecima.
   Redak treba biti oblika
    prezime ime: prosjek
   a prosjek je zaokružen na tri decimale.

   Biti će najviše 100 studenata. Možete ih čuvati u pomoćnom nizu.
*/


typedef struct _student
{
    int suma_ocjena;
    int broj_ocjena;
    char ime[51];
} student;

void rjesenje_3()
{
  student s[100];

  int n = 0; /* broj studenata */
  int i, j;

  /* pomocne varijable za ucitavanje: */
  char ime[51];
  char kolegij[101];
  int ocjena;

  FILE *f = fopen("ocjene.txt", "r");
  FILE *g = fopen("studenti.txt", "w");

  if (f == 0 || g == 0)
    exit(1);

  while (fscanf(f, "%[^:]:%[^/]/%d\n", ime, kolegij, &ocjena) == 3)
  {
    /* printf(".%s.%s.%d.\n", ime, kolegij, ocjena); */

    /* gdje umetnuti studenta? */
    for (i = 0; i < n; ++i)
      if (strcmp(ime, s[i].ime) == 0)
        break;

    if (i == n) /* prvi zapis za ovog studenta, treba inicijalizirati njegove varijable */
    {
      s[i].suma_ocjena = 0;
      s[i].broj_ocjena = 0;
      strcpy(s[i].ime, ime);
      ++n;
    }

    s[i].suma_ocjena += ocjena;
    ++s[i].broj_ocjena;
  }

  for (i = 0; i < n; ++i)
    for (j = i + 1; j < n; ++j)
      if (s[i].suma_ocjena * s[j].broj_ocjena < s[j].suma_ocjena * s[i].broj_ocjena)
      {
          student tmp = s[i];
          s[i] = s[j];
          s[j] = tmp;
      }

  for (i = 0; i < n; ++i)
    fprintf(g, "%s %s: %.3lf\n", s[i].prezime, s[i].ime, (double)s[i].suma_ocjena/s[i].broj_ocjena);

  fclose(f);
  fclose(g);
}




int main(int argc, char **argv)
{
  main_2(argc, argv);

  return 0;
}
