#!/bin/bash

LINUX_VER=4.19.64
MODULE_DEST=/lib/modules/$LINUX_VER/

if  ping 10.9.8.2 -w 1 | grep -q '100% packet loss'
then
    echo "Connection failure"
    exit 1
fi

# Create driver module
make clean -C ./spi_driver/
make -C ./spi_driver/

# Create dir
make clean -C ./dir/
make -C ./dir/

# Move driver and dir files to target
scp ./spi_driver/spi_drv.ko ./spi_driver/spi_drv.dtbo dir/autostart.sh dir/main root@10.9.8.2: 

# Cleanup
make clean -C ./spi_driver/
make clean -C ./dir/

# RPI commands
ssh root@10.9.8.2 << EOF
    mv spi_drv.ko $MODULE_DEST
    depmod -a
    echo "spi_drv" > /etc/modules-load.d/spi_drv.conf
    mv spi_drv.dtbo /boot/overlays/
    if grep -Fq "dtoverlay=spi_drv" /boot/config.txt
    then
        echo "/boot/config.txt alread modified."
    else
        # Create the definition
        echo "Adding spi_drv to /boot/config.txt"
        echo "dtoverlay=spi_drv" >> /boot/config.txt
    fi
    mv autostart.sh /etc/init.d/
    rm ~/autostart_debug.sh
    chmod +x /etc/init.d/autostart.sh
    update-rc.d autostart.sh defaults 99
EOF