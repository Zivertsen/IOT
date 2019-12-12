#include "BME280_waether.h"

Adafruit_BME280 bme;

bool BME280_init()
{

    if (!Serial)
        return false; // return false if serial not running
        

    bool status = bme.begin();
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, 
                    Adafruit_BME280::SAMPLING_X1, 
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::FILTER_OFF   );
    if (!status)
    {
        return false; // return false if the serson not found
    }

    return true;
}

float BME280_Temperatur()
{
    return bme.readTemperature();
}


float BME280_Pressure()
{
    return bme.readPressure()/100;
}

float BME280_Humidity()
{
    return bme.readHumidity();
}

void BME280_ForceMeasurement()
{
    bme.takeForcedMeasurement();
}

