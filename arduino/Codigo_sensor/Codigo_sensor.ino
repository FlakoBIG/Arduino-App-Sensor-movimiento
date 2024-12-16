#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// redes wifi
const char* ssid[] = {"The Skinnys", "Redmi", "FLAKOPC"};
const char* password[] = {"21542460-kCollado1019", "Collado1019", "123456789"};

WiFiClient client;
unsigned long channelID = 2786455;
const char* readAPIKey = "WBMZ0AUOTY7U0Y28";
const char* writeAPIKey = "IFNI5JJY82GZ1JTP";

const int ledPin = D5;
const int sensorPin = D3;

bool ledEncendido = false;
bool controlPorApp = false;

void setup() {
    Serial.begin(115200);
    pinMode(sensorPin, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    Serial.println("Iniciando sistema...");
    conectarWiFi();
    ThingSpeak.begin(client);
    Serial.println("Sincronizando estado inicial del LED...");
    verificarControlApp();
    Serial.println("Sistema iniciado correctamente.");
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi desconectado. Reintentando conexion...");
        conectarWiFi();
    }

    // Reducir tiempo entre verificaciones
    static unsigned long tiempoAnterior = 0;
    if (millis() - tiempoAnterior >= 1000) { //revisa cada 1 segundo 
        tiempoAnterior = millis();
        Serial.println("Verificando control desde la app...");
        verificarControlApp();
    }

    // si hay control con la app no usamos sensor
    if (controlPorApp) {
        return;
    }

    // control por sensor
    int sensorState = digitalRead(sensorPin);
    if (sensorState == HIGH && !ledEncendido) {
        Serial.println("Sensor detecto movimiento. Encendiendo LED...");
        actualizarField1(1); // Actualizar  field1 a 1
        encenderLED();
        delay(15010); // 15 segundos encendido
        Serial.println("Apagando LED despues del retardo.");
        apagarLED();
        actualizarField1(0); // Actualizar  field1 a 0
    }
}

// conectar con el wifi
void conectarWiFi() {
    int redActual = 0;
    bool conectado = false;

    while (redActual < 3 && !conectado) {
        Serial.print("Intentando conectar a WiFi: ");
        Serial.println(ssid[redActual]);

        WiFi.begin(ssid[redActual], password[redActual]);
        int intentos = 0;

        while (WiFi.status() != WL_CONNECTED && intentos < 20) {
            delay(500);
            Serial.print(".");
            intentos++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi conectado exitosamente.");
            Serial.print("IP asignada: ");
            Serial.println(WiFi.localIP());
            conectado = true;
        } else {
            Serial.println("\nNo se pudo conectar a esta red WiFi.");
            redActual++;
        }
    }

    if (!conectado) {
        Serial.println("\nNo se pudo conectar a ninguna red. Reiniciando...");
        ESP.restart();
    }
}

void verificarControlApp() {
    int field2Value = ThingSpeak.readIntField(channelID, 2, readAPIKey);
    if (ThingSpeak.getLastReadStatus() != 200) {
        Serial.print("Error al leer field2, error: ");
        Serial.println(ThingSpeak.getLastReadStatus());
        return;
    }

    Serial.print("Valor de field2: ");
    Serial.println(field2Value);

    if (field2Value == 1) {
        // Encender LED manualmente
        if (!ledEncendido) {
            Serial.println("Encendiendo LED por control de la app.");
            encenderLED();
        }
        controlPorApp = true;
    } else if (field2Value == 0) {
        // Apagar LED y reactivar sensor
        if (ledEncendido) {
            Serial.println("Apagando LED por control de la app.");
            apagarLED();
        }
        Serial.println("Reactivando control por sensor.");
        controlPorApp = false;
    }
}

void encenderLED() {
    digitalWrite(ledPin, HIGH);
    ledEncendido = true;
    Serial.println("LED encendido.");
}

void apagarLED() {
    digitalWrite(ledPin, LOW);
    ledEncendido = false;
    Serial.println("LED apagado.");
}

// Funcion para actualizar field1
void actualizarField1(int valor) {
    ThingSpeak.setField(1, valor);
    int status = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (status == 200) {
        Serial.print("Field1 actualizado correctamente a: ");
        Serial.println(valor);
    } else {
        Serial.print("Error al actualizar field1. Codigo de error: ");
        Serial.println(status);
    }
}
