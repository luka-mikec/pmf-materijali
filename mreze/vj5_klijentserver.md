Službena rješenja vježbi su na [webu kolegija](https://web.math.pmf.unizg.hr/nastava/mreze/slideovi.php)

Primjer kompilacije i komunikacije sa daytime serverom na portu 13 računala 128.138.141.172:
> cd (direktorij gdje je daytime_client.c)
> gcc daytime_client.c -o klijent
> ./klijent 128.138.141.172 13

Obrađivali smo primjer komunikacije klijenta i servera, na primjeru daytime protokola.

Jos neki daytime serveri (takoder na portu 13): [popis](https://tf.nist.gov/tf-cgi/servers.cgi). Napomena: neka računala na ovom popisu ne rade.

Skica strukture jednostavnog klijenta (općenito, ne samo za daytime):
 - poziv *socket(PF_INET, SOCK_STREAM, 0)*
 - ispuna *struct sockaddr_in* strukture (po potrebi korištenje *inet_aton()*, *htons()*, *memset()*)
 - poziv *connect()* koristeci upravo ispunjenu strukturu (uz pretvorbu/cast adrese u pokazivač *struct sockaddr*)
 - komunikacija: *send()*, *recv()*, ili oboje. Ovisno o protokolu koji pišemo.
 - poziv *close()*

Skica strukture jednostavnog servera (opet općenito):
 - poziv *socket(PF_INET, SOCK_STREAM, 0)*
 - ispuna *struct sockaddr_in* strukture (po potrebi koristenje *htons()*, *memset()*)
 - poziv *bind()* koristeci upravo ispunjenu strukturu (ponovno uz pretvorbu adrese u pokazivač)
 - poziv *listen()*
 - komunikacija (u jednoj niti/dretvi) je obično beskonačna petlja:
   - na početku, poziv *accept()* (ovo pauzira server dok se ne javi neki klijent), vraća novi socket samo za komunikaciju s tim klijentom
   - *send()*, *recv()*, ili oboje, dok ne završi komunikacija s klijentom - koristiti novi socket
   - poziv *close()* za novi socket
   - u sljedećoj iteraciji accept će opet čekati dok se ne spoji neki (novi) klijent
 - ponekad na kraju poziv *close()* za početni socket -- nije potrebno ako ranije imamo beskonačnu petlju 

Korisno je imati funkcije koje će slati i primati po jedan string (jer nam za slanje i primanje
treba petlja, zbog osobine *send()* i *recv()* funkcija da ne šalju sav sadržaj odjednom).
Primjer za takve funkcije je u datoteci *echo_client.c*.

Kad server pokrećete na vlastitom računalu (što je nama uobičajeno), njegova IP adresa je 127.0.0.1 (na tu se adresu spajate u klijentu).

U terminalu izvođenje servera prekidamo pritiskom na Ctrl + C.






























