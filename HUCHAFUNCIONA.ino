
#include <Wire.h> // Librería para comunicación I2C

#include <LiquidCrystal_I2C.h>  // Librería para controlar la pantalla LCD I2C

#include <EEPROM.h>  // Librería para manejar la memoria EEPROM

// Definición de pines para sensores y pantalla LCD

const int Trigger = 6;  // Pin para el trigger del sensor ultrasónico (no parece usarse en el código)

const int Echo = 5;  // Pin para el echo del sensor ultrasónico (no parece usarse en el código)

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Configuración de pantalla LCD con dirección 0x3F y tamaño 16x2
 
// Definición de pin para el botón de reset

const int botonPin = 13;
 
// Pines para sensores infrarrojos de detección de monedas

int irSensor_01 = 2;  // Sensor para monedas de 10 céntimos

float dinero_01;  //Variable para almacenar el dinero acumulado de monedas de 10 céntimos

int irSensor_005 = 4;  //Sensor para monedas de 5 céntimos

float dinero_005;  //Variable para almacenar el dinero acumulado de monedas de 5 céntimos


int irSensor_02 = 7;  //Sensor para monedas de 20 céntimos

float dinero_02;  //Variable para almacenar el dinero acumulado de monedas de 20 céntimos


int irSensor_1 = 8;  //Sensor para monedas de 1 euro

float dinero_1;  //Variable para almacenar el dinero acumulado de monedas de 1 euro


int irSensor_05 = 9;  //Sensor para monedas de 50 céntimos

float dinero_05;  //Variable para almacenar el dinero acumulado de monedas de 50 céntimos


int irSensor_2 = 10;  //Sensor para monedas de 2 euro

float dinero_2;  //Variable para almacenar el dinero acumulado de monedas de 2 euros


// Variables para manejar el total acumulado de dinero

float dinero_total_1;  // Variable que almacena el total antes de iniciar la lectura de nuevas monedas
 
// Variable para almacenar el total de dinero en EEPROM

float dinero_total;  // Esta variable se usará para guardar y recuperar el valor en EEPROM
 
 
// Función SETUP

void setup() {

   Serial.begin(9600);
// Configuración de la pantalla LCD

pinMode(Trigger, OUTPUT);  // Configuración del pin del Trigger como salida (no parece usarse)

pinMode(Echo, INPUT);  // Configuración del pin del Echo como entrada (no parece usarse)

digitalWrite(Trigger, LOW);

lcd.init();  // Inicializar pantalla LCD

lcd.backlight();  // Encender luz de fondo de la pantalla

lcd.setCursor(0, 0);

lcd.print("Bienvenido!");  // Mostrar mensaje de bienvenida

delay(1000);  // Esperar 1 segundo

lcd.clear();

lcd.setCursor(0, 0);

lcd.print("Total: ");  // Mostrar "Total: " en pantalla

lcd.setCursor(7, 0);

lcd.print(dinero_total_1);  // Mostrar el valor inicial del total


// Configuración de pines de sensores como entrada

pinMode(irSensor_01, INPUT);

pinMode(irSensor_005, INPUT);

pinMode(irSensor_02, INPUT);

pinMode(irSensor_1, INPUT);

pinMode(irSensor_05, INPUT);

pinMode(irSensor_2, INPUT);

// Configuración del botón de reset

pinMode(botonPin, INPUT);

delay(1000); // Esperar 1 segundo para evitar reinicios accidentales


// Borrar EEPROM si el botón de reset está presionado al iniciar

if (digitalRead(botonPin) == HIGH) {

  delay(200);  // Pequeño retraso para evitar rebotes

  while (digitalRead(botonPin) == HIGH); // Esperar hasta que el botón se suelte

  for (int i = 0; i < sizeof(dinero_total); i++) {

    EEPROM.write(i, 0);  // Escribir ceros en todas las posiciones de la EEPROM donde se guarda el dinero total

  }

  Serial.println("EEPROM borrada");

}

// Leer el valor almacenado en EEPROM y asignarlo a dinero_total_1

byte* p = (byte*)&dinero_total_1;

for (int i = 0; i < sizeof(dinero_total_1); i++) {
*(p + i) = EEPROM.read(i); // Leer cada byte de la EEPROM y reconstruir el float

}

// Imprimir el valor leído en la consola serial

Serial.print("Valor leído desde EEPROM: ");

Serial.println(dinero_total_1);

}


// Función LOOP

void loop() {
  // Detección de monedas y actualización de valores

   if (digitalRead(irSensor_01) == LOW) {

   delay(200); // Agregar un pequeño retraso para evitar lecturas erróneas

   dinero_01 += 0.1;

   }


   if (digitalRead(irSensor_005) == LOW) {

   delay(200); // Agregar un pequeño retraso para evitar lecturas erróneas

   dinero_005 += 0.05;

   }

   if (digitalRead(irSensor_02) == LOW) {

   delay(200); // Agregar un pequeño retraso para evitar lecturas erróneas

   dinero_02 += 0.2;

   }

   if (digitalRead(irSensor_1) == LOW) {

   delay(200); // Agregar un pequeño retraso para evitar lecturas erróneas

   dinero_1 += 1;

   }

   if (digitalRead(irSensor_05) == LOW) {

   delay(200); // Agregar un pequeño retraso para evitar lecturas erróneas

   dinero_05 += 0.5;

   }

   if (digitalRead(irSensor_2) == LOW) {

   delay(200); // Agregar un pequeño retraso para evitar lecturas erróneas

   dinero_2 += 2;

   }
 
// Calcular el total de dinero ingresado

dinero_total = dinero_total_1 + dinero_01 + dinero_005 + dinero_02 + dinero_1 + dinero_05 + dinero_2;

// Guardar el nuevo total en la EEPROM
  byte* p = (byte*)&dinero_total;
  for (int i = 0; i < sizeof(dinero_total); i++) {
    EEPROM.write(i, *(p + i));

    }
 
// Actualizar el valor mostrado en la pantalla LCD solo si cambia

static float last_dinero_total = 0.0; // Variable para comparar el último valor mostrado

if (dinero_total != last_dinero_total) {

last_dinero_total = dinero_total; // Actualizar la variable de comparación

lcd.setCursor(7, 0);

lcd.print(dinero_total, 2); // Mostrar el total con dos decimales en la pantalla LCD

}

}
 
