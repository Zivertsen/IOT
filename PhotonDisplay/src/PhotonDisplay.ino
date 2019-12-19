/*
 * Project Weather_station
 * Description: Reading the sensor BMP280 via i2c. 
 * Author: Andreas Sivertsen
 * Date: 3/9-19
 */

#include "math.h"
#include "Wire.h"
#include "MQTT.h"
#include "Display.h"
#include "String.h"
#include "TimeAndSleep.h"

#define SUBMIT_TIMER 60
#define BOOT_TIME 10
#define BACKSLEEPMULTI 2.5

retained static char Dtemp[20] = {0};
retained static char Dpress[20] = {0};
retained static char Dhumi[20] = {0};
retained static char DRain[20] = {0};

static int STemp = 0;
static int SPress = 0;
static int SHumi = 0;
static int SRain = 0;



//byte Server[] = {192,168,0,19};
char Server[] = "test.Mosquitto.org";
uint16_t Port = 1883;

MQTT client(Server , 1883, callback);

void callback(char* topic, byte *payload, unsigned int length)
{
      
  Serial.println("callback funktion");
  if (strcmp("weather/readtemp",topic) == 0)
  {
    memcpy(Dtemp,payload,length);
    Dtemp[length+1] = '\0';
    STemp = 1;
    Serial.println("callback Temp: ");
    Serial.printlnf("Stemp: %d",STemp);
    for (int i = 0; i < 20; i++)
    {
      Serial.print(Dtemp[i]);
    }
    Serial.println();
    
  }
  else if(strcmp("weather/readpress",topic) == 0)
  {
    memcpy(Dpress,payload,length);
    Dpress[length+1] = '\0';
    SPress = 1;
    Serial.println("callback Press: ");
    Serial.printlnf("SPree: %d",SPress);
    for (int i = 0; i < 20; i++)
    {
      Serial.print(Dpress[i]);
    }
    Serial.println();
  }
  else if(strcmp("weather/readhumi",topic) == 0)
  { 
    memcpy(Dhumi,payload,length);
    Dhumi[length+1] = '\0';
    SHumi = 1;
    Serial.println("callback Humi: ");
    Serial.printlnf("SHumi: %d",SHumi);
    for (int i = 0; i < 20; i++)
    {
      Serial.print(Dhumi[i]);
    }
    Serial.println();
  }
  else if(strcmp("weather/readrain",topic) == 0)
  { 
    memcpy(DRain,payload,length);
    DRain[length+1] = '\0';
    SRain = 1;
    
    Serial.println("callback Rain: ");
    Serial.printlnf("SRain: %d",SRain);
    for (int i = 0; i < 20; i++)
    {
      Serial.print(DRain[i]);
    }
    Serial.println();
  }

  
}

// setup() runs once, when the device is first turned on.
void setup() {

  Serial.begin(9600);
  Serial.println("Start");
  SDD1306_Init();
  client.connect("weatherclient");
  

  // publish/subscribe
    if (client.isConnected()) {
      Serial.println("subscribe");
      client.subscribe("weather/readtemp");
      client.subscribe("weather/readpress");
      client.subscribe("weather/readhumi");
      client.subscribe("weather/readrain");
    }
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  
  checktime();
    if (client.isConnected())
      client.loop();



  Serial.println("Main loop");
  SleepResult result = System.sleepResult();

  if(result.wokenUpByRtc())
  {
    client.connect("weatherclient");
    
    // publish/subscribe
    if (client.isConnected()) {
      Serial.println("subscribe");
      client.subscribe("weather/readtemp");
      client.subscribe("weather/readpress");
      client.subscribe("weather/readhumi");
      client.subscribe("weather/readrain");
    }

    delay(20);

    Serial.println("Get weather data");
    client.publish("weather/getweather","1");
    while(true)
    {
      if (client.isConnected())
      {
        client.loop();
      }
      
      if( STemp == 1 && SPress == 1 && SHumi == 1 && SRain == 1)
      {
        STemp = 0;
        SPress = 0;
        SHumi = 0;
        SRain = 0;
        Serial.println("MQTT goToSleep");
        goToSleep(SUBMIT_TIMER,BOOT_TIME);
        break;
      }
      delay(2);
    }
  }
  else if(result.wokenUpByPin())
  {
      Serial.println("Display draw");
      SDD1306_Temp(Dtemp);
      delay(10000);
      SDD1306_Press(Dpress);
      delay(10000);
      SDD1306_Humi(Dhumi);
      delay(10000);
      SDD1306_Rain(DRain);
      delay(10000);
      SDD1306_ClearDisplay();
      
      goToSleep(SUBMIT_TIMER,BOOT_TIME);
  }
  else
  { 
    Serial.println("No RTC or PIN - goToSleep");
    System.sleep(D2, RISING,5);
  }
  

  delay(500);
}


  
