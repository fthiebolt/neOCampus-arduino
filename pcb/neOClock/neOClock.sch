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
P 1300 3700
F 0 "IR1" V 1346 3522 50  0000 R CNN
F 1 "IRsensor" V 1255 3522 50  0000 R CNN
F 2 "neosensor_addon:IRsensor_TH_and_smd" H 1300 3700 50  0001 C CIN
F 3 "" H 1300 3700 50  0001 C CNN
	1    1300 3700
	-1   0    0    1   
$EndComp
$Comp
L pspice:0 #GND0111
U 1 1 60A9911E
P 1550 3600
F 0 "#GND0111" H 1550 3500 50  0001 C CNN
F 1 "0" H 1550 3500 50  0000 C CNN
F 2 "" H 1550 3600 50  0001 C CNN
F 3 "~" H 1550 3600 50  0001 C CNN
	1    1550 3600
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
Text GLabel 1550 3800 2    50   Input ~ 0
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
Text GLabel 850  3700 0    50   Input ~ 0
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
$Comp
L Display_Character:HDSP-7801 U4
U 1 1 60FD84CF
P 7825 1300
F 0 "U4" H 7825 1967 50  0000 C CNN
F 1 "HDSP-7801" H 7825 1876 50  0000 C CNN
F 2 "Display_7Segment:SBC18-11SURKCGKWA" H 7825 750 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-2553EN" H 7825 1300 50  0001 C CNN
	1    7825 1300
	-1   0    0    -1  
$EndComp
$Comp
L Display_Character:HDSP-7801 U5
U 1 1 60EA8860
P 8825 1300
F 0 "U5" H 8825 1967 50  0000 C CNN
F 1 "HDSP-7801" H 8825 1876 50  0000 C CNN
F 2 "Display_7Segment:SBC18-11SURKCGKWA" H 8825 750 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-2553EN" H 8825 1300 50  0001 C CNN
	1    8825 1300
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
L LED:APA102 D2
U 1 1 61274911
P 1150 5900
F 0 "D2" H 1150 6381 50  0000 C CNN
F 1 "APA102" H 1150 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 1200 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 1250 5525 50  0001 L TNN
	1    1150 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D3
U 1 1 612815DC
P 1750 5900
F 0 "D3" H 1750 6381 50  0000 C CNN
F 1 "APA102" H 1750 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 1800 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 1850 5525 50  0001 L TNN
	1    1750 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D4
U 1 1 6128410F
P 2350 5900
F 0 "D4" H 2350 6381 50  0000 C CNN
F 1 "APA102" H 2350 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 2400 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 2450 5525 50  0001 L TNN
	1    2350 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D5
U 1 1 612CF7A7
P 2950 5900
F 0 "D5" H 2950 6381 50  0000 C CNN
F 1 "APA102" H 2950 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 3000 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 3050 5525 50  0001 L TNN
	1    2950 5900
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0118
U 1 1 61413F41
P 850 6200
F 0 "#GND0118" H 850 6100 50  0001 C CNN
F 1 "0" V 850 6050 50  0000 L CNN
F 2 "" H 850 6200 50  0001 C CNN
F 3 "~" H 850 6200 50  0001 C CNN
	1    850  6200
	0    1    1    0   
$EndComp
Text GLabel 850  5600 0    50   Input ~ 0
5v
Text GLabel 850  5800 0    50   Input ~ 0
DI
Text GLabel 850  5900 0    50   Input ~ 0
CI
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
P 3550 5900
F 0 "D8" H 3550 6381 50  0000 C CNN
F 1 "APA102" H 3550 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 3600 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 3650 5525 50  0001 L TNN
	1    3550 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D9
U 1 1 60B650FD
P 4150 5900
F 0 "D9" H 4150 6381 50  0000 C CNN
F 1 "APA102" H 4150 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 4200 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 4250 5525 50  0001 L TNN
	1    4150 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D10
U 1 1 60B65103
P 4750 5900
F 0 "D10" H 4750 6381 50  0000 C CNN
F 1 "APA102" H 4750 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 4800 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 4850 5525 50  0001 L TNN
	1    4750 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D11
U 1 1 60B65109
P 5350 5900
F 0 "D11" H 5350 6381 50  0000 C CNN
F 1 "APA102" H 5350 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 5400 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 5450 5525 50  0001 L TNN
	1    5350 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D12
U 1 1 60B6B69E
P 5950 5900
F 0 "D12" H 5950 6381 50  0000 C CNN
F 1 "APA102" H 5950 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 6000 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 6050 5525 50  0001 L TNN
	1    5950 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D13
U 1 1 60B6B6A4
P 6550 5900
F 0 "D13" H 6550 6381 50  0000 C CNN
F 1 "APA102" H 6550 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 6600 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 6650 5525 50  0001 L TNN
	1    6550 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D14
U 1 1 60B6B6AA
P 7150 5900
F 0 "D14" H 7150 6381 50  0000 C CNN
F 1 "APA102" H 7150 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 7200 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 7250 5525 50  0001 L TNN
	1    7150 5900
	1    0    0    -1  
$EndComp
$Comp
L LED:APA102 D15
U 1 1 60B6B6B0
P 7750 5900
F 0 "D15" H 7750 6381 50  0000 C CNN
F 1 "APA102" H 7750 6290 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 7800 5600 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 7850 5525 50  0001 L TNN
	1    7750 5900
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
NoConn ~ 8050 5800
NoConn ~ 8050 5900
Wire Wire Line
	850  5600 1150 5600
Connection ~ 1150 5600
Wire Wire Line
	1150 5600 1750 5600
Connection ~ 1750 5600
Wire Wire Line
	1750 5600 2350 5600
Connection ~ 2350 5600
Wire Wire Line
	2350 5600 2950 5600
Connection ~ 2950 5600
Wire Wire Line
	2950 5600 3550 5600
Connection ~ 3550 5600
Wire Wire Line
	3550 5600 4150 5600
Connection ~ 4150 5600
Wire Wire Line
	4150 5600 4750 5600
Connection ~ 4750 5600
Wire Wire Line
	4750 5600 5350 5600
Connection ~ 5350 5600
Wire Wire Line
	5350 5600 5950 5600
Connection ~ 5950 5600
Wire Wire Line
	5950 5600 6550 5600
Connection ~ 6550 5600
Wire Wire Line
	6550 5600 7150 5600
Connection ~ 7150 5600
Wire Wire Line
	7150 5600 7750 5600
Wire Wire Line
	850  6200 1150 6200
Connection ~ 1150 6200
Wire Wire Line
	1150 6200 1750 6200
Connection ~ 1750 6200
Wire Wire Line
	1750 6200 2350 6200
Connection ~ 2350 6200
Wire Wire Line
	2350 6200 2950 6200
Connection ~ 2950 6200
Wire Wire Line
	2950 6200 3550 6200
Connection ~ 3550 6200
Wire Wire Line
	3550 6200 4150 6200
Connection ~ 4150 6200
Wire Wire Line
	4150 6200 4750 6200
Connection ~ 4750 6200
Wire Wire Line
	4750 6200 5350 6200
Connection ~ 5350 6200
Wire Wire Line
	5350 6200 5950 6200
Connection ~ 5950 6200
Wire Wire Line
	5950 6200 6550 6200
Connection ~ 6550 6200
Wire Wire Line
	6550 6200 7150 6200
Connection ~ 7150 6200
Wire Wire Line
	7150 6200 7750 6200
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
P 950 3950
F 0 "R1" H 1020 3996 50  0000 L CNN
F 1 "15k" H 1020 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 880 3950 50  0001 C CNN
F 3 "~" H 950 3950 50  0001 C CNN
	1    950  3950
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND0125
U 1 1 61384771
P 950 4100
F 0 "#GND0125" H 950 4000 50  0001 C CNN
F 1 "0" H 950 3950 50  0000 C CNN
F 2 "" H 950 4100 50  0001 C CNN
F 3 "~" H 950 4100 50  0001 C CNN
	1    950  4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  3700 950  3700
Wire Wire Line
	950  3700 950  3800
Connection ~ 950  3700
Wire Wire Line
	950  3700 1050 3700
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
Text Label 8125 1000 0    50   ~ 0
a10
Text Label 8125 1100 0    50   ~ 0
a9
Text Label 8125 1200 0    50   ~ 0
a8
Text Label 8125 1300 0    50   ~ 0
a5
Text Label 8125 1400 0    50   ~ 0
a4
Text Label 8125 1500 0    50   ~ 0
a2
Text Label 8125 1600 0    50   ~ 0
a3
Text Label 8125 1700 0    50   ~ 0
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
Wire Wire Line
	8125 1700 8225 1700
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
Entry Wire Line
	8225 1700 8325 1800
Text Label 8525 1000 2    50   ~ 0
a10
Text Label 8525 1100 2    50   ~ 0
a9
Text Label 8525 1200 2    50   ~ 0
a8
Text Label 8525 1300 2    50   ~ 0
a5
Text Label 8525 1400 2    50   ~ 0
a4
Text Label 8525 1500 2    50   ~ 0
a2
Text Label 8525 1600 2    50   ~ 0
a3
Text Label 8525 1700 2    50   ~ 0
a7
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
	8525 1500 8425 1500
Wire Wire Line
	8525 1600 8425 1600
Wire Wire Line
	8525 1700 8425 1700
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
Text Label 8525 2250 2    50   ~ 0
a7
Text Label 8525 2350 2    50   ~ 0
a3
Text Label 8525 2450 2    50   ~ 0
a2
Text Label 8525 2550 2    50   ~ 0
a4
Text Label 8525 2650 2    50   ~ 0
a5
Text Label 8525 2750 2    50   ~ 0
a8
Text Label 8525 2850 2    50   ~ 0
a9
Text Label 8525 2950 2    50   ~ 0
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
Text Label 9750 2950 0    50   ~ 0
b10
Text Label 9750 2850 0    50   ~ 0
b9
Text Label 9750 2750 0    50   ~ 0
b8
Text Label 9750 2650 0    50   ~ 0
b5
Text Label 9750 2550 0    50   ~ 0
b4
Text Label 9750 2450 0    50   ~ 0
b2
Text Label 9750 2350 0    50   ~ 0
b3
Text Label 9750 2250 0    50   ~ 0
b7
Entry Wire Line
	10050 1700 9950 1800
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
	10150 1700 10050 1700
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
Text Label 10150 1700 2    50   ~ 0
b7
Text Label 10150 1600 2    50   ~ 0
b3
Text Label 10150 1500 2    50   ~ 0
b2
Text Label 10150 1400 2    50   ~ 0
b4
Text Label 10150 1300 2    50   ~ 0
b5
Text Label 10150 1200 2    50   ~ 0
b8
Text Label 10150 1100 2    50   ~ 0
b9
Text Label 10150 1000 2    50   ~ 0
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
Text Label 9750 1700 0    50   ~ 0
b7
Text Label 9750 1600 0    50   ~ 0
b3
Text Label 9750 1500 0    50   ~ 0
b2
Text Label 9750 1400 0    50   ~ 0
b4
Text Label 9750 1300 0    50   ~ 0
b5
Text Label 9750 1200 0    50   ~ 0
b8
Text Label 9750 1100 0    50   ~ 0
b9
Text Label 9750 1000 0    50   ~ 0
b10
$Comp
L Display_Character:HDSP-7801 U8
U 1 1 60E5B8EC
P 10450 1300
F 0 "U8" H 10450 1967 50  0000 C CNN
F 1 "HDSP-7801" H 10450 1876 50  0000 C CNN
F 2 "Display_7Segment:SBC18-11SURKCGKWA" H 10450 750 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-2553EN" H 10450 1300 50  0001 C CNN
	1    10450 1300
	1    0    0    -1  
$EndComp
$Comp
L Display_Character:HDSP-7801 U7
U 1 1 60E5B8E6
P 9450 1300
F 0 "U7" H 9450 1967 50  0000 C CNN
F 1 "HDSP-7801" H 9450 1876 50  0000 C CNN
F 2 "Display_7Segment:SBC18-11SURKCGKWA" H 9450 750 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-2553EN" H 9450 1300 50  0001 C CNN
	1    9450 1300
	-1   0    0    -1  
$EndComp
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
Wire Wire Line
	7525 1600 7525 1700
Wire Wire Line
	10750 1600 10750 1700
Wire Wire Line
	9150 1600 9150 1700
Wire Wire Line
	9125 1600 9125 1700
Entry Wire Line
	8225 1925 8325 2025
Text Label 7825 1925 2    50   ~ 0
a1
$Comp
L Device:Q_NPN_CBE Q1
U 1 1 60B92168
P 7625 1925
F 0 "Q1" H 7816 1879 50  0000 L CNN
F 1 "2n2222a" H 7816 1970 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SC-59_Handsoldering" H 7825 2025 50  0001 C CNN
F 3 "~" H 7625 1925 50  0001 C CNN
	1    7625 1925
	-1   0    0    1   
$EndComp
Wire Wire Line
	10750 1700 10750 1725
Connection ~ 10750 1700
Wire Wire Line
	7525 1700 7525 1725
Connection ~ 7525 1700
Text Label 10450 1925 0    50   ~ 0
b1
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
Text Label 8525 3050 2    50   ~ 0
a1
Text Label 9750 3050 0    50   ~ 0
b1
Wire Wire Line
	7525 1600 7525 600 
Wire Wire Line
	7525 600  9150 600 
Wire Wire Line
	9150 600  9150 1600
Connection ~ 7525 1600
Connection ~ 9150 1600
Wire Wire Line
	10750 1600 10750 575 
Wire Wire Line
	10750 575  9125 575 
Wire Wire Line
	9125 575  9125 1600
Connection ~ 10750 1600
Connection ~ 9125 1600
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
NoConn ~ 6500 4300
NoConn ~ 6500 4200
NoConn ~ 6500 4100
NoConn ~ 6500 3900
NoConn ~ 6500 3700
NoConn ~ 6500 3600
Wire Bus Line
	8325 1100 8325 2950
Wire Bus Line
	9950 1100 9950 2950
$EndSCHEMATC
