#include "stdio.h"

/*
4. Napišite rekurzivnu funkciju 'podskupova' koja vraća broj podskupova n-članog skupa.

Ideja: kao i u ranijim zadacima, traženi broj prikazujemo kao sumu jednostavnijih brojeva,
što često prirodno dovede do rekurzivnog poziva.
Neka su a_1, ..., a_n članovi skupa.
Traženi broj =
    broj podskupova od {a_1, ..., a_n} koji sadrže a_n    +
  + broj podskupova od {a_1, ..., a_n} koji ne sadrže a_n
  
Uočimo da je pojam "podskup od {a_1, ..., a_n} koji ne sadrži a_n" ekvivalentan
pojmu "podskup od {a_1, ..., a_{n - 1}}". 
Zatim, uočimo da svaki "podskup od {a_1, ..., a_n} koji sadrži a_n" možemo
(bijektivno) dovesti u vezu s "podskup od {a_1, ..., a_{n - 1}}"
To je bijekcija koja u jednom smjeru dodaje, a u drugome izbacuje, element a_n.

Dakle, rješenje je
    broj podskupova od {a_1, ..., a_{n-1}}  +
  + broj podskupova od {a_1, ..., a_{n-1}}
*/

int podskupova(int n) {
    if (n == 0)
        return 1;
    return podskupova(n - 1) + podskupova(n - 1);
}

/*
5.  Napišite rekurzivnu funkciju 'podskupova3'
    koja vraća broj podskupova skupa {1, ..., n} za n iz Z, n >= 0 
    (u slučaju n = 0 smaramo da je {1, ..., n} prazan skup),
    a čija je suma članova djeljiva s 3.

Slično kao prethodni zadatak, pokušavamo svesti rješenje na zbroj nekih iznosa koji ne ovise o n,
nego n - 1 ili manjim brojevima. Rješenje =
    broj podskupova od {1, ..., n} čija je suma članova djeljiva s 3; koji sadrže n +
    broj podskupova od {1, ..., n} čija je suma članova djeljiva s 3; koji ne sadrže n
Dakle,
    broj podskupova od {1, ..., n - 1} čija je suma članova uvećana za n djeljiva s 3 +
    broj podskupova od {1, ..., n - 1} čija je suma članova djeljiva s 3 +

Uočimo da "podskupova od {1, ..., n - 1} čija je suma članova uvećana za n djeljiva s 3"
nije instanca našeg početnog problema: dio "uvećana za n" je višak.

Dakle, sveli smo problem 
    broj podskupova skupa {1, ..., n} čija je suma članova djeljiva s 3
na općenitiji (složeniji, generaliziran) problem:
    broj podskupova skupa {1, ..., n} čija je suma uvećana za neki x djeljiva s 3

Ovo je relativno česta situacija: 
da bismo riješili početni jednostavan problem, moramo riješiti neki složeniji.

Broj podskupova skupa {1, ..., n} čija je suma članova uvećana za x djeljiva s 3 =
    broj podskupova skupa {1, ..., n} čija je suma članova uvećana za x djeljiva s 3; koji sadrže n  +
  + broj podskupova skupa {1, ..., n} čija je suma članova uvećana za x djeljiva s 3; koji ne sadrže n  
Dakle: rješenje = 
    broj podskupova skupa {1, ..., n - 1} čija je suma članova uvećana za x + n djeljiva s 3  +
  + broj podskupova skupa {1, ..., n - 1} čija je suma članova uvećana za x djeljiva s 3
*/

int podskupova3(int n, int x)
{
    if (n == 0) /* prazan skup: 0 ili 1 'dobar' podskup */
        return x % 3 == 0;
        /* alternativno: return x % 3 == 0 ? 1 : 0;   ili  if (x % 3 == 0) return 1; else return 0; */
    return
        podskupova3(n - 1, x + n) +
        podskupova3(n - 1, x);
}

/*
6.  (ovo je otprilike 1. zadatak s kolokvija 17/18)
    Na raspolaganju nam je:
        n_A predmeta A, svaki vrijednosti v_A;
        n_B predmeta B, svaki vrijednosti v_B;
        n_C predmeta C, svaki vrijednosti v_C.

    Pretpostavite da su n_A, n_B, n_C, v_A, v_B, v_C cijeli brojevi te vrijedi:
        n_A, n_B, n_C >= 0;
        v_A, v_B, v_C >= 1.

    Napišite funkciju 'pakiranja' koja računa broj načina na koje dane predmete
    možemo rasporediti u pakete tako da:
        - svi paketi imaju pozitivnu i jednaku vrijednost
        - nijedan predmet ne ostane neraspoređen;
        - broj paketa je veći od 1 (barem 2);
        - razlikujemo poredak paketa, ali ne razlikujemo "poredak" (raspored) predmeta unutar paketa.

Prva ideja: korak-po-korak graditi sve pakete. U prvom koraku odaberemo neki od predmeta A, B ili C,
i tim predmetom započnemo gradnju jednog paketa. U drugom koraku odaberemo neki od predmeta A, B ili C,
te ga dodajemo ili u neki od postojećih paketa koje gradimo, ili njime započinjemo gradnju novog paketa.
Nastavljamo na isti način dok ne potrošimo sve predmete.
Ukratko: u svakom koraku (u svakom rekurzivnom pozivu funkcije) 
odaberemo neki od predmeta A, B ili C i neki redni broj, 
te u paket s tim rednim brojem dodamo odabrani predmet.

Problemi ove ideje:
  1. treba nam niz u kojem ćemo pamtiti trenutnu vrijednost svakog paketa kojeg smo do sada počeli graditi
  2. uzimamo u obzir ne samo poredak paketa nego i poredak dodavanja pojedinog predmeta u njegov paket, 
     ali u zadatku sadržaj paket [A B] ne želimo razlikovati od sadržaja paketa [B A]
  3. dodatno, uzimamo u obzir i poredak kojim "pristupamo" pojedinom paketu

Kako bismo eliminirali korištenje niza, gradit ćemo jedan po jedan paket.
Prvo odaberemo neki od predmeta A, B ili C i stavimo ih u (prvi) paket.
Nakon toga, ili svi daljnji paketi trebaju imati trenutnu vrijednost trenutnog paketa,
ili treba dodati još predmeta u trenutni paket.
U prvom slučaju, prelazimo na gradnju drugog paketa.
U drugom slučaju, dodajemo još jedan predmet i vraćamo se na grananje.

Za drugi paket, kao i za sve ostale, redom biramo predmete dok ne dostignemo ciljnu vrijednost.

Cijeli se postupak završava kada ili vrijednošću trenutnog paketa premašimo ciljnu vrijednost paketa,
ili nema više raspoloživih predmeta.

S ovakvim rješenjem i dalje imamo problem da je bitan (a ne bi trebao biti bitan) "poredak"
predmeta unutar paketa.
Jedno rješenje tog problema: pamtiti posljednji dodan predmet u paketu (A, B ili C), slično kao rješenje
zadatka s prethodnih vježbi o broju particija broja.
 */

/* paket_tren: trenutna vrijednost paketa kojeg (trenutno) gradimo (pakiramo),
   preth_korak: jesmo li u posljednjem koraku u trenutni paket dodali A (0), B (1) ili C (2)? */
int pakiranja(int nA, int nB, int nC, int vA, int vB, int vC, int paket_tren, int preth_korak, int paket_cilj, int zapakirano) {
    int novih = 0;

    /* Jesmo li premasili ciljnu vrijednost paketa (ako je uopce postavljena)? */
    if (paket_cilj != 0 && paket_tren > paket_cilj)
        return 0;

    /* Vrijednost trenutnog paketa je manja ili jednaka od ciljne vrijednosti (ili jos nema ciljne vrijednosti). */

    /* Imamo li jos objekata za rasporediti u pakete? Ako ne postoje nerasporedeni objekti, gotovi smo.
       Raspored je dobar ako je paket koji trenutno gradimo ciljne vrijednosti, i postoji bar jos jedan prethodno izraden paket */
    if (nA == 0 && nB == 0 && nC == 0)
        return paket_tren == paket_cilj && zapakirano > 0  ?  1  :  0;

    /* Postoje nerasporedeni objekti, i vrijednost trenutnog paketa je manja ili jednaka od ciljne vrijednosti (ili jos nema ciljne vrijednosti). */

    /* Jesmo li vec dostigli ciljnu vrijednost paketa (ako je postavljena)? */
    if (paket_cilj != 0 && paket_tren == paket_cilj)
        return pakiranja(nA, nB, nC, vA, vB, vC, 0, 0, paket_cilj, zapakirano + 1);

    /* Postoje nerasporedeni objekti, i vrijednost trenutnog paketa je strogo manja od ciljne vrijednosti (ili jos nema ciljne vrijednosti). */

    if (paket_cilj == 0 && paket_tren > 0)
    {
        /* Ako trenutna vrijednost moze biti ciljna vrijednost paketa */
        novih += pakiranja(nA, nB, nC, vA, vB, vC, 0, 0, paket_tren, 1);
    }

    /* Ako trenutna vrijednost nije ujedno ciljna vrijednost paketa */
    if (nA > 0 && preth_korak <= 0)
        novih += pakiranja(nA - 1, nB, nC, vA, vB, vC, paket_tren + vA, 0, paket_cilj, zapakirano);
    if (nB > 0 && preth_korak <= 1)
        novih += pakiranja(nA, nB - 1, nC, vA, vB, vC, paket_tren + vB, 1, paket_cilj, zapakirano);
    if (nC > 0 && preth_korak <= 2)
        novih += pakiranja(nA, nB, nC - 1, vA, vB, vC, paket_tren + vC, 2, paket_cilj, zapakirano);

    return novih;
}


int main() {
  return 0;
}
