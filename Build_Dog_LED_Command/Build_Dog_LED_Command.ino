#include <SoftwareSerial.h>
SoftwareSerial softSerail(2, 3);
//              $    D    >   mis   Len   data              CS
//byte test[] = {'$', 'D', '>', 0x00, 0x00, 0x00};
//byte test[] = {'$', 'D', '>', 0x01, 0x03, 0xFF, 0x00, 0x00, 0x00};
//byte test[] = {'$', 'D', '>', 0x01, 0x03,  0x00,0xFF, 0x00, 0x00};
//byte test[] = {'$', 'D', '>', 0x01, 0x03, 0x00, 0x00, 0xFF, 0x00};
//byte test[] = {'$', 'D', '>', 0x02, 0x04, 0xFF, 0xFF, 0xFF, 0x01, 0xFA, '$', 'D', '>', 0x02, 0x04, 0xFF, 0xFF, 0xFF, 0x01, 0xFA};
//byte test[] = {'$', 'D', '>', 0x02, 0x04, 0xFF, 0xFF, 0xFF, 0x01, 0xFA};
//byte test[] = {'$', 'D', '>', 0x03, 0x05, 0xFF, 0x00, 0x00,25, 0x01, 0x00};
//byte test[] = {'$', 'D', '>', 0x04, 0x00, 0x00};
//byte test[] = {'$', 'D', '>', 0x05, 0x01, 100, 0x00};
byte test[] = {'A', 'B', 'C', 'D'};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  softSerail.begin(57600);

  byte CS = 0x00;
  for (int i = 4; i < sizeof(test) - 1; i++) {
    CS = CS ^ test[i];
  }
  test[sizeof(test) - 1] = CS;
  Serial.write(test, sizeof(test));
//  Serial.println();
//  for (int i = 0; i < sizeof(test); i++) {
//    Serial.print("0x");
//    Serial.print(test[i], HEX);
//    Serial.print(" ");
//  }
}

void loop() {

}
