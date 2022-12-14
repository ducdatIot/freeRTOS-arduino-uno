#include <Arduino.h>

#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>

unsigned int time_handles = 0;

QueueHandle_t xQueue;

void VSendTask(void *pvParameters);
void VReceiveTask(void *pvParameters);

void setup()
{
  Serial.begin(9600);

  Serial.println(F("In Setup function"));

  xQueue = xQueueCreate(15, sizeof(int));

  if (xQueue == NULL)
    Serial.println("Error create queue");
  else
  {
    Serial.println("Create queue successful");

    xTaskCreate(VSendTask, "Sender1", 240, NULL, 1, NULL);
    xTaskCreate(VReceiveTask, "Receiver", 240, NULL, 1, NULL);

    vTaskStartScheduler();
  }

  time_handles = millis();
}

void loop()
{

  if (millis() - time_handles >= 100) // call with period 100 ms
  {
    Serial.println("infinite loop");

    time_handles = millis();
  }
}

void VSendTask(void *pvParameters)
{
  unsigned int  lValueToSend = 0;

  for (;;)
  {
    xQueueSend(xQueue, &lValueToSend, portMAX_DELAY);
    Serial.print("Sent = ");
    Serial.println(lValueToSend);

    lValueToSend++;
    if (lValueToSend >= 1000)
      lValueToSend = 0;

    vTaskDelay(100/portTICK_PERIOD_MS);

    // taskYIELD();
  }
}
void VReceiveTask(void *pvParameters)
{
  unsigned int  lReceivedValue = 0;

  for (;;)
  {
    if (xQueueReceive(xQueue, &lReceivedValue, portMAX_DELAY) == pdPASS)
    {
      Serial.print("Received = ");
      Serial.println(lReceivedValue);
    }

    vTaskDelay(110/portTICK_PERIOD_MS);

    // taskYIELD();
  }
}