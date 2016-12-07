from time import sleep
import serial
import httplib
import urllib2
from elasticsearch import Elasticsearch
import urlparse
from datetime import datetime
import time

kIndex = "project_depth_9"

es = Elasticsearch([{'host': '138.197.141.125', 'port': 9200}])

mapping = {
        "mappings": {
            "depth-mappings": {
                "properties": {
                    "location": {
                        "type": "geo_point"
                    }
                }
            }
        }
    }

es.indices.create(index=kIndex, body=mapping)


ser = serial.Serial('/dev/ttyUSB0', 9600) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish
while True:
    counter +=1
    ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino
    message = ser.readline()
    print message # Read the newest output from the Arduino

    urllib2.urlopen("http://159.203.4.227/depth/add.php?key=Saucy&%s" % message).read()

    parsed = urlparse.urlparse("http://159.203.4.227/depth/add.php?key=Saucy&%s" % message)

    print urlparse.parse_qs(parsed.query)['nodeid'][0]
    
    es.index(index=kIndex,
                 doc_type="depth-mappings",
                 body={"nodeid":int(urlparse.parse_qs(parsed.query)['nodeid'][0]),
                       "waterTemp": float(urlparse.parse_qs(parsed.query)['waterTemp'][0]),
                       "airTemp": float(urlparse.parse_qs(parsed.query)['airTemp'][0]),
                       "airHumidity": float(urlparse.parse_qs(parsed.query)['airHumidity'][0]),
                       "pH": float(urlparse.parse_qs(parsed.query)['pH'][0]),
			            "location": { 
                            "lat": 40.722,
                            "lon":-73.989
                            },
                        "timestamp": datetime.now()
                        })
    
    sleep(.1) # Delay for one tenth of a second
    if counter == 255:
        counter = 32
