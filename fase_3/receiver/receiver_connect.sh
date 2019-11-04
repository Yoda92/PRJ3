#!/bin/bash

SSID=rpiWifi
PW=wifi1234
CONNECTED="false"

connmanctl enable wifi

while [ "$CONNECTED" == "false" ]
do
    connmanctl scan wifi
    # Run services --> Find WIFI with SSID --> Set Hash to second field
    HASH=$(connmanctl services | grep $SSID | awk '{print $2}')
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

#cat << EOF > /var/lib/connman/$SSID-psk.config
#[service_$HASH]
#Type = wifi
#Name = $SSID
#Passphrase = $PW
#IPv4.method=manual
#IPv4.netmask_prefixlen=24
#IPv4.local_address=192.168.0.133
#IPv6=off 
#EOF

connmanctl connect $HASH