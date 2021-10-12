EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 7
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
L Rpi_Pico:Pico U?
U 1 1 617DE19D
P 7550 3350
F 0 "U?" H 7550 4565 50  0000 C CNN
F 1 "Pico" H 7550 4474 50  0000 C CNN
F 2 "RPi_Pico:RPi_Pico_SMD_TH" V 7550 3350 50  0001 C CNN
F 3 "" H 7550 3350 50  0001 C CNN
	1    7550 3350
	1    0    0    -1  
$EndComp
Text HLabel 8600 2400 2    50   Input ~ 0
pico_power+
Text HLabel 8600 2500 2    50   Input ~ 0
pico_power-
Wire Wire Line
	8600 2500 8600 2600
Wire Wire Line
	8600 2600 8250 2600
Wire Wire Line
	8250 2400 8600 2400
$Comp
L power:+3.3V #PWR?
U 1 1 617DF518
P 8500 2750
F 0 "#PWR?" H 8500 2600 50  0001 C CNN
F 1 "+3.3V" H 8515 2923 50  0000 C CNN
F 2 "" H 8500 2750 50  0001 C CNN
F 3 "" H 8500 2750 50  0001 C CNN
	1    8500 2750
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND?
U 1 1 617DF7FF
P 8550 3700
F 0 "#GND?" H 8550 3600 50  0001 C CNN
F 1 "0" H 8550 3789 50  0000 C CNN
F 2 "" H 8550 3700 50  0001 C CNN
F 3 "~" H 8550 3700 50  0001 C CNN
	1    8550 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2800 8500 2800
Wire Wire Line
	8500 2800 8500 2750
Wire Wire Line
	8250 3600 8550 3600
Wire Wire Line
	8550 3600 8550 3700
Text Label 6850 2400 2    50   ~ 0
SDA
Text Label 6850 2500 2    50   ~ 0
SCL
Text GLabel 6700 2800 0    50   Input ~ 0
SCL
Text GLabel 6700 2700 0    50   Input ~ 0
SDA
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 617E4783
P 3250 2650
F 0 "J?" H 3278 2626 50  0000 L CNN
F 1 "Conn_01x06_Female" H 3278 2535 50  0000 L CNN
F 2 "" H 3250 2650 50  0001 C CNN
F 3 "~" H 3250 2650 50  0001 C CNN
	1    3250 2650
	-1   0    0    -1  
$EndComp
Text GLabel 4250 2350 1    50   Input ~ 0
SCL
Text GLabel 4150 2350 1    50   Input ~ 0
SDA
$Comp
L power:+3.3V #PWR?
U 1 1 617F29FB
P 3950 2250
F 0 "#PWR?" H 3950 2100 50  0001 C CNN
F 1 "+3.3V" H 3965 2423 50  0000 C CNN
F 2 "" H 3950 2250 50  0001 C CNN
F 3 "" H 3950 2250 50  0001 C CNN
	1    3950 2250
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND?
U 1 1 617F2FD1
P 4050 2300
F 0 "#GND?" H 4050 2200 50  0001 C CNN
F 1 "0" H 4050 2389 50  0000 C CNN
F 2 "" H 4050 2300 50  0001 C CNN
F 3 "~" H 4050 2300 50  0001 C CNN
	1    4050 2300
	1    0    0    1   
$EndComp
Wire Wire Line
	3950 2250 3950 2450
Wire Wire Line
	4250 2350 4250 2750
Wire Wire Line
	4050 2300 4050 2550
Wire Wire Line
	3450 2450 3950 2450
Connection ~ 3950 2450
Wire Wire Line
	3450 2550 4050 2550
Connection ~ 4050 2550
Wire Wire Line
	4050 2550 4050 3150
Wire Wire Line
	3450 2650 4150 2650
Wire Wire Line
	4150 2350 4150 2650
Connection ~ 4150 2650
Wire Wire Line
	4150 2650 4150 3250
Wire Wire Line
	3450 2750 4250 2750
Connection ~ 4250 2750
Wire Wire Line
	4250 2750 4250 3350
Wire Wire Line
	3450 2850 4400 2850
Wire Wire Line
	3950 2450 3950 3050
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 617FE7FA
P 3250 3250
F 0 "J?" H 3278 3226 50  0000 L CNN
F 1 "Conn_01x06_Female" H 3278 3135 50  0000 L CNN
F 2 "" H 3250 3250 50  0001 C CNN
F 3 "~" H 3250 3250 50  0001 C CNN
	1    3250 3250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3450 3050 3950 3050
Wire Wire Line
	3450 3150 4050 3150
Wire Wire Line
	3450 3250 4150 3250
Wire Wire Line
	3450 3350 4250 3350
Wire Wire Line
	3450 3450 4400 3450
Connection ~ 3950 3050
Wire Wire Line
	3950 3050 3950 3650
Connection ~ 4050 3150
Wire Wire Line
	4050 3150 4050 3750
Connection ~ 4150 3250
Wire Wire Line
	4150 3250 4150 3850
Connection ~ 4250 3350
Wire Wire Line
	4250 3350 4250 3950
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 61803065
P 3250 3850
F 0 "J?" H 3278 3826 50  0000 L CNN
F 1 "Conn_01x06_Female" H 3278 3735 50  0000 L CNN
F 2 "" H 3250 3850 50  0001 C CNN
F 3 "~" H 3250 3850 50  0001 C CNN
	1    3250 3850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3450 3650 3950 3650
Wire Wire Line
	3450 3750 4050 3750
Wire Wire Line
	3450 3850 4150 3850
Wire Wire Line
	3450 3950 4250 3950
Wire Wire Line
	3450 4050 4400 4050
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 618041AF
P 3250 4450
F 0 "J?" H 3278 4426 50  0000 L CNN
F 1 "Conn_01x06_Female" H 3278 4335 50  0000 L CNN
F 2 "" H 3250 4450 50  0001 C CNN
F 3 "~" H 3250 4450 50  0001 C CNN
	1    3250 4450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3450 4250 3950 4250
Wire Wire Line
	3450 4350 4050 4350
Wire Wire Line
	3450 4450 4150 4450
Wire Wire Line
	3450 4550 4250 4550
Wire Wire Line
	3450 4650 4400 4650
Connection ~ 3950 3650
Wire Wire Line
	3950 3650 3950 4250
Connection ~ 3950 4250
Connection ~ 4050 3750
Wire Wire Line
	4050 3750 4050 4350
Connection ~ 4050 4350
Connection ~ 4150 3850
Wire Wire Line
	4150 3850 4150 4450
Connection ~ 4150 4450
Connection ~ 4250 3950
Wire Wire Line
	4250 3950 4250 4550
Connection ~ 4250 4550
Wire Wire Line
	6850 2700 6700 2700
Wire Wire Line
	6700 2800 6850 2800
Text GLabel 4400 2850 2    50   Input ~ 0
XSHUT_1
Text GLabel 4400 3450 2    50   Input ~ 0
XSHUT_2
Text GLabel 4400 4050 2    50   Input ~ 0
XSHUT_3
Text GLabel 4400 4650 2    50   Input ~ 0
XSHUT_4
Text GLabel 6850 2900 0    50   Input ~ 0
XSHUT_1
Text GLabel 6850 3000 0    50   Input ~ 0
XSHUT_2
Text GLabel 6850 3200 0    50   Input ~ 0
XSHUT_3
Text GLabel 6850 3300 0    50   Input ~ 0
XSHUT_4
$Comp
L pspice:0 #GND?
U 1 1 6180EDD5
P 6200 2600
F 0 "#GND?" H 6200 2500 50  0001 C CNN
F 1 "0" H 6200 2689 50  0000 C CNN
F 2 "" H 6200 2600 50  0001 C CNN
F 3 "~" H 6200 2600 50  0001 C CNN
	1    6200 2600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61811F2F
P 6250 2350
F 0 "#PWR?" H 6250 2200 50  0001 C CNN
F 1 "+3.3V" H 6265 2523 50  0000 C CNN
F 2 "" H 6250 2350 50  0001 C CNN
F 3 "" H 6250 2350 50  0001 C CNN
	1    6250 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2350 6250 2350
Wire Wire Line
	6100 2400 6850 2400
Wire Wire Line
	6100 2500 6850 2500
Wire Wire Line
	6100 2550 6200 2550
Wire Wire Line
	6200 2550 6200 2600
Entry Wire Line
	6000 2250 6100 2350
Entry Wire Line
	6000 2300 6100 2400
Entry Wire Line
	6000 2400 6100 2500
Entry Wire Line
	6000 2450 6100 2550
Wire Bus Line
	6000 2700 5850 2700
Text HLabel 5850 2700 0    50   Input ~ 0
I2C_3.3V
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 6165B6FE
P 3250 5050
F 0 "J?" H 3278 5026 50  0000 L CNN
F 1 "Conn_01x06_Female" H 3278 4935 50  0000 L CNN
F 2 "" H 3250 5050 50  0001 C CNN
F 3 "~" H 3250 5050 50  0001 C CNN
	1    3250 5050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3450 4850 3950 4850
Wire Wire Line
	3450 4950 4050 4950
Wire Wire Line
	3450 5050 4150 5050
Wire Wire Line
	3450 5150 4250 5150
Wire Wire Line
	3450 5250 4400 5250
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 6165B709
P 3250 5650
F 0 "J?" H 3278 5626 50  0000 L CNN
F 1 "Conn_01x06_Female" H 3278 5535 50  0000 L CNN
F 2 "" H 3250 5650 50  0001 C CNN
F 3 "~" H 3250 5650 50  0001 C CNN
	1    3250 5650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3450 5450 3950 5450
Wire Wire Line
	3450 5550 4050 5550
Wire Wire Line
	3450 5650 4150 5650
Wire Wire Line
	3450 5750 4250 5750
Wire Wire Line
	3450 5850 4400 5850
Wire Wire Line
	3950 4250 3950 4850
Wire Wire Line
	4050 4350 4050 4950
Wire Wire Line
	4150 4450 4150 5050
Wire Wire Line
	4250 4550 4250 5150
Connection ~ 3950 4850
Wire Wire Line
	3950 4850 3950 5450
Connection ~ 4050 4950
Wire Wire Line
	4050 4950 4050 5550
Connection ~ 4150 5050
Wire Wire Line
	4150 5050 4150 5650
Connection ~ 4250 5150
Wire Wire Line
	4250 5150 4250 5750
Text GLabel 4400 5250 2    50   Input ~ 0
XSHUT_5
Text GLabel 4400 5850 2    50   Input ~ 0
XSHUT_6
Text GLabel 6850 3400 0    50   Input ~ 0
XSHUT_5
Text GLabel 6850 3500 0    50   Input ~ 0
XSHUT_6
Wire Bus Line
	6000 2250 6000 2700
$EndSCHEMATC
