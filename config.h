#ifndef CONFIG_H
#define CONFIG_H

#define DHT11_PIN 2  // Pin for DHT11 Sensor

// Relay Pins
const int RELAY_FUN = 8;
const int RELAY_HEATER = 9;
const int RELAY_HUMIDIFIER = 10;
const int RELAY_DEHUMIDIFIER = 11;

// Button Pins
const int OK = A3;
const int UP = A1;
const int DOWN = A2;

// Default Thresholds
int T_threshold = 30;  // Temperature threshold in Celsius
int H_threshold = 60;  // Humidity threshold in Percentage

#endif  // CONFIG_H
