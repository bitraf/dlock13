How to make a base image.

A "base image" is an image which has all needed software for dlock13 already installed, you only need to change password, 
change hostname, add config files for the doors, setup systemd services and then you are ready to use it.

Things you need: a computer with SD card reader, a Raspberry Pi, a (micro)SD card that matches the Pi model used, the latest 
Raspberry Pi OS (the minimal image is recommended), a network cable and a power supply for the Pi.


# Set up the  SD card with Raspberry Pi OS

  Insert the SD card into your reader, and find out which device it is (often /dev/sdb, /dev/sdc or something).
  Note: be sure that you are selecting the right device; do not select a device with os or your data on!
  Note: select the device (example /dev/sdc) and not a partition (example: /dev/sdc1, /dev/sdc2)

write the image to the SD card (replace MYSDCARD with the name of the device):

    $ unzip 2020-05-27-raspios-buster-lite-armhf.zip | sudo dd of=/dev/MYSDCARD status=progress oflag=sync bs=1M

Enable ssh access

mount the boot partition and enable ssh access by creating a file named 'ssh' in the root directory:

    $ sudo touch ssh

change the hostname (avoids having many machines called 'raspberrypi.local'): mount the rootfs partition, edit etc/hostname
and etc/hosts, change raspberrypi to (for example) 'boxy0'.

unmount any partitions that are still mounted.

# Add necessary software and configuration to the SD card

Insert the card into the Raspberry Pi, connect the Pi to the same network as your computer and power it up. After it has 
booted, you should be able to ping it (if you named it boxy0): $ ping boxy0.local
When it answers you can ssh into it 
    $ ssh pi@boxy0.local 
(using the standard password for Raspberry Pi OS)

change the password of the pi user (always a good idea to change any standard passwords), but remember that you must document
this password, so use one that is not used anywhere else.

verify the the user is a member of the 'gpio' group
    $ groups

get the software from the repo into the home directory of the pi user

    $ git clone --recursive https://github.com/bitraf/dlock13.git

install the necessary packages for building

    $ sudo apt install git cmake libev-dev libmosquitto-dev

build the software

    $ cd dlock13-msgflo
    $ ./build.sh

copy the service file

    $ sudo cp dlock13@.service /etc/systemd/system/

make the directory for the config files

    $ sudo mkdir /etc/dlock13

power off the Pi and take out the SD card

# shrink the image

The Raspberry Pi OS is set up so that it expands the rootfs partition to fill the SD card on the first boot. Useful for a 
normal installation, but wasteful for a base image. Therefore, we will shrink that partition before making an image of 
the SD card.

Insert the SD card into your computer, and use (for example) gparted to shrink the rootfs so it is just a bit larger than 
used space. A bit below 4 GB is suitable.

# create the base image

First find out where the last partition ends. I like to use parted for this. Example:

    $ sudo parted /dev/sdc print
    Model: Generic- SD/MMC (scsi)
    Disk /dev/sdc: 15.6GB
    Sector size (logical/physical): 512B/512B
    Partition Table: msdos
    Disk Flags: 
    
    Number  Start   End     Size    Type     File system  Flags
     1      4194kB  273MB   268MB   primary  fat32        lba
     2      273MB   4362MB  4089MB  primary  ext4

Here the last partition ends at 4362 MB. To be safe, add a few megabytes, then copy the image

    $ sudo dd if=/dev/sdc status=progress oflag=sync bs=1M count=4399 > ./dlock13-base-image.img

Compress the image (use gzip, zip or a similar tool) and upload it somewhere public. Remember to document the password for the
pi user!
