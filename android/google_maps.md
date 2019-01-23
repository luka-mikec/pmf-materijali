# Google Maps

1. Stvorite novi projekt, 4.0.3, praznu aktivnost.
2. U build.gradle projekta, pod dependencies dodajte compile 'com.google.android.gms:play-services-maps:11.0.4' 
3. Izradite novi API ključ: [Googleove stranice](https://developers.google.com/maps/documentation/android-sdk/signup). **BITNO:** za ovaj je korak potrebno dodati način plaćanja, jer se korištenje *Google Maps* servisa naplaćuje nakon iskorištene kvote besplatnih zahtjeva. Na kolegiju zbog toga koristimo [Mapbox](mapbox.md).
4. U `res/values/strings.xml` dodajte novi string imena *google_maps_key*, a vrijednost postavite na ključ koji ste dobili u prethodnom koraku.
5. Izradite datoteku [res/menu/mainmenu.xml](google_maps/mainmenu.xml). Zamijenite sadržaje datoteka [MainActivity.java](google_maps/MainActivity.java) i [activity_main.xml](google_maps/activity_main.xml).
6. Dodajte ispravan *package* na vrhu `MainActivity.java` datoteke.
7. Unutar `manifest` taga `AndroidManifest.xml` datoteke dodajte 
    ```xml
    <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    ```
a unutar `application` taga:
    ```xml
    <meta-data android:name="com.google.android.geo.API_KEY" android:value="@string/google_maps_key" />
    ```



