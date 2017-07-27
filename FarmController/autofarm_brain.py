import picamera
import time
from PIL import Image
import RPi.GPIO as GPIO
import requests
import string

harvest_pin = 22
move_pin = 25

GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(24, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(harvest_pin, GPIO.OUT) #harvest output
GPIO.setup(move_pin, GPIO.OUT) #move output
brain_uri = 'http://40.112.185.77/AutoFarmApi/api/ActionDetection'

current_state = "TAKE_PICTURE"

GPIO.output(harvest_pin, GPIO.LOW)
GPIO.output(move_pin, GPIO.LOW)

def TakePicture():
	with  picamera.PiCamera() as camera:
	
			camera.brightness = 50
			camera.exposure_mode = 'auto'
			camera.meter_mode = 'average'
			camera.awb_mode = 'auto'
			camera.image_effect = 'none'
		
			camera.color_effects = None
			camera.roation = 90
			camera.hflip = False
			camera.vflip = False
		
			camera.crop = (0.0, 0.0, 1.0, 1.0)
			camera.resolution = (32, 32)
			name = 'image_' + str(int(time.time())) + '.jpg' 
			camera.capture(name)
	return name

def GetAction(imageName): 
        img = Image.open(imageName)
        img = img.rotate(270)
        img.save(imageName)
	print(imageName)
	file = {'file': open(imageName, 'r')}
	response = requests.post(brain_uri, files = {imageName: open(imageName, 'r')})
        print response
	print response.content
	return string.replace(response.content, "\"", "")

while (True):
        GPIO.output(harvest_pin, GPIO.LOW)
        GPIO.output(move_pin, GPIO.LOW)
	time.sleep(1)	
	if (current_state) == "TAKE_PICTURE":
		imageName = TakePicture()
		action = GetAction(imageName)
                		
		current_state = "AWAIT_ACK"	
		
		if (action == "HARVEST"):
			#send signal to GPIO 14 and go and wait for Ack
                        GPIO.output(move_pin, GPIO.LOW)
			GPIO.output(harvest_pin, GPIO.HIGH)
			current_state = "SENDING_HARVEST"
			print("SENDING_HARVEST")
		elif (action == "NO_HARVEST" or action == "NO_PLANT" or action == "NO_ACTION"): 
			#send signal to GPIO 15 and go and wait for Ack
                        GPIO.output(harvest_pin, GPIO.LOW)
			GPIO.output(move_pin, GPIO.HIGH)
			current_state = "SENDING_MOVING"
			print("SENDING_MOVING")
	try: 
		print("WAITING_FOR_ACK")
		GPIO.wait_for_edge(24, GPIO.RISING)
		print("Ack received")
                GPIO.output(harvest_pin, GPIO.LOW)
                GPIO.output(move_pin, GPIO.LOW)
		if (current_state == "SENDING_HARVEST"):
			current_state = "BUSY_HARVESTING"
		elif (current_state == "SENDING_MOVE"):
			current_state = "BUSY_MOVING"	
	except KeyboardInterrupt:	
		print("Wating for Ack interrupted")
		GPIO.cleanup() 
	
	try: 
		print("WAITING_FOR_DONE")	
		GPIO.wait_for_edge(23, GPIO.RISING)
		print("Done received")
		current_state = "TAKE_PICTURE"
		
	except KeyboardInterrut:	
		print("Wating for Done interrupted")
		GPIO.cleanup()
GPIO.cleanup()		

