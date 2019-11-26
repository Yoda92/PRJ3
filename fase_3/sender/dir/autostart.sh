#!/bin/bash

# Set inputs
echo 22 > /sys/class/gpio/export
echo 27 > /sys/class/gpio/export
echo 17 > /sys/class/gpio/export

echo in > /sys/class/gpio/gpio22/direction
echo in > /sys/class/gpio/gpio27/direction
echo in > /sys/class/gpio/gpio17/direction

sleep 30
connmanctl disable wifi 
connmanctl enable wifi 
connmanctl tether wifi on rpiWifi wifi1234
# Program loop
while true
do
    ISAP=$(connmanctl services | grep $SSID)
    if ping 192.168.0.1 -w 1 | grep -q '100% packet loss'
    then
        echo "AP failed. Attempting again in 2 seconds.." >> ~/log.txt 2>&1
        connmanctl disable wifi 
        connmanctl enable wifi 
        connmanctl tether wifi on rpiWifi wifi1234
    else
        echo "AP ready. Checking connection.." >> ~/log.txt 2>&1
        if ping 192.168.0.2 -w 1 | grep -q '100% packet loss'
        then
            echo "No connection. Waiting..." >> ~/log.txt 2>&1 &
        else
            echo "Connection established. Starting program." >> ~/log.txt 2>&1
            sleep 2
            # Run main loop
            ~/main
            echo "Program terminated." >> ~/log.txt 2>&1
        fi
    fi
    echo "Sleeping for 2 seconds..." >> ~/log.txt 2>&1
    sleep 2
done