//############   TTN SETTINGS   #######################################################
#define device_EUI "00BEA0888A7FE90A"
#define application_EUI "70B3D57ED003BB73"
#define app_key "800317A9D6AAD3C045DFFC285592481F"

#define device_address "26013242"
#define network_session_key "2D3311932EA0879115C6B4F7D8B73A8E"
#define app_session_key "456DED8EE204D36B72CF00297CBFA484"

//############   SENSOR SETTINGS   ###################################################
//Sensor thresholds (FlashStorage overwrite with downlink) -----------
#define allow_downlink true
#define base_maximal_temperature 35
#define base_minimal_temperature 8
#define base_maximal_humidity 70
#define base_minimal_humidity 15
#define base_maximal_illumination 420
#define base_range_pressure 22000
#define base_sleepCycli 5

//############   CODE INFORMATION   ###################################################
int Version = 1;

/*- DO NOT EDIT THE CODE BELOW THIS LINE -*/
#include "defines.h"
#include "sensor_BME680.hpp"
#include "sensor_TSL2561.hpp"
#include "sensor_SCD30.hpp"
#include "message_Version1.hpp"
#include "network_Lora.hpp"
#include "board_LoraWan.hpp"

sensor::BME680 bme680;
sensor::TSL2561 tsl2561;
sensor::SCD30 scd30;

message::Version1 version1_message;

network::Lora lora_network;

board::LoraWan my_board;

void setup(void) {
  my_board.setup();

  bme680.setup();
  tsl2561.setup();
  scd30.setup();

  // Set default thresholds to the flash memory if the downlink boolean is false, so no change with downlink value
  if (downlink_occured.read() == false) {
    maximal_temperature.write(base_maximal_temperature);
    minimal_temperature.write(base_minimal_temperature);
    maximal_humidity.write(base_maximal_humidity);
    minimal_humidity.write(base_minimal_humidity);
    maximal_illumination.write(base_maximal_illumination);
    range_pressure.write(base_range_pressure);
    sleepCycli.write(base_sleepCycli);
  }

  lora_network.setup();

  my_board.led(false);

  //RTC zetten
  rtc.begin(false);
  nextAlarmClock = rtc.getEpoch() + 60; // calculate the time of the first alarm (in one minute)
  rtc.setAlarmEpoch(nextAlarmClock);
  rtc.enableAlarm(rtc.MATCH_SS); //check the alarm based on seconds, so we wake up every minute

  delay(500);
  //perform first measurement on all sensors
  check_measurements();
  delay(500);
}

void loop(void) {
  Serial.println("########################## LOOP SEQUENCE #################################");

  lora_network.process();
  /* if (loraSending() && allow_downlink) { //return downlink data received */
    /* update_thresholds(); // change the threshold values in the flash if allowed */
  /* } */

  for (int i = 0; i < sleepCycli.read() ; i++) { // 10 sleep cycli of 60 seconds

#if use_global_sensors
    doSleep(); //sleep for one minute
#else
    delay(1000);
#endif

    data[0] = Version;

    if (bme680.valid())
    {
        Serial.println("BME680 is valid");
    }
    if (tsl2561.valid())
    {
        Serial.println("TSL2561 is valid");
    }
    if (scd30.valid())
    {
        Serial.println("SCD30 is valid");
        sensor::SCD30::Data my_data;
        if (scd30.measure(my_data))
        {
            version1_message.set(my_data);
            version1_message.print();
            alarm = true;
            lora_network.set_message(&version1_message);
        }
    }

#if use_global_sensors
    if (check_measurements()) { //an alarm was raised. Cut the sleepcycle and straight to send!
      alarm = true;
      break;
    } else if (alarm) { //no alarm this time, but in case the last time was; send once more
      alarm = false;
      break;
    }
#endif
  }
} // end of loop
