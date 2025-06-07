#pragma once
#define LGFX_USE_V1

#include <LovyanGFX.hpp>

#ifndef SPI_FREQUENCY
#define SPI_FREQUENCY 25000000
#endif

class LGFX_WT_SC01_PLUS : public lgfx::LGFX_Device
{

    lgfx::Panel_ST7796 _panel_instance;
    lgfx::Bus_Parallel8 _bus_instance;
    lgfx::Light_PWM _light_instance;
    lgfx::Touch_FT5x06 _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436

  public:
#ifdef USE_LANDSCAPE
    const uint16_t screenWidth = 480;
    const uint16_t screenHeight = 320;
    const uint8_t offsetRotation = 1;
#else
    const uint16_t screenWidth = 320;
    const uint16_t screenHeight = 480;
    const uint8_t offsetRotation = 0;
#endif

    bool hasButton(void) { return false; }

    LGFX_WT_SC01_PLUS(void)
    {
        {
            auto cfg = _bus_instance.config();
            cfg.freq_write = SPI_FREQUENCY;
#if CONFIG_IDF_TARGET_ESP32S3
            cfg.freq_read = 16000000;
#endif
            cfg.pin_wr = 13;
            cfg.pin_rd = -1;
            cfg.pin_rs = 14;
            cfg.pin_d0 = 11;
            cfg.pin_d1 = 10;
            cfg.pin_d2 = 17;
            cfg.pin_d3 = 9;
            cfg.pin_d4 = 2;
            cfg.pin_d5 = 1;
            cfg.pin_d6 = 46;
            cfg.pin_d7 = 42;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();

            cfg.pin_cs = -1;
            cfg.pin_rst = 41;
            cfg.pin_busy = -1;

#ifdef USE_LANDSCAPE
            cfg.panel_width = screenHeight;
            cfg.panel_height = screenWidth;
#else
            cfg.panel_width = screenWidth;
            cfg.panel_height = screenHeight;
#endif
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = offsetRotation;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = true;
            cfg.invert = true;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;

            _panel_instance.config(cfg);
        }

        {
            auto cfg = _light_instance.config();

            cfg.pin_bl = 45;
            cfg.invert = false;
            cfg.freq = 44100;
            cfg.pwm_channel = 7;

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

        {
            auto cfg = _touch_instance.config();

            cfg.x_min = 0;
            cfg.x_max = screenWidth - 1;
            cfg.y_min = 0;
            cfg.y_max = screenHeight - 1;
            cfg.pin_int = 40;
            cfg.bus_shared = true;
            cfg.offset_rotation = 0;
            // I2C
            cfg.i2c_port = 0;
            cfg.i2c_addr = 0x38;
            cfg.pin_sda = 39;
            cfg.pin_scl = 38;
            cfg.freq = 400000;

            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }

        setPanel(&_panel_instance);
    }
};
