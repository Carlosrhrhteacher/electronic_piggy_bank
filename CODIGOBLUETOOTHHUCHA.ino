#include <Wire.h> // Librería para comunicación I2C
#include <LiquidCrystal_I2C.h> // Librería para controlar la pantalla
LCD vía I2C
#include <EEPROM.h> // Librería para manejar la memoria
EEPROM
#include <SoftwareSerial.h> // Librería para comunicación serial por
software
// Configuración del módulo Bluetooth en los pines 10 (RX) y 11 (TX)
SoftwareSerial BT1(10, 11);
// Configuración de la pantalla LCD con dirección I2C 0x3F y dimensiones
16x2
LiquidCrystal_I2C lcd(0x3F, 16, 2);
// Definición del pin para el botón de reinicio
const int botonPin = 13;
// Definición de los pines para los sensores infrarrojos y variables
para el dinero acumulado por cada tipo de moneda
int irSensor_01 = 2; // Sensor para moneda de 0.1€
float dinero_01 = 0;
int irSensor_005 = 4; // Sensor para moneda de 0.05€
float dinero_005 = 0;
int irSensor_02 = 7; // Sensor para moneda de 0.2€
float dinero_02 = 0;
int irSensor_1 = 8; // Sensor para moneda de 1€
float dinero_1 = 0;
int irSensor_05 = 9; // Sensor para moneda de 0.5€
float dinero_05 = 0;
int irSensor_2 = 10; // Sensor para moneda de 2€
float dinero_2 = 0;
// Variables para almacenar el total de dinero acumulado
float dinero_total_1 = 0;
float dinero_total = 0;
void setup() {
 Serial.begin(9600); // Inicia la comunicación serial a 9600 bps
 BT1.begin(38400); // Inicia la comunicación con el módulo
Bluetooth a 38400 bps
 // Inicializa la pantalla LCD
 lcd.init();
 lcd.backlight(); // Enciende la luz de fondo de la pantalla
 lcd.setCursor(0, 0);
 lcd.print("Bienvenido!"); // Muestra mensaje de bienvenida
 delay(1000);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Total: ");
 lcd.setCursor(7, 0);
 lcd.print(dinero_total_1); // Muestra el total acumulado
 // Configura los pines de los sensores y el botón como entradas
 pinMode(irSensor_01, INPUT);
 pinMode(irSensor_005, INPUT);
 pinMode(irSensor_02, INPUT);
 pinMode(irSensor_1, INPUT);
 pinMode(irSensor_05, INPUT);
 pinMode(irSensor_2, INPUT);
 pinMode(botonPin, INPUT);
 delay(1000);
 // Si el botón de reinicio está presionado al iniciar, borra la
EEPROM
 if (digitalRead(botonPin) == HIGH) {
 delay(200);
 while (digitalRead(botonPin) == HIGH); // Espera a que se suelte
el botón
 for (int i = 0; i < sizeof(dinero_total); i++) {
 EEPROM.write(i, 0); // Escribe 0 en cada posición de la
EEPROM correspondiente
 }
 Serial.println("EEPROM borrada");
 }
 // Lee el valor almacenado en la EEPROM y lo asigna a dinero_total_1
 byte* p = (byte*)&dinero_total_1;
 for (int i = 0; i < sizeof(dinero_total_1); i++) {
 *(p + i) = EEPROM.read(i);
 }
 Serial.print("Valor leído desde EEPROM: ");
 Serial.println(dinero_total_1);
}
void loop() {
 // Verifica el estado de cada sensor infrarrojo y suma el valor
correspondiente si detecta una moneda
 if (digitalRead(irSensor_01) == LOW) { delay(200); dinero_01 += 0.1;
}
 if (digitalRead(irSensor_005) == LOW) { delay(200); dinero_005 +=
0.05; }
 if (digitalRead(irSensor_02) == LOW) { delay(200); dinero_02 += 0.2;
}
 if (digitalRead(irSensor_1) == LOW) { delay(200); dinero_1 += 1; }
 if (digitalRead(irSensor_05) == LOW) { delay(200); dinero_05 += 0.5;
}
 if (digitalRead(irSensor_2) == LOW) { delay(200); dinero_2 += 2; }
 // Calcula el total de dinero acumulado
 dinero_total = dinero_total_1 + dinero_01 + dinero_005 + dinero_02
+ dinero_1 + dinero_05 + dinero_2;
 // Guarda el nuevo total en la EEPROM
 byte* p = (byte*)&dinero_total;
 for (int i = 0; i < sizeof(dinero_total); i++) {
 EEPROM.write(i, *(p + i));
 }
 // Actualiza la pantalla LCD solo si el total ha cambiado
 static float last_dinero_total = 0.0;
 if (dinero_total != last_dinero_total) {
 last_dinero_total = dinero_total;
 lcd.setCursor(7, 0);
 lcd.print(dinero_total, 2); // Muestra el total con dos
decimales
 }
 recibirComandoBT(); // Verifica si hay comandos entrantes por
Bluetooth
 // Envía el total acumulado por Bluetooth cada 5 segundos
 if (millis() % 5000 == 0) {
 enviarDineroBT();
 }
}
// Función para enviar el total de dinero acumulado vía Bluetooth
void enviarDineroBT() {
 char buffer[10];
 sprintf(buffer, "%.2f", dinero_total); // Formatea el total con dos
decimales
 BT1.write('*'); // Carácter de inicio
 BT1.write('D'); // Indicador de dato
 BT1.write(buffer); // Total acumulado
 BT1.write('*'); // Carácter de fin
}
// Función para recibir y procesar comandos entrantes por Bluetooth
void recibirComandoBT() {
 if (BT1.available()) {
 char comando = BT1.read();
 if (comando == 'R') { // Si el comando es 'R', reinicia el
total acumulado
 dinero_total = 0;
 EEPROM.write(0, 0); // Borra el valor almacenado en la
EEPROM
 }
 }
}
