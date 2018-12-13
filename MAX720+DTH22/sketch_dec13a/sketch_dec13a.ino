// http://robotclass.ru/tutorials/arduino-matrix-8x8-max7219/

#include "DHT.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>


#define DHTPIN 2 // what digital pin we're connected to
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11 // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors. This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.

int LED_MATRIX_PIN = 9;


DHT dht(DHTPIN, DHTTYPE);

int numberOfHorizontalDisplays = 1; // числа матрец по горизонтале
int numberOfVerticalDisplays = 1;   // числа матрец по вертикале

Max72xxPanel matrix = Max72xxPanel(LED_MATRIX_PIN, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 150; // время между крайними перемещениями букв

int spacer = 0; // расстояние между буквами
int width = 6 + spacer; // размер шрифта
int brightness = 0; // яркость от 0 до 15

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
  matrix.setIntensity(brightness); // яркость
}

void loop() {
  delay(3000); // Wait a few seconds between measurements
  float h = dht.readHumidity();   // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();   // Read temperature as Celsius (the default)
  float f = dht.readTemperature(true);   // Read temperature as Fahrenheit (isFahrenheit = true)
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print ("Humidity: ");
  Serial.print (h);
  Serial.print (" %\t");
  Serial.print ("Temperature: ");
  Serial.print (t);
  Serial.print (" *C ");
  Serial.print (f);
  Serial.print (" *F\t");
  Serial.print ("Heat index: ");
  Serial.print (hic);
  Serial.print (" *C ");
  Serial.print (hif);
  Serial.println (" *F");

  String tape = String(round(h)) + "%    " + String(round(t)) + "C   "; // текст, который будет плыть

  for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {
    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length() ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }

    matrix.write();
    matrix.fillScreen(LOW);
    delay(wait);
  }
}
