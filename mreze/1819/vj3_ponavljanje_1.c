#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    /* rjesenje prvog zadatka, koristeći niz */
    int n, polje[100], i, max;
    scanf("%d", &n);

    for (i = 0; i < n; ++i)
        scanf("%d", &polje[i]); // moze i "polje + i" umjesto &polje[i]

    max = polje[0];
    for (i = 1; i < n; ++i) // moze i "i = 0" umjesto "i = 1"
        if (polje[i] > max)
            max = polje[i];

    printf("max je %d", max);

    return 0;
}

