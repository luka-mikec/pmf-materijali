Ovdje su bilješke s konzultacija/vježbi 20. 4., vezano uz zajedničko rješavanje primjerka kolokvija.

### Zadatak 1

 - `friend` ispred `razlomak operator+(const razlomak& a, const razlomak& b) { return razlomak(); }`, kako bismo operator učinili globalnom funkcijom. Operatori definirani kao funkcije članice primaju samo jedan argument (desni operand operatora), a ovaj operator uzima dva operanda, pa mora biti globalna funkcija.
 - `explicit` ispred `operator double() const { return 1.1; }` kako bismo onemogućili implicitnu pretvorbu iz `razlomak` u `double`. Bez toga, izraz `1 + a` može se shvatiti kao `double(1) + double(a)`, ali i kao `razlomak(1) + a`. Obje pretvorbe su jednako dobre, pa bi se kompajler bunio na ambiguoznost.

### Zadatak 2
Tip je `std::vector<int>::const_iterator`. Za detekciju tipa koristan je ovakav kod:
```c++
template <class T> struct detektiraj_tip;
...
detektiraj_tip<decltype(varijabla_kojoj_zelimo_znati_tip)> temp;
```
U grešci koju kompajler vrati možemo pročitati tip varijable. U ovom slučaju nam ovaj postupak nije posebno koristan jer je (uz kompajler `g++`) `const_iterator` definiran (`typedef`) pomoću internog pomoćnog tipa. 

### Zadatak 3 
Općenito o konstrukciji objekata čiji tip u svojoj hijerarhiji negdje sadrži virtualne bazne klase:
 - prvo se konstruiraju sve virtualne bazne klase (svaka virtualna bazna klasa imat će točno jednu virtualnu instancu);
 - među njima, prvo se konstruiraju one koje su baza drugim virtualnim baznim klasama;
 - za pojedinu virtualnu baznu klasu odabire se konstruktor koji je naveden u najizvedenijoj klasi (to je klasa koja se instancira, u zadatku je to klasa `D`), a ako takav konstruktor nije naveden u konstruktoru najizvedenije klase, poziva se defaultni konstruktor.

Redoslijed poziva u ovom zadatku:
 - poziv konstruktora od `X` (virtualna bazna klasa), bez argumenata;
 - poziv konstruktora od `A` (virtualna bazna klasa), bez argumenata;
 - sad je `D` konstruirao sve virtualne bazne klase u svojoj hijerarhiji, pa nastavlja s uobičajenom konstrukcijom;
 - redoslijed konstrukcije baznih klasa odgovara poretku u definiciji klase:  `struct D : B, C { ...`
 - prvi na redu: konstruktor `B('d')`; `B` je izvedena iz `A` (bez virtual), pa se poziva `A('b')` (ova instanca od `A` nije virtualna bazna klasa, njih može biti proizvoljno mnogo u hijerarhiji);
 - izvršava se tijelo konstruktora `B('d')`;
 - konstruktor `C('d')` (bazne klase su virtualne i već konstruirane);
 - konstruirane su sve bazne klase (i virtualne i ostale);
 - konstruiraju se varijable, prva na redu `a1('1')` (jer je `a1` prva definirana u strukturi), a ona poziva konstruktor za `X` (varijabla `a1` nije dio hijerarhije koju definira objekt `d` u funkciji `main`, općenito, svaki objekt ima vlastitu hijerarhiju);
 - `a2('2')`: analogno kao `a1`;
 - vraćamo se u tijelo klase `D`, pa u tijelo funkcije `main`;
 - destruktori: točno suprotan poredak poretku konstrukcije.

 Čitav ispis:
 ```
 X
 A
 Ab
 Bd
 Cd
 X
 A1
 X
 A2
 D
 Pocetak
 ~D
 ~A2
 (ostali destruktori)
 ~X
 ```

Verzija s dodatnom ključnom riječi `virtual`:
```
X
A
Ab  <-- ne poziva se konstruktor klase A koji ispisuje ovu liniju, pa je ispis jednak kao ranije, samo bez Ab i ~Ab
Bd      (u drugoj je verziji iskorištena virtualna bazna klasa A, a nju smo već konstruirali)
Cd
X
A1
X
A2
D
Pocetak
~D
~A2
...
~X
```

### Zadatak 4
Napomena: varijabla `x` iz klase `A` preimenovana je u `w`, jer se `x` javlja u drugom smislu i u nastavku koda.
```c++
class A {
public:
 int vrijednost() { return value(); }
 virtual int value() { return 1; }
 int w;
 void f() { cout << ++w << endl;}
};
class B : public A {
public:
 int value() { return 2; }
};
int main() {
 B x;
 A& y = x;  // U redu: referenca na baznu klasu može čuvati objekt izvedene klase
 A z = x;  // Kompajlira se, ali gotovo nikad to ne želimo ("slicing"), u kopiji nema dijela objekta koji pripada klasi B
 cout << x.vrijednost() << endl;  // Funkcija vrijednost poziva virtualnu funkciju value, koja vraća 2
 cout << y.vrijednost() << endl;  // Slično, i ovdje value također vraća 2, y referira na x 
 cout << z.vrijednost() << endl;  // value ovdje vraća 1, jer je nakon "kopiranja" (z = x, nije samo kopiranje već i rezanje) izgubljena veza s tipom B
 x.w = 0;  
 x.f();  // x.w iz prethodne linije sad se uvećava i ispisuje se 1
 ((A)x).f();  // (A)x također "reže" objekt x (izraz oblika ((A)x).f() je analogan sljedećem: A t = x; t.f()), taj izraz vraća novi objekt tipa A, ispisuje se 2, i inkrementirani w nije isti w kao x.w, već w koji pripada novom objektu koji je rezultat operacije (A)x
 ((A&)x).f();  // (A&)x stoji za isti objekt x s početna funkcije, stari w se uvećava (postaje 2) i ispisuje
 ((B)x).f();  // Analogno kao: B t = x; t.f();  x.w je bio jednak 2, pa je t.w jednako 2, sad t.w postaje 3 (x.w ne mijenja vrijednost)
 ((B&)x).f();  // Potpuno isti efekt kao naredba: x.f(); dakle, x.w se inkrementira, i postaje 3
```

### Zadatak 5
copy-konstruktor se poziva u linijama:
 - `X y = x;` (simbol `=` uz definiciju varijablu iznimno nema značenje operatora pridruživanja)
 - `return ret;` u funkciji `f1` (funkcija vraća objekt tipa `X` koji se konstruira iz reference `ret`)
 - `X* f2(X x)` (kopiranje vrijednosti u argument funkcije)

Ovo se nisam sjetio komentirati, ali možemo otprilike provjeriti rješenje korištenjem `__LINE__` makroa koji u svakoj liniji vraća njen broj linije:
```c++
    struct X { 
      X() { } 
      X(const X&) { std::cout << "cctor "; } 
    };
    
    X f1(X& x) { std::cout << __LINE__ << " ";
        X& ret = x; std::cout << __LINE__ << " ";
        return ret;
    }
    X* f2(X x) {  std::cout << __LINE__ << " ";
        X* ret = &x; std::cout << __LINE__ << " ";
        return ret;
    }
    int main() {
        X x; std::cout << __LINE__ << " ";
        X y = x; std::cout << __LINE__ << " ";
        x = y; std::cout << __LINE__ << " ";
        f1(x); std::cout << __LINE__ << " ";
        f2(y); std::cout << __LINE__ << " ";
        return 0;
    }
```

### Preostali zadaci

U prvom zadatku, prvu točku pod "dodatno" možete implementirati na način da dodate još jedan operator poziva, ali koji prima `const char *` (ili `std::string`). Treba vratiti `std::string` koji sadrži tekstualnu reprezentaciju polinoma s danim stringom kao varijablom.

U drugom zadatku, neka bazna klasa `Funkcija` bude apstraktna, s čistim virtualnim operatorom i funkcijom:
```c++
...
    virtual double operator()(double) const = 0;
    virtual double dx(double) const = 0;
...
```
