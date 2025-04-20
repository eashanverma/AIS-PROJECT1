#include <LiquidCrystal.h>

// Define sensor and actuator pins
#define MOISTURE_SENSOR A0
#define FLOAT_SWITCH A1
#define MOTOR_PIN 4
#define SOIL_STATUS_PIN 2
#define TANK_STATUS_PIN 3

// LCD pins: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// Moisture threshold
const int MOISTURE_THRESHOLD = 800;

void setup() {
    Serial.begin(9600);
    
    // Initialize LCD
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Auto Irrigation");
    lcd.setCursor(0, 1);
    lcd.print("System Starting");
    delay(2000);

    // Set pin modes
    pinMode(MOISTURE_SENSOR, INPUT);
    pinMode(FLOAT_SWITCH, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(SOIL_STATUS_PIN, OUTPUT);
    pinMode(TANK_STATUS_PIN, OUTPUT);

    // Ensure everything is off initially
    digitalWrite(MOTOR_PIN, LOW);
    digitalWrite(SOIL_STATUS_PIN, LOW);
    digitalWrite(TANK_STATUS_PIN, LOW);
}

void loop() {
    // Read sensor values
    int moisture = analogRead(MOISTURE_SENSOR);
    int tank_level = digitalRead(FLOAT_SWITCH);

    Serial.print("Moisture Level: "); Serial.println(moisture);
    Serial.print("Tank Level: "); Serial.println(tank_level == HIGH ? "HIGH" : "LOW");

    // Display moisture status
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOISTURE: ");
    if (moisture > MOISTURE_THRESHOLD) {
        lcd.print("HIGH");
        digitalWrite(SOIL_STATUS_PIN, HIGH);
    } else {
        lcd.print("LOW ");
        digitalWrite(SOIL_STATUS_PIN, LOW);
    }

    // Display tank status
    lcd.setCursor(0, 1);
    lcd.print("TANK: ");
    if (tank_level == HIGH) {
        lcd.print("FULL ");
        digitalWrite(TANK_STATUS_PIN, LOW);
    } else {
        lcd.print("EMPTY");
        digitalWrite(TANK_STATUS_PIN, HIGH);
    }

    // Control motor
    if (moisture < MOISTURE_THRESHOLD && tank_level == HIGH) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("IRRIGATING...");
        lcd.setCursor(0, 1);
        lcd.print("MOTOR ON");

        digitalWrite(MOTOR_PIN, HIGH);

        // Run motor until soil is sufficiently moist or tank is empty
        while (analogRead(MOISTURE_SENSOR) < MOISTURE_THRESHOLD && digitalRead(FLOAT_SWITCH) == HIGH) {
            delay(500);
        }

        // Turn motor off after irrigation
        digitalWrite(MOTOR_PIN, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Irrigation Done");
        lcd.setCursor(0, 1);
        lcd.print("MOTOR OFF");
        delay(3000);
    } else {
        digitalWrite(MOTOR_PIN, LOW); // Ensure motor is off if not needed
    }

    delay(500);
}
