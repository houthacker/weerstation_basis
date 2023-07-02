#ifndef WEERMETEN_WIFI_CONNECTION_H
#define WEERMETEN_WIFI_CONNECTION_H

#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <IPAddress.h>

#include <memory>

namespace weermeten {

static const int RSSI_MAX = -50;
static const int RSSI_MIN = -100;

typedef struct {
    const String ssid;
    const String psk;
} APConfig;

/**
 * Convenience wrapper around the WiFiMulti class from <code>ESP8266WiFi</code>.
 * 
 * @since 0.0.1
 * @author houthacker
 */
class WiFiConnection {
    private:
        const APConfig* config;
        const unsigned int config_size;
        const unsigned int connect_timeout;
        std::unique_ptr<ESP8266WiFiMulti> wifi;
        
    public:
        WiFiConnection(const APConfig* config, const unsigned int config_size, const unsigned int connect_timeout);
        ~WiFiConnection() =default;

        /**
         * @brief Connects to the given SSID using the given PSK.
         * 
         * @returns The connection status.
         */
        wl_status_t Connect();

        /**
         * @returns The SSID of the current connection.
         */
        const String GetSSID();

        /**
         * @returns The current WiFi connection status.
         */
        wl_status_t GetStatus();

        /**
         * @returns The local IP address of the WiFi adapter.
         */
        IPAddress GetLocalAddress();

        /**
         * @returns The signal strength in dBm.
         */
        int GetSignalStrength();

        /**
         * @brief Converts the given signal strength in dBm to a signal quality percentage.
         * 
         * @returns The signal quality.
         */
        int ToSignalQuality(const int strength);
};

}

#endif