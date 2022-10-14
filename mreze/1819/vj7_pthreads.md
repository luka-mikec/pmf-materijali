Službena rješenja vježbi su na [webu kolegija](https://web.math.pmf.unizg.hr/nastava/mreze/slideovi.php)

Uveli smo pojam dretvi, odnosno više istovremenih tokova izvršavanja programa unutar istog procesa.

Dretve započinjemo funkcijom `pthread_create`. S druge strane, funkcija `pthread_join` pauzira izvršavanje dretve u kojoj je pozvana (obično glavne dretve, tj. glavni tok programa) dok se dretva koja joj je proslijeđena ne završi. Bitno je u nekom trenutku pozvati tu funkciju, inače riskiramo da glavni program završi, a neka dretva ostane pokrenuta (to će "nasilno" prekinuti izvršavanje preostalih dretvi, prije nego što one završe posao koji smo im dali).
Posao se dretvama prenosi u obliku funkcije koje primaju i vraćaju `void*`. Naime, bilo koji niz željenih ulaznih argumenata možemo spremiti u strukturu, te koristiti adresu te strukture kao stvarni ulazni argument. Slično, povratnu vrijednost možemo alocirati dinamički, te vratiti njenu adresu. 

Dretve mogu pristupati zajedničkim resursima (datotekama, memoriji, ekranu itd.), no pritom treba paziti na moguće probleme kod istovremenog pristupa. U primjeru s brojanjem smo vidjeli probleme koji nastaju kod pokušaja istovremene promjene sadržaja ekrana (`printf`) i sadržaja memorije (inkrement globalnog brojača).
Najjednostavnije rješenje, i jedino koje smo obradili, su *mutexi* (kratko od **mut**ual **ex**clusion).

```
pthread_mutex_t lokot = PTHREAD_MUTEX_INITIALIZER;
```

Pozivanjem `pthread_mutex_lock(&lokot)`, sve druge dretve koje dođu do iste te linije koda u svom izvršavanju, pauziraju svoje izvršavanje. Čekanje tih dretvi traje dok se ne izvrši naredba `pthread_mutex_unlock(&lokot)`. U tom trenutku dretva koja je čekala (jedna od njih, ako je više dretvi čekalo) nastavlja izvršavanje, dok ona sama ne pozove `pthread_mutex_unlock(&lokot)` itd. Efektivno smo time postigli da se kod omeđen s `pthread_mutex_lock` i `pthread_mutex_unlock` (s istim lokotom, tj. istim *mutexom*) izvršava u najviše jednoj dretvi u bilo kojem danom trenutku.

Konačno, iskoristili smo dretve kako bismo omogućili da naš server radi s više klijenata odjednom. Ovo je skica cijelog rješenja:

  - definicija strukture koju ćemo slati kao argument dretvama 
    (sadrži: `int indexDretve, int commSocket`)
  - polje ulaznih argumenata (struktura)
  - polje statusnih brojeva koji opisuju status svake dretve (= {0}, za brzu inicijalizaciju vrijednosti na 0) 
    (npr. 0: neaktivna; 1: aktivna i radi; 2: aktivna i završila)
  - *mutex* za to polje (sve dretve moraju moći pisati u statusno polje)
  - u obradiKlijenta:
      - parametar je sad `void *parametar`, treba ga pretvoriti u
        pointer na odgovarajuću strukturu i pročitati `indexDretve` i `commSocket`
      - povratna vrijednost je `void*` (jer mora biti, ali nećemo ju koristiti)
      - na kraju (uz *mutex*) treba upisati ispravno novo stanje dretve
      - vraćamo `NULL` (nebitno, moramo nešto vratiti)
  - u funkciji `main()`:
      - polje identifikatora dretvi (`pthread_t`)
      - u petlji nakon `accept()`:
        - koristit ćemo *mutex* za statusno polje
        - pronađemo slobodno mjesto (status dretve 0 ili 2)
          ako je status bio 2, prvo pozivamo `pthread_join`
        - ako nema slobodnog mjesta, odbijamo klijenta
        - ako ima slobodnog mjesta, zovemo `pthread_create()`




