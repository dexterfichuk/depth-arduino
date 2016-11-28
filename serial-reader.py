from time import sleep
import serial
import httplib
import urllib2

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
    sleep(.1) # Delay for one tenth of a second
    if counter == 255:
        counter = 32