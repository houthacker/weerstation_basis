#include "NTP.h"

#include <Arduino.h>
#include <cstdlib> // getenv
#include <functional>
#include <TZ.h>

#include <Logger.h>

namespace weermeten {

static const char* get_tz_from_env(const char* or_else)
{
    auto tz = std::getenv("TZ");
    if (tz) {
        return tz;
    }

    LogDebug("Cannot get TZ from env");
    return or_else;
}

NTP::NTP(const char* server1, const char* server2, const char* server3) 
: server1(server1), server2(server2), server3(server3), time_is_set(false), last_time_sync(std::chrono::system_clock::time_point::min()) 
{
    settimeofday_cb([=](bool is_set){
        this->TimeSetCallback(is_set);
    });
}

void NTP::Begin(const char* time_zone, const std::chrono::system_clock::duration sync_interval_seconds)
{
    this->sync_interval_seconds = sync_interval_seconds;
    LogDebug(F("Retrieving time from NTP pool."));
    configTime(time_zone, this->server1, this->server2, this->server3);

    LogDebug(F("Waiting for NTP synchronization."));
    while (!this->time_is_set) {
        delay(100);
    }

    auto now = std::time(nullptr);
    if (now < 8 * 3600 * 2) {
        LogWarning("Time not set after NTP sync. Waiting..");
        while (now < 8 * 3600 * 2) {
            delay(25);
            now = std::time(nullptr);
        }
        LogDebug("Time has been set after NTP sync. Done.");
    }
}

void NTP::Loop()
{
    auto now = std::chrono::system_clock::now();
    if (now - this->last_time_sync >= this->sync_interval_seconds) {
        LogDebug(F("Retrieving time from NTP pool."));

        // If NTP::Begin executed successfully, the time zone was set there. 
        configTime(get_tz_from_env(TZ_Etc_UTC), this->server1, this->server2, this->server3);
    }
}

void NTP::TimeSetCallback(bool is_set)
{
    this->last_time_sync = std::chrono::system_clock::now();
    const std::time_t t = std::time(nullptr);

    LogDebug("NTP synchronized. Current local time is now %s.", std::asctime(std::localtime(&t)));
    this->time_is_set = is_set;
}

}