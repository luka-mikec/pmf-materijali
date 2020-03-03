#include <stdio.h>
#include <stdlib.h>

int p(int n)
{
    if (n == 0)
        return 1;
    else
        return 2 * p(n - 1);
}


/*
1. Napišite rekurzivnu funkciju koja za (int) niz a duljine n vraća broj podnizova
(uključno s praznim nizom, kojeg možemo označiti s epsilon) čija je suma djeljiva sa 3.

Ideja, ukratko: generalizirati problem na sljedeći problem (razlog generalizacije je ispod):
Napišite rekurzivnu funkciju koja za (int) niz a duljine n vraća broj podnizova
čija je suma, uvećana za neki x, djeljiva sa 3.

Ovi su problemi ekvivalentni za slučaj x = 0.

Rješenje izvornog problema
 = broj podnizova od {a_0, ..., a_{n-1}} čija je suma djeljiva s 3 i koji ne sadrže a_{n-1} +
 + broj podnizova od {a_0, ..., a_{n-1}} čija je suma djeljiva s 3 i koji sadrže a_{n-1}.
Dakle:
 = broj podnizova od {a_0, ..., a_{n-2}} čija je suma djeljiva s 3 +
 + broj podnizova od {a_0, ..., a_{n-2}} čija je suma, uvećana za a_{n-1}, djeljiva s 3.
Drugi pribrojnik više nije instanca izvornog problema. No, jest instanca generaliziranog problema.

To je rješenje izvornog problema pomoću (dva rješavanja) generaliziranog problema.
Za rješenje generaliziranog problema, moramo akumulirati x tokom poziva:
Broj podnizova od {a_0, ..., a_{n-1}} čija je suma, uvećana za neki x, djeljiva sa 3 =
 = broj podnizova od {a_0, ..., a_{n-2}} čija je suma, uvećana za x, djeljiva s 3 +
 + broj podnizova od {a_0, ..., a_{n-2}} čija je suma, uvećana za x + a_{n-1}, djeljiva s 3.

Napomena: smatramo da prazan podniz ima sumu 0.
*/
int f(int a[], int n, int x)
{
    if (n == 0)
        return x % 3 == 0; // 1 ako 3 dijeli sumu, inače 0
    else
        return f(a, n - 1, x) +
               f(a, n - 1, x + a[n - 1]);
}

int main()
{
    const int n = 3;
    int a[] = {1, 2, 6};
    printf("%d\n", f(a, n, 0)); // podnizovi: epsilon; 6; 1 2; 1 2 6

    return 0;
}
