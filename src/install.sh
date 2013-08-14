#!/bin/sh

set -e

sudo mkdir -p /usr/local/bin
sudo cp unlock /usr/local/bin
sudo chown root:root /usr/local/bin/unlock
sudo chmod u+s /usr/local/bin/unlock

echo Installed
