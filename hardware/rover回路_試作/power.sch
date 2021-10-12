EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 7
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
F2 "pico_power+" I L 5750 3500 50 
F3 "pico_power-" I L 5750 3550 50 
F4 "I2C_3.3V" I L 5750 3700 50 
$EndSheet
Wire Wire Line
	5750 3550 4900 3550
Wire Wire Line
	4900 3550 4900 3500
Wire Wire Line
	4900 3500 4650 3500
Wire Wire Line
	4650 3400 5550 3400
Wire Wire Line
	5550 3400 5550 3500
Wire Wire Line
	5550 3500 5750 3500
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
$Comp
L Device:Battery_Cell BT?
U 1 1 616480EF
P 1800 3500
F 0 "BT?" H 1918 3596 50  0000 L CNN
F 1 "Battery_Cell" H 1918 3505 50  0000 L CNN
F 2 "" V 1800 3560 50  0001 C CNN
F 3 "~" V 1800 3560 50  0001 C CNN
	1    1800 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3050 2400 3050
Wire Wire Line
	2400 3050 2400 2950
Wire Wire Line
	2400 2950 1800 2950
Wire Wire Line
	1800 3250 2450 3250
Wire Wire Line
	2450 3250 2450 3150
Wire Wire Line
	2450 3150 3050 3150
Wire Wire Line
	2550 3300 2550 3250
Wire Wire Line
	2550 3250 3050 3250
Wire Wire Line
	3050 3350 2450 3350
Wire Wire Line
	2450 3350 2450 3600
Wire Wire Line
	2450 3600 1800 3600
Wire Wire Line
	1800 3300 2550 3300
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
Wire Wire Line
	3050 3850 3000 3850
Wire Wire Line
	3000 4500 2250 4500
Wire Wire Line
	2250 4400 2950 4400
Wire Wire Line
	2950 4400 2950 3750
Wire Wire Line
	2950 3750 3050 3750
$Sheet
S 5550 2550 800  500 
U 6164CA09
F0 "raspberryPi" 50
F1 "raspberryPi.sch" 50
F2 "GND" I L 5550 2800 50 
F3 "GPIO_X" I L 5550 2700 50 
F4 "powerUSB" I R 6350 2650 50 
F5 "I2C" B L 5550 3000 50 
F6 "I2C_2" B R 6350 2800 50 
F7 "I2C_3" B R 6350 2900 50 
F8 "I2C_4" B R 6350 3000 50 
F9 "moterDriver" O R 6350 2750 50 
$EndSheet
Wire Wire Line
	6350 2650 6450 2650
Wire Wire Line
	6450 2650 6450 2400
Wire Wire Line
	4650 3100 5100 3100
Wire Wire Line
	5100 3100 5100 2700
Wire Wire Line
	5100 2700 5550 2700
Wire Wire Line
	5550 2800 5150 2800
Wire Wire Line
	5150 2800 5150 3200
Wire Wire Line
	5150 3200 4650 3200
Wire Wire Line
	5550 3000 5350 3000
Wire Wire Line
	5350 3000 5350 3300
Wire Wire Line
	5350 3300 4650 3300
Wire Wire Line
	3000 3850 3000 4500
$Sheet
S 3050 2900 1600 1350
U 6160D455
F0 "power_control" 50
F1 "power_control.sch" 50
F2 "converter_after+" I L 3050 3750 50 
F3 "converter_after-" I L 3050 3850 50 
F4 "converter_before+" O L 3050 3500 50 
F5 "converter_before-" O L 3050 3600 50 
F6 "moterPowerContolloer+" I R 4650 3100 50 
F7 "moterPowerContolloer-" I R 4650 3200 50 
F8 "battery2+" I L 3050 3250 50 
F9 "battery2-" I L 3050 3350 50 
F10 "battery1+" I L 3050 3050 50 
F11 "battery1-" I L 3050 3150 50 
F12 "pico_+5V" B R 4650 3400 50 
F13 "pico_GND" B R 4650 3500 50 
F14 "I2C" B R 4650 3300 50 
F15 "moter" B R 4650 3900 50 
F16 "USB_POWER_OUT" O R 4650 2950 50 
$EndSheet
$Sheet
S 1500 4050 750  650 
U 61649E63
F0 "converter" 50
F1 "converter.sch" 50
F2 "in+" I R 2250 4150 50 
F3 "in-" I R 2250 4250 50 
F4 "out+" O R 2250 4400 50 
F5 "out-" O R 2250 4500 50 
$EndSheet
Wire Wire Line
	4650 2950 4900 2950
Wire Wire Line
	4900 2950 4900 2400
Wire Wire Line
	4900 2400 6450 2400
Wire Wire Line
	5750 3700 5600 3700
Wire Wire Line
	5600 3700 5600 3900
Wire Wire Line
	5600 3900 6750 3900
Wire Wire Line
	6750 3900 6750 3000
Wire Wire Line
	6750 3000 6350 3000
$Sheet
S 6900 3000 500  150 
U 616787AD
F0 "direction_senser" 50
F1 "direction_senser.sch" 50
F2 "I2C" I L 6900 3100 50 
$EndSheet
Wire Wire Line
	6900 3100 6800 3100
Wire Wire Line
	6800 3100 6800 2900
Wire Wire Line
	6800 2900 6350 2900
$Sheet
S 5750 4200 1050 300 
U 616868C8
F0 "moterDriver" 50
F1 "moterDriver.sch" 50
F2 "moter" I L 5750 4300 50 
F3 "DriverControl" I R 6800 4300 50 
$EndSheet
Wire Wire Line
	4650 3900 5250 3900
Wire Wire Line
	5250 3900 5250 4300
Wire Wire Line
	5250 4300 5750 4300
Wire Wire Line
	6800 4300 8100 4300
Wire Wire Line
	8100 4300 8100 2750
Wire Wire Line
	8100 2750 6350 2750
$EndSCHEMATC
