from dataReading import tempReader, powerReader
import time
import os

fname = '/home/pi/Desktop/data_test.csv'
maxTime = 1800.0
pollTime = 10.0

fo = open(fname, 'a')

powrR = powerReader()
tempR = tempReader('/dev/ttyUSB0',2)

starttime = time.monotonic()
tnow = 0
data = str(tnow) + ', ' + powrR.read() + tempR.read()

nT = len(data.split(',')) - 5
header = 'time (s), voltage (V), current (mA), power (mW)'
for i in range(nT):
    header = header + ', temperature ' + str(i+1) + ' (deg C)'

print(header)
fo.write(header+'\n')

while tnow-starttime < maxTime:
    print(data)
    fo.write(data+'\n')
    time.sleep(pollTime-((time.monotonic()-starttime)%pollTime))

    tnow = time.monotonic()
    data = str(tnow-starttime) + ', ' + powrR.read() + tempR.read()

fo.close()