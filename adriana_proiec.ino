#include <Arduino_FreeRTOS.h>
#include <semphr.h>
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;

SemaphoreHandle_t xBinarySemaphore;

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(15, OUTPUT);

  xBinarySemaphore = xSemaphoreCreateBinary();
  xTaskCreate(TaskLED1,  "Led", 128, NULL, 1, &TaskHandle_1 );
  xTaskCreate(TaskLED2,  "Led", 128, NULL, 1, &TaskHandle_2 );
  xTaskCreate(TaskLED3,  "Led", 128, NULL, 1, &TaskHandle_3 );
  xTaskCreate(TaskLDR1,  "Led", 128, NULL, 1, NULL);
  xSemaphoreGive(xBinarySemaphore);
}

void TaskLED1(void *pvParameters)
{
  (void) pvParameters;

  while (1) {
    digitalWrite(3, HIGH);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    digitalWrite(3, LOW);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    Serial.println("1");
  }
}

void TaskLED2(void *pvParameters)
{
  (void) pvParameters;

  while (1) {
    digitalWrite(7, HIGH);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    digitalWrite(7, LOW);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    Serial.println("2");
  }
}

void TaskLED3(void *pvParameters)
{
  (void) pvParameters;

  while (1) {
    digitalWrite(11, HIGH);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    digitalWrite(11, LOW);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    Serial.println("3");
  }
}
void TaskLDR1(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  while (1)
  {
    int sensorValue = analogRead(A0);
    xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);

    vTaskDelay(50 / portTICK_PERIOD_MS);
    if (sensorValue >= 100) {
      digitalWrite(15, LOW);
      vTaskResume(TaskHandle_1);
    } else {
      Serial.print("4");
      digitalWrite(15, HIGH);
      vTaskSuspend(TaskHandle_1);
    }
    xSemaphoreGive(xBinarySemaphore);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void loop() {}
