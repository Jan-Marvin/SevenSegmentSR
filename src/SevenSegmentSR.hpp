static const long powersOf10[] = {
  1,
  10,
  100,
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000
};

static const byte CodeMap[] = {
  0b00001000, // #--------
  0b00000100, // -#-------
  0b00000010, // --#------
  0b00000001, // ---#-----
  0b10000000, // ----#----
  0b01000000, // -----#---
  0b00100000, // ------#--
  0b00010000, // --------#
  0b11000000, //  '0'
  0b11111001, //  '1'
  0b10100100, //  '2'
  0b10110000, //  '3'
  0b10011001, //  '4'
  0b10010010, //  '5'
  0b10000010, //  '6'
  0b11111000, //  '7'
  0b10000000, //  '8'
  0b10010000, //  '9'
  0b10001000, //  'A'
  0b10000011, //  'b'
  0b11000110, //  'C'
  0b10100001, //  'd'
  0b10000110, //  'E'
  0b10001110, //  'F'
  0b11000010, //  'G'
  0b10001001, //  'H'
  0b11001111, //  'I'
  0b11110001, //  'J'
  0b10001001, //  'K'
  0b11000111, //  'L'
  0b11111111, //  'M'  NO DISPLAY
  0b10101011, //  'n'
  0b11000000, //  'O'
  0b10001100, //  'P'
  0b10011000, //  'q'
  0b10101111, //  'r'
  0b10010010, //  'S'
  0b10000111, //  't'
  0b11000001, //  'U'
  0b11000001, //  'V'
  0b11111111, //  'W'  NO DISPLAY
  0b10001001, //  'X'
  0b01101110, //  'y'
  0b10100100, //  'Z'
  0b11111111, //  ' '  BLANK
  0b10111111, //  '-'
  0b11110111  //  '_'
};

// Pointers to data
const byte * const stepCodes = CodeMap;
const byte * const numeralCodes = CodeMap + 8;
const byte * const alphaCodes = CodeMap + 18;

template<uint8_t Size>
SevenSegmentSR<Size>::SevenSegmentSR(const uint8_t serialDataPin, const uint8_t clockPin, const uint8_t latchPin): ShiftRegister74HC595<Size>(serialDataPin, clockPin, latchPin) {
}

template<uint8_t Size>
void SevenSegmentSR<Size>::refresh_display() {
  if (millis() - _prev_time >= _offtime) {
    _prev_time = millis();
    for (int i = 0; i < 8; i++) {
      byte tmp_pinValues[] = { _pinValues[i], _pinValues[i + 8] };
      ShiftRegister74HC595<Size>::setAll(tmp_pinValues);
      ShiftRegister74HC595<Size>::setAllHigh();
    }
  }
}

template<uint8_t Size>
void SevenSegmentSR<Size>::display_char(const char str[], int clock_face) {
  byte digit[8];
  byte steps[8];

  for (byte i = 0; i < 8; i++) {
    char ch = str[i];
    if (ch == ' ') {
      digit[i] = 0b00000000;
      steps[i] = 0b00000000;
    }
    else if (ch >= 'A' && ch <= 'Z') {
      digit[i] = alphaCodes[ch - 'A'];
      steps[i] = stepCodes[i];
    }
    else if (ch >= 'a' && ch <= 'z') {
      digit[i] = alphaCodes[ch - 'a'];
      steps[i] = stepCodes[i];
    }
    else if (ch >= '0' && ch <= '9') {
      digit[i] = numeralCodes[ch - '0'];
      steps[i] = stepCodes[i];
    }
    else {
      digit[i] = 0b00000000;
      steps[i] = 0b00000000;
    }

    if (clock_face == 1) {
      if (i == 2) {
            bitClear(digit[i], 7);
            steps[i] = stepCodes[i];
      }
      else if (i == 7) {
            digit[i] = 0b00011110;
            steps[i] = stepCodes[i];
      }
    }
    else if (clock_face == 2) {
      if (i == 2) {
            bitClear(digit[i], 7);
            steps[i] = stepCodes[i];
      }
        else if (i == 6) {
            bitClear(digit[i], 7);
            steps[i] = stepCodes[i];
      }
        else if (i == 7) {
            digit[i] = 0b00011110;
            steps[i] = stepCodes[i];
      }
    }
    else if (clock_face == 3) {
      if (i == 2) {
            bitClear(digit[i], 7);
            steps[i] = stepCodes[i];
      }
      else if (i == 7) {
            bitClear(digit[i], 7);
            steps[i] = stepCodes[i];
      }
    }
  }
  
  
  for (int i = 0; i < 8; i++) {
      _pinValues[i] = digit[i];
      _pinValues[i + 8] = steps[i];
  }
}

template<uint8_t Size>
void SevenSegmentSR<Size>::set_chars(String chars, int clock_face) {
  while (chars.length() != 8) {
    chars += " ";
  }
  display_char(chars.c_str(), clock_face);
}

template<uint8_t Size>
void SevenSegmentSR<Size>::display_numbers(long number, byte nb_length) {
  byte digit[8];
  byte steps[8];
  byte nr[8];
  for (int i = 0; i < nb_length; i++) {
    nr[i] = (number / powersOf10[nb_length-1-i]) % 10;
  }
  for (byte i = 0; i < nb_length; i++) {
      digit[i] = numeralCodes[nr[i]];
      steps[i] = stepCodes[i];
  }

  for (byte i = nb_length; i < 8; i++) {
      digit[i] = 0b00000000;
      steps[i] = 0b00000000;
  }

  for (int i = 0; i < 8; i++) {
      _pinValues[i] = digit[i];
      _pinValues[i + 8] = steps[i];
  }
}

template<uint8_t Size>
void SevenSegmentSR<Size>::set_numbers(long numbers) {
  if(numbers > 9999999) {
    display_numbers(numbers,8);
  }
  else if (numbers > 999999) {
    display_numbers(numbers,7);
  }
  else if (numbers > 99999) {
    display_numbers(numbers,6);
  }
  else if (numbers > 9999) {
    display_numbers(numbers,5);
  }
  else if (numbers > 999) {
    display_numbers(numbers,4);
  }
  else if (numbers > 99) {
    display_numbers(numbers,3);
  }
  else if (numbers > 9) {
    display_numbers(numbers,2);
  }
  else {
    display_numbers(numbers,1);
  }
}

template<uint8_t Size>
void SevenSegmentSR<Size>::set_Brightness(int brightness) {
  _offtime = brightness;
}
