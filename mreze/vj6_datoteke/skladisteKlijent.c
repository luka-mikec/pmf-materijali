// skladisteKlijent.c
// kompilacija, pokretanje i spajanje na lokalni server (po potrebi promijenite port, broj na kraju):
// gcc skladisteKlijent.c skladisteProtokol.c -o klijent && ./klijent 127.0.0.1 5006
// (prije toga pokrenite server pomoću ./server 5006)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "skladisteProtokol.h"

void obradiSTAVI( int sock );
void obradiUZMI( int sock );
void obradiKOLIKO( int sock );
void obradiBOK( int sock );

int main( int argc, char **argv )
{
 
	if( argc != 3 )
		error2( "Upotreba: %s IP-adresa port\n", argv[0] );

	char dekadskiIP[20]; strcpy( dekadskiIP, argv[1] );
	int port;          sscanf( argv[2], "%d", &port );
	

	// socket...
	int mojSocket = socket( PF_INET, SOCK_STREAM, 0 );
	if( mojSocket == -1 )
		myperror( "socket" );
		
	// connect...
	struct sockaddr_in adresaServera;
	
	adresaServera.sin_family = AF_INET;
	adresaServera.sin_port = htons( port );
	if( inet_aton( dekadskiIP, &adresaServera.sin_addr ) == 0 )
		error2( "%s nije dobra adresa!\n", dekadskiIP );
	memset( adresaServera.sin_zero, '\0', 8 );
	
	if( connect( mojSocket,
		        (struct sockaddr *) &adresaServera,
		        sizeof( adresaServera ) ) == -1 )
		myperror( "connect" );
		
	// ispisi menu
	int gotovo = 0;
	while( !gotovo )
	{
		printf( "\n\nOdaberi opciju...\n"
				"   1. dodavanje artikla u skladiste\n"
				"   2. uzimanje artikla iz skladista\n"
				"   3. ispis kolicine nekog artikla na skladistu\n"
				"   4. izlaz iz programa\n"
				"   \n: " );
				
		int opcija;
		scanf( "%d", &opcija );
		
		switch( opcija )
		{
			case 1: obradiSTAVI( mojSocket ); break;
			case 2: obradiUZMI( mojSocket ); break;
			case 3: obradiKOLIKO( mojSocket ); break;
			case 4: obradiBOK( mojSocket ); gotovo = 1; break;
			default: printf( "Pogresna opcija...\n" ); break;
		}
	}

	close( mojSocket );

	return 0;
}

void obradiSTAVI( int sock )
{
	char imeArtikla[50];
	int kolicinaArtikla;
	
	printf( "Koji artikl treba staviti: " );
	scanf( "%s", imeArtikla );
	
	printf( "Koliko tog artikla treba staviti: " );
	scanf( "%d", &kolicinaArtikla );
	
	char poruka[100];
	sprintf( poruka, "%s %d", imeArtikla, kolicinaArtikla );
	
	posaljiPoruku( sock, STAVI, poruka );

	int vrstaPoruke; char *odgovor;
	if( primiPoruku( sock, &vrstaPoruke, &odgovor ) != OK )
		error1( "Doslo je do pogreske u komunikaciji sa serverom...\n" );
		
	if( vrstaPoruke != ODGOVOR )
		error1( "Doslo je do pogreske u komunikaciji sa serverom (nije poslao ODGOVOR)...\n" );
		
	if( strcmp( odgovor, "OK" ) != 0 )
		printf( "Greska: %s\n", odgovor );
	else
	    printf( "OK\n" );
	    
	free( odgovor );
}

void obradiUZMI( int sock )
{
       /* identičan kod kao ranije, do na tekst koji se ispisuje korisniku */
}

void obradiKOLIKO( int sock )
{

      /* slican kod kao ranije, ali nakon njega treba primiti jos jednu poruku,
	 naime server nakon OK poruke salje i odgovor na postavljeno pitanje;
	 odgovor je u formatu "IME broj", te se može pročitati koristeći
	
         sscanf( odgovor, "%*s %d", &kolicina );
      
         (%*s procita string IME, ali ga ne zapisuje u neku varijablu - ne treba nam jer znamo sto je IME)

     */ 
}

void obradiBOK( int sock )
{
     /* slicno kao prve dvije funkcije, ali ne treba nista upisivati ni ucitavati */
}
