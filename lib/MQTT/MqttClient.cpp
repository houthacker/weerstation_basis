#include "MqttClient.h"

#include <Arduino.h>
#include <functional>

#include <Logger.h>

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
        for(auto topic : this->auto_unsubscribe) {
            this->client.unsubscribe(topic);
        }
        this->client.disconnect();
    }
}

bool MqttClient::Connect() {
    for (int i = 0; !this->client.connected() && i < 5; i++) {
        LogInfo(F("Connecting to MQTT broker "));
        if (this->client.connect("", WM_MQTT_USER, WM_MQTT_PASSWORD)) {
            LogInfo("MQTT connection successful.");

            this->client.subscribe(WM_STATUS_TOPIC);
            return true;
        } else {
            LogError("MQTT connection failed. State:");
            Log(this->client.state());
            Logln(". Will retry in 5 seconds.");
            delay(5000);
        }
    }

    LogError("Connection to MQTT broker failed; retries exhausted.");
    return false;
}

bool MqttClient::Subscribe(const char* topic, MqttQoS qos, bool auto_unsubscribe)
{
    // PubSubClient does not support qos > 1, so log that and return false if that's the case.
    if (qos > MqttQoS::at_least_once) {
        LogWarning("Cannot subscribe: PubSubClient does not support QoS > 1");
        return false;
    }

    if (this->client.subscribe(topic, static_cast<uint8_t>(qos))) {
        if (auto_unsubscribe) {
            this->auto_unsubscribe.insert(topic);
        }
    }

    return true;
}

bool MqttClient::Unsubscribe(const char* topic) 
{
    if (this->client.unsubscribe(topic)) {
        if (auto it = this->auto_unsubscribe.find(topic); it != this->auto_unsubscribe.end()) {
            this->auto_unsubscribe.erase(it);
        }

        return true;
    }

    return false;
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
    Log("Message received from topic '");
    Log(topic);
    Log("' with payload: '");
    for (unsigned int i = 0; i < length; i++) {
        Log((char)payload[i]);
    }
    Logln("'");
}

}