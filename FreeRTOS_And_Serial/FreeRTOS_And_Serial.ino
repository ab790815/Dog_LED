#include <Arduino_FreeRTOS.h>
void readData( void *pvParameters );

void setup() {
  xTaskCreate(
    readData
    ,  (const portCHAR *)"readData"   // A name just for humans
    ,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
}

void loop()
{

}

void readData(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  Serial.begin(57600);
  Serial.setTimeout(20);
  for (;;) { // A Task shall never return or exit.
    byte buff[255];
    int len = 0;
    if (Serial.available()) {
      len = Serial.readBytes(buff, 255);
      Serial.print("----------------------------\n");
      for (int i = 0; i < len; i++) {
        Serial.print("buff[");
        Serial.print(i);
        Serial.print("] = 0x");
        Serial.println(buff[i], HEX);
      }
      Serial.print("----------------------------\n");
    }
  }
}
