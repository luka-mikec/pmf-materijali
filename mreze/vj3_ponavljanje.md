Podešavanje terminala u programu Code::Blocks ako koristite Linux Mint (primjerice u praktikumima): Settings -> Environment -> Terminal to launch console programs: najlakše je odabrati opciju
> gnome-terminal --disable-factory -t $TITLE -x 

te potom zamijeniti "gnome" s "mate".

Zadaci za vjezbu. U zadacima s dinamickom alokacijom ne morate oslobadati memoriju (pozivati free).

1. Napišite program koji učita broj n, te potom n cijelih brojeva. Ispisuje najveći od njih. Pretpostavite da će n sigurno biti veći od 0 i manji od 100.

Uputa: Implementirajte koristeći polje (npr. int niz[100]), tj. prvo učitajte sve brojeve, pa ispišite najveći (nije efikasno, ali da ponovimo polja).

2. Zadaci:
  - Napišite program koji učita broj n, te potom alocira polje (malloc) za n cijelih brojeva. Učita brojeve u polje. Ispisuje njihovu aritmetičku sredinu (koja možda nije cijeli broj). Pretpostavite da će n sigurno biti veći od 0 i manji od 100.
   
 - Ispišite adresu svakog člana polja, te sadržaj na toj adresi. Format za adrese za printf: %p

3. Napišite funkciju s deklaracijom: *void unos(char \*s)* koja traži od korisnika unos jedne riječi (pretpostavite da će korisnik unijeti riječ duljine 1 do 10 slova), te se riječ sprema u string s. 
Pozovite funkciju, promijenite prvo slovo riječi (na proizvoljan način), ispišite novu riječ.

4. Iskoristite funkciju unos kako bi učitali jednu riječ (opet, duljine 1 do 10 slova). Zatražite i unos broja n. Dinamički alocirajte polje u koje stane n stringova (char \*\*polje), a svaki string dinamički alocirajte tako da u njega stane 10 slova. Kopirajte zadanu riječ u prvih n - 1 stringova (v. funkciju strcpy). U posljednji član polja spremite 0. Ispišite sve stringove u polju, tako da ih redom ispisujete dok ne dođete do 0-stringa.

5. Izvedite prethodni zadatak koristeći polje struktura koje sadrže riječ, umjesto polje riječi: (rijec \*polje, gdje je rijec struktura koju prethodno definirate).





























