package com.example.arduino;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {

    private ImageView iconoLuz;
    private Button botonEncender;
    private Button botonApagar;
    private Button botonGrafica;
    private boolean luzEncendida = false;
    private static final String WRITE_API_KEY = "IFNI5JJY82GZ1JTP";
    private static final String CHANNEL_ID = "2786455";
    private static final String URL_UPDATE = "https://api.thingspeak.com/update?api_key=" + WRITE_API_KEY;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        iconoLuz = findViewById(R.id.icono_luz);
        botonEncender = findViewById(R.id.boton_encender);
        botonApagar = findViewById(R.id.boton_apagar);
        botonGrafica = findViewById(R.id.boton_grafica);

        botonEncender.setOnClickListener(v -> enviarComando(true));
        botonApagar.setOnClickListener(v -> enviarComando(false));
        botonGrafica.setOnClickListener(v -> abrirGraficaActivity());
    }

    //envia un comando para encender o apagar
    private void enviarComando(boolean encender) {
        String comando = encender ? "&field2=1" : "&field2=0";
        String url = URL_UPDATE + comando;

        deshabilitarBotones(); // desactiva los botone

        RequestQueue queue = Volley.newRequestQueue(this);
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    luzEncendida = encender;
                    actualizarEstadoUI();
                },
                error -> Toast.makeText(MainActivity.this, "Error al enviar el comando", Toast.LENGTH_SHORT).show()
        );

        queue.add(stringRequest);
    }

    //desactiva los botone con un color y tiempo de 15.5 s
    private void deshabilitarBotones() {
        botonEncender.setEnabled(false);
        botonApagar.setEnabled(false);
        botonEncender.setBackgroundColor(getResources().getColor(R.color.gris));
        botonApagar.setBackgroundColor(getResources().getColor(R.color.gris));

        new Handler().postDelayed(() -> {
            botonEncender.setEnabled(true);
            botonApagar.setEnabled(true);
            botonEncender.setBackgroundColor(getResources().getColor(R.color.color_primario));
            botonApagar.setBackgroundColor(getResources().getColor(R.color.color_secundario));
        }, 15500); // 15.5 segundos
    }

    // actualiza la intefas segun el estado del led
    private void actualizarEstadoUI() {
        if (luzEncendida) {
            iconoLuz.setImageResource(R.drawable.luzon); // icono on
            iconoLuz.setColorFilter(getResources().getColor(R.color.amarillo), android.graphics.PorterDuff.Mode.SRC_IN); // Aplica el tinte amarillo
        } else {
            iconoLuz.setImageResource(R.drawable.luzoff); // icono off
            iconoLuz.clearColorFilter(); // elimina tinte
        }
    }

    // grafica
    private void abrirGraficaActivity() {
        Intent intent = new Intent(this, GraficaActivity.class);
        startActivity(intent);
    }
}
