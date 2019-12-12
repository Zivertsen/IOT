/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "c:/Studie/IOT/IOT_projekt/PhotonSensor/src/main.ino"


/*
 * Project Weather_station
 * Description: Reading the sensor BMP280 via i2c and read the RainGuage. 
 * 
 * Author: Andreas Sivertsen og Henrik Kjær Jeppesen 
 * Date: 8/12-19
 */

#include "math.h"
#include "application.h"
#include "Wire.h"
#include "Includes/BME280_waether.h"
#include "Includes/RainGuage.h"
#include "Includes/TimeAndSleep.h"
#include "MQTT.h"


void callback(char* topic, byte* payload, unsigned int length);
void setup();
void loop();
#line 20 "c:/Studie/IOT/IOT_projekt/PhotonSensor/src/main.ino"
STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

#define SUBMIT_TIMER 30
#define BOOT_TIME 10
#define BACKSLEEPMULTI 2.5

//byte Server[] = {192,168,0,19};
char Server[] = "test.Mosquitto.org";
uint16_t Port = 1883;
int reProgram = D7; //Use to block program form running
int sensorPower = D2;
retained int rain_count = 0;


MQTT client(Server , 1883, callback);
void callback(char* topic, byte* payload, unsigned int length){

}

// setup() runs once, when the device is first turned on.
void setup() {

  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, HIGH);
  init_rainGuage();
  pinMode(reProgram, INPUT);

  Serial.begin(9600);

  client.connect("weatherclient");

  if(!BME280_init())
  {
    Serial.print("BME280 not initialized");
  }
}

// loop() runs over and over again, as quickly as it can execute.
void loop() 
{
  
  if (client.isConnected())
    client.loop();

  int curentTime = 0;
  char temp[10] = {0};
  char press[10] = {0};
  char humi[10] = {0};
  char rain[10] = {0};

  BME280_ForceMeasurement();

  delay(20);

  float temp1 = BME280_Temperatur();
  float humi1 = BME280_Humidity();
  float press1 = BME280_Pressure();


  //Checks to update RTC Clock
  checktime();

  //Sleep Blocker So you can program the device 
  while(digitalRead(reProgram) == 1)
  {
    delay(1000);
  }

  //Brodcast at time or go to sleep or wait
  curentTime = Time.second()%SUBMIT_TIMER;
  if (curentTime == 0)
  {
    // Get rain guage counct
    rain_count += get_rainGuageCount();

    sprintf(temp, "%.2f", temp1);
    sprintf(humi,"%.2f", humi1);
    sprintf(press, "%.2f", press1);
    sprintf(rain,"%d", rain_count);

    client.publish("weather/Temperatur",temp);
    delay(5);
    client.publish("weather/Tryk",press);
    delay(5);
    client.publish("weather/Fugtighed",humi);
    delay(5);
    client.publish("weather/Rainfall", rain);

    //demo funktion
    Serial.printlnf("curent: %d",rain_count);
    Serial.printlnf(Time.timeStr());
    Serial.print("Temp: ");

    for(int i =  0; i < 10; i++)
    {
      Serial.print(temp[i]);
    }
    Serial.println();
    Serial.print("Humi: ");

    for(int i =  0; i < 10; i++)
    {
      Serial.print(humi[i]);
    }
    Serial.println();
    Serial.print("Press: ");

    for(int i =  0; i < 10; i++)
    {
      Serial.print(press[i]);
    }
    Serial.println();

    delay(100);
    rain_count = 0; // resets rain counter befor sleep
    digitalWrite(sensorPower, LOW);
    goToDeepSleep(SUBMIT_TIMER,BOOT_TIME);
  }
  else if (curentTime <= SUBMIT_TIMER-(BOOT_TIME*BACKSLEEPMULTI))
  {
    // Get rain guage counct
    rain_count += get_rainGuageCount();
    digitalWrite(sensorPower, LOW);
    goToDeepSleep(SUBMIT_TIMER,BOOT_TIME);
  }
}


  
    
