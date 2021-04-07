#ifndef HEADER_board_LoraWan_hpp_ALREADY_INCLUDED
#define HEADER_board_LoraWan_hpp_ALREADY_INCLUDED

namespace board { 
    class LoraWan
    {
    public:
        void setup(unsigned int baudrate = 115200)
        {
            Serial.begin(baudrate);

            // LoRaWan Board hardware settings
            for (unsigned char i = 0; i < 26; i ++) { // Important, set all pins to HIGH to save power during sleeps
                pinMode(i, OUTPUT);
                digitalWrite(i, HIGH);
            }
            pinMode(38, OUTPUT); //Specifically turn pin 38 OUTPUT and HIGH to activate the Grove connectors
            digitalWrite(38, HIGH);
        }

        void led(bool on)
        {
            digitalWrite(LED_BUILTIN, on); //Turn the onboard led off now that we are joined
        }

    private:
    };
} 

#endif
