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
Wire Wire Line
	9700 4050 9400 4050
Text Label 9700 4050 2    50   ~ 0
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
F 1 "4K7" V 10000 2550 50  0000 C CNN
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
F 1 "4K7" V 9900 2550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 10000 2550 50  0001 C CNN
F 3 "~" H 10000 2550 50  0001 C CNN
	1    10000 2550
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR08
U 1 1 601CBB27
P 9900 2250
F 0 "#PWR08" H 9900 2100 50  0001 C CNN
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
Notes:\n- GPIO13 reserved for noise sensor\n- GPIO5 reserved for LED + clear sw\n- GPIO4 reserved for IR sensor\n- GPIO1 & GPIO3 are serial0 (prog.)\n- +3V3 on EN pin is optional
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
	9700 3150 9400 3150
Text Label 9700 3150 2    50   ~ 0
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
Wire Wire Line
	9800 3650 9400 3650
$Comp
L Device:R_Small R7
U 1 1 601E3F8F
P 9900 3650
F 0 "R7" V 9900 3650 50  0000 C CNN
F 1 "470" V 9850 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 9900 3650 50  0001 C CNN
F 3 "~" H 9900 3650 50  0001 C CNN
	1    9900 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	10000 3650 10150 3650
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
P 1525 6450
F 0 "Q1" H 1730 6404 50  0000 L CNN
F 1 "2N7002" H 1730 6495 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 1725 6375 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 1525 6450 50  0001 L CNN
	1    1525 6450
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 601F9492
P 1425 6925
F 0 "#PWR02" H 1425 6675 50  0001 C CNN
F 1 "GND" H 1430 6752 50  0000 C CNN
F 2 "" H 1425 6925 50  0001 C CNN
F 3 "" H 1425 6925 50  0001 C CNN
	1    1425 6925
	1    0    0    -1  
$EndComp
Wire Wire Line
	1425 6925 1425 6800
$Comp
L Device:R_Small R1
U 1 1 601FB734
P 1425 6025
F 0 "R1" V 1425 6025 50  0000 C CNN
F 1 "68" V 1350 6025 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1425 6025 50  0001 C CNN
F 3 "~" H 1425 6025 50  0001 C CNN
	1    1425 6025
	-1   0    0    1   
$EndComp
Wire Wire Line
	1425 6125 1425 6250
$Comp
L Device:LED_Small_ALT D1
U 1 1 601FE5C8
P 1425 5725
F 0 "D1" V 1471 5655 50  0000 R CNN
F 1 "Red" H 1500 5850 50  0000 R CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1425 5725 50  0001 C CNN
F 3 "~" V 1425 5725 50  0001 C CNN
	1    1425 5725
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1425 5925 1425 5825
$Comp
L power:+5V #PWR01
U 1 1 602009CC
P 1425 5525
F 0 "#PWR01" H 1425 5375 50  0001 C CNN
F 1 "+5V" H 1440 5698 50  0000 C CNN
F 2 "" H 1425 5525 50  0001 C CNN
F 3 "" H 1425 5525 50  0001 C CNN
	1    1425 5525
	1    0    0    -1  
$EndComp
Wire Wire Line
	1425 5625 1425 5525
$Comp
L Device:R_Small R3
U 1 1 602030FF
P 2025 6450
F 0 "R3" V 2025 6450 50  0000 C CNN
F 1 "470" V 1950 6450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 2025 6450 50  0001 C CNN
F 3 "~" H 2025 6450 50  0001 C CNN
	1    2025 6450
	0    1    1    0   
$EndComp
Wire Wire Line
	1725 6450 1800 6450
$Comp
L Device:R_Small R2
U 1 1 60219586
P 1800 6625
F 0 "R2" V 1800 6625 50  0000 C CNN
F 1 "15k" V 1725 6625 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1800 6625 50  0001 C CNN
F 3 "~" H 1800 6625 50  0001 C CNN
	1    1800 6625
	-1   0    0    1   
$EndComp
Wire Wire Line
	1800 6725 1800 6800
Wire Wire Line
	1800 6800 1425 6800
Connection ~ 1425 6800
Wire Wire Line
	1425 6800 1425 6650
Wire Wire Line
	1800 6525 1800 6450
Connection ~ 1800 6450
Wire Wire Line
	1800 6450 1925 6450
Wire Wire Line
	2125 6450 2425 6450
Text Label 2425 6450 2    50   ~ 0
LED
$Comp
L neosensor_addon:G71Y SW1
U 1 1 60223186
P 2150 5775
F 0 "SW1" H 2150 6000 50  0000 C CNN
F 1 "clear" H 2150 5909 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 2150 5775 50  0001 C CIN
F 3 "" H 2150 5775 50  0001 C CNN
	1    2150 5775
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 60224902
P 1950 5950
F 0 "#PWR03" H 1950 5700 50  0001 C CNN
F 1 "GND" H 1955 5777 50  0000 C CNN
F 2 "" H 1950 5950 50  0001 C CNN
F 3 "" H 1950 5950 50  0001 C CNN
	1    1950 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 5875 1950 5950
Wire Wire Line
	1950 5775 1800 5775
Wire Wire Line
	1800 5775 1800 6450
$Comp
L power:+3V3 #PWR04
U 1 1 60229D6D
P 2425 5700
F 0 "#PWR04" H 2425 5550 50  0001 C CNN
F 1 "+3V3" H 2440 5873 50  0000 C CNN
F 2 "" H 2425 5700 50  0001 C CNN
F 3 "" H 2425 5700 50  0001 C CNN
	1    2425 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 5775 2425 5775
Wire Wire Line
	2425 5775 2425 5700
Text Notes 1275 4850 0    100  ~ 0
LED and Clear_SW
$EndSCHEMATC
