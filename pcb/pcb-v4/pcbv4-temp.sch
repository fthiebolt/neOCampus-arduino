EESchema Schematic File Version 4
LIBS:pcbv4-temp-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "neOSensor-addon: temperature sensor board"
Date "2018-08-26"
Rev "v4"
Comp "(c) neOCampus / Dr THIEBOLT François"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L neosensor_addon:MCP9808_MSOP U1
U 1 1 5B83A3CF
P 4900 3500
F 0 "U1" H 4700 3950 50  0000 L CNN
F 1 "MCP9808" H 4750 3600 50  0000 L CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 4850 3600 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/22203b.pdf" H 4650 3950 50  0001 C CNN
	1    4900 3500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR03
U 1 1 5B83A501
P 4900 2900
F 0 "#PWR03" H 4900 2750 50  0001 C CNN
F 1 "+3V3" H 4915 3073 50  0000 C CNN
F 2 "" H 4900 2900 50  0001 C CNN
F 3 "" H 4900 2900 50  0001 C CNN
	1    4900 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 2900 4900 2950
$Comp
L Device:C_Small C1
U 1 1 5B83A56B
P 5350 3050
F 0 "C1" H 5442 3096 50  0000 L CNN
F 1 "100n" H 5442 3005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5350 3050 50  0001 C CNN
F 3 "~" H 5350 3050 50  0001 C CNN
	1    5350 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 2950 4900 2950
Connection ~ 4900 2950
Wire Wire Line
	4900 2950 4900 3000
$Comp
L power:GND #PWR05
U 1 1 5B83A612
P 5350 3250
F 0 "#PWR05" H 5350 3000 50  0001 C CNN
F 1 "GND" H 5450 3250 50  0000 C CNN
F 2 "" H 5350 3250 50  0001 C CNN
F 3 "" H 5350 3250 50  0001 C CNN
	1    5350 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3250 5350 3150
$Comp
L power:GND #PWR04
U 1 1 5B83A64A
P 4900 4100
F 0 "#PWR04" H 4900 3850 50  0001 C CNN
F 1 "GND" H 5000 4100 50  0000 C CNN
F 2 "" H 4900 4100 50  0001 C CNN
F 3 "" H 4900 4100 50  0001 C CNN
	1    4900 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 4100 4900 4000
Wire Wire Line
	5650 3500 5300 3500
Text Label 5650 3500 2    50   ~ 0
INT
Wire Wire Line
	4150 3200 4500 3200
Wire Wire Line
	4150 3300 4500 3300
Text Label 4150 3200 0    50   ~ 0
SDA
Text Label 4150 3300 0    50   ~ 0
SCL
Text Notes 4450 2550 0    50   ~ 0
Temperature sensor
$Comp
L Connector_Generic:Conn_01x05 J1
U 1 1 5B83A876
P 7150 3500
F 0 "J1" H 7070 3075 50  0000 C CNN
F 1 "Conn_01x05" H 7070 3166 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7150 3500 50  0001 C CNN
F 3 "~" H 7150 3500 50  0001 C CNN
	1    7150 3500
	-1   0    0    1   
$EndComp
Text Notes 7400 3000 0    50   ~ 0
Interface
Text Label 7800 3700 2    50   ~ 0
+3V3
Text Label 7800 3600 2    50   ~ 0
GND
$Comp
L power:+3V3 #PWR06
U 1 1 5B83AA44
P 7900 3800
F 0 "#PWR06" H 7900 3650 50  0001 C CNN
F 1 "+3V3" H 7915 3973 50  0000 C CNN
F 2 "" H 7900 3800 50  0001 C CNN
F 3 "" H 7900 3800 50  0001 C CNN
	1    7900 3800
	-1   0    0    1   
$EndComp
Wire Wire Line
	7900 3800 7900 3750
Wire Wire Line
	7350 3700 7900 3700
$Comp
L power:GND #PWR07
U 1 1 5B83AAEE
P 8100 3800
F 0 "#PWR07" H 8100 3550 50  0001 C CNN
F 1 "GND" H 8105 3627 50  0000 C CNN
F 2 "" H 8100 3800 50  0001 C CNN
F 3 "" H 8100 3800 50  0001 C CNN
	1    8100 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 3800 8100 3750
Wire Wire Line
	7350 3600 8100 3600
Wire Wire Line
	7800 3500 7350 3500
Text Label 7800 3500 2    50   ~ 0
SCL
Wire Wire Line
	7800 3400 7350 3400
Text Label 7800 3400 2    50   ~ 0
SDA
Wire Wire Line
	7800 3300 7350 3300
Text Label 7800 3300 2    50   ~ 0
INT
$Comp
L Jumper:SolderJumper_3_Bridged12 JP1
U 1 1 5B83AEF1
P 3250 3550
F 0 "JP1" V 3204 3618 50  0000 L CNN
F 1 "SolderJumper_3_Bridged12" V 3295 3618 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_Pad1.0x1.5mm" H 3250 3550 50  0001 C CNN
F 3 "~" H 3250 3550 50  0001 C CNN
	1    3250 3550
	0    -1   1    0   
$EndComp
$Comp
L Jumper:SolderJumper_3_Bridged12 JP2
U 1 1 5B83B050
P 3550 3700
F 0 "JP2" V 3504 3768 50  0000 L CNN
F 1 "SolderJumper_3_Bridged12" V 3595 3768 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_Pad1.0x1.5mm" H 3550 3700 50  0001 C CNN
F 3 "~" H 3550 3700 50  0001 C CNN
	1    3550 3700
	0    -1   1    0   
$EndComp
$Comp
L Jumper:SolderJumper_3_Bridged12 JP3
U 1 1 5B83B07A
P 3800 3850
F 0 "JP3" V 3754 3918 50  0000 L CNN
F 1 "SolderJumper_3_Bridged12" V 3845 3918 50  0000 L CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_Pad1.0x1.5mm" H 3800 3850 50  0001 C CNN
F 3 "~" H 3800 3850 50  0001 C CNN
	1    3800 3850
	0    -1   1    0   
$EndComp
Wire Wire Line
	4500 3600 4000 3600
Wire Wire Line
	4000 3600 4000 3550
Wire Wire Line
	4000 3550 3400 3550
Wire Wire Line
	4500 3700 3700 3700
$Comp
L power:GND #PWR02
U 1 1 5B83B62A
P 3550 4150
F 0 "#PWR02" H 3550 3900 50  0001 C CNN
F 1 "GND" H 3650 4150 50  0000 C CNN
F 2 "" H 3550 4150 50  0001 C CNN
F 3 "" H 3550 4150 50  0001 C CNN
	1    3550 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 4150 3550 4100
Wire Wire Line
	3800 4050 3800 4100
Wire Wire Line
	3800 4100 3550 4100
Connection ~ 3550 4100
Wire Wire Line
	3550 4100 3550 3900
Wire Wire Line
	3250 3750 3250 4100
Wire Wire Line
	3250 4100 3550 4100
$Comp
L power:+3V3 #PWR01
U 1 1 5B83BFAE
P 3550 3250
F 0 "#PWR01" H 3550 3100 50  0001 C CNN
F 1 "+3V3" H 3565 3423 50  0000 C CNN
F 2 "" H 3550 3250 50  0001 C CNN
F 3 "" H 3550 3250 50  0001 C CNN
	1    3550 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3350 3250 3250
Wire Wire Line
	3250 3250 3550 3250
Wire Wire Line
	3550 3500 3550 3250
Connection ~ 3550 3250
Wire Wire Line
	3800 3650 3800 3250
Wire Wire Line
	3800 3250 3550 3250
Text Notes 2500 3050 0    50   ~ 0
Note: temperature sensor default i2c addr is 0x48\nwith A0, A1, A2 = VCC
$Comp
L Device:C_Small C2
U 1 1 5B83D5AF
P 8000 3750
F 0 "C2" V 8100 3750 50  0000 C CNN
F 1 "100n" V 7900 3750 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8000 3750 50  0001 C CNN
F 3 "~" H 8000 3750 50  0001 C CNN
	1    8000 3750
	0    1    1    0   
$EndComp
Connection ~ 8100 3750
Wire Wire Line
	8100 3750 8100 3600
Connection ~ 7900 3750
Wire Wire Line
	7900 3750 7900 3700
Wire Wire Line
	3950 3850 4000 3850
Wire Wire Line
	4000 3850 4000 3800
Wire Wire Line
	4000 3800 4500 3800
$Comp
L Mechanical:MountingHole_Pad MH1
U 1 1 5B83F7B4
P 9650 3250
F 0 "MH1" V 9887 3255 50  0000 C CNN
F 1 "MountingHole_Pad" V 9796 3255 50  0000 C CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 9650 3250 50  0001 C CNN
F 3 "~" H 9650 3250 50  0001 C CNN
	1    9650 3250
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad MH2
U 1 1 5B84063A
P 9650 3650
F 0 "MH2" V 9887 3655 50  0000 C CNN
F 1 "MountingHole_Pad" V 9796 3655 50  0000 C CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 9650 3650 50  0001 C CNN
F 3 "~" H 9650 3650 50  0001 C CNN
	1    9650 3650
	0    -1   -1   0   
$EndComp
Text Notes 9600 2850 0    50   ~ 0
Mechanical
Wire Wire Line
	10200 3250 9750 3250
Text Label 10200 3250 2    50   ~ 0
GND
Wire Wire Line
	10200 3650 9750 3650
Text Label 10200 3650 2    50   ~ 0
GND
Text Label 4150 3600 0    50   ~ 0
A0
Text Label 4150 3700 0    50   ~ 0
A1
Text Label 4150 3800 0    50   ~ 0
A2
$EndSCHEMATC
