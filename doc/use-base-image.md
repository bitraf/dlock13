A base image exists at [2020-06-08 dlock13-image.img.gz](http://heim.bitraf.no/tingo/files/iso/dlock13-base-image.img.gz). 
The image is based on the Raspberry Pi OS image "2020-05-27-raspios-buster-lite-armhf.img". The password for the pi user
is the same as on the original Raspberry Pi OS image.

To use the image, copy it to a SD card

    $ gunzip -c dlock13-image.img.gz | sudo dd of=/dev/MYSDCARD status=progress oflag=sync bs=1M

replace MYSDCARD with the name of your sdcard, example: sdc.

Change hostname.

Mount the rootfs partition, and change the hostname by editing etc/hostname and etc/hosts. change 'boxy0' to the selected 
name, for example 'boxy5'.

Add config files

Add config file(s) for dlock13 in /etc/dlock13. The config files are named doorname.conf, for example firstdoor.conf, 
2nddoor.conf and so on. The contents of the config files is described in [../README.md].

Start the service

(This step requires that you have booted a Raspberry Pi with the SD card)
There is a systemd service for each door. That service needs to be enabled and started. Example:

    $ sudo systemctl enable dlock13@firstdoor
    $ sudo systemctl start dlock13@firstdoor
    $ sudo systemctl enable dlock13@2nddoor
    $ sudo systemctl start dlock13@2nddoor

If everything is correct, your door opener is now ready for use. To check if the service is working correctly, use

    $ systemctl status dlock13@firstdoor
    $ systemctl status dlock13@2nddoor
