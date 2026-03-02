# Razvoj grafičkih korisničkih sučelja

Ovdje su materijali s nastave iz kolegija Razvoj grafičkih korisničkih sučelja (RGKS).

## Cloud provideri

Za praćenje kolegija preporučljivo je korištenje cloud providera, primjerice Google (GCP), Amazon (AWS) ili Microsoft (Azure).
Kako u trenutku pisanja samo Azure nudi besplatne kredite za studente PMF-a ($100 koji vrijede 12 mjeseci), na nastavi ćemo koristiti Azure.

### Microsoft Azure

U slučaju da koristite Azure, postoje dvije opcije:
- Koristiti 'tenant' fakulteta. 
- Koristiti vlastiti 'tenant'.

Tenant predstavlja organizaciju, npr. firmu. 
Tenant obično okuplja više korisnika, npr. sve zaposlenike firme, sve korisnike servisa koje ta firma nudi itd.
Osim korisnika, tenant sadrži i resurse, npr. virtualne mašine, baze podataka, web aplikacije itd. o čijoj se infrastrukturi (hardver i softver) brine cloud provider.
Između tenanta i resursa u Azureu postoji još jedna apstrakcija: tzv. pretplata (_subscription_).
Kad koristite besplatne studentske kredite, dobit ćete vlastitu pretplatu s kreditima unutar tenanta fakulteta.
Dakle, svi sudionici kolegija dijele isti tenant, ali svatko ima svoju zasebnu pretplatu.
To inače nije uobičajeno, jer se pretplate obično dijele između više korisnika (npr. sve bi interne aplikacije mogle biti u jednoj pretplati, svi servisi za krajnje korisnike u drugoj itd.), ali u ovom slučaju je tako zbog načina na koji Microsoft dodjeljuje studentske kredite.

U nastavku su upute za prvu opciju, odnosno korištenje 'tenanta' fakulteta.

1. Zatražite studentske kredite na https://azure.microsoft.com/en-us/free/students. 
Prilikom prijave, koristite username i password AAI računa (on je ujedno Microsoft račun).
2. Pričekajte potvrdu da su krediti odobreni. Na vaš studentski mail stići će obavijest koja spominje `Subscription ID: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx`.
3. Prijavite se na [Azure Portal](https://portal.azure.com), ponovno koristeći AAI račun.
4. U gornjem desnom kutu kliknite na ikonu korisnika, a zatim na "Switch directory". Provjerite da je odabran "Prirodoslovno-matematički fakultet".
5. U polju za pretragu na vrhu Azure Portala upišite "Subscriptions" i odaberite tu opciju. Provjerite da je odabrana pretplata s ID-om koji ste dobili u mailu.
6. Instalirajte [Azure CLI](https://learn.microsoft.com/en-us/cli/azure/install-azure-cli) i prijavite se koristeći `az login`. Provjerite da je odabrana ispravna pretplata pomoću `az account show`.

Sada možete stvarati resurse koristeći upravo instalirani Azure CLI ili [Azure Portal](https://portal.azure.com).
Pripazite na cijenu resursa koje stvarate kako ne biste ostali bez kredita prije kraja semestra.

## GitHub račun

Zatražite [Student Developer Pack](https://education.github.com/pack). 

## Potreban softver

Softver za računalo koje ćete koristiti za rad na kolegiju. Svi materijali pretpostavljaju da radite na Linuxu. Za Mac će postupci biti uglavnom isti ili slični. Za Windowse, postavite WSL (Windows Subsystem for Linux) i sve alate (Azure CLI, Docker, NVM/Node, pyenv/Python) instalirajte unutar WSL-a. Također, u tom slučaju neka je vaš kod na 'disku' WSL-a. I dalje možete koristiti svoj IDE (JetBrains, VS Code, ...).
- [Azure CLI](https://learn.microsoft.com/en-us/cli/azure/install-azure-cli) za postavljanje Azure resursa, opisan detaljnije iznad u ovoj datoteci.
- [Docker](https://docs.docker.com/get-docker/) kako biste mogli lokalno izgraditi Docker slike koje će se kopirati na Azure. Nije vam potreban Docker Desktop (komercijalan alat).
- [Docker Compose](https://docs.docker.com/compose/), opcionalno, ako želite na svojem računalu pokretati Docker slike. Ovisno o tome kako instalirate Docker, Docker Desktop trebao bi doći predinstaliran s Dockerom.
- [Node.js](https://nodejs.org/) verzije 24 ili noviji. Preporuka: umjesto da instalirate Node.js direktno, instalirajte [NVM](https://github.com/nvm-sh/nvm) kako biste mogli koristiti više Node verzija na istom računalu. Kroz NVM možete instali
rati Node 24 ili noviji (trenutno je 25 aktualan).
- [Python](https://www.python.org/) verzije 3.12 ili noviji. Preporuka: umjesto toga, instalirajte [pyenv](https://github.com/pyenv/pyenv) kako biste mogli koristiti više Python verzija na istom računalu. Kroz pyenv možete instalirati Python 3.1
2.

