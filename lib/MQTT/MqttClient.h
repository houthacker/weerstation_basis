#ifndef WEERMETEN_MQTT_CLIENT_H
#define WEERMETEN_MQTT_CLIENT_H

#include <PubSubClient.h>

#include <cstdint>
#include <unordered_set>

namespace weermeten {

enum class MqttQoS {
    at_most_once = 0,
    at_least_once = 1,
    exactly_once = 2,
};

struct LastWillTestament {
    const char* topic = nullptr;
    const char* message = nullptr;
    const MqttQoS qos = MqttQoS::at_most_once;
    const bool retain = false;
};

class MqttClient {
    private:
        PubSubClient& client;

        std::unordered_set<const char*> auto_unsubscribe;

        const LastWillTestament lwt;

    public:
        MqttClient(PubSubClient& client, const LastWillTestament lwt = {});
        ~MqttClient();

        /**
         * @brief Connects to the configured MQTT broker.
         * @returns A boolean indicating success or failure.
         */
        bool Connect();

        /**
         * @brief Subscribes to the given topic.
         * @details The quality of service 
         * @param topic The topic to subscribe to.
         * @param qos The required quality of service.
         * @param auto_unsubscribe Whether to unsubscribe automatically when the @c MqttClient is destroyed. 
         * 
         * @returns Whether subscribing to the topic succeeded.
         */
        bool Subscribe(const char* topic, MqttQoS qos = MqttQoS::at_most_once, bool auto_unsubscribe = true);

        /**
         * @brief Unsubscribes from the given topic.
         * @returns Whether unsubscribing from the given topic succeeded. 
         */
        bool Unsubscribe(const char* topic);

        /**
         * @brief Publishes the given message.
         * @details This is a shorthand for <code>MqttClient::Publish(topic, payload, strlen(payload))</code>
         * @param topic The topic to send the message to.
         * @param payload The payload to send.
         * 
         * @returns Whether publishing the message succeeded. 
         */
        bool Publish(const char* topic, const char* payload);

        /**
         * @brief Publishes the given message.
         * @param topic The topic to send the message to.
         * @param payload The payload to send.
         * @param length The payload size in bytes.
         * 
         * @returns Whether publishing the message succeeded. 
         */
        bool Publish(const char* topic, const char* payload, uint32_t length);

        /**
         * @brief Checks for new messages and reconnects if necessary.
         * @details If new messages have arrived, @c MqttClient::OnMessage() is called for each message.
         */
        void Loop();

        /**
         * @brief Callback which is called when a new message has arrived. 
         */
        void OnMessage(char* topic, uint8_t* payload, uint32_t length);
};

}

#endif // WEERMETEN_MQTT_CLIENT_H