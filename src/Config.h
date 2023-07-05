#ifndef WEERMETEN_CONFIG_H
#define WEERMETEN_CONFIG_H

#ifdef WEERMETEN_DEBUG_ALL
#ifndef WEERMETEN_DEBUG
#define WEERMETEN_DEBUG
#endif
#define CORE_DEBUG_LEVEL 5
#define DEBUG_ESP_CORE 1
#define DEBUG_ESP_HTTP_UPDATE 1
#define DEBUG_ESP_PORT Serial
#endif

// Some required defines are added using compiler options
#ifndef WM_CHIP_ID
#error "Required compiler option WM_CHIP_ID is not defined. Add it to build_flags in platformio.ini or use -DWM_CHIP_ID directly."
#endif
#ifndef WM_VERSION
#error "Required compiler option WM_VERSION is not defined. Add it to build_flags in platformio.ini or use -DWM_VERSION directy."
#endif

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

// MQTT broker configuration
#define WM_MQTT_BROKER "mqtt.weermeten.nl"
#define WM_MQTT_PORT 8883

// Let's Encrypt intermediate certificate. This is used as trust anchor when
// setting up an encrypted connection to the MQTT broker.
const char LE_R3_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw
WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg
RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK
AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP
R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx
sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm
NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg
Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG
/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB
Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA
FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw
AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw
Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB
gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W
PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl
ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz
CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm
lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4
avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2
yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O
yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids
hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+
HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv
MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX
nLRbwHOoq7hHwg==
-----END CERTIFICATE-----
)EOF";

// Synchronize with NTP every x seconds.
#define WM_NTP_UPDATE_FREQUENCY_SECONDS 3600 * 24
#define WMConfigTime() configTime(TZ_Europe_Amsterdam, "pool.ntp.org", "time.nist.gov")

// Generic sensor configuration.
#ifdef WEERMETEN_DEBUG
#define WM_UPDATE_INTERVAL_SECS 10
#else
#define WM_UPDATE_INTERVAL_SECS 300
#endif
#define WM_STATUS_TOPIC "weermeten/status"
#define WM_SENSOR_STATE_TOPIC "weermeten/sensor/wm_" STRINGIFY(WM_CHIP_ID) "/state"
#define WM_SENSOR_AVAIL_TOPIC "weermeten/sensor/wm_" STRINGIFY(WM_CHIP_ID) "/availability"

// BME280 sensor topics.
#define WM_BME280_H_TOPIC "weermeten/sensor/wm_" STRINGIFY(WM_CHIP_ID) "_humidity"
#define WM_BME280_T_TOPIC "weermeten/sensor/wm_" STRINGIFY(WM_CHIP_ID) "_temperature"
#define WM_BME280_P_TOPIC "weermeten/sensor/wm_" STRINGIFY(WM_CHIP_ID) "_pressure"

// The address of the I2C bus to communicate with the BME280 sensor.
#define WM_BME280_I2C_ADDRESS 0x76

#endif // WEERMETEN_CONFIG_H