EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "neOSensor v5"
Date "2021-02-04"
Rev "v5"
Comp "Univ.Tlse3 / IRIT / neOCampus"
Comment1 "M2siame 2020-2021 project"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L neosensor_addon:OLED1.3_I2C U?
U 1 1 601B3200
P 5950 3300
F 0 "U?" H 6428 3304 50  0000 L CNN
F 1 "OLED1.3_I2C" H 6428 3213 50  0000 L CNN
F 2 "neosensor_addon:OLED1.3_I2C" H 5950 3300 50  0001 C CNN
F 3 "https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf" H 5950 3300 50  0001 C CNN
	1    5950 3300
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:ESP32_dev_30p U?
U 1 1 601B39D1
P 8950 3400
F 0 "U?" H 8950 4375 50  0000 C CNN
F 1 "ESP32_dev_30p" H 8950 4284 50  0000 C CNN
F 2 "neosensor_addon:ESP32_dev_30p" V 8950 3450 50  0001 C CIN
F 3 "" H 8950 3800 50  0001 C CNN
	1    8950 3400
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 601B4C77
P 8950 4950
F 0 "U?" H 8950 5192 50  0000 C CNN
F 1 "AMS1117-3.3" H 8950 5101 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 8950 5150 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 9050 4700 50  0001 C CNN
	1    8950 4950
	1    0    0    -1  
$EndComp
Text Notes 5950 900  0    100  ~ 0
I2C sensors connectors
Text Notes 9050 900  0    100  ~ 0
Serial sensors connector
$Comp
L Mechanical:MountingHole_Pad H?
U 1 1 601BFC00
P 6000 6800
F 0 "H?" V 5954 6950 50  0000 L CNN
F 1 "MountingHole_Pad" V 6045 6950 50  0000 L CNN
F 2 "" H 6000 6800 50  0001 C CNN
F 3 "~" H 6000 6800 50  0001 C CNN
	1    6000 6800
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H?
U 1 1 601C095B
P 6000 7100
F 0 "H?" V 5954 7250 50  0000 L CNN
F 1 "MountingHole_Pad" V 6045 7250 50  0000 L CNN
F 2 "" H 6000 7100 50  0001 C CNN
F 3 "~" H 6000 7100 50  0001 C CNN
	1    6000 7100
	0    1    1    0   
$EndComp
Text Notes 5400 6450 0    100  ~ 0
Board mounting holes
$Comp
L power:GND #PWR?
U 1 1 601C1C3A
P 8950 5500
F 0 "#PWR?" H 8950 5250 50  0001 C CNN
F 1 "GND" H 8955 5327 50  0000 C CNN
F 2 "" H 8950 5500 50  0001 C CNN
F 3 "" H 8950 5500 50  0001 C CNN
	1    8950 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 5500 8950 5400
Text Label 8950 5450 1    50   ~ 0
GND
Wire Wire Line
	5550 6800 5900 6800
Wire Wire Line
	5550 7100 5900 7100
Text Label 5550 6800 0    50   ~ 0
GND
Text Label 5550 7100 0    50   ~ 0
GND
Wire Wire Line
	8500 3950 8200 3950
Text Label 8200 3950 0    50   ~ 0
GND
Wire Wire Line
	9400 3950 9700 3950
Text Label 9700 3950 2    50   ~ 0
GND
$Comp
L power:+5V #PWR?
U 1 1 601C467B
P 8050 4050
F 0 "#PWR?" H 8050 3900 50  0001 C CNN
F 1 "+5V" H 8065 4223 50  0000 C CNN
F 2 "" H 8050 4050 50  0001 C CNN
F 3 "" H 8050 4050 50  0001 C CNN
	1    8050 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 4050 8500 4050
Text Label 8200 4050 0    50   ~ 0
VIN
Wire Wire Line
	8200 4950 8650 4950
Text Label 8200 4950 0    50   ~ 0
VIN
Wire Wire Line
	9250 4950 9350 4950
Text Label 9700 4950 2    50   ~ 0
3V3
Wire Wire Line
	9700 4050 9400 4050
Text Label 9700 4050 2    50   ~ 0
3V3
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 601C66E2
P 5750 1250
F 0 "J?" H 5700 1450 50  0000 L CNN
F 1 "Conn_01x04" H 5550 950 50  0000 L CNN
F 2 "" H 5750 1250 50  0001 C CNN
F 3 "~" H 5750 1250 50  0001 C CNN
	1    5750 1250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J?
U 1 1 601CACB8
P 7500 1350
F 0 "J?" H 7450 1650 50  0000 L CNN
F 1 "Conn_01x05" H 7300 1050 50  0000 L CNN
F 2 "" H 7500 1350 50  0001 C CNN
F 3 "~" H 7500 1350 50  0001 C CNN
	1    7500 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J?
U 1 1 601CBCDB
P 8450 1350
F 0 "J?" H 8400 1650 50  0000 L CNN
F 1 "Conn_01x05" H 8250 1050 50  0000 L CNN
F 2 "" H 8450 1350 50  0001 C CNN
F 3 "~" H 8450 1350 50  0001 C CNN
	1    8450 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 601CE9B0
P 10050 1250
F 0 "J?" H 10000 1450 50  0000 L CNN
F 1 "Conn_01x04" H 9850 950 50  0000 L CNN
F 2 "" H 10050 1250 50  0001 C CNN
F 3 "~" H 10050 1250 50  0001 C CNN
	1    10050 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1_Small C?
U 1 1 601D0AC1
P 9350 5200
F 0 "C?" H 9441 5246 50  0000 L CNN
F 1 "470µF 6.3v" H 9441 5155 50  0000 L CNN
F 2 "" H 9350 5200 50  0001 C CNN
F 3 "~" H 9350 5200 50  0001 C CNN
	1    9350 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 5100 9350 4950
Connection ~ 9350 4950
Wire Wire Line
	9350 4950 9700 4950
Wire Wire Line
	9350 5300 9350 5400
Wire Wire Line
	9350 5400 8950 5400
Connection ~ 8950 5400
Wire Wire Line
	8950 5400 8950 5250
Wire Wire Line
	9400 2750 9700 2750
Text Label 9700 2750 2    50   ~ 0
SCL
Wire Wire Line
	9400 3050 9700 3050
Text Label 9700 3050 2    50   ~ 0
SDA
Wire Wire Line
	5550 1150 5250 1150
Text Label 5250 1150 0    50   ~ 0
3V3
Wire Wire Line
	5250 1250 5550 1250
Text Label 5250 1250 0    50   ~ 0
GND
Wire Wire Line
	5250 1350 5550 1350
Text Label 5250 1350 0    50   ~ 0
SCL
Wire Wire Line
	5250 1450 5550 1450
Text Label 5250 1450 0    50   ~ 0
SDA
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 601D570B
P 6600 1250
F 0 "J?" H 6550 1450 50  0000 L CNN
F 1 "Conn_01x04" H 6400 950 50  0000 L CNN
F 2 "" H 6600 1250 50  0001 C CNN
F 3 "~" H 6600 1250 50  0001 C CNN
	1    6600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1150 6100 1150
Text Label 6100 1150 0    50   ~ 0
3V3
Wire Wire Line
	6100 1250 6400 1250
Text Label 6100 1250 0    50   ~ 0
GND
Wire Wire Line
	6100 1350 6400 1350
Text Label 6100 1350 0    50   ~ 0
SCL
Wire Wire Line
	6100 1450 6400 1450
Text Label 6100 1450 0    50   ~ 0
SDA
Wire Wire Line
	7300 1150 7000 1150
Text Label 7000 1150 0    50   ~ 0
3V3
Wire Wire Line
	7000 1250 7300 1250
Text Label 7000 1250 0    50   ~ 0
GND
Wire Wire Line
	7000 1350 7300 1350
Text Label 7000 1350 0    50   ~ 0
SCL
Wire Wire Line
	7000 1450 7300 1450
Text Label 7000 1450 0    50   ~ 0
SDA
Wire Wire Line
	8250 1150 7950 1150
Text Label 7950 1150 0    50   ~ 0
3V3
Wire Wire Line
	7950 1250 8250 1250
Text Label 7950 1250 0    50   ~ 0
GND
Wire Wire Line
	7950 1350 8250 1350
Text Label 7950 1350 0    50   ~ 0
SCL
Wire Wire Line
	7950 1450 8250 1450
Text Label 7950 1450 0    50   ~ 0
SDA
Wire Wire Line
	7000 1550 7300 1550
Wire Wire Line
	7950 1550 8250 1550
Text Label 7950 1550 0    50   ~ 0
INT
Text Label 7000 1550 0    50   ~ 0
INT
Wire Wire Line
	5800 3050 5800 2750
Wire Wire Line
	5900 3050 5900 2750
Wire Wire Line
	6000 3050 6000 2750
Wire Wire Line
	6100 3050 6100 2750
Text Label 5800 2750 3    50   ~ 0
3V3
Text Label 5900 2750 3    50   ~ 0
GND
Text Label 6000 2750 3    50   ~ 0
SCL
Text Label 6100 2750 3    50   ~ 0
SDA
$Comp
L Connector_Generic:Conn_01x08 J?
U 1 1 601E0809
P 10750 1450
F 0 "J?" H 10830 1442 50  0000 L CNN
F 1 "Conn_01x08" H 10830 1351 50  0000 L CNN
F 2 "" H 10750 1450 50  0001 C CNN
F 3 "~" H 10750 1450 50  0001 C CNN
	1    10750 1450
	1    0    0    -1  
$EndComp
Text Label 9750 3550 2    50   ~ 0
ESP_RX2
Text Label 9750 3450 2    50   ~ 0
ESP_TX2
$Comp
L Device:R_Small R?
U 1 1 601E5272
P 9900 3550
F 0 "R?" V 9900 3550 50  0000 C CNN
F 1 "220" V 10000 3550 50  0000 C CNN
F 2 "" H 9900 3550 50  0001 C CNN
F 3 "~" H 9900 3550 50  0001 C CNN
	1    9900 3550
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 601E590B
P 9900 3450
F 0 "R?" V 9900 3450 50  0000 C CNN
F 1 "220" V 9800 3450 50  0000 C CNN
F 2 "" H 9900 3450 50  0001 C CNN
F 3 "~" H 9900 3450 50  0001 C CNN
	1    9900 3450
	0    1    1    0   
$EndComp
Wire Wire Line
	9400 3450 9800 3450
Wire Wire Line
	9400 3550 9800 3550
Wire Wire Line
	10300 3550 10000 3550
Wire Wire Line
	10300 3450 10000 3450
$EndSCHEMATC
