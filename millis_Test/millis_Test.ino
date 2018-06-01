#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            2
#define NUMPIXELS      20
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500;
unsigned long time[2] = {0, 0};
void setup() {
  pinMode(13, OUTPUT);
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //  Serial.println(millis());

  time[0] = millis();           // 使 time[0] = Arduino開機時間
  if (time[1] != time[0]) {     // 單晶片每毫秒可以循環好幾次，此行程式碼是為了避免重覆運行
    time[1] = time[0];
  }
  LED_13(time[0]);
  LED_0(time[0]);
  LED_1(time[0]);
  LED_2(time[0]);
}
void LED_13_(long time_now) {
  int delay_1 = 2;
  int delay_2 = 1;
  int time_tmp_1 = 1000;

  int time_All = (delay_1 + delay_2) * time_tmp_1;
  if (time_now % 3000 == 0000) {
    digitalWrite(13, HIGH);
  } else if (time_now % 3000 == 2000) {
    digitalWrite(13, LOW);
  }
}
void LED_13(long time_now) {
  int delay_1 = 2;
  int delay_2 = 1;
  int time_tmp_1 = 1000;

  int time_All = (delay_1 + delay_2) * time_tmp_1;
  if (time_now % 6000 == 0000) {
    digitalWrite(13, HIGH);
  } else if (time_now % 6000 == 5000) {
    digitalWrite(13, LOW);
  } else if (time_now % 6000 == 4000) {
    digitalWrite(13, HIGH);
  } else if (time_now % 6000 == 2000) {
    digitalWrite(13, LOW);
  }
}

void LED_0(long time_now) {
  int delay_1 = 2;
  int delay_2 = 1;
  int time_tmp_1 = 1000;
  int time_tmp_2 = 2;

  if (time_now % 1000 == 0) {
    pixels.setPixelColor(0, pixels.Color(0, 150, 0));
    pixels.show();
  }
  else if (time[0] % 1000 == 500) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void LED_1(long time_now) {
  if (time_now % (6000 * 2) == 0) {
    pixels.setPixelColor(1, pixels.Color(150, 0, 0));
    pixels.show();
  }
  else if (time[0] % (6000 * 2) == (3000 * 2)) {
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void LED_2(long time_now) {
  int delay_1 = 1;
  int delay_2 = 1;
  int delay_3 = 2;
  int delay_4 = 1;
  int delay_All = delay_1 + delay_2 + delay_3 + delay_4;
  delay_All *= 1000;

  if (time_now % delay_All == 0) {
    pixels.setPixelColor(2, pixels.Color(0, 0, 150));
    pixels.show();
  }  else if (time_now % delay_All == ((delay_1 + delay_2 + delay_3) * 1000)) {
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.show();
  }  else if (time_now % delay_All ==  ((delay_1 + delay_2) * 1000)) {
    pixels.setPixelColor(2, pixels.Color(0, 0, 150));
    pixels.show();
  }  else if (time_now % delay_All == ((delay_1) * 1000)) {
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.show();
  }
}
