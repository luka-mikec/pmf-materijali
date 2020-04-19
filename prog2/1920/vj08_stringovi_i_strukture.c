#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Važno: sve pomoćne funkcije iz string.h koje mijenjaju stringove neće (re)alocirati memoriju,
        niti provjeravati ima li dovoljno memorije. Jedan od razloga za to je što te funkcije ne mogu znati
        primaju li pokazivač na dinamički alociranu (te stoga potencijalno rastezljivu) memoriju.

 strlen(x) = duljina stringa (broj znakova do, ne i uključujući, nul-znaka '\0')
 strcmp(x, y) < 0, ako je x leksikografski ispred y
             == 0, ako su x i y znak-po-znak jednaki
              > 0, ako je y leksikogradski ispred x
 strcasecmp, kao strcmp, ali ignorira razliku mala/velika slova
 strcpy(x, y): u x se prepisuje string y; tj. događa se sljedeće:
    x[0] = y[0], x[1] = y[1], ..., x[k] = y[k], gdje je k minimalan indeks takav da y[k] == '\0'
 strcat(x, y): na kraj stringa x nadovezuje se string y (i dodaje se '\0' na kraju tako dobivenog stringa)


 Postoje verzije gornjih funkcija koje rade odgovarajuće operacije samo s prvih n znakova ('\0' nije uračunat u tih n):

 strncat(x, y, n): nadovezivanje samo prvih (najviše) n znakova iz y (i dodavanje '\0')
 strncmp(x, y, n): usporedba prvih (najviše) n znakova iz x, odnosno y
 strncpy(x, y, n): kopiranje prvih (najviše) n znakova iz y (i dodavanje '\0')

 Ako proslijeđeni string(ovi) ima(ju) manje od n znakova, ove se funkcije ponašaju
 jednako kao osnovne verzije.
*/


/*
 1. Učitajte riječ  s  za čiju duljinu pretpostavite da je do 19 znakova i cijeli broj n >= 0.
    Kreirajte najkraću riječ sastavljenu od nadovezanih kopija riječi  s,
    tako da duljina kreirane riječi bude barem  n.
    Možete koristiti zaglavlje string.h.
*/
void rjesenje1() /* rješenje koristeci realokaciju */
{
    int n;
    char s[20];
    char *s2 = (char *)malloc(1 * sizeof(char)); /* za pocetak samo znak \0 */
    strcpy(s2, ""); /* ili s2[0] = '\0'; */

    scanf("%s %d", s, &n);

    while (strlen(s2) < n)
    {
        s2 = (char *)realloc(s2, (strlen(s) + strlen(s2) + 1) * sizeof(char));
        strcat(s2, s);
    }

    printf("%s", s2); /* testiramo */
    free(s2);
}

/*
  ZADATAK: riješite gornji zadatak bez realokacije,
  tj. unaprijed izračunajte potreban broj znakova za string  s2 (jedno rješenje je u skripti).
*/


/*
2. Napišite program koji učitava prirodni broj  n  te potom  n  stringova navedenih u zasebnim recima
   (zanima nas cijeli redak, dakle bilo kakav niz znakova, osim znaka za novi red \n).
   Niz treba sortirati uzlazno (uz zanemarivanje razlike velikih/malih slova) i ispisati.
   Možete koristiti string.h.
*/

void rjesenje2()
{
    char **stringovi;
    int i, j, n;
    char znak; /* ne znamo duljinu stringova pa ih ucitavamo znak-po-znak - tu cemo spremati trenutni znak */

    scanf("%d\n", &n); /* bitno je pročitati \n nakon %d, inače se prvi string čita iz retka u kojem je upisan broj */

    /* inicijaliziramo niz u kojem pamtimo adrese redaka */
    stringovi = (char**)malloc(n * sizeof(char*));
    for (i = 0; i < n; ++i)
    {
        /* alociramo prostor za trenutni string; za početak, samo jedan bajt (toliko nam treba za spremiti prazan string: jedan nul-znak) */
        stringovi[i] = (char *)malloc(1 * sizeof(char));
        /* inicijaliziramo stringovi[i] na prazan niz, tj. na ""; ekvivalentno nizovi[i][0] = '\0';  */
        strcpy(stringovi[i], "");

        znak = 0;
        while (znak != '\n')
        {
            scanf("%c", &znak);
            if (znak != '\n')
            {
                stringovi[i] = (char *)realloc(stringovi[i], (strlen(stringovi[i]) + 1 + 1) * sizeof(char));
                /* varijabla znak nije string nego char; ali (&znak) je "skoro" string, samo u memoriji (općenito)
                   nedostaje '\0' na adresi (&znak) + 1; no, mi znamo koliko charova treba kopirati (1 char),
                   a strncat nece provjeravati postoji li '\0' nakon što pređemo broj znakova koji zatražimo kopirati */
                strncat(stringovi[i], &znak, 1);
            }
        }
    }

    /* sortiramo */
    for (i = 0; i < n; ++i)
        for (j = i + 1; j < n; ++j)
            if (strcasecmp(stringovi[i], stringovi[j]) > 0)
            {
                char *t = stringovi[i];
                stringovi[i] = stringovi[j];
                stringovi[j] = t;
            }

    /* ispisujemo */
    for (i = 0; i < n; ++i)
        printf("%s\n", stringovi[i]);

    /* dealociramo */
    for (i = 0; i < n; ++i)
        free(stringovi[i]);
    free(stringovi);
}

/*
 3. (ovaj zadatak je slican kolokviju 16/17)
    Napisite funkciju
        char *prosiri(char *tekst, char **s, char **p, int n)
    koja kao parametre prima string tekst, nizove od n stringova s (skraćenice) i p (potpune fraze), te broj n >= 0.

    Treba stvoriti novi string dobiven iz teksta tekst,
    ali svaku pojavu nekog stringa x iz s treba zamijeniti odgovarajucim (istog rednog broja) stringom iz p.

    Pretpostavite da nijedan string iz s nije podstring drugog stringa iz s,
    kao i da se kroz transformaciju neće pojaviti nove pojave stringova iz s u novom stringu.
    Možete koristiti string.h.
*/

char *prosiri(char *tekst, char **s, char **p, int n)
{
    char *novi = (char *)malloc(1 * sizeof(char));
    strcpy(novi, "");

    /* pokazivac  tekst  cemo kroz donju petlju pomicati malo-po-malo udesno */
    while (*tekst != '\0') /* dok nismo dosli do kraja teksta */
    {
        int i, pronadena = 0;

        /* zapocinje li tekst nekom skracenicom? ako da, preskacemo ju, a nadovezujemo puni oblik */
        for (i = 0; i < n; ++i)
            if (strncmp(tekst, s[i], strlen(s[i])) == 0)
            {
                novi = (char *)realloc(novi, (strlen(novi) + strlen(p[i]) + 1) * sizeof(char));
                strcat(novi, p[i]);

                pronadena = 1;
                tekst += strlen(s[i]); /* pomicemo se u tekstu za potreban broj mjesta */
                break;
            }

        /* tekst ne zapocinje skracenicom, preskacemo prvo slovo i kopiramo ga u rezultat */
        if (!pronadena)
        {
            novi = (char *)realloc(novi, (strlen(novi) + 1 + 1) * sizeof(char));
            strncat(novi, tekst, 1);
            tekst += 1; /* naravno, ili ++tekst */
        }
    }

    return novi; /* ne zaboraviti free u glavnom programu */
}

/*
  U jeziku C moguce je definirati vlastiti tip podatka povezivanjem u cjelinu vec postojecih tipova.

  Definicija novog tipa (strukture):
  typedef struct pomocno_ime {
      int a;
      int b;
      char c;
      void* d;
  } ime;

  (skripta - detaljnije o drugim načinima definicije struktura)

  Jedna konvencija: pomocno_ime je podvlaka + ime, kao u sljedećem primjeru:

  npr. typedef struct _kompleksni_broj {
    double real, imag;
  } kompleksni_broj;

  Dva ekvivalentna nacina definicija varijable ciji je tip struktura:
    struct pomocno_ime x; (struct _kompleksni_broj x;)
    ime x;                (kompleksni_broj x)

  Kasnije cemo vidjeti zasto postoji i pomocno_ime, i ime.

  Varijabla kompleksni_broj  z  ima znacenje kao, otprilike, definicija dvije varijable: double z_real, z_imag;
  Dakle, ta jedna varijabla  z  u sebi "sadrži" dvije varijable.

  Pristup clanovima (varijablama) unutar strukture radi se pomocu pomocu operatora . (tocka):
    ime.identifikator = nova_vrijednost; (npr.: z.real = 5;)

  Kao i kod polja, moguce je odjednom inicijalizirati sve elemente, ali samo u naredbi u kojoj se varijabla definira:
    ime x = {5, 6, 'x', 0};
  Donji kod nije dobar (tocka-zarez je izmedu, tj. to su dvije naredbe):
    ime x; x = {5, 6, 'x', 0};

  Dopušteni operatori nad varijablama čiji je tip struktura:
      pridruživanje  = (ali ne i ==)
      pristup članovima  .
      adresa objekta  &
      veličina objekta  sizeof()

*/


int main(void)
{
    return 0;
}


