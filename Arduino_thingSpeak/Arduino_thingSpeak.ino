#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Configuración de la red WiFi
const char* ssid = "The Skinnys";       
const char* password = "21542460-kCollado1019";  

WiFiClient client;  
unsigned long channelID = 123456;  
const char* apiKey = "QWLL9A6BN89VNK0K";  

const int ledPin = D5;  
const int sensorPin = D3; 

bool estadoManual = false; 

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  pinMode(sensorPin, INPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  int sensorState = digitalRead(sensorPin);

  // Control automático del LED
  if (sensorState == HIGH && !estadoManual) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Movimiento detectado: LED encendido");
  } else if (sensorState == LOW && !estadoManual) {
    digitalWrite(ledPin, LOW);
    Serial.println("Sin movimiento: LED apagado");
  }

  // Leer el valor de field2 desde ThingSpeak
  int field2Value = ThingSpeak.readIntField(channelID, 2); // Leemos el valor de field2

  // Si field2 es 1, encender el LED
  if (field2Value == 1) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED encendido por field2");
  }

  // Manejo de solicitudes HTTP
  WiFiClient webClient = server.available();
  if (webClient) {
    String request = webClient.readStringUntil('\r');
    Serial.println("Solicitud recibida: " + request);
    webClient.flush();

    if (request.indexOf("/ENCENDER") != -1) {
      digitalWrite(ledPin, HIGH);
      estadoManual = true;
      Serial.println("LED encendido por comando");
    } else if (request.indexOf("/APAGAR") != -1) {
      digitalWrite(ledPin, LOW);
      estadoManual = false;
      Serial.println("LED apagado por comando");
    }

    // Respuesta rápida
    webClient.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nComando recibido");
  }

  // Enviar datos a ThingSpeak solo si el estado del sensor cambia
  static int lastSensorState = -1;
  if (sensorState != lastSensorState) {
    ThingSpeak.setField(1, sensorState);  
    ThingSpeak.writeFields(channelID, apiKey);  
    lastSensorState = sensorState;
    Serial.println("Datos enviados a ThingSpeak");
  }

  delay(1000);  // Espera corta para no bloquear el bucle
}
