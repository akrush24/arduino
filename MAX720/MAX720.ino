#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 9;
int numberOfHorizontalDisplays = 6; // теперь у нас по-горизонтали 6 матриц
int numberOfVerticalDisplays = 1; // а по-вертикали, по-прежнему, одна

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape = "HELLO"; // текст, который будет плыть
int wait = 100; // время между крайними перемещениями букв

int spacer = 1; // расстояние между буквами
int width = 5 + spacer; // размер шрифта
int brightness = 0; // яркость от 0 до 15

void setup() {
    matrix.setIntensity(brightness); // яркость
}

void loop() {
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
        delay(wait);
    }
}
