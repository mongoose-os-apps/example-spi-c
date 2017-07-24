# C SPI API usage example

## Overview

This example demonstartes how to use [C SPI API](https://github.com/mongoose-os-libs/spi/blob/master/src/mgos_spi.h).

Example code reads a JEDEC ID from an SPI flash attached to the SPI interface pins.
Pretty much any SPI flash chip should work. The result should be something like this:

```
[Jul 24 17:51:49.996] mgos_spi_create      SPI3 init ok (MISO: 19, MOSI: 23, SCLK: 18; CS0/1/2: 5/-1/-1; native? yes)
[Jul 24 17:51:50.002] mgos_app_init        JEDEC ID: ef 40 16
```

JEDEC ID consists of a manufacturer ID, bus options and capacity (`0xef` is Winbond, 2 ^ `0x16` = 4MBytes). You can find more info about it on the Internet, but as long as it's not all `00` or `ff`, SPI is working.

*Note:* This example requires an extra chip, it is not the same chip that comes with ESP8266, ESP32 and CC3200 modules.

## Platform support

To use SPI, `spi.enable` needs to be set to `true` (this example changes the default to true, see [mos.yml](mos.yml)).
Other than that, each platform has a few other options.

### ESP32

[Here](mos_esp32.yml) is the SPI configuration for ESP32. You can change pins pretty much however you like, the defaults are native pin functions of SPI3 (aka VSPI). Native/non-native mapping is significant for higher SPI frequencies: 40 and 80MHz clock can only be used if pins are mapped to their native functions, not via GPIO matrix. If you don't understand what it all means, just use the defaults and/or stick to 20MHz and lower frequencies.

The hardware supports up to 3 CS pins, if you need more devices, you'll need to manually assert CS before running the transaction.

### ESP8266

Currently ESP8266 uses software bit-banging implementation which can deliver clock speed of ~700 KHz.
The configuration parameters are similar and specified [here](mos_esp8266.yml).

### CC3200

Currently CC3200 uses software bit-banging implementation which can deliver clock speed of ~700 KHz.
The configuration parameters are similar and specified [here](mos_cc3200.yml).
