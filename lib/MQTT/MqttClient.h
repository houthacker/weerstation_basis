#include <PubSubClient.h>

namespace weermeten {

class MqttClient {
    private:
        PubSubClient& client;
    public:
        MqttClient(PubSubClient& client);
        ~MqttClient();

        bool Connect();

        void Loop();

        void OnMessage(char* topic, uint8_t* payload, unsigned int length);

};

}