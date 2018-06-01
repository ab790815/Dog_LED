#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            2
#define NUMPIXELS      20
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long time[2] = {0, 0};

int delayval = 500; // delay for half a second
byte Mis = 0;
byte R = 0, G = 0, B = 0, Time = 0;
byte R_L = 0;
bool is_find_head = false;

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
}

void loop() {
  // put your main code here, to run repeatedly:
  read_data();
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
