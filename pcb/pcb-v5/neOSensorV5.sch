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
L neosensor_addon:OLED1.3_I2C U2
U 1 1 601B3200
P 6250 5550
F 0 "U2" H 6728 5554 50  0000 L CNN
F 1 "OLED1.3_I2C" H 6728 5463 50  0000 L CNN
F 2 "neosensor_addon:OLED1.3_I2C" H 6250 5550 50  0001 C CNN
F 3 "https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf" H 6250 5550 50  0001 C CNN
	1    6250 5550
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:ESP32_dev_30p U3
U 1 1 601B39D1
P 8950 3400
F 0 "U3" H 8950 4375 50  0000 C CNN
F 1 "ESP32_dev_30p" H 8950 4284 50  0000 C CNN
F 2 "neosensor_addon:ESP32_dev_30p" V 8950 3450 50  0001 C CIN
F 3 "" H 8950 3800 50  0001 C CNN
	1    8950 3400
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U4
U 1 1 601B4C77
P 8950 5350
F 0 "U4" H 8950 5592 50  0000 C CNN
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
L Mechanical:MountingHole_Pad H1
U 1 1 601BFC00
P 5550 7050
F 0 "H1" V 5504 7200 50  0000 L CNN
F 1 "3.2mm for MH3 screw" V 5595 7200 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 5550 7050 50  0001 C CNN
F 3 "~" H 5550 7050 50  0001 C CNN
	1    5550 7050
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 601C095B
P 5550 7350
F 0 "H2" V 5504 7500 50  0000 L CNN
F 1 "3.2mm for MH3 screw" V 5595 7500 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 5550 7350 50  0001 C CNN
F 3 "~" H 5550 7350 50  0001 C CNN
	1    5550 7350
	0    1    1    0   
$EndComp
Text Notes 4950 6700 0    100  ~ 0
Board mounting holes
$Comp
L power:GND #PWR06
U 1 1 601C1C3A
P 8950 5900
F 0 "#PWR06" H 8950 5650 50  0001 C CNN
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
L power:+5V #PWR05
U 1 1 601C467B
P 8050 4050
F 0 "#PWR05" H 8050 3900 50  0001 C CNN
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
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 601C66E2
P 5750 1250
F 0 "J1" H 5700 1450 50  0000 L CNN
F 1 "Conn_01x04" H 5550 950 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5750 1250 50  0001 C CNN
F 3 "~" H 5750 1250 50  0001 C CNN
	1    5750 1250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J3
U 1 1 601CACB8
P 7500 1350
F 0 "J3" H 7450 1650 50  0000 L CNN
F 1 "Conn_01x05" H 7300 1050 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7500 1350 50  0001 C CNN
F 3 "~" H 7500 1350 50  0001 C CNN
	1    7500 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J4
U 1 1 601CBCDB
P 8450 1350
F 0 "J4" H 8400 1650 50  0000 L CNN
F 1 "Conn_01x05" H 8250 1050 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8450 1350 50  0001 C CNN
F 3 "~" H 8450 1350 50  0001 C CNN
	1    8450 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1_Small C2
U 1 1 601D0AC1
P 9350 5600
F 0 "C2" H 9441 5646 50  0000 L CNN
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
Text Label 10725 2750 2    50   ~ 0
SCL
Wire Wire Line
	9400 3050 10000 3050
Text Label 10750 3050 2    50   ~ 0
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
L Connector_Generic:Conn_01x04 J2
U 1 1 601D570B
P 6600 1250
F 0 "J2" H 6550 1450 50  0000 L CNN
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
	6200 5300 6200 5000
Wire Wire Line
	6300 5300 6300 5000
Wire Wire Line
	6400 5300 6400 5000
Text Label 6100 5000 3    50   ~ 0
+3V3
Text Label 6200 5000 3    50   ~ 0
GND
Text Label 6300 5000 3    50   ~ 0
SCL
Text Label 6400 5000 3    50   ~ 0
SDA
$Comp
L Device:R_Small R6
U 1 1 601E5272
P 9900 3550
F 0 "R6" V 9900 3550 50  0000 C CNN
F 1 "470" V 9850 3400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 9900 3550 50  0001 C CNN
F 3 "~" H 9900 3550 50  0001 C CNN
	1    9900 3550
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R5
U 1 1 601E590B
P 9900 3450
F 0 "R5" V 9900 3450 50  0000 C CNN
F 1 "470" V 9850 3300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 9900 3450 50  0001 C CNN
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
L power:+3V3 #PWR07
U 1 1 601C80B4
P 9400 5150
F 0 "#PWR07" H 9400 5000 50  0001 C CNN
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
L Device:R_Small R4
U 1 1 601C96F3
P 9900 2550
F 0 "R4" V 9900 2550 50  0000 C CNN
F 1 "4K7" V 9925 2400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 9900 2550 50  0001 C CNN
F 3 "~" H 9900 2550 50  0001 C CNN
	1    9900 2550
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R8
U 1 1 601C9D65
P 10000 2550
F 0 "R8" V 10000 2550 50  0000 C CNN
F 1 "4K7" V 10025 2400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 10000 2550 50  0001 C CNN
F 3 "~" H 10000 2550 50  0001 C CNN
	1    10000 2550
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR08
U 1 1 601CBB27
P 9900 2175
F 0 "#PWR08" H 9900 2025 50  0001 C CNN
F 1 "+3V3" H 9915 2348 50  0000 C CNN
F 2 "" H 9900 2175 50  0001 C CNN
F 3 "" H 9900 2175 50  0001 C CNN
	1    9900 2175
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 2175 9900 2250
Wire Wire Line
	9900 2250 10000 2250
Wire Wire Line
	10000 2250 10000 2450
Connection ~ 9900 2250
Wire Wire Line
	9900 2250 9900 2450
Wire Wire Line
	9900 2650 9900 2750
Connection ~ 9900 2750
Wire Wire Line
	9900 2750 10725 2750
Wire Wire Line
	9400 2750 9900 2750
Wire Wire Line
	10000 2650 10000 3050
Connection ~ 10000 3050
Wire Wire Line
	10000 3050 10750 3050
Text Notes 6000 4800 0    100  ~ 0
Display
Text Notes 8400 4800 0    100  ~ 0
Power supply
Text Notes 6050 3100 0    50   ~ 0
Notes:\n- GPIO13 reserved for noise sensor\n- GPIO5 reserved for LED + clear sw\n- GPIO4 reserved for IR sensor\n- ADC2 pins cannot be used when WiFi in use\n- prefer ADC1 pins (GPIO32 --> 39)\n- GPIO1 & GPIO3 are serial0 (prog.)\n- +3V3 on EN pin is optional
$Comp
L Device:C_Small C1
U 1 1 60206A1D
P 8500 5600
F 0 "C1" H 8592 5646 50  0000 L CNN
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
L Connector:TestPoint TP1
U 1 1 6022057A
P 10150 3350
F 0 "TP1" H 10208 3468 50  0000 L CNN
F 1 "tp_tx2" H 10208 3377 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.50mm_Drill1.0mm" H 10350 3350 50  0001 C CNN
F 3 "~" H 10350 3350 50  0001 C CNN
	1    10150 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP2
U 1 1 60220FFE
P 10400 3350
F 0 "TP2" H 10458 3468 50  0000 L CNN
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
L Connector_Generic:Conn_01x05 J5
U 1 1 60238DBA
P 10850 1550
F 0 "J5" H 10800 1850 50  0000 L CNN
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
TODO:\n
Wire Wire Line
	10750 3150 10400 3150
Text Label 10750 3150 2    50   ~ 0
INT
$Comp
L neosensor_addon:HC-SR01 U1
U 1 1 601D6A06
P 4250 5800
F 0 "U1" H 4578 5904 50  0000 L CNN
F 1 "HC-SR01" H 4578 5813 50  0000 L CNN
F 2 "neosensor_addon:PIR_HC-SR01" H 4250 5800 50  0001 C CNN
F 3 "" H 4250 5800 50  0001 C CNN
	1    4250 5800
	1    0    0    -1  
$EndComp
Text Notes 3700 4850 0    100  ~ 0
PIR sensor
Wire Wire Line
	4150 5200 4150 5500
Wire Wire Line
	4250 5200 4250 5500
Wire Wire Line
	4350 5200 4350 5500
Text Label 4150 5200 3    50   ~ 0
+5V
Text Label 4250 5200 3    50   ~ 0
IR
Text Label 4350 5200 3    50   ~ 0
GND
Text Label 10750 3650 2    50   ~ 0
IR
Text Notes 3700 5050 0    50   ~ 0
HC-SR01 V+ accepts from 5 to 20v\nOUT pin is 3.3 TTL
$Comp
L Device:R_Small R9
U 1 1 601EA941
P 10150 3850
F 0 "R9" V 10150 3850 50  0000 C CNN
F 1 "15k" V 10075 3850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 10150 3850 50  0001 C CNN
F 3 "~" H 10150 3850 50  0001 C CNN
	1    10150 3850
	-1   0    0    1   
$EndComp
Wire Wire Line
	10150 3750 10150 3650
Connection ~ 10150 3650
Wire Wire Line
	10150 3650 10750 3650
$Comp
L power:GND #PWR09
U 1 1 601ECAC3
P 10150 4050
F 0 "#PWR09" H 10150 3800 50  0001 C CNN
F 1 "GND" H 10155 3877 50  0000 C CNN
F 2 "" H 10150 4050 50  0001 C CNN
F 3 "" H 10150 4050 50  0001 C CNN
	1    10150 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 4050 10150 3950
$Comp
L Transistor_FET:2N7002 Q1
U 1 1 601EF3E3
P 1325 5500
F 0 "Q1" H 1530 5454 50  0000 L CNN
F 1 "2N7002" H 1530 5545 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 1525 5425 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 1325 5500 50  0001 L CNN
	1    1325 5500
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 601F9492
P 1225 5975
F 0 "#PWR02" H 1225 5725 50  0001 C CNN
F 1 "GND" H 1230 5802 50  0000 C CNN
F 2 "" H 1225 5975 50  0001 C CNN
F 3 "" H 1225 5975 50  0001 C CNN
	1    1225 5975
	1    0    0    -1  
$EndComp
Wire Wire Line
	1225 5975 1225 5850
$Comp
L Device:R_Small R1
U 1 1 601FB734
P 1225 5075
F 0 "R1" V 1225 5075 50  0000 C CNN
F 1 "68" V 1150 5075 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1225 5075 50  0001 C CNN
F 3 "~" H 1225 5075 50  0001 C CNN
	1    1225 5075
	-1   0    0    1   
$EndComp
Wire Wire Line
	1225 5175 1225 5300
$Comp
L Device:LED_Small_ALT D1
U 1 1 601FE5C8
P 1225 4775
F 0 "D1" V 1271 4705 50  0000 R CNN
F 1 "Red" H 1300 4900 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1225 4775 50  0001 C CNN
F 3 "~" V 1225 4775 50  0001 C CNN
	1    1225 4775
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1225 4975 1225 4875
$Comp
L power:+5V #PWR01
U 1 1 602009CC
P 1225 4575
F 0 "#PWR01" H 1225 4425 50  0001 C CNN
F 1 "+5V" H 1240 4748 50  0000 C CNN
F 2 "" H 1225 4575 50  0001 C CNN
F 3 "" H 1225 4575 50  0001 C CNN
	1    1225 4575
	1    0    0    -1  
$EndComp
Wire Wire Line
	1225 4675 1225 4575
$Comp
L Device:R_Small R3
U 1 1 602030FF
P 1825 5500
F 0 "R3" V 1825 5500 50  0000 C CNN
F 1 "470" V 1750 5500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1825 5500 50  0001 C CNN
F 3 "~" H 1825 5500 50  0001 C CNN
	1    1825 5500
	0    1    1    0   
$EndComp
Wire Wire Line
	1525 5500 1600 5500
$Comp
L Device:R_Small R2
U 1 1 60219586
P 1600 5675
F 0 "R2" V 1600 5675 50  0000 C CNN
F 1 "15k" V 1525 5675 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1600 5675 50  0001 C CNN
F 3 "~" H 1600 5675 50  0001 C CNN
	1    1600 5675
	-1   0    0    1   
$EndComp
Wire Wire Line
	1600 5775 1600 5850
Wire Wire Line
	1600 5850 1225 5850
Connection ~ 1225 5850
Wire Wire Line
	1225 5850 1225 5700
Wire Wire Line
	1600 5575 1600 5500
Connection ~ 1600 5500
Wire Wire Line
	1600 5500 1725 5500
Wire Wire Line
	1925 5500 2225 5500
Text Label 2225 5500 2    50   ~ 0
LED
$Comp
L neosensor_addon:G71Y SW1
U 1 1 60223186
P 1950 4825
F 0 "SW1" H 1950 5050 50  0000 C CNN
F 1 "clear" H 1950 4959 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 1950 4825 50  0001 C CIN
F 3 "" H 1950 4825 50  0001 C CNN
	1    1950 4825
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 60224902
P 1750 5000
F 0 "#PWR03" H 1750 4750 50  0001 C CNN
F 1 "GND" H 1755 4827 50  0000 C CNN
F 2 "" H 1750 5000 50  0001 C CNN
F 3 "" H 1750 5000 50  0001 C CNN
	1    1750 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 4925 1750 5000
Wire Wire Line
	1750 4825 1600 4825
Wire Wire Line
	1600 4825 1600 5500
$Comp
L power:+3V3 #PWR04
U 1 1 60229D6D
P 2225 4750
F 0 "#PWR04" H 2225 4600 50  0001 C CNN
F 1 "+3V3" H 2240 4923 50  0000 C CNN
F 2 "" H 2225 4750 50  0001 C CNN
F 3 "" H 2225 4750 50  0001 C CNN
	1    2225 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 4825 2225 4825
Wire Wire Line
	2225 4825 2225 4750
Text Notes 1150 4200 0    100  ~ 0
LED and SWITCHES
$Comp
L neosensor_addon:G71Y SW2
U 1 1 601EB9F1
P 1150 6600
F 0 "SW2" H 1150 6825 50  0000 C CNN
F 1 "+" H 1150 6734 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 1150 6600 50  0001 C CIN
F 3 "" H 1150 6600 50  0001 C CNN
	1    1150 6600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 601EBDD7
P 1150 7075
F 0 "#PWR011" H 1150 6825 50  0001 C CNN
F 1 "GND" H 1155 6902 50  0000 C CNN
F 2 "" H 1150 7075 50  0001 C CNN
F 3 "" H 1150 7075 50  0001 C CNN
	1    1150 7075
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR010
U 1 1 601EBDE4
P 800 6550
F 0 "#PWR010" H 800 6400 50  0001 C CNN
F 1 "+3V3" H 815 6723 50  0000 C CNN
F 2 "" H 800 6550 50  0001 C CNN
F 3 "" H 800 6550 50  0001 C CNN
	1    800  6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  6550 800  6600
Wire Wire Line
	800  6600 950  6600
Wire Wire Line
	950  6700 900  6700
Wire Wire Line
	900  6700 900  7000
Wire Wire Line
	900  7000 1150 7000
Wire Wire Line
	1150 7000 1150 7075
$Comp
L Device:R_Small R10
U 1 1 6021D332
P 1450 6800
F 0 "R10" V 1450 6800 50  0000 C CNN
F 1 "15k" V 1375 6800 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1450 6800 50  0001 C CNN
F 3 "~" H 1450 6800 50  0001 C CNN
	1    1450 6800
	-1   0    0    1   
$EndComp
Wire Wire Line
	1150 7000 1450 7000
Wire Wire Line
	1450 7000 1450 6900
Connection ~ 1150 7000
Wire Wire Line
	1350 6600 1450 6600
Wire Wire Line
	1450 6600 1450 6700
Wire Wire Line
	1550 6600 1450 6600
Connection ~ 1450 6600
Wire Wire Line
	1750 6600 1850 6600
Text Label 2050 6600 2    50   ~ 0
SW2
$Comp
L neosensor_addon:G71Y SW3
U 1 1 60238ADD
P 2600 6600
F 0 "SW3" H 2600 6825 50  0000 C CNN
F 1 "-" H 2600 6734 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 2600 6600 50  0001 C CIN
F 3 "" H 2600 6600 50  0001 C CNN
	1    2600 6600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 60238D6F
P 2600 7050
F 0 "#PWR013" H 2600 6800 50  0001 C CNN
F 1 "GND" H 2605 6877 50  0000 C CNN
F 2 "" H 2600 7050 50  0001 C CNN
F 3 "" H 2600 7050 50  0001 C CNN
	1    2600 7050
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR012
U 1 1 60238D79
P 2250 6550
F 0 "#PWR012" H 2250 6400 50  0001 C CNN
F 1 "+3V3" H 2265 6723 50  0000 C CNN
F 2 "" H 2250 6550 50  0001 C CNN
F 3 "" H 2250 6550 50  0001 C CNN
	1    2250 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 6550 2250 6600
Wire Wire Line
	2250 6600 2400 6600
Wire Wire Line
	2400 6700 2350 6700
Wire Wire Line
	2350 6700 2350 7000
Wire Wire Line
	2350 7000 2600 7000
Wire Wire Line
	2600 7000 2600 7050
Wire Wire Line
	2600 7000 2900 7000
Wire Wire Line
	2900 7000 2900 6900
Connection ~ 2600 7000
Wire Wire Line
	2800 6600 2900 6600
Wire Wire Line
	2900 6600 2900 6700
Wire Wire Line
	3000 6600 2900 6600
Connection ~ 2900 6600
Wire Wire Line
	3200 6600 3275 6600
Text Label 3500 6600 2    50   ~ 0
SW3
Wire Wire Line
	9700 3250 9400 3250
Wire Wire Line
	9700 2650 9400 2650
Text Label 9700 3250 2    50   ~ 0
SW2
Text Label 9700 2650 2    50   ~ 0
SW3
Text Notes 2175 825  0    100  ~ 0
ANALOG inputs
Text Notes 2175 1175 0    50   ~ 0
Translate 5v analog input to 3v\n(NOT 3.3v due to ESP32 non linearity)\nV+ = 5v x 33k / 55k = 3v\nop. amp as follower
$Comp
L Amplifier_Operational:LM2902 U5
U 1 1 6025DC0F
P 2850 1750
F 0 "U5" H 2850 1550 50  0000 C CNN
F 1 "LM2902" H 2850 2000 50  0000 C CNN
F 2 "Package_SO:SO-14_5.3x10.2mm_P1.27mm" H 2800 1850 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 2900 1950 50  0001 C CNN
	1    2850 1750
	1    0    0    1   
$EndComp
$Comp
L Device:R_Small R16
U 1 1 6025F63C
P 2450 2050
F 0 "R16" V 2450 2050 50  0000 C CNN
F 1 "33k" V 2375 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 2450 2050 50  0001 C CNN
F 3 "~" H 2450 2050 50  0001 C CNN
	1    2450 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	2550 1650 2450 1650
Wire Wire Line
	2450 1650 2450 1400
Wire Wire Line
	2450 1400 3250 1400
Wire Wire Line
	3250 1400 3250 1750
Wire Wire Line
	3250 1750 3175 1750
Wire Wire Line
	3250 1750 3550 1750
Connection ~ 3250 1750
Wire Wire Line
	2550 1850 2450 1850
Wire Wire Line
	2450 1850 2450 1950
$Comp
L power:GND #PWR0101
U 1 1 60288F06
P 2450 2250
F 0 "#PWR0101" H 2450 2000 50  0001 C CNN
F 1 "GND" H 2455 2077 50  0000 C CNN
F 2 "" H 2450 2250 50  0001 C CNN
F 3 "" H 2450 2250 50  0001 C CNN
	1    2450 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 2150 2450 2250
$Comp
L Device:R_Small R15
U 1 1 6028ECAA
P 2300 1850
F 0 "R15" V 2300 1850 50  0000 C CNN
F 1 "22k" V 2225 1850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 2300 1850 50  0001 C CNN
F 3 "~" H 2300 1850 50  0001 C CNN
	1    2300 1850
	0    -1   -1   0   
$EndComp
Connection ~ 2450 1850
Wire Wire Line
	2400 1850 2450 1850
$Comp
L Connector_Generic:Conn_01x03 J6
U 1 1 602DDA42
P 1400 1850
F 0 "J6" H 1318 2167 50  0000 C CNN
F 1 "Conn_01x03" H 1318 2076 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 1400 1850 50  0001 C CNN
F 3 "~" H 1400 1850 50  0001 C CNN
	1    1400 1850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1600 1750 1900 1750
Text Label 1900 1750 2    50   ~ 0
+5V
$Comp
L power:GND #PWR0102
U 1 1 602F9EE3
P 1700 2050
F 0 "#PWR0102" H 1700 1800 50  0001 C CNN
F 1 "GND" H 1705 1877 50  0000 C CNN
F 2 "" H 1700 2050 50  0001 C CNN
F 3 "" H 1700 2050 50  0001 C CNN
	1    1700 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 1950 1700 1950
Wire Wire Line
	1700 1950 1700 2050
$Comp
L Amplifier_Operational:LM2902 U5
U 5 1 6030251D
P 1000 2450
F 0 "U5" H 950 2450 50  0000 L CNN
F 1 "LM2902" V 800 2300 50  0000 L CNN
F 2 "Package_SO:SO-14_5.3x10.2mm_P1.27mm" H 950 2550 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 1050 2650 50  0001 C CNN
	5    1000 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 60334590
P 900 2850
F 0 "#PWR0103" H 900 2600 50  0001 C CNN
F 1 "GND" H 905 2677 50  0000 C CNN
F 2 "" H 900 2850 50  0001 C CNN
F 3 "" H 900 2850 50  0001 C CNN
	1    900  2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  2850 900  2800
$Comp
L power:+3V3 #PWR0104
U 1 1 6033AFD7
P 900 2050
F 0 "#PWR0104" H 900 1900 50  0001 C CNN
F 1 "+3V3" H 915 2223 50  0000 C CNN
F 2 "" H 900 2050 50  0001 C CNN
F 3 "" H 900 2050 50  0001 C CNN
	1    900  2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  2050 900  2100
$Comp
L Device:R_Small R19
U 1 1 60362791
P 2450 3250
F 0 "R19" V 2450 3250 50  0000 C CNN
F 1 "33k" V 2375 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 2450 3250 50  0001 C CNN
F 3 "~" H 2450 3250 50  0001 C CNN
	1    2450 3250
	-1   0    0    1   
$EndComp
Wire Wire Line
	2550 2850 2450 2850
Wire Wire Line
	2450 2850 2450 2600
Wire Wire Line
	2450 2600 3250 2600
Wire Wire Line
	3250 2600 3250 2950
Wire Wire Line
	3250 2950 3150 2950
Wire Wire Line
	3250 2950 3550 2950
Connection ~ 3250 2950
Wire Wire Line
	2550 3050 2450 3050
Wire Wire Line
	2450 3050 2450 3150
$Comp
L power:GND #PWR015
U 1 1 603627A4
P 2450 3450
F 0 "#PWR015" H 2450 3200 50  0001 C CNN
F 1 "GND" H 2455 3277 50  0000 C CNN
F 2 "" H 2450 3450 50  0001 C CNN
F 3 "" H 2450 3450 50  0001 C CNN
	1    2450 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 3350 2450 3450
$Comp
L Device:R_Small R18
U 1 1 603627AF
P 2300 3050
F 0 "R18" V 2300 3050 50  0000 C CNN
F 1 "22k" V 2225 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 2300 3050 50  0001 C CNN
F 3 "~" H 2300 3050 50  0001 C CNN
	1    2300 3050
	0    -1   -1   0   
$EndComp
Connection ~ 2450 3050
Wire Wire Line
	2400 3050 2450 3050
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 603627C7
P 1400 3050
F 0 "J7" H 1318 3367 50  0000 C CNN
F 1 "Conn_01x03" H 1318 3276 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 1400 3050 50  0001 C CNN
F 3 "~" H 1400 3050 50  0001 C CNN
	1    1400 3050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1600 2950 1900 2950
Text Label 1900 2950 2    50   ~ 0
+5V
$Comp
L power:GND #PWR014
U 1 1 603627D3
P 1700 3250
F 0 "#PWR014" H 1700 3000 50  0001 C CNN
F 1 "GND" H 1705 3077 50  0000 C CNN
F 2 "" H 1700 3250 50  0001 C CNN
F 3 "" H 1700 3250 50  0001 C CNN
	1    1700 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 3150 1700 3150
Wire Wire Line
	1700 3150 1700 3250
$Comp
L Device:C_Small C3
U 1 1 603D61F0
P 1200 2450
F 0 "C3" H 1292 2496 50  0000 L CNN
F 1 "100nF" H 1292 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1200 2450 50  0001 C CNN
F 3 "~" H 1200 2450 50  0001 C CNN
	1    1200 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 2350 1200 2100
Wire Wire Line
	1200 2100 900  2100
Connection ~ 900  2100
Wire Wire Line
	900  2100 900  2150
Wire Wire Line
	1200 2550 1200 2800
Wire Wire Line
	1200 2800 900  2800
Connection ~ 900  2800
Wire Wire Line
	900  2800 900  2750
Wire Wire Line
	8200 2750 8500 2750
Wire Wire Line
	8200 2850 8500 2850
Text Label 8200 2750 0    50   ~ 0
ADC1_0
Text Label 8200 2850 0    50   ~ 0
ADC1_3
Text Label 3550 1750 2    50   ~ 0
ADC1_0
Text Label 3550 2950 2    50   ~ 0
ADC1_3
Text Label 1900 1850 2    50   ~ 0
ANA_0
Text Label 1900 3050 2    50   ~ 0
ANA_1
Wire Wire Line
	1600 1850 2200 1850
Wire Wire Line
	1600 3050 2200 3050
Wire Wire Line
	9400 3650 10150 3650
$Comp
L Device:R_Small R7
U 1 1 602171FA
P 1650 6600
F 0 "R7" V 1650 6600 50  0000 C CNN
F 1 "15k" V 1575 6600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1650 6600 50  0001 C CNN
F 3 "~" H 1650 6600 50  0001 C CNN
	1    1650 6600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R12
U 1 1 60238D89
P 2900 6800
F 0 "R12" V 2900 6800 50  0000 C CNN
F 1 "15k" V 2825 6800 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 2900 6800 50  0001 C CNN
F 3 "~" H 2900 6800 50  0001 C CNN
	1    2900 6800
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R11
U 1 1 60217A45
P 3100 6600
F 0 "R11" V 3100 6600 50  0000 C CNN
F 1 "15k" V 3025 6600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 3100 6600 50  0001 C CNN
F 3 "~" H 3100 6600 50  0001 C CNN
	1    3100 6600
	0    -1   -1   0   
$EndComp
$Comp
L Device:C_Small C4
U 1 1 602190D3
P 1850 6800
F 0 "C4" H 1942 6846 50  0000 L CNN
F 1 "1uF" H 1942 6755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1850 6800 50  0001 C CNN
F 3 "~" H 1850 6800 50  0001 C CNN
	1    1850 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 6700 1850 6600
Connection ~ 1850 6600
Wire Wire Line
	1850 6600 2050 6600
Wire Wire Line
	1850 6900 1850 7000
Wire Wire Line
	1850 7000 1450 7000
Connection ~ 1450 7000
$Comp
L Device:C_Small C5
U 1 1 6022DD92
P 3275 6800
F 0 "C5" H 3367 6846 50  0000 L CNN
F 1 "1uF" H 3367 6755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 3275 6800 50  0001 C CNN
F 3 "~" H 3275 6800 50  0001 C CNN
	1    3275 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3275 6700 3275 6600
Connection ~ 3275 6600
Wire Wire Line
	3275 6600 3500 6600
Wire Wire Line
	3275 7000 2900 7000
Wire Wire Line
	3275 6900 3275 7000
Connection ~ 2900 7000
NoConn ~ 9400 4050
$Comp
L Amplifier_Operational:LM2902 U5
U 2 1 6026EAC1
P 2850 2950
F 0 "U5" H 2850 2750 50  0000 C CNN
F 1 "LM2902" H 2850 3200 50  0000 C CNN
F 2 "Package_SO:SO-14_5.3x10.2mm_P1.27mm" H 2800 3050 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 2900 3150 50  0001 C CNN
	2    2850 2950
	1    0    0    1   
$EndComp
$Comp
L Amplifier_Operational:LM2902 U5
U 3 1 602854BD
P 4925 2125
F 0 "U5" H 4925 1925 50  0000 C CNN
F 1 "LM2902" H 4925 2375 50  0000 C CNN
F 2 "Package_SO:SO-14_5.3x10.2mm_P1.27mm" H 4875 2225 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 4975 2325 50  0001 C CNN
	3    4925 2125
	1    0    0    1   
$EndComp
$Comp
L Device:R_Small R17
U 1 1 60285967
P 4525 2425
F 0 "R17" V 4525 2425 50  0000 C CNN
F 1 "33k" V 4450 2425 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 4525 2425 50  0001 C CNN
F 3 "~" H 4525 2425 50  0001 C CNN
	1    4525 2425
	-1   0    0    1   
$EndComp
Wire Wire Line
	4625 2025 4525 2025
Wire Wire Line
	4525 2025 4525 1775
Wire Wire Line
	4525 1775 5325 1775
Wire Wire Line
	5325 1775 5325 2125
Wire Wire Line
	5325 2125 5225 2125
Wire Wire Line
	5325 2125 5625 2125
Connection ~ 5325 2125
Wire Wire Line
	4625 2225 4525 2225
Wire Wire Line
	4525 2225 4525 2325
$Comp
L power:GND #PWR018
U 1 1 6028597A
P 4525 2625
F 0 "#PWR018" H 4525 2375 50  0001 C CNN
F 1 "GND" H 4530 2452 50  0000 C CNN
F 2 "" H 4525 2625 50  0001 C CNN
F 3 "" H 4525 2625 50  0001 C CNN
	1    4525 2625
	1    0    0    -1  
$EndComp
Wire Wire Line
	4525 2525 4525 2625
$Comp
L Device:R_Small R13
U 1 1 60285985
P 4375 2225
F 0 "R13" V 4375 2225 50  0000 C CNN
F 1 "22k" V 4300 2225 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 4375 2225 50  0001 C CNN
F 3 "~" H 4375 2225 50  0001 C CNN
	1    4375 2225
	0    -1   -1   0   
$EndComp
Connection ~ 4525 2225
Wire Wire Line
	4475 2225 4525 2225
$Comp
L Connector_Generic:Conn_01x03 J8
U 1 1 60285991
P 3475 2225
F 0 "J8" H 3393 2542 50  0000 C CNN
F 1 "Conn_01x03" H 3393 2451 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3475 2225 50  0001 C CNN
F 3 "~" H 3475 2225 50  0001 C CNN
	1    3475 2225
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3675 2125 3975 2125
Text Label 3975 2125 2    50   ~ 0
+5V
$Comp
L power:GND #PWR016
U 1 1 6028599D
P 3775 2425
F 0 "#PWR016" H 3775 2175 50  0001 C CNN
F 1 "GND" H 3780 2252 50  0000 C CNN
F 2 "" H 3775 2425 50  0001 C CNN
F 3 "" H 3775 2425 50  0001 C CNN
	1    3775 2425
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 2325 3775 2325
Wire Wire Line
	3775 2325 3775 2425
Text Label 5625 2125 2    50   ~ 0
ADC1_6
Text Label 3975 2225 2    50   ~ 0
ANA_2
Wire Wire Line
	3675 2225 4275 2225
Wire Wire Line
	8200 2950 8500 2950
Text Label 8200 2950 0    50   ~ 0
ADC1_6
Wire Wire Line
	8200 3050 8500 3050
Text Label 8200 3050 0    50   ~ 0
ADC1_7
NoConn ~ 8500 3150
NoConn ~ 8500 3250
NoConn ~ 8500 3350
NoConn ~ 8500 3450
NoConn ~ 8500 3550
NoConn ~ 8500 3650
NoConn ~ 8500 3750
NoConn ~ 9400 3850
$Comp
L Amplifier_Operational:LM2902 U5
U 4 1 6030FAE4
P 4925 3325
F 0 "U5" H 4925 3125 50  0000 C CNN
F 1 "LM2902" H 4925 3575 50  0000 C CNN
F 2 "Package_SO:SO-14_5.3x10.2mm_P1.27mm" H 4875 3425 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2902-n.pdf" H 4975 3525 50  0001 C CNN
	4    4925 3325
	1    0    0    1   
$EndComp
$Comp
L Device:R_Small R20
U 1 1 6031019C
P 4525 3625
F 0 "R20" V 4525 3625 50  0000 C CNN
F 1 "33k" V 4450 3625 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 4525 3625 50  0001 C CNN
F 3 "~" H 4525 3625 50  0001 C CNN
	1    4525 3625
	-1   0    0    1   
$EndComp
Wire Wire Line
	4625 3225 4525 3225
Wire Wire Line
	4525 3225 4525 2975
Wire Wire Line
	4525 2975 5325 2975
Wire Wire Line
	5325 2975 5325 3325
Wire Wire Line
	5325 3325 5250 3325
Wire Wire Line
	5325 3325 5625 3325
Connection ~ 5325 3325
Wire Wire Line
	4625 3425 4525 3425
Wire Wire Line
	4525 3425 4525 3525
$Comp
L power:GND #PWR019
U 1 1 603101AF
P 4525 3825
F 0 "#PWR019" H 4525 3575 50  0001 C CNN
F 1 "GND" H 4530 3652 50  0000 C CNN
F 2 "" H 4525 3825 50  0001 C CNN
F 3 "" H 4525 3825 50  0001 C CNN
	1    4525 3825
	1    0    0    -1  
$EndComp
Wire Wire Line
	4525 3725 4525 3825
$Comp
L Device:R_Small R14
U 1 1 603101BA
P 4375 3425
F 0 "R14" V 4375 3425 50  0000 C CNN
F 1 "22k" V 4300 3425 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 4375 3425 50  0001 C CNN
F 3 "~" H 4375 3425 50  0001 C CNN
	1    4375 3425
	0    -1   -1   0   
$EndComp
Connection ~ 4525 3425
Wire Wire Line
	4475 3425 4525 3425
$Comp
L Connector_Generic:Conn_01x03 J9
U 1 1 603101C6
P 3475 3425
F 0 "J9" H 3393 3742 50  0000 C CNN
F 1 "Conn_01x03" H 3393 3651 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3475 3425 50  0001 C CNN
F 3 "~" H 3475 3425 50  0001 C CNN
	1    3475 3425
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3675 3325 3975 3325
Text Label 3975 3325 2    50   ~ 0
+5V
$Comp
L power:GND #PWR017
U 1 1 603101D2
P 3775 3625
F 0 "#PWR017" H 3775 3375 50  0001 C CNN
F 1 "GND" H 3780 3452 50  0000 C CNN
F 2 "" H 3775 3625 50  0001 C CNN
F 3 "" H 3775 3625 50  0001 C CNN
	1    3775 3625
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 3525 3775 3525
Wire Wire Line
	3775 3525 3775 3625
Text Label 5625 3325 2    50   ~ 0
ADC1_7
Text Label 3975 3425 2    50   ~ 0
ANA_3
Wire Wire Line
	3675 3425 4275 3425
$Comp
L Connector:TestPoint TP4
U 1 1 603873FB
P 3175 1625
F 0 "TP4" H 3233 1743 50  0000 L CNN
F 1 "tp_an0" H 3233 1652 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.50mm_Drill1.0mm" H 3375 1625 50  0001 C CNN
F 3 "~" H 3375 1625 50  0001 C CNN
	1    3175 1625
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP3
U 1 1 603885E4
P 3150 2850
F 0 "TP3" H 3208 2968 50  0000 L CNN
F 1 "tp_an1" H 3208 2877 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.50mm_Drill1.0mm" H 3350 2850 50  0001 C CNN
F 3 "~" H 3350 2850 50  0001 C CNN
	1    3150 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP6
U 1 1 6038950F
P 5250 3225
F 0 "TP6" H 5308 3343 50  0000 L CNN
F 1 "tp_an3" H 5308 3252 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.50mm_Drill1.0mm" H 5450 3225 50  0001 C CNN
F 3 "~" H 5450 3225 50  0001 C CNN
	1    5250 3225
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP5
U 1 1 6038A46F
P 5225 2000
F 0 "TP5" H 5283 2118 50  0000 L CNN
F 1 "tp_an2" H 5283 2027 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.50mm_Drill1.0mm" H 5425 2000 50  0001 C CNN
F 3 "~" H 5425 2000 50  0001 C CNN
	1    5225 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3175 1625 3175 1750
Connection ~ 3175 1750
Wire Wire Line
	3175 1750 3150 1750
Wire Wire Line
	3150 2850 3150 2950
Connection ~ 3150 2950
Wire Wire Line
	5225 2000 5225 2125
Connection ~ 5225 2125
Wire Wire Line
	5250 3225 5250 3325
Connection ~ 5250 3325
Wire Wire Line
	5250 3325 5225 3325
Wire Wire Line
	10175 2250 10000 2250
Connection ~ 10000 2250
Connection ~ 10175 3150
Wire Wire Line
	10175 3150 9400 3150
Wire Wire Line
	10175 2450 10175 2250
Wire Wire Line
	10175 2650 10175 3150
$Comp
L Device:R_Small R21
U 1 1 603D554C
P 10175 2550
F 0 "R21" V 10175 2550 50  0000 C CNN
F 1 "15K" V 10200 2400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 10175 2550 50  0001 C CNN
F 3 "~" H 10175 2550 50  0001 C CNN
	1    10175 2550
	-1   0    0    1   
$EndComp
$Comp
L Connector:TestPoint TP7
U 1 1 6047575F
P 10400 2975
F 0 "TP7" H 10458 3093 50  0000 L CNN
F 1 "tp_INT" H 10458 3002 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.50mm_Drill1.0mm" H 10600 2975 50  0001 C CNN
F 3 "~" H 10600 2975 50  0001 C CNN
	1    10400 2975
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 2975 10400 3150
Connection ~ 10400 3150
Wire Wire Line
	10400 3150 10175 3150
Wire Wire Line
	6100 5300 6100 5000
Text Label 950  7000 0    50   ~ 0
GND
Text Label 2400 7000 0    50   ~ 0
GND
$EndSCHEMATC
