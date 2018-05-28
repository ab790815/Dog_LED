#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(57600);
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  /////////////////////////////////////////////////////////////////////////////////////////////
}
byte R = 0, G = 0, B = 0, Time = 0;
int Mode = 0;

void loop() {
  // put your main code here, to run repeatedly:
  
  int len = 0;
  if (Serial.available()) {
    byte buff[255];
    len = Serial.readBytes(buff, 255);
    for (int i = 0; i < len; i++) {
      if (buff[i] == '$') {
        if (CheckSum(buff, len, i)) {
          Serial.println("True");
          switch (buff[i + 3]) {
            case 0:
              Mode = 0;
              R = G = B = 0;
              Serial.println("0");
              break;
            case 1:
              Mode = 1;
              R = buff[i + 5];
              G = buff[i + 6];
              B = buff[i + 7];
              Serial.println("1");
              break;
            case 2:
              Mode = 2;
              R = buff[i + 5];
              G = buff[i + 6];
              B = buff[i + 7];
              Time = buff[i + 8];
              Serial.println("2");
              break;
            case 3:
              Mode = 3;
              break;
            case 4:
              Mode = 4;
              break;
            case 5:
              Mode = 5;
              break;
          }
        }
        else {
          Serial.println("False");
          for (int i = 0; i < len; i++) {
            Serial.print("0x");
            Serial.print(buff[i],HEX);
            Serial.print(" ");
          }
          Serial.print("\n");
        }
        break;
      }
    }
  }

  switch (Mode) {
    case 0:
      RGB(0, 0, 0);
//      Serial.println("0");
      break;
    case 1:
      RGB(R, G, B);
//      Serial.println("1");
      break;
    case 2:
      RGB_Flashing(R, G, B, Time);
//      Serial.println("2");
      break;
    case 3:
//      Serial.println("3");
      break;
    case 4:
//      Serial.println("4");
      break;
    case 5:
//      Serial.println("5");
      break;
  }
  delay(50);
}


bool CheckSum(byte data[], int len, int idx) {

  byte CS = 0x00;
  for (int i = idx + 4; i < len - 1; i++) {
    CS = CS ^ data[i];
  }
  if (data[len - 1] == CS)
    return true;
  else
    return false;
}

void RGB(byte R, byte G, byte B) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(R, G, B));
    strip.show();
  }
}

void RGB_Flashing(byte R, byte G, byte B, byte Time) {
  int t = Time * 100;
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(R, G, B));
    strip.show();
  }
  delay(t);
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
  }
  delay(t);
}





















