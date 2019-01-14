'''
Non micro stepping, better torque, but jerkyer.
'''
from time import sleep
import RPi.GPIO as GPIO

DIR = 20   # Direction GPIO Pin
STEP = 21  # Step GPIO Pin
CW = 1     # Clockwise Rotation
CCW = 0    # Counterclockwise Rotation
SPR = 48   # Steps per Revolution (360 / 7.5)

GPIO.setmode(GPIO.BCM)
GPIO.setup(DIR, GPIO.OUT)
GPIO.setup(STEP, GPIO.OUT)
GPIO.output(DIR, CW)

step_count = SPR
delay = 1/SPR 


while True:
	cmd = input("Enter command:\n")
	if cmd == "up":
		for x in range(step_count/8): #drive motor 1/8 ful revolution in 1/8 second clockwise
		    GPIO.output(STEP, GPIO.HIGH)
		    sleep(delay)
		    GPIO.output(STEP, GPIO.LOW)
		    sleep(delay)
		GPIO.cleanup()
	elif cmd == "down":
		GPIO.output(DIR, CCW)
		for x in range(step_count/8): #drive motor 1/8 ful revolution in 1/8 second counter clockwise
		    GPIO.output(STEP, GPIO.HIGH)
		    sleep(delay)
		    GPIO.output(STEP, GPIO.LOW)
		    sleep(delay)
		GPIO.cleanup()
	elif cmd == "quit":
		GPIO.cleanup()
		False
	else:
		print("Invalid command\n")

