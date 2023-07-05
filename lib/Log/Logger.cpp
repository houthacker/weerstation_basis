#include "Logger.h"

#include <HardwareSerial.h>
#include <cstdio>

namespace weermeten {

static const char* levels[5] = {
    "CRIT",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"
};

void wm_log(const char* file, int32_t line, LogLevel level, const __FlashStringHelper* msg) 
{
    Serial.print(F("["));
    Serial.print(levels[static_cast<uint32_t>(level)]);
    Serial.print(F("] "));
    Serial.print(file);
    Serial.print(F(":"));
    Serial.print(line);
    Serial.print(F(" "));
    Serial.println(msg);
}

void wm_log(const char* file, int32_t line, LogLevel level, const char* fmt, ...)
{
    Serial.print(F("["));
    Serial.print(levels[static_cast<uint32_t>(level)]);
    Serial.print(F("] "));
    Serial.print(file);
    Serial.print(F(":"));
    Serial.print(line);
    Serial.print(F(" "));

    va_list arg;
    va_start(arg, fmt);
    char temp[64];
    char* buffer = temp;
    auto len = std::vsnprintf(temp, sizeof(temp), fmt, arg);
    va_end(arg);
    if (len > static_cast<decltype(len)>(sizeof(temp)) - 1) {
        buffer = new (std::nothrow) char[len + 1];
        if (!buffer) {
            return;
        }

        va_start(arg, fmt);
        vsnprintf(buffer, len + 1, fmt, arg);
        va_end(arg);
    }

    Serial.write((const uint8_t*) buffer, len);
    Serial.println();
    
    if (buffer != temp) {
        delete[] buffer;
    }
}

}