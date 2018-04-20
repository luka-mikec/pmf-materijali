#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/* Rjesenja zadataka s prvog kolokvija 16/17. Neka rjesenja su vrlo slicna sluzbenima, neka su drugacija. */ 


/*
    1. zadatak (usporedite s https://github.com/luka-mikec/pmf-materijali/blob/master/vj2_rekurzije.c#L35)
*/

int rel_prost(int a, int b)
{
  return b ? rel_prost(b, a % b) : a;
}

/* dobar - jesu li grane koje sadrze ovaj poziv ispunile uvjet o rel. pr.?
   d     - duljina do sada izgradene grane */
int f(int m, int n, int max_x, int max_y, int dobar, int d)
{
  int i, j, rez = 0;
  if (m < 0 || n < 0)
    return 0;
  if (m == 0 && n == 0)
    if (dobar && d % 2 == 0)
      return 1;
    else
      return 0;
  for(i = max_x; i <= m; ++i) {
    for(j = max_y; j <= n; ++j)
      rez += f(m - i, n - j, i, j, dobar || rel_prost(i,j), d + 1);
  }
  return rez;
}


/*
    2. zadatak
*/

void BlokDijagonalna(double X[55][35], int m, int n)
{
  int i, j, k, dijag = 0;
  int max_k = ((m < n) ? m : n) / 2;

  for (k = max_k; k > 0; k--)
  {
    dijag = 1;
    for (i = 0; i < k; ++i)
      for (j = 0; j < k; ++j)
        if (X[i][j] != X[i+k][j+k] || X[i][j+k] != 0 || X[i+k][j] != 0)
          dijag = 0;

    if (dijag) {
      for (i = 0; i < 2*k; ++i) {
        for (j = 0; j < 2*k; ++j)
          printf("%lf ", X[i][j]);
        printf("\n");
      }
      break;
    }
  }

  if (!dijag)
    printf("Nema takvih podmatrica!\n");
}

/*
    4. zadatak (dio)
*/


char* izbaci(char *recenica)
{
  int i;
  int max_broj = -1, max_pocetak, max_duljina; /* za rijec s maksimalnim brojem slova: koliko ih je, gdje pocinje rijec, koliko je dugacka */
  int broj; /* broj velikih slova u trenutnoj rijeci */

  char *rijec = NULL;

  if (recenica[0] == '\0')
    return rijec;

  /* trazimo rijec s najvecim brojem velikih slova */
  for (i = 0; recenica[i] != '\0'; ++i)
  {
    int pocetak = i;

    for (broj = 0; recenica[i] != '+'; ++i)
      if (isupper(recenica[i]))
        ++broj;

    if (broj > max_broj) {
      max_broj = broj;
      max_pocetak = pocetak;
      max_duljina = i - pocetak;
    }
  }

  /* prvo kopiramo trazenu rijec u novi string */
  rijec = (char*) malloc((max_duljina + 1) * sizeof(char));
  for (i = 0; i < max_duljina; ++i)
    rijec[i] = recenica[max_pocetak + i];
  rijec[max_duljina] = '\0';

  /* potom brisemo tu rijec, ali i plus nakon nje */
  for (i = max_pocetak; recenica[i + max_duljina + 1] != '\0'; ++i)
    recenica[i] = recenica[i + max_duljina + 1];
  recenica[i] = '\0'; /* ne zaboraviti znak \0; alternativno, u gornjoj for petlji, + 2 umjesto + 1 */

  return rijec;
}

/*
    3. zadatak (dio)
*/


/* korak, broj_mrkvi, dimenzije, indeksi su varijabilni argumenti - uvijek im se pristupa s * na pocetku! */
/* ploče u zadatku NISU specificirane kao stringovi - zato se vodi evidencija o dimenzijama (nema \0)  */
char** igraj(char **buseni, int *korak, int *broj_mrkvi, int **dimenzije, int **indeksi)
{
  int i,
      lokacija = -1, /* što je korisnik odabrao? */
      duljina, /* duljina trenutne ploce, prije brisanja */
      pocetak, kraj; /* pocetak i kraj podniza kojeg brisemo kad se odabere '*' */
  char znak;

  while(*broj_mrkvi > 0)
  {
    /* povecavamo polja */
    *indeksi = (int *) realloc(*indeksi, (*korak + 1)*sizeof(int));
    buseni = (char**)realloc(buseni, (*korak + 2) * sizeof(char*));
    *dimenzije = (int*)realloc(*dimenzije, (*korak + 2)*sizeof(int));

    /* ispisujemo duljinu polja, ucitamo novi indeks i spremamo ga */
    duljina = (*dimenzije)[*korak];
    printf("%d\n", duljina);
    scanf("%d", &lokacija);
    (*indeksi)[*korak] = lokacija;

    /* koji je znak upisan? */
    znak = buseni[*korak][lokacija];

    if(znak == '$')
    {
      /* treba generirati novi string, bez ove mrkve */
      --*broj_mrkvi;
      (*dimenzije)[*korak + 1] = duljina - 1;
      buseni[*korak + 1] = (char*) malloc((*dimenzije)[*korak + 1] * sizeof(char));

      /* kopiramo prvo komad prije odabrane lokacije, pa poslije */
      for (i = 0; i < lokacija; ++i)
        buseni[*korak + 1][i] = buseni[*korak][i];
      for ( ; i < (*dimenzije)[*korak + 1]; ++i)
        buseni[*korak + 1][i] = buseni[*korak][i + 1];
    }
    if (znak == '*') {
      /* gdje je kraj zvjezdicama? */
      for (kraj = lokacija;
         kraj < duljina && buseni[*korak][kraj] == '*';
         ++kraj);
      /* brisemo i busen na desnom kraju, ako postoji */
      if (kraj == duljina)
        kraj = duljina - 1;

      /* gdje je pocetak zvjezdicama? */
      for (pocetak = lokacija;
         pocetak >= 0 && buseni[*korak][pocetak] == '*';
         --pocetak);
      /* brisemo i busen na lijevom kraju, ako postoji */
      if (pocetak < 0)
        pocetak = 0;

      /* koliko ce mrkvi nestati? mogu najviše dvije: jedna na početku, jedna na kraju */
      if (buseni[*korak][pocetak] == '$')
        --*broj_mrkvi;
      if (buseni[*korak][kraj] == '$')
        --*broj_mrkvi;

      /* pamtimo nove dimenzije */
      (*dimenzije)[*korak + 1] = duljina - kraj + pocetak - 1;

      /* kopiramo preživjeli dio ploče u novo polje */
      buseni[*korak + 1] = (char*) malloc((*dimenzije)[*korak + 1] * sizeof(char));
      for (i = 0; i < pocetak; ++i)
        buseni[*korak + 1][i] = buseni[*korak][i];
      for (; i < (*dimenzije)[*korak + 1]; ++i)
        buseni[*korak + 1][i] = buseni[*korak][i + kraj - pocetak + 1];
    }
    ++ *korak;
  }
  return buseni;
}



int main(void)
{

}



