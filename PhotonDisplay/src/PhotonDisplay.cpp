/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "application.h"
#line 1 "c:/Studie/IOT/IOT_projekt/PhotonDisplay/src/PhotonDisplay.ino"
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

void callback(char* topic, byte *payload, unsigned int length);
void setup();
void loop();
#line 14 "c:/Studie/IOT/IOT_projekt/PhotonDisplay/src/PhotonDisplay.ino"
#define SUBMIT_TIMER 30
#define BOOT_TIME 10
#define BACKSLEEPMULTI 2.5

static char Dtemp[20] = {0};
static char Dpress[20] = {0};
static char Dhumi[20] = {0};
static char DRain[20] = {0};

static int STemp = 0;
static int SPress = 0;
static int SHumi = 0;
static int SRain = 0;

byte Server[] = {192,168,0,19};
//char Server[] = "test.Mosquitto.org";
uint16_t Port = 1883;

MQTT client(Server , 1883, callback);

void callback(char* topic, byte *payload, unsigned int length){
      
    
  if (strcmp("weather/readtemp",topic) == 0)
  {
    memcpy(Dtemp,payload,length);
    Dtemp[length+1] = '\0';
    STemp = 1;
    Serial.println("callback Temp: ");
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

  client.publish("weather/getweather","1");
  
 
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
    if (client.isConnected())
      client.loop();

    
  if( STemp == 1 && SPress == 1 && SHumi == 1 && SRain == 1)
  {
    Serial.println("Display draw");
    delay(10);
    SDD1306_Temp(Dtemp);
    delay(10000);
    SDD1306_Press(Dpress);
    delay(10000);
    SDD1306_Humi(Dhumi);
    delay(10000);
    SDD1306_Rain(DRain);
    delay(10000);
    SDD1306_ClearDisplay();
    STemp = 0;
    SPress = 0;
    SHumi = 0;
    SRain = 0;
    System.sleep(SLEEP_MODE_DEEP);
  }
  
  delay(500);
}


  
