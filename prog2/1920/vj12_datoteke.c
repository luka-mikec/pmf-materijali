#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Postoje dvije "vrste" datoteka.
    - tekstualne: sastavljene od linija međusobno odvojenih posebnim znakovima \n ili \r ili \r\n (ovisno o platformi),
                  same linije sadrže uobičajene (i prikazive, osim u iznimnim okolnostima) znakove,
                  a tekst editori na mjestima gdje se nalaze \n, \r ili \r\n prelamaju tekst.
                  (Danas je uobičajeno kodiranje pojedinih znakova UTF-8, koje je kompatibilno s ASCII standardom.)
                  Npr. broj 18 u tekstualnoj datoteci bit će zapisan kao znamenka 1, pa znamenka 8,
                  umjesto binarnog zapisa 00010010
    - binarne: čitaju i pišu se onako kako su zapisane (pročitani bajt = zapisani bajt), 
               pročitani bajt je uvijek točno onaj koji doista piše u datoteci

    - binarne (ostale)
    
  Obje "vrste" datoteka su nizovi bajtova (slično kao nizovi u jeziku C); razlika je u tome kako ih interpretiramo.
  U praksi, jedina razlika je što će kod tekstualnih datoteka C čitati/pisati ispravne
  znakove umjesto '\n'; npr. fprintf(datoteka, "\n") će po potrebi umjesto "\n" zapisati "\r" ili "\r\n".
  Npr. fprintf(datoteka, "\n") će na Windowsima umjesto "\n" zapisati niz bajtova "\r\n".


  Osnovne funkcije:
  FILE *fopen(char* adresa, char* tip)  i  void fclose(FILE* datoteka)

  Mogući stringovi tip: samo čitanje: r
                        samo pisanje: w
                        samo pisanje na kraj: a
                        č. i p. postojeće datoteke: r+ (ako datoteka već ne postoji, greška)
                        č. i p. nove datoteke: w+ (ako postoji, bit će prebrisana)
                        č. i p. na kraj dat.: a+ (ako postoji, neće se prebrisati, ako ne postoji, stvorit će se)
  Ovi tipovi ne pokrivaju sve slučajeve - npr. ne postoji varijanta tipa r+ koja će stvoriti datoteku ako ona ne postoji.

  Ako želimo binaran rad (tj. da C čita i piše znakove \n, \r i \r\n doslovno), dodajemo još 'b' na kraj tipa.
  (Noviji standardi dopuštaju i "x", koji onemogućava pisanje preko postojeće datoteke, ali ni to ne pokriva sve slučajeve)


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
    vraća EOF (negativna cjelobrojna konstantna koja označava kraj datoteke (End Of File))
    ako se niti jedan traženi argument nije uspio pročitati, a inače broj učitanih objekata (formata)

  int fflush(datoteka) kod upisivanja u datoteku, prije fizičkog pisanja u datoteku, podaci koje želimo pisati prvo odlaze
    u međuspremnik u radnoj memoriji; ova funkcija prebacuje svu memoriju iz međuspremnika u samu datoteku
    (obično ovo nije potrebno ručno pozivati; ali korisno npr. kad nakon pisanja u datoteku odmah želimo čitati iz nje)

  int fgetc(datoteka) vraća EOF ili pročitani znak; npr. često je potrebno čitanje cijele datoteke znak-po-znak:
    int znak; [int jer tip char ne mora moći spremati negativne vrijednosti, a EOF je negativna vrijednost]
    while ((znak = fgetc(f)) != EOF)
      printf("%c", znak);

  char* fgets(str, n, datoteka) učitava prvih n - 1 simbola linije (linija uključuje pripadni '\n') u polje str; 
    polje ima prostor za n char elemenata, i funkcija vraća str (adresu str koja je proslijeđena) ako je uspješna, 
    a inače 0 (NULL)
    
    char c[100];
    while ((fgets(c, 100, f)) != 0) 
      printf("%s", c);
      
    linija u datoteci u gornjem primjeru smije imati do uključno 98 znakova: pretposljednji element polja rezerviran je za
    znak \n, a posljednji za znak \0 (nul-znak)

  void rewind(datoteka) vraća "kursor" na početak datoteke, 
    što je korisno ako ju želimo ponovno čitati (efikasnije je od zatvaranja i ponovnog otvaranja)

  int feof(datoteka) provjerava jesmo li pokušali čitati izvan datoteke (je li "kursor" na nepostojećem znaku)

  int fputs(str, datoteka), isto što i fprintf(datoteka, "%s", str)

  int fputc(znak, datoteka)
  
  Koristit ćemo i sprintf i sscanf; funkcije analogne fprintf i fscanf, 
    ali rade s nizovima znakova (tj. char *) a ne datoteka.

  vidi u slideovima predavanja: fread, fwrite, fseek, ftell
*/

/*
  1. Napišite dio programa koji otvara datoteku ulaz.txt,
     i prepisuje ju u izlaz.txt.
     Nakon svakog retka treba se nalaziti broj znakova u tom retku u ulaz.txt (ne računajući znak \n),
     npr.  "abc def" |-> "abc def [7]"
*/
void rjesenje_1()
{
  int n = 0;
  int c;
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

  if (n > 0) /* nakon posljednje linije mozda nema znaka \n; takvu situaciju prepoznajemo koristeći brojač n  */
    fprintf(izlaz, " [%d]", n);


  fclose(ulaz);
  fclose(izlaz);
}



/*
   2. napišite funkciju
          void s(char* d, int n)
   koja ispisuje sve sume stupaca matrice dimenzija m x n iz datoteke na adresi d.
   Možete koristiti polja.
*/

void rjesenje1_s(char *d, int n) {
    int j;
    /* 
       Zašto polje za sume stupaca: matricu nemamo u memoriji, pa možemo ili n puta proći cijelom datotekom,
       svaki put promatrajući samo jedan stupac; ili jednom proći čitavom datotekom.
       U potonjem slučaju (koji i koristimo), moramo negdje pamtiti (među)sume svih stupaca.
    */
    double *sume = (double *)malloc( n * sizeof(double) );
    FILE *fp = fopen(d, "r");
    if (fp == NULL)
        return;

    for (j = 0; j < n; ++j)
        sume[j] = 0;

    while (!feof(fp))
    {
        for (j = 0; j < n; ++j)
        {
            double broj;
            fscanf(fp, "%lf", &broj);
            if (feof(fp)) /* ovo se s ispravnim datotekama može dogoditi samo za j == 0 */
                break;

            sume[j] += broj;
        }
    }

    fclose(fp);

    for (j = 0; j < n; ++j) {
        printf("%lf ", sume[j]);
    }

    free(sume);
}

/*
   3. (slično zadatku s kolokvija 13/14)
  
   U datotekama 1.txt, 2.txt, ..., n.txt (za nepoznat n) zapisan je isti tekst.
   No, umjesto ispravnog znaka, na nekim mjestima u nekim datotekama pise znak +.
   Napišite dio programa koji rekonstruira tekst u datoteku sve.txt.
   Ako se znak + javlja na nekom mjestu u svim datotekama, onda je on i ispravan znak.
 */

void rjesenje2_k1314()
{
    int i;
    FILE *izlaz = fopen("sve.txt", "w+");
    if (izlaz == 0)
        exit(1);

    for (i = 1;  ; ++i)
    {
        FILE *ulaz;
        int znak;
        char adresa[100];

        /* formiramo adresu datoteke */
        sprintf(adresa, "%d.txt", i);

        if ((ulaz = fopen(adresa, "r")) == 0)
            break;

        while ((znak = fgetc(ulaz)) != EOF)
            if (i == 1 || znak != '+')
                fputc(znak, izlaz);
            else
            {
                fflush(izlaz);
                fgetc(izlaz);
                fflush(izlaz);
                /* alternativno: fseek(izlaz, 1, SEEK_CUR);
                 * potencijalni problemi ako bismo i \n uključili među 'znakove' u tekstu zadatka */
            }
        fclose(ulaz);
        rewind(izlaz);
    }

    fclose(izlaz);
}



/*
 4.
   Niz redova oblika
     ime:kolegij/ocjena
   čuva se u __tekstualnoj__ datoteci ocjene.txt.

   Pritom znamo da su ime i kolegij neprazni nizovi s do 10 znakova (ne sadrže ':' i '/'), a ocjena prirodni broj

   Napišite dio programa koji ispisuje sve studente i njihove prosjeke u datoteku studenti.txt, sortirane silazno po prosjecima.
   Redak treba biti oblika
     ime: prosjek

   Biti će najviše 100 studenata. Možete koristiti pomoćni niz.

   SAMI: bez niza, pomoću fseek/ftell ili rewind
*/

#include "string.h"

typedef struct _student
{
    int suma_ocjena;
    int broj_ocjena;
    char ime[11];
} student;

void rjesenje_3()
{
    student s[100];

    int n = 0, i, j;

    /* pomocne varijable za ucitavanje: */
    char ime[11], kolegij[11];
    int ocjena;

    /* datoteke */
    FILE *f = fopen("ocjene.txt", "r");
    FILE *g = fopen("studenti.txt", "w");

    if (f == NULL || g == NULL)
        exit(1);

    while (fscanf(f, "%11[^:]:%11[^/]/%d\n", ime, kolegij, &ocjena) == 3)
    {
        /* gdje umetnuti studenta? */
        for (i = 0; i < n; ++i)
            if (strcmp(ime, s[i].ime) == 0)
                break;

        if (i == n) /* prvi zapis u ocjene.txt za ovog studenta, treba inicijalizirati varijable za tog novog studenta */
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
        fprintf(g, "%s: %.3lf\n", s[i].ime, (double)s[i].suma_ocjena/s[i].broj_ocjena);

    fclose(f);
    fclose(g);
}
