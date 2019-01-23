import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import com.mapbox.android.core.permissions.PermissionsListener;
import com.mapbox.android.core.permissions.PermissionsManager;
import com.mapbox.mapboxsdk.Mapbox;
import com.mapbox.mapboxsdk.annotations.MarkerOptions;
import com.mapbox.mapboxsdk.annotations.PolylineOptions;
import com.mapbox.mapboxsdk.camera.CameraPosition;
import com.mapbox.mapboxsdk.constants.Style;
import com.mapbox.mapboxsdk.geometry.LatLng;
import com.mapbox.mapboxsdk.location.LocationComponent;
import com.mapbox.mapboxsdk.location.LocationComponentOptions;
import com.mapbox.mapboxsdk.location.modes.CameraMode;
import com.mapbox.mapboxsdk.location.modes.RenderMode;
import com.mapbox.mapboxsdk.maps.MapView;
import com.mapbox.mapboxsdk.maps.MapboxMap;
import com.mapbox.mapboxsdk.maps.OnMapReadyCallback;

import java.util.List;

public class MainActivity extends AppCompatActivity implements
        OnMapReadyCallback, PermissionsListener {

    private PermissionsManager permissionsManager;
    private MapboxMap mapboxMap;
    private MapView mapView;

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.mainmenu, menu);
        return true;
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Mapbox.getInstance(this, getString(R.string.access_token));

        setContentView(R.layout.activity_main);

        mapView = findViewById(R.id.mapView);
        mapView.onCreate(savedInstanceState);
        mapView.getMapAsync(this);
    }

    @Override
    public void onMapReady(MapboxMap mapboxMap) {
        MainActivity.this.mapboxMap = mapboxMap;
    }

    @SuppressWarnings( {"MissingPermission"})
    private void enableLocationComponent() {
        if (PermissionsManager.areLocationPermissionsGranted(this)) {

            LocationComponentOptions options =
                    LocationComponentOptions.builder(this)
                            .trackingGesturesManagement(true)
                            .accuracyColor(ContextCompat.getColor(this,
                                    R.color.mapbox_blue))
                            .build();

            LocationComponent locationComponent =
                    mapboxMap.getLocationComponent();

            locationComponent.activateLocationComponent(this, options);

            locationComponent.setLocationComponentEnabled(true);

            locationComponent.setCameraMode(CameraMode.TRACKING);
            locationComponent.setRenderMode(RenderMode.COMPASS);
        } else {
            permissionsManager = new PermissionsManager(this);
            permissionsManager.requestLocationPermissions(this);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull
            String[] permissions, @NonNull int[] grantResults) {
        permissionsManager.onRequestPermissionsResult(requestCode,
                permissions, grantResults);
    }

    @Override
    public void onExplanationNeeded(List<String> permissionsToExplain) {
        Toast.makeText(this, "Zasto nam treba lokacija...",
                Toast.LENGTH_LONG).show();
    }

    @Override
    public void onPermissionResult(boolean granted) {
        if (granted) {
            enableLocationComponent();
        } else {
            Toast.makeText(this, "Korisnik nije dao dopustenje...",
                    Toast.LENGTH_LONG).show();
            finish();
        }
    }

    @Override
    @SuppressWarnings( {"MissingPermission"})
    protected void onStart() {
        super.onStart();
        mapView.onStart();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mapView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mapView.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mapView.onStop();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        mapView.onSaveInstanceState(outState);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mapView.onDestroy();
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
        mapView.onLowMemory();
    }

    private void zoom(double koliko)
    {
        CameraPosition pos = new CameraPosition.Builder()
                .target(mapboxMap.getCameraPosition().target)
                .bearing(mapboxMap.getCameraPosition().bearing)
                .tilt(mapboxMap.getCameraPosition().tilt)
                .zoom(mapboxMap.getCameraPosition().zoom * koliko).build();
        mapboxMap.setCameraPosition(pos);
    }

    private void pomak(double geoSirina, double geoDuzina)
    {
        CameraPosition pos = new CameraPosition.Builder()
                .target(new LatLng(geoSirina, geoDuzina))
                .bearing(mapboxMap.getCameraPosition().bearing)
                .tilt(mapboxMap.getCameraPosition().tilt)
                .zoom(mapboxMap.getCameraPosition().zoom).build();
        mapboxMap.setCameraPosition(pos);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        switch (item.getItemId()) {

            case R.id.menu_sethybrid:
                mapboxMap.setStyleUrl(Style.SATELLITE_STREETS);
                break;
            case R.id.menu_showtraffic:
                mapboxMap.setStyleUrl(Style.TRAFFIC_DAY);
                break;
            case R.id.menu_zoomin:
                zoom(2);
                break;
            case R.id.menu_zoomout:
                zoom(0.5);
                break;
            case R.id.menu_getcurrentlocation:
                enableLocationComponent();
                break;
            case R.id.menu_gotolocation:
                pomak(5, 10);
                break;
            case R.id.menu_addmarker:
                MarkerOptions m = new MarkerOptions();
                m.setPosition(new LatLng(5, 10));
                mapboxMap.addMarker(m);
                break;
            case R.id.menu_lineconnecttwopoints:
                PolylineOptions p = new PolylineOptions();
                p.add(new LatLng(4, 10));
                p.add(new LatLng(5, 11));
                p.add(new LatLng(5, 9));
                mapboxMap.addPolyline(p);
                break;
        }

        return true;
    }

