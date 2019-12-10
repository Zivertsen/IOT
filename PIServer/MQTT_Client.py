#!/usr/bin/python
# -*- coding: utf-8 -*-



#import context  # Ensures paho is in PYTHONPATH
import paho.mqtt.client as mqtt
import time
import python_webhook as pwh
import DataStore as DS 
from datetime import datetime
import DataStore as DS
import python_webhook as WH
import urllib3
import json

#server = "test.mosquitto.org"
server = "192.168.0.19"
port = 1883


def compare_weather(sensor, temp, press, humi, rain):

    if (sensor[0] + 1.0) > float (temp) and (sensor[0] - 1.0) < float (temp):
        Ttemp = temp
    else:
        Ttemp = None
        

    if sensor[1] + 10.0 > float (press) and (sensor[1] - 10.0) < float (press):
        Tpress = press
    else:
        Tpress = None
        

    if sensor[2] + 2.0 > float(humi) and sensor[2] - 2.0 < float(humi):
        Thumi = humi
    else:
        Thumi = None

    TimeNow = datetime.now()
    
    print("TTemp: " + str(Ttemp))
    print("TPress: " + str(Tpress))
    print("THumi: " + str(Thumi))
    main.weatherStemp = temp
    main.weatherSpress = press
    main.weatherShumi = humi
    main.weatherSrain = rain

    DS.DataStorage(Ttemp,Tpress,Thumi, rain, TimeNow)

def on_connect(mqttc, obj, flags, rc):
    print("rc: " + str(rc))


def on_message(mqttc, obj, msg):
    # python way to make static variable 
    compare_weather.status_temp = getattr(compare_weather, 'status_temp', 0)
    compare_weather.status_press = getattr(compare_weather, 'status_press', 0)
    compare_weather.status_humi = getattr(compare_weather, 'status_humi', 0)
    compare_weather.status_rain = getattr(compare_weather, 'status_rain', 0)
    compare_weather.temp = getattr(compare_weather, 'temp', 0)
    compare_weather.press = getattr(compare_weather, 'press', 0)
    compare_weather.humi = getattr(compare_weather, 'humi', 0)
    compare_weather.rain = getattr(compare_weather, 'rain', 0)

    if msg.topic == "weather/Temperatur":
        compare_weather.temp =  msg.payload
        compare_weather.status_temp = 1
        print("Temp: " + compare_weather.temp)
    elif msg.topic == "weather/Tryk":
        compare_weather.press = msg.payload
        compare_weather.status_press = 1
        print("Press: " + compare_weather.press)
    elif msg.topic == "weather/Fugtighed":
        compare_weather.humi = msg.payload
        compare_weather.status_humi = 1
        print("Humi: " + compare_weather.humi)
    elif msg.topic == "weather/Rainfall":
        compare_weather.rain = msg.payload
        compare_weather.status_rain = 1
        print("Rain: " + compare_weather.rain)
    elif msg.topic == "weather/getweather":
        print("getweather stat")
        main.weatherRead = 1
    else:
        compare_weather.temp = 0
        compare_weather.humi = 0
        compare_weather.press = 0
        compare_weather.rain = 0
    
    if (int (compare_weather.status_temp) == 1 and 
    int (compare_weather.status_press) == 1 and 
    int (compare_weather.status_humi) == 1 and 
    int (compare_weather.status_rain == 1)):
        print("compare_weather funktion")
        compare_weather.status_humi = 0
        compare_weather.status_press = 0
        compare_weather.status_temp = 0
        sensor = WH.webhook("Viborg")
        compare_weather(sensor, compare_weather.temp, compare_weather.press, 
        compare_weather.humi, compare_weather.rain)
    

def on_publish(mqttc, obj, mid):
    print("mid: " + str(mid))
    pass


def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def on_log(mqttc, obj, level, string):
    print(string)



# If you want to use a specific client id, use
# mqttc = mqtt.Client("client-id")
# but note that the client id must be unique on the broker. Leaving the client
# id parameter empty will generate a random id for you.
mqttc = mqtt.Client()
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
mqttc.connect(server, port, 60)

mqttc.loop_start()
mqttc.subscribe("weather/Temperatur",qos=1)
mqttc.subscribe("weather/Tryk",qos=1)
mqttc.subscribe("weather/Fugtighed",qos=1)
mqttc.subscribe("weather/Rainfall",qos=1)
mqttc.subscribe("weather/getweather", qos=1)

def main():
    main.weatherRead = getattr(main, 'weatherRead', 0)
    main.weatherStemp = getattr(main, 'weatherStemp', 0)
    main.weatherSpress = getattr(main, 'weatherSpress', 0)
    main.weatherShumi = getattr(main, 'weatherShumi', 0)
    main.weatherSrain = getattr(main, 'weatherSrain', 0)
    while True:
        if main.weatherRead == 1:
            print("STemp: " + str(main.weatherStemp))
            infot = mqttc.publish("weather/readtemp", str(main.weatherStemp) , qos=1)
            infot.wait_for_publish()
            time.sleep(2)
            print("SPress: " + str(main.weatherSpress))
            infot = mqttc.publish("weather/readpress", str(main.weatherSpress) , qos=1)
            infot.wait_for_publish()
            time.sleep(2)
            print("SHumi: " + str(main.weatherShumi))
            infot = mqttc.publish("weather/readhumi", str(main.weatherShumi) , qos=1)
            infot.wait_for_publish()
            time.sleep(2)
            print("SRain: " + str(main.weatherSrain))
            infot = mqttc.publish("weather/readrain", str(main.weatherSrain) , qos=1)
            infot.wait_for_publish()
            main.weatherRead = 0
        
        
if __name__== "__main__":
  main()