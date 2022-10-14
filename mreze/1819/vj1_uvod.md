Na vježbama koristimo Linux.

## O instalaciji Linuxa

Linux možete instalirati na dva načina: klasično, kao punokrvni operativni sustav (samostalno ili uz Windowse), ili unutar već postojećeg operativnog sustava. Upute za drugi način: preuzmite npr. VirtualBox:  
>    https://www.virtualbox.org/wiki/Downloads (klik na "Windows hosts").   

Trebat će vam i neki Linux, npr. Ubuntu 18.04:  
>    https://www.ubuntu.com/download/desktop  

Programe poput ifconfig, ping i traceroute pokrećete unutar terminala (pronađite popis programa na svom Linuxu pa odaberite terminal). Potražite tutorial o terminalu (termini istoznačni ili bliski terminu "terminal": bash, shell, cli). Konkretno, neke korisne naredbe:   

-    ls (listanje sadržaja direktorija),   
-    ls -lat (ls s parametrima l, a, t),   
-    man ime-neke-naredbe (pomoć o naredbi),   
-    cd ime-direktorija (ulazak u direktorij),   
-    cd .. (ulazak u roditeljski direktorij),   
-    cd ~ (ulazak u osobni, tzv. home direktorij (nije isto što i HOME u praktikumima)),    
-    pwd (prikaz adrese trenutnog direktorija),   
-    cp a b (kopiranje datoteke a u datoteku b),  
-    mv a b (premještanje a u b),  
-    xgd-open a (otvaranje datoteke a u editoru),  
-    bilo-koja-naredba-s-dugim-ispisom | less (čini ispis prve naredbe "skrolabilnim")  
-    cat > a (započinje upis teksta u datoteku a, kad ste gotovi s upisom pritisnite CTRL i C na tipkovnici)  
-    cat a (ispisuje sadržaj tekstualne datoteke a unutar terminala)  



## Neki mrežni alati

ifconfig
 - ispisuje ip adresu (lokalnu ako između nas i Interneta postoji ruter, inače "prava" eksterna IP adresa)
 - također koristan za dohvaćanje tzv. broadcast IP adrese (Bcast): slanjem na takvu adresu poruka stiže na sva računala u lokalnoj mreži
 - ako ne radi, probajte ipconfig, ili ip

nslookup domena
 - traži od posebnog servisa, tzv. DNS servera, da nam kaže koja je IP adresa računala registriranog pod domenom domena 
 - npr. nslookup www.math.hr

netstat
 - ispisuje neke podatke o trenutno aktualnim tokovima komunikacije
 - ako nas zanimaju samo portovi spremni za zahtjeve izvana (listening): netstat -l

nmap adresa
 - pretražuje otvorene portove na nekom računalu, npr. zanima nas postoje li na nekom računalu web stranice
 - za skeniranje i prikaz svih računala u lokalnoj mreži: nmap -sn (vidi i dolje, ping)
 - u PR1 nije instaliran, ali možete ga instalirati kod sebe
 - npr. ako je naša IP adresa (vidi ifconfig) 100.100.100.1, pokretanje nmap -sn 100.100.100.* bi na kućnom računalu trebalo ispisati ostala računala (također mobitele itd.)

ping adresa
 - je li neko računalo dostupno za mrežnu komunikaciju?
 - ping -c ako želimo neki drugi broj paketa
 - ping -b adresa ako želimo slati na broadcast adresu (koja piše pored Bcast iz ifconfig)
 - ako nas zanima popis računala u mrežu, nakon ping -b ..., pokrećemo arp -a 

