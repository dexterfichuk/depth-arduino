from time import sleep
import serial
import httplib

ser = serial.Serial('/dev/tty.usbmodem1d11', 9600) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish
while True:
     counter +=1
     ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino
     message = ser.readline()
     print message # Read the newest output from the Arduino
     
     def printText(txt):
    lines = txt.split('\n')
    for line in lines:
        print line.strip()

        httpServ = httplib.HTTPConnection("159,203,4,227", 80)
        httpServ.connect()

        # quote = "test"
        httpServ.request('POST', '/depth/add.php?', 'key=Saucy&%s' % message)

        response = httpServ.getresponse()
        if response.status == httplib.OK:
            print "Output from CGI request"
            printText (response.read())

        httpServ.close()

     sleep(.1) # Delay for one tenth of a second
     if counter == 255:
     counter = 32