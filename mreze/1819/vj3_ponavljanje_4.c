#include <stdio.h>
#include <stdlib.h>

void unos(char *s)
{
    scanf("%s", s); // bez operatora & ispred s
                    // razlog: s već jest memorijska adresa
                    // na koju pišemo (string je niz znakova,
                    // a u C-u na niz referiramo adresom njegovog pocetka)
}

int main(int argc, char **argv)
{
   char tekst[11];

   char **polje;
   int i, n;

   unos(tekst);

   scanf("%d", &n);
   polje = (char**)malloc(n * sizeof(char*));


   for (i = 0; i < n - 1; ++i)
   {
       polje[i] = (char*)malloc(sizeof(char) * 11);
       strcpy(polje[i], tekst); // poredak argumenata strcpy mozete
                                // zamisljati kao prvi_arg = drugi_arg
   }

   polje[n - 1] = NULL; // 0 = NULL = '\0'

   for (i = 0; polje[i] != NULL; ++i)
      printf("%s\n", polje[i]);

   return 0;
}

