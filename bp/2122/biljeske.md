Ovo su neki materijali s petih i šestih vježbi iz Baza podataka. 
U nekim rješenjima ispisuju se i primarni ključevi i kad zadatak to ne traži, pa takva rješenja ne odgovaraju tekstu zadatka.
U takvim zadacima doslovno rješenje zadatka bilo bi bez ispisa takvih dodatnih atributa, ali bi trebalo dodati `DISTINCT` ako postoji mogućnost da se neki redak ponavlja (npr. ispis imena studenata koji ispunjavaju neko svojstvo).

### Indeksi (četvrte vježbe) i pete vježbe
Rad s indeksima:
```SQL
SHOW INDEX FROM NASTAVNIK;

SELECT LOAD_FILE('/var/lib/mysql-files/brojevi_do_100000.txt');

CREATE TABLE VELIKA(BROJ INT);

LOAD DATA INFILE '/var/lib/mysql-files/brojevi_do_100000.txt'
INTO TABLE VELIKA;

CREATE UNIQUE INDEX MOJ_INDEKS_UNIQUE ON VELIKA(BROJ);

DROP INDEX MOJ_INDEKS_UNIQUE ON VELIKA;
```

Primjer kombiniranja informacija iz više tablica (Kartezijev produkt i izjednačavanje primarnog i vanjskog ključa): 
```SQL
SELECT 
  * 
FROM 
  NASTAVNIK N, PREDMET P
WHERE 
  N.OIB = P.OIB;
```

###### 1)
```SQL
SELECT 
  UPISAO.JMBAG, UPISAO.SIFRA, IMES, PREZIMES, NASLOV
FROM 
  UPISAO, STUDENT, PREDMET
WHERE
  UPISAO.JMBAG = STUDENT.JMBAG
  AND UPISAO.SIFRA = PREDMET.SIFRA;
```

```SQL
SELECT 
  UPISAO.JMBAG, UPISAO.SIFRA, IMES, PREZIMES, NASLOV
FROM 
  UPISAO, STUDENT, PREDMET
WHERE
  UPISAO.JMBAG = STUDENT.JMBAG
  AND UPISAO.SIFRA = PREDMET.SIFRA
  AND IMES = 'Janko'
  AND PREZIMES = 'Vukovic';
```

###### 2)
```SQL
SELECT 
  UPISAO.JMBAG, UPISAO.SIFRA, IMES, PREZIMES, NASLOV
FROM 
  UPISAO, STUDENT, PREDMET
WHERE
  UPISAO.JMBAG = STUDENT.JMBAG
  AND UPISAO.SIFRA = PREDMET.SIFRA
  AND GODINA != 3;
```

```SQL
SELECT 
  UPISAO.SIFRA, NASLOV
FROM 
  UPISAO, STUDENT, PREDMET
WHERE
  UPISAO.JMBAG = STUDENT.JMBAG
  AND UPISAO.SIFRA = PREDMET.SIFRA
  AND GODINA = 2;
```

###### 3)
```SQL
SELECT 
  N1.OIB, N1.PREZIMEN, N2.OIB, N2.PREZIMEN
FROM 
  NASTAVNIK N1, NASTAVNIK N2
WHERE
  N1.BRSOBE = N2.BRSOBE
  AND N1.OIB < N2.OIB;
```

###### 4)
1. rješenje
```SQL
SELECT 
  N1.OIB, N1.PREZIMEN, N1.PLACA  /* , NCODD.PLACA 'Coddova placa' */
FROM 
  NASTAVNIK N1, NASTAVNIK NCODD
WHERE
  NCODD.PREZIMEN = 'Codd'
  AND N1.PLACA > NCODD.PLACA;
```

2. rješenje
```SQL
SELECT 
  N1.OIB, N1.PREZIMEN, N1.PLACA
FROM 
  NASTAVNIK N1
WHERE
  N1.PLACA > (
    SELECT N2.PLACA 
    FROM NASTAVNIK N2
    WHERE N2.PREZIMEN = 'Codd'
  );
```

###### 5)
Želimo JMBAG-ove studenata koji nisu među studentima koji su upisali trenutni kolegij (sa sifrom SIFRA)

1. rješenje
```SQL
SELECT 
  STUDENT.JMBAG, STUDENT.IMES, STUDENT.PREZIMES, PREDMET.SIFRA, PREDMET.NASLOV
FROM
  STUDENT, PREDMET
WHERE 
  STUDENT.JMBAG NOT IN (
    SELECT UPISAO.JMBAG FROM UPISAO 
    WHERE UPISAO.SIFRA = PREDMET.SIFRA
  );
```

2. rješenje
```SQL
SELECT 
  STUDENT.JMBAG, STUDENT.IMES, STUDENT.PREZIMES, PREDMET.SIFRA, PREDMET.NASLOV
FROM
  STUDENT, PREDMET
WHERE 
  PREDMET.SIFRA NOT IN (
    SELECT UPISAO.SIFRA FROM UPISAO 
    WHERE UPISAO.JMBAG = STUDENT.JMBAG
  );
```

3. rješenje
```SQL
SELECT 
  STUDENT.JMBAG, STUDENT.IMES, STUDENT.PREZIMES, PREDMET.SIFRA, PREDMET.NASLOV
FROM
  STUDENT, PREDMET
WHERE 
  NOT EXISTS (
    SELECT * FROM UPISAO 
    WHERE
      UPISAO.JMBAG = STUDENT.JMBAG
      AND UPISAO.SIFRA = PREDMET.SIFRA
  );
```

###### 6)
```SQL
SELECT 
  JMBAG, IMES, PREZIMES
FROM 
  STUDENT
WHERE 
  JMBAG NOT IN (SELECT JMBAG FROM UPISAO);
```

###### 7)
1. rješenje prvog dijela
```SQL
SELECT DISTINCT  /* DISTINCT jer jedan student može upisati više kolegija */
  UPISAO.JMBAG, IMES, PREZIMES
FROM 
  STUDENT, UPISAO
WHERE 
  STUDENT.JMBAG = UPISAO.JMBAG;
```

2. rješenje prvog dijela
```SQL
SELECT
  JMBAG, IMES, PREZIMES
FROM 
  STUDENT
WHERE 
  JMBAG IN (SELECT JMBAG FROM UPISAO);
```

1. rješenje drugog dijela
```SQL
SELECT DISTINCT
  BRSOBE
FROM 
  NASTAVNIK, PREDMET
WHERE 
  NASTAVNIK.OIB = PREDMET.OIB;
```

2.
```SQL
SELECT DISTINCT
  BRSOBE
FROM
  NASTAVNIK
WHERE 
  NASTAVNIK.OIB IN (
    SELECT PREDMET.OIB
    FROM PREDMET
  );
```

###### 8)
```SQL
SELECT EXP(SIN(1.2)) + EXP(COS(1.2));
```

###### 9)
```SQL
SELECT 
  OIB, 
  ROUND(PLACA / 7.5) 'Placa u stranoj valuti' 
FROM 
  NASTAVNIK;
```

###### 10)
```SQL
SELECT 
  S1.JMBAG, S2.JMBAG
FROM 
  STUDENT S1, STUDENT S2
WHERE 
  dayofweek(S1.DATR) = dayofweek(S2.DATR)
  AND S1.JMBAG < S2.JMBAG;
```

###### 11)
```SQL
SELECT 
  JMBAG, 
  REPEAT('*', OCJENA) 'Ocjena kao zvjezdice'
FROM 
  UPISAO
WHERE
  OCJENA IS NOT NULL
  AND OCJENA > 1
  AND SIFRA = 56001;
```

###### 12)
```SQL
SELECT USER(), VERSION();
```



### Šeste vježbe

Primjer: maksimalna plaća nastavnika
```SQL
SELECT MAX(PLACA) FROM NASTAVNIK;
```
Još jedno rješenje:
```SQL
SELECT 
  PLACA 
FROM 
  NASTAVNIK 
ORDER BY 
  PLACA DESC
LIMIT 
  0, 1;
```

Primjer: 2., 3. i 4. najveća plaća nastavnika
```SQL
SELECT 
  PLACA 
FROM 
  NASTAVNIK 
ORDER BY 
  PLACA DESC
LIMIT 
  1, 3; 
```

###### 13)
```SQL
SELECT 
  SUM(PLACA) 
FROM 
  NASTAVNIK;
```

```SQL
SELECT 
  AVG(PLACA) 
FROM 
  NASTAVNIK;
```

```SQL
SELECT 
  OIB, IMEN, PREZIMEN, PLACA
FROM 
  NASTAVNIK
WHERE 
  PLACA = (
    SELECT MAX(PLACA) FROM NASTAVNIK
  );
```

Primjer: nastavnik s najvećom plaćom među nastavnicima čija je plaća manja od 13000:

```SQL
SELECT 
  OIB, PLACA
FROM 
  NASTAVNIK
WHERE 
  PLACA = (
    SELECT MAX(PLACA) FROM NASTAVNIK WHERE PLACA < 13000
  );
```

###### 14)
```SQL
SELECT 
  COUNT(*) 
FROM 
  STUDENT
WHERE 
  GODINA = 1;
```

```SQL
SELECT 
  COUNT(*) 
FROM 
  STUDENT
WHERE 
  GODINA = 2;
```

```SQL
SELECT 
  COUNT(DISTINCT GODINA) 
FROM 
  STUDENT;
```

```SQL
SELECT 
  COUNT(*) 
FROM 
  STUDENT;
```

###### 15)
```SQL
SELECT 
  GODINA, COUNT(*) 
FROM 
  STUDENT
GROUP BY 
  GODINA;
```

Poredak klauzula u izrazu (to nije poredak izračunavanja):
```
SELECT ...
FROM ...
WHERE ...
GROUP BY ...
HAVING ...
ORDER BY ...
LIMIT ...
```

Poredak izračunavanja klauzula:
```
FROM (Kartezijev produkt tablica)
WHERE (filtriranje redaka unutar produkta)
GROUP BY (grupiranje redaka prema jednakim vrijednostima navedenih izraza)
HAVING (filtriranje grupa)
ORDER BY (sortiranje grupa ako smo koristili GROUP BY, inače redaka)
LIMIT (odabir grupa ako smo koristili GROUP BY, inače redaka, po indeksima)
SELECT (formira stupce, u slučaju grupirajućih upita povezuje retke grupe u jedan redak)
```

###### 16)
```SQL
SELECT 
  BRSOBE, MAX(PLACA), MIN(PLACA),
  MAX(PLACA) - MIN(PLACA) 'Raspon'
FROM 
  NASTAVNIK
GROUP BY 
  BRSOBE;
```

1. rješenje drugog dijela
```SQL
SELECT 
  BRSOBE, MAX(PLACA), MIN(PLACA),
  MAX(PLACA) - MIN(PLACA) 'Raspon'
FROM 
  NASTAVNIK
WHERE 
  BRSOBE = 127;
```

2. rješenje drugog dijela (malo manje efikasno)
```SQL
SELECT 
  BRSOBE, MAX(PLACA), MIN(PLACA),
  MAX(PLACA) - MIN(PLACA) 'Raspon'
FROM 
  NASTAVNIK
GROUP BY 
  BRSOBE
HAVING 
  BRSOBE = 127;
```

###### 17)
Rješenje koje uključuje sve kolegije:
```SQL
SELECT 
  SIFRA, 
  AVG(OCJENA),
  STD(OCJENA)
FROM 
  UPISAO
GROUP BY 
  SIFRA;
```

Rješenje koje uključuje samo kolegije gdje postoji barem jedna ne-null ocjena:
```SQL
SELECT 
  SIFRA, 
  AVG(OCJENA),
  STD(OCJENA)
FROM 
  UPISAO
GROUP BY 
  SIFRA
HAVING 
  COUNT(OCJENA) > 0;
```

###### 18)
```SQL
SELECT 
  SIFRA, GODINA, MAX(OCJENA)
FROM
  UPISAO, STUDENT
WHERE
  UPISAO.JMBAG = STUDENT.JMBAG
GROUP BY
  SIFRA, GODINA;
```

###### 19)
```SQL
SELECT
  BRSOBE, AVG(PLACA)
FROM 
  NASTAVNIK
GROUP BY
  BRSOBE
HAVING 
  COUNT(*) > 1;
```

###### 20)
```SQL
SELECT
  BRSOBE, SUM(PLACA)
FROM 
  NASTAVNIK
GROUP BY
  BRSOBE
ORDER BY 
  SUM(PLACA) DESC;
```

###### 21)
```SQL
SELECT
  OIB, IMEN, PREZIMEN, PLACA
FROM 
  NASTAVNIK
ORDER BY 
  PLACA DESC
LIMIT 0, 3;
```

Dodatni zadaci:
1. Ispisati brojeve soba čiji nastavnici ukupno predaju točno 3 kolegija.

```SQL
SELECT
  BRSOBE
FROM
  NASTAVNIK, PREDMET
WHERE
  NASTAVNIK.OIB = PREDMET.OIB
GROUP BY
  BRSOBE
HAVING
  COUNT(*) = 3;
```

2. Ispisati rang-listu godina studija (1, ..., 5) po ukupnom broju ostvarenih prolazaka na toj godini.
```SQL
SELECT
  GODINA, COUNT(*)
FROM
  UPISAO, STUDENT
WHERE
  UPISAO.JMBAG = STUDENT.JMBAG
  AND OCJENA > 1
GROUP BY
  GODINA
ORDER BY
  COUNT(*) DESC;
```

3. Za svaku sobu ispisati najveću ostvarenu ocjenu među svim ocjenama svih kolegija svih nastavnika iz te sobe.
```SQL
SELECT
  BRSOBE, MAX(OCJENA)
FROM
  UPISAO, PREDMET, NASTAVNIK
WHERE
  UPISAO.SIFRA = PREDMET.SIFRA
  AND PREDMET.OIB = NASTAVNIK.OIB
GROUP BY
  BRSOBE;
```
