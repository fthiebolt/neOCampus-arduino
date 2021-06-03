EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4700 2000 4300 2000
Wire Wire Line
	4700 2100 4300 2100
Wire Wire Line
	4700 2200 4300 2200
Wire Wire Line
	4700 2300 4300 2300
Wire Wire Line
	4700 2400 4300 2400
Wire Wire Line
	4700 2600 4300 2600
Wire Wire Line
	4700 2700 4300 2700
Wire Wire Line
	4700 2800 4300 2800
Wire Wire Line
	4700 3000 4300 3000
Wire Wire Line
	4700 2500 4300 2500
Wire Wire Line
	5700 3000 6100 3000
Wire Wire Line
	5700 2700 6100 2700
Wire Wire Line
	5700 2600 6100 2600
Wire Wire Line
	5700 2400 6100 2400
Wire Wire Line
	5700 2300 6100 2300
Wire Wire Line
	5700 2200 6100 2200
Wire Wire Line
	5700 2000 6100 2000
Wire Wire Line
	5700 2100 6100 2100
Text Label 4300 2000 0    50   ~ 0
GND
Text Label 4300 2100 0    50   ~ 0
VS*
Text Label 4300 2200 0    50   ~ 0
VDD
Text Label 4300 2300 0    50   ~ 0
VEXT
Text Label 4300 2400 0    50   ~ 0
GND
Text Label 4300 2500 0    50   ~ 0
SCL
Text Label 4300 2600 0    50   ~ 0
SDA
Text Label 4300 2700 0    50   ~ 0
IR
Text Label 4300 2800 0    50   ~ 0
GPIO4
Text Label 3800 2900 0    50   ~ 0
GPIO5
Text Label 4300 3000 0    50   ~ 0
RST
Text Label 6100 3000 0    50   ~ 0
ADC
Text Label 6100 2600 0    50   ~ 0
RX
Text Label 6100 2500 0    50   ~ 0
TX
Text Label 6100 2400 0    50   ~ 0
SCK
Text Label 6100 2300 0    50   ~ 0
MISO
Text Label 6100 2200 0    50   ~ 0
MOSI
Text Label 6100 2100 0    50   ~ 0
VIN
Text Label 6100 2000 0    50   ~ 0
GND
$Comp
L Mechanical:MountingHole_Pad MH2
U 1 1 5F580B6E
P 8150 2950
F 0 "MH2" V 8387 2953 50  0000 C CNN
F 1 "MountingHole_Pad" V 8296 2953 50  0000 C CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 8150 2950 50  0001 C CNN
F 3 "~" H 8150 2950 50  0001 C CNN
	1    8150 2950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8250 2950 8650 2950
Text Label 8650 2950 0    50   ~ 0
GND
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 5F585CD1
P 9450 2150
F 0 "J2" H 9530 2192 50  0000 L CNN
F 1 "Conn_01x05" H 9530 2101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 9450 2150 50  0001 C CNN
F 3 "~" H 9450 2150 50  0001 C CNN
	1    9450 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 2150 9050 2150
Wire Wire Line
	9250 2250 9050 2250
Wire Wire Line
	9250 2350 9050 2350
Text Label 9050 2050 2    50   ~ 0
GND
Text Label 9050 2150 2    50   ~ 0
SCL
Text Label 9050 2250 2    50   ~ 0
SDA
Text Label 9050 2350 2    50   ~ 0
GPIO1
Text Notes 8700 1900 0    50   ~ 0
Expansion connectors for i2C sensors with interup\n\n
Text Label 9100 1000 2    50   ~ 0
Vsensor
Text Notes 9550 2900 0    50   ~ 0
Programming switch\n\n\n\n
$Comp
L Device:R R1
U 1 1 5F5B3249
P 4200 3500
F 0 "R1" H 4270 3546 50  0000 L CNN
F 1 "10K" H 4270 3455 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4130 3500 50  0001 C CNN
F 3 "~" H 4200 3500 50  0001 C CNN
	1    4200 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 3000 9450 3000
Wire Wire Line
	10250 3000 10250 3100
Text Label 10800 3000 0    50   ~ 0
GPIO5
Text Label 4200 3900 0    50   ~ 0
Vsensor
Wire Wire Line
	4200 3650 4200 3900
Wire Wire Line
	5700 2800 6100 2800
Wire Wire Line
	3800 2900 4200 2900
Wire Wire Line
	4200 2900 4200 3350
Connection ~ 4200 2900
Wire Wire Line
	4200 2900 4700 2900
Text Label 7900 2350 2    50   ~ 0
GPIO1
Text Label 7900 2250 2    50   ~ 0
SDA
Text Label 7900 2150 2    50   ~ 0
SCL
Text Label 7900 2050 2    50   ~ 0
GND
Text Label 7900 1950 2    50   ~ 0
Vsensor
Wire Wire Line
	8100 2350 7900 2350
Wire Wire Line
	8100 2250 7900 2250
Wire Wire Line
	8100 2150 7900 2150
$Comp
L Connector_Generic:Conn_01x05 J1
U 1 1 5F5836D7
P 8300 2150
F 0 "J1" H 8380 2192 50  0000 L CNN
F 1 "Conn_01x05" H 8380 2101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8300 2150 50  0001 C CNN
F 3 "~" H 8300 2150 50  0001 C CNN
	1    8300 2150
	1    0    0    -1  
$EndComp
Text Notes 7750 2650 0    50   ~ 0
Board mouting helo (with pads)\n
Text Label 6100 2700 0    50   ~ 0
GPIO3
Text Label 6100 2800 0    50   ~ 0
GPIO2
$Comp
L Mechanical:MountingHole_Pad MH1
U 1 1 5F5E57EB
P 8150 3350
F 0 "MH1" V 8387 3353 50  0000 C CNN
F 1 "MountingHole_Pad" V 8296 3353 50  0000 C CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 8150 3350 50  0001 C CNN
F 3 "~" H 8150 3350 50  0001 C CNN
	1    8150 3350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8250 3350 8650 3350
Text Label 8650 3350 0    50   ~ 0
GND
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 5F58D8D1
P 9500 1100
F 0 "J3" H 9580 1092 50  0000 L CNN
F 1 "Conn_01x04" H 9580 1001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9500 1100 50  0001 C CNN
F 3 "~" H 9500 1100 50  0001 C CNN
	1    9500 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 1200 9100 1200
Wire Wire Line
	9300 1300 9100 1300
Text Notes 8600 850  0    50   ~ 0
Expansion connectors for i2C sensors without interup
Text Label 9100 1300 2    50   ~ 0
SDA
Text Label 9100 1200 2    50   ~ 0
SCL
Text Label 9100 1100 2    50   ~ 0
GND
Text Label 10200 1000 2    50   ~ 0
Vsensor
$Comp
L Connector_Generic:Conn_01x04 J6
U 1 1 5F5FBA79
P 10600 1100
F 0 "J6" H 10680 1092 50  0000 L CNN
F 1 "Conn_01x04" H 10680 1001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 10600 1100 50  0001 C CNN
F 3 "~" H 10600 1100 50  0001 C CNN
	1    10600 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 1200 10200 1200
Wire Wire Line
	10400 1300 10200 1300
Text Label 10200 1300 2    50   ~ 0
SDA
Text Label 10200 1200 2    50   ~ 0
SCL
Text Label 10200 1100 2    50   ~ 0
GND
Text Label 10200 2350 2    50   ~ 0
GPIO1
Text Label 10200 2250 2    50   ~ 0
SDA
Text Label 10200 2150 2    50   ~ 0
SCL
Text Label 10200 2050 2    50   ~ 0
GND
Text Label 10200 1950 2    50   ~ 0
Vsensor
Wire Wire Line
	10400 2350 10200 2350
Wire Wire Line
	10400 2250 10200 2250
Wire Wire Line
	10400 2150 10200 2150
$Comp
L Connector_Generic:Conn_01x05 J5
U 1 1 5F605F1D
P 10600 2150
F 0 "J5" H 10680 2192 50  0000 L CNN
F 1 "Conn_01x05" H 10680 2101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 10600 2150 50  0001 C CNN
F 3 "~" H 10600 2150 50  0001 C CNN
	1    10600 2150
	1    0    0    -1  
$EndComp
Text Label 6400 2900 0    50   ~ 0
GPIO1
$Comp
L Device:R R2
U 1 1 5F5DD731
P 6300 3200
F 0 "R2" H 6370 3246 50  0000 L CNN
F 1 "10K" H 6370 3155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6230 3200 50  0001 C CNN
F 3 "~" H 6300 3200 50  0001 C CNN
	1    6300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2900 6300 2900
Wire Wire Line
	6300 2900 6300 3050
Connection ~ 6300 2900
Wire Wire Line
	6300 2900 6400 2900
Wire Wire Line
	6300 3350 6300 3550
Text Label 6300 3550 0    50   ~ 0
Vsensor
$Comp
L Graphic:Logo_Open_Hardware_Small #LOGO1
U 1 1 5F600CA5
P 10950 6350
F 0 "#LOGO1" H 10950 6625 50  0001 C CNN
F 1 "Logo_Open_Hardware_Small" H 10950 6125 50  0001 C CNN
F 2 "" H 10950 6350 50  0001 C CNN
F 3 "~" H 10950 6350 50  0001 C CNN
	1    10950 6350
	1    0    0    -1  
$EndComp
Text Label 1445 1225 0    50   ~ 0
SCL
Text Label 1445 1325 0    50   ~ 0
SDA
Text Label 845  1325 2    50   ~ 0
GND
Text Label 1445 1025 0    50   ~ 0
Vsensor
$Comp
L neosensor_addon:HC-SR01 U2
U 1 1 60B0C27A
P 6700 5500
F 0 "U2" H 7028 5604 50  0000 L CNN
F 1 "HC-SR01" H 7028 5513 50  0000 L CNN
F 2 "neosensor_addon:PIR_HC-SR01" H 6700 5500 50  0001 C CNN
F 3 "" H 6700 5500 50  0001 C CNN
	1    6700 5500
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:OLED1.3_I2C U3
U 1 1 60B0C862
P 8455 5800
F 0 "U3" H 8933 5804 50  0000 L CNN
F 1 "OLED1.3_I2C" H 8933 5713 50  0000 L CNN
F 2 "neosensor_addon:OLED1.3_I2C" H 8455 5800 50  0001 C CNN
F 3 "" H 8455 5800 50  0001 C CNN
	1    8455 5800
	0    -1   -1   0   
$EndComp
Text Label 6000 5700 3    50   ~ 0
Vsensor
Text Label 8205 5750 2    50   ~ 0
SCL
Text Label 8205 5650 2    50   ~ 0
SDA
Text Label 8205 5850 2    50   ~ 0
GND
Text Label 6800 5200 1    50   ~ 0
GND
Text Label 8205 5950 2    50   ~ 0
Vsensor
Text Label 6700 5200 1    50   ~ 0
IR
$Comp
L neosensor_addon:HTCC-AB01 U1
U 1 1 60B68D5B
P 4900 3100
F 0 "U1" H 5200 4465 50  0000 C CNN
F 1 "HTCC-AB01" H 5200 4374 50  0000 C CNN
F 2 "neosensor_addon:HTCC-AB01" V 5200 3700 50  0001 C CIN
F 3 "" H 4900 3100 50  0001 C CNN
	1    4900 3100
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:ATECC508A U5
U 1 1 60B75613
P 1145 1125
F 0 "U5" H 1145 1492 50  0000 C CNN
F 1 "ATECC508A" H 1145 1401 50  0000 C CNN
F 2 "neosensor_addon:SOIC-8_3.9x5.4mm_P1.27mm_HandSolder" H 345 1475 50  0001 L BIN
F 3 "" H 1145 1125 50  0001 L BNN
	1    1145 1125
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:G71Y SW1
U 1 1 60B86387
P 9850 3000
F 0 "SW1" H 9850 3225 50  0000 C CNN
F 1 "G71Y" H 9850 3134 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 9850 3000 50  0001 C CIN
F 3 "" H 9850 3000 50  0001 C CNN
	1    9850 3000
	-1   0    0    -1  
$EndComp
$Comp
L neosensor_addon:IRsensor IR1
U 1 1 60B77FCE
P 6100 5450
F 0 "IR1" H 6100 5775 50  0000 C CNN
F 1 "IRsensor" H 6100 5684 50  0000 C CNN
F 2 "neosensor_addon:IRsensor_TH_and_smd" H 6100 5450 50  0001 C CIN
F 3 "" H 6100 5450 50  0001 C CNN
	1    6100 5450
	0    -1   -1   0   
$EndComp
Text Label 6600 5200 1    50   ~ 0
VIN
$Comp
L Device:R R5
U 1 1 60B63050
P 10400 3000
F 0 "R5" V 10193 3000 50  0000 C CNN
F 1 "15K" V 10284 3000 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 10330 3000 50  0001 C CNN
F 3 "~" H 10400 3000 50  0001 C CNN
	1    10400 3000
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 60B632E0
P 10250 3250
F 0 "R4" H 10320 3296 50  0000 L CNN
F 1 "15K" H 10320 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 10180 3250 50  0001 C CNN
F 3 "~" H 10250 3250 50  0001 C CNN
	1    10250 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 60B6378A
P 10600 3250
F 0 "C1" H 10715 3296 50  0000 L CNN
F 1 "1µF" H 10715 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 10638 3100 50  0001 C CNN
F 3 "~" H 10600 3250 50  0001 C CNN
	1    10600 3250
	1    0    0    -1  
$EndComp
Connection ~ 10250 3000
Wire Wire Line
	10550 3000 10600 3000
Wire Wire Line
	10600 3000 10600 3100
Wire Wire Line
	10600 3000 10800 3000
Connection ~ 10600 3000
Wire Wire Line
	10050 3400 10200 3400
Wire Wire Line
	10050 3100 10050 3400
Wire Wire Line
	10250 3400 10600 3400
Wire Wire Line
	10200 3400 10200 3450
Text Label 10200 3450 3    50   ~ 0
GND
Text Label 9450 3000 2    50   ~ 0
Vsensor
Wire Wire Line
	10200 3400 10250 3400
Connection ~ 10200 3400
Connection ~ 10250 3400
Wire Wire Line
	10050 3000 10250 3000
Wire Wire Line
	9100 1100 9300 1100
Wire Wire Line
	9100 1000 9300 1000
Wire Wire Line
	10400 1000 10200 1000
Wire Wire Line
	10400 1100 10200 1100
Wire Wire Line
	8100 1950 7900 1950
Wire Wire Line
	8100 2050 7900 2050
Wire Wire Line
	9250 2050 9050 2050
Wire Wire Line
	9250 1950 9050 1950
Text Label 9050 1950 2    50   ~ 0
Vsensor
Wire Wire Line
	10400 2050 10200 2050
Wire Wire Line
	10400 1950 10200 1950
Text Label 2100 6400 0    50   ~ 0
3v3
Text Label 750  6400 2    50   ~ 0
VIN
Text Label 1600 6750 3    50   ~ 0
GND
$Comp
L Device:C C2
U 1 1 60C4535A
P 1000 6550
F 0 "C2" H 1115 6596 50  0000 L CNN
F 1 "100nF" H 1115 6505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1038 6400 50  0001 C CNN
F 3 "~" H 1000 6550 50  0001 C CNN
	1    1000 6550
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117 U4
U 1 1 60C3FD4D
P 1600 6400
F 0 "U4" H 1600 6642 50  0000 C CNN
F 1 "AMS1117" H 1600 6551 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 1600 6600 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 1700 6150 50  0001 C CNN
	1    1600 6400
	1    0    0    -1  
$EndComp
Text Label 3700 6400 0    50   ~ 0
VDD
Text Label 2550 6400 2    50   ~ 0
3v3
Text Label 3100 5850 1    50   ~ 0
Vsensor
Wire Wire Line
	2550 6400 2900 6400
Wire Wire Line
	3100 5850 3100 6250
Wire Wire Line
	3300 6400 3700 6400
$Comp
L Jumper:SolderJumper_3_Bridged12 JP1
U 1 1 5F5EEAA0
P 3100 6400
F 0 "JP1" V 3054 6468 50  0000 L CNN
F 1 "SolderJumper_3_Bridged12" V 3145 6468 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged2Bar12_Pad1.0x1.5mm" H 3100 6400 50  0001 C CNN
F 3 "~" H 3100 6400 50  0001 C CNN
	1    3100 6400
	1    0    0    1   
$EndComp
Text Label 6200 5700 3    50   ~ 0
GND
Text Label 6100 5200 1    50   ~ 0
IR
$Comp
L Device:CP1_Small C3
U 1 1 60B83D1D
P 2000 6600
F 0 "C3" H 2091 6646 50  0000 L CNN
F 1 "100µF" H 2091 6555 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 2000 6600 50  0001 C CNN
F 3 "~" H 2000 6600 50  0001 C CNN
	1    2000 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 6700 1600 6700
Connection ~ 1600 6700
Wire Wire Line
	1600 6700 1000 6700
Wire Wire Line
	750  6400 1000 6400
Connection ~ 1000 6400
Wire Wire Line
	1000 6400 1300 6400
Wire Wire Line
	1900 6400 2000 6400
Wire Wire Line
	2000 6400 2000 6500
Wire Wire Line
	2000 6400 2100 6400
Connection ~ 2000 6400
Wire Wire Line
	1600 6700 1600 6750
Wire Wire Line
	6100 2500 5700 2500
$EndSCHEMATC
