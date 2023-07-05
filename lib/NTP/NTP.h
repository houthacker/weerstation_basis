#include <cstdint>
#include <chrono>

namespace weermeten {

/**
 * @brief NTP synchronization wrapper.
 * 
 * @since 0.0.1
 * @author houthacker
 */
class NTP {
    private:
        const char* server1;
        const char* server2;
        const char* server3;

        bool time_is_set;
        std::chrono::system_clock::duration sync_interval_seconds;
        std::chrono::system_clock::time_point last_time_sync;
        
    public:

        /**
         * @brief Creates a new @c NTP instance with the given pool servers.
         */
        NTP(const char* server1, const char* server2 = nullptr, const char* server3 = nullptr);
        ~NTP();

        /**
         * @brief Synchronizes the system clock with NTP and sets up the synchronization interval.
         * @details This method returns after NTP synchronization has updated the system clock.
         */
        void Begin(const char* time_zone,const std::chrono::system_clock::duration sync_interval_seconds);

        /**
         * @brief Synchronizes the system clock with NTP every @c NTP::sync_interval_seconds. 
         */
        void Loop();

        /**
         * @brief This method is called after the system clock has been set, for example after NTP synchronization.
         * @note This method does nothing if called directly.
         */
        void TimeSetCallback(bool is_set);
};

}