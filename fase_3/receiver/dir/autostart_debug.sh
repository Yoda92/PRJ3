#!/bin/bash

SSID=rpiWifi
PW=wifi1234
CONNECTED="false"

while true
do
    if ping 192.168.0.1 -w 1 | grep -q '100% packet loss'
    then
        echo "Attempting connection..." 
        connmanctl disable wifi
        connmanctl enable wifi
        connmanctl scan wifi
        # Run services --> Find WIFI with SSID --> Set Hash to third or second field
            echo "Scanning network..." 
            HASH=$(connmanctl services | grep $SSID | awk '{print $3}')
            if [ "$HASH" == "" ] 
            then 
                HASH=$(connmanctl services | grep $SSID | awk '{print $2}')
                echo $HASH
            fi
            if [ "$HASH" == "" ] 
            then
                echo "WIFI: $SSID was not found"
            else
                echo "Discovered WIFI: $SSID with hash: $HASH"
                echo "Writing config file for WIFI: $SSID"
cat << EOF > /var/lib/connman/$SSID-psk.config
[service_$HASH]
Type = wifi
Name = $SSID
Passphrase = $PW
EOF
connmanctl connect $HASH
            fi 
    else
        echo "Connected. Starting program.."
        ~/main
    fi
    sleep 2
done