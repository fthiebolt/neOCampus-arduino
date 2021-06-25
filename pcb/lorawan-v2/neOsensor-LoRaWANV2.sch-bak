EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "neOSensor-LoRaWAN V2"
Date "2021-06-08"
Rev "2"
Comp "(c) neOCampus / Dr THIEBOLT François"
Comment1 "Univ.Tlse3 / IRIT / neOCampus"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 2050 2600 2    100  ~ 0
User push button
$Comp
L neosensor_addon:ATECC508A U2
U 1 1 60B75613
P 1400 1400
F 0 "U2" H 1400 1767 50  0000 C CNN
F 1 "ATECC508A" H 1400 1676 50  0000 C CNN
F 2 "neosensor_addon:SOIC-8_3.9x5.4mm_P1.27mm_HandSolder" H 600 1750 50  0001 L BIN
F 3 "" H 1400 1400 50  0001 L BNN
	1    1400 1400
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:IRsensor IR1
U 1 1 60B77FCE
P 2975 5825
F 0 "IR1" H 2975 6150 50  0000 C CNN
F 1 "IRsensor" H 2975 6059 50  0000 C CNN
F 2 "neosensor_addon:IRsensor_TH_and_smd" H 2975 5825 50  0001 C CIN
F 3 "" H 2975 5825 50  0001 C CNN
	1    2975 5825
	1    0    0    -1  
$EndComp
Text Label 10375 5775 2    50   ~ 0
Vsensor
Text Notes 875  850  0    100  ~ 0
I2C crypto flash
Wire Wire Line
	850  1600 1075 1600
Text Label 850  1600 0    50   ~ 0
GND
Wire Wire Line
	1925 1600 1700 1600
Text Label 1925 1600 2    50   ~ 0
SDA
Wire Wire Line
	1925 1500 1700 1500
Text Label 1925 1500 2    50   ~ 0
SCL
Wire Wire Line
	1925 1300 1750 1300
Text Label 1925 1300 0    50   ~ 0
Vsensor
Wire Notes Line
	6850 6550 4700 6550
Wire Notes Line
	4700 6550 4700 7750
$Comp
L neosensor_addon:OLED1.3_I2C U1
U 1 1 601B3200
P 1250 5575
F 0 "U1" H 1728 5579 50  0000 L CNN
F 1 "OLED1.3_I2C" H 1728 5488 50  0000 L CNN
F 2 "neosensor_addon:OLED1.3_I2C" H 1250 5575 50  0001 C CNN
F 3 "https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf" H 1250 5575 50  0001 C CNN
	1    1250 5575
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U3
U 1 1 601B4C77
P 7425 5450
F 0 "U3" H 7425 5692 50  0000 C CNN
F 1 "AMS1117-3.3" H 7425 5601 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 7425 5650 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 7525 5200 50  0001 C CNN
	1    7425 5450
	1    0    0    -1  
$EndComp
Text Notes 5800 775  0    100  ~ 0
I2C sensors connectors
Text Notes 8800 775  0    100  ~ 0
Serial sensors connector
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 601C095B
P 5500 7500
F 0 "H2" V 5454 7650 50  0000 L CNN
F 1 "3.2mm for MH3 screw" V 5545 7650 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 5500 7500 50  0001 C CNN
F 3 "~" H 5500 7500 50  0001 C CNN
	1    5500 7500
	0    1    1    0   
$EndComp
Text Notes 4900 6850 0    100  ~ 0
Board mounting holes
Wire Wire Line
	7425 6000 7425 5900
Text Label 7425 5950 1    50   ~ 0
GND
Wire Wire Line
	5050 7200 5400 7200
Wire Wire Line
	5050 7500 5400 7500
Text Label 5050 7200 0    50   ~ 0
GND
Text Label 5050 7500 0    50   ~ 0
GND
Wire Wire Line
	6675 5450 6975 5450
Text Label 6675 5450 0    50   ~ 0
VIN
Wire Wire Line
	7725 5450 7825 5450
Text Label 8175 5450 2    50   ~ 0
+3V3
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 601C66E2
P 4650 1125
F 0 "J1" H 4600 1325 50  0000 L CNN
F 1 "Conn_01x04" H 4450 825 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 4650 1125 50  0001 C CNN
F 3 "~" H 4650 1125 50  0001 C CNN
	1    4650 1125
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J5
U 1 1 601CBCDB
P 8300 1225
F 0 "J5" H 8250 1525 50  0000 L CNN
F 1 "Conn_01x05" H 8100 925 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8300 1225 50  0001 C CNN
F 3 "~" H 8300 1225 50  0001 C CNN
	1    8300 1225
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1_Small C3
U 1 1 601D0AC1
P 7825 5700
F 0 "C3" H 7916 5746 50  0000 L CNN
F 1 "100µF 6.3v" H 7916 5655 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 7825 5700 50  0001 C CNN
F 3 "~" H 7825 5700 50  0001 C CNN
	1    7825 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7825 5600 7825 5450
Connection ~ 7825 5450
Wire Wire Line
	7825 5450 7875 5450
Wire Wire Line
	7825 5800 7825 5900
Wire Wire Line
	7825 5900 7425 5900
Connection ~ 7425 5900
Wire Wire Line
	7425 5900 7425 5750
Wire Wire Line
	4450 1025 4150 1025
Text Label 4150 1025 0    50   ~ 0
Vsensor
Wire Wire Line
	4150 1125 4450 1125
Text Label 4150 1125 0    50   ~ 0
GND
Wire Wire Line
	4150 1225 4450 1225
Text Label 4150 1225 0    50   ~ 0
SCL
Wire Wire Line
	4150 1325 4450 1325
Text Label 4150 1325 0    50   ~ 0
SDA
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 601D570B
P 5500 1125
F 0 "J3" H 5450 1325 50  0000 L CNN
F 1 "Conn_01x04" H 5300 825 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5500 1125 50  0001 C CNN
F 3 "~" H 5500 1125 50  0001 C CNN
	1    5500 1125
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 1025 5000 1025
Text Label 5000 1025 0    50   ~ 0
Vsensor
Wire Wire Line
	5000 1125 5300 1125
Text Label 5000 1125 0    50   ~ 0
GND
Wire Wire Line
	5000 1225 5300 1225
Text Label 5000 1225 0    50   ~ 0
SCL
Wire Wire Line
	5000 1325 5300 1325
Text Label 5000 1325 0    50   ~ 0
SDA
Wire Wire Line
	6175 1025 5875 1025
Text Label 5875 1025 0    50   ~ 0
Vsensor
Wire Wire Line
	5875 1125 6175 1125
Text Label 5875 1125 0    50   ~ 0
GND
Wire Wire Line
	5875 1225 6175 1225
Text Label 5875 1225 0    50   ~ 0
SCL
Wire Wire Line
	5875 1325 6175 1325
Text Label 5875 1325 0    50   ~ 0
SDA
Wire Wire Line
	8100 1025 7800 1025
Text Label 7800 1025 0    50   ~ 0
Vsensor
Wire Wire Line
	7800 1125 8100 1125
Text Label 7800 1125 0    50   ~ 0
GND
Wire Wire Line
	7800 1225 8100 1225
Text Label 7800 1225 0    50   ~ 0
SCL
Wire Wire Line
	7800 1325 8100 1325
Text Label 7800 1325 0    50   ~ 0
SDA
Wire Wire Line
	7800 1425 8100 1425
Text Label 7800 1425 0    50   ~ 0
INT
Wire Wire Line
	1200 5325 1200 5025
Wire Wire Line
	1300 5325 1300 5025
Wire Wire Line
	1400 5325 1400 5025
Text Label 1100 5025 3    50   ~ 0
Vsensor
Text Label 1200 5025 3    50   ~ 0
GND
Text Label 1300 5025 3    50   ~ 0
SCL
Text Label 1400 5025 3    50   ~ 0
SDA
$Comp
L power:+3V3 #PWR04
U 1 1 601C80B4
P 7875 5250
F 0 "#PWR04" H 7875 5100 50  0001 C CNN
F 1 "+3V3" H 7890 5423 50  0000 C CNN
F 2 "" H 7875 5250 50  0001 C CNN
F 3 "" H 7875 5250 50  0001 C CNN
	1    7875 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7875 5250 7875 5450
Connection ~ 7875 5450
Wire Wire Line
	7875 5450 8175 5450
$Comp
L Device:R_Small R3
U 1 1 601C96F3
P 6900 2650
F 0 "R3" V 6900 2650 50  0000 C CNN
F 1 "4K7" V 6925 2500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6900 2650 50  0001 C CNN
F 3 "~" H 6900 2650 50  0001 C CNN
	1    6900 2650
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R4
U 1 1 601C9D65
P 7000 2650
F 0 "R4" V 7000 2650 50  0000 C CNN
F 1 "4K7" V 7025 2500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7000 2650 50  0001 C CNN
F 3 "~" H 7000 2650 50  0001 C CNN
	1    7000 2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	6900 2350 7000 2350
Wire Wire Line
	7000 2350 7000 2550
Wire Wire Line
	6900 2350 6900 2550
Wire Wire Line
	7000 2750 7000 3000
Text Notes 1000 4825 0    100  ~ 0
Display
Text Notes 6250 4800 0    100  ~ 0
Power supply
Text Notes 4250 2600 0    50   ~ 0
Notes:\n- ADC must be left floating to enable VBat reading\n  (onboard BR1 not removed)\n- Panasonic IR sensor delivers upto 100µA, hence 47K
$Comp
L Device:C_Small C2
U 1 1 60206A1D
P 6975 5700
F 0 "C2" H 7067 5746 50  0000 L CNN
F 1 "100nF" H 7067 5655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6975 5700 50  0001 C CNN
F 3 "~" H 6975 5700 50  0001 C CNN
	1    6975 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6975 5600 6975 5450
Connection ~ 6975 5450
Wire Wire Line
	6975 5450 7125 5450
Wire Wire Line
	6975 5800 6975 5900
Wire Wire Line
	6975 5900 7425 5900
Wire Wire Line
	10250 1275 10550 1275
Wire Wire Line
	10250 1375 10550 1375
Wire Wire Line
	10250 1475 10550 1475
Wire Wire Line
	10250 1575 10550 1575
Wire Wire Line
	10250 1675 10550 1675
Text Label 10250 1275 0    50   ~ 0
VIN
Text Label 10250 1375 0    50   ~ 0
GND
Text Label 10250 1575 0    50   ~ 0
TX
Text Label 10250 1675 0    50   ~ 0
RX
$Comp
L Connector_Generic:Conn_01x05 J6
U 1 1 60238DBA
P 10750 1475
F 0 "J6" H 10700 1775 50  0000 L CNN
F 1 "Conn_01x05" H 10550 1175 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 10750 1475 50  0001 C CNN
F 3 "~" H 10750 1475 50  0001 C CNN
	1    10750 1475
	1    0    0    -1  
$EndComp
Text Notes 8800 1075 0    50   ~ 0
Notes about Particule Meters sensors\n- PM_EN is optionnally intended to enable/disable the\nparticule meter (not to have it always on)
Text Label 10250 1475 0    50   ~ 0
PM_EN
$Comp
L neosensor_addon:HC-SR01 IR2
U 1 1 601D6A06
P 4200 5850
F 0 "IR2" H 4528 5954 50  0000 L CNN
F 1 "HC-SR01" H 4528 5863 50  0000 L CNN
F 2 "neosensor_addon:PIR_HC-SR01" H 4200 5850 50  0001 C CNN
F 3 "" H 4200 5850 50  0001 C CNN
	1    4200 5850
	1    0    0    -1  
$EndComp
Text Notes 3000 4775 0    100  ~ 0
PIR sensors
Wire Wire Line
	4100 5250 4100 5550
Wire Wire Line
	4200 5250 4200 5550
Wire Wire Line
	4300 5250 4300 5550
Text Label 4100 5250 3    50   ~ 0
VIN
Text Label 4200 5250 3    50   ~ 0
IR
Text Label 4300 5250 3    50   ~ 0
GND
Text Notes 3600 5150 0    50   ~ 0
HC-SR01 V+ accepts from 5 to 20v\nOUT pin is 3.3 TTL
$Comp
L neosensor_addon:G71Y SW1
U 1 1 60238ADD
P 1225 3025
F 0 "SW1" H 1225 3250 50  0000 C CNN
F 1 "G71Y" H 1225 3159 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 1225 3025 50  0001 C CIN
F 3 "" H 1225 3025 50  0001 C CNN
	1    1225 3025
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 60238D6F
P 1300 3475
F 0 "#PWR01" H 1300 3225 50  0001 C CNN
F 1 "GND" H 1305 3302 50  0000 C CNN
F 2 "" H 1300 3475 50  0001 C CNN
F 3 "" H 1300 3475 50  0001 C CNN
	1    1300 3475
	1    0    0    -1  
$EndComp
Wire Wire Line
	650  3025 1025 3025
Wire Wire Line
	1050 3425 1300 3425
Wire Wire Line
	1300 3425 1300 3475
Wire Wire Line
	1300 3425 1425 3425
Wire Wire Line
	1600 3425 1600 3325
Connection ~ 1300 3425
Wire Wire Line
	1425 3025 1600 3025
Wire Wire Line
	1600 3025 1600 3125
Wire Wire Line
	1700 3025 1600 3025
Connection ~ 1600 3025
Wire Wire Line
	1900 3025 1975 3025
Text Label 2200 3025 2    50   ~ 0
SW1
$Comp
L Device:R_Small R1
U 1 1 60238D89
P 1600 3225
F 0 "R1" V 1600 3225 50  0000 C CNN
F 1 "15k" V 1525 3225 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1600 3225 50  0001 C CNN
F 3 "~" H 1600 3225 50  0001 C CNN
	1    1600 3225
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R2
U 1 1 60217A45
P 1800 3025
F 0 "R2" V 1800 3025 50  0000 C CNN
F 1 "15k" V 1725 3025 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1800 3025 50  0001 C CNN
F 3 "~" H 1800 3025 50  0001 C CNN
	1    1800 3025
	0    -1   -1   0   
$EndComp
$Comp
L Device:C_Small C1
U 1 1 6022DD92
P 1975 3225
F 0 "C1" H 2067 3271 50  0000 L CNN
F 1 "1uF" H 2067 3180 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1975 3225 50  0001 C CNN
F 3 "~" H 1975 3225 50  0001 C CNN
	1    1975 3225
	1    0    0    -1  
$EndComp
Wire Wire Line
	1975 3125 1975 3025
Connection ~ 1975 3025
Wire Wire Line
	1975 3025 2200 3025
Wire Wire Line
	1975 3425 1600 3425
Wire Wire Line
	1975 3325 1975 3425
Connection ~ 1600 3425
Wire Wire Line
	1100 5325 1100 5025
Text Label 1050 3425 0    50   ~ 0
GND
$Comp
L power:GND #PWR03
U 1 1 60232CA5
P 7425 6000
F 0 "#PWR03" H 7425 5750 50  0001 C CNN
F 1 "GND" H 7430 5827 50  0000 C CNN
F 2 "" H 7425 6000 50  0001 C CNN
F 3 "" H 7425 6000 50  0001 C CNN
	1    7425 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1425 3125 1425 3425
Connection ~ 1425 3425
Wire Wire Line
	1425 3425 1600 3425
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 601BFC00
P 5500 7200
F 0 "H1" V 5454 7350 50  0000 L CNN
F 1 "3.2mm for MH3 screw" V 5545 7350 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 5500 7200 50  0001 C CNN
F 3 "~" H 5500 7200 50  0001 C CNN
	1    5500 7200
	0    1    1    0   
$EndComp
Wire Wire Line
	3500 5825 3225 5825
Text Label 3500 5825 2    50   ~ 0
IR
$Comp
L power:GND #PWR02
U 1 1 60E8BA52
P 2650 6075
F 0 "#PWR02" H 2650 5825 50  0001 C CNN
F 1 "GND" H 2655 5902 50  0000 C CNN
F 2 "" H 2650 6075 50  0001 C CNN
F 3 "" H 2650 6075 50  0001 C CNN
	1    2650 6075
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 6075 2650 5925
Wire Wire Line
	2650 5925 2725 5925
Wire Wire Line
	2425 5725 2725 5725
Text Label 2425 5725 0    50   ~ 0
Vsensor
Text Notes 2750 4900 0    50   ~ 0
either IR1 or IR2 (both is not allowed)
Text Notes 2425 5425 0    50   ~ 0
IR1 output is 100µA max\n+ R pull down needed (47K)
Text Notes 8700 4875 0    100  ~ 0
Sensors power supply selector
Wire Wire Line
	9525 6125 9825 6125
Wire Wire Line
	9825 6125 9825 5975
Text Label 9525 6125 0    50   ~ 0
VDD
Wire Wire Line
	9525 5425 9825 5425
Wire Wire Line
	9825 5425 9825 5575
Text Label 9525 5425 0    50   ~ 0
+3V3
Text Label 650  3025 0    50   ~ 0
Vsensor
$Comp
L Jumper:SolderJumper_3_Open JP1
U 1 1 60C167A1
P 9825 5775
F 0 "JP1" V 9779 5842 50  0000 L CNN
F 1 "SolderJumper_3_Open" V 9870 5842 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Open_RoundedPad1.0x1.5mm" H 9825 5775 50  0001 C CNN
F 3 "~" H 9825 5775 50  0001 C CNN
	1    9825 5775
	0    -1   1    0   
$EndComp
Text Notes 8825 5125 0    50   ~ 0
USB powered: junction 1-2\nSolar powered: junction 2-3 (AMS1117 not needed)
$Comp
L neosensor_addon:HTCC-AB01 U4
U 1 1 60CE8BBB
P 8200 2900
F 0 "U4" H 8200 3665 50  0000 C CNN
F 1 "HTCC-AB01" H 8200 3574 50  0000 C CNN
F 2 "neosensor_addon:HTCC-AB01" V 8200 2900 50  0001 C CIN
F 3 "" H 7900 2300 50  0001 C CNN
	1    8200 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 2400 7700 2400
Text Label 7400 2400 0    50   ~ 0
GND
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 60D14B89
P 5950 5550
F 0 "J2" H 6030 5542 50  0000 L CNN
F 1 "Conn_01x02" H 6030 5451 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5950 5550 50  0001 C CNN
F 3 "~" H 5950 5550 50  0001 C CNN
	1    5950 5550
	1    0    0    -1  
$EndComp
Wire Notes Line
	8550 6450 8550 4700
Wire Notes Line
	8550 4700 11150 4700
Text Notes 6900 5000 0    50   ~ 0
USB power
Text Notes 5750 5000 0    50   ~ 0
Solar panel
Wire Wire Line
	5450 5650 5750 5650
Text Label 5450 5650 0    50   ~ 0
GND
Wire Wire Line
	5450 5550 5750 5550
Text Label 5450 5550 0    50   ~ 0
VS+
Wire Wire Line
	7400 2500 7700 2500
Text Label 7400 2500 0    50   ~ 0
VS+
Wire Wire Line
	7400 2600 7700 2600
Text Label 7400 2600 0    50   ~ 0
VDD
NoConn ~ 7700 2700
Wire Wire Line
	7400 2800 7700 2800
Text Label 7400 2800 0    50   ~ 0
GND
NoConn ~ 7700 3400
NoConn ~ 8700 3400
Wire Wire Line
	9000 2500 8700 2500
Text Label 9000 2500 2    50   ~ 0
VIN
Wire Wire Line
	9000 2400 8700 2400
Text Label 9000 2400 2    50   ~ 0
GND
NoConn ~ 8700 2600
NoConn ~ 8700 2700
NoConn ~ 8700 2800
Wire Wire Line
	7700 2900 6900 2900
Wire Wire Line
	6900 2750 6900 2900
Wire Wire Line
	7700 3000 7000 3000
$Comp
L Device:R_Small R6
U 1 1 603D554C
P 9375 3075
F 0 "R6" V 9375 3075 50  0000 C CNN
F 1 "15k" V 9400 2925 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 9375 3075 50  0001 C CNN
F 3 "~" H 9375 3075 50  0001 C CNN
	1    9375 3075
	-1   0    0    1   
$EndComp
Text Label 6600 2900 0    50   ~ 0
SCL
Text Label 6700 3000 0    50   ~ 0
SDA
Wire Wire Line
	8700 2900 8750 2900
Text Label 9225 3000 2    50   ~ 0
RX
Wire Wire Line
	9225 3000 8700 3000
Text Label 9225 2900 2    50   ~ 0
TX
Wire Wire Line
	9225 2900 8950 2900
$Comp
L Device:R_Small R5
U 1 1 60E22AB0
P 8850 2900
F 0 "R5" V 8850 2900 50  0000 C CNN
F 1 "470" V 8800 3075 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8850 2900 50  0001 C CNN
F 3 "~" H 8850 2900 50  0001 C CNN
	1    8850 2900
	0    1    1    0   
$EndComp
NoConn ~ 8700 3100
Text Label 6900 2350 0    50   ~ 0
Vsensor
Wire Wire Line
	9375 2975 9375 2750
Wire Wire Line
	9375 2750 9675 2750
Text Label 9675 2750 2    50   ~ 0
Vsensor
Wire Wire Line
	9375 3300 9375 3175
Text Label 9675 3300 2    50   ~ 0
INT
NoConn ~ 7700 3100
Wire Wire Line
	7400 3300 7700 3300
Text Label 7400 3300 0    50   ~ 0
SW1
Text Label 9000 3200 2    50   ~ 0
PM_EN
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 60CE8FB1
P 4250 6900
F 0 "#FLG0101" H 4250 6975 50  0001 C CNN
F 1 "PWR_FLAG" H 4250 7073 50  0000 C CNN
F 2 "" H 4250 6900 50  0001 C CNN
F 3 "~" H 4250 6900 50  0001 C CNN
	1    4250 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9975 5775 10375 5775
Wire Wire Line
	4250 7325 4250 6900
Text Label 4250 7325 1    50   ~ 0
Vsensor
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 60D0A8BB
P 4000 7025
F 0 "#FLG0102" H 4000 7100 50  0001 C CNN
F 1 "PWR_FLAG" H 4000 7198 50  0000 C CNN
F 2 "" H 4000 7025 50  0001 C CNN
F 3 "~" H 4000 7025 50  0001 C CNN
	1    4000 7025
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 7325 4000 7025
Text Label 4000 7325 1    50   ~ 0
GND
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 60D11109
P 3725 6900
F 0 "#FLG0103" H 3725 6975 50  0001 C CNN
F 1 "PWR_FLAG" H 3725 7073 50  0000 C CNN
F 2 "" H 3725 6900 50  0001 C CNN
F 3 "~" H 3725 6900 50  0001 C CNN
	1    3725 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3725 7325 3725 6900
Text Label 3725 7325 1    50   ~ 0
VDD
$Comp
L power:PWR_FLAG #FLG0104
U 1 1 60D19036
P 3450 7025
F 0 "#FLG0104" H 3450 7100 50  0001 C CNN
F 1 "PWR_FLAG" H 3450 7198 50  0000 C CNN
F 2 "" H 3450 7025 50  0001 C CNN
F 3 "~" H 3450 7025 50  0001 C CNN
	1    3450 7025
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 7325 3450 7025
Text Label 3450 7325 1    50   ~ 0
VIN
Wire Wire Line
	9000 3200 8700 3200
$Comp
L power:PWR_FLAG #FLG0105
U 1 1 60D2205A
P 3200 6900
F 0 "#FLG0105" H 3200 6975 50  0001 C CNN
F 1 "PWR_FLAG" H 3200 7073 50  0000 C CNN
F 2 "" H 3200 6900 50  0001 C CNN
F 3 "~" H 3200 6900 50  0001 C CNN
	1    3200 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 7325 3200 6900
Text Label 3200 7325 1    50   ~ 0
VS+
$Comp
L Connector_Generic:Conn_01x05 J7
U 1 1 60D63244
P 7500 1225
F 0 "J7" H 7450 1525 50  0000 L CNN
F 1 "Conn_01x05" H 7300 925 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7500 1225 50  0001 C CNN
F 3 "~" H 7500 1225 50  0001 C CNN
	1    7500 1225
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 1025 7000 1025
Text Label 7000 1025 0    50   ~ 0
Vsensor
Wire Wire Line
	7000 1125 7300 1125
Text Label 7000 1125 0    50   ~ 0
GND
Wire Wire Line
	7000 1225 7300 1225
Text Label 7000 1225 0    50   ~ 0
SCL
Wire Wire Line
	7000 1325 7300 1325
Text Label 7000 1325 0    50   ~ 0
SDA
Wire Wire Line
	7000 1425 7300 1425
Text Label 7000 1425 0    50   ~ 0
INT
Wire Wire Line
	8700 3300 9375 3300
Wire Wire Line
	9675 3300 9375 3300
Connection ~ 9375 3300
Wire Wire Line
	6700 3000 7000 3000
Connection ~ 7000 3000
Wire Wire Line
	6600 2900 6900 2900
Connection ~ 6900 2900
$Comp
L Device:R_Small R8
U 1 1 60C35398
P 7500 3200
F 0 "R8" V 7500 3200 50  0000 C CNN
F 1 "470" V 7425 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7500 3200 50  0001 C CNN
F 3 "~" H 7500 3200 50  0001 C CNN
	1    7500 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	7700 3200 7600 3200
Text Label 7000 3200 0    50   ~ 0
IR
$Comp
L Device:R_Small R7
U 1 1 60C3D53E
P 7275 3475
F 0 "R7" V 7275 3475 50  0000 C CNN
F 1 "47K" V 7200 3475 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7275 3475 50  0001 C CNN
F 3 "~" H 7275 3475 50  0001 C CNN
	1    7275 3475
	-1   0    0    1   
$EndComp
Wire Wire Line
	7275 3375 7275 3200
$Comp
L power:GND #PWR0101
U 1 1 60C41A48
P 7275 3650
F 0 "#PWR0101" H 7275 3400 50  0001 C CNN
F 1 "GND" H 7280 3477 50  0000 C CNN
F 2 "" H 7275 3650 50  0001 C CNN
F 3 "" H 7275 3650 50  0001 C CNN
	1    7275 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7275 3650 7275 3575
Connection ~ 7275 3200
Wire Wire Line
	7275 3200 7400 3200
Wire Wire Line
	7000 3200 7275 3200
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 60C16F38
P 6375 1125
F 0 "J4" H 6325 1325 50  0000 L CNN
F 1 "Conn_01x04" H 6175 825 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 6375 1125 50  0001 C CNN
F 3 "~" H 6375 1125 50  0001 C CNN
	1    6375 1125
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C4
U 1 1 60C48C70
P 1425 1850
F 0 "C4" V 1325 1975 50  0000 C CNN
F 1 "100n" V 1325 1700 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 1425 1850 50  0001 C CNN
F 3 "~" H 1425 1850 50  0001 C CNN
	1    1425 1850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1325 1850 1075 1850
Wire Wire Line
	1075 1850 1075 1600
Connection ~ 1075 1600
Wire Wire Line
	1075 1600 1100 1600
Wire Wire Line
	1525 1850 1750 1850
Wire Wire Line
	1750 1850 1750 1300
Connection ~ 1750 1300
Wire Wire Line
	1750 1300 1700 1300
Wire Notes Line
	3200 2875 6125 2875
Wire Notes Line
	6125 2875 6125 4275
Wire Notes Line
	6125 4275 3200 4275
Wire Notes Line
	3200 4275 3200 2875
Text Notes 3250 3025 0    71   ~ 14
>>> TODO: LoRaWAN V2.1 <<<
Text Notes 3250 3200 0    50   ~ 0
- set battery connector JST xxx\n- M3 holes not aligned !!\n
$EndSCHEMATC
