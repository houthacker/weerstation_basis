#include "Logger.h"

#include <HardwareSerial.h>

namespace weermeten {

static const char* levels[5] = {
    "CRIT",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"
};

void wm_log(const char* file, int32_t line, LogLevel level, const __FlashStringHelper* msg) {
    Serial.print(F("["));
    Serial.print(levels[static_cast<uint32_t>(level)]);
    Serial.print(F("]"));
    Serial.print(file);
    Serial.print(F(":"));
    Serial.print(line);
    Serial.print(F(" "));
    Serial.println(msg);
}

void wm_log(const char* file, int32_t line, LogLevel level, const char* msg) {
    Serial.print(F("["));
    Serial.print(levels[static_cast<uint32_t>(level)]);
    Serial.print(F("]"));
    Serial.print(file);
    Serial.print(F(":"));
    Serial.print(line);
    Serial.print(F(" "));
    Serial.println(msg);
}

}