#include <Arduino_FreeRTOS.h>

void Task_A( void *pvParameters );
void Task_B( void *pvParameters );
void Task_C( void *pvParameters );
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  //  xTaskCreate(Task_A, (const portCHAR *)"Task_A", 1038, NULL, 1, NULL);
  //  xTaskCreate(Task_B, (const portCHAR *)"Task_B", 256, NULL, 2, NULL);
  //  xTaskCreate(Task_C, (const portCHAR *)"Task_C", 1054, NULL, 2, NULL);
  xTaskCreate(Task_C, (const portCHAR *)"Task_C", 3, NULL, 2, NULL);
}

void loop() {}

void Task_A(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  UBaseType_t uxHighWaterMark;
  uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  for (;;) {
    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    Serial.print("Task A : ");
    Serial.print(uxHighWaterMark);
    Serial.print("\t");
    Serial.print(portTICK_PERIOD_MS);
    Serial.println();
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void Task_B(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  UBaseType_t uxHighWaterMark;
  uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  for (;;)  {
    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    for (int i = 0; i < 10000; i++);
    Serial.print("Task B : ");
    Serial.print(uxHighWaterMark);
    Serial.print("\t");
    Serial.print(portTICK_PERIOD_MS);
    Serial.println();
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }
}
void Task_C(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  UBaseType_t uxHighWaterMark;
  uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  for (;;)  {
    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    for (int i = 0; i < 10000; i++);
    Serial.print("Task C : ");
    Serial.print(uxHighWaterMark);
    Serial.print("\t");
    Serial.print(portTICK_PERIOD_MS);
    Serial.println();
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }
}
