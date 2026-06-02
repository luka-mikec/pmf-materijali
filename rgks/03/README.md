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
