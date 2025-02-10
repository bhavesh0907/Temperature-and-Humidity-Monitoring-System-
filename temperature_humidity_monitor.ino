#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dht11.h>

#define DHT11_PIN 2  // Sensor pin
dht11 DHT;

// Button Pins
const int OK = A3;
const int UP = A1;
const int DOWN = A2;

// Relay Pins
const int RELAY_FUN = 8;
const int RELAY_HEATER = 9;
const int RELAY_HUMIDIFIER = 10;
const int RELAY_DEHUMIDIFIER = 11;

int T_threshold = 30;  // Default temperature threshold
int H_threshold = 60;  // Default humidity threshold
bool T_condition = true;
bool H_condition = true;
bool SET = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    pinMode(OK, INPUT_PULLUP);
    pinMode(UP, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);

    pinMode(RELAY_FUN, OUTPUT);
    pinMode(RELAY_HEATER, OUTPUT);
    pinMode(RELAY_HUMIDIFIER, OUTPUT);
    pinMode(RELAY_DEHUMIDIFIER, OUTPUT);

    digitalWrite(RELAY_FUN, LOW);
    digitalWrite(RELAY_HEATER, LOW);
    digitalWrite(RELAY_HUMIDIFIER, LOW);
    digitalWrite(RELAY_DEHUMIDIFIER, LOW);

    lcd.init();
    lcd.backlight();
    Serial.begin(9600);

    lcd.setCursor(0, 0);
    lcd.print("Temp & Humidity");
    lcd.setCursor(0, 1);
    lcd.print("Monitoring");
    delay(3000);
    lcd.clear();
}

void loop() {
    if (!SET) {
        setThresholds();
    }

    int chk = DHT.read(DHT11_PIN);  // Read sensor data
    if (chk != DHTLIB_OK) {
        displayError(chk);
        return;
    }

    Serial.print("Temperature: ");
    Serial.print(DHT.temperature);
    Serial.print("C, Humidity: ");
    Serial.println(DHT.humidity);

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(DHT.temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(DHT.humidity);
    lcd.print("%");

    controlRelays(DHT.temperature, DHT.humidity);
    delay(1000);
}

void setThresholds() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set Temp:");
    lcd.setCursor(0, 1);
    lcd.print(T_threshold);
    lcd.print("C");

    while (T_condition) {
        if (digitalRead(UP) == LOW) {
            T_threshold++;
            lcd.setCursor(0, 1);
            lcd.print(T_threshold);
            lcd.print("C ");
            delay(200);
        }
        if (digitalRead(DOWN) == LOW) {
            T_threshold--;
            lcd.setCursor(0, 1);
            lcd.print(T_threshold);
            lcd.print("C ");
            delay(200);
        }
        if (digitalRead(OK) == LOW) {
            T_condition = false;
            delay(200);
        }
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set Humidity:");
    lcd.setCursor(0, 1);
    lcd.print(H_threshold);
    lcd.print("%");

    while (H_condition) {
        if (digitalRead(UP) == LOW) {
            H_threshold++;
            lcd.setCursor(0, 1);
            lcd.print(H_threshold);
            lcd.print("% ");
            delay(200);
        }
        if (digitalRead(DOWN) == LOW) {
            H_threshold--;
            lcd.setCursor(0, 1);
            lcd.print(H_threshold);
            lcd.print("% ");
            delay(200);
        }
        if (digitalRead(OK) == LOW) {
            H_condition = false;
            delay(200);
        }
    }

    SET = true;
}

void controlRelays(int temp, int hum) {
    digitalWrite(RELAY_FUN, temp > T_threshold ? HIGH : LOW);
    digitalWrite(RELAY_HEATER, temp < T_threshold ? HIGH : LOW);
    digitalWrite(RELAY_HUMIDIFIER, hum < H_threshold ? HIGH : LOW);
    digitalWrite(RELAY_DEHUMIDIFIER, hum > H_threshold ? HIGH : LOW);
}

void displayError(int errorCode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    lcd.setCursor(0, 1);

    switch (errorCode) {
        case DHTLIB_ERROR_CHECKSUM:
            lcd.print("Checksum Error");
            break;
        case DHTLIB_ERROR_TIMEOUT:
            lcd.print("Timeout Error");
            break;
        default:
            lcd.print("Unknown Error");
            break;
    }

    digitalWrite(RELAY_FUN, LOW);
    digitalWrite(RELAY_HEATER, LOW);
    digitalWrite(RELAY_HUMIDIFIER, LOW);
    digitalWrite(RELAY_DEHUMIDIFIER, LOW);
    delay(1000);
}
