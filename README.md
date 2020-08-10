# neOCampus-arduino library
neOCampus IoT support library for Arduino. This library will provides support for the following neOCampus boards:
  * neOSensor based on ESP8266 (rev. >= V2)
  * neOSensor-AirQuality board based on ESP32
  * neOSensor based on ESP32 module

## NEWS ##
 * **[Aug.20]** new definition board for neOSensor-AirQuality board :)

## neOSensor AirQuality board ##
This board comes from CNRS LCC lab, it's a PhD work undertalen by Aymen Sendhi (sendhi@laas.fr).
The board features four specific sensors able to measure NO2, CO, CH20, NO2 alternate.
We added support to our neOCampus IoT management infrastructure named [sensOCampus](https://neocampus.univ-tlse3.fr "neOCampus's Wiki Home") which enables this sensor to publish data and to have those data stored in our databases.

Once you've gone through the later [getting started](#getting-started), you can compile and upload the firmware to the board (we're currently using a regular ESP32-WROOM32 board).
To achiev this, you'll need to:
  - @preferences --> set your Arduino sketchbook to `~/neOCampus-arduino/neosensor-airquality' ... this will lead to proper libraries
  - select board 'ESP32 arduino' --> 'neOSensor AirQuality'
  - compile :)

### Getting started
First of all, you ought to install esp32 support in your Arduino IDE.

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Then, run script './deploy.sh' and board will get added to your Arduino boards list :)

### TODO
a lot ... this library is both a port from what has been done for neOSensor_esp8266 along with a real use case for CNRS LCC lab air-quality sensor :)
... stay tuned !

