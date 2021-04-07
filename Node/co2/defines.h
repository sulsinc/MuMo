#define atmosferic_pressure 101325 //Pa
#include <Wire.h>
#include <RTCZero.h>
#include <LoRaWan.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <Adafruit_TSL2561_U.h>
#include <FlashStorage.h> /* https://github.com/cmaglie/FlashStorage */

// Sensors -----------------------------------------------------------
#define use_global_sensors 0
#if use_global_sensors
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(0x29, 12345);

Adafruit_BME680 bme;
#endif

// Prepare Flash storage slots
FlashStorage(downlink_occured, boolean);
FlashStorage(maximal_temperature, float);
FlashStorage(minimal_temperature, float);
FlashStorage(maximal_humidity, float);
FlashStorage(minimal_humidity, float);
FlashStorage(maximal_illumination, long);
FlashStorage(range_pressure, long);
FlashStorage(sleepCycli, int);

// RTC --------------------------------------------------------------
RTCZero rtc;
long nextAlarmClock;

// Data var --------------------------------------------------------------
unsigned char data[16];
unsigned char downlinkData[10];
char buffer[256];

boolean alarm = false;
