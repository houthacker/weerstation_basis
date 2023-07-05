#include <cstdint>
#include <chrono>

namespace weermeten {

class NTP {
    private:
        const char* server1;
        const char* server2;
        const char* server3;

        bool time_is_set;
        std::chrono::system_clock::duration sync_interval_seconds;
        std::chrono::system_clock::time_point last_time_sync;
        
    public:
        NTP(const char* server1, const char* server2 = nullptr, const char* server3 = nullptr);
        ~NTP() =default;

        void Begin(const char* time_zone,const std::chrono::system_clock::duration sync_interval_seconds);

        void Loop();

        void TimeSetCallback(bool is_set);
};

}