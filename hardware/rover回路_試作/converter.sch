EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4250 3550 0    50   Input ~ 0
in+
Text HLabel 4250 4550 0    50   Input ~ 0
in-
Text HLabel 8350 3400 2    50   Output ~ 0
out+
Text HLabel 8350 4550 2    50   Output ~ 0
out-
Text HLabel 8350 3600 2    50   Output ~ 0
out2+
Text HLabel 8350 4750 2    50   Output ~ 0
out2-
$Comp
L New_Library:HRD05003 IC?
U 1 1 61FFECDD
P 6250 3100
F 0 "IC?" H 6375 3065 50  0000 C CNN
F 1 "HRD05003" H 6375 2974 50  0000 C CNN
F 2 "" H 6250 3000 50  0001 C CNN
F 3 "" H 6250 3000 50  0001 C CNN
	1    6250 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3550 5350 3550
Wire Wire Line
	4250 4550 4800 4550
$Comp
L Device:C C?
U 1 1 620015B3
P 5350 3950
F 0 "C?" H 5465 3996 50  0000 L CNN
F 1 "0.1uF" H 5465 3905 50  0000 L CNN
F 2 "" H 5388 3800 50  0001 C CNN
F 3 "~" H 5350 3950 50  0001 C CNN
	1    5350 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 620018BA
P 7950 3950
F 0 "C?" H 8065 3996 50  0000 L CNN
F 1 "0.1uF" H 8065 3905 50  0000 L CNN
F 2 "" H 7988 3800 50  0001 C CNN
F 3 "~" H 7950 3950 50  0001 C CNN
	1    7950 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C?
U 1 1 6200219E
P 4800 3950
F 0 "C?" H 4918 3996 50  0000 L CNN
F 1 "470uF" H 4918 3905 50  0000 L CNN
F 2 "" H 4838 3800 50  0001 C CNN
F 3 "~" H 4800 3950 50  0001 C CNN
	1    4800 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C?
U 1 1 620027A3
P 7450 3950
F 0 "C?" H 7568 3996 50  0000 L CNN
F 1 "220uF" H 7568 3905 50  0000 L CNN
F 2 "" H 7488 3800 50  0001 C CNN
F 3 "~" H 7450 3950 50  0001 C CNN
	1    7450 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3550 4800 3800
Connection ~ 4800 3550
Wire Wire Line
	4800 3550 4250 3550
Wire Wire Line
	4800 4100 4800 4550
Connection ~ 4800 4550
Wire Wire Line
	4800 4550 5350 4550
Wire Wire Line
	5350 4550 5350 4100
Connection ~ 5350 4550
Wire Wire Line
	5350 4550 5700 4550
Connection ~ 5350 3550
Wire Wire Line
	5350 3550 4800 3550
Wire Wire Line
	5950 3400 5700 3400
Wire Wire Line
	5700 3400 5700 3850
Connection ~ 5700 4550
Wire Wire Line
	5950 3850 5700 3850
Connection ~ 5700 3850
Wire Wire Line
	5700 3850 5700 4550
Wire Wire Line
	5950 4000 5950 4350
Wire Wire Line
	5950 4350 7050 4350
Wire Wire Line
	7050 4350 7050 3700
Wire Wire Line
	7050 3700 6800 3700
Wire Wire Line
	5950 3700 5850 3700
Wire Wire Line
	5850 3700 5850 4200
Wire Wire Line
	5850 4200 6900 4200
Wire Wire Line
	6900 4200 6900 3850
Wire Wire Line
	6900 3850 6800 3850
NoConn ~ 6800 3550
Wire Wire Line
	5700 4550 7450 4550
Wire Wire Line
	6800 3400 7450 3400
Wire Wire Line
	8200 3400 8200 3600
Wire Wire Line
	8200 3600 8350 3600
Connection ~ 8200 3400
Wire Wire Line
	8200 3400 8350 3400
Wire Wire Line
	8200 4750 8350 4750
Wire Wire Line
	8200 4550 8200 4750
Connection ~ 8200 4550
Wire Wire Line
	8200 4550 8350 4550
Wire Wire Line
	5350 3550 5350 3800
Wire Wire Line
	7450 3800 7450 3400
Connection ~ 7450 3400
Wire Wire Line
	7450 3400 7950 3400
Connection ~ 7450 4550
Wire Wire Line
	7450 4550 7950 4550
Wire Wire Line
	7450 4100 7450 4550
Connection ~ 7950 4550
Wire Wire Line
	7950 4550 8200 4550
Wire Wire Line
	7950 4100 7950 4550
Wire Wire Line
	7950 3800 7950 3400
Connection ~ 7950 3400
Wire Wire Line
	7950 3400 8200 3400
$EndSCHEMATC
