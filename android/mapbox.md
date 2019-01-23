# Mapbox

1. Stvorite novi projekt, 4.0.3, praznu aktivnost.
2. File -> Project Structure -> Gradle version: 4.10.1
3. Unutar `manifest` taga `AndroidManifest.xml` datoteke dodajte 
	```xml
	<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
	<uses-permission android:name="android.permission.INTERNET" />
	```
4. U `build.gradle` (za **module**) unutar android {} grupe dodajte, npr. na kraj, sljedeće:
    ```xml
    compileOptions {
        sourceCompatibility JavaVersion.VERSION_1_8
        targetCompatibility JavaVersion.VERSION_1_8
    }
    ```
    unutar defaultConfig {} grupe dodajte, npr. na kraj, sljedeće:
    ```xml
    jackOptions {
        enabled true
    }
    ```
    unutar dependencies {} grupe dodajte, npr. na kraj, sljedeće:
     ```xml
    compile 'com.mapbox.mapboxsdk:mapbox-android-sdk:6.6.0'
    ```
    Ako koristite noviji Android Studio, vjerojatno neće trebati dodati `jackOptions`, a u posljednjem retku umjesto `compile` vjerojatno treba pisati `implementation`.
5. U `build.gradle` (za **projekt**) unutar **obje** repositories {} grupe dodajte ono što nedostaje od sljedećeg:
    ```xml
    google()
    jcenter()
    mavenCentral()
    ```
6. U `res/values/strings.xml` dodajte novi string imena *access_token*, a vrijednost postavite na ključ koji možete (bez dodavanja načina plaćanja) generirati na [stranicama *Mapboxa*](https://www.mapbox.com/signup/). Možete koristiti i [ovaj ključ](https://web.math.pmf.unizg.hr/~lmikec/auth/mapbox_access_token.txt). 
7. Ovo je dobar trenutak da provjerite je li instalacija *Mapboxa* uspješna. Sinkronizirajte *Gradle* skripte i probajte pokrenuti projekt. (Ne biste još trebali vidjeti karte.)
8. Izradite datoteku [res/menu/mainmenu.xml](mapbox/mainmenu.xml). Zamijenite sadržaje datoteka [MainActivity.java](mapbox/MainActivity.java) i [activity_main.xml](mapbox/activity_main.xml).
9. Dodajte ispravan *package* na vrhu `MainActivity.java` datoteke.
