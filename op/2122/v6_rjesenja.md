Ovdje su rješenja prvog i trećeg zadatka (rješenje drugog zadatka je u [prethodnoj datoteci](v6.md)).

### Zadatak 1

```c++
template <class T>
class niz {
    T* elementi;
    int n;
public:
    niz() : n(0), elementi(nullptr) {}

    niz(const niz& postojeci) :
        n(postojeci.n),
        elementi(new T[postojeci.n])
    {
        std::copy(postojeci.elementi, postojeci.elementi + n, elementi);
    }

    niz& operator=(const niz& postojeci) {
        if (this == &postojeci)
            return *this;

        delete[] elementi;  // Ne moramo provjeravati je li elementi null-pokazivač jer je delete[] nullptr legalan poziv

        n = postojeci.n;
        elementi = new T[n];
        std::copy(postojeci.elementi, postojeci.elementi + n, elementi);
        return *this;
    }

    ~niz() {
        delete[] elementi;
    }

    void resize(int novi_n) {
        T* novi_elementi = new T[novi_n];
        std::copy(elementi, elementi + std::min(n, novi_n), novi_elementi);
        delete[] elementi;
        elementi = novi_elementi;
        n = novi_n;
    }

    T& element(int i) {
        return elementi[i];
    }

    const T& element(int i) const {
        return elementi[i];
    }

    template <class U>
    friend std::ostream& operator<<(std::ostream& os, niz<U> p);
};

template <class T>
std::ostream &operator<<(std::ostream &os, niz<T> a) {
    os << a.n << ": ";
    for (int i = 0; i < a.n; ++i)
        os << a.element(i) << " ";
    return os << std::endl;
}
```

### Zadatak 3

```c++
void serijaliziraj(std::ostream& o, const std::string& s) {
    size_t n = s.size();
    o.write(reinterpret_cast<char*>(&n), sizeof(n));
    o.write(&s[0], n);  // Mogli smo i slovo-po-slovo, ili koristeći funkciju c_str
}

template<class T>
void serijaliziraj(std::ostream& o, const std::vector<T>& v) {
    size_t n = v.size();
    o.write(reinterpret_cast<char*>(&n), sizeof(n));
    for (size_t j = 0; j < n; ++j)
        serijaliziraj(o, v[j]);
}

void deserijaliziraj(std::istream& i, std::string& s) {
    size_t n;
    i.read(reinterpret_cast<char*>(&n), sizeof(n));
    s = std::string(n, ' ');
    i.read(&s[0], n);  // Mogli smo i slovo-po-slovo
}

template<class T>
void deserijaliziraj(std::istream& i, std::vector<T>& v) {
    size_t n;
    i.read(reinterpret_cast<char*>(&n), sizeof(n));
    v.resize(n);
    for (size_t j = 0; j < n; ++j)
        deserijaliziraj(i, v[j]);
}

int main() {
    std::vector<std::string> vec{"abc", "def", "ghi"};
    std::ofstream izlazna_datoteka("binarna_datoteka.bin", std::ios::binary);
    serijaliziraj(izlazna_datoteka, vec);
    izlazna_datoteka.close();

    vec.resize(0);
    std::ifstream ulazna_datoteka("binarna_datoteka.bin", std::ios::binary);
    deserijaliziraj(ulazna_datoteka, vec);
    for (auto val : vec)
        std::cout << val << std::endl;

    return 0;
}
```
