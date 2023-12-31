#ifndef WEERMETEN_BME280_H
#define WEERMETEN_BME280_H

#include <Adafruit_BME280.h>
#include <MqttClient.h>

#include <cstdint>
#include <ctime>

namespace weermeten {

struct SensorMeta {
    const char* device_class;
    const char* unit_of_measurement;
    const char* discovery_topic;
    const char* state_topic;
    const char* availability_topic;
};

/**
 * @brief MQTT-managed BME280 sensor.
 * 
 * @since 0.0.1
 * @author houthacker 
 */
class BME280 {
    private:
        Adafruit_BME280 bme;
        weermeten::MqttClient& mqtt;

        SensorMeta temperature_meta;
        SensorMeta pressure_meta;
        SensorMeta humidity_meta;

        uint32_t send_interval_seconds;
        uint64_t last_send_millis;
        
    public:
        BME280(weermeten::MqttClient& mqtt, uint32_t send_interval_seconds);
        ~BME280() =default;

        /**
         * @brief Initializes this sensor.
         * @returns Whether the initialization succeeded.
         */
        bool Begin(int sda, int sdl, uint8_t address);

        /**
         * @brief Announces the sensor to the backend management interface.
         * @details This describes the sensor to the management interface and announces that is
         * is online now.
         */
        void Announce();

        /**
         * @brief Prints the sensor details to the serial interface. 
         */
        void PrintDetails();

        /**
         * @brief Retrieves the latest measurements and sends them to MQTT.
         */
        void Loop();
};

}

#endif // WEERMETEN_BME280_H