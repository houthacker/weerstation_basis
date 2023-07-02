#include <Adafruit_BME280.h>
#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <WiFiManager.h>
#include <Wire.h>

Adafruit_BME280 bme;
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();
Adafruit_Sensor *bme_temperature = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();

void setup() {
  Serial.begin(115200);
  Serial.println(F("**** Initializing Weerstation Basis ****"));

  // Detect and configure hardware
  Serial.println(F("Initializing BME280 h/t/p sensor..."));
  Wire.begin(D2, D1);
  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find a valid BME280 sensor at address 0x76, check wiring!"));
    while (1) delay(10);
  } else {
    Serial.println(F("Done. Found BME280 at address 0x76. Sensor details:"));
    bme_temperature->printSensorDetails();
    bme_pressure->printSensorDetails();
    bme_humidity->printSensorDetails();
  }

  // Configure wifi manager
  Serial.println("Initializing WiFi...");
  WiFi.mode(WiFiMode::WIFI_STA);
  
  WiFiManager wifi_manager;
  // test
  wifi_manager.resetSettings();
  wifi_manager.setTimeout(300);

  String ap = "WeerstationBasis-" + String(ESP.getChipId());
  if (!wifi_manager.autoConnect(ap.c_str(), "WeerstationBasis")) {
    Serial.println(F("Could not connect to WiFi within five minutes, rebooting."));
    delay(1000);

    // Try again.
    ESP.reset();
  } else {
    Serial.println(F("Done. WiFi connected."));
  }
}

void loop() {
  sensors_event_t humidity_event, temperature_event, pressure_event;
  bme_humidity->getEvent(&humidity_event);
  bme_temperature->getEvent(&temperature_event);
  bme_pressure->getEvent(&pressure_event);

  Serial.print(F("Humidity = "));
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");

  Serial.print(F("Temperature = "));
  Serial.print(temperature_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.println();
  delay(10000);
}