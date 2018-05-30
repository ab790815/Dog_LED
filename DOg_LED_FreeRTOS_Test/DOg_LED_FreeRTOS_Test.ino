#include <Adafruit_NeoPixel.h>
#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
SoftwareSerial softSerial(2, 3);
int delayval = 500; // delay for half a second
byte Mis = 0;
byte R = 0, G = 0, B = 0, Time = 0;
byte R_L = 0;
bool is_find_head = false;
void readData( void *pvParameters );
void Blink( void *pvParameters );
void setLight( void *pvParameters );

#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            6
#define NUMPIXELS      20
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.setTimeout(20);
  softSerial.begin(57600);
  softSerial.setTimeout(20);
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

  xTaskCreate(readData, (const portCHAR *)"readData", 389 +10, NULL, 3, NULL );
//  xTaskCreate(Blink, (const portCHAR *)"Blink", 84 + 20, NULL, 1 ,  NULL );
  xTaskCreate(setLight, (const portCHAR *)"setLight", 97, NULL, 2 ,  NULL );

}

void loop() {}

void Blink(void *pvParameters) {
  (void) pvParameters;
  pinMode(13, OUTPUT);
  for (;;) {
    digitalWrite(13, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS );
    digitalWrite(13, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS );
    Serial.print("Blink: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
  }
}

void readData(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    read_data();
    vTaskDelay(50 / portTICK_PERIOD_MS);
    Serial.print("readData: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
  }
}

void setLight(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    light();
    vTaskDelay(50 / portTICK_PERIOD_MS);
    Serial.print("light: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
  }
}

bool CheckSum(int payload_size, byte data[], int CS) {
  Serial.println("------------------------");
  Serial.print("payload_size = 0x");
  Serial.println(payload_size, HEX);
  byte CheckSum_Value = 0x00;
  CheckSum_Value = CheckSum_Value ^ payload_size;
  for (int i = 0; i < payload_size; i++) {
    Serial.print("data[");
    Serial.print(i);
    Serial.print("] = 0x");
    Serial.print(data[i], HEX);
    Serial.print("\n");
    CheckSum_Value = CheckSum_Value ^ data[i];
  }
  Serial.print("CS_size = 0x");
  Serial.println(CS, HEX);
  Serial.print("CheckSum_Value = 0x");
  Serial.println(CheckSum_Value, HEX);
  Serial.println("------------------------");
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
      if (read_data())
        break;
    }
  } else if (R_L == 1) {
    for (int i = (strip.numPixels() - 1); i >= 0; i--) {
      strip.clear();
      strip.setPixelColor(i, strip.Color(R, G, B));
      strip.show();
      delay(t);
      if (read_data())
        break;
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
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    if (read_data())
      break;
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
  if (softSerial.available()) {
    len = softSerial.readBytes(buff, 255);
    Serial.print("1----------------------------\n");
    for (int i = 0; i < len; i++) {
      Serial.print("buff[");
      Serial.print(i);
      Serial.print("] = 0x");
      Serial.println(buff[i], HEX);
    }
    Serial.print("1----------------------------\n");
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
        Serial.print("i = ");
        Serial.println(i);
        Serial.print("Mis = 0x");
        Serial.println(Mis, HEX);
        Serial.print("CS = 0x");
        Serial.println(CS, HEX);
        Serial.print("payload_size = 0x");
        Serial.println(payload_size, HEX);
        Serial.print("payload = ");
        for (int j = 0 ; j < payload_size; j++) {
          Serial.print("0x");
          Serial.print(payload[j] , HEX);
          Serial.print(" ");
        }
        Serial.print("\n");
        if (CheckSum(payload_size, payload, CS)) {
          Serial.println("True");
          output = true;
          R = G = B = Time = 0;
          switch (Mis) {
            case 0x00:
              R = G = B = 0;
              Serial.println("0");
              EEPROM.write(0, 0);
              break;
            case 0x01:
              R = payload[0];
              G = payload[1];
              B = payload[2];
              Serial.println("1");
              Serial.print("R = 0x");
              Serial.println(R, HEX);
              Serial.print("G = 0x");
              Serial.println(G, HEX);
              Serial.print("B = 0x");
              Serial.println(B, HEX);
              EEPROM.write(0, 1);
              EEPROM.write(1, R);
              EEPROM.write(2, G);
              EEPROM.write(3, B);
              break;
            case 0x02:
              R = payload[0];
              G = payload[1];
              B = payload[2];
              Time = payload[3];
              Serial.println("2");
              EEPROM.write(0, 2);
              EEPROM.write(1, R);
              EEPROM.write(2, G);
              EEPROM.write(3, B);
              EEPROM.write(4, Time);
              break;
            case 0x03:
              R = payload[0];
              G = payload[1];
              B = payload[2];
              Time = payload[3];
              R_L = payload[4];
              Serial.println("3");
              EEPROM.write(0, 3);
              EEPROM.write(1, R);
              EEPROM.write(2, G);
              EEPROM.write(3, B);
              EEPROM.write(4, Time);
              EEPROM.write(5, R_L);
              break;
            case 0x04:
              Serial.println("4");
              EEPROM.write(0, 4);
              break;
            case 0x05:
              Time = payload[0];
              Serial.println("5");
              EEPROM.write(0, 5);
              EEPROM.write(1, Time);
              break;
          }
        } else {
          Serial.println("False");
          output = false;
          int tmp = 0;
          for (int j = i; j < i + payload_size + 5; j++) {
            Serial.print("buff[");
            Serial.print(j);
            Serial.print("] = 0x");
            Serial.println(buff[j], HEX);
            tmp++;
          }
          Serial.print("tmp = ");
          Serial.println(tmp);
        }

        i = i + payload_size + 5;
      }
    }
    if (!is_find_head) {
      Serial.println("Not Find Head!");
      for (int i = 0; i < len; i++) {
        Serial.print("buff[");
        Serial.print(i);
        Serial.print("] = 0x");
        Serial.println(buff[i], HEX);
      }
    }
    Serial.print("\n");
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
