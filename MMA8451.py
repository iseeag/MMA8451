# -*- coding:utf-8 -*-
#!/usr/bin/python

# repeated start problem solution for MMA845x
# sudo chmod 666 /sys/module/i2c_bcm2708/parameters/combined 
# sudo echo -n 1 > /sys/module/i2c_bcm2708/parameters/combined // it used to be 0

import smbus
import time
import os
import RPi.GPIO as GPIO

# Define a class called Accel
class Accel():
    myBus=1
    b = smbus.SMBus(myBus)
    def setUp(self):
        self.b.write_byte_data(0x1C,0x2B,0x40) # REG_CTRL_REG2 0x2B, reset 0x40
        time.sleep(0.01)
        self.b.write_byte_data(0x1C,0x0E,0x02) # REG_XYZ_DATA_CFG 0X0E RANGE_4_G
        self.b.write_byte_data(0x1C,0x2B,0x02) # REG_CTRL_REG2, High Resolution
        self.b.write_byte_data(0x1C,0x2D,0x01) # REG_CTRL_REG4, Low Noise
        self.b.write_byte_data(0x1C,0x2A,0x01) # REG_PL_CFG, Activate 
#       self.b.write_byte_data(0x1C,0x14,0) # Calibrate
#       self.b.write_byte_data(0x1C,0x15,0) # Calibrate
    def getValueX_MSB(self):
        return self.b.read_byte_data(0x1C,0x01) 
    def getValueX_LSB(self):
        return self.b.read_byte_data(0x1C,0x02)
    def getValueY_MSB(self):
        return self.b.read_byte_data(0x1C,0x03)
    def getValueY_LSB(self):
        return self.b.read_byte_data(0x1C,0x04)
    def getValueZ_MSB(self):
        return self.b.read_byte_data(0x1C,0x05)
    def getValueZ_LSB(self):
        return self.b.read_byte_data(0x1C,0x06)
    def getValueWHO_AM_I(self):
        return self.b.read_byte_data(0x1C,0x0D) 


MMA8451 = Accel()
MMA8451.setUp()

for a in range(10000):
    x = MMA8451.getValueX_MSB()
    y = MMA8451.getValueY_MSB()
    z = MMA8451.getValueZ_MSB()
    device_id = MMA8451.getValueWHO_AM_I()
    print("X=", x)
    print("Y=", y)
    print("Z=", z)
    print("device id: ", device_id)
    time.sleep(0.1)
    os.system("clear")
