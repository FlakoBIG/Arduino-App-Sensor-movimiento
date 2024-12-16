package com.example.arduino;

import android.os.Bundle;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import androidx.appcompat.app.AppCompatActivity;

public class GraficaActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_grafica);

        // grafica field1
        WebView webViewField1 = findViewById(R.id.webview_grafica_field1);
        webViewField1.setWebViewClient(new WebViewClient());
        webViewField1.getSettings().setJavaScriptEnabled(true);
        webViewField1.loadUrl("https://thingspeak.com/channels/2786455/charts/1?api_key=WBMZ0AUOTY7U0Y28&bgcolor=%23ffffff&color=%23d62020&days=1&dynamic=true&results=10&type=line\n");

        // grafica field2
        WebView webViewField2 = findViewById(R.id.webview_grafica_field2);
        webViewField2.setWebViewClient(new WebViewClient());
        webViewField2.getSettings().setJavaScriptEnabled(true);
        webViewField2.loadUrl("https://thingspeak.com/channels/2786455/charts/2?api_key=WBMZ0AUOTY7U0Y28&bgcolor=%23ffffff&color=%2320d620&days=1&dynamic=true&results=10&type=line\n");
    }
}
