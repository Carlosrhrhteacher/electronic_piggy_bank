#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// Constantes

// LCD pantalla
const int Trigger = 6;
const int Echo = 5;
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Pulsador
const int botonPin = 13;

// Monedas 5 cent
int irSensor_005 = 2;
float dinero_005;

// Monedas 10 cent
int irSensor_01 = 4;
float dinero_01;

// Monedas 20 cent
int irSensor_02 = 7;
float dinero_02;

// Monedas 1 euro
int irSensor_1 = 8;
float dinero_1;

// Monedas 50 cent
int irSensor_05 = 9;
float dinero_05;

// Monedas 2 euros
int irSensor_2 = 10;
float dinero_2;

// Total dinero
float dinero_total_1;

// EEPROM
float dinero_total; // Ahora almacenaremos el valor de dinero_total en la EEPROM

// Función SETUP
void setup() {
  Serial.begin(9600);

  // LCD pantalla
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  digitalWrite(Trigger, LOW);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Total: ");
  lcd.setCursor(7, 0);
  lcd.print(dinero_total_1);

  // Sensores
  pinMode(irSensor_005, INPUT);
  pinMode(irSensor_01, INPUT);
  pinMode(irSensor_02, INPUT);
  pinMode(irSensor_1, INPUT);
  pinMode(irSensor_05, INPUT);
  pinMode(irSensor_2, INPUT);

  // Pulsador
  pinMode(botonPin, INPUT);
  delay(1000); // tiempo pulsando el boton para evitar borrar por error

  // Borrar EEPROM
  if (digitalRead(botonPin) == HIGH) {
    delay(200);  // Para evitar rebotes
    while (digitalRead(botonPin) == HIGH);  // Esperar hasta que el botón sea liberado
    for (int i = 0; i < sizeof(dinero_total); i++) {
      EEPROM.write(i, 0); // Escribir ceros en cada dirección de la EEPROM
    }
    Serial.println("EEPROM borrada");
  }

  // Leer el valor de la EEPROM y reconstruir el float
  byte* p = (byte*)&dinero_total_1;
  for (int i = 0; i < sizeof(dinero_total_1); i++) {
    *(p + i) = EEPROM.read(i);
  }

  // Imprimir el valor leído
  Serial.print("Valor leído desde EEPROM: ");
  Serial.println(dinero_total_1);
}

// Función LOOP
void loop() {
  // Monedas 5 cent
  if (digitalRead(irSensor_005) == LOW) {
    delay(200);  // Agregar un pequeño retraso para evitar lecturas erróneas
    dinero_005 += 0.05;
  }

  // Monedas 10 cent
  if (digitalRead(irSensor_01) == LOW) {
    delay(200);
    dinero_01 += 0.1;
  }

  // Monedas 20 cent
  if (digitalRead(irSensor_02) == LOW) {
    delay(200);
    dinero_02 += 0.2;
  }

  // Monedas 1 euro
  if (digitalRead(irSensor_1) == LOW) {
    delay(200);
    dinero_1 += 1;
  }

  // Monedas 50 cent
  if (digitalRead(irSensor_05) == LOW) {
    delay(200);
    dinero_05 += 0.5;
  }

  // Monedas 2 euros
  if (digitalRead(irSensor_2) == LOW) {
    delay(200);
    dinero_2 += 2;
  }

  // Monitorear monedas individuales y actualizar el total
  dinero_total = dinero_total_1 + dinero_005 + dinero_01 + dinero_02 + dinero_1 + dinero_05 + dinero_2;

  // Actualizar el valor en la EEPROM (4 bytes)
  byte* p = (byte*)&dinero_total;
  for (int i = 0; i < sizeof(dinero_total); i++) {
    EEPROM.write(i, *(p + i));
  }

  // Actualizar el total en LCD solo si ha cambiado
  static float last_dinero_total = 0.0;
  if (dinero_total != last_dinero_total) {
    last_dinero_total = dinero_total;
    lcd.setCursor(7, 0);
    lcd.print(dinero_total, 2); // Mostrar el total con dos decimales
  }
}
