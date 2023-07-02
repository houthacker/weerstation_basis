#include "MqttClient.h"

#include <functional>

#include "../../src/Config.h"
#include "../../src/WeermetenSecrets.h"

namespace weermeten {

MqttClient::MqttClient(PubSubClient& client) : client(client)
{
    this->client.setServer(WM_MQTT_BROKER, WM_MQTT_PORT);

    std::function<void(char*, uint8_t*, unsigned int)> callback 
        = [=](char* topic, uint8_t* payload, unsigned int length) {
            this->OnMessage(topic, payload, length);
        };
    this->client.setCallback(callback);
}

MqttClient::~MqttClient()
{
    if (this->client.connected()) {
        this->client.unsubscribe(WM_STATUS_TOPIC);
    }
}

bool MqttClient::Connect() {
    while (!this->client.connected()) {
        Serial.print(F("Connecting to MQTT broker "));
        if (this->client.connect("", WM_MQTT_USER, WM_MQTT_PASSWORD)) {
            Serial.println("successful.");

            this->client.subscribe(WM_STATUS_TOPIC);
            return true;
        } else {
            Serial.print("failed. State=");
            Serial.print(this->client.state());
            Serial.println(". Will retry in 5 seconds.");
            delay(5000);
        }
    }

    return true;
}

void MqttClient::Loop()
{
    if (!this->client.connected()) {
        this->Connect();
    }

    this->client.loop();
}

void MqttClient::OnMessage(char* topic, uint8_t* payload, unsigned int length)
{
    Serial.print("Mesage received from topic '");
    Serial.print(topic);
    Serial.print("With payload: [");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println("]");
}

}