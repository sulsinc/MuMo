#include "sensor_BME680.hpp"
#include "sensor_TSL2561.hpp"
#include "sensor_SCD30.hpp"
#include "com_Message.hpp"
#include "com_Lora.hpp"
#include "board_LoraWan.hpp"
#include "clock_RealTime.hpp"
#include "device_subsoil.hpp"
#include <type_traits>

board::LoraWan my_board;

sensor::BME680 bme680;
sensor::TSL2561 tsl2561;
sensor::SCD30 scd30;

template <typename Ftor>
void each_sensor(Ftor &&ftor)
{
    //Not really a sensor, but we do measure the battery percentage
    ftor(my_board, "Board");

    //Actual sensors
    ftor(bme680, "BME680");
    ftor(tsl2561, "TSL2561");
    ftor(scd30, "SCD30");
}

com::Message message;

com::Lora lora_com;

clock::RealTime rt_clock;

void setup(void) {
  auto setup_sensor = [](auto &sensor, const char *name){
      sensor.setup();
  };
  each_sensor(setup_sensor);

  const auto device_info = device::subsoil();
  lora_com.setup(device_info);

  my_board.led(false);

  rt_clock.setup();

  delay(500);
  //perform first measurement on all sensors
  auto take_measurement = [&](auto &sensor, const char *name){
      if (!sensor.valid())
          return;

      using Sensor = decltype(sensor);
      typename std::remove_reference_t<Sensor>::Data my_data;
      if (!sensor.measure(my_data))
          return;
  };
  each_sensor(take_measurement);
  delay(500);
}

void loop(void) {
    Serial.println("########################## LOOP SEQUENCE #################################");

    lora_com.process();

    for (unsigned int i = 0; i < 10 ; i++)
    {
#if 0
        my_board.sleep(rt_clock);//sleep for one minute
#else
        delay(1000);
#endif

        auto take_measurement = [&](auto &sensor, const char *name){
            if (!sensor.valid())
                return;

            Serial.print(name); Serial.println(" is valid");

            using Sensor = decltype(sensor);
            typename std::remove_reference_t<Sensor>::Data my_data;
            if (!sensor.measure(my_data))
                return;

            message.set(my_data);
        };
        each_sensor(take_measurement);

        if (message.valid())
        {
            message.print();
            lora_com.set_message(&message);
        }
    }
}
