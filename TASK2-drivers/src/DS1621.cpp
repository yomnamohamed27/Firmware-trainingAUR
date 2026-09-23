/*
 * DS1621 - library for DS1621 digital thermometer
 */

#include <DS1621.h>
#include <Wire.h>

#define StartConversion 0xEE
#define StopConversion 0x22
#define GetTemperature 0xAA
#define TemperatureHigh 0xA1
#define TemperatureLow 0xA2
#define Config 0xAC
#define ReadCounter 0xA8
#define ReadSlope 0xA9

//Constructor
TempSense::TempSense(int TempSenseAddress)
{
    address = TempSenseAddress;
    configSettings = 0;
    temperature = 0;
}

/*
 * @brief Checks if chip is done temperature conversion
 */
void TempSense::checkConvert()
{
    updateConfig();
    // Bit 7 (DONE bit) is 1 when conversion is complete
    while ((configSettings & 0x80) == 0)
    {
        delay(10);
        updateConfig();
    }
}

/*
 * @brief Gets temperature value from chip
 */
void TempSense::updateTemp()
{
    // Start conversion
    Wire.beginTransmission(address);
#if ARDUINO >= 100
    Wire.write(StartConversion);
#else
    Wire.send(StartConversion);
#endif
    Wire.endTransmission();

    // Wait until conversion completes
    checkConvert();

    // Read temperature
    Wire.beginTransmission(address);
#if ARDUINO >= 100
    Wire.write(GetTemperature);
#else
    Wire.send(GetTemperature);
#endif
    Wire.endTransmission(false);

    Wire.requestFrom(address, 2);
#if ARDUINO >= 100
    if (Wire.available())
    {
        temperature = Wire.read(); // Sign-extended 8-bit integer
        Wire.read();               // Fractional byte (ignored)
    }
#else
    if (Wire.available())
    {
        temperature = Wire.receive();
        Wire.receive();
    }
#endif
}

/*
 * @brief Gets configuration from chip
 */
void TempSense::updateConfig()
{
    Wire.beginTransmission(address);
#if ARDUINO >= 100
    Wire.write(Config);
#else
    Wire.send(Config);
#endif
    Wire.endTransmission(false);

    Wire.requestFrom(address, 1);
#if ARDUINO >= 100
    if (Wire.available())
    {
        configSettings = Wire.read();
    }
#else
    if (Wire.available())
    {
        configSettings = Wire.receive();
    }
#endif
}

/*
 * @brief Retrieves Temperature in Fahrenheit
 */
int16_t TempSense::getTempF()
{
    updateTemp();
    return ((temperature * 9 / 5) + 32);
}

/*
 * @brief Retrieves Temperature in Celsius
 */
int16_t TempSense::getTempC()
{
    updateTemp();
    return temperature;
}

/*
 * @brief Sets output trigger
 */
void TempSense::setTrigger(int16_t tempTrigger)
{
    Wire.beginTransmission(address);
#if ARDUINO >= 100
    Wire.write(TemperatureHigh);
    Wire.write(tempTrigger);
    Wire.write(0);
#else
    Wire.send(TemperatureHigh);
    Wire.send(tempTrigger);
    Wire.send(0);
#endif
    Wire.endTransmission();
}

/*
 * @brief Sets output reset
 */
void TempSense::setReset(int16_t tempReset)
{
    Wire.beginTransmission(address);
#if ARDUINO >= 100
    Wire.write(TemperatureLow);
    Wire.write(tempReset);
    Wire.write(0);
#else
    Wire.send(TemperatureLow);
    Wire.send(tempReset);
    Wire.send(0);
#endif
    Wire.endTransmission();
}

/*
 * @brief Retrieves Configuration settings
 */
int TempSense::getConfig()
{
    updateConfig(); // Correction: calling updateConfig() instead of recursive getConfig()
    return configSettings;
}

status TempSense::write_register(uint8_t reg, uint8_t *data, uint8_t len)
{
    Wire.beginTransmission(address); // ha7ot el address hena
    Wire.write(reg);
    for (uint8_t i = 0; i < len; i++)
        Wire.write(data[i]);

    uint8_t error = Wire.endTransmission();
    return (status)error;
}

status TempSense::read_register(uint8_t reg, uint8_t *data, uint8_t len)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    uint8_t error = Wire.endTransmission(false);
    if (error != 0)
        return (status)error;
    uint8_t bytesRead = Wire.requestFrom(address, len);
    if (bytesRead != len)
        return Other;

    for (uint8_t i = 0; i < len; i++)
        data[i] = Wire.read();

    return OK;
}

status TempSense :: start_conv()
{Wire.beginTransmission(address);
 Wire.write(0xEE);
 uint8_t error = Wire.endTransmission();
 return (status)error;   

}

/*status setconig(uint8_t config_value)
{
    uint8_t data[1] = {config_value};
    return Write_register(0xAC, data, 1);
}*/

/*status set_operartion_mode(mode mode)
{
    uint8_t CURRENT = 0;
    status status = Read_register(0xAC, &CURRENT, 1);
    if (status != OK)
        return status;
    if (mode == shot_mode1)
        CURRENT |= 0x01;
    else
        CURRENT &= ~0x01;
    return set_operartion_mode(CURRENT);
}*/