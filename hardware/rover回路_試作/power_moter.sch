EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 7
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
	3300 2450 4100 2450
Connection ~ 3300 2450
Wire Wire Line
	3300 2150 3300 2450
Connection ~ 3300 1400
Wire Wire Line
	3300 1850 3300 1400
$Comp
L Device:C C_raspberryPi_powerStable
U 1 1 615D8405
P 3300 2000
F 0 "C_raspberryPi_powerStable" V 3415 2046 50  0000 L CNN
F 1 "C" H 3415 1955 50  0000 L CNN
F 2 "" H 3338 1850 50  0001 C CNN
F 3 "~" H 3300 2000 50  0001 C CNN
	1    3300 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 2450 1800 2050
Wire Wire Line
	4100 2300 4100 2450
Wire Wire Line
	4400 1400 4400 1700
Wire Wire Line
	1800 1950 1800 1400
$Comp
L Connector:USB_A J_RaspberryPi_power
U 1 1 615D2795
P 4100 1900
F 0 "J_RaspberryPi_power" H 4157 2367 50  0000 C CNN
F 1 "USB_A" H 4157 2276 50  0000 C CNN
F 2 "" H 4250 1850 50  0001 C CNN
F 3 " ~" H 4250 1850 50  0001 C CNN
	1    4100 1900
	1    0    0    -1  
$EndComp
Text HLabel 1800 1950 0    50   Input ~ 0
converter_after+
Text HLabel 1800 2050 0    50   Input ~ 0
converter_after-
Text HLabel 7400 1700 1    50   Input ~ 0
moterPowerContolloer-
Text HLabel 7300 1700 1    50   Input ~ 0
moterPowerContolloer+
Connection ~ 8100 2150
Wire Wire Line
	8150 2150 8100 2150
Wire Wire Line
	8150 2700 8150 2150
Wire Wire Line
	8100 2700 8150 2700
Wire Wire Line
	8200 2500 8100 2500
Wire Wire Line
	8100 1700 8100 2150
Wire Wire Line
	7400 1700 8100 1700
Connection ~ 7300 2150
Wire Wire Line
	7300 2150 7300 1700
Wire Wire Line
	7300 2150 7600 2150
Wire Wire Line
	7300 2700 7300 2150
Wire Wire Line
	7500 2700 7300 2700
Wire Wire Line
	8100 2150 8000 2150
$Comp
L pspice:DIODE D_surge_voltage_control
U 1 1 61585B5D
P 7800 2150
F 0 "D_surge_voltage_control" H 7846 1943 50  0000 R CNN
F 1 "DIODE" H 7755 2022 50  0000 R CNN
F 2 "" H 7800 2150 50  0001 C CNN
F 3 "~" H 7800 2150 50  0001 C CNN
	1    7800 2150
	-1   0    0    1   
$EndComp
Wire Wire Line
	8200 2850 8350 2850
Wire Wire Line
	8200 2850 8200 2500
Wire Wire Line
	6100 3700 6100 3300
Text HLabel 6700 3550 2    50   Input ~ 0
battery2+
Text HLabel 6100 3800 0    50   Output ~ 0
converter_before-
Text HLabel 6100 3700 0    50   Output ~ 0
converter_before+
Wire Wire Line
	7250 3800 7250 3950
Connection ~ 6100 3300
Wire Wire Line
	6100 3950 6100 3800
$Comp
L Connector:Conn_01x02_Female J_checker1
U 1 1 615490F8
P 7050 3700
F 0 "J_checker1" V 6805 3748 50  0000 L CNN
F 1 "Conn_01x02_Female" V 6896 3748 50  0000 L CNN
F 2 "" H 7050 3700 50  0001 C CNN
F 3 "~" H 7050 3700 50  0001 C CNN
F 4 "?" H 7078 3669 50  0001 L BNN "Field4"
F 5 "?" H 7078 3584 50  0001 L BNN "Field5"
	1    7050 3700
	-1   0    0    -1  
$EndComp
Connection ~ 6300 2900
Wire Wire Line
	6100 2900 6300 2900
Wire Wire Line
	6100 3050 6100 2900
Wire Wire Line
	6100 3150 6100 3300
$Comp
L Connector:Conn_01x02_Female J_subConverterSw
U 1 1 61548187
P 5900 3050
F 0 "J_subConverterSw" H 5928 3026 50  0000 L CNN
F 1 "Conn_01x02_Female" H 5928 2935 50  0000 L CNN
F 2 "" H 5900 3050 50  0001 C CNN
F 3 "~" H 5900 3050 50  0001 C CNN
	1    5900 3050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6200 3300 6100 3300
Wire Wire Line
	6300 2900 6300 2850
$Comp
L Switch:SW_Push_SPDT SW_Converter
U 1 1 6158068D
P 6300 3100
F 0 "SW_Converter" H 6300 3385 50  0000 C CNN
F 1 "SW_Push_SPDT" H 6300 3294 50  0000 C CNN
F 2 "" H 6300 3100 50  0001 C CNN
F 3 "~" H 6300 3100 50  0001 C CNN
	1    6300 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	8550 2850 8550 3850
Wire Wire Line
	3300 1400 4400 1400
Wire Wire Line
	8550 3850 9800 3850
Entry Wire Line
	9800 4300 9900 4400
Entry Wire Line
	9800 4100 9900 4200
Entry Wire Line
	9800 3950 9900 4050
Entry Wire Line
	9800 3850 9900 3950
Entry Wire Line
	9800 3750 9900 3850
Entry Wire Line
	9800 3500 9900 3600
Wire Bus Line
	9900 4450 10300 4450
Text HLabel 6650 3550 0    50   Input ~ 0
battery2-
Text HLabel 6700 3450 2    50   Input ~ 0
battery1+
Text HLabel 6650 3450 0    50   Input ~ 0
battery1-
$Comp
L Connector:Conn_01x02_Female J_checker2
U 1 1 616DF91B
P 2850 1950
F 0 "J_checker2" V 2605 1998 50  0000 L CNN
F 1 "Conn_01x02_Female" V 2696 1998 50  0000 L CNN
F 2 "" H 2850 1950 50  0001 C CNN
F 3 "~" H 2850 1950 50  0001 C CNN
F 4 "?" H 2878 1919 50  0001 L BNN "Field4"
F 5 "?" H 2878 1834 50  0001 L BNN "Field5"
	1    2850 1950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3050 2050 3050 2450
Wire Wire Line
	3050 1400 3050 1950
$Comp
L Connector:Conn_01x04_Female J_checker1_I2C
U 1 1 616EAB64
P 2500 3450
F 0 "J_checker1_I2C" H 2528 3426 50  0000 L CNN
F 1 "Conn_01x04_Female" H 2528 3335 50  0000 L CNN
F 2 "" H 2500 3450 50  0001 C CNN
F 3 "~" H 2500 3450 50  0001 C CNN
	1    2500 3450
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Female J_checker2_I2C
U 1 1 616F6DBD
P 2500 3950
F 0 "J_checker2_I2C" H 2528 3926 50  0000 L CNN
F 1 "Conn_01x04_Female" H 2528 3835 50  0000 L CNN
F 2 "" H 2500 3950 50  0001 C CNN
F 3 "~" H 2500 3950 50  0001 C CNN
	1    2500 3950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2700 3350 2800 3350
Wire Wire Line
	2800 3350 2800 3850
Wire Wire Line
	2800 3850 2700 3850
Wire Wire Line
	2700 3450 2850 3450
Wire Wire Line
	2850 3450 2850 3950
Wire Wire Line
	2850 3950 2700 3950
Wire Wire Line
	2700 3550 2900 3550
Wire Wire Line
	2900 4050 2700 4050
Wire Wire Line
	2950 3650 2950 4150
Wire Wire Line
	2950 4150 2700 4150
Wire Wire Line
	2700 3650 2950 3650
Wire Wire Line
	2900 3550 2900 4050
Wire Wire Line
	2950 3650 3050 3650
Connection ~ 2950 3650
Wire Wire Line
	2900 3550 3050 3550
Connection ~ 2900 3550
Wire Wire Line
	2850 3450 3050 3450
Connection ~ 2850 3450
Wire Wire Line
	2800 3350 3050 3350
Connection ~ 2800 3350
Text Label 3050 3350 2    50   ~ 0
5V
Text Label 3050 3450 2    50   ~ 0
SDA
Text Label 3050 3550 2    50   ~ 0
SCL
Text Label 3050 3650 2    50   ~ 0
GND
Entry Wire Line
	3050 3350 3150 3450
Entry Wire Line
	3050 3450 3150 3550
Entry Wire Line
	3050 3550 3150 3650
Entry Wire Line
	3050 3650 3150 3750
Wire Bus Line
	3150 3750 3300 3750
Text HLabel 3300 3750 2    50   BiDi ~ 0
I2C
$Comp
L pspice:0 #GND?
U 1 1 61724C76
P 6100 4000
F 0 "#GND?" H 6100 3900 50  0001 C CNN
F 1 "0" H 6100 4089 50  0000 C CNN
F 2 "" H 6100 4000 50  0001 C CNN
F 3 "~" H 6100 4000 50  0001 C CNN
	1    6100 4000
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND?
U 1 1 61725403
P 1800 2500
F 0 "#GND?" H 1800 2400 50  0001 C CNN
F 1 "0" H 1800 2589 50  0000 C CNN
F 2 "" H 1800 2500 50  0001 C CNN
F 3 "~" H 1800 2500 50  0001 C CNN
	1    1800 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 2500 1800 2450
Connection ~ 1800 2450
Wire Wire Line
	6100 4000 6100 3950
Connection ~ 6100 3950
$Comp
L power:+5V #PWR?
U 1 1 6172ABE2
P 1800 1350
F 0 "#PWR?" H 1800 1200 50  0001 C CNN
F 1 "+5V" H 1815 1523 50  0000 C CNN
F 2 "" H 1800 1350 50  0001 C CNN
F 3 "" H 1800 1350 50  0001 C CNN
	1    1800 1350
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 6172C3DB
P 6100 2750
F 0 "#PWR?" H 6100 2600 50  0001 C CNN
F 1 "+12V" H 6115 2923 50  0000 C CNN
F 2 "" H 6100 2750 50  0001 C CNN
F 3 "" H 6100 2750 50  0001 C CNN
	1    6100 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2750 6100 2900
Connection ~ 6100 2900
Wire Wire Line
	1800 1400 1800 1350
Connection ~ 1800 1400
Wire Wire Line
	7250 2850 7250 3700
Wire Wire Line
	6100 3950 6650 3950
Wire Wire Line
	6650 3450 6650 3950
Wire Wire Line
	6700 3550 6700 2850
Wire Wire Line
	6300 2850 6700 2850
Wire Wire Line
	7750 3550 7750 3400
Wire Wire Line
	7750 3650 7750 3800
$Comp
L Connector:Conn_01x02_Female J_subDriverSw?
U 1 1 6176C926
P 7550 3550
F 0 "J_subDriverSw?" V 7578 3526 50  0000 L CNN
F 1 "Conn_01x02_Female" V 7651 3435 50  0000 L CNN
F 2 "" H 7550 3550 50  0001 C CNN
F 3 "~" H 7550 3550 50  0001 C CNN
	1    7550 3550
	-1   0    0    -1  
$EndComp
$Comp
L Switch:SW_Push_SPDT SW_Driver
U 1 1 6176C92C
P 7950 3600
F 0 "SW_Driver" H 7950 3885 50  0000 C CNN
F 1 "SW_Push_SPDT" H 7950 3794 50  0000 C CNN
F 2 "" H 7950 3600 50  0001 C CNN
F 3 "~" H 7950 3600 50  0001 C CNN
	1    7950 3600
	0    1    1    0   
$EndComp
$Comp
L My_Device:946H-1C-3D RL?
U 1 1 61557469
P 8000 2400
F 0 "RL?" H 8200 3075 50  0000 C CNN
F 1 "946H-1C-3D" H 8200 2984 50  0000 C CNN
F 2 "" H 8650 2950 50  0001 C CNN
F 3 "" H 8650 2950 50  0001 C CNN
	1    8000 2400
	-1   0    0    1   
$EndComp
Wire Wire Line
	7750 3400 7950 3400
Wire Wire Line
	7850 3800 7750 3800
Connection ~ 6650 3950
Connection ~ 6700 2850
Wire Wire Line
	6650 3950 7250 3950
Wire Wire Line
	6700 2850 7250 2850
Wire Wire Line
	7750 3800 7750 3900
Wire Wire Line
	7750 3900 8350 3900
Wire Wire Line
	8350 3900 8350 2850
Connection ~ 7750 3800
Connection ~ 8350 2850
Wire Wire Line
	8350 2850 8550 2850
Wire Wire Line
	7750 3400 7400 3400
Wire Wire Line
	7400 3400 7400 2850
Connection ~ 7750 3400
Wire Wire Line
	7400 2850 7500 2850
Wire Wire Line
	7400 2850 7250 2850
Connection ~ 7400 2850
Connection ~ 7250 2850
Wire Wire Line
	7250 3950 9800 3950
Connection ~ 7250 3950
Connection ~ 3050 1400
Wire Wire Line
	3050 1400 3300 1400
Connection ~ 3050 2450
Wire Wire Line
	3050 2450 3300 2450
Wire Wire Line
	1800 1400 3050 1400
Wire Wire Line
	1800 2450 3050 2450
Wire Wire Line
	2800 2450 2800 2050
Wire Wire Line
	2800 1950 2800 1400
Text HLabel 2800 1950 0    50   BiDi ~ 0
pico_+5V
Text HLabel 2800 2050 0    50   BiDi ~ 0
pico_GND
Text HLabel 10300 4450 2    50   BiDi ~ 0
moter
Text HLabel 5150 1950 0    50   Output ~ 0
USB_POWER_OUT
$Comp
L Connector:Conn_01x02_Female J_+left_moter
U 1 1 61569E17
P 9450 3450
F 0 "J_+left_moter" H 9478 3426 50  0000 L CNN
F 1 "Conn_01x02_Female" H 9478 3335 50  0000 L CNN
F 2 "" H 9450 3450 50  0001 C CNN
F 3 "~" H 9450 3450 50  0001 C CNN
	1    9450 3450
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J_-left_moter
U 1 1 6156D528
P 9450 3650
F 0 "J_-left_moter" H 9478 3626 50  0000 L CNN
F 1 "Conn_01x02_Female" H 9478 3535 50  0000 L CNN
F 2 "" H 9450 3650 50  0001 C CNN
F 3 "~" H 9450 3650 50  0001 C CNN
	1    9450 3650
	-1   0    0    -1  
$EndComp
Connection ~ 9650 3650
Connection ~ 9650 3450
Wire Wire Line
	9650 3450 9800 3450
Wire Wire Line
	9650 3650 9800 3650
Wire Wire Line
	9650 3750 9650 3650
Wire Wire Line
	9650 3550 9650 3450
Wire Wire Line
	9800 3450 9800 3500
Wire Wire Line
	9800 3650 9800 3750
$Comp
L Connector:Conn_01x02_Female J_+right_moter
U 1 1 6156A6CC
P 9450 4050
F 0 "J_+right_moter" H 9478 4026 50  0000 L CNN
F 1 "Conn_01x02_Female" H 9478 3935 50  0000 L CNN
F 2 "" H 9450 4050 50  0001 C CNN
F 3 "~" H 9450 4050 50  0001 C CNN
	1    9450 4050
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J_-right_moter
U 1 1 6156ABCF
P 9450 4300
F 0 "J_-right_moter" H 9478 4276 50  0000 L CNN
F 1 "Conn_01x02_Female" H 9478 4185 50  0000 L CNN
F 2 "" H 9450 4300 50  0001 C CNN
F 3 "~" H 9450 4300 50  0001 C CNN
	1    9450 4300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9650 4150 9650 4050
Wire Wire Line
	9650 4300 9650 4400
Connection ~ 9650 4400
Connection ~ 9650 4150
Wire Wire Line
	9800 4400 9650 4400
Wire Wire Line
	9650 4150 9800 4150
Wire Wire Line
	9800 4100 9800 4150
Wire Wire Line
	9800 4300 9800 4400
Wire Bus Line
	3150 3450 3150 3750
Wire Bus Line
	9900 3600 9900 4450
$EndSCHEMATC
