## *CSS 3*

Prošli smo neke jednostavne primjere stiliziranja stranica koristeći CSS. 
Ovaj put nismo prolazili brojne primjere sa [stranica profesora Čačića](https://web.math.pmf.unizg.hr/~veky/rp1/vjezbe4/),
ali preporučam da ih pogledate i vidite još neke mogućnosti CSS-a prije/tijekom izrade zadaće.

### Gdje pisati CSS

Postoje tri mjesta na kojima se mogu nalaziti CSS pravila.

 - U vanjskoj datoteci (ili više njih). Pojedinu datoteku uključujemo (zapravo, pozivamo) u HTML koristeći `<link>` tag:
   ```
   <head>
     ...
     <link rel="stylesheet" href="ime_datoteke.css" />
     ...
   </head>
   ```
 - U HTML datoteci. Navodimo ih unutar `<style>` taga, koji se obično nalazi unutar `<head>`.
   ```
   <head>
     ...
     <link rel="stylesheet" href="ime_datoteke.css" />
     ...
   </head>
   ```
 - U atributu `style` pojedinog objekta na kojeg želimo primijeniti stil. Stil će se primijeniti samo na taj konkretan objekt.
   ```
   <neki-tag style="..."> 
     ...
   </neki-tag>

### Pravila

Pretpostavimo da koristimo neki od prva dva gornja pristupa (što je gotovo uvijek slučaj), dakle kod koji pišemo je u vanjskoj datoteci, ili unutar `<style>` taga.
Tada najčešće pišemo neki niz **pravila**, od kojih se svako pravilo sastoji od niza **deklaracija**. 

Pravilo:
```
selektor { deklaracija1 deklaracija2 ... deklaracijan }
```

Deklaracije imaju oblik: `ime: vrijednost;`, npr. `color: blue;` ili `border: solid 1px black;`

U pravilima, `selektor` je najčešće:
 - neki tag (bez simbola &lt; i &gt;), npr. `p`, 
 - ime neke klase koju smo odlucili definirati, npr. `.vazno` (dodajemo točku ispred imena klasa),
 - ili ime identifikatora, npr. `#copyright_poruka` (dodajemo ljestve ispred imena identifikatora).

Od složenijih selektora, neki česti scenariji su:
 - isti niz deklaracija želimo primijeniti na više selektora koji ne moraju biti ni u kojem odnosu. Npr. `p, .vazno { color: yellow; }` će obojati sve paragrafe, i sve objekte s klasom `vazno`. (Uočite **zarez**, bez njega bi efekt ovog pravila bio bitno drukčiji.)
 - niz deklaracija želimo primijeniti na objekte koji istovremeno ispunjavaju više selektora, npr. `p.vazno.novo { color: yellow; }` će obojati paragrafe koji imaju i klasu `bitno`, i klasu `novo`.
 - niz deklaracija želimo primijeniti na selektore koji su neposredni ili posredni sljedbenici (djeca) nekog drugog selektora, npr. `.vazno p { color: yellow; }` će obojati paragrafe unutar objekata koji su označeni kao `vazno`. Uočite da ovdje nema zareza: da ga ima, obojali bi se svi paragrafi, i svi objekti označeni s `vazno`.

Osim njih, korisne su još i pseudoklase i pseudoelementi.  

Više o CSS-u:
 - [Slideovi](https://web.math.pmf.unizg.hr/nastava/mreze/slideovi/2015/MR%20-%20Vjezbe%20-%2009.pdf)
 - [*float* i *clear*](https://developer.mozilla.org/en-US/docs/Web/CSS/clear)
 - [*position*](https://developer.mozilla.org/en-US/docs/Web/CSS/position)
