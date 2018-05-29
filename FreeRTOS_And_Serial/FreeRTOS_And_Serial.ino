#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
SoftwareSerial softSerial(2, 3);
void readData( void *pvParameters );
void Blink( void *pvParameters );

void setup() {
//  xTaskCreate(
//    readData
//    ,  (const portCHAR *)"readData"   // A name just for humans
//    ,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
//    ,  NULL
//    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
//    ,  NULL );
  xTaskCreate(
    Blink
    ,  (const portCHAR *)"Blink"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
}

void loop()
{

}


void Blink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  pinMode(13, OUTPUT);
  for (;;) {
    digitalWrite(13,HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS );
    digitalWrite(13,LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS );
  }
}

void readData(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  Serial.begin(57600);
  Serial.setTimeout(20);
  softSerial.begin(57600);
  softSerial.setTimeout(20);
  for (;;) { // A Task shall never return or exit.
    byte buff[255];
    int len = 0;
    if (softSerial.available()) {
      len = softSerial.readBytes(buff, 255);
      Serial.print("----------------------------\n");
      for (int i = 0; i < len; i++) {
        Serial.print("buff[");
        Serial.print(i);
        Serial.print("] = 0x");
        Serial.println(buff[i], HEX);
      }
      Serial.print("----------------------------\n");
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
