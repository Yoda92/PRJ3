#!/bin/bash

# Set inputs
echo 22 > /sys/class/gpio/export
echo 27 > /sys/class/gpio/export
echo 17 > /sys/class/gpio/export

echo in > /sys/class/gpio/gpio22/direction
echo in > /sys/class/gpio/gpio27/direction
echo in > /sys/class/gpio/gpio17/direction

# Run main loop
~/main >> ~/log_rpi.txt 2>&1 &

# Create AP
while true
do
    if ping 192.168.0.1 -w 1 | grep -q '100% packet loss'
    then
        echo "Connection failed. Attempting again in 2 seconds.." >> ~/log.txt 2>&1
        connmanctl enable wifi >> ~/log.txt 2>&1
        connmanctl tether wifi on rpiWifi wifi1234 >> ~/log.txt 2>&1
    else
        echo "Connected. Sleep for 2 seconds.." >> ~/log.txt 2>&1
    fi
    sleep 2
done