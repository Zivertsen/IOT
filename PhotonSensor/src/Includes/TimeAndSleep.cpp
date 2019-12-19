
#include "TimeAndSleep.h"
#include "Particle.h"
#include "math.h"


#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)

time_t lastSyncTimestamp;
int currentTime;
int timeAtSleep;

void checktime()
{
// Control the time
unsigned long lastSync = Particle.timeSyncedLast(lastSyncTimestamp);
if (millis() - lastSync > ONE_DAY_MILLIS) 
{
    unsigned long cur = millis();
    Serial.printlnf("Time was last synchronized %lu milliseconds ago", millis() - lastSync);
    if (lastSyncTimestamp > 0)
    {
        Serial.print("Time received from Particle Device Cloud was: ");
        Serial.println(Time.timeStr(lastSyncTimestamp));
    }
    // Request time synchronization from Particle Device Cloud
    Particle.syncTime();
    // Wait until Photon receives time from Particle Device Cloud (or connection to Particle Device Cloud is lost)
    waitUntil(Particle.syncTimeDone);
    // Check if synchronized successfully
    if (Particle.timeSyncedLast() >= cur)
        {
            // Print current time
            Serial.println(Time.timeStr());
        }
    }
}

void goToDeepSleep(int timer,int bTime)
{
    timeAtSleep = (Time.second()%timer);
    while(true)
    {
        currentTime = (Time.second()%timer);
        if (currentTime != timeAtSleep)
        {
            
            unsigned int sleepTime = abs(timer-currentTime-bTime);
            Serial.printlnf("Time: %d",sleepTime);

            System.sleep(SLEEP_MODE_DEEP,sleepTime);
        }  
    }  
}