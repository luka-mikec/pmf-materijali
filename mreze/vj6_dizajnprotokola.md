Službena rješenja vježbi su na [webu kolegija](https://web.math.pmf.unizg.hr/nastava/mreze/slideovi.php)

Dizajnirali smo protokol za jednostavno skladište: server čuva podatke o skladištu i nudi 
akcije stavljanja i preuzimanja objekata sa skladišta (te provjere trenutnog stanja).

Klijenti se spajaju na server i zahtijevaju izvršenje neke od tih akcija.

Popunjavali smo [ove datoteke](vj6_datoteke), tako da rezultiraju službenim rješenjima (poredak funkcija u datotekama je bio malo drugačiji).

Na vrhu tih (pomoćnih) datoteka nalaze se upute oko kompilacije i pokretanja.

Komentirali smo razne tipove grešaka koje se mogu javiti:
 - funkcije *posaljiPoruku()* odnosno *primiPoruku()* vraćaju brojevne konstante OK ili NIJEOK. To je greška u komunikaciji najniže razine: npr. dio poruke nije stigao, klijentsko ili serversko računalo su u kvaru itd.
 - moguće je da poruka ispravno stigne, ali da to nije očekivan tip poruke. Primjerice, stigne poruka BOK umjesto poruke ODGOVOR.
 - moguće je da je stigla očekivana poruka, ali da postoji neki problem vezan uz logiku protokola, npr. poruka "UZMI stvar 10" kad je stanje *stvari* na skladištu 9.


## Dodavanje nove funkcionalnosti u protokol skladišta
Kako dodati novu funkcionalnost, POPIS? Za početak, dodajmo ono što će biti zajedničko klijentu i serveru u skladisteProtokol.h:
```c
#define POPIS         7
#define POPIS_R       8
```

("_R" u POPIS_R je ponovno kratko za RESPONSE, tj. tip poruke koja dolazi kao odgovor na POPIS)

Na serverskoj strani, u slučaju primitka nove poruke pozivamo odgovarajuću funkciju:
```c
case POPIS: obradiPOPIS( sock, poruka ); break;
```

Tu funkciju (`void obradiPOPIS( int sock, char *poruka );`) deklariramo negdje iznad, npr. tamo gdje su i ostale deklaracije.
Jedna moguća implementacija je:

```c
void obradiPOPIS( int sock, char *poruka )
{
    posaljiPoruku( sock, ODGOVOR, "OK"); /* ne moze biti los zahtjev za popisom */
    
    int duljina = 1;
    for( i = 0; i < brojArtikala; ++i )
        duljina += strlen(imeArtikla[i]) + 1;

    char *odg = (char *)malloc(duljina * sizeof(char));
    *odg = '\0'; /* ili strcpy(odg, ""); */

    for( i = 0; i < brojArtikala; ++i )
    {
        strcat(odg, imeArtikla[i]); 
        strcat(odg, " ");
    }

    posaljiPoruku( sock, POPIS_R, odg);
}
```


Analogne promjene treba dodati i u klijentski kod. Ključan dio je implementaciju funkcije obradiPOPIS:

```c
void obradiPOPIS( int sock )
{
	posaljiPoruku( sock, POPIS, "");

	int vrstaPoruke; char *odgovor;
	if( primiPoruku( sock, &vrstaPoruke, &odgovor ) != OK )
		error1( "Doslo je do pogreske u komunikaciji sa serverom...\n" );

	if( vrstaPoruke != ODGOVOR )
		error1( "Doslo je do pogreske u komunikaciji sa serverom (nije poslao ODGOVOR)...\n" );

	if( strcmp( odgovor, "OK" ) != 0 )
	{
		printf( "Greska: %s\n", odgovor );
		return; 
	}
	else
	    printf( "OK\n" );

	free( odgovor );


	if( primiPoruku( sock, &vrstaPoruke, &odgovor ) != OK )
		error1( "Doslo je do pogreske u komunikaciji sa serverom...\n" );

	if( vrstaPoruke != POPIS_R )
		error1( "Doslo je do pogreske u komunikaciji sa serverom (nije poslao KOLIKO_R)...\n" );

        printf( "Artikli: %s\n", odgovor );

	free( odgovor );

}
```
