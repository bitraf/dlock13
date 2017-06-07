# dlock13

Breakout design (in EAGLE) and source code for our door lock actuator, running on a Raspberry PI(1).
Communication with the lock happens over [MQTT](https://en.wikipedia.org/wiki/MQTT).
Does not contain user interface, check [door-web](https://github.com/bitraf/door-web) for that.

## MQTT interface

To open the door, send the *number of seconds to open* to:

    /bitraf/door/$door_id/open  # Ex: 20

On success, will output values:

    /bitraf/door/$door_id/isopen # Ex: true/false
    /bitraf/door/$door_id/openuntil # Unix timestamp when door will lock again. Ex: 1495469392

On error, will send a string with an error message:

    /bitraf/door/$door_id/error # Ex: "Time to open is negative"

Where `$door_id` is for instance `2floor`, `frontdoor`, `4floor`.
The `/bitraf/door/` prefix is configurable, can be set to `/mysystem/doorlocks`.

## Code

At [dlock13-msgflo/dlock13.cpp](./dlock13-msgflo/dlock13.cpp).

Impementation is in C++11, using [msgflo-cpp](https://github.com/msgflo/msgflo-cpp). 

## Python API

Also provided is a simple Python module which will handle checking the MQTT messages
being sent out on success/error.

Installing

    pip install dlock13

A command-line tool is included

    dlock13-open /bitraf/door/$door_id 60

For how to use programatically, use [dlock13-open](./dlock13-open) as example code.

## Board

![RPI breakout board with components](./doc/rpi-breakout-populated.jpg)

The board has two MOSFETS for driving standard electronic locks.
Expansion for two inputs also exists.

Single-sided board with through-hole components, can easily be CNC milled or etched.

![RPi breakout board schematic](./doc/rpi-breakout-schematic.png)

### Pinout

    # Function | Connector pin |  BCM SoC pin |  WiringPi pin  |
    ===
    mosfet1    |     19        |   10         |  12
    mosfet2    |     2         |   11         |  14

BCM SoC pin is the on used for `/sys/class/gpio/gpioNN` and similar.

## Deployment

A systemd service template is normally used to each doorlock: [dlock13@.service](dlock13-msgflo/dlock13@.service).

    systemctl status dlock13@2floor

The following environment variables are used. They can be specified in `/etc/dlock13/$door_id.conf`.

* `MSGFLO_BROKER` - the MQTT broker connection details. Ex: `mqtt://user:password@mqtt.example.net`
* `DLOCK13_PIN` - the GPIO pin number to use. Ex: `11`

