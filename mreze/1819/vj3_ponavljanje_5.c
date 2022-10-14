#include <stdio.h>
#include <stdlib.h>

// Imamo više opcija za memorijski razmještaj;
// npr. mozemo imati dinamicko polje struktura
// ili dinamicko polje pokazivaca na strukture.
// Unutar strukture mozemo imati polje znakova (char s[11]),
// ili pokazivač (char *), potonje u slučaju da koristimo dinamičku alokaciju.
// Svejedno je (u ovom zadatku) što odaberemo, samo treba konzistentno koristiti.

typedef struct _rijec {
    char s[11]; // ili char *s; uz dinamičku alokaciju
    // ovdje mogu doći drugi podaci
} rijec;

void unos(char *s)
{
    scanf("%s", s);
}

int main(int argc, char **argv)
{
   char tekst[11];

   rijec **polje;
   int i, n;

   unos(tekst);

   scanf("%d", &n);
   polje = (rijec**)malloc(n * sizeof(rijec*));

   for (i = 0; i < n - 1; ++i)
   {
       polje[i] = (rijec*)malloc(sizeof(rijec));
       strcpy(polje[i]->s, tekst); // zašto koristimo ->: zato što je polje[i]
                                   // pokazivač na strukturu,
                                   // a ne sama struktura
                                   // (ovo je kao da piše *(polje[i]).s)
   }

   polje[n - 1] = NULL; // 0 = NULL = '\0'

   for (i = 0; polje[i] != NULL; ++i)
      printf("%s\n", polje[i]->s);

   return 0;
}
