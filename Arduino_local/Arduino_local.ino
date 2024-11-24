#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Configuración de la red WiFi
const char* ssid = "The Skinnys";       // Nombre de la red WiFi
const char* password = "21542460-kCollado1019";  // Contraseña WiFi

WiFiClient client;

// Configuración de ThingSpeak
unsigned long myChannelNumber = 2760562; // Número de tu canal
const char* myWriteAPIKey = "QWLL9A6BN89VNK0K"; // API Key de escritura

// Definir los pines
const int ledPin = D5;      // Pin D5 donde está conectado el LED
const int sensorPin = D3;   // Pin D3 donde está conectado el sensor

void setup() {
  // Inicializar el monitor serie
  Serial.begin(115200);
  delay(10);

  // Configurar el LED como salida
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // LED apagado inicialmente

  // Configurar el sensor como entrada
  pinMode(sensorPin, INPUT);

  // Conexión a la red WiFi
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConexión WiFi establecida");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Leer el estado del sensor
  int sensorState = digitalRead(sensorPin);

  // Control del LED basado en el sensor
  if (sensorState == HIGH) {
    digitalWrite(ledPin, HIGH); // Encender LED si hay movimiento
    Serial.println("Movimiento detectado: LED encendido");

    // Enviar datos a ThingSpeak
    ThingSpeak.setField(1, 1); // Movimiento detectado (campo 1)
    ThingSpeak.setField(2, 1); // LED encendido (campo 2)
  } else {
    digitalWrite(ledPin, LOW); // Apagar LED si no hay movimiento
    Serial.println("Sin movimiento: LED apagado");

    // Enviar datos a ThingSpeak
    ThingSpeak.setField(1, 0); // Sin movimiento (campo 1)
    ThingSpeak.setField(2, 0); // LED apagado (campo 2)
  }

  // Enviar los campos configurados a ThingSpeak
  int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (responseCode == 200) {
    Serial.println("Datos enviados a ThingSpeak");
  } else {
    Serial.println("Error enviando datos: " + String(responseCode));
  }

  delay(20000); // Evita exceder el límite de envío de ThingSpeak (máx. 1 cada 15 s)
}
