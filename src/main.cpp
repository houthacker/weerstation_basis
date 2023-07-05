#include <Arduino.h>
#include <Config.h>

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <cstdint>
#include <TZ.h>

#include <Logger.h>
#include <NTP.h>
#include <MqttClient.h>
#include <BME280.h>

WiFiClientSecure wifi_client;
X509List trust_anchors;

PubSubClient mqtt_client(wifi_client);
weermeten::MqttClient mqtt(mqtt_client, weermeten::LastWillTestament{
  .topic = WM_SENSOR_AVAIL_TOPIC, 
  .message = "offline",
  .qos = weermeten::MqttQoS::at_least_once
});

weermeten::BME280 bme_sensor(mqtt, WM_UPDATE_INTERVAL_SECS);
weermeten::NTP ntp("pool.ntp.org", "time.nist.gov");

void setup() {
  Serial.begin(115200);
  LogInfo(F("**** Initializing Weerstation Basis ****"));

  // Detect and configure hardware
  LogInfo(F("Initializing BME280 sensor..."));
  if (!bme_sensor.Begin(D2, D1, WM_BME280_I2C_ADDRESS)) {
    LogError(F("Could not find a valid BME280 sensor at address 0x76, check wiring!"));
    while (1) delay(10);
  } else {
    LogInfo(F("Found BME280 at address 0x76. Sensor details:"));
    bme_sensor.PrintDetails();
  }

  // Configure wifi manager
  LogInfo(F("Initializing WiFi..."));
  WiFi.mode(WiFiMode::WIFI_STA);
  
  WiFiManager wifi_manager;

#ifdef WEERMETEN_RESET_CONFIG
  wifi_manager.resetSettings();
#endif
  wifi_manager.setTimeout(300);

  String ap = "WeerstationBasis-" + String(ESP.getChipId());
  if (!wifi_manager.autoConnect(ap.c_str(), "WeerstationBasis")) {
    LogError(F("Could not connect to WiFi within five minutes, rebooting."));
    delay(1000);

    // Try again.
    ESP.reset();
  } else {
    LogInfo(F("WiFi connected."));
  }

  // When we have a WiFi connection, set the clock using NTP.
  // This is required to validate TLS certificates later.
  ntp.Begin(TZ_Europe_Amsterdam, std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::seconds(WM_NTP_UPDATE_FREQUENCY_SECONDS)));

  // Setup TLS trust anchor
  trust_anchors.append(LE_R3_CERT);
  wifi_client.setTrustAnchors(&trust_anchors);

  LogDebug(F("Configuring MQTT"));
  if (mqtt.Connect()) {
    if (mqtt.Subscribe(WM_STATUS_TOPIC)) {
      LogDebug(F("MQTT configuration succeeded."));
    } else {
      LogError(F("MQTT connected, but topic subscriptions failed; rebooting."));
      delay(1000);
      ESP.reset();
    }
  } else {
    LogError(F("Could not connect to MQTT, rebooting."));
    delay(1000);
    ESP.reset();
  }

  // Now that we have setup TLS over WiFi, we can use the MQTT connection to
  // announce the sensors.
  bme_sensor.Announce();

  LogInfo(F("**** Weerstation Basis initialized ****"));
}

void loop() {
  // Synchronizes with NTP every WM_NTP_SYNC_FREQUENCY_SECONDS
  ntp.Loop();

  // Retrieve h/t/p values and report them.
  bme_sensor.Loop();

  // And check for new messages.
  mqtt.Loop();
}