#!/bin/bash

SSID=rpiWifi
PW=wifi1234
CONNECTED="false"

connmanctl disable wifi
connmanctl enable wifi

while [ "$CONNECTED" == "false" ]
do
    connmanctl scan wifi
    # Run services --> Find WIFI with SSID --> Set Hash to second field
    HASH=$(connmanctl services | grep $SSID | awk '{print $3}')
    if [ "$HASH" == "" ] 
    then 
        echo "WIFI: $SSID was not found"
        echo "Scanning network..."
        sleep 1
    else
        echo "Discovered WIFI: $SSID with hash: $HASH"
        CONNECTED=$(true)
    fi
done

echo "Writing config file for WIFI: $SSID"

cat << EOF > /var/lib/connman/$SSID-psk.config
[service_$HASH]
Type = wifi
Name = $SSID
Passphrase = $PW
EOF

connmanctl connect $HASH

./main