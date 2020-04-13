#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/* 
    Ponavljanje dosadašnjeg dijela gradiva: rješenja nekih zadataka s prvog kolokvija 16/17.

    Kolokvij: http://degiorgi.math.hr/prog2/kolokviji/p2-kolokvij-1617-1.pdf
    Službena rješenja: http://degiorgi.math.hr/prog2/kolokviji/p2-kolokvij-1617-1-rj.zip

    Neka su rješenja vrlo slična službenima, neka su drugačija. 
*/ 



/*
    1. zadatak (usporedite s https://github.com/luka-mikec/pmf-materijali/blob/master/prog2/1920/vj01_rekurzije.c#L160)
    
    Umjesto učitavanja s tipkovnice, ovdje je funkcija koju pozivamo s pripremljenim vrijednostima (za lakše testiranje).
*/

int rel_prost(int a, int b)
{
  return b > 0 ? rel_prost(b, a % b) : a;
}

/* dobar: jesu li grane stabla rekurzivnih poziva koje sadrže ovaj poziv ispunile uvjet o relativnoj prostosti?
   d: duljina do sada izgrađene grane */
int rjesenje_z1(int m, int n, int max_x, int max_y, int dobar, int d)
{
  int i, j, rez = 0;
  
  /* bazni slučajevi */
  
  if (m < 0 || n < 0)
    return 0;
  
  if (m == 0 && n == 0)
    return (dobar && d % 2 == 0) ? 1 : 0;
  
  /* rekurzivni korak */ 
  
  for(i = max_x; i <= m; ++i) 
    for(j = max_y; j <= n; ++j)
      rez += f(m - i, n - j, i, j, (dobar || rel_prost(i,j)) ? 1 : 0, d + 1);
  
  return rez;
}


/*
    2. zadatak
*/

void BlokDijagonalna(double X[55][35], int m, int n)
{
  int i, j, k, dijag = 0;
  int max_k = ((m < n) ? m : n) / 2;

  /* prolazimo svim mogućim veličinama tražene podmatrice, počevši od najvećih */
  for (k = max_k; k > 0; k--)
  {
    /* pretpostavimo da je podmatrica u gornjem lijevom kutu tražena matrica A */
    dijag = 1;
    /* ... i tražimo protuprimjer tome */
    for (i = 0; i < k; ++i)
      for (j = 0; j < k; ++j)
        if (X[i][j] != X[i + k][j + k] || X[i][j + k] != 0 || X[i + k][j] != 0)
          dijag = 0; 
    
    /* ako nismo pronašli protuprimjer: */
    if (dijag) {
      for (i = 0; i < 2*k; ++i) {
        for (j = 0; j < 2*k; ++j)
          printf("%lf ", X[i][j]);
        printf("\n");
      }
      break; /* pronašli smo najveću podmatricu traženog oblika, prekidamo potragu */
    }
  }

  if (!dijag)
    printf("Nema takvih podmatrica!\n");
}

/*
    4. zadatak, podzadatak (a)
*/


char* izbaci(char *recenica)
{
  int i;
  
  /* za riječ s maksimalnim brojem slova: koliko je slova, gdje počinje ta riječ, koliko je dugačka */
  int max_broj = -1, max_pocetak, max_duljina; 
  
  /* ovdje ćemo spremiti riječ koju vraćamo */
  char *rijec = NULL;

  if (recenica[0] == '\0') /* je li proslijeđen string prazan: ako jest, treba vratiti NULL */
    return rijec;

  /* tražimo riječ s najvećim brojem velikih slova */
  for (i = 0; recenica[i] != '\0'; ++i)
  {
    /* sada smo na početku neke riječi (kroz slova unutar riječi prolazimo donjom petljom) */
    int pocetak = i;
    /* broj velikih slova u trenutnoj riječi */
    int broj; 

    /* prolazimo slovima unutar riječi i brojimo koliko je velikih */
    for (broj = 0; recenica[i] != '+'; ++i)
      if (isupper(recenica[i]))
        ++broj;

    if (broj > max_broj) {
      max_broj = broj;
      max_pocetak = pocetak;
      max_duljina = i - pocetak;
    }
  }

  /* prvo kopiramo traženu riječ u novi string */
  rijec = (char*) malloc((max_duljina + 1) * sizeof(char));
  for (i = 0; i < max_duljina; ++i)
    rijec[i] = recenica[max_pocetak + i];
  rijec[max_duljina] = '\0';

  /* potom brišemo tu riječ iz početnog stringa, ali i plus koji je označavao njen kraj */
  for (i = max_pocetak; recenica[i + max_duljina + 1] != '\0'; ++i)
    recenica[i] = recenica[i + max_duljina + 1];
  recenica[i] = '\0'; /* ne zaboraviti znak \0 */

  return rijec;
}

/*
    3. zadatak: ako želite, rješenje glavnog dijela zadatka možete pogledati u (pret)prošlogodišnjoj verziji ovih vježbi:
    https://github.com/luka-mikec/pmf-materijali/blob/master/prog2/1718/vj7_ponavljanje.c
    
    Međutim, taj zadatak ima nedorečenosti i dvosmislenosti pa vjerojatno nije prikladan zadatak za vježbu.
    Na gornjem je linku riješen jer je netko od studenata tada pitao za rješenje.
*/

