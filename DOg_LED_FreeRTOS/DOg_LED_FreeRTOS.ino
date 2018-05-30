#include <Adafruit_NeoPixel.h>
#include <Arduino_FreeRTOS.h>
#include <EEPROM.h>
int delayval = 500; // delay for half a second
byte Mis = 0;
byte R = 0, G = 0, B = 0, Time = 0;
byte R_L = 0;
bool is_find_head = false;
void readData( void *pvParameters );
void setLight( void *pvParameters );

#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            2
#define NUMPIXELS      20
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.setTimeout(20);

  //
  strip.begin(); // This initializes the NeoPixel library.
  //
  Mis = EEPROM.read(0);
  switch (Mis) {
    case 0x00:
      R = G = B = 0;
      break;
    case 0x01:
      R = EEPROM.read(1);
      G = EEPROM.read(2);
      B = EEPROM.read(3);
      break;
    case 0x02:
      R = EEPROM.read(1);
      G = EEPROM.read(2);
      B = EEPROM.read(3);
      Time = EEPROM.read(4);
      break;
    case 0x03:
      R = EEPROM.read(1);
      G = EEPROM.read(2);
      B = EEPROM.read(3);
      Time = EEPROM.read(4);
      R_L = EEPROM.read(5);
      break;
    case 0x04:
      break;
    case 0x05:
      Time = EEPROM.read(1);
      break;
  }

  xTaskCreate(readData, (const portCHAR *)"readData", 389 + 10, NULL, 3, NULL );
  xTaskCreate(setLight, (const portCHAR *)"setLight", 97, NULL, 2 ,  NULL );

}

void loop() {}

void readData(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    read_data();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void setLight(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    light();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

bool CheckSum(int payload_size, byte data[], int CS) {
  byte CheckSum_Value = 0x00;
  CheckSum_Value = CheckSum_Value ^ payload_size;
  for (int i = 0; i < payload_size; i++) {
    CheckSum_Value = CheckSum_Value ^ data[i];
  }
  if (CheckSum_Value == CS)
    return true;
  else
    return false;
}

void RGB(byte R, byte G, byte B) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(R, G, B));
    strip.show();
  }
  delay(100);
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

void RGB_Circle(byte R, byte G, byte B, byte Time, byte R_L) {
  int t = Time * 10;
  if (R_L == 0) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.clear();
      strip.setPixelColor(i, strip.Color(R, G, B));
      strip.show();
      delay(t);
    }
  } else if (R_L == 1) {
    for (int i = (strip.numPixels() - 1); i >= 0; i--) {
      strip.clear();
      strip.setPixelColor(i, strip.Color(R, G, B));
      strip.show();
      delay(t);
    }
  }
}

void rainbow() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels())) & 255));
  }
  strip.show();
  delay(1000);
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    Serial.print("");
//    Serial.println(Mis);
    if (Mis != 5) {
      break;
    }
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait * 10);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

bool read_data() {
  bool output = false;

  byte buff[255];
  int len = 0;
  if (Serial.available()) {
    len = Serial.readBytes(buff, 255);
    for (int i = 0; i < len; i++) {
      if (buff[i] == '$' && buff[i + 1] == 'D' && buff[i + 2] == '>') {
        is_find_head = true;
        Mis = buff[i + 3];
        byte payload_size = buff[i + 4]; // 3
        byte CS = buff[i + payload_size + 5];
        byte payload[payload_size];
        for (int j = 0 ; j < payload_size; j++) {
          payload[j] = buff[i + j + 5];
        }
        if (CheckSum(payload_size, payload, CS)) {
          output = true;
          R = G = B = Time = 0;
          switch (Mis) {
            case 0x00:
              R = G = B = 0;
              EEPROM.write(0, 0);
              Serial.println("read_data:0");
              break;
            case 0x01:
              R = payload[0];
              G = payload[1];
              B = payload[2];
              EEPROM.write(0, 1);
              EEPROM.write(1, R);
              EEPROM.write(2, G);
              EEPROM.write(3, B);
              Serial.println("read_data:1");
              break;
            case 0x02:
              R = payload[0];
              G = payload[1];
              B = payload[2];
              Time = payload[3];
              EEPROM.write(0, 2);
              EEPROM.write(1, R);
              EEPROM.write(2, G);
              EEPROM.write(3, B);
              EEPROM.write(4, Time);
              Serial.println("read_data:2");
              break;
            case 0x03:
              R = payload[0];
              G = payload[1];
              B = payload[2];
              Time = payload[3];
              R_L = payload[4];
              EEPROM.write(0, 3);
              EEPROM.write(1, R);
              EEPROM.write(2, G);
              EEPROM.write(3, B);
              EEPROM.write(4, Time);
              EEPROM.write(5, R_L);
              Serial.println("read_data:3");
              break;
            case 0x04:
              EEPROM.write(0, 4);
              Serial.println("read_data:4");
              break;
            case 0x05:
              Time = payload[0];
              EEPROM.write(0, 5);
              EEPROM.write(1, Time);
              Serial.println("read_data:5");
              break;
          }
        } else {
          output = false;
        }

        i = i + payload_size + 5;
      }
    }
  }
  return output;
}

void light() {
  switch (Mis) {
    case 0x00:
      RGB(0, 0, 0);
      //      Serial.println("0");
      break;
    case 0x01:
      RGB(R, G, B);
      //      Serial.println("1");
      break;
    case 0x02:
      RGB_Flashing(R, G, B, Time);
      //      Serial.println("2");
      break;
    case 0x03:
      RGB_Circle(R, G, B, Time, R_L);
      //      Serial.println("3");
      break;
    case 0x04:
      rainbow();
      //      Serial.println("4");
      break;
    case 0x05:
      rainbowCycle(Time);
      //      Serial.println("5");
      break;
  }
}
