#!/bin/bash

# Set inputs
echo 22 > /sys/class/gpio/export
echo 27 > /sys/class/gpio/export
echo 17 > /sys/class/gpio/export

echo in > /sys/class/gpio/gpio22/direction
echo in > /sys/class/gpio/gpio27/direction
echo in > /sys/class/gpio/gpio17/direction

# Create AP
while true
do
    if ping 192.168.0.1 -w 1 | grep -q '100% packet loss'
    then
        echo "AP failed. Attempting again in 2 seconds.."
        connmanctl disable wifi 
        connmanctl enable wifi 
        connmanctl tether wifi on rpiWifi wifi1234
    else
        echo "AP ready. Checking connection.."
        if ping 192.168.0.2 -w 1 | grep -q '100% packet loss'
        then
            echo "No connection. Waiting..."
        else
            echo "Connection established. Starting program."
            # Run main loop
            ~/main
            echo "Program terminated."
        fi
    fi
    echo "Sleeping for 2 seconds..."
    sleep 2
done