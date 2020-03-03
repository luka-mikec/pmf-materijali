#include <stdio.h>
#include <stdlib.h>


/*
 * 4.
 * Neka je zadana struktura:
 */
typedef struct _cvor {
    int x;
    struct _cvor * sljedeci;
} cvor;
/*
 * Napisite funkcije za (tekstualno) ucitavanje i spremanje odgovarajućih vezanih listi
 */

void  spremi(char * adresa, cvor * lista)
{
    FILE *fp = fopen(adresa, "w");

    if (!fp) exit(1); /* nije specificirano, ali ako treba signalizirati gresku u ovakvoj generickoj funkciji,
                         u praksi bi bolje bilo da je povratni tip int i vracamo 0 na neuspjeh */

    cvor *t = lista;
    for (t = lista; t != NULL; t = t = t->sljedeci) /* ili analogna while petlja */
        fprintf(fp, "%d\n", t->x);

    fclose(fp);
}

cvor * ucitaj(char * adresa)
{
    FILE *fp = fopen(adresa, "r");
    if (!fp) exit(1); /* komentar kao i gore */

    cvor * lista = NULL,  * posljednji = NULL;
    int x;

    while (fscanf(fp, "%d", &x) == 1)
    {
        /* lista vec postoji? */
        if (lista)
        {
            posljednji->sljedeci = (cvor *)malloc(sizeof(cvor));
            posljednji = posljednji->sljedeci;
        }
        else
        {
            lista = posljednji = (cvor *)malloc(sizeof(cvor));
        }

        /* sada lista svakako postoji (ako nije postojala, stvorena je) */
        posljednji->x = x;
        posljednji->sljedeci = NULL;
    }

    fclose(fp);
    return lista;
}

/* (slično zadatku 8.1.47.)
 * 5. Napišite dio programa koji prepisuje linije iz ulaz.txt u izlaz.txt, u obratnom redoslijedu.
 *    Pretpostavite da nema praznih linija.
 *    Možete koristiti liste i nizove.
 */

typedef struct _linija {
    char *slova;
    int duljina;
    struct _linija * sljed;
} linija;

void rjesenje5() {
    FILE *ulaz = fopen("ulaz.txt", "r"), *izlaz = fopen("izlaz.txt", "w");
    if (!ulaz || !izlaz)
        exit(1);

    linija * poc = NULL;

    int nova_linija = 1; /* treba li sljedeci char c spremiti u novu liniju? */

    int c;

    while ((c = fgetc(ulaz)) != EOF)
    {
        if (c == '\n')
        {
            nova_linija = 1;
        }
        else
        {
            if (nova_linija)
            {
                linija * l = (linija *)malloc(sizeof(linija));
                l->slova = NULL;
                l->duljina = 0;
                l->sljed = poc;

                poc = l;

                nova_linija = 0;
            }

            poc->slova = realloc(poc->slova, poc->duljina + 1 + 1);
            ++ poc->duljina;
            poc->slova[poc->duljina - 1] = c;
            poc->slova[poc->duljina] = '\0';
        }
    }

    while (poc)
    {
        linija *t = poc->sljed;
        fprintf(izlaz, "%s\n", poc->slova);

        free(poc->slova);
        free(poc);
        poc = t;
    }

    fclose(ulaz);
    fclose(izlaz);
}

/*
 * ~ 8.1.48
 * 6. u datoteci A.txt nalaze se redovi oblika (x, f(x)), a u datoteci B.txt redovi oblika (x, g(x))
 * za sve x iz S podskup N
 * U datoteku C.txt zapišite trojke oblika (x, f(x), g(x)), za svaki x iz S.
 * (Bez polja/lista, zanemarite (ne)efikasnost)
 */
void rjesenje6() {
    FILE *A = fopen("A.txt", "r"), *B = fopen("B.txt", "r"), *C = fopen("C.txt", "w");
    if (!A || !B || !C) exit(1);

    int ax, ay, bx, by;
    while (fscanf(A, "(%d, %d)\n", &ax, &ay) == 2) /* ili praznina (npr. \n) na kraju format stringa,
 *                                                    ili praznina (npr. razmak) na početku */
    {
        rewind(B);
        while (fscanf(B, "(%d, %d)\n", &bx, &by) == 2)
        {
            if (ax == bx) {
                fprintf(C, "(%d, %d, %d)\n", ax, ay, by);
            }
        }
    }

    fclose(A);
    fclose(B);
    fclose(C);
}


/* 7. zadatak (slično kolokviju prošle godine)
   U datoteci bus.txt nalazi se niz imena (jedna riječ do 5 znakova) autobusnih linija.
   Za svaku liniju LIN postoji datoteka LIN.txt čiji su reci oblika "sat:minute  S"
    - sat i minute su cijeli brojevi (0 <= sat <= 23, 0 <= minute <= 59)
    - S predstavlja smjer: S = 'A' ili S = 'B'
   Napišite dio programa koji pronalazi minimalno vrijeme V u danu takvo da
   u segmentu [V, V + 5minuta] postoji barem 2 polaska (nekih) buseva [istoga dana!] u smjeru 'A'.

   Ako vrijeme V postoji, ispišite ga u datoteku izlaz.txt, u formatu sati:minute

   Nisu dopušteni nizovi, osim za imena datoteka.

   (Pretpostavite da nema gresaka s datotekama, ispravno su zapisane itd., nisu nuzno sortirane, nema ponavljanja redaka)
*/


void rjesenje7()
{
    FILE *f = fopen("bus.txt", "r");
    int t;

    /* sva moguca pocetna vremena, u minutama od pocetka dana, tj. od 00:00. */
    for (t = 0; t < 24 * 60; ++t)
    {
        char lin[6];
        int nadjeno = 0;
        rewind(f);

        while (fscanf(f, "%s", lin) == 1) {
            char lin_adresa[10];
            FILE *lin_dat;

            int h, m, v;
            char smjer;

            sprintf(lin_adresa, "%s.txt", lin);
            lin_dat = fopen(lin_adresa, "r");

            while (fscanf(lin_dat, "%d:%d %c\n", &h, &m, &smjer) == 3) {
                v = h*60 + m;

                if (smjer == 'A' && t <= v && v <= t + 5)
                    ++nadjeno;
            }

            fclose(lin_dat);
        }

        if (nadjeno >= 2) {
            FILE *izlaz = fopen("izlaz.txt", "w");
            fprintf(izlaz, "%d:%d\n", t / 60, t % 60);
            fclose(izlaz);
            break;
        }
    }

    fclose(f); /* mozda sam uzivo ovdje napisao jos jedan poziv fclose; treba samo fclose(f) na ovom mjestu */
}
