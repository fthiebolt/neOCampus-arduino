# neOCampus-arduino library
neOCampus IoT support library for Arduino. This library will provides support for the following neOCampus boards:
  * neOSensor boards based on ESP8266 (rev. >= V2)
  * neOSensor boards based on ESP32
  * neOSensor-AirQuality board (ESP32) from CNRS' LCC lab.

## NEWS ##
 * **[Aug.20]** new definition board for neOSensor-AirQuality board :)\
added definition board for (future) neOSensor esp32 based board\
added suppport for boards configuration via sensOCampus JSON config\
added Arduino IDE support for our various neOSensor boards\

## neOSensor boards ##
neOSensor first boards releases were based on esp8266. Nowadays, we added support for esp32 devices :)

We added support to our neOCampus IoT management infrastructure named [sensOCampus](https://neocampus.univ-tlse3.fr "neOCampus's Wiki Home") to enable a central configuration for all of our neOSensor boards; these boards will then be able to publish their data to our infrastructure and to have those data stored in our databases.

### neOSensor-AirQuality board ###
This board comes from CNRS LCC lab, it's a PhD work undertaken by Aymen Sendi (aymen.sendi@laas.fr).
The board features four specific sensors able to measure NO2, CO, CH20, NO2 alternate.

## Getting started ##
First of all, you ought to install esp32 or esp8266  support in your Arduino IDE.

  - esp8266 https://arduino.esp8266.com/stable/package_esp8266com_index.json
  - esp32 https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Then, run script './deploy.sh' and boards will get added to your Arduino boards list :)

Once you've launched the Arduino IDE, before compiling and uploading sketches to your board, you need to undertake the following steps:
  - @preferences --> set your Arduino sketchbook to `~/neOCampus-arduino/neosensor' ... this will lead to proper libraries
  - either select boards 'ESP32 arduino' --> 'neOSensor' or 'neOSensor-AirQuality' or select board 'ESP8266 arduino' --> 'neOSensor'
  - compile and upload :)
  - debug serial link speed 115200

