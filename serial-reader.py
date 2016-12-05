from time import sleep
import serial
import httplib
import urllib2
from elasticsearch import Elasticsearch
import urlparse
from datetime import datetime


es = Elasticsearch([{'host': '138.197.141.125', 'port': 9200}])

ser = serial.Serial('/dev/ttyUSB0', 9600) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish
while True:
    counter +=1
    ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino
    message = ser.readline()
    print message # Read the newest output from the Arduino
    #  GET Attempt 1
    # Test message
    # message = "nodeid=4&waterTemp=38"

    urllib2.urlopen("http://159.203.4.227/depth/add.php?key=Saucy&%s" % message).read()

    parsed = urlparse.urlparse("http://159.203.4.227/depth/add.php?key=Saucy&%s" % message)

    print urlparse.parse_qs(parsed.query)['nodeid'][0]

    es.index(index="project_depth_1",
                 doc_type="depth_test",
                 body={"nodeid": urlparse.parse_qs(parsed.query)['nodeid'][0],
                       "waterTemp": urlparse.parse_qs(parsed.query)['waterTemp'][0],
                       "airTemp": urlparse.parse_qs(parsed.query)['airTemp'][0],
                       "airHumidity": urlparse.parse_qs(parsed.query)['airHumidity'][0],
                       "pH": urlparse.parse_qs(parsed.query)['pH'][0],
			            "location": { 
                            "lat":     40.722,
                            "lon":    -73.989
                            }
                        })
    
    sleep(.1) # Delay for one tenth of a second
    if counter == 255:
        counter = 32