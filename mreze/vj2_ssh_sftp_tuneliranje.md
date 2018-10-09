## Pristup udaljenom računalu kroz ssh i sftp

Otvorite terminal i upišite 
>    ssh korisnicko_ime@student.math.hr

Password se neće prikazati dok ga upisujete. Enter kad ste upisali.
Ako ima pitanja prije ili poslije upisa passworda, yes pa enter.
Sada ste u svojem direktoriju na računalu student, /home/student1/korisnicko_ime.

Izrada nove datoteke: vidi [prve vježbe](vj1_uvod.md), pod "cat >" i "cat". 

Svoje web stranice možete postaviti u direktorij public_html.
Bit će dostupne na adresi:
>    http://web.studenti.math.pmf.unizg.hr/~korisnicko_ime

Izrada direktorija public_html (ako ga već nemate): 
>    mkdir public_html

Postavljanje dopuštenja (mora biti javno čitljiv): 
>    chmod 701 public_html

Ulazak u direktorij:
>    cd public_html
	
U nekom uređivaču teksta (npr. xed na Linux Mint, gedit na Ubuntu) napišite neki tekst i spremite ga kao index.html u svoj lokalni korisnički direktorij (ako radite od doma, vjerojatno je adresa /home/vaše_ime/).

U terminalu (možete otvoriti novi tab ili novi prozor terminala) upišite 
>    sftp korisnicko_ime@student.math.hr

Proces ulogiravanja je kao za ssh (vidi gore).
Sada ste u dva direktorija odjednom, jedan lokalno, jedan na studentu. Naredba ls će ispisati sadržaj udaljenog direktorija (vjerojatno je to direktorij /home/student1/korisnicko_ime), a lls lokalnog direktorija (vjerojatno je to direktorij /home/korisničko_ime). 
Kako biste uploadali datoteku index.html, pozicionirajte se u public_html:
>    cd public_html

Potom prenesite s:
>    put index.html

Možda je potrebno postaviti dopuštenja. Bilo u ranijoj ssh sesiji, ili sada kroz sftp, upišite: 
>    chmod 755 index.html

Provjerite radi li stranica. Ponovite postupak za proizvoljnu slikovnu datoteku slika.jpg umjesto datoteke index.html, provjerite prikazuje li se (na adresi http://web.studenti.math.pmf.unizg.hr/~korisnicko_ime).



## SSH tunel

Ideja: spajati se na Internet kroz neko drugo računalo, s kojim imamo kriptiranu vezu.
>    Naše računalo ===sigurna veza (kriptirana)=== udaljeno računalo --- Internet

Svrhe:

 - kad smo na nesigurnoj mreži i želimo kriptirati čitav promet (neke stranice kriptiraju same, neke ne)
 - kad želimo pristupiti resursu koji je ograničen geografski ili drugačije (npr. baze znanstvenih časopisa)

Izrada tunela u Linuxu:
>    ssh -D 8123 -C -q -N korisnicko_ime@student.math.hr

Povezivanje na tunel (npr. u Firefoxu)
Postavke -> na dnu pronađite proxy postavke -> ručna (manual) konfiguracija -> SOCKS
Upišite localhost, a pod port 8123. Pohranite.
Provjerite IP adresu koristeći neku stranicu koja vam javlja vašu IP adresu.




## Ponavljanje C-a 

(na ovim vježbama meni nije radio CodeBlocks, pa smo ručno kompajlali)

Program (spremite ga u bilo kojem editoru teksta):
> (program je u datoteci [vj2_ponavljanje.c](vj2_ponavljanje.c))

Kompajlanje (ako ne radi CodeBlocks ili drugi program):
 - otvorite terminal, upišite cd adresa-direktorija-gdje-ste-spremili-main.c
 - gcc main.c -o program
 - pokretanje: ./program

Nastavak ponavljanja C-a na idućim vježbama.
