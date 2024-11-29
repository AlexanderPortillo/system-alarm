//este es
int val = 0;                  // Variable donde almacenamos el estado del sensor de fuego
int smokeSensorValue = 0;     // Variable donde almacenamos el valor del sensor MQ-7 (humo)
const int relayPin = 6;       // Pin donde está conectado el relay
const int buzzerPin = 11;     // Pin donde está conectado el buzzer
const int rainSensorPin = 4;  // Pin donde está conectado el sensor de fuego
const int ledSmokePin = 3;    // Pin donde está conectado el LED (humo)
const int ledFirePin = 2;     // Pin donde está conectado el LED (fuego)
const int mq7Pin = A0;        // Pin analógico donde está conectado el sensor MQ-7 (humo)

const int buzzerTone = 1000;  // Frecuencia del tono del buzzer

// Variables para el parpadeo de los LEDS
unsigned long previousMillis = 0;       // Para almacenar el último tiempo de parpadeo
unsigned long previousMillisSmoke = 0;  // Para almacenar el último tiempo de parpadeo del LED de humo
unsigned long lastSerialPrint = 0;      // Para controlar la frecuencia de las impresiones seriales
const long interval = 500;              // Intervalo de parpadeo (en milisegundos)
const long serialPrintInterval = 1000;  // Intervalo para imprimir el valor del sensor (en milisegundos)

void setup() {
  Serial.begin(9600);             // Inicializar la comunicación serial
  pinMode(rainSensorPin, INPUT);  // Configurar el pin del sensor de fuego como entrada
  pinMode(relayPin, OUTPUT);      // Configurar el pin del relay como salida
  pinMode(buzzerPin, OUTPUT);     // Configurar el pin del buzzer como salida
  pinMode(ledFirePin, OUTPUT);    // Configurar el pin del LED (fuego) como salida
  pinMode(ledSmokePin, OUTPUT);   // Configurar el pin del LED (humo) como salida
  pinMode(mq7Pin, INPUT);         // Configurar el pin del sensor MQ-7 como entrada

  digitalWrite(relayPin, HIGH);    // Inicializar el relay en HIGH
  digitalWrite(ledFirePin, LOW);   // Asegurar que el LED de fuego esté apagado al inicio
  digitalWrite(ledSmokePin, LOW);  // Asegurar que el LED de humo esté apagado al inicio
}

void loop() {
  val = digitalRead(rainSensorPin);       // Leer el estado del sensor de fuego
  smokeSensorValue = analogRead(mq7Pin);  // Leer el valor del sensor MQ-7 (humo)

  // Solo se imprime si se detecta humo o fuego
  if (millis() - lastSerialPrint >= serialPrintInterval) {
    lastSerialPrint = millis();
    // Enviar los datos solo cuando se detecta un evento importante
    if (val == LOW) {
      Serial.println("¡Fuego detectado!");  // Enviar mensaje de fuego detectado
    } else if (smokeSensorValue > 300) {
      Serial.println("¡Humo detectado!");  // Enviar mensaje de humo detectado
    } else {
      Serial.println("¡Nada detectado!");  // No se detecta nada
    }
  }

  // Llamamos a las funciones para manejar las detecciones de fuego y humo
  if (val == LOW) {  // Si se detecta el fuego
    handleFireDetection();
  } else if (smokeSensorValue > 300) {  // Si se detecta el humo
    handleSmokeDetection();
  } else {  // Si no se detecta ni fuego ni humo
    handleNoDetection();
  }

  delay(100);  // Pequeño delay para evitar impresión serial excesiva
}

// Función para manejar la detección de fuego
void handleFireDetection() {
  digitalWrite(relayPin, HIGH);  // Activar el relay
  tone(buzzerPin, buzzerTone);   // Emitir un tono para indicar fuego

  // Parpadeo del LED de fuego
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int ledState = digitalRead(ledFirePin);
    if (ledState == LOW) {
      digitalWrite(ledFirePin, HIGH);  // Encender el LED de fuego
    } else {
      digitalWrite(ledFirePin, LOW);  // Apagar el LED de fuego
    }
  }

  digitalWrite(ledSmokePin, LOW);  // Apagar el LED de humo cuando se detecta fuego
}

// Función para manejar la detección de humo
void handleSmokeDetection() {
  digitalWrite(relayPin, HIGH);  // Activar el relay
  tone(buzzerPin, buzzerTone);   // Emitir un tono para indicar humo

  // Parpadeo del LED de humo
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisSmoke >= interval) {
    previousMillisSmoke = currentMillis;
    int ledSmokeState = digitalRead(ledSmokePin);
    if (ledSmokeState == LOW) {
      digitalWrite(ledSmokePin, HIGH);  // Encender el LED de humo
    } else {
      digitalWrite(ledSmokePin, LOW);  // Apagar el LED de humo
    }
  }

  digitalWrite(ledFirePin, LOW);  // Apagar el LED de fuego cuando se detecta humo
}

// Función para manejar el caso cuando no se detecta ni fuego ni humo
void handleNoDetection() {
  digitalWrite(relayPin, LOW);     // Desactivar el relay
  noTone(buzzerPin);               // Detener el tono del buzzer
  digitalWrite(ledFirePin, LOW);   // Apagar el LED de fuego
  digitalWrite(ledSmokePin, LOW);  // Apagar el LED de humo
}