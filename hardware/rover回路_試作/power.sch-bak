EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 5750 3400 950  400 
U 617DDA35
F0 "picoTof" 50
F1 "picoTof.sch" 50
F2 "PICO_QUICK" B R 6700 3550 50 
$EndSheet
Wire Wire Line
	3050 3050 2400 3050
Wire Wire Line
	2400 3050 2400 2950
Wire Wire Line
	2450 3250 2450 3150
Wire Wire Line
	2450 3150 3050 3150
Wire Wire Line
	3050 3500 2550 3500
Wire Wire Line
	2550 3500 2550 4150
Wire Wire Line
	2550 4150 2250 4150
Wire Wire Line
	2250 4250 2600 4250
Wire Wire Line
	2600 4250 2600 3600
Wire Wire Line
	2600 3600 3050 3600
$Sheet
S 1500 4050 750  650 
U 61649E63
F0 "converter" 50
F1 "converter.sch" 50
F2 "in+" I R 2250 4150 50 
F3 "in-" I R 2250 4250 50 
F4 "out+" O R 2250 4400 50 
F5 "out-" O R 2250 4500 50 
F6 "out2+" O L 1500 4400 50 
F7 "out2-" O L 1500 4500 50 
$EndSheet
Wire Wire Line
	6900 3000 6800 3000
Wire Wire Line
	6800 3000 6800 2800
Wire Wire Line
	6800 2800 6350 2800
$Sheet
S 5750 4200 1050 300 
U 616868C8
F0 "moterDriver" 50
F1 "moterDriver.sch" 50
F2 "moter" I L 5750 4300 50 
F3 "DriverControl" I R 6800 4300 50 
$EndSheet
Wire Wire Line
	6800 4300 8100 4300
Wire Wire Line
	8100 4300 8100 2750
Wire Wire Line
	8100 2750 6350 2750
Wire Wire Line
	2400 2950 1800 2950
Wire Wire Line
	1800 3250 2450 3250
$Comp
L Device:Battery_Cell BT?
U 1 1 61647480
P 1800 3150
F 0 "BT?" H 1918 3246 50  0000 L CNN
F 1 "Battery_Cell" H 1918 3155 50  0000 L CNN
F 2 "" V 1800 3210 50  0001 C CNN
F 3 "~" V 1800 3210 50  0001 C CNN
	1    1800 3150
	1    0    0    -1  
$EndComp
$Sheet
S 3050 2900 1600 1350
U 6160D455
F0 "power_control" 50
F1 "power_control.sch" 50
F2 "converter_before+" O L 3050 3500 50 
F3 "converter_before-" O L 3050 3600 50 
F4 "battery1+" I L 3050 3050 50 
F5 "battery1-" I L 3050 3150 50 
F6 "moter" B R 4650 3900 50 
$EndSheet
Wire Wire Line
	2250 4400 2950 4400
Wire Bus Line
	4650 3900 5350 3900
Wire Bus Line
	5350 3900 5350 4300
Wire Bus Line
	5350 4300 5750 4300
Entry Wire Line
	1350 4400 1450 4500
Entry Wire Line
	1350 4300 1450 4400
Wire Wire Line
	1450 4400 1500 4400
Wire Wire Line
	1450 4500 1500 4500
Wire Bus Line
	1350 4250 1050 4250
Text Label 1150 4250 0    50   ~ 0
TypeC
Wire Bus Line
	1050 4250 1050 2300
Wire Bus Line
	1050 2300 6500 2300
Wire Bus Line
	6500 2300 6500 2650
Wire Bus Line
	6350 2650 6500 2650
$Sheet
S 5300 2550 1050 550 
U 6164CA09
F0 "raspberryPi" 50
F1 "raspberryPi.sch" 50
F2 "I2C" B L 5300 3000 50 
F3 "I2C_2" B R 6350 2800 50 
F4 "I2C_3" B R 6350 2900 50 
F5 "I2C_4" B R 6350 3000 50 
F6 "moterDriver" O R 6350 2750 50 
F7 "PICO_QUICK" B L 5300 2700 50 
F8 "5V_GND" B L 5300 2600 50 
F9 "5V_typeC" B R 6350 2650 50 
$EndSheet
Entry Bus Bus
	2950 4400 3050 4500
Entry Bus Bus
	2950 4500 3050 4600
Wire Bus Line
	3050 4700 2850 4700
Wire Bus Line
	2850 4700 2850 2600
Wire Bus Line
	2850 2600 5300 2600
$Sheet
S 6900 2900 500  150 
U 616787AD
F0 "direction_senser" 50
F1 "direction_senser.sch" 50
F2 "I2C" I L 6900 3000 50 
$EndSheet
Wire Bus Line
	5300 2700 5200 2700
Wire Bus Line
	5200 2700 5200 3250
Wire Bus Line
	5200 3250 6800 3250
Wire Bus Line
	6800 3250 6800 3550
Wire Bus Line
	6800 3550 6700 3550
$Sheet
S 6900 3300 500  150 
U 61EECFE3
F0 "sheet61EECFE0" 50
F1 "INA.sch" 50
F2 "I2C" I L 6900 3400 50 
$EndSheet
Wire Bus Line
	6900 3400 6850 3400
Wire Bus Line
	6850 3400 6850 3100
Wire Bus Line
	6850 3100 6450 3100
Wire Bus Line
	6450 3100 6450 3000
Wire Bus Line
	6450 3000 6350 3000
Wire Bus Line
	3050 4500 3050 4700
Wire Wire Line
	2250 4500 3000 4500
Wire Bus Line
	1350 4250 1350 4400
$EndSCHEMATC
