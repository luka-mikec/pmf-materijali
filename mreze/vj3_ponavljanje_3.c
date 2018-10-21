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
   char rijec[11]; // bitno: ovaj niz je alociran u mainu
   unos(rijec);
   rijec[0] = 'A';
   printf("%s", rijec);
   return 0;
}


