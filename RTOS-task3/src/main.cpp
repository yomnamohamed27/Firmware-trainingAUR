#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

QueueHandle_t MessagesQueue;

void consumer(void *parameter)
{
    int flag;
    char messages[25];
    while (true)
    {
        flag = uxQueueMessagesWaiting(MessagesQueue);
        if (flag)
        {
            xQueueReceive(MessagesQueue, messages, 10);
            Serial.println(messages);
        }
        else
            vTaskDelay(100);
    }
}

void producerA(void *Parameter)
{
    while (true)
    {
        char arr1[25] = "Task one is working";
        xQueueSend(MessagesQueue, arr1, 10);
        vTaskDelay(100);
    }
}

void producerB(void *parameter)
{
    while (true)
    {
        char arr2[25] = "Task two is working";
        xQueueSend(MessagesQueue, arr2, 10);
        vTaskDelay(200);
    }
}

void setup()
{
    Serial.begin(9600);
    MessagesQueue = xQueueCreate(6, sizeof(char[25]));
    xTaskCreate(consumer, "consumer", 128, NULL, 1, NULL);
    xTaskCreate(producerA, "producerA", 128, NULL, 1, NULL);
    xTaskCreate(producerB, "producerB", 128, NULL, 1, NULL);
}

void loop()
{
}
