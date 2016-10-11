# dlock13

Breakout design (in EAGLE) and source code for our door lock actuator, running on a Raspberry PI(1).
Communication with the lock happens over [MQTT](https://en.wikipedia.org/wiki/MQTT).
Does not contain user interface, check [door-web](https://github.com/bitraf/door-web) for that.

## Board

![RPI breakout board with components](./doc/rpi-breakout-populated.jpg)

The board has two MOSFETS for driving standard electronic locks.
Expansion for two inputs also exists.

Single-sided board with through-hole components, can easily be CNC milled or etched.

### Pinout

    # Function | Connector pin |  BCM SoC pin |  WiringPi pin  |
    ===
    mosfet1    |     19        |   10         |  12
    mostfet2   |     2         |   11         |  14

BCM SoC pin is the on used for `/sys/class/gpio/gpioNN` and similar.

## Code

`TODO: document`
