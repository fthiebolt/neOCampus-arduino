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
L neosensor_addon:IRsensor IR1
U 1 1 60A834AB
P 1450 3750
F 0 "IR1" V 1496 3572 50  0000 R CNN
F 1 "IRsensor" V 1405 3572 50  0000 R CNN
F 2 "neosensor_addon:IRsensor_TH_and_smd" H 1450 3750 50  0001 C CIN
F 3 "" H 1450 3750 50  0001 C CNN
	1    1450 3750
	-1   0    0    1   
$EndComp
$Comp
L pspice:0 #GND0111
U 1 1 60A9911E
P 1700 3650
F 0 "#GND0111" H 1700 3550 50  0001 C CNN
F 1 "0" H 1700 3550 50  0000 C CNN
F 2 "" H 1700 3650 50  0001 C CNN
F 3 "~" H 1700 3650 50  0001 C CNN
	1    1700 3650
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 60AB1160
P 3950 1100
F 0 "J2" H 4030 1092 50  0000 L CNN
F 1 "Conn_01x04" H 4030 1001 50  0000 L CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_1x04_P2.00mm_Vertical" H 3950 1100 50  0001 C CNN
F 3 "~" H 3950 1100 50  0001 C CNN
	1    3950 1100
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 60AB1E8C
P 3950 2200
F 0 "J3" H 4030 2192 50  0000 L CNN
F 1 "Conn_01x04" H 4030 2101 50  0000 L CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_1x04_P2.00mm_Vertical" H 3950 2200 50  0001 C CNN
F 3 "~" H 3950 2200 50  0001 C CNN
	1    3950 2200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 60AB25DA
P 3950 1650
F 0 "J4" H 4030 1642 50  0000 L CNN
F 1 "Conn_01x04" H 4030 1551 50  0000 L CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_1x04_P2.00mm_Vertical" H 3950 1650 50  0001 C CNN
F 3 "~" H 3950 1650 50  0001 C CNN
	1    3950 1650
	-1   0    0    1   
$EndComp
$Comp
L pspice:0 #GND0113
U 1 1 60AC4429
P 4150 2000
F 0 "#GND0113" H 4150 1900 50  0001 C CNN
F 1 "0" V 4150 1850 50  0000 L CNN
F 2 "" H 4150 2000 50  0001 C CNN
F 3 "~" H 4150 2000 50  0001 C CNN
	1    4150 2000
	0    -1   -1   0   
$EndComp
Text GLabel 4150 1200 2    50   Input ~ 0
3v3
Text GLabel 4150 2300 2    50   Input ~ 0
3v3
Text GLabel 4150 1750 2    50   Input ~ 0
3v3
Text GLabel 4150 1100 2    50   Input ~ 0
SDA
Text GLabel 4150 2200 2    50   Input ~ 0
SDA
Text GLabel 4150 1650 2    50   Input ~ 0
SDA
Text GLabel 4150 1000 2    50   Input ~ 0
SCL
Text GLabel 4150 2100 2    50   Input ~ 0
SCL
Text GLabel 4150 1550 2    50   Input ~ 0
SCL
Text GLabel 1700 3850 2    50   Input ~ 0
3v3
$Comp
L pspice:0 #GND?
U 1 1 60B5D4F9
P 1050 7050
AR Path="/61173FE1/60B5D4F9" Ref="#GND?"  Part="1" 
AR Path="/60B5D4F9" Ref="#GND0108"  Part="1" 
F 0 "#GND0108" H 1050 6950 50  0001 C CNN
F 1 "0" H 1050 6943 50  0000 C CNN
F 2 "" H 1050 7050 50  0001 C CNN
F 3 "~" H 1050 7050 50  0001 C CNN
	1    1050 7050
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Barrel_Jack J?
U 1 1 60B5D507
P 750 6950
AR Path="/61173FE1/60B5D507" Ref="J?"  Part="1" 
AR Path="/60B5D507" Ref="J1"  Part="1" 
F 0 "J1" H 807 7275 50  0000 C CNN
F 1 "Barrel_Jack" H 807 7184 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 800 6910 50  0001 C CNN
F 3 "~" H 800 6910 50  0001 C CNN
	1    750  6950
	1    0    0    -1  
$EndComp
Text GLabel 3400 6450 2    50   Input ~ 0
12v
Text GLabel 3400 6550 2    50   Input ~ 0
5v
Text GLabel 1750 900  2    50   Input ~ 0
Push_1
Text GLabel 1750 1650 2    50   Input ~ 0
Push_2
Text GLabel 1750 2350 2    50   Input ~ 0
Push_3
Text GLabel 700  3750 0    50   Input ~ 0
IR
Text GLabel 3400 6850 2    50   Input ~ 0
3v3
$Comp
L pspice:0 #GND?
U 1 1 60B5D4F3
P 2050 7250
AR Path="/61173FE1/60B5D4F3" Ref="#GND?"  Part="1" 
AR Path="/60B5D4F3" Ref="#GND0107"  Part="1" 
F 0 "#GND0107" H 2050 7150 50  0001 C CNN
F 1 "0" H 2050 7142 50  0000 C CNN
F 2 "" H 2050 7250 50  0001 C CNN
F 3 "~" H 2050 7250 50  0001 C CNN
	1    2050 7250
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND?
U 1 1 60B5D4E5
P 3300 7150
AR Path="/61173FE1/60B5D4E5" Ref="#GND?"  Part="1" 
AR Path="/60B5D4E5" Ref="#GND0106"  Part="1" 
F 0 "#GND0106" H 3300 7050 50  0001 C CNN
F 1 "0" H 3298 7038 50  0000 C CNN
F 2 "" H 3300 7150 50  0001 C CNN
F 3 "~" H 3300 7150 50  0001 C CNN
	1    3300 7150
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0105
U 1 1 60B5D4D6
P 2900 7150
AR Path="/60B5D4D6" Ref="#GND0105"  Part="1" 
AR Path="/61173FE1/60B5D4D6" Ref="#GND?"  Part="1" 
F 0 "#GND0105" H 2900 7050 50  0001 C CNN
F 1 "0" H 2900 7052 50  0000 C CNN
F 2 "" H 2900 7150 50  0001 C CNN
F 3 "~" H 2900 7150 50  0001 C CNN
	1    2900 7150
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0104
U 1 1 60B5D4C9
P 1850 7250
AR Path="/60B5D4C9" Ref="#GND0104"  Part="1" 
AR Path="/61173FE1/60B5D4C9" Ref="#GND?"  Part="1" 
F 0 "#GND0104" H 1850 7150 50  0001 C CNN
F 1 "0" H 1850 7141 50  0000 C CNN
F 2 "" H 1850 7250 50  0001 C CNN
F 3 "~" H 1850 7250 50  0001 C CNN
	1    1850 7250
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117 U2
U 1 1 60B5D4C3
P 2900 6850
AR Path="/60B5D4C3" Ref="U2"  Part="1" 
AR Path="/61173FE1/60B5D4C3" Ref="U?"  Part="1" 
F 0 "U2" H 2900 7092 50  0000 C CNN
F 1 "AMS1117" H 2900 7001 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2900 7050 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 3000 6600 50  0001 C CNN
	1    2900 6850
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0101
U 1 1 60B5D4B0
P 2500 7500
AR Path="/60B5D4B0" Ref="#GND0101"  Part="1" 
AR Path="/61173FE1/60B5D4B0" Ref="#GND?"  Part="1" 
F 0 "#GND0101" H 2500 7400 50  0001 C CNN
F 1 "0" H 2500 7390 50  0000 C CNN
F 2 "" H 2500 7500 50  0001 C CNN
F 3 "~" H 2500 7500 50  0001 C CNN
	1    2500 7500
	1    0    0    -1  
$EndComp
Text GLabel 4100 2700 2    50   Input ~ 0
3v3
Text GLabel 4100 2900 2    50   Input ~ 0
SCL
Text GLabel 4100 3000 2    50   Input ~ 0
SDA
$Comp
L pspice:0 #GND0117
U 1 1 6125F9DF
P 3500 3000
F 0 "#GND0117" H 3500 2900 50  0001 C CNN
F 1 "0" V 3500 2850 50  0000 L CNN
F 2 "" H 3500 3000 50  0001 C CNN
F 3 "~" H 3500 3000 50  0001 C CNN
	1    3500 3000
	0    1    1    0   
$EndComp
$Comp
L LED:APA102 D3
U 1 1 612815DC
P 3750 5750
F 0 "D3" H 3750 6231 50  0000 C CNN
F 1 "APA102" H 3750 6140 50  0000 C CNN
F 2 "LED_SMD:LED_WS2812_PLCC6_5.0x5.0mm_P1.6mm" H 3800 5450 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 3850 5375 50  0001 L TNN
	1    3750 5750
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D4
U 1 1 6128410F
P 4350 5750
F 0 "D4" H 4350 6231 50  0000 C CNN
F 1 "APA102" H 4350 6140 50  0000 C CNN
F 2 "LED_SMD:LED_WS2812_PLCC6_5.0x5.0mm_P1.6mm" H 4400 5450 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 4450 5375 50  0001 L TNN
	1    4350 5750
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D5
U 1 1 612CF7A7
P 4950 5750
F 0 "D5" H 4950 6231 50  0000 C CNN
F 1 "APA102" H 4950 6140 50  0000 C CNN
F 2 "LED_SMD:LED_WS2812_PLCC6_5.0x5.0mm_P1.6mm" H 5000 5450 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 5050 5375 50  0001 L TNN
	1    4950 5750
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0118
U 1 1 61413F41
P 3450 6050
F 0 "#GND0118" H 3450 5950 50  0001 C CNN
F 1 "0" V 3450 5900 50  0000 L CNN
F 2 "" H 3450 6050 50  0001 C CNN
F 3 "~" H 3450 6050 50  0001 C CNN
	1    3450 6050
	0    1    1    0   
$EndComp
Text GLabel 3450 5450 0    50   Input ~ 0
5v
$Comp
L neosensor_addon:ATECC508A J5
U 1 1 60AFCD17
P 3800 2800
F 0 "J5" H 3800 3167 50  0000 C CNN
F 1 "ATECC508A" H 3800 3076 50  0000 C CNN
F 2 "neosensor_addon:SOIC-8_3.9x5.4mm_P1.27mm_HandSolder" H 3800 2800 50  0001 L BNN
F 3 "" H 3800 2800 50  0001 L BNN
F 4 "Harting" H 3800 2800 50  0001 L BNN "MANUFACTURER"
F 5 "2021-04-27" H 3800 2800 50  0001 L BNN "PARTREV"
F 6 "8 mm" H 3800 2800 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 7 "Manufacturer Recommendations" H 3800 2800 50  0001 L BNN "STANDARD"
	1    3800 2800
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:DCDC_LM2596 U1
U 1 1 60AFE4C0
P 1950 6950
F 0 "U1" H 1950 7315 50  0000 C CNN
F 1 "DCDC_LM2596" H 1950 7224 50  0000 C CNN
F 2 "neosensor_addon:DC_DC_LM2596" H 1900 6950 50  0001 C CNN
F 3 "" H 1900 6950 50  0001 C CNN
	1    1950 6950
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:SAA1064 U6
U 1 1 60AFEF04
P 9150 2600
F 0 "U6" H 9150 3400 50  0000 C CNN
F 1 "SAA1064" H 9150 3300 50  0000 C CNN
F 2 "neosensor_addon:SOIC-24W_7.5x15.4mm_P1.27mm_HandSolder" H 9150 2600 50  0001 L BNN
F 3 "" H 9150 2600 50  0001 L BNN
	1    9150 2600
	1    0    0    -1  
$EndComp
$Comp
L neosensor_addon:G71Y SW1
U 1 1 60B2B808
P 950 900
F 0 "SW1" H 950 1125 50  0000 C CNN
F 1 "G71Y" H 950 1034 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 950 900 50  0001 C CIN
F 3 "" H 950 900 50  0001 C CNN
	1    950  900 
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 60AFBB62
P 2500 950
F 0 "H1" H 2600 999 50  0000 L CNN
F 1 "MountingHole_Pad" H 2600 908 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 2500 950 50  0001 C CNN
F 3 "~" H 2500 950 50  0001 C CNN
	1    2500 950 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 60AFD2AF
P 2500 1750
F 0 "H3" H 2600 1799 50  0000 L CNN
F 1 "MountingHole_Pad" H 2600 1708 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 2500 1750 50  0001 C CNN
F 3 "~" H 2500 1750 50  0001 C CNN
	1    2500 1750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 60B00D96
P 2500 1350
F 0 "H2" H 2600 1399 50  0000 L CNN
F 1 "MountingHole_Pad" H 2600 1308 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 2500 1350 50  0001 C CNN
F 3 "~" H 2500 1350 50  0001 C CNN
	1    2500 1350
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 60B04819
P 2500 2150
F 0 "H4" H 2600 2199 50  0000 L CNN
F 1 "MountingHole_Pad" H 2600 2108 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 2500 2150 50  0001 C CNN
F 3 "~" H 2500 2150 50  0001 C CNN
	1    2500 2150
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0121
U 1 1 60B14802
P 2500 2250
F 0 "#GND0121" H 2500 2150 50  0001 C CNN
F 1 "0" H 2500 2150 50  0000 C CNN
F 2 "" H 2500 2250 50  0001 C CNN
F 3 "~" H 2500 2250 50  0001 C CNN
	1    2500 2250
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0122
U 1 1 60B150C7
P 2500 1450
F 0 "#GND0122" H 2500 1350 50  0001 C CNN
F 1 "0" H 2500 1350 50  0000 C CNN
F 2 "" H 2500 1450 50  0001 C CNN
F 3 "~" H 2500 1450 50  0001 C CNN
	1    2500 1450
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0123
U 1 1 60B15AC9
P 2500 1850
F 0 "#GND0123" H 2500 1750 50  0001 C CNN
F 1 "0" H 2495 1735 50  0000 C CNN
F 2 "" H 2500 1850 50  0001 C CNN
F 3 "~" H 2500 1850 50  0001 C CNN
	1    2500 1850
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0124
U 1 1 60B16380
P 2500 1050
F 0 "#GND0124" H 2500 950 50  0001 C CNN
F 1 "0" H 2500 950 50  0000 C CNN
F 2 "" H 2500 1050 50  0001 C CNN
F 3 "~" H 2500 1050 50  0001 C CNN
	1    2500 1050
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D8
U 1 1 60B650F7
P 5550 5750
F 0 "D8" H 5550 6231 50  0000 C CNN
F 1 "APA102" H 5550 6140 50  0000 C CNN
F 2 "LED_SMD:LED_WS2812_PLCC6_5.0x5.0mm_P1.6mm" H 5600 5450 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 5650 5375 50  0001 L TNN
	1    5550 5750
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D9
U 1 1 60B650FD
P 6150 5750
F 0 "D9" H 6150 6231 50  0000 C CNN
F 1 "APA102" H 6150 6140 50  0000 C CNN
F 2 "LED_SMD:LED_WS2812_PLCC6_5.0x5.0mm_P1.6mm" H 6200 5450 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 6250 5375 50  0001 L TNN
	1    6150 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C5
U 1 1 60B92AE4
P 3300 7050
F 0 "C5" H 3388 7096 50  0000 L CNN
F 1 "100µF" H 3388 7005 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 3300 7050 50  0001 C CNN
F 3 "~" H 3300 7050 50  0001 C CNN
	1    3300 7050
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N4148 D1
U 1 1 60B7E350
P 1200 6850
F 0 "D1" H 1200 6634 50  0000 C CNN
F 1 "1N4148" H 1200 6725 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 1200 6675 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 1200 6850 50  0001 C CNN
	1    1200 6850
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 60BBC5F1
P 1300 1100
F 0 "R2" H 1370 1146 50  0000 L CNN
F 1 "15K" H 1370 1055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1230 1100 50  0001 C CNN
F 3 "~" H 1300 1100 50  0001 C CNN
	1    1300 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 60BBF12C
P 1700 1100
F 0 "C1" H 1815 1146 50  0000 L CNN
F 1 "1µF" H 1815 1055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1738 950 50  0001 C CNN
F 3 "~" H 1700 1100 50  0001 C CNN
	1    1700 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 60BC40BF
P 1500 900
F 0 "R5" V 1293 900 50  0000 C CNN
F 1 "15K" V 1384 900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1430 900 50  0001 C CNN
F 3 "~" H 1500 900 50  0001 C CNN
	1    1500 900 
	0    1    1    0   
$EndComp
Wire Wire Line
	1350 6850 1450 6850
Wire Wire Line
	1450 6850 1450 6450
Connection ~ 1450 6850
Wire Wire Line
	1450 6850 1550 6850
Wire Wire Line
	2350 6850 2500 6850
Wire Wire Line
	3200 6850 3300 6850
Wire Wire Line
	2500 6850 2500 6550
Wire Wire Line
	2500 6550 3400 6550
Connection ~ 2500 6850
Wire Wire Line
	2500 6850 2600 6850
Wire Wire Line
	1450 6450 3400 6450
$Comp
L Device:C C4
U 1 1 60CC56B3
P 2500 7350
F 0 "C4" H 2615 7396 50  0000 L CNN
F 1 "100nF" H 2615 7305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 2538 7200 50  0001 C CNN
F 3 "~" H 2500 7350 50  0001 C CNN
	1    2500 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 6850 2500 7200
Wire Wire Line
	1150 900  1300 900 
Wire Wire Line
	1650 900  1700 900 
Wire Wire Line
	1700 950  1700 900 
Connection ~ 1700 900 
Wire Wire Line
	1700 900  1750 900 
Wire Wire Line
	1300 950  1300 900 
Connection ~ 1300 900 
Wire Wire Line
	1300 900  1350 900 
Wire Wire Line
	1150 1000 1150 1250
Wire Wire Line
	1150 1250 1300 1250
Connection ~ 1300 1250
Wire Wire Line
	1300 1250 1700 1250
$Comp
L pspice:0 #GND0115
U 1 1 60D3CF49
P 1150 1300
F 0 "#GND0115" H 1150 1200 50  0001 C CNN
F 1 "0" H 1150 1200 50  0000 C CNN
F 2 "" H 1150 1300 50  0001 C CNN
F 3 "~" H 1150 1300 50  0001 C CNN
	1    1150 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 1250 1150 1300
Connection ~ 1150 1250
$Comp
L neosensor_addon:G71Y SW2
U 1 1 60D6D9C5
P 950 1650
F 0 "SW2" H 950 1875 50  0000 C CNN
F 1 "G71Y" H 950 1784 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 950 1650 50  0001 C CIN
F 3 "" H 950 1650 50  0001 C CNN
	1    950  1650
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 60D6D9CB
P 1300 1850
F 0 "R3" H 1370 1896 50  0000 L CNN
F 1 "15K" H 1370 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1230 1850 50  0001 C CNN
F 3 "~" H 1300 1850 50  0001 C CNN
	1    1300 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 60D6D9D1
P 1700 1850
F 0 "C2" H 1815 1896 50  0000 L CNN
F 1 "1µF" H 1815 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1738 1700 50  0001 C CNN
F 3 "~" H 1700 1850 50  0001 C CNN
	1    1700 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 60D6D9D7
P 1500 1650
F 0 "R6" V 1293 1650 50  0000 C CNN
F 1 "15K" V 1384 1650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1430 1650 50  0001 C CNN
F 3 "~" H 1500 1650 50  0001 C CNN
	1    1500 1650
	0    1    1    0   
$EndComp
Wire Wire Line
	1150 1650 1300 1650
Wire Wire Line
	1650 1650 1700 1650
Wire Wire Line
	1700 1700 1700 1650
Connection ~ 1700 1650
Wire Wire Line
	1700 1650 1750 1650
Wire Wire Line
	1300 1700 1300 1650
Connection ~ 1300 1650
Wire Wire Line
	1300 1650 1350 1650
Wire Wire Line
	1150 1750 1150 2000
Wire Wire Line
	1150 2000 1300 2000
Connection ~ 1300 2000
Wire Wire Line
	1300 2000 1700 2000
$Comp
L pspice:0 #GND0119
U 1 1 60D6D9E9
P 1150 2050
F 0 "#GND0119" H 1150 1950 50  0001 C CNN
F 1 "0" H 1150 1950 50  0000 C CNN
F 2 "" H 1150 2050 50  0001 C CNN
F 3 "~" H 1150 2050 50  0001 C CNN
	1    1150 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 2000 1150 2050
Connection ~ 1150 2000
$Comp
L neosensor_addon:G71Y SW3
U 1 1 60D76A18
P 950 2350
F 0 "SW3" H 950 2575 50  0000 C CNN
F 1 "G71Y" H 950 2484 50  0000 C CNN
F 2 "neosensor_addon:G71Y_µsw_smd" H 950 2350 50  0001 C CIN
F 3 "" H 950 2350 50  0001 C CNN
	1    950  2350
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 60D76A1E
P 1300 2550
F 0 "R4" H 1370 2596 50  0000 L CNN
F 1 "15K" H 1370 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1230 2550 50  0001 C CNN
F 3 "~" H 1300 2550 50  0001 C CNN
	1    1300 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 60D76A24
P 1700 2550
F 0 "C3" H 1815 2596 50  0000 L CNN
F 1 "1µF" H 1815 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 1738 2400 50  0001 C CNN
F 3 "~" H 1700 2550 50  0001 C CNN
	1    1700 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 60D76A2A
P 1500 2350
F 0 "R7" V 1293 2350 50  0000 C CNN
F 1 "15K" V 1384 2350 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1430 2350 50  0001 C CNN
F 3 "~" H 1500 2350 50  0001 C CNN
	1    1500 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	1150 2350 1300 2350
Wire Wire Line
	1650 2350 1700 2350
Wire Wire Line
	1700 2400 1700 2350
Connection ~ 1700 2350
Wire Wire Line
	1700 2350 1750 2350
Wire Wire Line
	1300 2400 1300 2350
Connection ~ 1300 2350
Wire Wire Line
	1300 2350 1350 2350
Wire Wire Line
	1150 2450 1150 2700
Wire Wire Line
	1150 2700 1300 2700
Connection ~ 1300 2700
Wire Wire Line
	1300 2700 1700 2700
$Comp
L pspice:0 #GND0120
U 1 1 60D76A3C
P 1150 2750
F 0 "#GND0120" H 1150 2650 50  0001 C CNN
F 1 "0" H 1150 2650 50  0000 C CNN
F 2 "" H 1150 2750 50  0001 C CNN
F 3 "~" H 1150 2750 50  0001 C CNN
	1    1150 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 2700 1150 2750
Connection ~ 1150 2700
Text GLabel 750  900  0    50   Input ~ 0
3v3
Text GLabel 750  1650 0    50   Input ~ 0
3v3
Text GLabel 750  2350 0    50   Input ~ 0
3v3
Wire Wire Line
	4500 8675 5100 8675
Wire Wire Line
	4700 8825 5300 8825
$Comp
L pspice:0 #GND0114
U 1 1 60AC4E5B
P 4150 1450
F 0 "#GND0114" H 4150 1350 50  0001 C CNN
F 1 "0" V 4150 1300 50  0000 L CNN
F 2 "" H 4150 1450 50  0001 C CNN
F 3 "~" H 4150 1450 50  0001 C CNN
	1    4150 1450
	0    -1   -1   0   
$EndComp
$Comp
L pspice:0 #GND0112
U 1 1 60AC3B51
P 4150 900
F 0 "#GND0112" H 4150 800 50  0001 C CNN
F 1 "0" V 4150 750 50  0000 L CNN
F 2 "" H 4150 900 50  0001 C CNN
F 3 "~" H 4150 900 50  0001 C CNN
	1    4150 900 
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 61383ACF
P 1100 4000
F 0 "R1" H 1170 4046 50  0000 L CNN
F 1 "47k" H 1170 3955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1030 4000 50  0001 C CNN
F 3 "~" H 1100 4000 50  0001 C CNN
	1    1100 4000
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0125
U 1 1 61384771
P 1100 4150
F 0 "#GND0125" H 1100 4050 50  0001 C CNN
F 1 "0" H 1100 4000 50  0000 C CNN
F 2 "" H 1100 4150 50  0001 C CNN
F 3 "~" H 1100 4150 50  0001 C CNN
	1    1100 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 3750 1100 3750
Wire Wire Line
	1100 3750 1100 3850
Connection ~ 1100 3750
Wire Wire Line
	1100 3750 1200 3750
Wire Wire Line
	3300 6850 3300 6950
Connection ~ 3300 6850
Wire Wire Line
	3300 6850 3400 6850
Text GLabel 9500 3150 2    50   Input ~ 0
12v
$Comp
L pspice:0 #GND0109
U 1 1 60BA5CFB
P 8775 2050
F 0 "#GND0109" H 8775 1950 50  0001 C CNN
F 1 "0" V 8775 1925 50  0000 L CNN
F 2 "" H 8775 2050 50  0001 C CNN
F 3 "~" H 8775 2050 50  0001 C CNN
	1    8775 2050
	0    1    1    0   
$EndComp
Text GLabel 7525 2125 3    50   Input ~ 0
12v
Text GLabel 9500 2050 2    50   Input ~ 0
SCL
Text GLabel 9500 2150 2    50   Input ~ 0
SDA
Text GLabel 10750 2125 3    50   Input ~ 0
12v
$Comp
L pspice:0 #GND0110
U 1 1 60BA5D91
P 8775 3150
F 0 "#GND0110" H 8775 3050 50  0001 C CNN
F 1 "0" V 8775 3025 50  0000 L CNN
F 2 "" H 8775 3150 50  0001 C CNN
F 3 "~" H 8775 3150 50  0001 C CNN
	1    8775 3150
	0    1    1    0   
$EndComp
$Comp
L pspice:0 #GND0116
U 1 1 60BA5D97
P 8775 2150
F 0 "#GND0116" H 8775 2050 50  0001 C CNN
F 1 "0" V 8775 2025 50  0000 L CNN
F 2 "" H 8775 2150 50  0001 C CNN
F 3 "~" H 8775 2150 50  0001 C CNN
	1    8775 2150
	0    1    1    0   
$EndComp
Text Label 8125 1600 0    50   ~ 0
a10
Text Label 8125 1500 0    50   ~ 0
a9
Text Label 7225 1500 2    50   ~ 0
a8
Text Label 8125 1100 0    50   ~ 0
a6
Text Label 8125 1200 0    50   ~ 0
a4
Text Label 8125 1400 0    50   ~ 0
a2
Text Label 8125 1300 0    50   ~ 0
a3
Text Label 8125 1000 0    50   ~ 0
a7
Wire Wire Line
	8125 1000 8225 1000
Wire Wire Line
	8125 1100 8225 1100
Wire Wire Line
	8125 1200 8225 1200
Wire Wire Line
	8125 1300 8225 1300
Wire Wire Line
	8125 1400 8225 1400
Wire Wire Line
	8125 1500 8225 1500
Wire Wire Line
	8125 1600 8225 1600
Entry Wire Line
	8225 1000 8325 1100
Entry Wire Line
	8225 1100 8325 1200
Entry Wire Line
	8225 1200 8325 1300
Entry Wire Line
	8225 1300 8325 1400
Entry Wire Line
	8225 1400 8325 1500
Entry Wire Line
	8225 1500 8325 1600
Entry Wire Line
	8225 1600 8325 1700
Text Label 8525 1600 2    50   ~ 0
a10
Text Label 8525 1500 2    50   ~ 0
a9
Text Label 8525 1700 2    50   ~ 0
a8
Text Label 8525 1100 2    50   ~ 0
a6
Text Label 8525 1200 2    50   ~ 0
a4
Text Label 8525 1400 2    50   ~ 0
a2
Text Label 8525 1300 2    50   ~ 0
a3
Text Label 8525 1000 2    50   ~ 0
a7
Entry Wire Line
	8425 1000 8325 1100
Entry Wire Line
	8425 1100 8325 1200
Entry Wire Line
	8425 1200 8325 1300
Entry Wire Line
	8425 1300 8325 1400
Entry Wire Line
	8425 1400 8325 1500
Entry Wire Line
	8425 1500 8325 1600
Entry Wire Line
	8425 1600 8325 1700
Entry Wire Line
	8425 1700 8325 1800
Text Label 8775 2950 2    50   ~ 0
a7
Text Label 8775 2650 2    50   ~ 0
a3
Text Label 8775 2550 2    50   ~ 0
a2
Text Label 8775 2750 2    50   ~ 0
a4
Text Label 8775 2850 2    50   ~ 0
a6
Text Label 8775 2250 2    50   ~ 0
a8
Text Label 8775 2450 2    50   ~ 0
a9
Text Label 8775 2350 2    50   ~ 0
a10
Entry Wire Line
	8425 2950 8325 2850
Entry Wire Line
	8425 2850 8325 2750
Entry Wire Line
	8425 2550 8325 2450
Entry Wire Line
	8425 2350 8325 2250
Entry Wire Line
	8425 2250 8325 2150
Entry Wire Line
	8425 2450 8325 2350
Entry Wire Line
	8425 2650 8325 2550
Entry Wire Line
	8425 2750 8325 2650
Entry Wire Line
	9850 2750 9950 2650
Entry Wire Line
	9850 2650 9950 2550
Entry Wire Line
	9850 2450 9950 2350
Entry Wire Line
	9850 2250 9950 2150
Entry Wire Line
	9850 2350 9950 2250
Entry Wire Line
	9850 2550 9950 2450
Entry Wire Line
	9850 2850 9950 2750
Entry Wire Line
	9850 2950 9950 2850
Text Label 9500 2350 0    50   ~ 0
b10
Text Label 9500 2450 0    50   ~ 0
b9
Text Label 9500 2250 0    50   ~ 0
b8
Text Label 9500 2850 0    50   ~ 0
b6
Text Label 9500 2750 0    50   ~ 0
b4
Text Label 9500 2550 0    50   ~ 0
b2
Text Label 9500 2650 0    50   ~ 0
b3
Text Label 9500 2950 0    50   ~ 0
b7
Entry Wire Line
	10050 1600 9950 1700
Entry Wire Line
	10050 1500 9950 1600
Entry Wire Line
	10050 1400 9950 1500
Entry Wire Line
	10050 1300 9950 1400
Entry Wire Line
	10050 1200 9950 1300
Entry Wire Line
	10050 1100 9950 1200
Entry Wire Line
	10050 1000 9950 1100
Wire Wire Line
	10150 1600 10050 1600
Wire Wire Line
	10150 1500 10050 1500
Wire Wire Line
	10150 1400 10050 1400
Wire Wire Line
	10150 1300 10050 1300
Wire Wire Line
	10150 1200 10050 1200
Wire Wire Line
	10150 1100 10050 1100
Wire Wire Line
	10150 1000 10050 1000
Text Label 10150 1000 2    50   ~ 0
b7
Text Label 10150 1300 2    50   ~ 0
b3
Text Label 10150 1400 2    50   ~ 0
b2
Text Label 10150 1200 2    50   ~ 0
b4
Text Label 10150 1100 2    50   ~ 0
b6
Text Label 11050 1500 0    50   ~ 0
b8
Text Label 10150 1500 2    50   ~ 0
b9
Text Label 10150 1600 2    50   ~ 0
b10
Entry Wire Line
	9850 1700 9950 1800
Entry Wire Line
	9850 1600 9950 1700
Entry Wire Line
	9850 1500 9950 1600
Entry Wire Line
	9850 1400 9950 1500
Entry Wire Line
	9850 1300 9950 1400
Entry Wire Line
	9850 1200 9950 1300
Entry Wire Line
	9850 1100 9950 1200
Entry Wire Line
	9850 1000 9950 1100
Wire Wire Line
	9750 1700 9850 1700
Wire Wire Line
	9750 1600 9850 1600
Wire Wire Line
	9750 1500 9850 1500
Wire Wire Line
	9750 1400 9850 1400
Wire Wire Line
	9750 1300 9850 1300
Wire Wire Line
	9750 1200 9850 1200
Wire Wire Line
	9750 1100 9850 1100
Wire Wire Line
	9750 1000 9850 1000
Text Label 9750 1000 0    50   ~ 0
b7
Text Label 9750 1300 0    50   ~ 0
b3
Text Label 9750 1400 0    50   ~ 0
b2
Text Label 9750 1200 0    50   ~ 0
b4
Text Label 9750 1100 0    50   ~ 0
b6
Text Label 9750 1700 0    50   ~ 0
b8
Text Label 9750 1500 0    50   ~ 0
b9
Text Label 9750 1600 0    50   ~ 0
b10
$Comp
L Device:Q_NPN_CBE Q2
U 1 1 60CE98A8
P 10650 1925
F 0 "Q2" H 10841 1879 50  0000 L CNN
F 1 "2n2222a" H 10841 1970 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92" H 10850 2025 50  0001 C CNN
F 3 "~" H 10650 1925 50  0001 C CNN
	1    10650 1925
	1    0    0    1   
$EndComp
Entry Wire Line
	8225 1925 8325 2025
Text Label 7875 1925 2    50   ~ 0
a1_5
$Comp
L Device:Q_NPN_CBE Q1
U 1 1 60B92168
P 7625 1925
F 0 "Q1" H 7816 1879 50  0000 L CNN
F 1 "2n2222a" H 7816 1970 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92" H 7825 2025 50  0001 C CNN
F 3 "~" H 7625 1925 50  0001 C CNN
	1    7625 1925
	-1   0    0    1   
$EndComp
Text Label 10400 1925 0    50   ~ 0
b1_5
Entry Wire Line
	9950 2025 10050 1925
Wire Wire Line
	7825 1925 8225 1925
Wire Wire Line
	10450 1925 10050 1925
Wire Wire Line
	8425 2250 8775 2250
Wire Wire Line
	9500 2250 9850 2250
Wire Wire Line
	9500 2350 9850 2350
Wire Wire Line
	9500 2450 9850 2450
Wire Wire Line
	9500 2550 9850 2550
Wire Wire Line
	9500 2650 9850 2650
Wire Wire Line
	9500 2750 9850 2750
Wire Wire Line
	9500 2850 9850 2850
Wire Wire Line
	9500 2950 9850 2950
Wire Wire Line
	8425 2350 8775 2350
Wire Wire Line
	8425 2450 8775 2450
Wire Wire Line
	8425 2550 8775 2550
Wire Wire Line
	8425 2650 8775 2650
Wire Wire Line
	8425 2750 8775 2750
Wire Wire Line
	8425 2850 8775 2850
Wire Wire Line
	8425 2950 8775 2950
Wire Wire Line
	9500 3050 9850 3050
Wire Wire Line
	8425 3050 8775 3050
Entry Wire Line
	8325 2950 8425 3050
Entry Wire Line
	9850 3050 9950 2950
Text Label 8775 3050 2    50   ~ 0
a1_5
Text Label 9500 3050 0    50   ~ 0
b1_5
Wire Wire Line
	7525 600  9150 600 
Wire Wire Line
	10750 575  9125 575 
$Comp
L pspice:0 #GND0103
U 1 1 60DC3790
P 6500 4700
F 0 "#GND0103" H 6500 4600 50  0001 C CNN
F 1 "0" H 6500 4550 50  0000 C CNN
F 2 "" H 6500 4700 50  0001 C CNN
F 3 "~" H 6500 4700 50  0001 C CNN
	1    6500 4700
	0    -1   -1   0   
$EndComp
$Comp
L pspice:0 #GND0102
U 1 1 60DBED57
P 5600 4700
F 0 "#GND0102" H 5600 4600 50  0001 C CNN
F 1 "0" H 5600 4600 50  0000 C CNN
F 2 "" H 5600 4700 50  0001 C CNN
F 3 "~" H 5600 4700 50  0001 C CNN
	1    5600 4700
	0    1    1    0   
$EndComp
Text GLabel 6950 3500 2    50   Input ~ 0
SCL
Text GLabel 7550 3800 2    50   Input ~ 0
SDA
$Comp
L neosensor_addon:ESP32_dev_30p U3
U 1 1 60A65F7C
P 6050 4150
F 0 "U3" H 6050 5125 50  0000 C CNN
F 1 "ESP32_dev_30p" H 6050 5034 50  0000 C CNN
F 2 "neosensor_addon:ESP32_dev_30p" V 6050 4200 50  0001 C CIN
F 3 "" H 6050 4550 50  0001 C CNN
	1    6050 4150
	1    0    0    -1  
$EndComp
Text GLabel 5600 4800 0    50   Input ~ 0
5v
Text GLabel 5600 3500 0    50   Input ~ 0
Push_1
Text GLabel 5600 3700 0    50   Input ~ 0
Push_3
Text GLabel 5600 3600 0    50   Input ~ 0
Push_2
Text GLabel 6500 4400 2    50   Input ~ 0
IR
Text GLabel 6500 3400 2    50   Input ~ 0
DI
Text GLabel 6500 4000 2    50   Input ~ 0
CI
$Comp
L Device:R R9
U 1 1 60E904A2
P 7300 3650
F 0 "R9" H 7230 3604 50  0000 R CNN
F 1 "4.7K" H 7230 3695 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 7230 3650 50  0001 C CNN
F 3 "~" H 7300 3650 50  0001 C CNN
	1    7300 3650
	-1   0    0    1   
$EndComp
$Comp
L Device:R R8
U 1 1 60E90D0E
P 6800 3350
F 0 "R8" H 6730 3304 50  0000 R CNN
F 1 "4.7K" H 6730 3395 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6730 3350 50  0001 C CNN
F 3 "~" H 6800 3350 50  0001 C CNN
	1    6800 3350
	-1   0    0    1   
$EndComp
Wire Wire Line
	6500 3500 6800 3500
Connection ~ 6800 3500
Wire Wire Line
	6800 3500 6950 3500
Wire Wire Line
	6500 3800 7300 3800
Connection ~ 7300 3800
Wire Wire Line
	7300 3800 7550 3800
Text GLabel 6800 3200 1    50   Input ~ 0
3v3
Text GLabel 7300 3500 1    50   Input ~ 0
3v3
NoConn ~ 5600 3400
NoConn ~ 5600 3800
NoConn ~ 5600 3900
NoConn ~ 5600 4000
NoConn ~ 5600 4100
NoConn ~ 5600 4200
NoConn ~ 5600 4300
NoConn ~ 5600 4400
NoConn ~ 5600 4500
NoConn ~ 5600 4600
NoConn ~ 6500 4600
NoConn ~ 6500 4500
NoConn ~ 6500 4800
NoConn ~ 6450 5750
NoConn ~ 6500 3900
NoConn ~ 6500 3700
NoConn ~ 6500 3600
$Comp
L Display_Character:SA15-11GWA U4
U 1 1 60BA85DE
P 7825 1300
F 0 "U4" H 7825 1967 50  0000 C CNN
F 1 "SA15-11GWA" H 7825 1876 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 7825 1250 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/SA15-11GWA.pdf" H 7825 1250 50  0001 C CNN
	1    7825 1300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8525 1500 8425 1500
Wire Wire Line
	8525 1700 8425 1700
Wire Wire Line
	8525 1000 8425 1000
Wire Wire Line
	8525 1100 8425 1100
Wire Wire Line
	8525 1200 8425 1200
Wire Wire Line
	8525 1300 8425 1300
Wire Wire Line
	8525 1400 8425 1400
Wire Wire Line
	8525 1600 8425 1600
$Comp
L Display_Character:SA15-11GWA U5
U 1 1 60BA473B
P 8825 1300
F 0 "U5" H 8825 1967 50  0000 C CNN
F 1 "SA15-11GWA" H 8825 1876 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 8825 1250 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/SA15-11GWA.pdf" H 8825 1250 50  0001 C CNN
	1    8825 1300
	1    0    0    -1  
$EndComp
Connection ~ 9125 1600
Wire Wire Line
	9125 575  9125 1600
Wire Wire Line
	9125 1600 9125 1700
Wire Wire Line
	7525 600  7525 1500
Wire Wire Line
	7525 1700 7525 1725
Wire Wire Line
	7525 1600 7525 1700
Connection ~ 7525 1600
Connection ~ 7525 1700
$Comp
L Display_Character:SA15-11GWA U8
U 1 1 60C590DD
P 10450 1300
F 0 "U8" H 10450 1967 50  0000 C CNN
F 1 "SA15-11GWA" H 10450 1876 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 10450 1250 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/SA15-11GWA.pdf" H 10450 1250 50  0001 C CNN
	1    10450 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10750 1700 10750 1725
Wire Wire Line
	10750 1600 10750 1700
Connection ~ 10750 1600
Connection ~ 10750 1700
$Comp
L Display_Character:SA15-11GWA U7
U 1 1 60C42FF4
P 9450 1300
F 0 "U7" H 9450 633 50  0000 C CNN
F 1 "SA15-11GWA" H 9450 724 50  0000 C CNN
F 2 "Display_7Segment:SA15-11xxx" H 9450 1250 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/SA15-11GWA.pdf" H 9450 1250 50  0001 C CNN
	1    9450 1300
	-1   0    0    -1  
$EndComp
Text GLabel 3450 5750 0    50   Input ~ 0
CI_LED
Text GLabel 3450 5650 0    50   Input ~ 0
DI_LED
Wire Wire Line
	4650 9375 5250 9375
Wire Wire Line
	4650 9475 5250 9475
$Comp
L Device:R R10
U 1 1 60C86E5A
P 6800 4200
F 0 "R10" V 6900 4125 50  0000 C CNN
F 1 "470" V 6900 4325 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6730 4200 50  0001 C CNN
F 3 "~" H 6800 4200 50  0001 C CNN
	1    6800 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	9150 600  9150 1600
Connection ~ 9150 1600
Wire Wire Line
	9150 1600 9150 1700
$Comp
L Device:LED D6
U 1 1 60CB96E0
P 10900 1500
F 0 "D6" H 10893 1245 50  0000 C CNN
F 1 "LED" H 10893 1336 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 10900 1500 50  0001 C CNN
F 3 "~" H 10900 1500 50  0001 C CNN
	1    10900 1500
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D2
U 1 1 60CBAB59
P 7375 1500
F 0 "D2" H 7368 1716 50  0000 C CNN
F 1 "LED" H 7368 1625 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 7375 1500 50  0001 C CNN
F 3 "~" H 7375 1500 50  0001 C CNN
	1    7375 1500
	1    0    0    -1  
$EndComp
Connection ~ 10750 1500
Wire Wire Line
	10750 1500 10750 1600
Connection ~ 7525 1500
Wire Wire Line
	7525 1500 7525 1600
Wire Wire Line
	10750 575  10750 1500
NoConn ~ 8125 1700
NoConn ~ 10150 1700
$Comp
L Device:R R11
U 1 1 60C98695
P 850 3750
F 0 "R11" V 643 3750 50  0000 C CNN
F 1 "470" V 734 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 780 3750 50  0001 C CNN
F 3 "~" H 850 3750 50  0001 C CNN
	1    850  3750
	0    1    1    0   
$EndComp
Text GLabel 6450 5650 2    50   Input ~ 0
BACK_LED
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 60D750A1
P 1900 5900
F 0 "Q?" V 2149 5900 50  0000 C CNN
F 1 "2N7000" V 2240 5900 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 2100 5825 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7000.pdf" H 1900 5900 50  0001 L CNN
	1    1900 5900
	0    1    1    0   
$EndComp
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 60D7AD06
P 1550 5550
F 0 "Q?" V 1799 5550 50  0000 C CNN
F 1 "2N7000" V 1890 5550 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 1750 5475 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7000.pdf" H 1550 5550 50  0001 L CNN
	1    1550 5550
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 60D87858
P 950 5400
F 0 "R?" H 1020 5446 50  0000 L CNN
F 1 "2k2" H 1020 5355 50  0000 L CNN
F 2 "" V 880 5400 50  0001 C CNN
F 3 "~" H 950 5400 50  0001 C CNN
	1    950  5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60D88785
P 1200 5400
F 0 "R?" H 1270 5446 50  0000 L CNN
F 1 "2k2" H 1270 5355 50  0000 L CNN
F 2 "" V 1130 5400 50  0001 C CNN
F 3 "~" H 1200 5400 50  0001 C CNN
	1    1200 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60D88BB5
P 2200 5400
F 0 "R?" H 2270 5446 50  0000 L CNN
F 1 "2k2" H 2270 5355 50  0000 L CNN
F 2 "" V 2130 5400 50  0001 C CNN
F 3 "~" H 2200 5400 50  0001 C CNN
	1    2200 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60D88F34
P 2450 5400
F 0 "R?" H 2520 5446 50  0000 L CNN
F 1 "2k2" H 2520 5355 50  0000 L CNN
F 2 "" V 2380 5400 50  0001 C CNN
F 3 "~" H 2450 5400 50  0001 C CNN
	1    2450 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  5550 950  5650
Wire Wire Line
	950  5650 1350 5650
Wire Wire Line
	1200 5550 1200 6000
Wire Wire Line
	1200 6000 1700 6000
Wire Wire Line
	950  5250 1200 5250
Wire Wire Line
	1550 5250 1550 5350
Connection ~ 1200 5250
Wire Wire Line
	1200 5250 1550 5250
Wire Wire Line
	1550 5250 1900 5250
Wire Wire Line
	1900 5250 1900 5700
Connection ~ 1550 5250
Wire Wire Line
	2450 5250 2200 5250
Wire Wire Line
	2100 6000 2200 6000
Wire Wire Line
	2200 5550 2200 6000
Wire Wire Line
	1750 5650 2450 5650
Wire Wire Line
	2450 5650 2450 5550
Wire Wire Line
	950  5250 800  5250
Connection ~ 950  5250
Wire Wire Line
	2450 5250 2700 5250
Connection ~ 2450 5250
Wire Wire Line
	950  5650 800  5650
Connection ~ 950  5650
Wire Wire Line
	1200 6000 800  6000
Connection ~ 1200 6000
Wire Wire Line
	2450 5650 2700 5650
Connection ~ 2450 5650
Wire Wire Line
	2200 6000 2700 6000
Connection ~ 2200 6000
Text GLabel 800  5250 0    50   Input ~ 0
3v3
Text GLabel 2700 5250 2    50   Input ~ 0
5v
Text GLabel 800  5650 0    50   Input ~ 0
DI
Text GLabel 800  6000 0    50   Input ~ 0
CI
Text GLabel 2700 5650 2    50   Input ~ 0
DI_LED
Text GLabel 2700 6000 2    50   Input ~ 0
CI_LED
Wire Wire Line
	4200 9075 5400 9075
Wire Wire Line
	4200 9675 5400 9675
Wire Wire Line
	4650 9175 6150 9175
Wire Wire Line
	4650 9775 6150 9775
Wire Wire Line
	3450 5450 3750 5450
Connection ~ 3750 5450
Wire Wire Line
	3750 5450 4350 5450
Connection ~ 4350 5450
Wire Wire Line
	4350 5450 4950 5450
Connection ~ 4950 5450
Wire Wire Line
	4950 5450 5550 5450
Connection ~ 5550 5450
Wire Wire Line
	5550 5450 6150 5450
Wire Wire Line
	3450 6050 3750 6050
Connection ~ 3750 6050
Wire Wire Line
	3750 6050 4350 6050
Connection ~ 4350 6050
Wire Wire Line
	4350 6050 4950 6050
Connection ~ 4950 6050
Wire Wire Line
	4950 6050 5550 6050
Connection ~ 5550 6050
Wire Wire Line
	5550 6050 6150 6050
$Comp
L Connector_Generic:Conn_01x05 J?
U 1 1 610998F9
P 5200 1100
F 0 "J?" H 5280 1142 50  0000 L CNN
F 1 "Conn_01x05" H 5280 1051 50  0000 L CNN
F 2 "" H 5200 1100 50  0001 C CNN
F 3 "~" H 5200 1100 50  0001 C CNN
	1    5200 1100
	1    0    0    -1  
$EndComp
Text GLabel 5000 900  0    50   Input ~ 0
5v
$Comp
L pspice:0 #GND?
U 1 1 6109C13E
P 5000 1000
F 0 "#GND?" H 5000 900 50  0001 C CNN
F 1 "0" V 5000 850 50  0000 L CNN
F 2 "" H 5000 1000 50  0001 C CNN
F 3 "~" H 5000 1000 50  0001 C CNN
	1    5000 1000
	0    1    1    0   
$EndComp
Text GLabel 5000 1200 0    50   Input ~ 0
TX
Text GLabel 5000 1300 0    50   Input ~ 0
RX
Text GLabel 5000 1100 0    50   Input ~ 0
PM_EN
Text GLabel 6500 4100 2    50   Input ~ 0
PM_EN
Wire Wire Line
	6500 4200 6650 4200
Text GLabel 6950 4200 2    50   Input ~ 0
TX
Text GLabel 6500 4300 2    50   Input ~ 0
RX
Wire Bus Line
	9950 1100 9950 2950
Wire Bus Line
	8325 1100 8325 2950
$EndSCHEMATC
