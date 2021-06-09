#pragma once
#include "ShiftRegister74HC595.h"

template<uint8_t Size>
class SevenSegmentSR : public ShiftRegister74HC595<Size> {
  public:
    SevenSegmentSR(const uint8_t serialDataPin, const uint8_t clockPin, const uint8_t latchPin);
    void refresh_display();
    void set_chars(String chars, int clock_face);
    void display_char(const char str[], int clock_face);
    void display_numbers(long number, byte nb_length);
    void set_numbers(long numbers);
    void set_Brightness(int brightness);
  private:
    byte _pinValuesStep[8];
    byte _pinValuesDigit[8];
    byte _pinValues[16];
    unsigned long _prev_time;
    int _offtime;
};

#include "SevenSegmentSR.hpp"