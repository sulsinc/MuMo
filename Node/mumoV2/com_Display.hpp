#ifndef HEADER_com_Display_hpp_ALREADY_INCLUDED
#define HEADER_com_Display_hpp_ALREADY_INCLUDED

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

namespace com { 

    class Display
    {
    public:
        void setup()
        {
            u8g2_.begin();
        }

        template <typename Ftor>
        void show(Ftor &&ftor)
        {
            u8g2_.clearBuffer();                   // clear the internal memory
            u8g2_.setFont(u8g2_font_pressstart2p_8u);   // choose a suitable font

            ftor(u8g2_);

            u8g2_.sendBuffer();
        }

    private:
        U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2_{U8G2_R0, /* reset=*/ U8X8_PIN_NONE};
    };

} 

#endif
