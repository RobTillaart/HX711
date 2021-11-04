
[![Arduino CI](https://github.com/RobTillaart/HX711/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/HX711/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/HX711/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/HX711/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/HX711/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/HX711/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/HX711.svg?maxAge=3600)](https://github.com/RobTillaart/HX711/releases)


# HX711

Arduino library for HX711 24 bit ADC  used for load cells and scales.


## Description

This HX711 library has an interface which is a superset of a library made by Bogde.
Some missing functions were added to get more info from the lib. 

Another important difference is that this library uses floats. 
The 23 bits mantissa of the IEE754 float matches the 24 bit ADC very well. Furthermore it gave a smaller footprint.


## Main flow

First action is to call **begin(dataPin, clockPin)** to make connection to the **HX711**.

Second step is calibration for which a number of functions exist.
- **tare()** measures zero point
- **set_scale(factor)** set a known conversion factor e.g. from EEPROM.
- **calibrate_scale(WEIGHT, TIMES)** determines the scale factor based upon a known weight e.g. 1 Kg.

Steps to take for calibration
1. clear the scale
1. call tare() to set the zero offset
1. put a known weight on the scale 
1. call calibrate_scale(weight) 
1. scale is calculated.
1. save the offset and scale for later use e.g. EEPROM.


## Interface


#### Base

- **HX711()** constructor
- **~HX711();
- **void begin(uint8_t dataPin, uint8_t clockPin)** sets a fixed gain 128 for now
- **void reset()**
- **bool is_ready()** checks if loadcell is ready to read.
- **void wait_ready(uint32_t ms = 0)** wait until ready, check every ms
- **bool wait_ready_retry(uint8_t retries = 3, uint32_t ms = 0)** wait max retries.
- **bool wait_ready_timeout(uint32_t timeout = 1000, uint32_t ms = 0)** wait max timeout.
- **float read()** raw read.
- **float read_average(uint8_t times = 10)** get average of times raw reads. times = 1 or more.
- **float read_median(uint8_t times = 7)** get median of multiple raw reads. 
times = 3..15 - odd numbers preferred.
- **float read_medavg(uint8_t times = 7)** get average of "middle half" of multiple raw reads.
times = 3..15 - odd numbers preferred.
- **float read_runavg(uint8_t times = 7, float alpha = 0.5)** get running average over times measurements.
The weight alpha can be set to any value between 0 and 1, times >= 1.
- **uint32_t last_read()** returns timestamp in milliseconds


#### Gain

read datasheet - see also Connections HX711 below
- **void set_gain(uint8_t gain = 128)** values: 128 (default), 64 32  - only 128 tested & verified
- **uint8_t get_gain()** returns set gain.


#### Mode 

Get and set the operational mode for get_value() and indirect get_units().
In median and medavg mode only 3..15 samples are allowed.
- **void set_average_mode()**
- **void set_median_mode()**
- **void set_medavg_mode()**
- **void set_runavg_mode()** default alpha = 0.5
- **uint8_t  get_mode()**


#### get values corrected for offset and scale

- **float get_value(uint8_t times = 1)** read value, corrected for offset.
- **float get_units(uint8_t times = 1)** read value, converted to proper units.
- **void set_scale(float scale = 1.0)** set scale factor; scale > 0
- **float get_scale()** returns set scale factor.
- **void set_offset(long offset = 0)** set offset
- **long get_offset()**


#### TARE & CALIBRATION

Steps to take for calibration
1. clear the scale
1. call tare() to set the zero offset
1. put a known weight on the scale 
1. call calibrate_scale(weight) 
1. scale is calculated.
1. save the offset and scale for later use e.g. EEPROM.

- **void tare(uint8_t times = 10)** call tare to calibrate zero level
- **float get_tare()**
- **bool tare_set()**
- **void calibrate_scale(uint16_t weight, uint8_t times = 10)**


#### POWER MANAGEMENT

- **void power_down()** idem
- **void power_up()** idem


#### Pricing

Some price functions were added to make it easy to use this library
for pricing goods or for educational purposes. 
These functions are under discussion if they will stay.
For weight conversion functions see https://github.com/RobTillaart/weight

- **float get_price(uint8_t times = 1)**
- **void  set_unit_price(float price = 1.0)**
- **float get_unit_price()**


## Notes


### Scale values for loadcells

These scale values worked pretty well with a set of load-cells, 
Use calibrate to find your values.

- 5 KG loadcell   scale.set_scale(420.52); 
- 20 KG loadcell  scale.set_scale(127.15); 


### Connections HX711

- A+/A-  uses gain of 128 or 64
- B+/B-  uses gain of 32


### Connections

| HX711 Pin | Color |
|:----:|:----:|
|  E+  |  red           |
|  E-  |  black         |
|  A-  |  white         |
|  A+  |  green         |
|  B-  |  not connected |
|  B+  |  not connected |


| HX711 Pin | Color |
|:----:|:----:|
|  E+  | red           |
|  E-  | black         |
|  A-  | blue          |
|  A+  | white         |
|  B-  | not connected |
|  B+  | not connected |


### Temperature

Load-cells do have a temperature related error. (check datasheet)
This can be reduced by doing the calibration and take the tare 
at the temperature one also uses for the measurements.

Another way to handle this is to add a good temperature sensor
(e.g. DS18B20, SHT85) and compensate for the temperature
differences in your code. 


## Operation

See examples


## Future

- update documentation
- add examples
- test different load-cells


#### the adding scale

- void weight_clr(), void weight_add(), float weight_get() - adding scale 

