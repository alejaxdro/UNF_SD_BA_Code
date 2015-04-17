
# Any command here will be run on startup -Jonathan Frias

# Mount SD card for data logging
mount /dev/mmcblk1p2 /root/sdcard

sleep 2

# Enable Devices
/root/TestMoveWheelsCode/build.sh

sleep 2

# Reset wifi to ensure reliable connection
/root/wifi-reset/wifi-reset.sh

# Run program for AppControl
/root/AppControl/appControl_2
