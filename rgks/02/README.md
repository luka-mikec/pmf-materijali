# Minimalna *full-stack* aplikacija

Cilj je izraditi aplikaciju sa sljedećim elementima:
- Autentikacija: OIDC protokol uz Entra ID (bivši Azure Active Directory) kao *identity provider*;
- Frontend: kao i do sada, koristimo Vite, Vue i Vuetify;
- Backend: REST API koristeći Django;
- Baza: PostgreSQL (Azure Database for PostgreSQL), a lokalno SQLite.

Osim toga, cilj je imati jednostavan CI/CD pipeline za *deployment* na Azure kad god se promijeni `main` grana.

Prvo ćemo pogledati biblioteku Django s lokalnom bazom i kako povezati frontend i backend. Kasnije ćemo dodati autentikaciju i *deployment* na Azure.  

## Potrebni alati

Pogledajte [README](https://github.com/luka-mikec/pmf-materijali/blob/master/rgks/README.md).

## Postavljanje aplikacije s Django *backendom*

Stvorite direktorij za aplikaciju i opcionalno inicijalizirajte git repozitorij (`git init .`).

Aplikacije ćemo strukturirati kao dva direktorija s nekim pomoćnim datotekama u korijenskom direktoriju:
- `frontend/` (Vite, Vue i Vuetify)
- `backend/` (Django, DRF i lokalna SQLite bazom)
- `start_local.sh` (skripta koja lokalno pokreće i frontend i backend)
- ...

## Frontend

Prvo stvorimo Vuetify projekt:

```bash
npm create vuetify@latest
```
Odaberite `Base` template, a projekt nazovite `frontend`.

Provjerite funkcionira li Vuetify aplikacija:

```bash
cd frontend
npm run dev
```

Frontend će biti dostupan na [http://localhost:3000](http://localhost:3000). Možemo promijeniti port na 5173 kako bismo bili u skladu s Vue/Vite konvencijom. U svakom slučaju, želimo da sve `/api/...` URL-ove ne obrađuje Vite nego Django (kojeg ćemo uskoro dodati). Django ćemo pokretati na `localhost:8000`, pa možemo konfigurirati Vite da sve `/api/...` HTTP zahtjeve proslijedi na tu adresu. U `vite.config.mts`, promijenite `server` blok:

```JavaScript
server: {
  port: 5173,
  proxy: {
    "/api": {
      target: "http://localhost:8000",
      changeOrigin: true
    }
  }
}
```

## Backend

Za backend koristimo Django. U odnosu na Flask, imat ćemo nekoliko prednosti:
- ORM (Object-Relational Mapping), način rada s bazom koristeći Python objekte umjesto SQL upite.  Na ovaj način ne moramo pisati različit kod (različite upite) ovisno o bazi koju koristimo (SQLite, PostgreSQL).
- Automatsko stvaranje migracija baze podataka. Primjerice, kada obrišemo u Pythonu varijablu koja opisuje stupac u definiciji tablice, Django za nas može stvoriti kod koji mijenja tablicu u bazi (umjesto da se sami spajamo na bazu i pišemo `ALTER TABLE ...`).
- Jednostavno generiranje administracijskog sučelja za upravljanje podacima u bazi. Ovo je grafičko sučelje za dodavanje, mijenjanje i brisanje redaka u tablicama. Vrlo je korisno za debuggiranje, a može se koristiti i kao osnova za izradu jednostavnijih sučelja za krajnje korisnike (mi za to ipak koristimo Vue i Vuetify).
Sljedećim naredbama stvaramo direktorij `backend/`, u njemu postavljamo Python verziju na 3.12 (`pyenv` čita datoteku `.python-version`), te stvaramo i aktiviramo virtualno okruženje.

```bash
cd ..  # Ako je terminal još uvijek u frontend direktoriju
mkdir backend && cd backend
echo "3.12" > .python-version
python --version  # Očekujemo 3.12.*
python -m venv .venv
source .venv/bin/activate
```

Provjerite da `which python` spominje `.venv` direktorij. Svrha virtualnog okruženja jest izolirati instalirane Python pakete; svaki projekt koristi vlastite verzije paketa, neovisne o drugima na sustavu (slično kao `node_modules` u npm projektima). Deaktivirate ga s `deactivate`. Aktivacija znači da će naredbe `python` i `pip` u trenutnom terminalu pozivati verziju unutar virtualnog okruženja, a ne globalnu verziju, a deaktivacija vraća na globalnu verziju.

Kad kasnije otvorite novi terminal za rad u backendu, ponovno aktivirajte okruženje (`source .venv/bin/activate`) prije nego što pokrećete bilo koju Poetry ili Django naredbu.

Poetry je alat za upravljanje verzijama paketa. Primjerice, za svaki paket možemo definirati minimalne verzije, a Poetry će pronaći kompatibilan skup paketa koji ispunjava sve uvjete. Sve pakete instalirat ćemo koristeći `poetry add ...` naredbe.

Sada instalirajte Poetry unutar virtualnog okruženja (aktivirajte virtualno okruženje ako ste ga prethodno deaktivirali ili ako više niste u istoj sesiji terminala), inicijalizirajte projekt i dodajte pakete:

```bash
pip install poetry
poetry init  # Odgovorite zadanim odgovorima
poetry add django djangorestframework django-cors-headers "psycopg[binary]" gunicorn
```

U git repozitoriju treba pratiti datoteke `pyproject.toml` i `poetry.lock`, ali ne i virtualno okruženje (`.venv/`). Pokretanjem `poetry install` u aktiviranom virtualnom okruženju, Poetry će pročitati `pyproject.toml` i `poetry.lock` i instalirati identične verzije paketa.

Uz Django, ovo su paketi koje smo instalirali:
- `djangorestframework`: sadržava neke korisne dodatke za izradu REST API-a.  `django-cors-headers`: često su frontend i backend dostupni na različitim URL-ovima (npr. localhost:5173 i localhost:8000, ili stranica.com i api.stranica.com). U takvim situacijama trebamo omogućiti CORS (Cross-Origin Resource Sharing) kako bi web preglednik dopustio pozivanje API-a na drugoj domeni. Kad bi preglednik to dopuštao u svim situacijama, maliciozna bi stranica mogla pozivati npr. API korisnikove banke. U našem slučaju nije potreban CORS jer ćemo backend pozivati preko proxyja kojeg kontrolira frontend, i taj će proxy proslijediti poziv backendu (kad u pregledniku pozovemo `localhost:5173/api/`, proxy će interno pozvati `localhost:8000/api/`). No kako je CORS često potreban, uvrštavamo ga u projekt.
- `psycopg`: PostgreSQL adapter za Python. Nećemo ga koristiti direktno, Django će ga koristiti kad komunicira s PostgreSQL bazom. U početku nam zapravo nije potreban ni ovaj paket jer ćemo za početak koristiti SQLite bazu.
- `gunicorn`: 'server' koji stoji između web servera poput (npr. nginx) i Django aplikacije. Omogućuje definiranje npr. broja procesa i dretvi koji će obrađivati API zahtjeve. Također nam nije odmah potreban jer ćemo lokalno koristiti Djangov ugrađeni development server.

Sada inicijaliziramo Django projekt:

```bash
django-admin startproject config .  # Točka na kraju
django-admin startapp api
```

`startproject` stvara nekoliko datoteka, uključujući `manage.py` (skripta s korisnim Django naredbama) i direktorij `config/` (globalne postavke projekta). `startapp api` stvara direktorij `api/` koji predstavlja jednu Django "aplikaciju" (tu ćemo pisati većinu našeg koda). 

`api` aplikaciju moramo registrirati u `config/settings.py`. Uz nju, registriramo i `rest_framework` te `corsheaders`.

```python
INSTALLED_APPS = [
    ...
    'rest_framework',
    'corsheaders',
    'api',
]

MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'corsheaders.middleware.CorsMiddleware',
    ...
]
```

Dodajemo i CORS postavke, iako nije nužno u našem slučaju kao što je već spomenuto: API-u ćemo pristupati koristeći proxy (lokalno Vite, a na serveru `nginx`) koji sve `/api/...` zahtjeve šalje backendu. Ako bismo htjeli izravno pozivati `localhost:8000/api/...` iz frontenda (localhost:8000), bilo bi nužno dodati sljedeće:

```python
CORS_ALLOWED_ORIGINS = [
    'http://localhost:5173',
]
```

### Migracije

Django koristi tzv. migracije za upravljanje shemom baze. Svaka je migracija Python datoteka koja opisuje promjenu sheme: stvaranje tablice, dodavanje stupca, brisanje stupca, itd. Django ima ugrađen alat za generiranje migracija koji možete pozvati kad mijenjate modele. Zbog toga obično ne moramo pisati `ALTER TABLE ...` naredbe ili razmišljati gdje privremeno kopirati korisničke podatke kad mijenjamo shemu baze; Django se brine o tome da se stvarne instance baze prilagođavaju trenutnoj definiciji modela u Python kodu.

Naša je baza na početku prazna (bez tablica). Django dolazi s nekim ugrađenim modelima (npr. `User`) za koje ima i unaprijed pripremljene migracije. Pokretanjem tih migracija u našoj će se bazi inicijalizirati tablice potrebne za ugrađene modele:

```bash
python manage.py migrate
```

Ova naredba izvršava sve nepokrenute migracije u projektu. U našem slučaju stvorit će i `db.sqlite3` datoteku u trenutnom direktoriju jer u postavkama u Django templateu piše da koristimo SQLite bazu koja se tako zove (vidi `DATABASES` u `settings.py`). 

### Admin sučelje

Django dolazi s gotovim administracijskim sučeljem na URL-u `/admin/`. Za pristup trebate Django korisnika s administratorskim privilegijama. Stvorite ga s:

```bash
python manage.py createsuperuser
```

Unesite username, email i lozinku. (Email nije obavezan.) Pokrenite server (ekvivalent `npm run dev`):

```bash
python manage.py runserver
```

i posjetite [http://localhost:8000/admin/](http://localhost:8000/admin/). Prijavite se s podacima koje ste upisali za administratorskog korisnika. Vidjet ćete tablice za ugrađene modele, za sada prazne (osim svojeg korisnika).

### Model za *Task*

Definiramo model `Task` u `api/models.py`:

```python
from django.db import models


class Task(models.Model):
    PRIORITY_CHOICES = [
        ('Low', 'Low'),
        ('Medium', 'Medium'),
        ('High', 'High'),
    ]

    title = models.CharField(max_length=200)
    done = models.BooleanField(default=False)
    priority = models.CharField(max_length=10, choices=PRIORITY_CHOICES, default='Medium')
    created_at = models.DateTimeField(auto_now_add=True)

    class Meta:
        ordering = ['-created_at']

    def __str__(self):
        return self.title
```

Kako bismo dobili tablicu u bazi za ovaj model, generiramo migraciju i pokrećemo je:

```bash
python manage.py makemigrations
python manage.py migrate
```

Naredba `makemigrations` analizira modele u svim `models.py` datotekama, i sve postojeće migracije, te na temelju to dvoje generira datoteku `api/migrations/0001_initial.py` (kasnije 0002 itd.) koja izvršava sve što je potrebno napraviti za implementirati trenutno stanje modela u bazi. U našem slučaju to je stvaranje tablice `api_task` s odgovarajućim tipovima stupaca. 

Migracije su važan dio projekta i treba ih pratiti u gitu zajedno s ostatkom koda. U timskim projektima, bitno je da nakon `git pull` pokrenete `migrate`, kako bi baza imala najnoviju shemu. Slično, u našoj deployment skripti pozivat ćemo `migrate` kako bi naša Azure baza imala ispravnu shemu kad god postavljamo novu verziju aplikacije na server.

Sve modele koje izradite treba ručno registrirati kako bi se pojavili u administratorskom sučelju. To radimo u datoteci `api/admin.py`:

```python
from django.contrib import admin

from .models import Task


admin.site.register(Task)
```

Osvježite admin sučelje. Sad biste trebali vidjeti i sekciju "API" s "Tasks" tablicom. Možete probati dodati ili obrisati zadatke kroz administratorsko sučelje.

### Object-relational mapping (ORM) i `manage.py shell`

Django ORM omogućuje rad s bazom kroz Python objekte (umjesto pisanja SQL upita). Pokrenite interaktivni Python shell s pristupom Django postavkama:

```bash
python manage.py shell
```

Pogledajmo kako stvarati, čitati, listati i mijenjati bazu koristeći ORM:
```python
>>> from api.models import Task
>>> Task.objects.create(title='Moj prvi zadatak', priority='Low')
<Task: Moj prvi zadatak>
>>> Task.objects.all()
<QuerySet [<Task: Moj prvi zadatak>, ...]>
>>> Task.objects.filter(done=False).count()
1
>>> task = Task.objects.first()
>>> task.done = True
>>> task.save()
```

Ako vas zanima što Django zapravo izvršava kod upita, možete pročitati `query` svojstvo:

```python
>>> str(Task.objects.filter(done=False).query)
'SELECT ... FROM api_task WHERE NOT api_task.done'
```

### `flush`

Naredba `python manage.py flush` briše sve podatke iz baze, ali zadržava shemu (tablice). Korisno je za reset podataka bez ponovnog pokretanja migracija. Ako želite potpuno obrisati bazu, dakle ne samo podatke već i shemu, onda je najjednostavnije obrisati `db.sqlite3` (ako koristite SQLite lokalno, kao što mi koristimo) i ponovno pokrenuti `migrate`. Vaš administratorski Django korisnik također će biti obrisan, pa ga treba ponovo stvoriti.

### Serializers

DRF (Django REST Framework) *serializer* služi kao most između podataka u bazi i pozivatelja API-a (tzv. klijenata, npr. naš frontend). Primjerice, kad naš frontend želi pročitati podatke iz baze, trebamo ih dostaviti frontendu u JSON formatu. Jedna od uloga *serializera* jest konstruirati ovaj JSON: odabrati ispravna polja i ispravno formatirati svako polje. U drugom smjeru, kad želimo mijenjati bazu, serializer definira shemu ulaza, validira podatke i stvara ili mijenja podatke u bazi. 

Stvorite `api/serializers.py`:

```python
from rest_framework import serializers

from .models import Task


class TaskSerializer(serializers.ModelSerializer):
    class Meta:
        model = Task
        fields = ['id', 'title', 'done', 'priority', 'created_at']
        read_only_fields = ['id', 'created_at']
```

`fields` definira koji stupci trebaju biti prisutni u JSON ulazu (stvaranje nove instance) i izlazu (čitanje postojeće instance). Obično ulaz i izlaz nisu isti (npr. `id` je auto-inkrement pa ne može biti dio ulaza), pa možemo definirati i `read_only_fields` (za rijetku situaciju kad su nam potrebni *write-only* atributi također postoji mehanizam).

U `fields` možemo definirati i polja koja nisu postojeći stupci u bazi, nego su izvedena iz njih. Na primjer, mogli bismo imati `is_overdue` polje koje je `True` ako je zadatak još uvijek nedovršen nakon određenog vremena od `created_at`. U tom slučaju, dodali bismo `is_overdue = serializers.SerializerMethodField()` i napisali metodu `TaskSerializer.get_is_overdue` (uočite `get_` prefiks) koja računa tu vrijednost.

`ModelSerializer` pretpostavlja da su ulazni i izlazni podaci manje više jednako strukturirani kao model u bazi, uz spomenute prilagodbe. Alternativa je `serializers.Serializer` koja ne pretpostavlja da želimo pisati/čitati polja točno jedne instance. Osim toga, opcija je i da izravno pristupamo modelima iz pogleda, uz nedostatak da tada sami pišemo validaciju.

### Views

Svaki je API endpoint jedan *view*. To je funkcija ili klasa koja prima HTTP zahtjev i vraća HTTP odgovor.
Mi ćemo koristiti tzv. *function-based* *views* gdje za svaki endpoint (`/api/endpoint`) pišemo Python funkciju s dekoratorom `@api_view`.

`api/views.py`:

```python
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.pagination import PageNumberPagination
from rest_framework.response import Response

from .models import Task
from .serializers import TaskSerializer


@api_view(['GET', 'PATCH', 'DELETE'])
def task_detail(request, pk):
    try:
        task = Task.objects.get(pk=pk)
    except Task.DoesNotExist:
        return Response(status=status.HTTP_404_NOT_FOUND)

    if request.method == 'GET':
        serializer = TaskSerializer(task)
        return Response(serializer.data)

    if request.method == 'PATCH':
        serializer = TaskSerializer(task, data=request.data, partial=True)
        serializer.is_valid(raise_exception=True)  # DRF zahtijeva da uvijek prije .save pozovemo .is_valid
        serializer.save()
        return Response(serializer.data)

    task.delete()
    return Response(status=status.HTTP_204_NO_CONTENT)


@api_view(['GET', 'POST'])
def task_list(request):
    if request.method == 'GET':
        # PageNumberPagination vraća podskup rezultata, korisno za vrlo velike tablice:
        paginator = PageNumberPagination()
        page = paginator.paginate_queryset(Task.objects.all(), request)
        serializer = TaskSerializer(page, many=True)
        return paginator.get_paginated_response(serializer.data)

        # Uobičajenija alternativa bez paginacije:
        # tasks = Task.objects.all()
        # serializer = TaskSerializer(tasks, many=True)
        # return Response(serializer.data)

    serializer = TaskSerializer(data=request.data)
    serializer.is_valid(raise_exception=True)
    serializer.save()
    return Response(serializer.data, status=status.HTTP_201_CREATED)
```

Uspješni zahtjevi s `GET` i `PATCH` metodama vraćaju klijentu (pozivatelju API-a) status 200 (default), za ostale metode (`POST`, `DELETE`) eksplicitno postavljamo statuse jer za njih HTTP standard očekuje 201 odnosno 204 (no, većina bi klijenata ispravno protumačila 200 kao uspjeh za sve HTTP metode).

`@api_view(['GET', 'POST'])` znači da funkcija prihvaća GET i POST HTTP zahtjeve. Parametar `partial` u konstruktoru serializera znači da `data` uključuje samo polja koja se mijenjaju, dok ostala ostaju ista kao u `task`.

Trebamo definirati putanje za implementirane endpointe. Stvorite `api/urls.py`:

```python
from django.urls import path

from . import views


urlpatterns = [
    path('tasks/', views.task_list, name='task-list'),
    path('tasks/<int:pk>/', views.task_detail, name='task-detail'),
]
```

U `config/urls.py` uključite `api.urls`:

```python
from django.contrib import admin
from django.urls import include, path

urlpatterns = [
    path('admin/', admin.site.urls),
    path('api/', include('api.urls')),
]
```

### Paginacija

DRF podržava automatsku paginaciju kroz globalnu postavku `REST_FRAMEWORK["DEFAULT_PAGINATION_CLASS"]`, ali ona se ne primjenjuje u našem slučaju (*function-based* *views*). Veličina stranice `REST_FRAMEWORK["PAGE_SIZE"]` primjenjuje se i u našem slučaju. 
Primjer ovih postavki u `config/settings.py`:

```python
REST_FRAMEWORK = {
    'DEFAULT_AUTHENTICATION_CLASSES': [],
    'DEFAULT_PAGINATION_CLASS': 'rest_framework.pagination.PageNumberPagination',
    'PAGE_SIZE': 20,
}
```

Postavljanjem `DEFAULT_AUTHENTICATION_CLASSES` na praznu listu isključujemo DRF-ovu autentikaciju na razini API-ja (ne i administracijskog sučelja). Kasnije ćemo dodati svoju autentikaciju (baziranu na Microsoftovom Entra ID).

Ovako izgleda izlaz GET HTTP metode na `/api/tasks/`:

```json
{
  "count": 42,
  "next": "http://.../api/tasks/?page=2",
  "previous": null,
  "results": [...]
}
```

Klijent dohvaća sljedeću stranicu kroz `?page=2` URL parametar. U frontendu, da bismo dobili sve zadatke iz baze, prolazimo kroz sve stranice u petlji i spajamo `data.results` u jednu listu (vidi `fetchTasks` niže). U praksi je obično bolje prikazati stranicu po stranicu (npr. kroz Vuetifyjev `v-data-table-server`) ili koristiti tzv. *infinite scroll*.

### Provjera

Pokrenite server (ako već nije pokrenut):

```bash
python manage.py runserver
```

Posjetite [http://localhost:8000/api/tasks/](http://localhost:8000/api/tasks/). DRF nudi 'browsable API' sučelje preko kojeg se mogu stvarati zahtjevi direktno iz web preglednika. Primjerice, možete dodati zadatak i vidjeti odgovor. Ovo sučelje dostupno je samo kad DRF detektira da klijent prihvaća HTML (umjesto npr. JSON-a), u suprotnom se vraća samo JSON. S obzirom na to da smo postavili proxy u Viteu, kad pozovemo `http://localhost:5173/api/tasks/` (port 5173 umjesto porta 8000) dobit ćemo isti izlaz.

## Pozivanje API-a iz frontenda i `vue-query`

Sad želimo da frontend dohvaća i mijenja podatke kroz API, umjesto da koristi `localStorage`.

Kad radimo s vanjskim resursima poput API-a čiji pozivi imaju neko trajanje i završavaju uspjehom ili greškom, korisno je da sučelje reagira na trenutno stanje poziva. Primjerice, vrijeme prije poziva, poziv u tijeku i čekanje da završi, poziv je završio uspješno i poziv je završio s greškom. Ovisno o stanju, imat ćemo različite posljedice po naše sučelje. Neki primjeri:
- Čekanje: vjerojatno želimo onemogućiti ponavljanje interakcije koja je dovela do poziva (npr. gumb "Add" kad dodajemo zadatak) kako ne bismo duplicirali radnju. Osim toga, možda želimo prikazati progress bar.
- Uspjeh: želimo prikazati nove podatke i možda otkriti neke dijelove sučelja koji ovise o tim podacima.
- Greška: želimo prikazati poruku o grešci i onemogućiti interakciju s elementima koji ovise o uspješnom pozivu.

Kako je reakcija na stanje poziva čest obrazac, koristit ćemo biblioteku `vue-query` (poznatu i kao TanStack Query) koja olakšava rad s asinkronim pozivima i njihovim stanjima.

Instalacija biblioteke `vue-query`:

```bash
cd frontend
npm install @tanstack/vue-query
```

U `src/plugins/index.ts`, registrirajte vue-query plugin uz Vuetify:

```typescript
import type { App } from 'vue'

import { VueQueryPlugin } from '@tanstack/vue-query'
import vuetify from './vuetify'

export function registerPlugins (app: App) {
  app.use(vuetify)
  app.use(VueQueryPlugin)
}
```

## Tipovi, API pozivi i obrada grešaka

`src/types.ts`:

```typescript
export type Priority = 'Low' | 'Medium' | 'High'

export type Task = {
  id: number
  title: string
  done: boolean
  priority: Priority
  created_at: string
}

export type TasksPage = {
  count: number
  next: string | null
  previous: string | null
  results: Task[]
}
```

DRF u pravilu vraća greške (HTTP statusi 4xx i 5xx) u jednom od dva oblika: za većinu iznimki  tijelo je `{"detail": "..."}`, dok za greške validacije (status 400) tijelo je `{"polje": ["poruka", ...], ...}`. 

U frontendu možemo razlikovati ove greške, što je često poželjno. Npr. za greške validacije nema smisla korisniku sugerirati da pokuša ponovno kasnije, dok za većinu drugih grešaka ima smisla pokušati ponovno kasnije. Definirat ćemo funkciju `throwIfError` koja razlikuje ova dva slučaja: za status 400 proizvodi `ValidationError` koji čuva objekt `fields` (polja s grešakama i njihove greške), a inače proizvodi ugrađeni `Error` object s `detail` porukom kao tekstom greške (ili samo s HTTP statusom ako tijelo nije JSON). Definirat ćemo i funkciju `formatError` čija je uloga proizvesti korisniku čitljiv tekst na temelju dane greške.

```typescript
// src/api.ts

import type { Priority, Task, TasksPage } from './types'

export class ValidationError extends Error {
  constructor(public fields: Record<string, string[]>) {
    super('Validation error')
  }
}

async function throwIfError(response: Response) {
  if (response.ok) return
  let body
  try {
    body = await response.json()
  } catch {
    throw new Error(`${response.status} ${response.statusText}`)
  }
  if (response.status === 400) throw new ValidationError(body)
  throw new Error(body.detail ?? `${response.status} ${response.statusText}`)
}

export function formatError(err: unknown) {
  if (err instanceof ValidationError) {
    return Object.entries(err.fields)
        .map(([field, msgs]) => `${field}: ${msgs.join(', ')}`)
        .join('\n')
  }
  return err instanceof Error ? err.message : String(err)
}

export async function fetchTasks() {
  // Spajamo sve stranice u jednu listu. U praksi bismo umjesto toga dohvaćali
  // stranicu po stranicu (npr. v-data-table-server ili infinite scroll).
  const tasks: Task[] = []
  let page = 1
  while (true) {
    const response = await fetch(`/api/tasks/?page=${page}`)
    if (response.status === 404 && page > 1) break
    await throwIfError(response)
    const data = await response.json() as TasksPage
    tasks.push(...data.results)
    if (!data.next) break
    ++page
  }
  return tasks
}

export async function createTask(payload: { title: string; priority: Priority }) {
  const response = await fetch('/api/tasks/', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload),
  })
  await throwIfError(response)
  return await response.json() as Task
}

export async function updateTask(task: Task) {
  const response = await fetch(`/api/tasks/${task.id}/`, {
    method: 'PATCH',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(task),
  })
  await throwIfError(response)
  return await response.json() as Task
}

export async function deleteTask(id: number) {
  const response = await fetch(`/api/tasks/${id}/`, { method: 'DELETE' })
  await throwIfError(response)
}
```

## `useQuery`

Funkcija `useQuery` iz `vue-query` biblioteke poziva funkciju za dohvat podataka prati njeno stanje. Vraća `data`, `error`, `isPending`, `isLoading`, `isFetching`, `isSuccess`, `isError` i druge reference kojima možemo pratiti stanje poziva.

```typescript
import { useQuery } from '@tanstack/vue-query'
import { fetchTasks } from '@/api'

const tasksQuery = useQuery({
  queryKey: ['tasks'],
  queryFn: fetchTasks,
})

// tasksQuery.data.value je Task[] | undefined
// tasksQuery.isPending.value je boolean
```

`queryKey` je niz koji jedinstveno identificira query. Npr. u nekoj kompleksnijoj aplikaciji mogao bi biti nešto poput `["korisnik", "a@a.com", "album", "Ljeto 2025", "slika", "5"]` za dohvatiti sliku pod rednim brojem 5 u albumu nekog korisnika. Ovakvi hijerarhijski ključevi korisni su kad želimo napraviti neku akciju za sve ključeve s danim prefiksom (npr. ponovno dohvatiti sve slike danog korisnika). Ako više komponenti pozove `useQuery` s istim `queryKey`, vue-query izvrši samo jedan dohvat i sve komponente dijele rezultat (*cache*).

Osim `queryKey` i `queryFn`, možemo proslijediti još neke opcije:
- `enabled`: čim postane `true`, započinje prvi poziv `queryFn`; ponekad je korisno postaviti na neki uvjet prije kojeg ne želimo započeti poziv (default: true);
- `refetchOnMount`: treba li se `queryFn` pozvati kad se komponenta prikaže u DOM-u (default: true);
- `refetchOnWindowFocus`: treba li se `queryFn` pozvati kad se prozor vrati u fokus (default: true);
- `refetchOnReconnect`: treba li se `queryFn` pozvati kad se, nakon gubitka internetske veze, veza ponovno uspostavi (default: true);
- `refetchInterval`: ako je postavljeno na broj, `queryFn` će se pozivati periodično svakih toliko milisekundi (default: false);
- `onSuccess`, `onError`: funkcije koje se pozivaju nakon uspješnog odnosno neuspješnog poziva `queryFn`, pokušajte izbjeći da logika u njima mijenja stanje aplikacije osim jednokratnih radnji poput prikaza notifikacija. Lako je zaboraviti da se `queryFn` i `onSuccess` funkcije neće pozvati ako su podaci već u memoriji od ranije, pa ako imamo logiku u `onSuccess`, ta se logika tada neće izvršiti. Ova se napomena odnosi na `useQuery`, za `useMutation` (sljedeća sekcija) ne trebamo izbjegavati `onSuccess` i `onError`.


## `useMutation`

`useMutation` koristimo za `POST`, `PATCH` i `DELETE` HTTP metode. Za razliku od `useQuery` funkcija, `useMutation` funkcije ne pozivaju se automatski, već pozivom funkcije `mutate(...)` ili `mutateAsync(...)` (potonja podržava `await mutateAsync(...)`).

Često se mutacijom mijenjaju podaci koje čita neki `useQuery`. Npr. na istoj je stranici mutacija za dodavanje novog zadatka, i `useQuery` koji čita popis svih zadataka. Vue ne može znati da je to dvoje povezano, pa moramo sami osvježiti `useQuery` podatke nakon promjene. Imamo nekoliko opcija:

1. U `onSuccess` izravno izmijeniti `useQuery` podatke kroz `queryClient.setQueryData(...)`.
2. Umjesto da se oslanjamo na `data` koji vraća `useQuery`, možemo kao i ranije u `localStorage` primjerima imati vlastite varijable za lokalno stanje. Promjene zapisujemo lokalno i na server, ali ne čitamo podatke sa servera osim kad moramo (npr. inicijalno učitavanje).
3. U `onSuccess` možemo pozvati `queryClient.invalidateQueries({ queryKey })` koja uzrokuje ponovno dohvaćanje podataka s backenda za sve ključeve koji započinju prefiksom `queryKey`. Dakle, nakon uspješne izrade zadatka ponovno bismo učitali zadatke koji bi sada među sobom imali i novi zadatak. 
4. Kad smo ranije spremali podatke u `localStorage`, na svaku promjenu lokalnog stanja spremali bismo čitavo stanje (npr. sve zadatke) iznova u `localStorage` koristeći `watchEffect`. U principu to možemo i sada napraviti. No želimo izbjeći velike HTTP zahtjeve (koji sadrže npr. stotine zadataka) i nepotrebno ponovno pisanje u bazu. 
 
Treći je pristup najjednostavniji pristup, ali uzrokuje nepotreban poziv serveru. Primjerice, ako je stvaranje zadatka na serveru bilo uspješno, ne moramo zbog toga ponovno dohvaćati sve stare zadatke. Umjesto toga, možemo zaključiti da su prisutni svi zadaci kao ranije, osim novododanog. Osim nepotrebnog poziva serveru, drugi se problem s ovim pristupom javlja kad radimo više istovremenih promjena (npr. u kratkom vremenu pokušamo dodati 10 zadataka). Svaka od ovih promjena pozvat će `invalidateQueries`. No, `vue-query` ne podržava istovremeno paralelno dohvaćanje za isti query, pa svaki poziv `invalidateQueries` prekida dotadašnji poziv dohvaćanja zadataka i pokreće novi takav poziv. Ako smo u kratkom vremenu pokušali stvoriti 10 zadataka, tek ćemo nakon uspješnog dodavanja desetog zadatka i ponovnog dohvaćanja svih zadataka vidjeti prvi dodani zadatak. 

U praksi su prva tri pristupa prihvatljiva. No zbog navedenih nedostataka trećeg pristupa, mi ćemo preferirati prva dva pristupa. U primjerima u ovom poglavlju prvi je pristup najprikladniji.

```typescript
import { useMutation, useQueryClient } from '@tanstack/vue-query'
import { createTask } from '@/api'
import type { Task } from '@/types'

const queryClient = useQueryClient()

const createMutation = useMutation({
  mutationKey: ['createTask'],
  mutationFn: createTask,
  onSuccess: (outputData, inputData) => {
    queryClient.setQueryData<Task[]>(['tasks'], (old) =>
        old ? [outputData, ...old] : [outputData],
    )
  },
})

// Npr. kad korisnik klikne 'Add':
createMutation.mutate({ title: 'Do work', priority: 'Low' })
```

`onSuccess` prima dva argumenta: `outputData` (povratna vrijednost funkcije `mutationFn`, u ovom primjeru to je zadatak koji nam je vratio backend) i `inputData` (ulaz s kojim je `mutationFn` pozvana, npr. `{ title: 'Do work', priority: 'Low' }`). Analogni `useMutation` pozivi za `updateTask` i `deleteTask` bit će dio Zadatka 6.

`mutationKey` (npr. `['createTask']`) identificira mutaciju. Koristit ćemo te ključeve za dobiti popis mutacija koje se trenutno izvršavaju.

## `useMutationState`

Ponekad je korisno znati koji se `useMutation` pozivi trenutno odvijaju. Npr. za vrijeme `PATCH` ili `DELETE` poziva nad nekim zadatkom, za taj zadatak želimo onemogućiti *checkbox* za promjenu završenosti zadatka te i gumb za brisanje. Za to koristimo `useMutationState`, koja vraća reaktivnu listu varijabli (ulaza) trenutno aktivnih mutacija čiji `mutationKey` započinje zadanim prefiksom.

```typescript
import { useMutationState } from '@tanstack/vue-query'

const updatingTaskIds = useMutationState({
  filters: { mutationKey: ['updateTask'], status: 'pending' },
  select: (m) => (m.state.variables as Task).id
})

const deletingTaskIds = useMutationState({
  filters: { mutationKey: ['deleteTask'], status: 'pending' },
  select: (m) => m.state.variables as number,
})
```

`updatingTaskIds.value` i `deletingTaskIds.value` su sad reaktivne liste ID-eva zadataka čije su izmjene odnosno brisanja u tijeku. Koristit ćemo ih u Zadatku 6 za prikazivanje stanja čekanja u UI-ju.

## start_local.sh

Za jednostavno pokretanje frontenda i backenda jednom naredbom imamo `start_local.sh` u korijenu (uz `frontend/` i `backend/` direktorije):

```bash
#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Starting backend..."
cd "$SCRIPT_DIR/backend"

source .venv/bin/activate
python manage.py makemigrations
python manage.py migrate
python manage.py runserver &
BACKEND_PID=$!

echo "Starting frontend..."
cd "$SCRIPT_DIR/frontend"

npx vite &
FRONTEND_PID=$!

cleanup() {
  trap - EXIT INT TERM
  echo "Stopping..."
  kill $BACKEND_PID $FRONTEND_PID 2>/dev/null || true
  wait $BACKEND_PID $FRONTEND_PID 2>/dev/null || true
  echo "Done"
}
trap cleanup EXIT INT TERM

echo "Frontend: http://localhost:5173"
echo "Admin: http://localhost:8000/admin/"

wait
```

Pokrenite `chmod +x start_local.sh` prije prvog pokretanja, i potom:

```bash
./start_local.sh
```

Frontend je sad dostupan na [http://localhost:5173](http://localhost:5173). Skripta po potrebi stvara i pokreće migracije, i potom pokreće backend i frontend servere u pozadini.
Skriptu zaustavljate s `Ctrl + C`, što će zaustaviti oba servera


## Autentikacija

U današnjim web aplikacijama za autentikaciju gotovo se isključivo koristi OIDC protokol. U tom protokolu razlikujemo sljedeće glavne komponente:
- *Identity provider (IdP)*: servis koji pohranjuje korisničke račune. U našem slučaju to je Microsoftova Entra ID platforma (bivši Azure Active Directory), što je ujedno i najčešće korišten IdP. Vrlo je popularan i Google Identity Platform ('Login with Google').
- *Resource server*: servis koji imamo potrebu zaštititi (zbog kojeg nam je potrebna autentikacija). Obično je to neki API. U našem slučaju to je naš backend API. 
- *Client*: aplikacija koja želi pristupiti resursu (API-ju) u ime korisnika. U našem slučaju to je frontend aplikacija. Ponekad se klijent i resurs mogu promatrati kao jedna cjelina, što ćemo i mi napraviti kako bismo pojednostavili konfiguraciju.

Više je metoda ('flow') koje klijent može koristiti da se autenticira i dobije pristup resursu. Mi ćemo koristiti tzv. 'authorization code flow with PKCE'. U praksi ćete se vjerojatno susretati i s 'client credentials flow' te 'authorization code, confidential' (ako vas zanima jednostavno rješenje za OIDC izvan ovog kolegija, pogledajte OAuth2 Proxy, koji se oslanja na potonji). Ovo su koraci u authorization code flow with PKCE:
1. Klijent (npr. frontend) preusmjeri korisnika na IdP (Entra ID) da se autenticira. Uz to, klijent šalje i neke dodatne informacije, poput `client_id` (koji identificira klijenta), `scope` (koji specificira koje profilne informacije trebaju klijentu, i koje privilegije klijent treba na resursu) i `redirect_uri` (koji specificira gdje IdP treba preusmjeriti browser nakon autentikacije).
2. Nakon uspješne autentikacije, IdP preusmjeri korisnika natrag na klijentsku aplikaciju (npr. URL frontenda) uz postavljen authorization code.
3. Klijent zatim koristi authorization code da zatraži tzv. *ID token* (informacije o korisniku) i *access token* (token koji će se koristiti za pristup resurs serveru) od IdP-a. 
4. Ako je sve u redu, IdP vraća ID token i access token klijentu.
5. Klijent sada može pročitati informacije o korisniku iz ID tokena, i pozivati resurs. Npr., ako je resurs HTTP API, najčešće se *access token* šalje kao HTTP headeru koji se zove `Authorization`. Resurs koristi javni ključ IdP-a (JWKS) da provjeri je li token valjan i ima li potrebne privilegije. Ako je sve u redu, izvršava zahtjev klijenta. Isti access token koristi se i u budućim pozivima API-a, dok ne istekne trajanje tokena.

Kako bismo mogli koristiti OIDC, prvo moramo registrirati naš resurs i našeg klijenta u Entra ID-u; u našem slučaju, klijent i resurs promatramo kao istu aplikaciju, pa će nam trebati samo jedna registracija. To radimo kroz Azure Portal:

1. Otvorite [Azure Portal -> App registrations -> New registration](https://portal.azure.com/#view/Microsoft_AAD_RegisteredApps/ApplicationsListBlade)
2. Name: `RGKS App`
3. Supported account types: Single tenant
4. Redirect URI: **Single-page application (SPA)**, upišite `http://localhost:5173`
5. Kliknite na Register. Nakon registracije, otvorite *Overview* stranicu registrirane aplikacije i kopirajte *Tenant ID (directory)* i *Client ID (application)*.
6. Kliknite na **Manage**, **Expose an API**, pa **Add a scope**. U formi koja se pojavi:
   - Postavite Application ID URI: `api://client-id` (zamijenite `client-id` stvarnom vrijednošću Client ID-a iz prethodnog koraka) i potvrdite unos,
   - Dodajte scope: `access_as_user` (arbitrarno ime), postavite da obični korisnici mogu dati *consent* za korištenje aplikacije, te ostala polja ispunite po želji, npr. ime može biti "Pristup RGSK aplikaciji". Spremite promjene.
7. Otvorite **Manifest** i postavite `"requestedAccessTokenVersion": 2`. Kliknite **Save**.

U direktoriju gdje se nalazi `start_local.sh` stvorite i datoteku naziva `.env`. Zalijepite Tenant ID i Client ID u tu datoteku: 
```
VITE_CLIENT_ID=...
VITE_TENANT_ID=...
```

### Integracija autentikacije

Sad trebamo prilagoditi naš frontend kako bi se ponašao kao OIDC klijent, i naš backend kako bi se ponašao kao OIDC resurs:
- Dodat ćemo biblioteku za autentikaciju za frontend (MSAL) i povezati ju s ostatkom frontend koda.
- U backendu dodajemo provjeru *bearer* tokena u `Authorization` headeru svakog API zahtjeva, i pišemo kod koji povezuje Django korisničke račune (`User`) s Entra ID korisničkim računima.

#### Frontend

Instalirajte MSAL (*Microsoft Authentication Library*):

```bash
cd frontend
npm i @azure/msal-browser
```

Vite očekuje `.env` u korijenu frontend projekta (`frontend/`), no naš se `.env` nalazi jedan direktorij iznad (uz `start_local.sh`). Zato u `vite.config.mts` dodajemo `envDir`:

```typescript
// vite.config.mts
...
  envDir: fileURLToPath(new URL('..', import.meta.url)),
  server: {
    port: 5173,
    proxy: {
      "/api": {
        target: "http://localhost:8000",
        changeOrigin: true,
      },
    },
  }
...
```

`@azure/msal-browser` ne koristi Vue reaktivnost, pa pišemo reaktivni *wrapper* u `src/auth/useAuth.ts`. Trebamo sljedeće funkcionalnosti: inicijalizacija MSAL biblioteke, referenca `account` koja čuva trenutnog korisnika, funkcije `login` i `logout`, te funkcija koja dohvaća *access token* za pozive prema našem API-u.

```typescript
// src/auth/useAuth.ts
import {
  EventType,
  InteractionRequiredAuthError,
  PublicClientApplication,
  type AccountInfo,
  type Configuration,
  type SilentRequest,
} from '@azure/msal-browser'
import { computed, ref } from 'vue'

// Dohvaćanje varijabli iz .env datoteke: import.meta.env.VITE_* (važno je da varijabla počinje prefiksom VITE_)
// Client ID (app registration) i Tenant ID (Azure AD tenant) imaju iste vrijednosti za sva okruženja.
// No, kako se radi o donekle povjerljivim podacima, ne želimo ih imati kao dio git koda.
// Ne bi bio ozbiljan problem da se nalaze u kodu jer će biti vidljivi u JavaScriptu koji se šalje klijentu.

const clientId = import.meta.env.VITE_CLIENT_ID
const tenantId = import.meta.env.VITE_TENANT_ID

const msalConfig: Configuration = {
  auth: {
    clientId,
    authority: `https://login.microsoftonline.com/${tenantId}`,
    redirectUri: window.location.origin,
  },
  cache: { cacheLocation: 'sessionStorage' },
}

const tokenRequest: SilentRequest = {
  // Kod izrade App Registration odabrali smo ime access_as_user za 'scope' koje je bilo arbitratno, sad je važno da koristimo isto ime
  scopes: [`api://${clientId}/access_as_user`],
}

let msalInstance: PublicClientApplication | null = null
const accounts = ref<AccountInfo[]>([])
const isAuthenticated = computed(() => accounts.value.length > 0)
const account = computed(() => accounts.value[0] ?? null)

export async function initAuth() {
  // Ova funkcija poziva se samo jednom (u main.ts) i inicijalizira globalne varijable u ovoj datoteci.
  msalInstance = new PublicClientApplication(msalConfig)
  await msalInstance.initialize()

  // Kad Azure završi login proces, preusmjerava preglednik na našu aplikaciju.
  // U tom slučaju MSAL treba pročitati postavljene parametre u URL-u i obraditi ih.
  // Ovo želimo napraviti prije inicijalizacije Vue aplikacije, jer bi Azureovi URL parametri (`http://localhost:5173?code=...#state=...`)  mogli zbuniti našu aplikaciju.
  await msalInstance.handleRedirectPromise()
  accounts.value = msalInstance.getAllAccounts()

  msalInstance.addEventCallback((event) => {
    if (event.eventType === EventType.LOGIN_SUCCESS ||
        event.eventType === EventType.ACQUIRE_TOKEN_SUCCESS) {
      accounts.value = msalInstance!.getAllAccounts()
    }
    if (event.eventType === EventType.LOGOUT_SUCCESS) {
      accounts.value = []
    }
  })
}

function login() {
  msalInstance!.loginRedirect({ scopes: ['openid', 'profile', 'email'] })
}

function logout() {
  msalInstance!.logoutRedirect()
}

async function getApiToken(): Promise<string> {
  // ID token sadrži podatke o korisniku, ali ne i dopuštenja koja korisnik ima za pojedini API.
  // Zbog toga kad šaljemo zahtjeve vanjskim servisima (uključujući i naš API), trebamo access token.
  try {
    const response = await msalInstance!.acquireTokenSilent({
      ...tokenRequest,
      account: account.value!,
    })
    return response.accessToken
  } catch (error) {
    if (error instanceof InteractionRequiredAuthError) {
      // Preusmjerava na login; ovim pozivom završava cijela naša skripta jer browser napušta trenutnu stranicu
      await msalInstance!.acquireTokenRedirect(tokenRequest)
    }
    throw error
  }
}

export function useAuth() {
  return { isAuthenticated, account, login, logout, getApiToken }
}
```

Kako moramo pričekati da `initAuth()` završi prije inicijalizacije Vue aplikacije, moramo dodati asinkronu funkciju (npr. `main` ili `bootstrap`) koja može pozvati `await initAuth()`, i unutar te funkcije inicijalizirati Vue aplikaciju. To radimo u `src/main.ts`:

```typescript
// src/main.ts
import { createApp } from 'vue'
import App from './App.vue'
import { initAuth } from './auth/useAuth'
import { registerPlugins } from '@/plugins'

async function bootstrap() {
  await initAuth()
  const app = createApp(App)
  registerPlugins(app)
  app.mount('#app')
}

bootstrap()
```

Sad u `src/api.ts` u svakom `fetch` pozivu prema backendu dohvaćamo *access token* i dodajemo ga u `Authorization` header:

```typescript
// src/api.ts (modifikacija postojećih funkcija)
import { useAuth } from './auth/useAuth'

const { getApiToken } = useAuth()

export async function fetchTasks() {
  const tasks: Task[] = []
  let page = 1
  while (true) {
    const token = await getApiToken()
    const response = await fetch(`/api/tasks/?page=${page}`, {
      headers: { Authorization: `Bearer ${token}` },
    })
    if (response.status === 404 && page > 1) break
    await throwIfError(response)
    const data = await response.json() as TasksPage
    tasks.push(...data.results)
    if (!data.next) break
    ++page
  }
  return tasks
}

export async function createTask(payload: { title: string; priority: Priority }) {
  const token = await getApiToken()
  const response = await fetch('/api/tasks/', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json', Authorization: `Bearer ${token}` },
    body: JSON.stringify(payload),
  })
  await throwIfError(response)
  return await response.json() as Task
}
```

Slične promjene možete dodati u preostale funkcije koje pozivaju API. Zatim mijenjamo `App.vue`.

```typescript
// src/App.vue, promjene u <script setup>
import { useAuth } from './auth/useAuth'

const { isAuthenticated, account, login, logout } = useAuth()
```

U `<v-app-bar>`, kroz `#append` slot, dodajemo ime prijavljenog korisnika i *Log in* i *Log out* radnje.

```vue
<v-app-bar color="primary">
  <v-app-bar-title>To-do List</v-app-bar-title>

  <template #append>
    <span v-if="isAuthenticated" class="mr-3 text-body-2">
      {{ account?.name ?? account?.username }}
    </span>
    <v-btn v-if="!isAuthenticated" variant="outlined" @click="login">Log in</v-btn>
    <v-btn v-else variant="outlined" @click="logout">Log out</v-btn>
  </template>
</v-app-bar>
```

Kad korisnik nije prijavljen prikazujemo poruku:

```vue
<v-main>
  <v-container>
    <v-alert v-if="!isAuthenticated" type="info">
      Log in to view tasks.
    </v-alert>

    <v-card v-else variant="outlined">
      <!-- ... -->
    </v-card>
  </v-container>
</v-main>
```

#### Backend

Pišemo vlastitu DRF autentikacijsku klasu koja za svaki HTTP zahtjev čita *access token* iz `Authorization` headera, lokalno verificira njegov potpis koristeći javne ključeve Entra ID-a (JWKS), i ako je sve u redu vraća odgovarajući Django `User` zapis. Za parsiranje i verifikaciju JWT-a koristimo `PyJWT` paket (aktivirajte virtualno okruženje i pokrenite `poetry add PyJWT`).

```python
# api/authentication.py

# Iako za autentikaciju koristimo Azure, a ne Django (koji nudi nekoliko vlastitih tipova autentikacije),
# uz donju klasu i dalje možemo koristiti primjerice request.user za pristup informacijama o korisniku.

import jwt
from django.conf import settings
from django.contrib.auth.models import User
from jwt import PyJWKClient
from rest_framework.authentication import BaseAuthentication
from rest_framework.exceptions import AuthenticationFailed

_jwk_client: PyJWKClient | None = None


def _get_jwk_client() -> PyJWKClient:
    global _jwk_client
    if _jwk_client is None:
        jwks_url = (
            f"https://login.microsoftonline.com/"
            f"{settings.AZURE_AD_TENANT_ID}/discovery/v2.0/keys"
        )
        _jwk_client = PyJWKClient(jwks_url)
    return _jwk_client


class AzureADAuthentication(BaseAuthentication):
    def authenticate(self, request):
        auth_header = request.META.get("HTTP_AUTHORIZATION", "")
        if not auth_header.startswith("Bearer "):
            return None

        token = auth_header[7:]
        try:
            signing_key = _get_jwk_client().get_signing_key_from_jwt(token)
            payload = jwt.decode(
                token,
                signing_key.key,
                algorithms=["RS256"],
                audience=settings.AZURE_AD_CLIENT_ID,
                issuer=[
                    f"https://login.microsoftonline.com/{settings.AZURE_AD_TENANT_ID}/v2.0",
                    f"https://sts.windows.net/{settings.AZURE_AD_TENANT_ID}/",
                ],
                options={"require": ["exp", "nbf", "iss", "aud", "sub"]},
            )
        except jwt.ExpiredSignatureError:
            raise AuthenticationFailed("Token has expired.")
        except jwt.InvalidTokenError as exc:
            print("Token validation failed:", exc)
            raise AuthenticationFailed(f"Invalid token: {exc}")

        user = self._get_or_create_user(payload)
        return (user, payload)

    @staticmethod
    def _get_or_create_user(payload: dict) -> User:
        sub = payload.get("sub", "")
        if not sub:
            raise AuthenticationFailed("Token missing 'sub' claim.")

        given_name = payload.get("given_name", "")
        family_name = payload.get("family_name", "")
        full_name = payload.get("name", "")

        if given_name or family_name:
            first_name = given_name
            last_name = family_name
        else:
            first_name = full_name
            last_name = ""

        user, created = User.objects.get_or_create(
            username=sub,
            defaults={
                "email": payload.get("preferred_username", ""),
                "first_name": first_name,
                "last_name": last_name,
            },
        )

        if not created:
            # Korisniku u Entra ID može se promijeniti ime ili email; zbog toga po potrebi osvježavamo Django korisnika
            new_email = payload.get("preferred_username", user.email)
            new_first_name = first_name or user.first_name
            new_last_name = last_name or user.last_name
            if (user.email, user.first_name, user.last_name) != (new_email, new_first_name, new_last_name):
                user.email = new_email
                user.first_name = new_first_name
                user.last_name = new_last_name
                user.save(update_fields=["email", "first_name", "last_name"])

        return user
```

Funkcija `_get_or_create_user` osigurava da se prvi put kad se korisnik prijavi automatski stvori `User` redak u Django bazi. Kao `username` koristimo nepromjenjiv `sub` claim iz Entra ID-a (ne email, jer se email može promijeniti).

U `config/settings.py` čitamo Tenant ID i Client ID iz okruženja te uključujemo novu autentikacijsku klasu. Mijenjamo i `DEFAULT_PERMISSION_CLASSES` tako da svi zahtijevaju autentikaciju:

```python
# config/settings.py
import os

AZURE_AD_TENANT_ID = os.environ.get("VITE_TENANT_ID", "")
AZURE_AD_CLIENT_ID = os.environ.get("VITE_CLIENT_ID", "")

REST_FRAMEWORK = {
    "DEFAULT_AUTHENTICATION_CLASSES": [
        "api.authentication.AzureADAuthentication",
    ],
    "DEFAULT_PERMISSION_CLASSES": [
        "rest_framework.permissions.IsAuthenticated",
    ],
    # paginacija kao i ranije
    "DEFAULT_PAGINATION_CLASS": "rest_framework.pagination.PageNumberPagination",
    "PAGE_SIZE": 20,
}
```

Da bi `start_local.sh` proslijedio `VITE_TENANT_ID` i `VITE_CLIENT_ID` Django procesu, prije pokretanja backenda učitavamo varijable iz `.env`:

```bash
# start_local.sh, prije "Starting backend..."
set -a
source .env
set +a
```

Sad prilikom obrade zahtjeva možemo koristiti `request.user`. To je instanca Django `User` modela koja sadrži podskup podataka iz tokena. Ako neki endpoint želite ostaviti javnim, dodajte mu `@permission_classes([AllowAny])` (iz `rest_framework.decorators` i `rest_framework.permissions`).

```bash
./start_local.sh
```

## Zadatak 6

Promijenite rješenje Zadatka 4 (Vuetify To-Do lista s lokalnim stanjem) tako da koristi `vue-query` i backend umjesto da sprema zadatke u `localStorage`. Glavne razlike:

- `tasks` je sad `computed` svojstvo koje vraća `tasksQuery.data.value`.
- `nextId` nije potreban: backend dodjeljuje ID-eve.
- `addTask`, `toggleTask`, `deleteTask` (preimenujte jer imamo istoimenu funkciju u `api.ts`), `saveEdit` sad pozivaju `mutate(...)` na odgovarajućoj `useMutation` instanci (`createMutation`, `updateMutation`, `deleteMutation`).
- Filteri poput prioriteta neka zasad ostanu na klijentskoj strani, dakle koristeći `computed` svojstvo `filteredTasks` kao i ranije.
- Za query koji dohvaća zadatke prikažite poruku kad se prvi put učitava (`isLoading.value`), npr. koristeći `v-alert` komponentu, i formatiranu poruku o grešci koristeći `formatError` ako dođe do greške (`isError.value`).
- Za svaku mutaciju prikažite formatiranu grešku (ako postoji) u `v-alert` komponenti.

S obzirom na to da serverske operacije mogu potrajati neko vrijeme, ovisno o stanju internetske veze i zagušenja servera, uvodimo i sljedeće promjene:

- `TaskItem` neka prima novo svojstvo `isMutating: boolean`. U `App.vue` postavite ga kao `:isMutating="updatingTaskIds.includes(task.id) || deletingTaskIds.includes(task.id)"`.
- Unutar `TaskItem` komponente, na `v-list-item` komponenti postavite `:disabled="isMutating"`. Vuetify time blokira `@click` handler i CSS stilom signalizira da je ta stavka liste onemogućena. Na delete gumbu unutar reda koristite `:loading="isMutating"` (Vuetify prikazuje animaciju umjesto ikone i blokira klik).
- `TaskForm` neka prima svojstvo `isMutating: boolean`. U `App.vue` izračunajte ga kao: kreiranje u tijeku (`createMutation.isPending`) ili je u tijeku mijenjanje zadatka koji se uređuje (`editingId !== null && (updatingTaskIds.includes(editingId) || deletingTaskIds.includes(editingId))`). Na gumbu unutar forme postavite `:loading="isMutating"`.

Sve komponente možete kopirati iz Zadatka 4 i dodati opisane promjene. Datoteka `ConfirmModal.vue` ostaje ista kao ranije.

## Zadatak 7

U ovom zadatku mijenjamo rješenje prethodnog zadatka. Umjesto `v-list` komponente koja je prikazivala sve zadatke odjednom, koristimo `v-data-table-server` komponentu koja prikazuje samo jednu stranicu zadataka. Među filtere dodajte i Search, tj. tekstualno polje koje filtrira zadatke na način da su prikazani samo zadaci koji sadrže upisan tekst. Filtriranje (pretragu, status i prioritet) prebacite na server (koristite npr. `Task.objects.filter(...)`). Promjena bilo kojeg filtera neka resetira paginaciju na prvu stranicu. 

Kako bi klijent mogao zatražiti svoju veličinu stranice (`v-data-table-server` omogućava odabir veličine stranice) kroz `page_size` URL query parametar, u datoteci `api/views.py` dodajte sljedeći kod, i potom koristite tu klasu u ostatku datoteke `api/views.py` umjesto klase `PageNumberPagination`:

```python
class TaskPagination(PageNumberPagination):
    page_size_query_param = 'page_size'
```

U svim `onSuccess` funkcijama gdje smo do sada mijenjali stanje koristeći `queryClient.setQueryData`, u ovom zadatku koristite `queryClient.invalidateQueries({ queryKey: ['tasks'] })`, jer prisutnost filtriranja i paginacije komplicira mijenjanje lokalnih podataka.

Kako za prikaz pojedinog zadatka već imamo komponentu `TaskItem`, nećemo koristiti uobičajen tablični prikaz koji koristi komponenta `v-data-table-server`. Kod bi trebao imati ovakvu strukturu:

```vue
<v-data-table-server
   ...
>
   <template #item="{ item }">
     <tr>
       <td :colspan="tableHeaders.length" class="pa-0">
         <TaskItem ... />
       </td>
     </tr>
   </template>
</v-data-table-server>
```

`v-data-table-server` podržava i sortiranje po stupcima klikom na ime stupca. Dodajte podršku za sortiranje za naš (jedini) stupac. Neka sortiranje bude sortiranje po vremenu izrade zadatka. U backend API-u osim filtera trebat će stoga primiti i trenutno zatražen poredak sortiranja (ako postoji).

Uz to, dodajte u backend novi model `AuditLog` koji bilježi svaku promjenu u zadacima. Pri svakom stvaranju, izmjeni ili brisanju zadatka treba zapisati novi `AuditLog` redak s referencom na Django korisnika koji je napravio izmjenu (koristite isti tip autentikacije kao do sada) i tipom akcije. Ako se radi o promjeni zadatka, treba pisati novo stanje zadatka kao JSON tekst (npr. `old_state` i `new_state`).
