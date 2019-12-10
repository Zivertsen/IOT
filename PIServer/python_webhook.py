import urllib3
import json


def webhook(City):
    url = 'http://api.openweathermap.org/data/2.5/weather?q='+City+',dnk&units=metric&appid=b98d0020fa02648017ccac25d35c5c52'
    http = urllib3.PoolManager()
    r = http.request('GET',url)
    json_data = r.data.decode("utf-8") 

    data = json.loads(json_data)

    temp = data['main'].get("temp") 
    pressure = data['main'].get("pressure")
    humiduty = data['main'].get("humidity")

    print("Web temp: " + str(temp))
    print("web press: " + str(pressure))
    print("web humi: " + str(humiduty))
    return temp, pressure, humiduty