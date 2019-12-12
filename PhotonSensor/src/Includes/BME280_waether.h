#include "Adafruit_BME280.h"



#define SEALEVELPRESSURE_HPA (1013.25)

bool BME280_init();

float BME280_Temperatur();
float BME280_Pressure();
float BME280_Humidity();
void BME280_ForceMeasurement();

