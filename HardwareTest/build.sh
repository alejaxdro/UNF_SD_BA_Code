#!/bin/bash
# build file
# Date Created:  11/11/2014
# Authors:       Alejandro Lepervanche
#                Elizabeth Voelkel
# 	Change Log:
#	1/12/2015
#		Alejandro added lines 25 to 27 and decided to comment out 27; read below
#
# This build file needs to be run every time beaglebone black is restarted

echo " "

# Implementing device tree overlay to initialize SPI hardware
echo "Export echo BB-SPIDEV0 to /sys/devices/bone_capemgr.9/slots"
echo BB-SPIDEV0 > /sys/devices/bone_capemgr.9/slots

# Implementing device tree overlay to initialize I2C hardware
#     This was written and compiled manually to verify pin placement and mode
echo "Export echo BB-I2C1 to /sys/devices/bone_capemgr.9/slots"
echo BB-I2C1 > /sys/devices/bone_capemgr.9/slots

# Implementing device tree overlay to initialize UART4 hardware
echo "Export echo BB-UART4 to /sys/devices/bone_capemgr.9/slots"
echo BB-UART4 > /sys/devices/bone_capemgr.9/slots
# Set parameters for serial comm
stty -F /dev/ttyO4 raw
stty -F /dev/ttyO4 38400
#stty -F /dev/ttyO4 parenb # This line was removed because of RoboClaw parameters for UART

# Display Device Tree Overlays currently implemented 
cat /sys/devices/bone_capemgr.9/slots

# Motivation to keep going
echo "Stay Calm and Program On"
echo " "

