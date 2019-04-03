//Proyecto ejemplo para realizar una minicentral meteorológica.
//Subido a GITHUB 03/04/2019.
//Copyright 2019 Francisco Javier Merchán Macías

//LCD. Librerías.
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//DHT o sensor humedad y temperatura. Librerías.
#include <SimpleDHT.h>
//Teclado membrana. Librerias.
#include <Keypad.h>

bool LUZPANTALLA = true;
int CONTADORLUZPANTALLA = 10;

//Sensor distancia. Variables y constantes.
long distancia;
long tiempo;
const int TRIGER = 13;
const int ECHO = 12;
const int LEDROJO = 2;
const int LEDVERDE = 3;
const int LEDAZUL = 4;
//DHT o sensor humedad y temperatura. Variables y constantes.
int pinDHT11 = 2;
SimpleDHT11 dht11;

//Teclado
const int TECLADOCOLUMNA01 = 24;
const int TECLADOCOLUMNA02 = 25;
const int TECLADOCOLUMNA03 = 26;
const int TECLADOCOLUMNA04 = 27;
const int TECLADOFILA01 = 28;
const int TECLADOFILA02 = 29;
const int TECLADOFILA03 = 22;
const int TECLADOFILA04 = 23;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {28, 29, 22, 23}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {24, 25, 26, 27}; //connect to the column pinouts of the keypad
//byte rowPins[ROWS] = {TECLADOFILA01, TECLADOFILA02, TECLADOFILA03, TECLADOFILA04}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {TECLADOCOLUMNA01, TECLADOCOLUMNA02, TECLADOCOLUMNA03, TECLADOCOLUMNA04}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);




//LCD. Desclarar I2C.
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
//LiquidCrystal_I2C lcd(0x27, 16,2);  // Set the LCD I2C address
//LiquidCrystal_I2C lcd(0x27, 6, 5, 4, 11, 12, 13, 14, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


void setup() {
  Wire.begin();
  Serial.begin(115200);
  lcd.begin(16, 2);

  pinMode(TRIGER, OUTPUT); //Enviamos el ping por el puerto 13.
  pinMode(ECHO, INPUT); //Recibimos la señal del ping del puerto 13 en el puerto 12.
  pinMode(LEDROJO, OUTPUT);
  pinMode(LEDVERDE, OUTPUT);
  pinMode(LEDAZUL, OUTPUT);

  /*
    //Teclado
    pinMode(TECLADOPIN01, INPUT);
    pinMode(TECLADOPIN02, INPUT);
    pinMode(TECLADOPIN03, INPUT);
    pinMode(TECLADOPIN04, INPUT);
    pinMode(TECLADOPIN05, INPUT);
    pinMode(TECLADOPIN06, INPUT);
    pinMode(TECLADOPIN07, INPUT);
    pinMode(TECLADOPIN08, INPUT);
  */
  //LCD
  /*
    //  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
    // ------- Quick 3 blinks of backlight  -------------
    for(int i = 0; i< 3; i++)
    {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
    }
    lcd.backlight(); // finish with backlight on
  */
  //-------- Write characters on the display ------------------
  // NOTE: Cursor Position: (CHAR, LINE) start at 0
  lcd.setCursor(0, 0); //Start at character 4 on line 0
  lcd.print(" Javier Merchan");
  delay(4000);
  lcd.noBacklight();
  delay(250);
  lcd.backlight();
  delay(250);
}


void loop() {
  /*
    //LCD
      // when characters arrive over the serial port...
    if (Serial.available()) {
      // wait a bit for the entire message to arrive
      delay(100);
      // clear the screen
      lcd.clear();
      // read all the available characters
      while (Serial.available() > 0) {
        // display each character to the LCD
        lcd.write(Serial.read());
        }
     }

  */

  digitalWrite(TRIGER, LOW); //Inicializamos el sensor
  delayMicroseconds(5);
  digitalWrite(TRIGER, HIGH); //Enviamos un pulso de 10 microsegundos.
  delayMicroseconds(10);
  tiempo = pulseIn(ECHO, HIGH); //Función para medir la longitud del pulso entrante.
  distancia = int(0.017 * tiempo); //Calculamos la distancia obteniendo un valor entero.
  //Enviamos la distancia por el puerto serie para poderlo ver en el PC.
  Serial.println("Distancia en cm");
  Serial.println(distancia);
  //Imprimimos la distanción en el LCD.
  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.setCursor(2, 1);
  lcd.print("    ");
  lcd.setCursor(2, 1);
  lcd.print(distancia);

  if (CONTADORLUZPANTALLA == 0) {
    lcd.noBacklight();
    LUZPANTALLA = false;
  }

if (distancia <= 10) {
    //Si la distancia es menor o igual a 10 cms, encendemos el Rojo, Verde y Azul, es decir,
    //conseguimos un LED BLANCO.
    digitalWrite(LEDROJO, LOW);
    digitalWrite(LEDAZUL, LOW);
    digitalWrite(LEDVERDE, LOW);
    lcd.backlight();
    CONTADORLUZPANTALLA = 20;
  }
  else if (distancia > 10 && distancia <= 20) {
    //Si la distancia está entre 10 cms (excluido) y 20 cms (incluido), encendemos el led ROJO.
    digitalWrite(LEDROJO, LOW);
    digitalWrite(LEDAZUL, HIGH);
    digitalWrite(LEDVERDE, HIGH);
    lcd.backlight();
    CONTADORLUZPANTALLA = 10;
  }
  else if (distancia > 20 && distancia <= 35) {
    //Si la distancia está entre 20 cms (excluido) y 35 cms (incluido), encendemos el led AZUL.
    digitalWrite(LEDROJO, HIGH);
    digitalWrite(LEDAZUL, LOW);
    digitalWrite(LEDVERDE, HIGH);
  }
  else {
    //Si la distancia es mayor o igual que 35cms encendemos el led VERDE.
    digitalWrite(LEDROJO, HIGH);
    digitalWrite(LEDAZUL, HIGH);
    digitalWrite(LEDVERDE, LOW);
  }

  //DHT o sensor humedad y temperatura. Leer y mostrar datos en LCD y serial.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    //    Serial.print("Lectura de temperatura incorrecta o fallida");
    //    Serial.print("");
    return;
  }
  Serial.print("Sample RAW Bits: ");
  for (int i = 0; i < 40; i++) {
    Serial.print((int)data[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
      Serial.print(' ');
    }
  }
  Serial.println("");

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" %");
  lcd.setCursor(6, 1);
  lcd.print(" H=   T=  ");
  lcd.setCursor(9, 1);
  lcd.print(humidity);
  lcd.setCursor(14, 1);
  lcd.print(temperature);


  //Yeclado membrana.
  char customKey = customKeypad.getKey();

  if (customKey) {
    Serial.println("");
    Serial.print("Tecla pulsada: ");
    Serial.println(customKey);
    if (customKey == 9) {
      lcd.clear();
    }
  }

  CONTADORLUZPANTALLA--;
  delay(1000);
}
