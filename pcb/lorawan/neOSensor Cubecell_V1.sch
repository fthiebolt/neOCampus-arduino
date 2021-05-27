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
$Comp
L neOSensor-Cubecell_V1-rescue:HTCC-AB01-htcc-ab01-neOSensor_Cubecell_V1-rescue U1
U 1 1 5F579B0C
P 4900 3100
F 0 "U1" H 5200 4465 50  0000 C CNN
F 1 "HTCC-AB01" H 5200 4374 50  0000 C CNN
F 2 "HTCC-AB01:htcc-ab01" H 4900 3100 50  0001 C CNN
F 3 "" H 4900 3100 50  0001 C CNN
	1    4900 3100
	1    0    0    -1  
$EndComp
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
	5700 2500 6100 2500
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
GPIO0
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
P 7800 4400
F 0 "MH2" V 8037 4403 50  0000 C CNN
F 1 "MountingHole_Pad" V 7946 4403 50  0000 C CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 7800 4400 50  0001 C CNN
F 3 "~" H 7800 4400 50  0001 C CNN
	1    7800 4400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7900 4400 8300 4400
Text Label 8300 4400 0    50   ~ 0
GND
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 5F585CD1
P 9300 2300
F 0 "J2" H 9380 2342 50  0000 L CNN
F 1 "Conn_01x05" H 9380 2251 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 9300 2300 50  0001 C CNN
F 3 "~" H 9300 2300 50  0001 C CNN
	1    9300 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 2300 8900 2300
Wire Wire Line
	9100 2400 8900 2400
Wire Wire Line
	9100 2500 8900 2500
Text Label 8800 2400 3    50   ~ 0
GND
Text Label 8900 2300 0    50   ~ 0
SCL
Text Label 8900 2400 0    50   ~ 0
SDA
Text Label 8900 2500 0    50   ~ 0
GPIO1
Text Notes 7300 1900 0    50   ~ 0
Expansion connectors for i2C sensors with interup\n\n
Text Label 6500 1200 3    50   ~ 0
Vsensor
$Comp
L neOSensor-Cubecell_V1-rescue:G71Y-neosensor_addon-neOSensor_Cubecell_V1-rescue SW1
U 1 1 5F5ADA5E
P 10000 4400
F 0 "SW1" H 10000 4625 50  0000 C CNN
F 1 "G71Y" H 10000 4534 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 10000 4400 50  0001 C CIN
F 3 "" H 10000 4400 50  0001 C CNN
	1    10000 4400
	1    0    0    -1  
$EndComp
Text Notes 9600 4300 0    50   ~ 0
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
	9800 4400 9600 4400
Wire Wire Line
	9800 4500 9600 4500
Wire Wire Line
	10200 4400 10300 4400
Wire Wire Line
	10300 4400 10300 4700
Wire Wire Line
	10300 4700 9800 4700
Wire Wire Line
	9800 4700 9800 4500
Connection ~ 9800 4500
Text Label 9600 4500 0    50   ~ 0
GND
Text Label 9600 4400 0    50   ~ 0
GPIO5
Text Label 4200 3900 0    50   ~ 0
VIN
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
$Comp
L Jumper:SolderJumper_3_Bridged12 JP1
U 1 1 5F5EEAA0
P 9900 1300
F 0 "JP1" V 9854 1368 50  0000 L CNN
F 1 "SolderJumper_3_Bridged12" V 9945 1368 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged2Bar12_Pad1.0x1.5mm" H 9900 1300 50  0001 C CNN
F 3 "~" H 9900 1300 50  0001 C CNN
	1    9900 1300
	0    1    1    0   
$EndComp
Wire Wire Line
	9900 1100 9900 1000
Wire Wire Line
	9400 1300 9750 1300
Text Label 9400 1300 0    50   ~ 0
Vsensor
Text Label 9900 1700 0    50   ~ 0
VEXT
Wire Wire Line
	9900 1500 9900 1700
Text Label 9900 1000 0    50   ~ 0
VDD
Wire Wire Line
	8600 2100 9100 2100
Wire Wire Line
	8800 2200 9100 2200
Wire Wire Line
	8800 2200 8800 2400
Wire Wire Line
	7200 2100 7800 2100
Connection ~ 7400 2200
Wire Wire Line
	7400 2200 7400 2400
Connection ~ 7200 2200
Wire Wire Line
	7200 2200 7200 2400
Wire Wire Line
	7200 2100 7200 2200
Wire Wire Line
	7400 2200 7800 2200
$Comp
L Device:C_Small C1
U 1 1 5F591F30
P 7300 2200
F 0 "C1" V 7529 2200 50  0000 C CNN
F 1 "100n" V 7438 2200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7300 2200 50  0001 C CNN
F 3 "~" H 7300 2200 50  0001 C CNN
	1    7300 2200
	0    -1   -1   0   
$EndComp
Text Label 7600 2500 0    50   ~ 0
GPIO1
Text Label 7600 2400 0    50   ~ 0
SDA
Text Label 7600 2300 0    50   ~ 0
SCL
Text Label 7400 2400 3    50   ~ 0
GND
Text Label 7200 2400 3    50   ~ 0
Vsensor
Wire Wire Line
	7800 2500 7600 2500
Wire Wire Line
	7800 2400 7600 2400
Wire Wire Line
	7800 2300 7600 2300
$Comp
L Connector_Generic:Conn_01x05 J1
U 1 1 5F5836D7
P 8000 2300
F 0 "J1" H 8080 2342 50  0000 L CNN
F 1 "Conn_01x05" H 8080 2251 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8000 2300 50  0001 C CNN
F 3 "~" H 8000 2300 50  0001 C CNN
	1    8000 2300
	1    0    0    -1  
$EndComp
Text Notes 7400 4100 0    50   ~ 0
Board mouting helo (with pads)\n
Text Label 6100 2700 0    50   ~ 0
GPIO3
Text Label 6100 2800 0    50   ~ 0
GPIO2
$Comp
L Mechanical:MountingHole_Pad MH1
U 1 1 5F5E57EB
P 7800 4800
F 0 "MH1" V 8037 4803 50  0000 C CNN
F 1 "MountingHole_Pad" V 7946 4803 50  0000 C CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 7800 4800 50  0001 C CNN
F 3 "~" H 7800 4800 50  0001 C CNN
	1    7800 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7900 4800 8300 4800
Text Label 8300 4800 0    50   ~ 0
GND
Connection ~ 6500 1000
Wire Wire Line
	6500 1000 6500 1200
Wire Wire Line
	6500 900  6500 1000
Connection ~ 6700 1000
Wire Wire Line
	6700 1000 6700 1200
Wire Wire Line
	6700 1000 7100 1000
Wire Wire Line
	6500 900  7100 900 
$Comp
L Device:C_Small C3
U 1 1 5F594E5A
P 6600 1000
F 0 "C3" V 6829 1000 50  0000 C CNN
F 1 "100n" V 6738 1000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6600 1000 50  0001 C CNN
F 3 "~" H 6600 1000 50  0001 C CNN
	1    6600 1000
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 5F58D8D1
P 7300 1000
F 0 "J3" H 7380 992 50  0000 L CNN
F 1 "Conn_01x04" H 7380 901 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 7300 1000 50  0001 C CNN
F 3 "~" H 7300 1000 50  0001 C CNN
	1    7300 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 1100 6900 1100
Wire Wire Line
	7100 1200 6900 1200
Text Notes 6700 800  0    50   ~ 0
Expansion connectors for i2C \nsensors without interup\n\n
Text Label 6900 1200 0    50   ~ 0
SDA
Text Label 6900 1100 0    50   ~ 0
SCL
Text Label 6700 1200 3    50   ~ 0
GND
Text Label 8000 1200 3    50   ~ 0
Vsensor
Connection ~ 8000 1000
Wire Wire Line
	8000 1000 8000 1200
Wire Wire Line
	8000 900  8000 1000
Connection ~ 8200 1000
Wire Wire Line
	8200 1000 8200 1200
Wire Wire Line
	8200 1000 8600 1000
Wire Wire Line
	8000 900  8600 900 
$Comp
L Device:C_Small C6
U 1 1 5F5FBA73
P 8100 1000
F 0 "C6" V 8329 1000 50  0000 C CNN
F 1 "100n" V 8238 1000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8100 1000 50  0001 C CNN
F 3 "~" H 8100 1000 50  0001 C CNN
	1    8100 1000
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J6
U 1 1 5F5FBA79
P 8800 1000
F 0 "J6" H 8880 992 50  0000 L CNN
F 1 "Conn_01x04" H 8880 901 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8800 1000 50  0001 C CNN
F 3 "~" H 8800 1000 50  0001 C CNN
	1    8800 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 1100 8400 1100
Wire Wire Line
	8600 1200 8400 1200
Text Label 8400 1200 0    50   ~ 0
SDA
Text Label 8400 1100 0    50   ~ 0
SCL
Text Label 8200 1200 3    50   ~ 0
GND
Wire Wire Line
	7200 2900 7800 2900
Connection ~ 7400 3000
Wire Wire Line
	7400 3000 7400 3200
Connection ~ 7200 3000
Wire Wire Line
	7200 3000 7200 3200
Wire Wire Line
	7200 2900 7200 3000
Wire Wire Line
	7400 3000 7800 3000
$Comp
L Device:C_Small C5
U 1 1 5F605F0F
P 7300 3000
F 0 "C5" V 7529 3000 50  0000 C CNN
F 1 "100n" V 7438 3000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7300 3000 50  0001 C CNN
F 3 "~" H 7300 3000 50  0001 C CNN
	1    7300 3000
	0    -1   -1   0   
$EndComp
Text Label 7600 3300 0    50   ~ 0
GPIO1
Text Label 7600 3200 0    50   ~ 0
SDA
Text Label 7600 3100 0    50   ~ 0
SCL
Text Label 7400 3200 3    50   ~ 0
GND
Text Label 7200 3200 3    50   ~ 0
Vsensor
Wire Wire Line
	7800 3300 7600 3300
Wire Wire Line
	7800 3200 7600 3200
Wire Wire Line
	7800 3100 7600 3100
$Comp
L Connector_Generic:Conn_01x05 J5
U 1 1 5F605F1D
P 8000 3100
F 0 "J5" H 8080 3142 50  0000 L CNN
F 1 "Conn_01x05" H 8080 3051 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8000 3100 50  0001 C CNN
F 3 "~" H 8000 3100 50  0001 C CNN
	1    8000 3100
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
VIN
$Comp
L Graphic:Logo_Open_Hardware_Small #LOGO1
U 1 1 5F600CA5
P 5850 4200
F 0 "#LOGO1" H 5850 4475 50  0001 C CNN
F 1 "Logo_Open_Hardware_Small" H 5850 3975 50  0001 C CNN
F 2 "" H 5850 4200 50  0001 C CNN
F 3 "~" H 5850 4200 50  0001 C CNN
	1    5850 4200
	1    0    0    -1  
$EndComp
Text Notes 9345 5915 0    50   ~ 0
ATECC508A-SSHDA
Text Label 10085 5570 0    50   ~ 0
SCL
Text Label 10085 5670 0    50   ~ 0
SDA
Text Label 9285 5670 2    50   ~ 0
GND
Text Label 10085 5370 0    50   ~ 0
Vsensor
NoConn ~ 10085 5470
NoConn ~ 9285 5370
NoConn ~ 9285 5470
NoConn ~ 9285 5570
$Comp
L neosensor_addon:HC-SR01 U2
U 1 1 60B0C27A
P 6840 5635
F 0 "U2" H 7168 5739 50  0000 L CNN
F 1 "HC-SR01" H 7168 5648 50  0000 L CNN
F 2 "neosensor_addon:PIR_HC-SR01" H 6840 5635 50  0001 C CNN
F 3 "" H 6840 5635 50  0001 C CNN
	1    6840 5635
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
Text Label 6740 5335 1    50   ~ 0
Vsensor
Text Label 8205 5750 2    50   ~ 0
SCL
Text Label 8205 5650 2    50   ~ 0
SDA
Text Label 8205 5850 2    50   ~ 0
GND
Text Label 6940 5335 1    50   ~ 0
GND
Text Label 8205 5950 2    50   ~ 0
Vsensor
$Comp
L neosensor_addon:ATECC508A J7
U 1 1 60B67E25
P 9685 5470
F 0 "J7" H 9685 5837 50  0000 C CNN
F 1 "ATECC508A" H 9685 5746 50  0000 C CNN
F 2 "neosensor_addon:ATECC508A" H 9685 5470 50  0001 L BNN
F 3 "" H 9685 5470 50  0001 L BNN
F 4 "Harting" H 9685 5470 50  0001 L BNN "MANUFACTURER"
F 5 "2021-04-27" H 9685 5470 50  0001 L BNN "PARTREV"
F 6 "8 mm" H 9685 5470 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 7 "Manufacturer Recommendations" H 9685 5470 50  0001 L BNN "STANDARD"
	1    9685 5470
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 2100 8600 2400
Text Label 6840 5335 1    50   ~ 0
GPIO0
Text Label 8600 2400 3    50   ~ 0
VDD
$Comp
L Regulator_Linear:AMS1117 U?
U 1 1 60C3FD4D
P 5035 5490
F 0 "U?" H 5035 5732 50  0000 C CNN
F 1 "AMS1117" H 5035 5641 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 5035 5690 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 5135 5240 50  0001 C CNN
	1    5035 5490
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 60C4535A
P 4480 5640
F 0 "C?" H 4595 5686 50  0000 L CNN
F 1 "100nF" H 4595 5595 50  0000 L CNN
F 2 "" H 4518 5490 50  0001 C CNN
F 3 "~" H 4480 5640 50  0001 C CNN
	1    4480 5640
	1    0    0    -1  
$EndComp
Wire Wire Line
	5335 5490 5410 5490
$Comp
L Device:C C?
U 1 1 60C4A52B
P 5410 5640
F 0 "C?" H 5525 5686 50  0000 L CNN
F 1 "C" H 5525 5595 50  0000 L CNN
F 2 "" H 5448 5490 50  0001 C CNN
F 3 "~" H 5410 5640 50  0001 C CNN
	1    5410 5640
	1    0    0    -1  
$EndComp
Connection ~ 5410 5490
Wire Wire Line
	5410 5490 5520 5490
Wire Wire Line
	4480 5490 4735 5490
Wire Wire Line
	4480 5490 4240 5490
Connection ~ 4480 5490
Wire Wire Line
	4480 5790 5035 5790
Connection ~ 5035 5790
Wire Wire Line
	5035 5790 5410 5790
Wire Wire Line
	5035 5790 5035 5855
Text Label 5035 5855 3    50   ~ 0
GND
$EndSCHEMATC