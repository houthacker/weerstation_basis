#include "BME280.h"
#include <Arduino.h>
#include <cmath>
#include <cstdio>
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

static const auto STATE_BUF_SIZE = 100;
static const char STATE_TEMPLATE[] = R"EOF({
    "temperature": %.1f,
    "humidity": %.1f,
    "pressure": %.1f
})EOF";

static char ANNOUNCE_BUF[ANNOUNCE_BUF_SIZE];
static char STATE_BUF[STATE_BUF_SIZE];

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

static char* make_state(const sensors_event_t& temp_event, const sensors_event_t& hum_event, const sensors_event_t& pressure_event)
{
    std::memset(STATE_BUF, 0, STATE_BUF_SIZE);
    std::sprintf(STATE_BUF, STATE_TEMPLATE, temp_event.temperature, hum_event.relative_humidity, pressure_event.pressure);

    return STATE_BUF;
}

BME280::BME280(weermeten::MqttClient& mqtt, uint32_t send_interval_seconds) : mqtt(mqtt), send_interval_seconds(send_interval_seconds), last_send_millis(0)
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
        LogDebug("Announced BME280/temperature sensor.");
    } else {
        LogWarning("Announcing BME280/temperature failed.");
    }

    // Announce humidity
    payload = make_announce(this->humidity_meta);
    if (this->mqtt.Publish(this->humidity_meta.discovery_topic, payload, std::strlen(payload))) {
        LogDebug("Announced BME280/humidity sensor.");
    } else {
        LogWarning("Discovery of BME280/humidity failed.");
    }

    // Announce pressure
    payload = make_announce(this->pressure_meta);
    if (this->mqtt.Publish(this->pressure_meta.discovery_topic, payload, std::strlen(payload))) {
        LogDebug("Announced BME280/pressure sensor.");
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
    auto uptime = millis();
    auto diff_millis = uptime - this->last_send_millis;
    if (diff_millis >= this->send_interval_seconds * 1000) {
        LogDebug("Reached BME280::send_interval_seconds; publishing sensor states.");
        sensors_event_t humidity_event, temperature_event, pressure_event;
        this->bme.getHumiditySensor()->getEvent(&humidity_event);
        this->bme.getTemperatureSensor()->getEvent(&temperature_event);
        this->bme.getPressureSensor()->getEvent(&pressure_event);
        auto message = make_state(temperature_event, humidity_event, pressure_event);
        if (!this->mqtt.Publish(WM_SENSOR_STATE_TOPIC, message)) {
            LogWarning("Could not send current state to MQTT.");
        } else {
            this->last_send_millis = uptime;
        }
    }
}
}