# neOCampus-arduino | home of the neOSensor end-devices #
________________________________________________________________

This repository provides support for the following neOCampus boards:
  * neOSensor boards based on ESP32
  * neOSensor-AirQuality board (ESP32) from CNRS' LCC lab.
  * neOSensor boards based on ESP8266 (rev. >= V2)

All this work was undertaken at Université Toulouse 3 / IRIT laboratory in the context of the [neOCampus](https://neocampus.org "neOCampus corporate") operation.

![neOSensorv5](/images/neOSensorV5.jpg)

## NEWS ##
 * **[Jun.21]** added KiCad PCB for LoRaWAN V2 (Heltec CubeCell)
 * **[Apr.21]** added support for 3 NTP servers + NTP from DHCP to lwip. Have a look to `arduinoIDE_esp32_boards/README.md`
                added KiCad PCB for neOSensor v5.1 (ESP32 based)
 * **[Feb.21]** added KiCad PCB for neOSensor v5 (ESP32 based)
 * **[Dec.20]** added PCB from ESP8266 version 4 (KiCad)
 * **[Aug.20]** new definition board for neOSensor-AirQuality board
added definition board for (future) neOSensor esp32 based board\
added suppport for boards configuration via sensOCampus JSON config\
added Arduino IDE support for our various neOSensor boards

## neOSensor boards ##
neOSensor first boards releases were based on esp8266. Nowadays, we added support for esp32 and CubeCell devices :)

We added support to our neOCampus IoT management infrastructure named [sensOCampus](https://neocampus.univ-tlse3.fr "neOCampus's Wiki Home") to enable a central configuration for all of our neOSensor boards; these boards will then be able to publish their data to our infrastructure and to have those data stored in our databases.

### neOSensor-AirQuality board ###
This board comes from CNRS LCC lab, it's a PhD work undertaken by Aymen Sendi (aymen.sendi@laas.fr).
The board features four specific sensors able to measure NO2, CO, CH20, NO2 alternate.

## Getting started ##
First of all, you ought to install esp32, esp8266 or CubeCell support in your Arduino IDE.

| Device   | Arduino Board Manager json file                                                             |
|----------|---------------------------------------------------------------------------------------------|
| esp8266  | https://arduino.esp8266.com/stable/package_esp8266com_index.json                            |
| esp32    | https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json |
| CubeCell | https://resource.heltec.cn/download/package_CubeCell_index.json                             |

Then, run script `./deploy.sh` and boards will get added to your Arduino boards list :)

If not already installed, you'll need python-serial package; thus either you install it at the system level
```
[fedora] dnf -y install python3-pyserial
[ubuntu] apt-get -y install python3-serial python-is-python3
```

Once you've launched the Arduino IDE, before compiling and uploading sketches to your board, you need to undertake the following steps:
  - @preferences --> set your Arduino sketchbook to `~/neOCampus-arduino/neosensor' ... this will lead to proper libraries
  - either select boards 'ESP32 arduino' --> 'neOSensor' or 'neOSensor-AirQuality' or select board 'ESP8266 arduino' --> 'neOSensor'
  - compile and upload :)
  - debug serial link speed 115200

### WiFi setup ###
Only for ESP based boards.

Once started, the board will provides a WiFi Access Point (AP) named: `neOSensor_xxxx` where xxxx are the MAC address last bytes.

**First time setup**\
In this case, AP will get available for 5mn for you to configure SSID along with WPA2 key.
So you'll have to search with your smartphone for this peucliar AP, and then connect. In case automatic redirection does not work, open a browser with http:192.168.4.1

**Changing WiFi setup**\
Once WiFi has been set, on each reboot, the neOSensor AP will get available for 50s in order for you to tweak some parameters ... or to change the WiFi it connects to.

**Reset whole configuration**\
To erase all of the previously set parameters (including WiFi settings), press the RST button for 10s when powering the device.


