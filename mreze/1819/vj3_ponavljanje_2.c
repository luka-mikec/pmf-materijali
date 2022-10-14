#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int n, *polje, i, suma;
    double sredina; // ili float sredina

    scanf("%d", &n);

    polje = (int *) //pretvaramo adresu koju vrati malloc u adresu na cijeli broj
        malloc(sizeof(int) * n); // treba nam sizeof(int) (veličina
                                 // jednog broja), n puta jer trebamo n brojeva;
                                 // malloc vraća adresu alociranog komada memorije

    for (i = 0; i < n; ++i)
        scanf("%d", &polje[i]);

    suma = 0; // nemojte zaboraviti inicijalizirati
    for (i = 0; i < n; ++i)
        suma += polje[i];

    sredina = suma / n; // NIJE dobro - dobit ćemo cijeli broj, a želimo decimalni
    sredina = (double)suma / n; // da smo deklarirali float sredina, ovdje (float) umjesto (double)

    printf("sredina je %lf\n", sredina); // %lf je format za double, \n novi red


   // B dio:
    for (i = 0; i < n; ++i)
        printf("%p: %d\n", &polje[i], polje[i]);

    return 0;
}




