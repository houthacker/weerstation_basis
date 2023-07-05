#ifndef WEERMETEN_LOGGER_H
#define WEERMETEN_LOGGER_H

#include <Arduino.h>
#include <cstdint>

#define LogCritical(fmt, ...) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Critical, fmt, ## __VA_ARGS__)
#define LogError(fmt, ...) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Error, fmt, ## __VA_ARGS__)
#define LogWarning(fmt, ...) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Warning, fmt, ## __VA_ARGS__)
#define LogInfo(fmt, ...) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Info, fmt, ## __VA_ARGS__)

#ifdef WEERMETEN_DEBUG
#define LogDebug(fmt, ...) weermeten::wm_log(__FILE__, __LINE__, weermeten::LogLevel::Debug, fmt, ## __VA_ARGS__)
#else
#define LogDebug(fmt, ...) void(msg)
#endif

namespace weermeten {

enum class LogLevel {
    Critical = 0,
    Error,
    Warning,
    Info,
    Debug
};

void wm_log(const char* file, int32_t line, LogLevel level, const __FlashStringHelper* msg);
void wm_log(const char* file, int32_t line, LogLevel level, const char* fmt, ...);

}

#endif // WEERMETEN_LOGGER_H