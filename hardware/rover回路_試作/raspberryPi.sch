EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4600 4250 0    50   Input ~ 0
GND
Text HLabel 2150 4050 0    50   Input ~ 0
GPIO_X
Text HLabel 6000 4550 0    50   Input ~ 0
powerUSB
$Comp
L Connector:Raspberry_Pi_2_3 J?
U 1 1 6167A180
P 3450 4650
F 0 "J?" H 3450 6131 50  0000 C CNN
F 1 "Raspberry_Pi_2_3" H 3450 6040 50  0000 C CNN
F 2 "" H 3450 4650 50  0001 C CNN
F 3 "https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/rpi_SCH_3bplus_1p0_reduced.pdf" H 3450 4650 50  0001 C CNN
	1    3450 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 6167C640
P 3550 2950
F 0 "#PWR?" H 3550 2800 50  0001 C CNN
F 1 "+3.3V" H 3565 3123 50  0000 C CNN
F 2 "" H 3550 2950 50  0001 C CNN
F 3 "" H 3550 2950 50  0001 C CNN
	1    3550 2950
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND?
U 1 1 6167D447
P 3050 6150
F 0 "#GND?" H 3050 6050 50  0001 C CNN
F 1 "0" H 3050 6239 50  0000 C CNN
F 2 "" H 3050 6150 50  0001 C CNN
F 3 "~" H 3050 6150 50  0001 C CNN
	1    3050 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 2950 3550 3350
Wire Wire Line
	3050 5950 3050 6150
Wire Wire Line
	4250 4050 4850 4050
Wire Wire Line
	4250 4150 4850 4150
$Comp
L power:+3.3V #PWR?
U 1 1 6167EBF4
P 4600 3900
F 0 "#PWR?" H 4600 3750 50  0001 C CNN
F 1 "+3.3V" H 4615 4073 50  0000 C CNN
F 2 "" H 4600 3900 50  0001 C CNN
F 3 "" H 4600 3900 50  0001 C CNN
	1    4600 3900
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND?
U 1 1 6167F603
P 4600 4350
F 0 "#GND?" H 4600 4250 50  0001 C CNN
F 1 "0" H 4600 4439 50  0000 C CNN
F 2 "" H 4600 4350 50  0001 C CNN
F 3 "~" H 4600 4350 50  0001 C CNN
	1    4600 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3900 4600 4000
Wire Wire Line
	4600 4000 4850 4000
Wire Wire Line
	4600 4350 4600 4250
Wire Wire Line
	4600 4250 4850 4250
Text HLabel 5350 3650 3    50   BiDi ~ 0
I2C
Text HLabel 5250 3650 3    50   BiDi ~ 0
I2C_2
Text HLabel 5150 3650 3    50   BiDi ~ 0
I2C_3
Text HLabel 5050 3650 3    50   BiDi ~ 0
I2C_4
Entry Wire Line
	4850 4000 4950 4100
Entry Wire Line
	4850 4050 4950 4150
Entry Wire Line
	4850 4150 4950 4250
Entry Wire Line
	4850 4250 4950 4350
Wire Bus Line
	4950 3650 5350 3650
Wire Wire Line
	4250 4750 4450 4750
Wire Wire Line
	4450 4750 4450 5250
Wire Wire Line
	4500 4550 4500 5150
Wire Wire Line
	4250 4550 4500 4550
Wire Wire Line
	4450 5250 6100 5250
Wire Wire Line
	4500 5150 6100 5150
Wire Wire Line
	4250 5350 6100 5350
Wire Wire Line
	4250 5450 6100 5450
Text Label 4600 5150 0    50   ~ 0
PWM_PIN2(R_PWM)
Text Label 4600 5250 0    50   ~ 0
PWM_PIN1(L_PWM)
Text Label 4600 5350 0    50   ~ 0
DIR_PIN_1(L_DIR)
Text Label 4600 5450 0    50   ~ 0
DIR_PIN_2(R_DIR)
Entry Wire Line
	6100 5150 6200 5250
Entry Wire Line
	6100 5250 6200 5350
Entry Wire Line
	6100 5350 6200 5450
Entry Wire Line
	6100 5450 6200 5550
$Comp
L pspice:0 #GND?
U 1 1 61685741
P 6100 5700
F 0 "#GND?" H 6100 5600 50  0001 C CNN
F 1 "0" H 6100 5789 50  0000 C CNN
F 2 "" H 6100 5700 50  0001 C CNN
F 3 "~" H 6100 5700 50  0001 C CNN
	1    6100 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 5700 6100 5600
Entry Wire Line
	6100 5600 6200 5700
Wire Bus Line
	6200 5800 6350 5800
Text HLabel 6350 5800 2    50   Output ~ 0
moterDriver
Wire Wire Line
	2150 4050 2650 4050
Wire Bus Line
	4950 3650 4950 4350
Wire Bus Line
	6200 5250 6200 5800
$EndSCHEMATC
