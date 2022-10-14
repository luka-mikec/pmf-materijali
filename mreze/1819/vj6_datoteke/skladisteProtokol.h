#ifndef __SKLADISTEPROTOCOL_H_
#define __SKLADISTEPROTOCOL_H_

#define UZMI          1
#define STAVI         2
#define KOLIKO        3
#define KOLIKO_R      4
#define BOK           5
#define ODGOVOR       6

#define OK      1
#define NIJEOK  0

int primiPoruku( int sock, int *vrstaPoruke, char **poruka );
int posaljiPoruku( int sock, int vrstaPoruke, const char *poruka );

/* pomocni makroi za ispis gresaka */
#define error1( s ) { printf( s ); exit( 0 ); }
#define error2( s1, s2 ) { printf( s1, s2 ); exit( 0 ); }
#define myperror( s ) { perror( s ); exit( 0 ); }

#endif
