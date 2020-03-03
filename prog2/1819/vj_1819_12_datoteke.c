#include <stdio.h>
#include <stdlib.h>


/*
  Datoteke.

  Dvije ``vrste datoteka'':
    - tekstualne: ~ niz linija odvojenih novim redom (ovisno o platformi, najčešće: \n ili \r ili \r\n), obično formatirane
      (npr. broj 18 bit će zapisan kao znamenka 1, pa znamenka 8, umjesto binarnog zapisa 0001 0010
    - binarne: čitaju i pišu se onako kako su zapisane (pročitani bajt = zapisani bajt), obično nisu formatirane
  Obje ``vrste datoteka'' su zapravo nizovi bajtova (slično kao C-ovski nizovi); razlika je u tome kako se čitaju i pišu.

  C za prazan red apstrahira od platforme, i čita/piše ispravne
  znakove umjesto '\n'; npr. fprintf(datoteka, "\n") će na Windowsima umjesto "\n" zapisati niz bajtova "\r\n".


  OTVARANJE I ZATVARANJE

  FILE* = objekt koji predstavlja datoteku
  Struktura FILE bilježi neke informacije o datoteci; npr. jesmo li čitajući datoteku prešli kraj datoteke.
  Te informacije ne čitamo izravno, nego koristeći pomoćne funkcije; npr. feof govori jesmo li prošli kraj datoteke.

  FILE *fopen(adresa, tip)  i  void fclose(datoteka)

  Mogući stringovi tip: samo čitanje: r
                        samo pisanje: w
                        samo pisanje na kraj: a
                        č. i p. postojeće datoteke: r+ (ako ne postoji, greška)
                        č. i p. nove datoteke: w+ (ako postoji, bit će prebrisana)
                        č. i p. na kraj dat.: a+ (ako postoji, neće se prebrisati, ako ne postoji, stvorit će se)
  Uočite da ovo ne pokriva sve slučajeve - npr. ne postoji varijanta r+ koja će stvoriti datoteku ako ona ne postoji.


  Primjer korištenja:

  FILE *f = fopen("a.txt", "r");
  if (f == 0) {
    printf("greska\n");
    exit(1);
  }
  ... rad s datotekom f ...
  fclose(f);


  RAD S DATOTEKAMA

  int fprintf(datoteka, ...) radi točno isto što i printf(...), ali piše u datoteku umjesto na std. izlaz
    vraća broj uspješno ispisanih znakova

  int fscanf(datoteka, ...) radi točno isto što i scanf(...), ali učitava iz datoteke umjesto sa std. ulaza
    vraća EOF (negativna cjelobrojna konstantna koja označava end-of-file) ako se niti jedan traženi argument nije uspio pročitati,
    inače broj učitanih objekata zatraženih u format stringu

  int fgetc(datoteka) vraća EOF ili pročitani znak; npr. često je potrebno čitanje cijele datoteke znak-po-znak:
    int znak; [int jer za char nije garantirano da je signed, a želimo dopustiti negativne vrijednosti zbog EOF-a]
    while ((znak = fgetc(f)) != EOF)
      printf("%c", znak);

  char* fgets(str, n, datoteka) učitava prvih n - 1 simbola linije (linija uključuje pripadni '\n') u polje str; polje ima barem n znakova
    funkcija vraća str (adresu prvog znaka) ako je uspjesna, inace 0 (NULL)
    char c[100];
    while ((fgets(c, 100, f)) != 0) [ovdje linija u datoteci, bez \n, smije imati do uključno 98 znakova]
      printf("%s", c);

  int fputs(str, datoteka), isto što i fprintf(datoteka, "%s", str)

  int fputc(znak, datoteka)

  int feof(datoteka) provjerava jesmo li pokušali čitati izvan datoteke (jesmo li prešli kraj datoteke čitajući ju)

  void rewind(datoteka) vraća "kursor" na početak datoteke, ako ju želimo ponovno čitati (efikasnije od zatvaranja i ponovnog otvaranja)

  int fflush(datoteka) - kod upisivanja u datoteku, prije fizičkog pisanja u datoteku, podaci koje želimo pisati prvo odlaze
    u međuspremnik u radnoj memoriji; ova funkcija prebacuje memoriju iz međuspremnika u datoteku
    (obično nije potrebno ručno pozivati; ali korisno npr. kad nakon pisanja u datoteku odmah želimo čitati iz nje, v. predavanja)

  Koristit ćemo i: sprintf, sscanf; kao fprintf, fscanf, ali ulaz je niz znakova (tj. char *)

  v. u slideovima predavanja: fread, fwrite, fseek, ftell

*/

/*
 *  1. napišite funkciju
 *         void s(char* d, int n)
 *  koja ispisuje sve sume stupaca matrice dimenzija m x n iz datoteke na adresi a.
 *  Možete koristiti polja.
 *
 */

void rjesenje1_s(char *d, int n) {
    int j;
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
            if (feof(fp))
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
 * 2. (slično zadatku s kolokvija 13/14)
 *
 * U datotekama 1.txt, 2.txt, ..., n.txt (za nepoznat n) zapisan je isti tekst.
 * No, umjesto ispravnog znaka, na nekim mjestima u nekim datotekama pise znak +.
 * Napišite dio programa koji rekonstruira tekst u datoteku sve.txt.
 *
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
 3.
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

