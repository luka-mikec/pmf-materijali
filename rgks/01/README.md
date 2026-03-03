# Pregled biblioteke Vue

Cilj je upoznati se s osnovama Vue.js frameworka. Nakon što prođemo osnovne koncepte, izradit ćemo aplikaciju za upravljanje zadacima (To-do lista). Aplikaciju ćemo najprije izraditi koristeći čisti HTML i CSS, a zatim refaktorirati koristeći Vuetify biblioteku komponenti. 

## Potrebni alati

- Računalo koje ćete koristiti za rad na kolegiju. Svi materijali pretpostavljaju da radite na Linuxu. Za Mac će postupci biti uglavnom isti ili slični. Za Windowse, postavite WSL (Windows Subsystem for Linux) i sve alate (NVM/Node) instalirajte unutar WSL-a. Također, neka je vaš kod na 'disku' WSL-a. I dalje možete koristiti svoj IDE (JetBrains, VS Code, ...).
- [Node.js](https://nodejs.org/) verzije 24 ili noviji. Preporuka: umjesto da instalirate Node.js direktno, instalirajte [NVM](https://github.com/nvm-sh/nvm) kako biste mogli koristiti više Node verzija na istom računalu. Kroz NVM možete instalirati Node 25:

```bash
nvm install 25
nvm use 25
```

Provjerite instalaciju:

```bash
node --version   # trebao bi ispisati v24.x.x
npm --version    # npm dolazi s Node.js instalacijom
```

## Stvaranje projekta

Za stvaranje frontend projekta koristimo Vite.
On ima dvije ključne uloge:
1. Sadrži tzv. development server. Njegovim pokretanjem frontend će biti dostupan na URL-u `http://localhost:5173`. Kad napravimo promjenu u izvornom kodu, development server to detektira i automatski osvježava stranicu (*hot reload*).
2. Sadrži build alat koji iz izvornog koda generira statičke resurse (HTML, CSS, JavaScript) optimizirane veličine.

Pokrenite sljedeće kako biste stvorili novi Vue.js TypeScript projekt:

```bash
npm init vite@latest to-do-lista -- --template vue-ts
```

Na pitanja možete odgovoriti sa zadanim odgovorima. Projekt će se odmah i pokrenuti, zaustaviti ga možete s `Ctrl + C`.

Development server možete ponovno pokrenuti kasnije:

```bash
npm run dev
```

Otvorite [http://localhost:5173](http://localhost:5173) u pregledniku. Trebali biste vidjeti hello-world Vite/Vue projekt.

### TypeScript

TypeScript je 'konzervativna' nadogradnja JavaScripta: sav JavaScript je i validan TypeScript. Jedina razlika je u tome što TypeScript radi statičku provjere tipova, npr. upozorit će nas ako koristimo `Boolean` na mjestu gdje se očekuje `String`. Neke biblioteke, uključno s Vueom, imaju dublju 'integraciju' s TypeScriptom, tj. TypeScript sintaksa koju pišemo imat će neke efekte na ponašanje aplikacije, ne samo na kompilaciju (primjerice, Vue funkcije `defineProps` i `defineEmits` koje ćemo vidjeti kasnije).

Provjeru tipova u projektu možemo pokrenuti s:

```bash
npx vue-tsc -b
```

Na ovom kolegiju materijali će biti pisani u TypeScriptu, ali sasvim je u redu ako ne koristite TypeScript i pišete JavaScript. Ako koristite TypeScript, ali želite ignorirati greške tipova na određenom dijelu koda, možete koristiti `// @ts-ignore` i `// @ts-nocheck` direktive, ili pokrenuti kompilaciju bez provjere tipova.

```ts
// @ts-ignore
const y: number = "hello"
```

Alternativno, na vrhu datoteke možete dodati `// @ts-nocheck` da isključite provjere za cijelu datoteku.

Konačno, ako želite kompilirati projekt bez obzira na probleme oko tipova, možete pokrenuti sljedeće:
```bash
npx vite build
```

Uobičajenu kompilaciju projekta radimo s `npm run build`, što je kratica za `npx vue-tsc -b && npx vite build`, tj. prvo se provjeravaju tipovi, a tek onda gradi projekt.

Kratak pregled TypeScript sintakse:
* **Osnovni tipovi:**
    `let tekst: string = "Bok"; let broj: number = 42; let aktivan: boolean = true;`
* **Funkcije i lambda funkcije:**
    `function zbroj(a: number, b?: number): number { return a + (b || 0); }`
    `const ispis = (poruka: string): void => console.log(poruka);`
* **Liste:**
    `let brojevi: number[] = [1, 2, 3];`
* **Objekti (s poznatom shemom):**
    `type Korisnik = { ime: string; opcionalanBroj?: number; };`
* **Objekti (mape)**
    `let rjecnik: Record<string, number> = { "kljuc1": 2 };`
* **N-torke (*tuples*):**
    `let uredeniPar: [number, string] = [1, "Ivan"];`
* **Unije tipova:** `let brojIliTekstIliNull: number | string | null = "ABC";`
* **Literali:** `let status: "success" | "error";`
* **Any:** `let nesto: any = nekaFunkcijaSKompliciranimIzlazom();`
* **Cast:** `let brojIliTekstIliNull: number | string | null = Math.random() < 0.5 ? "tekst" : 123;
let duljina = (brojIliTekstIliNull as string).length;`

### Struktura projekta

Datoteka `index.html` sadrži `<div id="app"></div>` element u koji će Vue postaviti aplikaciju. Datoteka `src/main.ts` je ulazna točka koja stvara i postavlja glavnu Vue komponentu (naša se zove App). Komponenta App definirana je u `src/App.vue`.

```ts
import { createApp } from 'vue'
import App from './App.vue'

createApp(App).mount('#app')
```

Funkcija `createApp()` stvara novu Vue aplikaciju. Prima korijensku komponentu (`App`) kao argument. Metoda `.mount('#app')` postavlja spomenutu komponentu u DOM element čiji je`id` jednak `"app"` (taj je element definiran u `index.html`). 

## Sintaksa Vue (.vue) datoteka

Vue datoteke (tzv. *Single-File Components*)  sastoje se od tri opcionalna dijela:
1. `<script setup lang="ts">`: inicijalizacija komponente i sav TypeScript (ili JavaScript) kod.
2. `<template>`: HTML predložak s proširenom Vue sintaksom koji definira strukturu korisničkog sučelja.
3. `<style>`: CSS stilovi koji se primjenjuju na tu komponentu.

Zamijenite sadržaj `src/App.vue` sljedećim kodom:

```vue
<script setup lang="ts">
// Ovo nije doslovno učitavanje slike, 
// već način na koji referiramo na statičke resurse poput slika. 
// Evaluirati će se u string s relativnom putanjom do slike.
import vueLogo from './assets/vue.svg' 
const appName = 'To-do lista'
const rawHtml = '<em>Vue.js</em> aplikacija'
</script>

<template>
  <h1>{{ appName }}</h1>
  <p>Ovo je <span v-html="rawHtml"></span>.</p>
  <img v-bind:src="vueLogo" width="50" />
  <img :src="vueLogo" width="50" />
</template>
```

Dvostruke vitičaste zagrade `{{ }}` (tzv. mustache interpolacija) ispisuju vrijednost JavaScript izraza kao tekst. Vue će osigurati da se sadržaj prikazuje kao tekst, čak i ako sadrži HTML kod.

Direktiva `v-html` može se koristiti za evaluaciju HTML-a. Trebalo bi ju izbjegavati, jer ako nismo potpuno sigurni da je string koji šaljemo `v-html` direktivi siguran, može se dogoditi da u korisničkom pregledniku evaluiramo maliciozan kod.

Direktiva `v-bind` (ili skraćeno `:`) dinamički postavlja HTML atribute. U primjeru `:src="vueLogo"` postavlja `src` atribut slike na vrijednost varijable `imageUrl`.

## Reaktivnost: `ref()`

Varijable deklarirane u `<script setup>` u prethodnom primjeru nisu 'reaktivne'. Tj., ako im promijenimo vrijednost, Vue neće ponovno iscrtati predložak. Za reaktivnost koristimo `ref()`:

```vue
<script setup lang="ts">
import { ref } from 'vue'

const count = ref(0)

function increment() {
  ++count.value
}
</script>

<template>
  <p>Brojač: {{ count }}</p>
  <button @click="increment">+1</button>
</template>
```

Funkcija `ref()` vraća objekt s `.value` svojstvom. U `<script setup>` bloku pristupamo vrijednosti kroz `.value`, ali u predlošku Vue automatski "odmotava" referencu i možemo (tj. moramo) pisati `{{ count }}` umjesto `{{ count.value }}`.

Kad se `.value` promijeni, Vue automatski ponovno iscrtava sve dijelove predloška koji su promijenjeni. Zbog ovoga se kaže da je Vue 'state-driven', 'data-driven' ili 'deklarativan': ne opisujemo kako se DOM mijenja (koje HTML elemente treba dodati/promijeniti/obrisati), već definiramo stanje (*state*), a Vue se brine o tome da DOM uvijek odražava trenutno stanje.

Ne trebaju sve varijable u Vue komponenti biti reaktivne (reference), već samo one čija promjena izravno ili neizravno utječe na sučelje.

## Izvedeno stanje (`computed`)

Zamijenite `App.vue`:

```vue
<script setup lang="ts">
import { ref, computed } from 'vue'

interface Task {
  id: number
  title: string
  done: boolean
}

const tasks = ref<Task[]>([
  { id: 1, title: 'Naučiti Vue', done: true },
  { id: 2, title: 'Izraditi Vue projekt', done: false },
  { id: 3, title: 'Kompilirati Vue projekt', done: false },
])

const totalTasks = computed(() => tasks.value.length)
const doneTasks = computed(() => tasks.value.filter(t => t.done).length)
const pendingTasks = computed(() => totalTasks.value - doneTasks.value)
</script>

<template>
  <h1>To-do lista</h1>
  <p>Ukupno: {{ totalTasks }} | Završeno: {{ doneTasks }} | Preostalo: {{ pendingTasks }}</p>
</template>
```

`computed()` stvara izvedeno stanje, vrijednost koja se automatski izračunava na temelju referenci i drugih izvedenih stanja. Vue prati ovisnosti (ovo je praćenje, možda iznenađujuće, moguće postići bez ikakve statičke analize koda) i ponovno izračunava vrijednosti po potrebi. 

Još jedan primjer koji pokazuje da Vue ponovno izračunava izvedeno stanje samo kad je to nužno.

```vue
<script setup lang="ts">
import { ref, computed } from 'vue'

const a = ref(1)
const b = ref(2)
const useA = ref(true)

const result = computed(() => {
  console.log('computed se izvršava')
  return useA.value ? a.value : b.value
})
</script>

<template>
  <p>result = {{ result }}</p>
  <button @click="++a">++a</button>
  <button @click="++b">++b</button>
  <button @click="useA = !useA">toggle a/b</button>
</template>
```

Kad je `useA` istinit, `computed` ovisi samo o `a` i `useA`, pa promjena `b` neće pokrenuti ponovni izračun. Kad `useA` postavimo na `false`, `computed` sad ovisi o `b` i `useA`, a promjena `a` više nema efekta.

## Dinamički stilovi

Vue omogućuje dinamičko postavljanje CSS klasa i stilova.

```vue
<script setup lang="ts">
import { ref } from 'vue'

const isActive = ref(true)
const hasError = ref(false)
const fontSize = ref(16)
</script>

<template>
  <!-- Objekt-sintaksa za CSS klase: klasa se dodaje ako je uvjet ispunjen -->
  <div :class="{ active: isActive, 'text-error': hasError }">
    Primjer teksta (ovisi o active, error)
  </div>

  <!-- Lista-sintaksa za CSS klase: uvijek prisutne klase i dodane dinamičke -->
  <div :class="['underline', { active: isActive }]">
    Primjer s poljem (ovisi o underline, active)
  </div>

  <!-- CSS stil -->
  <p :style="{ fontSize: fontSize + 'px', color: hasError ? 'red' : 'black' }">
    Dinamički stil (ovisi o font size, error)
  </p>

  <!-- CSS stil kao običan string -->
  <p :style="'font-size: ' + fontSize + 'px; color: ' + (hasError ? 'red' : 'black')">
    Dinamički stil kao string (ovisi o font size, error)
  </p>

  <button @click="isActive = !isActive">Toggle Active</button>
  <button @click="hasError = !hasError">Toggle Error</button>
  <button @click="fontSize += 2">Increase Font Size</button>
</template>

<style>
.active { font-weight: bold; color: green; }
.text-error { color: red; }
.underline { text-decoration: underline; }
</style>

```

`:class` prima objekt (ključ je ime klase, vrijednost je uvjet) ili polje. `:style` prima objekt s CSS svojstvima u camelCase obliku ili uobičajen CSS string.


## Uvjetno prikazivanje

`v-if`, `v-else-if` i `v-else` kontroliraju hoće li se element stvoriti u DOM-u. Ako želimo da je element prisutan, ali možda skriven, koristimo `v-show`.

```vue
<script setup lang="ts">
import { ref } from 'vue'

const status = ref<'loading' | 'empty' | 'ready'>('ready')
const showDetails = ref(false)
</script>

<template>
  <div v-if="status === 'loading'">Učitavanje...</div>
  <div v-else-if="status === 'empty'">Nema zadataka.</div>
  <div v-else>Zadaci su učitani.</div>

  <!-- v-show: element je uvijek u DOM-u, samo mu se mijenja vidljivost -->
  <p v-show="showDetails">Ovo su detalji koji se mogu sakriti.</p>

  <button @click="status = 'loading'">Postavi na Loading</button>
  <button @click="status = 'empty'">Postavi na Empty</button>
  <button @click="status = 'ready'">Postavi na Ready</button>
  <button @click="showDetails = !showDetails">
    {{ showDetails ? 'Sakrij' : 'Prikaži' }} detalje
  </button>
</template>
```

Koristite `v-if` kad se uvjet rijetko mijenja (izbjegava stvaranje nepotrebnih elemenata). Koristite `v-show` kad se uvjet često mijenja (izbjegava sporo ponavljano stvaranje i uklanjanje elemenata).

## Prikazivanje lista

`v-for` iterira po listi i prikazuje po jedan HTML element ili Vue komponentu za svaki element kolekcije.

```vue
<script setup lang="ts">
import { ref } from 'vue'

interface Task {
  id: number
  title: string
  done: boolean
}

const tasks = ref<Task[]>([
  { id: 1, title: 'Naučiti Vue', done: true },
  { id: 2, title: 'Izraditi Vue projekt', done: false },
  { id: 3, title: 'Položiti ispit', done: false },
])

function removeTask(id: number) {
  tasks.value = tasks.value.filter(t => t.id !== id)
}
</script>

<template>
  <h1>To-do lista</h1>
  <ul>
    <li v-for="task in tasks" :key="task.id">
      {{ task.title }}
      <button @click="removeTask(task.id)">Obriši</button>
    </li>
  </ul>
  <p v-if="tasks.length === 0">Nema zadataka.</p>
</template>
```

Atribut `:key` je obavezan i treba ga postaviti na vrijednost koja je jedinstvena (unutar liste). Vue ga koristi za praćenje identiteta elemenata prilikom mijenjanja liste. Npr. kad se u listi od tri elemenata obriše drugi element, Vue će znati da novu listu čine (bivši) prvi i treći element, a ne da je drugi element zamijenjen s trećim. U našem primjeru to nije problem, ali da ovi elementi imaju neko stanje (npr. `input` element) i da nismo odabrali dobar `key` (npr. da smo koristili indeks unutar polja), to bi se stanje pogrešno prenijelo iz obrisanog elementa u bivši treći element:
```vue
<script setup lang="ts">
import { ref } from 'vue'

const elements = ref(["a", "b", "c"]);

function removeElement(index: number) {
  elements.value = elements.value.filter((_, elementIndex) => elementIndex !== index);
}
</script>

<template>
  <h1>To-do lista</h1>
  <ul>
    <li v-for="(element, elementIndex) in elements" :key="elementIndex">
      {{ element }}
      <input />
      <button @click="removeElement(elementIndex)">Obriši</button>
    </li>
  </ul>
  <p v-if="elements.length === 0">Nema zadataka.</p>
</template>
```

Sintaksa `(element, elementIndex) in elements` u svakoj iteraciji vraća i trenutni element i njegov indeks. 
Ako u ovom primjeru unesete redom vrijednosti `x`, `y`, `z` u input polja, a zatim obrišete srednji element, očekivali biste da ostanu `x` i `z`, ali zbog korištenja indeksa kao ključa, Vue će pogrešno prenijeti stanje iz obrisanog elementa u preostali element, pa će ostati `x` i `y`.

## Događaji

Direktiva `v-on` (skraćeno `@`) definira koju funkciju ('event handler') pozvati kad se dogodi neki događaj ('event'). Npr. što napraviti kad korisnik klikne na gumb, ili kad pokuša poslati formu (`submit` event, obično se aktivira kad korisnik pritisne Enter unutar polja u formi).

```vue
<script setup lang="ts">
import { ref } from 'vue'

const count = ref(0)

function handleClick(event: MouseEvent) {
  ++count.value
  alert(`Kliknuto na koordinatama: ${event.clientX}, ${event.clientY}`);
}

const handleSubmit = () => {
  alert('Forma poslana');
};

</script>

<template>
  <!-- Inline funkcija -->
  <button @click="count++">+1 (inline funkcija)</button>

  <!-- Funkcija definirana izvan templatea -->
  <button @click="handleClick">+1 (obična funkcija)</button>

  <!-- Modifikatori događaja: .prevent zaustavlja zadano ponašanje za neke događaje,
       npr. preglednikovu redirekciju nakon submitanja forme -->
  <form @submit.prevent="handleSubmit">
    <button type="submit">Pošalji</button>
  </form>

  <!-- .once onemogućuje višestruku aktivaciju istog događaja -->
  <button @click.stop.once="count++">Samo jednom</button>

  <!-- Tipke -->
  <input @keyup.enter="console.log('Enter pritisnut')" />

  <p>Brojač: {{ count }}</p>
</template>
```

Modifikatori su sufiksi koji modificiraju ponašanje direktive. Npr., `.prevent` poziva `event.preventDefault()`, `.stop` poziva `event.stopPropagation()`.

## Direktiva `v-model`

`v-model` stvara dvosmjerno vezanje (*two-way binding*) između stanja i komponente: ne samo da komponenti šaljemo neko stanje, već komponenta može mijenjati to stanje. Neke komponente ne podržavaju `v-model`. Više o `v-model` direktivi vidjet ćemo nešto kasnije, zasad nas zanima samo kako izgleda korištenje komponente koja podržava `v-model`.

```vue
<script setup lang="ts">
import { ref } from 'vue'

const newTaskTitle = ref('')
const priority = ref('medium')
const isUrgent = ref(false)
const notes = ref('')

const tasks = ref<{ id: number; title: string }[]>([])
let nextId = 1

function addTask() {
  if (!newTaskTitle.value.trim()) return
  tasks.value.push({ id: nextId++, title: newTaskTitle.value.trim() })
  newTaskTitle.value = ''
}
</script>

<template>
  <h1>To-do lista</h1>

  <form @submit.prevent="addTask">
    <!-- .trim automatski uklanja razmake s početka i kraja -->
    <input v-model.trim="newTaskTitle" placeholder="Novi zadatak..." />
    <button type="submit">Dodaj</button>
  </form>

  <ul>
    <li v-for="task in tasks" :key="task.id">{{ task.title }}</li>
  </ul>

  <hr />

  Primjeri različitih inputa i njihove dvosmjerne veze:

  <hr />

  <select v-model="priority">
    <option value="low">Niski</option>
    <option value="medium">Srednji</option>
    <option value="high">Visoki</option>
  </select>

  v-model: {{ priority }}

  <hr />

  <label>
    <input type="checkbox" v-model="isUrgent" /> Hitno
  </label>

  v-model: {{ isUrgent }}

  <hr />

  <!-- Textarea s .lazy: sinkronizira tek na 'change' (blur), ne na svaku promjenu (zbog performansi) -->
  <textarea v-model.lazy="notes" placeholder="Bilješke..."></textarea>

  v-model: <pre>{{ notes }}</pre>

</template>
```

Neki modifikatori za `v-model`: `.lazy` sinkronizira na `change` događaj umjesto `input` događaj, `.number` pretvara unos u broj, `.trim` uklanja razmake s početka i kraja.

## `watch` i `watchEffect`

Izvedena stanja (`computed`) prate promjene reaktivnih vrijednosti i vraćaju nova stanja. No, ponekad nam ne treba novo stanje, već izvršavanje akcije u ovisnosti o promjenama reaktivnog stanja. Primjerice, imamo aplikaciju čije stanje želimo prikazati (`template`) za što nam je dovoljno `ref` i `computed`, ali isto stanje želimo spremiti u `localStorage` kad god se promijeni. U tom slučaju možemo koristiti `watch()` i `watchEffect()`, koji prate promjene reaktivnih vrijednosti.

```vue
<script setup lang="ts">
import { ref, watch, watchEffect } from 'vue'

const initialValue = JSON.parse(localStorage.getItem('value') ?? "null")
const value = ref(initialValue ?? 'Sinkronizirano s Local Storageom')

// watch: prati jednu ili više specifičnih reaktivnih vrijednosti
watch(value, (newValue) => {
  localStorage.setItem('value', JSON.stringify(newValue))
}, { deep: true })


// watchEffect: automatski prati sve reaktivne vrijednosti koje se koriste unutar funkcije
watchEffect(() => {
  // Isti efekt kao ranije, ali ne moramo sami pratiti koje su ovisnosti
  // localStorage.setItem('value', JSON.stringify(value.value))
})
</script>

<template>
  <div class="app">
    <textarea
        v-model="value"
        rows="50"
        :cols="80"
    />
  </div>
</template>
```

`watch()` prima reaktivno stanje (npr. `ref`) i *callback* funkciju koja se poziva kad se vrijednost promijeni. Opcija `deep: true` omogućuje praćenje promjena na svim razinama unutar objekata (npr. na objektu `{"a": {"b": "x"}}` praćenje promjene `a.b`) ili polja (promjene svojstava unutar polja). Osim `deep: true`, često je korisna i opcija `immediate: true`. Njeno uključivanje znači da se funkcija poziva pri budućim promjenama, ali i na početku (tijekom inicijalizacije komponente).

`watchEffect()` automatski prati sve reaktivne vrijednosti korištene unutar funkcije. Ne prima eksplicitni izvor, već Vue sam detektira ovisnosti pri prvom pokretanju. Smisao eksplicitnih vrijednosti kod `watch` funkcije su situacije u kojima želimo čitati npr. reaktivne vrijednosti `a` i `b`, ali samo promjene vrijednosti `a` trebaju pokrenuti funkciju, dok promjene vrijednosti `b` ne trebaju. U tom slučaju koristimo `watch(a, callback)` umjesto `watchEffect()`, jer bi `watchEffect()` pokrenuo funkciju i kad se dogodi promjena u `b`.

**Napomena**: najčešće je bolje pisati kod koji ne ovisi o `watch` funkciji. Problem s `watch` konstrukcijama jest što njihovi pozivi nisu eksplicitno prisutni u kodu, pa je debuggiranje znatno teže. Stoga ako želimo uvijek izvršiti neku radnju `f()` kad se promijeni stanje `st`, obično je bolje za mijenjanje stanja `st` napraviti funkciju poput `function setSt(newValue) { st.value = newValue; f(); }` i koristiti `setSt()` umjesto direktnog mijenjanja `st.value`. Sada smo postigli isti efekt, ali kod je lakše debuggirati.

## Referenca na elemente

Osim što se reference mogu koristiti za uobičajene JavaScript vrijednosti, mogu se koristiti i za HTML elemente koji su dio našeg sučelja. Kako Vue sam stvara i upravlja DOM elementima, postupak za inicijalizaciju ovakvih referenci izgleda ovako:
1. Definiramo referencu u `<script setup>` bloku, npr. `const node = ref<HTMLEment | null>(null)`. Tip će uvijek biti oblika `html-element | null`, jer Vue ne crta sučelje prije nego izvrši čitav `<script setup>` blok, pa je `null` jedina moguća početna vrijednost.
2. Vue iscrtava sučelje. Negdje u predlošku trebamo koristiti atribut `ref="node"` na elementu čiju referencu želimo. Ova posebna sintaksa znači da Vue treba postaviti `node.value` na taj element nakon što ga stvori.
3. Vrijednost `node.value` definirana je jednom kad je cijelo sučelje iscrtano.

```vue
<script setup lang="ts">
import { ref, nextTick } from 'vue'

const tasks = ref<{ id: number; title: string }[]>([
  { id: 1, title: 'A' },
  { id: 2, title: 'B' },
  { id: 3, title: 'C' },
])
const editingId = ref<number | null>(null)
const editText = ref('')
const editInput = ref<HTMLInputElement | null>(null)

async function startEditing(task: { id: number; title: string }) {
  editingId.value = task.id
  editText.value = task.title

  // Input se tek sada pojavljuje u DOM-u, moramo pričekati iscrtavanje
  await nextTick()
  // Pozivamo .focus() kako korisnik ne bi morao kliknuti na polje prije pisanja
  editInput.value?.focus()
}

function finishEditing() {
  const task = tasks.value.find(t => t.id === editingId.value)
  if (task) {
    task.title = editText.value.trim()
  }
  editingId.value = null
}
</script>

<template>
  Kliknite na stavku da biste započeli uređivanje:

  <ul>
    <li v-for="task in tasks" :key="task.id">
      <span @click="startEditing(task)">{{ task.title }}</span>
    </li>
  </ul>

  <div v-if="editingId !== null">
    <input
        ref="editInput"
        v-model="editText"
        @keyup.enter="finishEditing"
    />
    <button @click="finishEditing">Spremi</button>
  </div>
</template>
```

U gornjem primjeru koristimo `ref` kako bismo pozvali metodu `.focus()` na `input` elementu (to je DOM metoda, nije specifična za Vue). U ovom primjeru vidim i primjer korištenja `nextTick()`, koji vraća `Promise` koji se izvršava nakon što Vue završi s iscrtavanjem DOM-a. To znači da će se kod nakon `await nextTick()` izvršiti tek nakon što se `input` element pojavi u DOM-u, pa će `editInput.value` biti definiran i moći ćemo pozvati `.focus()`. Sintaksa `a?.b()` znači "ako je `a.b` definirana vrijednost, pozovi metodu `a.b()`, a inače ne radi ništa". U našem slučaju znamo da je `editInput.value?.focus` definirano, ali TypeScript ne zna da će `editInput.value` biti definiran nakon `nextTick()`, pa koristimo ovu sintaksu da izbjegnemo TypeScript grešku.

## Zadatak 1
Koristeći `table`, `tr` i `td` elemente, iscrtajte tablicu množenja. Broj redaka i stupaca neka se unosi putem `input` elemenata (na početku neka su oba broja 10), a pritiskom `enter` tablica se treba iscrtati koristeći trenutno uneseni broj redaka i stupaca. Uneseni brojevi trebaju ostati zapisani i nakon refresha stranice. Neka se ispod tablice prikazuje tekst oblika "Broj prostih brojeva manjih od {{ redaka * stupaca }} jest {{ brojProstihBrojeva }}" gdje je `brojProstihBrojeva` izvedeno stanje (`computed`). Konačno, neka cijela tablica koristi dinamički CSS stil: ovisno o broju redaka i stupaca postavlja zoom na način da je tablica uvijek cijela vidljiva na vašem ekranu (čak i ako zbog toga postane nečitljiva). Ako je broj redaka ili stupaca veći od 100, neka se umjesto tablice prikaže poruka "Previše redaka/stupaca".
Provjerite da izvorni kod nema TypeScript grešaka pokretanjem `npx vue-tsc -b`.
