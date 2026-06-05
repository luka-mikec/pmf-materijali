# Druge teme

Pogledat ćemo još dvije teme:
- **Upravljanje rutama**: primjerice, kako postići da URL putanja poput `/tasks/123` u frontendu koristi komponentu za prikaz zadatka i učita zadatak 123.
- **Testiranje**: automatizirana provjera specifikacija. Dodat ćemo `unit` testove za backend koristeći *pytest*, i tzv. *end-to-end* (E2E) testove za čitavu aplikaciju (koristeći *Playwright*). 


## Vue Router

Većina web aplikacija sadrži više stranica. Ovisno o putanji na kojoj se nalazimo (npr. `/`, `/tasks/123` ili `/settings`) želimo prikazati odgovarajuću stranicu. Koristeći Vue, mogli bismo u `App.vue` napisati nešto poput `<HomePage v-if="isHomePage" /> <SettingsPage v-if="isSettingsPage" />` gdje je `isSettingsPage` npr. `window.location.pathname === '/settings'`. To je legitimno rješenje, ali potreban je dodatan posao za riješiti neke probleme. Npr. `window.location.pathname` nije reaktivno stanje. Zatim, treba nam kod za parsiranje argumenata iz rute (npr. `123` iz `/tasks/123`). Te i druge probleme rješava paket Vue Router. Slični paketi postoje za druge frontend biblioteke (npr. React Router za React).

Pojedino pridruživanje komponente putanji nazivamo ruta.
U našem primjeru imat ćemo sljedeće rute:
- `/`: Početna stranica s popisom zadataka.
- `/tasks/:id`: Detalji zadatka.
- `/tasks/:id/tags`: Prikaz tagova. 

Pretpostavimo da iz nekog razloga ne želimo vidjeti popis tagova na stranici `/tasks/:id` te zbog toga uvodimo `/tasks/:id/tags`.

### Instalacija Vue Routera

Stvorite novi Vuetify projekt (`npm create vuetify@latest`) i, kao i inače, odaberite `Base` preset.
Za ovaj primjer ne treba nam backend, napravit ćemo jednostavan simulirani backend unutar frontenda.

Zatim instalirajte Vue Router (instalirat ćemo i Vue Query):

```bash
cd frontend
npm install vue-router @tanstack/vue-query
```

### Podaci

Nemamo backend pa ćemo zadatke držati u memoriji. Dohvaćat ćemo ih asinkrono jer želimo simulirati dulje trajanje dohvaćanja.

```ts
// src/tasks.ts
export type Priority = 'Low' | 'Medium' | 'High'

export type Task = {
  id: number
  title: string
  done: boolean
  priority: Priority
  tags: string[]
}

const tasks: Task[] = [
  { id: 1, title: 'Task A', done: false, priority: 'Low', tags: ['high priority'] },
  { id: 2, title: 'Task B', done: false, priority: 'High', tags: [] },
  { id: 3, title: 'Task C', done: true, priority: 'Medium', tags: ['meeting', 'postpone'] },
  { id: 4, title: 'Task D', done: false, priority: 'High', tags: ['low priority'] },
  { id: 5, title: 'Task E', done: false, priority: 'High', tags: ['meeting'] },
]

function withDelay<T>(value: T): Promise<T> {
  return new Promise(resolve => setTimeout(() => resolve(value), 300))
}

export function fetchTasks(): Promise<Task[]> {
  return withDelay(tasks)
}

export function fetchTask(id: number): Promise<Task> {
  const task = tasks.find(t => t.id === id)
  if (!task) return Promise.reject(new Error('Task not found'))
  return withDelay(task)
}
```

### Konfiguracija ruta

Rute definiramo u `src/router.ts`. Svaka ruta povezuje putanju (`path`) s Vue komponentom.

```ts
// src/router.ts
import { createRouter, createWebHistory } from 'vue-router'
import type { RouteRecordRaw } from 'vue-router'
import TaskListView from '@/TaskListView.vue'
import TaskDetailView from '@/TaskDetailView.vue'
import TaskTags from '@/TaskTags.vue'

const routes: RouteRecordRaw[] = [
  {
    path: '/',
    name: 'task-list',
    component: TaskListView,
  },
  {
    // Parametar :id funkcionira jednako kao API endpoint parametri u Django `urls.py` datoteci
    path: '/tasks/:id',
    name: 'task-detail',
    component: TaskDetailView,
    children: [
      {
        path: 'tags',
        name: 'task-tags',
        component: TaskTags,
      },
    ],
  },
]

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes,
})

export default router
```

Imena su opcionalna, no korisna su za navigaciju. Npr. kad želimo korisnika preusmjeriti na drugu putanju, umjesto da sami konstruiramo string (što je mukotrpno za složenije putanje i zahtijeva izmjene kad npr. promijenimo poredak parametara), možemo koristiti ime rute i proslijediti parametre kao objekt:

```ts
router.push({ name: 'task-detail', params: { id: 123 } })
```

Napomena: parametri rute poput `id` u primjeru uvijek su stringovi. To obično nije problem kad konstruiramo putanju, npr. u gornjem primjeru Vue Router sam pretvara broj `123` u string `"123"`. No, u komponenti odgovornoj za rutu treba eksplicitno pretvoriti parametre (stringove) u ispravne tipove (npr. `number`)

Vue Router potrebno je registrirati s `app.use(router)`, jednako kao Vuetify i vue-query:

```ts
// src/plugins/index.ts
// Types
import type { App } from 'vue'

// Plugins
import vuetify from './vuetify'
import router from '@/router'
import { VueQueryPlugin } from '@tanstack/vue-query'

export function registerPlugins (app: App) {
  app.use(vuetify)
  app.use(router)
  app.use(VueQueryPlugin)
}
```

Datoteka `App.vue` i dalje postoji, no sada će njena uloga biti prikaz zajedničkog dijela sučelja.
Negdje unutar te datoteke treba nam `<router-view/>`. 
Na tom mjestu prikazivat će se komponenta ovisna o ruti (npr. `TaskDetailView` za `/tasks/:id` itd.).

```vue
<!-- src/App.vue -->
<template>
  <v-app>
    <v-app-bar>
      <v-app-bar-title>To-Do List</v-app-bar-title>
    </v-app-bar>
    <v-navigation-drawer app>
      <v-list-item :to="{ name: 'task-list' }" title="Tasks" />
    </v-navigation-drawer>
    <v-main>
      <v-container>
        <router-view />
      </v-container>
    </v-main>
  </v-app>
</template>
```

Direktiva `to` dostupna je na brojnim Vuetify komponentama (npr. `v-btn`).
Ova direktiva ima ulogu nalik atributu `href`, no referencira imena ruta (umjesto putanje) i omogućava elegantnije prosljeđivanje parametara (npr. `:to="{ name: 'task-detail', params: { id: task.id } }"`).

### Učitavanje podataka u komponenti zaduženoj za rutu

Za prikaz liste zadataka, što je početna stranica naše aplikacije, definiramo komponentu `TaskListView`.
Moguće je da na ovu stranicu dolazimo na dva načina:
- Iz iste aplikacije (npr. klikom na *Tasks* u izborniku). U tom slučaju, moguće je da su zadaci već učitani u memoriji i možda ih nije nužno ponovno preuzimati.
- Na bilo koji drugi način. U tom slučaju, zadatke je potrebno preuzeti.
Stoga ova komponenta ne može pretpostaviti da su zadaci već dostupni.
Vue Query ima jednostavno rješenje za ovaj problem; ako je ključ upita (`queryKey`) već viđen, vratit će se postojeći sadržaj, a inače će se dohvatiti novi.
U sučelju, kao i inače kad imamo asinkrone pozive, treba razlikovati učitavanje, uspjeh i grešku.

```vue
<!-- src/TaskListView.vue -->
<script setup lang="ts">
import { computed } from 'vue'
import { useQuery } from '@tanstack/vue-query'
import { fetchTasks } from '@/tasks'
import type { Task } from '@/tasks'

const tasksQuery = useQuery({ queryKey: ['tasks'], queryFn: fetchTasks })
const tasks = computed<Task[]>(() => tasksQuery.data.value ?? [])
</script>

<template>
  <v-progress-linear v-if="tasksQuery.isLoading.value" indeterminate />
  <v-alert v-else-if="tasksQuery.isError.value" type="error">
    Failed to load tasks.
  </v-alert>
  <v-list v-else>
    <v-list-item
      v-for="task in tasks"
      :key="task.id"
      :to="{ name: 'task-detail', params: { id: task.id } }"
      :title="task.title"
    />
  </v-list>
</template>
```

### Čitanje parametara rute

Implementiramo komponentu `TaskDetailView` za prikaz pojedinog zadatka, odnosno za rutu `/tasks/:id`.
Za razliku od popisa svih zadataka, ova je ruta parametrizirana (`:id`), pa komponenta nekako treba pročitati parametre.

Osim toga, ova ruta sadrži ugniježđenu rutu `/tasks/:id/tags`.

Ugniježđena ruta znači da komponenta `TaskDetailView` treba i sama sadržavati `<router-view />` (povrh `<router-view />` u komponenti `App.vue`).

```vue
<!-- src/TaskDetailView.vue -->
<script setup lang="ts">
import { computed } from 'vue'
import { useRoute } from 'vue-router'
import { useQuery } from '@tanstack/vue-query'
import { fetchTask } from '@/tasks'

const route = useRoute()

// route.params sadrži stringove, uvijek pretvaramo u odgovarajući tip
const taskId = computed(() => parseInt(route.params.id))

const taskQuery = useQuery({
  queryKey: computed(() => ['task', taskId.value]),
  queryFn: () => fetchTask(taskId.value),
})

const task = computed(() => taskQuery.data.value)
</script>

<template>
  <v-progress-linear v-if="taskQuery.isLoading.value" indeterminate />
  <v-alert v-else-if="taskQuery.isError.value" type="error">
    Failed to load task.
  </v-alert>

  <v-card v-else-if="taskQuery.isSuccess.value" variant="outlined">
    <v-card-title>{{ task?.title }}</v-card-title>

    <v-card-text>
      <v-btn :to="{ name: route.name === 'task-detail' ? 'task-tags' : 'task-detail', params: { id: taskId } }" variant="text">Show/hide Tags</v-btn>

      <p>Priority: {{ task?.priority }}</p>
      <p>Status: {{ task?.done ? 'Done' : 'Active' }}</p>

      <!-- Ako unaprijed znamo komponente koje se javljaju unutar (pod)rute, možemo im proslijediti svojstva koja očekuju -->
      <router-view :task="task" />
    </v-card-text>

    <v-card-actions>
      <v-btn :to="{ name: 'task-list' }">Back</v-btn>
    </v-card-actions>
  </v-card>
</template>
```

Umjesto prosljeđivanja zadatka u (pod)rute koristeći svojstva na `router-view` komponenti, mogli smo unutar tih komponenti napisati `useQuery`.
Kako bismo tada koristili isti identifikator zadatka, ne bi bilo nepotrebnih poziva.

Sad dodajemo komponentu `TaskTags` za ugniježđenu rutu.

```vue
<!-- src/TaskTags.vue -->
<script setup lang="ts">
import type { Task } from '@/tasks'

defineProps<{ task: Task }>()
</script>

<template>
  <v-row class="my-4">
    <v-col>
      <v-chip v-for="tag in task.tags" :key="tag" size="small" class="mr-1">{{ tag }}</v-chip>
    </v-col>
  </v-row>
</template>
```

### Query parametri

Osim parametara koji su dio putanje (poput `:id`) koji obično definiraju koji resurs se prikazuje, često se koriste i *query* parametri (npr. `?priority=high`) koji definiraju kako se resurs prikazuje.

Čest je obrazac dio stanja aplikacije, npr. odabrane filtere, držati izravno u URL-u, kao parametre rute ili query parametre.
Kad želimo pročitati vrijednosti parametara, čitamo `route.params` ili `route.query`. 
Kad želimo postaviti vrijednosti, koristimo `router.push` (navigacija na danu putanju) ili `router.replace` (kao `router.push`, ali ne stvara novi element u povijesti preglednika).
Kad stanje čuvamo u URL-u najčešće želimo upravo `router.replace`. Ne želimo svaku promjenu filtera registrirati kao prijelaz u povijesti preglednika.

Koristimo `computed` s `get` i `set` blokom: `get` čita iz URL-a, `set` poziva `router.replace`.

```ts
<!-- src/TaskListView.vue, promjene -->
import { useRoute, useRouter } from 'vue-router'

const route = useRoute()
const router = useRouter()

const searchQuery = computed<string>({
  get() {
    const searchQuery = route.query.searchQuery;
    return typeof searchQuery === "string" ? searchQuery : "";
  },
  set(value) {
    router.replace({ query: { ...route.query, searchQuery: value } })
  },
})

const filtered = computed<Task[]>(() =>
    tasks.value.filter(t => t.title.toLowerCase().includes(searchQuery.value.toLowerCase())),
)
```

```vue
<v-text-field v-model="searchQuery" label="Search" density="compact" hide-details class="mb-2" />

<v-list-item
  v-for="task in filtered"
  ...
/>
```

## Zadatak 9

U primjer u poglavlju dodajte novu rutu `/about` koja koristi komponentu `AboutView.vue`. Neka sadrži kratak opis aplikacije i broj zadataka te postotak završenih zadataka. Dodajte link na novu stranicu u izborniku, uz postojeći link na popis zadataka.

## Testiranje

Automatski testovi osiguravaju da buduće promjene izvornog koda neće unijeti greške, odnosno osiguravaju da se softver nastavlja ponašati u skladu s originalnim specifikacijama.
Treba spomenuti da automatski testovi nisu zamjena za testiranje koje programer treba sam napraviti tijekom razvoja, a koje je u pravilu bitno opsežnije od onoga što se može automatizirati.

Najčešće spominjani tipovi testova:
- Jedinični test (*unit test*): provjerava jednu funkcionalnost (najčešće jednu funkciju) u frontendu ili backendu.
- Integracijski test (*integration test*): provjerava interakciju između odvojenih servisa, primjerice frontenda i backenda ili backenda i baze. Primjer: šaljemo HTTP zahtjev backendu, backend ispravno obrađuje zahtjev i vraća očekivani HTTP odgovor.
- *End-to-end (E2E) test*: provjerava ispravnost funkcionalnosti koje se tiču cijelog sustava, primjerice u pregledniku se simulira klik na gumb, aplikacija izvrši akciju (npr. pozove backend koji pozove bazu) i potom se čita HTML stranice da se provjeri rezultat.

Za jedinične testove koristit ćemo **pytest**. 
Potom ćemo se fokusirati na **E2E testove** (koristeći **Playwright**) gdje simuliramo interakciju u stvarnom pregledniku i provjeravamo ispravnost izvršavanja. 
Oba tipa testova dodat ćemo u naš CI/CD pipeline. Na taj će način biti onemogućeno postavljanje nove verzije aplikacije koja ima greške detektirane testovima. 

### Jedinični backend testovi (*pytest*)

Jedinični test za funkciju `f` ima sljedeću strukturu:
- Priprema: poziv funkcije `f` možda očekuje određene objekte u bazi, environment varijable itd. Prvi je korak stvoriti sve takve potrebne uvjete za izvršavanje funkcije `f`.
- Poziv funkcije `f`.
- Provjera: ispravnost rezultata najčešće provjeravamo koristeći Pythonovu `assert` naredbu koja vraća grešku ako joj argument nije istinit. Npr. `assert x == 5` provjerava da je `x` jednako `5`. Bilo koja greška u testu (zbog `assert` ili bilo kojeg drugog razloga) uzrokuje neuspjeh testa, a inače je test uspješan.

Često pišemo više testova za istu funkciju, ili za više sličnih funkcija.
Tada često u pripremi koristimo i iste objekte, primjerice, testnog korisnika aplikacije (Django `User` objekt).
Takve objekte zovemo *fixtures* i njihovu konstrukciju pišemo u posebnim funkcijama označenima dekoratorom `@pytest.fixture`.
Kad ih želimo koristiti u testnoj funkciji, dovoljno je napisati njihovo ime kao parametar funkcije, i pytest će nam proslijediti konstruirane objekte.

U ovom poglavlju možete kopirati rješenje Zadatka 6 (gdje imamo frontend, backend i autentikaciju) kao početnu točku.
Možete koristiti i bilo koji drugi projekt u kojem ste dodali autentikaciju.

```bash
cd backend
poetry add --group dev pytest pytest-django
```

Parametar `--group dev` znači da će `poetry` ove pakete instalirati samo kad zatražimo instalaciju `dev` grupe paketa. 
U finalnoj Docker slici obično ne želimo `dev` pakete (ne pokrećemo testiranje u produkcijskoj instanci aplikacije).

Paketu `pytest-django` trebamo reći koji Django projekt testiramo. Na kraj datoteke `pyproject.toml` dodajte sljedeće:

```toml
# backend/pyproject.toml
[tool.pytest.ini_options]
DJANGO_SETTINGS_MODULE = "config.settings"
```

Napisat ćemo po jedan test za dva endpointa: `GET /api/tasks/` i `POST /api/tasks/`.
Pozivanje endpoint funkcije kao obične Python funkcije nije sasvim trivijalno jer Django prilikom poziva u pozadini postavlja `request` objekte, poziva middleware, provjerava autentikaciju itd. 
Zbog toga postoji pomoćna klasa `APIClient` (`rest_framework.test.APIClient`), koja simulira pozivanje endpointa.

Kako bismo u jediničnim testovima isključili autentikaciju koju DRF očekuje, koristimo funkciju `APIClient.force_authenticate(user=...)`. 
Pritom ćemo kao argument poslati stvarnog privremeno stvorenog korisnika jer naši endpointi u nekim slučajevima čitaju `request.user`.


```python
# backend/api/test_tasks.py
import pytest
from rest_framework.test import APIClient

from django.contrib.auth.models import User
from api.models import Task


@pytest.fixture
def user():
    return User.objects.create(username='test_user')


@pytest.fixture
def client(user):
    api_client = APIClient()
    api_client.force_authenticate(user=user)
    return api_client


# Svi testovi koji trebaju bazu označeni su dekoratorom @pytest.mark.django_db
@pytest.mark.django_db
def test_list_tasks(client):
    # Priprema
    Task.objects.create(title='A')
    Task.objects.create(title='B')

    # Poziv funkcije koja se testira
    response = client.get('/api/tasks/')

    # Provjera
    assert response.status_code == 200
    data = response.json()
    assert set(data.keys()) == {'count', 'next', 'previous', 'results'}
    assert data['count'] == 2
    assert len(data['results']) == 2


@pytest.mark.django_db
def test_create_task(client):
    # Poziv funkcije koja se testira
    response = client.post('/api/tasks/', {'title': 'C', 'priority': 'High'}, format='json')

    # Provjera
    assert response.status_code == 201
    assert Task.objects.count() == 1
    task = Task.objects.get()
    assert task.title == 'C'
    assert task.priority == 'High'
    assert task.done is False
```

Sve funkcije čije ime započinje prefiksom `test_` u svim datotekama čije ime započinje prefiksom `test_` (npr. `test_tasks.py`) bit će prepoznate kao testovi i izvršene kad pokrenemo `pytest` alat (`pytest` je biblioteka koja dolazi s alatom, tj. skriptom, koja se također zove `pytest`). 
U složenijim projektima korisna je konvencija da se datoteke s testovima nalaze odmah pokraj datoteka koje sadrže funkcije koje se testiraju, npr. `api/views.py` i `api/test_views.py`, ili `api/a/b/c.py` i `api/a/b/test_c.py`. 

Pokretanje testiranja:
```bash
poetry run pytest
poetry run pytest -v  # Ispis rezultata za svaki test odvojeno
poetry run pytest api/test_tasks.py::test_create_task  # Samo konkretan test
```

### E2E testovi

E2E testovi koriste punokrvni preglednik i simuliraju korisničku interakciju. 

U našoj aplikaciji koristimo OIDC i Entra ID za autentikaciju, što nije jednostavno automatizirati, pa ćemo implementirati poseban testni način rada autentikacije koji zaobilazi uobičajenu autentikaciju. Taj način rada koristit ćemo samo prilikom pokretanja E2E testova.

Uvodimo jednu *environment* varijablu, `VITE_E2E_TEST_MODE`. Podsjetimo da Vite varijable s prefiksom `VITE_` automatski čini dostupnima frontendu (`import.meta.env.VITE_E2E_TEST_MODE`), a u backendu ćemo ručno pročitati tu varijablu koristeći `os.environ` (isti pristup koji smo koristili za varijable `VITE_CLIENT_ID` i `VITE_TENANT_ID`). Kad je varijabla `VITE_E2E_TEST_MODE` postavljena na `"true"` vrijedit će sljedeće:
- Frontend funkcija `login()` postavit će "račun" objekt s popunjenim osnovnim podacima, ali nećemo koristiti MSAL.
- Backend će primiti token, ali neće pročitati taj token već će uvijek vraćati isti `User` objekt.


#### Implementacija testnog načina rada autentikacije

Prvo u frontendu u datoteci `src/auth/useAuth.ts` radimo sljedeće promjene:

```ts
// src/auth/useAuth.ts
const E2ETestMode = import.meta.env.VITE_E2E_TEST_MODE === 'true'
    
...


export async function initAuth() {
  if (E2ETestMode) {
    return
  }
  // (Postojeći kod dolazi ovdje)
}

function login() {
  if (E2ETestMode) {
    accounts.value = [{
      username: 'user@example.com',
      name: 'E2E User',
    } as AccountInfo]
    return
  }
  // Postojeći kod:
  msalInstance!.loginRedirect({ scopes: ['openid', 'profile', 'email'] })
}

function logout() {
  if (E2ETestMode) {
    accounts.value = []
    return
  }
  // Postojeći kod:
  msalInstance!.logoutRedirect()
}

async function getApiToken(): Promise<string> {
  if (E2ETestMode) {
    return ""
  }
  // (Postojeći kod dolazi ovdje)
}
```

Zatim, u backendu u datoteci `config/settings.py` dodajemo čitanje nove environment varijable:

```python
# config/settings.py
...

E2E_TEST_MODE = os.environ.get("VITE_E2E_TEST_MODE", "").lower() == "true"
```

Konačno, u autentikacijskom sloju `api/authentication.py` mijenjamo `authenticate` (funkcija koja se poziva za svaki HTTP zahtjev i definira vrijednost za `request.user` varijablu).

```python
# api/authentication.py
class AzureADAuthentication(BaseAuthentication):
    def authenticate(self, request):
        if settings.E2E_TEST_MODE:
            auth_header = request.META.get("HTTP_AUTHORIZATION", "")
            if not auth_header.startswith("Bearer "):
                # Ako nema autorizacijskog zaglavlja, E2E test još nije kliknuo na gumb za prijavu, pa zasad ne vraćamo korisnika
                return None

            # Arbitrarne vrijednosti za subject (kojeg koristimo i kao username i email) i display_name
            subject = "user@example.com"
            display_name = "E2E User"
            user, _ = User.objects.get_or_create(
                username=subject,
                defaults={"email": subject, "first_name": display_name},
            )
            return (user, {"sub": subject, "name": display_name})

        # (Postojeći kod dolazi ovdje)
```

#### Postavljanje E2E testova

```bash
# Osigurajte da ste u korijenu projekta (E2E testovi bit će pokraj frontend i backend direktorija).
mkdir e2e_tests
cd e2e_tests
npm init playwright@latest
```

Na pitanje gdje želimo instalirati testove možete odgovoriti proizvoljno; u ovom poglavlju pretpostavljamo da ste napisali `tests`.
Za GitHub Actions workflow pitanje možete odgovoriti potvrdno.

Nakon instalacije pokrenite `npx playwright install --with-deps`.
Možda ćete trebati instalirati i dodatne pakete na operativnom sustavu, popis biste trebali vidjeti pokretanjem npr. `npx playwright test` 

Konfigurirat ćemo Playwright da sam pokrene i backend i frontend servere prije testova (slično kao naša `start_local.sh` skripta). 

```ts
// e2e_tests/playwright.config.ts

// Iznad `export default defineConfig` dodajte:
const environment_variables = { 
    VITE_E2E_TEST_MODE: 'true' 
}

export default defineConfig({
  ...
        
  // Pronađite gdje se spominje baseUrl, umjesto te linije napišite:
  baseURL:  'http://localhost:5173',  
    
  ...
    
  // Na kraj konfiguracije dodajte:
  webServer: [
    {
      command: '.venv/bin/python manage.py runserver 8000',
      cwd: '../backend',
      url: 'http://localhost:8000/api/tasks/',
      reuseExistingServer: !process.env.CI,
      timeout: 120 * 1000,
      env: environment_variables,
    },
    {
      command: 'npm run dev',
      cwd: '../frontend',
      url: 'http://localhost:5173',
      reuseExistingServer: !process.env.CI,
      timeout: 120 * 1000,
      env: environment_variables,
    },
  ]
})
```


#### Testovi


```ts
// e2e_tests/tests/auth.spec.ts
import { test, expect } from '@playwright/test'

test('user can log in', async ({ page }) => {
  await page.goto('/')

  // Prije prijave očekujemo da aplikacija prikazuje Log in gumb.
  await expect(page.getByText('Log in to view tasks.')).toBeVisible()
    
  // Prijava
  await page.getByRole('button', { name: 'Log in' }).click()

  // Nakon prijave gumb bi trebao postati Log out.
  await expect(page.getByRole('button', { name: 'Log out' })).toBeVisible()
})
```

```ts
// e2e_tests/tests/tasks.spec.ts
import { test, expect } from '@playwright/test'

test('create a task', async ({ page }) => {
  await page.goto('/')
  await page.getByRole('button', { name: 'Log in' }).click()

  // Generiramo jedinstven naslov da kasnije kad provjeravamo postojanje zadatka budemo sigurni da se radi o istom zadatku
  const title = `Zadatak A ${Date.now()}`

  await page.getByLabel('New task').fill(title)
  await page.getByRole('button', { name: 'Add', exact: true }).click()

  // Provjera je li novi zadatak u listi
  await expect(page.getByText(title)).toBeVisible()
    
  // Je li zadatak u listi i nakon refresha?
  await page.reload()
  await expect(page.getByText(title)).toBeVisible() 
})
```

Lokalno pokretanje E2E testova (pozicionirajte se u `e2e_tests` direktorij):

```bash
npx playwright test
```

Ako niste obrisali primjere testova koje je stvorio Playwright, oni će također biti pokrenuti.

Ako želite vizualno pratiti kako se testovi izvršavaju u pregledniku, pokrenite:

```bash
npx playwright test --ui
```

Prije nego se testovi pokrenu, Playwright će testirati jesu li frontend i backend serveri pokrenuti i stoga možete u terminalu vidjeti `[WebServer] Forbidden: /api/tasks/`.
To nije problem, radi se samo o Playwrightovom testiranju je li server pokrenut.
Kako naš backend nema javnih ruta, koristimo `http://localhost:8000/api/tasks/` kao testni URL u konfiguraciji.
No taj endpoint zahtijeva autentikaciju, pa će vratiti grešku koju Playwright ispisuje (kao informaciju).

### Pokretanje testova kao dio CI/CD pipelinea

Želimo da se testovi pokreću automatski pri svakoj promjeni na grani `main` (prije postavljanja na server) i kad se stvori novi *pull request*.

Ako radite na kopiji projekta gdje već imate GitHub workflow datoteku `deploy.yml`, mogli bismo promijeniti tu datoteku i u nju umetnuti poziv testova.
No kako je Playwright već za nas stvorio `.github/workflows/playwright.yml`, koristit ćemo tu datoteku.
To je i bolja opcija, jer želimo pokrenuti testove i kod stvaranja *pull requesta*, ne samo kod promjene na grani `main`.

Playwright je stvorio datoteku `e2e_tests/.github/workflows/playwright.yml`, no nama ona treba izvan `e2e_tests` direktorija, direktno u korijenu projekta, pa ju premjestite na tu adresu (`.github/workflows/playwright.yml`). 

```yaml
# .github/workflows/playwright.yml
name: Tests

on:
  push:
    branches: [main]
  pull_request:

jobs:
  backend:
    name: Unit tests
    runs-on: ubuntu-latest
    defaults:
      run:
        working-directory: ./backend
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with:
          python-version: '3.12'
      - name: Install Poetry
        run: pipx install poetry
      - name: Install dependencies
        run: poetry install --no-root
      - name: Run pytest
        run: poetry run pytest

  e2e:
    name: E2E tests (Playwright)
    runs-on: ubuntu-latest
    env:
      VITE_E2E_TEST_MODE: 'true'
      CI: 'true'
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v5
        with:
          python-version: '3.12'
      - uses: actions/setup-node@v4
        with:
          node-version: 25
          cache: npm
          cache-dependency-path: frontend/package-lock.json
      - name: Install Poetry
        working-directory: ./backend
        run: pipx install poetry && poetry config --local virtualenvs.in-project true && poetry install --no-root && poetry run python manage.py migrate --noinput
      - name: Install frontend
        working-directory: ./frontend
        run: npm ci
      - name: Install Playwright
        working-directory: ./e2e_tests
        run: npm ci && npx playwright install --with-deps
      - name: Run E2E tests
        working-directory: ./e2e_tests
        run: npx playwright test
      - name: Upload error screenshots
        if: ${{ !cancelled() }}
        uses: actions/upload-artifact@v4
        with:
          name: E2E Error Screenshots
          path: e2e_tests/playwright-report/
          retention-days: 7
```

U slučaju da je bilo grešaka tijekom pokretanja testova, screenshotove grešaka možete pronaći na GitHubu unutar, klikom na korak `Upload error screenshots` i potom na link koji piše u posljednjoj prikazanoj poruci.

Ako u projektu imate datoteku `deploy.yml`, možemo dodati uvjet da je postavljanje na server dopušteno samo ako su testovi bili uspješni. 
Trebat će vam sljedeći blok kao prvi element `jobs` liste:
```yaml 
tests:
  uses: ./.github/workflows/playwright.yml
```
Postojeći prvi element liste (npr. `deploy`) treba imati dodan parametar `needs: tests`.
Konačno, u datoteci `playwright.yml` umjesto `push: branches: [main]` treba napisati `workflow_call:`. 

U nastavku je primjer minimalne `deploy.yml` datoteke (bez samog koda za postavljanje nove verzije aplikacije na server) koja ovisi o testovima:

```yaml
name: Deploy

on:
  push:
    branches: [main]

jobs:
  tests:
    uses: ./.github/workflows/playwright.yml
  deploy:
    needs: tests
    name: Deploy
    runs-on: ubuntu-latest
    steps:
      - run: echo "Tests passed, deploying ..."
```
 
Kao i u ranijem slučaju, u datoteci `playwright.yml` umjesto `push: branches: [main]` treba napisati `workflow_call:`. 
Testovi će se pozivati prilikom izrade *pull requesta*.
Kod svake promjene na grani `main` prvo će se pokrenuti testovi, a potom će se izvršiti ostatak `deploy.yml` datoteke.