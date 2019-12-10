

def DataStorage (temp, humi, press, rain ,time):

    with open('weahter_file.txt','r') as f:
        Wlist = f.readlines()

    with open('weather_file.txt','w') as f:
        #looking for the lenght of the list 
        #if over 4320 (30 days of measurements)
        #remove the oldes measurement
       
        if int (len(Wlist)) >= 4320:
            Wlist.remove(Wlist[0])
        
        #write all the old measurement to the file 
        for item in Wlist:
            f.write("%s" % item)
        
        #write the new measurement to the file
        f.write(time.strftime("%Y-%m-%d %H:%M:%S") + ',' + str(temp) + ',' + str(humi) + ',' + str(press) + str(rain) + '\n')
        

      