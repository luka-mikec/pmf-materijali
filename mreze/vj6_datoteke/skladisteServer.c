// skladisteServer.c
// kompilacija i pokretanje (po potrebi promijenite port, broj na kraju):
// gcc skladisteServer.c skladisteProtokol.c -o server && ./server 5006

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

void komunicirajSaKlijentom( int sock );
void obradiSTAVI( int sock, char *poruka );
void obradiUZMI( int sock, char *poruka );
void obradiKOLIKO( int sock, char *poruka );

#define MAXARTIKALA     100
int brojArtikala = 0, kolicinaArtikla[MAXARTIKALA];
char imeArtikla[MAXARTIKALA][100];

int main( int argc, char **argv )
{
	if( argc != 2 )
		error2( "Upotreba: %s port\n", argv[0] );
		
	int port; sscanf( argv[1], "%d", &port );
	
	// socket...
        int listenerSocket = socket( PF_INET, SOCK_STREAM, 0 );
	if( listenerSocket == -1 )
		myperror( "socket" );
		
	// bind...
	struct sockaddr_in mojaAdresa;

	mojaAdresa.sin_family = AF_INET;
	mojaAdresa.sin_port = htons( port );
	mojaAdresa.sin_addr.s_addr = INADDR_ANY;
	memset( mojaAdresa.sin_zero, '\0', 8 );
	
	if( bind( listenerSocket,
		      (struct sockaddr *) &mojaAdresa,
		      sizeof( mojaAdresa ) ) == -1 )
		myperror( "bind" );
		
	// listen
	if( listen( listenerSocket, 10 ) == -1 )
		myperror( "listen" );
		
	// accept
	while( 1 )
	{
        struct sockaddr_in klijentAdresa;
		int lenAddr = sizeof( klijentAdresa );

		int commSocket = accept( listenerSocket,
			                     (struct sockaddr *) &klijentAdresa,
			                     &lenAddr );

		if( commSocket == -1 )
			myperror( "accept" );

		char *dekadskiIP = inet_ntoa( klijentAdresa.sin_addr );
		printf( "Prihvatio konekciju od %s [socket=%d]\n", dekadskiIP, commSocket );
		
		komunicirajSaKlijentom( commSocket );

		close( commSocket );
		printf( "Zavrsio komunikaciju sa %s [socket=%d]\n", dekadskiIP, commSocket );
	}
	
	return 0;
}

void komunicirajSaKlijentom( int sock )
{
	int vrstaPoruke, gotovo = 0;
	char *poruka;
	
	while( !gotovo )
	{
		if( primiPoruku( sock, &vrstaPoruke, &poruka ) != OK )
		{
			printf( "Greska u komunikaciji sa klijentom [socket=%d]...\n", sock );
			gotovo = 1;
			continue;
		}
		
		switch( vrstaPoruke )
		{
			case UZMI: obradiUZMI( sock, poruka ); break; 
			case STAVI: obradiSTAVI( sock, poruka ); break;
			case KOLIKO: obradiKOLIKO( sock, poruka ); break;
			case BOK: posaljiPoruku( sock, ODGOVOR, "OK" ); gotovo = 1; break;
			default: posaljiPoruku( sock, ODGOVOR, "Nepostojeci kod poruke!\n" );
		}
		
		free( poruka );
	}
}

void obradiSTAVI( int sock, char *poruka )
{
	int koliko;
	char ime[100];

	if( sscanf( poruka, "%s %d\n", ime, &koliko ) != 2 || koliko <= 0 )
	{
		posaljiPoruku( sock, ODGOVOR, "Pogresan oblik naredbe STAVI" );
		return;
	}

	int i, postoji = 0;
	for( i = 0; i < brojArtikala; ++i )
		if( strcmp( imeArtikla[i], ime ) == 0 )
		{
			posaljiPoruku( sock, ODGOVOR, "OK" );
			kolicinaArtikla[i] += koliko;

			postoji = 1;
			break;
		}

	if( !postoji )
	{
		if( brojArtikala < MAXARTIKALA )
		{
			strcpy( imeArtikla[brojArtikala], ime );
			kolicinaArtikla[brojArtikala] = koliko;
			
			++brojArtikala;
			posaljiPoruku( sock, ODGOVOR, "OK" );
		}
		else
			posaljiPoruku( sock, ODGOVOR, "Nema vise mjesta na skladistu" );
	}
}


void obradiUZMI( int sock, char *poruka )
{
	/* prvo citanje poruke, kao i gore; a potom slicno kao gore, s time da su umjesto prostora na skladistu,
           sada moguce greske nepostojanje artikla na skladistu, ili postojanje u nedovoljnom broju */
	
}



void obradiKOLIKO( int sock, char *poruka )
{
       /* citanje poruke je jednostavno, a potom slicno kao obradiUZMI, samo ne smanjujemo kvantitetu */
}
