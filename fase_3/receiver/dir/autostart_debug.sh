#!/bin/bash

SSID=rpiWifi
PW=wifi1234

sleep 30
connmanctl disable wifi
connmanctl enable wifi

while true
do
    echo "Scanning network..."
    connmanctl scan wifi
    HASH=$(connmanctl services | grep $SSID | awk '{print $3}')
    if [ "$HASH" == "" ] 
    then 
        HASH=$(connmanctl services | grep $SSID | awk '{print $2}')
    fi
    # Check for connection
    if [ "$HASH" != "" ] 
    then
        echo "Discovered WIFI: $SSID with hash: $HASH"
        echo "Writing config file for WIFI: $SSID"
# <-------- Indent error due to bash EOF error
cat << EOF > /var/lib/connman/$SSID-psk.config
[service_$HASH]
Type = wifi
Name = $SSID
Passphrase = $PW
EOF
# <-------- Indent error due to bash EOF error
            connmanctl connect $HASH 
            echo "Connected. Starting program.."
            sleep 2
            ~/main
            echo "Program terminated."
    else
        echo "AP not found..."
    fi
    echo "Sleeping for 2 seconds..."
    sleep 2
done