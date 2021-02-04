EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "neOSensor v5"
Date "2021-02-04"
Rev "v5"
Comp "(c) neOCampus / Dr THIEBOLT François"
Comment1 "M2siame 2020-2021 project"
Comment2 "Univ.Tlse3 / IRIT / neOCampus"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L neosensor_addon:OLED1.3_I2C U?
U 1 1 601B3200
P 6250 5550
F 0 "U?" H 6728 5554 50  0000 L CNN
F 1 "OLED1.3_I2C" H 6728 5463 50  0000 L CNN
F 2 "neosensor_addon:OLED1.3_I2C" H 6250 5550 50  0001 C CNN
F 3 "https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf" H 6250 5550 50  0001 C CNN
	1    6250 5550
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
P 8950 5350
F 0 "U?" H 8950 5592 50  0000 C CNN
F 1 "AMS1117-3.3" H 8950 5501 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 8950 5550 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 9050 5100 50  0001 C CNN
	1    8950 5350
	1    0    0    -1  
$EndComp
Text Notes 5950 900  0    100  ~ 0
I2C sensors connectors
Text Notes 8900 850  0    100  ~ 0
Serial sensors connector
$Comp
L Mechanical:MountingHole_Pad H?
U 1 1 601BFC00
P 5550 7050
F 0 "H?" V 5504 7200 50  0000 L CNN
F 1 "3.2mm for MH3 screw" V 5595 7200 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 5550 7050 50  0001 C CNN
F 3 "~" H 5550 7050 50  0001 C CNN
	1    5550 7050
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H?
U 1 1 601C095B
P 5550 7350
F 0 "H?" V 5504 7500 50  0000 L CNN
F 1 "3.2mm for MH3 screw" V 5595 7500 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 5550 7350 50  0001 C CNN
F 3 "~" H 5550 7350 50  0001 C CNN
	1    5550 7350
	0    1    1    0   
$EndComp
Text Notes 4950 6700 0    100  ~ 0
Board mounting holes
$Comp
L power:GND #PWR?
U 1 1 601C1C3A
P 8950 5900
F 0 "#PWR?" H 8950 5650 50  0001 C CNN
F 1 "GND" H 8955 5727 50  0000 C CNN
F 2 "" H 8950 5900 50  0001 C CNN
F 3 "" H 8950 5900 50  0001 C CNN
	1    8950 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 5900 8950 5800
Text Label 8950 5850 1    50   ~ 0
GND
Wire Wire Line
	5100 7050 5450 7050
Wire Wire Line
	5100 7350 5450 7350
Text Label 5100 7050 0    50   ~ 0
GND
Text Label 5100 7350 0    50   ~ 0
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
+5V
Wire Wire Line
	8200 5350 8500 5350
Text Label 8200 5350 0    50   ~ 0
+5V
Wire Wire Line
	9250 5350 9350 5350
Text Label 9700 5350 2    50   ~ 0
+3V3
Wire Wire Line
	9700 4050 9400 4050
Text Label 9700 4050 2    50   ~ 0
+3V3
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 601C66E2
P 5750 1250
F 0 "J?" H 5700 1450 50  0000 L CNN
F 1 "Conn_01x04" H 5550 950 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5750 1250 50  0001 C CNN
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
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7500 1350 50  0001 C CNN
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
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8450 1350 50  0001 C CNN
F 3 "~" H 8450 1350 50  0001 C CNN
	1    8450 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1_Small C?
U 1 1 601D0AC1
P 9350 5600
F 0 "C?" H 9441 5646 50  0000 L CNN
F 1 "470µF 6.3v" H 9441 5555 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 9350 5600 50  0001 C CNN
F 3 "~" H 9350 5600 50  0001 C CNN
	1    9350 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 5500 9350 5350
Connection ~ 9350 5350
Wire Wire Line
	9350 5350 9400 5350
Wire Wire Line
	9350 5700 9350 5800
Wire Wire Line
	9350 5800 8950 5800
Connection ~ 8950 5800
Wire Wire Line
	8950 5800 8950 5650
Text Label 10200 2750 2    50   ~ 0
SCL
Wire Wire Line
	9400 3050 10000 3050
Text Label 10200 3050 2    50   ~ 0
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
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 6600 1250 50  0001 C CNN
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
	6100 5300 6100 5000
Wire Wire Line
	6200 5300 6200 5000
Wire Wire Line
	6300 5300 6300 5000
Wire Wire Line
	6400 5300 6400 5000
Text Label 6100 5000 3    50   ~ 0
3V3
Text Label 6200 5000 3    50   ~ 0
GND
Text Label 6300 5000 3    50   ~ 0
SCL
Text Label 6400 5000 3    50   ~ 0
SDA
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
	10750 3550 10400 3550
Wire Wire Line
	10750 3450 10150 3450
$Comp
L power:+3V3 #PWR?
U 1 1 601C80B4
P 9400 5150
F 0 "#PWR?" H 9400 5000 50  0001 C CNN
F 1 "+3V3" H 9415 5323 50  0000 C CNN
F 2 "" H 9400 5150 50  0001 C CNN
F 3 "" H 9400 5150 50  0001 C CNN
	1    9400 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 5150 9400 5350
Connection ~ 9400 5350
Wire Wire Line
	9400 5350 9700 5350
$Comp
L Device:R_Small R?
U 1 1 601C96F3
P 9900 2550
F 0 "R?" V 9900 2550 50  0000 C CNN
F 1 "4K7" V 10000 2550 50  0000 C CNN
F 2 "" H 9900 2550 50  0001 C CNN
F 3 "~" H 9900 2550 50  0001 C CNN
	1    9900 2550
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 601C9D65
P 10000 2550
F 0 "R?" V 10000 2550 50  0000 C CNN
F 1 "4K7" V 9900 2550 50  0000 C CNN
F 2 "" H 10000 2550 50  0001 C CNN
F 3 "~" H 10000 2550 50  0001 C CNN
	1    10000 2550
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 601CBB27
P 9900 2250
F 0 "#PWR?" H 9900 2100 50  0001 C CNN
F 1 "+3V3" H 9915 2423 50  0000 C CNN
F 2 "" H 9900 2250 50  0001 C CNN
F 3 "" H 9900 2250 50  0001 C CNN
	1    9900 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 2250 9900 2350
Wire Wire Line
	9900 2350 10000 2350
Wire Wire Line
	10000 2350 10000 2450
Connection ~ 9900 2350
Wire Wire Line
	9900 2350 9900 2450
Wire Wire Line
	9900 2650 9900 2750
Connection ~ 9900 2750
Wire Wire Line
	9900 2750 10200 2750
Wire Wire Line
	9400 2750 9900 2750
Wire Wire Line
	10000 2650 10000 3050
Connection ~ 10000 3050
Wire Wire Line
	10000 3050 10200 3050
Text Notes 6000 4800 0    100  ~ 0
Display
Text Notes 8700 4800 0    100  ~ 0
Power supply
Text Notes 6050 3100 0    50   ~ 0
Notes:\n- GPIO13 reserved for noise sensor\n- GPIO5 reserved for LED + clear sw\n- GPIO4 reserved for IR sensor (really?)\n- GPIO1 & GPIO3 are serial0 (prog.)\n- +3V3 on EN pin is optional
$Comp
L Device:C_Small C?
U 1 1 60206A1D
P 8500 5600
F 0 "C?" H 8592 5646 50  0000 L CNN
F 1 "100nF" H 8592 5555 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 8500 5600 50  0001 C CNN
F 3 "~" H 8500 5600 50  0001 C CNN
	1    8500 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 5500 8500 5350
Connection ~ 8500 5350
Wire Wire Line
	8500 5350 8650 5350
Wire Wire Line
	8500 5700 8500 5800
Wire Wire Line
	8500 5800 8950 5800
Wire Wire Line
	8500 2650 8200 2650
Text Label 8200 2650 0    50   ~ 0
+3V3
$Comp
L Connector:TestPoint TP?
U 1 1 6022057A
P 10150 3350
F 0 "TP?" H 10208 3468 50  0000 L CNN
F 1 "tp_tx2" H 10208 3377 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.50mm_Drill1.0mm" H 10350 3350 50  0001 C CNN
F 3 "~" H 10350 3350 50  0001 C CNN
	1    10150 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP?
U 1 1 60220FFE
P 10400 3350
F 0 "TP?" H 10458 3468 50  0000 L CNN
F 1 "tp_rx2" H 10458 3377 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.50mm_Drill1.0mm" H 10600 3350 50  0001 C CNN
F 3 "~" H 10600 3350 50  0001 C CNN
	1    10400 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 3350 10150 3450
Connection ~ 10150 3450
Wire Wire Line
	10150 3450 10000 3450
Wire Wire Line
	10400 3350 10400 3550
Connection ~ 10400 3550
Wire Wire Line
	10400 3550 10000 3550
Text Label 10750 3450 2    50   ~ 0
TX2
Text Label 10750 3550 2    50   ~ 0
RX2
Wire Wire Line
	10350 1350 10650 1350
Wire Wire Line
	10350 1450 10650 1450
Wire Wire Line
	10350 1550 10650 1550
Wire Wire Line
	10350 1650 10650 1650
Wire Wire Line
	10350 1750 10650 1750
Text Label 10350 1350 0    50   ~ 0
+5V
Text Label 10350 1450 0    50   ~ 0
GND
Text Label 10350 1650 0    50   ~ 0
TX2
Text Label 10350 1750 0    50   ~ 0
RX2
$Comp
L Connector_Generic:Conn_01x05 J?
U 1 1 60238DBA
P 10850 1550
F 0 "J?" H 10800 1850 50  0000 L CNN
F 1 "Conn_01x05" H 10650 1250 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 10850 1550 50  0001 C CNN
F 3 "~" H 10850 1550 50  0001 C CNN
	1    10850 1550
	1    0    0    -1  
$EndComp
Text Notes 8900 1150 0    50   ~ 0
Notes about Particule Meters sensors\n- PM_EN is optionnally intended to enable/disable the\nparticule meter (not to have it always on)
Text Label 10350 1550 0    50   ~ 0
PM_EN
NoConn ~ 9400 2850
NoConn ~ 9400 2950
Wire Wire Line
	9700 3750 9400 3750
Text Label 9700 3750 2    50   ~ 0
PM_EN
NoConn ~ 8500 3850
Wire Wire Line
	9400 3350 9700 3350
Text Label 9700 3350 2    50   ~ 0
LED
Text Notes 6050 2100 0    50   ~ 0
TODO:\n- associate INT with an ESP32 pin
$EndSCHEMATC
