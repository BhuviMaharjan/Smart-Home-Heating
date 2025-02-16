#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

// Define pin connections
#define PIR_PIN 6
#define TEMP_SENSOR_PIN A1
#define LED_POWER 8
#define LED_TEMP_LOW 9
#define LED_TEMP_HIGH 10
#define MOTOR_PIN 11
#define IR_PIN 7

// Create LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);
IRrecv irrecv(IR_PIN); // Initialize IR receiver on pin 7
decode_results results;

// Variables
bool pirDetected = false;
String lastIRCommand = "None";

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_TEMP_LOW, OUTPUT);
  pinMode(LED_TEMP_HIGH, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Heating");

  irrecv.enableIRIn(); // Start the IR receiver

  delay(2000);
  lcd.clear();
}

void loop() {
  // Motion detection
  pirDetected = digitalRead(PIR_PIN);

  if (pirDetected) {
    lcd.setCursor(0, 0);
    lcd.print("Motion Detected");

    int tempValue = analogRead(TEMP_SENSOR_PIN);
    float temperature = (tempValue / 1023.0) * 100; // Convert to Celsius

    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperature, 1); // One decimal place
    lcd.print(" C ");

    if (temperature < 20) {
      digitalWrite(MOTOR_PIN, HIGH);
      digitalWrite(LED_TEMP_LOW, HIGH);
      digitalWrite(LED_TEMP_HIGH, LOW);
    } else {
      digitalWrite(MOTOR_PIN, LOW);
      digitalWrite(LED_TEMP_LOW, LOW);
      digitalWrite(LED_TEMP_HIGH, HIGH);
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("No Motion      ");

    digitalWrite(MOTOR_PIN, LOW);
    digitalWrite(LED_TEMP_LOW, LOW);
    digitalWrite(LED_TEMP_HIGH, LOW);
  }

  // IR remote data handling
  if (irrecv.decode(&results)) {
    lastIRCommand = String(results.value, HEX); // Convert IR value to HEX string
    irrecv.resume(); // Resume the IR receiver

    // Display the last IR command on the LCD
    lcd.setCursor(0, 1);
    lcd.print("System on!");
    
    lcd.print("     "); // Clear trailing characters

    // Blink LED when any data is received from the IR remote
    blinkLED(LED_POWER, 5, 200); // Blink LED 5 times with 200ms interval
  }

  delay(200); // Small delay for stability
}

// Function to blink an LED
void blinkLED(int pin, int times, int interval) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(interval);
    digitalWrite(pin, LOW);
    delay(interval);
  }
}
