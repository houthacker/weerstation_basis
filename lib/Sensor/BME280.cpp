#include "BME280.h"
#include <Arduino.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <Wire.h>

#include <Logger.h>
#include "../../src/Config.h"

namespace weermeten {

static const auto ANNOUNCE_BUF_SIZE = 550;
static const char ANNOUNCE_TEMPLATE[] = R"EOF({
  "device_class": "%s",
  "name": "WM_%u_%s",
  "state_topic": "%s",
  "unit_of_measurement": "%s",
  "value_template": "{{ value_json.%s }}",
  "availability_topic": "%s",
  "unique_id": "weermeten_%u_%s",
  "device": {
    "name": "Weermeten_Basis_%u",
    "identifiers": ["wm_%u"],
    "manufacturer": "Espressif",
    "model": "ESP8266",
    "sw_version": "%s"
  }
})EOF";
static char ANNOUNCE_BUF[ANNOUNCE_BUF_SIZE];

static char* make_announce(const SensorMeta& sensor)
{
    std::memset(ANNOUNCE_BUF, 0, ANNOUNCE_BUF_SIZE);
    std::sprintf(ANNOUNCE_BUF, ANNOUNCE_TEMPLATE, 
        sensor.device_class, 
        WM_CHIP_ID, 
        sensor.device_class, 
        sensor.state_topic, 
        sensor.unit_of_measurement, 
        sensor.device_class, 
        sensor.availability_topic,
        WM_CHIP_ID,
        sensor.device_class,
        WM_CHIP_ID,
        WM_CHIP_ID,
        STRINGIFY(WM_VERSION));
    
    return ANNOUNCE_BUF;
}

BME280::BME280(weermeten::MqttClient& mqtt) : mqtt(mqtt) 
{
    this->temperature_meta = {
        .device_class = "temperature",
        .unit_of_measurement = "°C",
        .discovery_topic = WM_BME280_T_TOPIC "/config",
        .state_topic = WM_SENSOR_STATE_TOPIC,
        .availability_topic = WM_SENSOR_AVAIL_TOPIC
    };
    this->humidity_meta = {
        .device_class = "humidity",
        .unit_of_measurement = "%",
        .discovery_topic = WM_BME280_H_TOPIC "/config",
        .state_topic = WM_SENSOR_STATE_TOPIC,
        .availability_topic = WM_SENSOR_AVAIL_TOPIC
    };
    this->pressure_meta = {
        .device_class = "pressure",
        .unit_of_measurement = "hPa",
        .discovery_topic = WM_BME280_P_TOPIC "/config",
        .state_topic = WM_SENSOR_STATE_TOPIC,
        .availability_topic = WM_SENSOR_AVAIL_TOPIC
    };
}

bool BME280::Begin(int sda, int sdl, uint8_t address)
{
    Wire.begin(sda, sdl);

    return this->bme.begin(address);
}

void BME280::Announce()
{
    // Announce temperature
    auto payload = make_announce(this->temperature_meta);
    if (this->mqtt.Publish(this->temperature_meta.discovery_topic, payload, std::strlen(payload))) {
        LogDebug("Discovery of BME280/temperature sent.");
    } else {
        LogWarning("Discovery of BME280/temperature failed.");
    }

    // Announce humidity
    payload = make_announce(this->humidity_meta);
    if (this->mqtt.Publish(this->humidity_meta.discovery_topic, payload, std::strlen(payload))) {
        LogDebug("Discovery of BME280/humidity sent.");
    } else {
        LogWarning("Discovery of BME280/humidity failed.");
    }

    // Announce pressure
    payload = make_announce(this->pressure_meta);
    if (this->mqtt.Publish(this->pressure_meta.discovery_topic, payload, std::strlen(payload))) {
        LogDebug("Discovery of BME280/pressure sent.");
    } else {
        LogWarning("Discovery of BME280/pressure failed.");
    }
}

void BME280::PrintDetails()
{
    this->bme.getTemperatureSensor()->printSensorDetails();
    this->bme.getPressureSensor()->printSensorDetails();
    this->bme.getHumiditySensor()->printSensorDetails();
}

void BME280::Loop()
{
    sensors_event_t humidity_event, temperature_event, pressure_event;
    this->bme.getHumiditySensor()->getEvent(&humidity_event);
    this->bme.getTemperatureSensor()->getEvent(&temperature_event);
    this->bme.getPressureSensor()->getEvent(&pressure_event);

    Log(F("Humidity = "));
    Log(humidity_event.relative_humidity);
    Logln(" %");

    Log(F("Temperature = "));
    Log(temperature_event.temperature);
    Logln(" *C");

    Log(F("Pressure = "));
    Log(pressure_event.pressure);
    Logln(" hPa");
    Logln();
}
}