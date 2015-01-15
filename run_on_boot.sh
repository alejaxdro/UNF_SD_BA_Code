# Any command here will be run on startup -Jonathan Frias
echo "The Beaglebone Black will Self-Destruct in ..."
sleep(1)
echo "3..."
sleep(.5)
echo "2..."
sleep(.25)
echo "1..."

# Now real development
touch me

# Mount SD card for data logging
mount /dev/mmcblk1p2 /root/sdcard

# Enable Devices
/root/HardwareTest/build


