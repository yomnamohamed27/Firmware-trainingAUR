#include <Arduino.h>
#include <DS1621.h>
#include <Wire.h>

/* I2C add = 0x48
config register = 0xAC
cont mode = 0x00
*/

TempSense sensor(0x48);
uint8_t config = 0x00;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    Serial.println("STARTING");
    status STATUS1 = sensor.write_register(0xAC, &config, 1);
    if (STATUS1 == OK)
        Serial.println("SUCCESSFUL!!!");
    else
        Serial.println("FAILED!!!");

    uint8_t DONE = 0;
    uint8_t ERROR = 0;
    uint8_t temp[2];
    float TEMP;

    status STATUS2 = sensor.start_conv();
    if (STATUS2 == OK)
    {
        Serial.println("SUCCESSFUL!!!");

        status STATUS3 = sensor.read_register(0xAC, &config, 1);
        if (STATUS3 == OK)
            Serial.println("SUCCESSFUL!!!");
        else
        {
            Serial.println("FAILED!!!");
            ERROR = 1;
        }

        while (DONE != 1 && ERROR == 0)
        {
            if (config & (1 << 7))
            {
                DONE = 1;
                break;
            }
            else
            {
                delay(100);
                STATUS3 = sensor.read_register(0xAC, &config, 1);
                if (STATUS3 == OK)
                    Serial.println("SUCCESSFUL!!!");
                else
                {
                    Serial.println("FAILED!!!");
                    ERROR = 1;
                    break;
                }
            }
        }
        if (ERROR == 0)
        {
            status STATUS4 = sensor.read_register(0xAA, temp, 2);

            if (STATUS4 == OK)
            {
                Serial.println("SUCCESSFUL!!!");
                if (temp[1] & (1 << 7))
                {
                    TEMP = temp[0] + 0.5;
                }
                else
                    TEMP = temp[0];
                Serial.print(TEMP);
                Serial.println(" C`");
            }
            else
                Serial.println("FAILED!!!");
        }
    }
    else
        Serial.println("FAILED!!!");
}
    void loop()
    {
    }
