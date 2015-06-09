#!/bin/bash

#setup your pi 
# This little script ensures that the kernel I2C drivers are loaded, and it
# changes permissions on /dev/i2c-0 so you can run the demos as a regular user

sudo modprobe i2c-dev
sleep 0.1
sudo modprobe i2c-bcm2708
sleep 0.1
sudo chmod 666 /dev/i2c-1 # sudo chmod 666 /dev/i2c-0 for rpi version 1
sleep 0.1
sudo chmod 666 /sys/module/i2c_bcm2708/parameters/combined 
sleep 0.1
sudo echo -n 1 > /sys/module/i2c_bcm2708/parameters/combined 
