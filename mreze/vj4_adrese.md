Službena rješenja vježbi su na [webu kolegija](https://web.math.pmf.unizg.hr/nastava/mreze/slideovi.php)

U prvom dijelu vježbi smo proučavali funkcije *inet_aton* (alphanumeric to network) i *inet_ntoa*. 
Pritom se network odnosi na konvenciju da se mrežni promet odvija u big endian formatu (vidi dolje). 

Potom smo proučavali funkciju *gethostbyname*. 

Osim stvari na slideovima, napomenuli smo da char\* u Socket API-u ne mora biti string ili pokazivač na znak.
Može se raditi o generičkom pointeru (kao void\*), kojeg prije upotrebe treba castati (pretvoriti) u pravi tip.
Primjerice, u strukturi *hostent* imamo:
> char \*\*h_addr_list

Radi se o nizu pokazivača na struct in_addr objekte. Želimo li, primjerice, dohvatiti treći pokazivač, izraz bi bio:
> ((struct in_addr \*)neki_hostent_pokazivac->h_addr_list[2])

Pritom nam *neki_hostent_pokazivac->h_addr_list[2]* vraća treći pokazivač (to funkcionira unatoč pogrešnom tipu
jer su pokazivači svih tipova jednako dugački, tj. imaju isti broj bitova). 
Nakon toga taj pokazivač iz char\* pretvaramo u struct in_addr\* pokazivač.
Za čitanje same adrese potrebno je još derefencirati taj pokazivač (pročitati ono na što pokazivač pokazuje):
> *((struct in_addr \*)neki_hostent_pokazivac->h_addr_list[2])

Na satu nismo radili komandnolinijski dio zadatka. Pogledajte službena rješenja za implementaciju tog dijela zadatka
(posebno obratite pažnju na *sscanf*).

## O endianessu
Endianess je poredak bajtova unutar nekog (višebajtnog) objekta, npr. unutar 4-bajtnog broja.

IP adresa se može predstaviti kao 4-bajtni broj, na sljedeći način. Recimo da promatramo adresu 10.9.16.1
Svakom dijelu adrese (svakom oktetu) možemo pridružiti odgovarajući bajt (bajtove obično zapisujemo heksadekadski).
 
 -   10  = 0A
 -   9   = 09
 -   16  = 10
 -   1   = 01

Nadovežemo li ove bajtove jedan na drugoga, dobivamo sljedeći memorijski sadržaj: 0A091001.
Ako su bajtovi 0A, 09, 10, 01 zapisani upravo tim redom, dakle "prirodno", kažemo da je korišten *big endian*.

*Little endian* je zapis u kojem su bajtovi poredani obratno, dakle prvo 01, pa 10, 09, 0A.
Pritom treba spomenuti da su sami bitovi unutar pojedinog bajta i dalje poredani prirodno (u obje konvencija).
Endianess se odnosi samo na poredak samih bajtova.

Na većini današnjih računala koristi se little endian, na većini mobitela i manjih uređaja big endian.
Jedna bitna iznimka je studentski server na Odsjeku ("student"), koji također koristi big endian.

Koja god konvencija bila prisutna na nekom računalu, nju zovemo "host konvencija". 
Mrežna (network) konvencija je uvijek big endian.





























