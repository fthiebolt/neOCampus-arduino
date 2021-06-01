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
Text Notes 9700 4300 0    50   ~ 0
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
	10400 4400 10400 4500
Text Label 10950 4400 0    50   ~ 0
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
$Comp
L Jumper:SolderJumper_3_Bridged12 JP1
U 1 1 5F5EEAA0
P 2450 5300
F 0 "JP1" V 2404 5368 50  0000 L CNN
F 1 "SolderJumper_3_Bridged12" V 2495 5368 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged2Bar12_Pad1.0x1.5mm" H 2450 5300 50  0001 C CNN
F 3 "~" H 2450 5300 50  0001 C CNN
	1    2450 5300
	0    1    1    0   
$EndComp
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
Text Label 9995 5575 0    50   ~ 0
SCL
Text Label 9995 5675 0    50   ~ 0
SDA
Text Label 9395 5675 2    50   ~ 0
GND
Text Label 9995 5375 0    50   ~ 0
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
Text Label 5650 5300 1    50   ~ 0
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
Wire Wire Line
	8600 2100 8600 2400
Text Label 6700 5200 1    50   ~ 0
GPIO0
Text Label 8600 2400 3    50   ~ 0
VDD
$Comp
L Regulator_Linear:AMS1117 U4
U 1 1 60C3FD4D
P 3685 5890
F 0 "U4" H 3685 6132 50  0000 C CNN
F 1 "AMS1117" H 3685 6041 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 3685 6090 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 3785 5640 50  0001 C CNN
	1    3685 5890
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 60C4535A
P 3130 6040
F 0 "C2" H 3245 6086 50  0000 L CNN
F 1 "100nF" H 3245 5995 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3168 5890 50  0001 C CNN
F 3 "~" H 3130 6040 50  0001 C CNN
	1    3130 6040
	1    0    0    -1  
$EndComp
Wire Wire Line
	3985 5890 4060 5890
$Comp
L Device:C C4
U 1 1 60C4A52B
P 4060 6040
F 0 "C4" H 4175 6086 50  0000 L CNN
F 1 "C" H 4175 5995 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4098 5890 50  0001 C CNN
F 3 "~" H 4060 6040 50  0001 C CNN
	1    4060 6040
	1    0    0    -1  
$EndComp
Connection ~ 4060 5890
Wire Wire Line
	4060 5890 4170 5890
Wire Wire Line
	3130 5890 3385 5890
Wire Wire Line
	3130 5890 2890 5890
Connection ~ 3130 5890
Wire Wire Line
	3130 6190 3685 6190
Connection ~ 3685 6190
Wire Wire Line
	3685 6190 4060 6190
Wire Wire Line
	3685 6190 3685 6255
Text Label 3685 6255 3    50   ~ 0
GND
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
P 9695 5475
F 0 "U5" H 9695 5842 50  0000 C CNN
F 1 "ATECC508A" H 9695 5751 50  0000 C CNN
F 2 "neosensor_addon:SOIC-8_3.9x5.4mm_P1.27mm_HandSolder" H 8895 5825 50  0001 L BIN
F 3 "" H 9695 5475 50  0001 L BNN
	1    9695 5475
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:G71Y SW1
U 1 1 60B86387
P 10000 4400
F 0 "SW1" H 10000 4625 50  0000 C CNN
F 1 "G71Y" H 10000 4534 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 10000 4400 50  0001 C CIN
F 3 "" H 10000 4400 50  0001 C CNN
	1    10000 4400
	-1   0    0    -1  
$EndComp
Text Label 2890 5890 2    50   ~ 0
VIN
Text Label 4170 5890 0    50   ~ 0
3v3
$Comp
L neosensor_addon:IRsensor IR?
U 1 1 60B77FCE
P 5900 5400
F 0 "IR?" H 5900 5725 50  0000 C CNN
F 1 "IRsensor" H 5900 5634 50  0000 C CNN
F 2 "neosensor_addon:IRsensor_TH_and_smd" H 5900 5400 50  0001 C CIN
F 3 "" H 5900 5400 50  0001 C CNN
	1    5900 5400
	1    0    0    -1  
$EndComp
Text Label 6600 5200 1    50   ~ 0
VIN
Wire Wire Line
	2450 5500 2450 5900
Wire Wire Line
	1900 5300 2300 5300
Wire Wire Line
	2450 4750 2450 5100
Text Label 1900 5300 2    50   ~ 0
Vsensor
Text Label 2450 4750 1    50   ~ 0
3v3
Text Label 2450 5900 3    50   ~ 0
VDD
$Comp
L Device:R R?
U 1 1 60BBF3F3
P 3600 3900
F 0 "R?" H 3670 3946 50  0000 L CNN
F 1 "R" H 3670 3855 50  0000 L CNN
F 2 "" V 3530 3900 50  0001 C CNN
F 3 "~" H 3600 3900 50  0001 C CNN
	1    3600 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60B63050
P 10550 4400
F 0 "R?" V 10343 4400 50  0000 C CNN
F 1 "15K" V 10434 4400 50  0000 C CNN
F 2 "" V 10480 4400 50  0001 C CNN
F 3 "~" H 10550 4400 50  0001 C CNN
	1    10550 4400
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 60B632E0
P 10400 4650
F 0 "R?" H 10470 4696 50  0000 L CNN
F 1 "15K" H 10470 4605 50  0000 L CNN
F 2 "" V 10330 4650 50  0001 C CNN
F 3 "~" H 10400 4650 50  0001 C CNN
	1    10400 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 60B6378A
P 10750 4650
F 0 "C?" H 10865 4696 50  0000 L CNN
F 1 "1µF" H 10865 4605 50  0000 L CNN
F 2 "" H 10788 4500 50  0001 C CNN
F 3 "~" H 10750 4650 50  0001 C CNN
	1    10750 4650
	1    0    0    -1  
$EndComp
Connection ~ 10400 4400
Wire Wire Line
	10700 4400 10750 4400
Wire Wire Line
	10750 4400 10750 4500
Wire Wire Line
	10750 4400 10950 4400
Connection ~ 10750 4400
Wire Wire Line
	10200 4800 10350 4800
Wire Wire Line
	10200 4500 10200 4800
Wire Wire Line
	10400 4800 10750 4800
Wire Wire Line
	10350 4800 10350 4850
Text Label 10350 4850 3    50   ~ 0
GND
Text Label 9600 4400 2    50   ~ 0
3v3
Wire Wire Line
	10350 4800 10400 4800
Connection ~ 10350 4800
Connection ~ 10400 4800
Wire Wire Line
	10200 4400 10400 4400
$EndSCHEMATC
