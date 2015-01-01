from xbee import ZigBee
import serial

ser = serial.Serial('/dev/ttyUSB0', 9600)

zb = ZigBee(ser)

num = 1
while True:
	try:
		print('Receiving frame {0} ...'.format(num))
		num += 1
		response = zb.wait_read_frame()
		print response
	except KeyboardInterrupt:
		break
        
ser.close()

