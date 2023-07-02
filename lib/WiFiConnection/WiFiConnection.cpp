#include "WiFiConnection.h"

namespace weermeten {

WiFiConnection::WiFiConnection(const APConfig* config, const unsigned int config_size, const unsigned int connect_timeout)
    : config(config), config_size(config_size), connect_timeout(connect_timeout), wifi(std::make_unique<ESP8266WiFiMulti>())
{ }

wl_status_t WiFiConnection::Connect()
{
    for (unsigned int i = 0; i < this->config_size; i++) {
        auto ap = this->config[i];
        this->wifi->addAP(ap.ssid.c_str(), ap.psk.c_str());
    }
    WiFi.mode(WiFiMode::WIFI_STA);

    return this->wifi->run(this->connect_timeout);
}

const String WiFiConnection::GetSSID()
{
    return WiFi.SSID();
}

wl_status_t WiFiConnection::GetStatus()
{
    return WiFi.status();
}

IPAddress WiFiConnection::GetLocalAddress()
{
    return WiFi.localIP();
}

int WiFiConnection::GetSignalStrength()
{
    return WiFi.RSSI();
}

int WiFiConnection::ToSignalQuality(const int dBm)
{
    int quality;
    if (dBm <= RSSI_MIN) {
        quality = 0;
    } else if (dBm >= RSSI_MAX) {
        quality = 100;
    } else {
        quality = 2 * (dBm + 100);
    }

    return quality;
}

}