import serial
import board
import busio
import adafruit_ina260
import numpy as np
import time
from ky_STS3X import *


class tempReader(object):
    def __init__(self, serialPort, timeout):
        self.ser = serial.Serial(serialPort, timeout=timeout)
        with self.ser as ser:
            ser.baudrate = 9600
            ser.bytesize = 8
            ser.stopbits = 1
            ser.rtscts = False
            ser.parity = 'N'
            ser.xonxoff= False
        

    def read(self):
        with self.ser as ser:
            ser.reset_input_buffer()
            w = 0
            while w == 0:
                w = ser.in_waiting
                time.sleep(0.1)
                
            line = ser.readline()
            line = line.decode()
            line = line.split()
            str = ""
            for i in line:
                str = str + i + ', '
            return str
            
class powerReader(object):

    def __init__(self, nAvgs = 100):
        self.i2c = busio.I2C(board.SCL, board.SDA)
        self.ina260 = adafruit_ina260.INA260(self.i2c)
        self.nAvgs = nAvgs

    def readAvg(self, method):
        array = np.zeros(self.nAvgs)
        for i in range(0,self.nAvgs):
            array[i] = getattr(self.ina260, method)
        return array.mean()
    
    def read(self):
        I = self.readAvg('current')
        V = self.readAvg('voltage')
        P = self.readAvg('power')
        return "{0}, {1}, {2}, ".format(V, I, P)
    
class tempReaderSTS(object):
    # Determine I2C address according to the ADDR pin pull-up or pull-down
    # ADDR pin pull-down: STS3X_I2C_ADDRESS_A   0x4A
    # ADDR pin pull-up: STS3X_I2C_ADDRESS_B   0x4B
    def __init__(self, i2c_addr = STS3X_I2C_ADDRESS_A, bus = 1):
        self.sensor = ky_STS3X(i2c_addr = i2c_addr, bus = bus)
        self.sensor.set_repeat(sensor.REPEAT_HIGH)
        self.sensor.sensor_break()
        
    def read(self):
        temperature = self.sensor.get_temperature_single()
        return "{0}, ".format(temperature)
