#!/bin/bash

SSID=rpiWifi
PW=wifi1234

sleep 30
connmanctl disable wifi
connmanctl enable wifi

while true
do
    echo "Scanning network..." >> ~/log.txt 2>&1
    connmanctl scan wifi
    HASH=$(connmanctl services | grep $SSID | awk '{print $3}')
    if [ "$HASH" == "" ] 
    then 
        HASH=$(connmanctl services | grep $SSID | awk '{print $2}')
    fi
    # Check for connection
    if [ "$HASH" != "" ] 
    then
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
            connmanctl connect $HASH >> ~/log.txt 2>&1
            echo "Connected. Starting program.." >> ~/log.txt 2>&1
            sleep 2
            ~/main
            echo "Program terminated." >> ~/log.txt 2>&1
    else
        echo "AP not found..." >> ~/log.txt 2>&1
    fi
    echo "Sleeping for 2 seconds..." >> ~/log.txt 2>&1
    sleep 2
done