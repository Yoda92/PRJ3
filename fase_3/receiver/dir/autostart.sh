#!/bin/bash

SSID=rpiWifi
PW=wifi1234

while true
do
    # Check for connection
    if ping 192.168.0.1 -w 1 | grep -q '100% packet loss'
    then
        echo "No connection. Attempting to connect..." >> ~/log.txt 2>&1
        connmanctl disable wifi
        connmanctl enable wifi
        connmanctl scan wifi
        echo "Scanning network..." >> ~/log.txt 2>&1
        # Run services --> Find WIFI with SSID --> Set Hash to third or second field
        HASH=$(connmanctl services | grep $SSID | awk '{print $3}')
        if [ "$HASH" == "" ] 
        then 
            HASH=$(connmanctl services | grep $SSID | awk '{print $2}')
        fi
        if [ "$HASH" == "" ] 
        then
            echo "WIFI: $SSID was not found" >> ~/log.txt 2>&1
        else
            echo "Discovered WIFI: $SSID with hash: $HASH" >> ~/log.txt 2>&1
            echo "Writing config file for WIFI: $SSID" >> ~/log.txt 2>&1
# <-------- Indent error due to bash EOF error
cat << EOF > /var/lib/connman/$SSID-psk.config
[service_$HASH]
Type = wifi
Name = $SSID
Passphrase = $PW
EOF
# <-------- Indent error due to bash EOF error
            connmanctl connect $HASH
            fi 
    else
        echo "Connected. Starting program.." >> ~/log.txt 2>&1
        ~/main >> ~/log.txt 2>&1
        echo "Program terminated. Sleeping for 2 seconds..." >> ~/log.txt 2>&1
    fi
    sleep 2
done