#ifndef WEERMETEN_LOGGER_H
#define WEERMETEN_LOGGER_H

#include <Arduino.h>
#include <cstdint>

#define LogCritical(msg) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Critical, (msg))
#define LogError(msg) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Error, (msg))
#define LogWarning(msg) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Warning, (msg))
#define LogInfo(msg) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Info, (msg))

#ifdef WEERMETEN_DEBUG
#define LogDebug(msg) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Debug, (msg))
#else
#define LogDebug(msg) (void)(msg)
#endif

#define Log(msg) Serial.print(msg)
#define Logln(msg) Serial.println(msg)

namespace weermeten {

enum class LogLevel {
    Critical = 0,
    Error,
    Warning,
    Info,
    Debug
};

void wm_log(const char* file, int32_t line, LogLevel level, const __FlashStringHelper* msg);
void wm_log(const char* file, int32_t line, LogLevel level, const char* msg);

}

#endif // WEERMETEN_LOGGER_H