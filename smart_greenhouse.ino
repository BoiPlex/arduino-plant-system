#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd_1(0);

// Inputs
int buttonPin = 7; // switch between 2 modes: blueberry and spinach
int lightPin = A0;
int TDSPin = A1;
int moisturePin = A2;

// Outputs
int redPin = 10;
int greenPin = 8;
int bluePin = 9;
int pumpPin = 11;

// Blueberry and spinach modes
int mode = 0; // 0 for blueberry (default), 1 for spinach
int lightLimit[] = {100, 200};
int TDSRange[][] = {{500, 1000}, {600, 1100}};
int moistureLimit[] = {300, 400};

void setup() {
    lcd_1.begin(16, 2);
    // lcd_1.print("hello world");

    pinMode(buttonPin, INPUT);
    pinMode(lightPin, INPUT);
    pinMode(TDSPin, INPUT);
    pinMode(moisturePin, INPUT);

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(pumpPin, OUTPUT);
}

void loop() {
    // Toggle mode if button is pressed
    if (digitalRead(buttonPin) == HIGH) {
        if (mode == 0) mode = 1;
        else mode = 0;
    }

    float lightLevel = analogRead(lightPin);
    float TDSLevel = analogRead(TDSPin);
    float moistureLevel = analogRead(moisturePin);
    // Not sure how much you can fit into the LED, you'll have to test it
    // The LED's first row is reserved for the status message
    lcd_1.setCursor(0, 1);
    lcd_1.print(lightLevel);
    lcd_1.setCursor(0, 2);
    lcd_1.print(TDSLevel);
    lcd_1.setCursor(0, 3);
    lcd_1.print(moistureLevel);

    // Smart greenhouse logic tree
    if (lightLevel < lightLimit[mode]) { // Low light
        setLED(255, 255, 0); // yellow
        lcd_1.setCursor(0, 0);
        lcd_1.print("Light level too low!");
    }
    else { // Proper light
        if (TDSLevel < TDSRange[mode][0]) { // Low TDS
            setLED(255, 165, 0); // orange
            lcd_1.setCursor(0, 0);
            lcd_1.print("TDS level too low!");
        }
        else if (TDSLevel > TDSRange[mode][1]) { // High TDS
            setLED(255, 0, 0); // red
            lcd_1.setCursor(0, 0);
            lcd_1.print("TDS level too high!");
        }
        else { // Proper TDS
            if (moistureLevel < moistureLimit[mode]) { // Low moisture
                analogWrite(pumpPin, 100) // activate pump
                setLED(0, 0, 255); // blue
                lcd_1.setCursor(0, 0);
                lcd_1.print("Pumping water!");
            }
            else { // Everything is good
                analogWrite(pumpPin, 0) // deactivate pump
                setLED(0, 255, 0); // green
                lcd_1.setCursor(0, 0);
                lcd_1.print("Everything is good!");
            }
        }
    }

    // Delay?
    delay(500);
}

void setLED(red, green, blue) {
    analogWrite(redPin, red);
    analogWrite(redPin, green);
    analogWrite(redPin, blue);
}

/* For reference (delete later)

lcd_1.setCursor(0, 1);
lcd_1.print(seconds);

lcd_1.setBacklight(0);
delay(500);
*/
