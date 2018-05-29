#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
SoftwareSerial softSerial(2, 3);
void readData( void *pvParameters );
void Blink( void *pvParameters );
void vTask1( void *pvParameters );

void setup() {
  Serial.begin(57600);
  Serial.setTimeout(20);
  softSerial.begin(57600);
  softSerial.setTimeout(20);

  xTaskCreate(readData, (const portCHAR *)"readData", 512, NULL, 3, NULL );
  xTaskCreate(Blink, (const portCHAR *)"Blink", 128, NULL, 2 ,  NULL );
  xTaskCreate(vTask1, (const portCHAR *)"vTask1", 64, NULL, 0 ,  NULL );
}

void loop()
{

}


void Blink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  pinMode(13, OUTPUT);
  for (;;) {
    digitalWrite(13, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS );
    digitalWrite(13, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS );
//    vTask_Test("Blink");
    Serial.print("Blink: ");
    Serial.print(uxTaskGetStackHighWaterMark(NULL));
  }
}

void readData(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) { // A Task shall never return or exit.
    byte buff[255];
    int len = 0;
    if (softSerial.available()) {
      len = softSerial.readBytes(buff, 255);
      Serial.println("----------------------------");
      for (int i = 0; i < len; i++) {
        Serial.print("buff[");
        Serial.print(i);
        Serial.print("] = 0x");
        Serial.println(buff[i], HEX);
      }
      Serial.println("----------------------------");
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
//    vTask_Test("readData");   
    Serial.print("readData: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
  }
}

void vTask1( void * pvParameters )
{
  (void) pvParameters;
  for ( ;; )
  {
    /* Call any function. */
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    Serial.print("Blink: ");
    Serial.print(uxTaskGetStackHighWaterMark(Blink));
    Serial.print("\t");
    Serial.print("readData: ");
    Serial.println(uxTaskGetStackHighWaterMark(readData));
  }
}

void vTask_Test(String Name)
{
  UBaseType_t uxHighWaterMark;
  uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  Serial.print(Name);
  Serial.print(" : ");
  Serial.println(uxHighWaterMark);
}
