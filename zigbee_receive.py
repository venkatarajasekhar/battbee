from xbee import ZigBee
import serial

ser = serial.Serial('/dev/ttyUSB0', 9600)

zb = ZigBee(ser)

while True:
	try:
		print('Receiving frame ...')
		response = zb.wait_read_frame()
		print response
	except KeyboardInterrupt:
		break
        
ser.close()

