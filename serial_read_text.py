import serial
import time

t0 = time.monotonic()

serialPort = '/dev/ttyUSB0'

ser = serial.Serial(serialPort, timeout=2)

ser.baudrate = 9600
ser.bytesize = 8
ser.stopbits = 1
ser.rtscts = False
ser.parity = 'N'
ser.xonxoff= False

for i in range(0, 100):
    ser.reset_input_buffer()
    w = ser.in_waiting
    x=ser.readline() #(w)
    print(w, x)#.decode())
    time.sleep(3.0-((time.monotonic()-t0)%1.0))
