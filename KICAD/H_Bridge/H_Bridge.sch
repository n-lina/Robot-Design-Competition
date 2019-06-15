EESchema Schematic File Version 4
LIBS:H_Bridge-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "H Bridge"
Date "2019-06-15"
Rev "v01"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L h_bridge:MCT6 U?
U 1 1 5D04CD9F
P 3000 3800
F 0 "U?" H 3025 4315 50  0000 C CNN
F 1 "MCT6" H 3025 4224 50  0000 C CNN
F 2 "" H 2900 3750 50  0001 C CNN
F 3 "" H 2900 3750 50  0001 C CNN
	1    3000 3800
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:IRLB3034PBF Q1
U 1 1 5D0567E1
P 7850 2650
F 0 "Q1" H 7958 2703 60  0000 L CNN
F 1 "IRFZ44" H 7958 2597 60  0000 L CNN
F 2 "digikey-footprints:TO-220-3" H 8050 2850 60  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irlb3034pbf.pdf?fileId=5546d462533600a40153566027b22585" H 8050 2950 60  0001 L CNN
F 4 "IRLB3034PBF-ND" H 8050 3050 60  0001 L CNN "Digi-Key_PN"
F 5 "IRLB3034PBF" H 8050 3150 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 8050 3250 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 8050 3350 60  0001 L CNN "Family"
F 8 "https://www.infineon.com/dgdl/irlb3034pbf.pdf?fileId=5546d462533600a40153566027b22585" H 8050 3450 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/infineon-technologies/IRLB3034PBF/IRLB3034PBF-ND/2096638" H 8050 3550 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET N-CH 40V 195A TO220AB" H 8050 3650 60  0001 L CNN "Description"
F 11 "Infineon Technologies" H 8050 3750 60  0001 L CNN "Manufacturer"
F 12 "Active" H 8050 3850 60  0001 L CNN "Status"
	1    7850 2650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5D05941E
P 2000 3550
F 0 "R1" V 1793 3550 50  0000 C CNN
F 1 "330" V 1884 3550 50  0000 C CNN
F 2 "" V 1930 3550 50  0001 C CNN
F 3 "~" H 2000 3550 50  0001 C CNN
	1    2000 3550
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5D05B4A1
P 2000 4050
F 0 "R2" V 1793 4050 50  0000 C CNN
F 1 "330" V 1884 4050 50  0000 C CNN
F 2 "" V 1930 4050 50  0001 C CNN
F 3 "~" H 2000 4050 50  0001 C CNN
	1    2000 4050
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5D05BA40
P 3650 5000
F 0 "R3" H 3720 5046 50  0000 L CNN
F 1 "1k" H 3720 4955 50  0000 L CNN
F 2 "" V 3580 5000 50  0001 C CNN
F 3 "~" H 3650 5000 50  0001 C CNN
	1    3650 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5D05C435
P 3950 5000
F 0 "R4" H 4020 5046 50  0000 L CNN
F 1 "1k" H 4020 4955 50  0000 L CNN
F 2 "" V 3880 5000 50  0001 C CNN
F 3 "~" H 3950 5000 50  0001 C CNN
	1    3950 5000
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:IRLB3034PBF Q2
U 1 1 5D05DC1B
P 9250 2650
F 0 "Q2" H 9358 2703 60  0000 L CNN
F 1 "IRFZ44" H 9358 2597 60  0000 L CNN
F 2 "digikey-footprints:TO-220-3" H 9450 2850 60  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irlb3034pbf.pdf?fileId=5546d462533600a40153566027b22585" H 9450 2950 60  0001 L CNN
F 4 "IRLB3034PBF-ND" H 9450 3050 60  0001 L CNN "Digi-Key_PN"
F 5 "IRLB3034PBF" H 9450 3150 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 9450 3250 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 9450 3350 60  0001 L CNN "Family"
F 8 "https://www.infineon.com/dgdl/irlb3034pbf.pdf?fileId=5546d462533600a40153566027b22585" H 9450 3450 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/infineon-technologies/IRLB3034PBF/IRLB3034PBF-ND/2096638" H 9450 3550 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET N-CH 40V 195A TO220AB" H 9450 3650 60  0001 L CNN "Description"
F 11 "Infineon Technologies" H 9450 3750 60  0001 L CNN "Manufacturer"
F 12 "Active" H 9450 3850 60  0001 L CNN "Status"
	1    9250 2650
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:2N7000 Q5
U 1 1 5D06072A
P 6700 3600
F 0 "Q5" H 6808 3653 60  0000 L CNN
F 1 "VN2222L" H 6808 3547 60  0000 L CNN
F 2 "digikey-footprints:TO-92-3" H 6900 3800 60  0001 L CNN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 6900 3900 60  0001 L CNN
F 4 "2N7000FS-ND" H 6900 4000 60  0001 L CNN "Digi-Key_PN"
F 5 "2N7000" H 6900 4100 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 6900 4200 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 6900 4300 60  0001 L CNN "Family"
F 8 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 6900 4400 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/on-semiconductor/2N7000/2N7000FS-ND/244278" H 6900 4500 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET N-CH 60V 200MA TO-92" H 6900 4600 60  0001 L CNN "Description"
F 11 "ON Semiconductor" H 6900 4700 60  0001 L CNN "Manufacturer"
F 12 "Active" H 6900 4800 60  0001 L CNN "Status"
	1    6700 3600
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:2N7000 Q6
U 1 1 5D062ADD
P 6750 4650
F 0 "Q6" H 6858 4703 60  0000 L CNN
F 1 "VN2222L" H 6858 4597 60  0000 L CNN
F 2 "digikey-footprints:TO-92-3" H 6950 4850 60  0001 L CNN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 6950 4950 60  0001 L CNN
F 4 "2N7000FS-ND" H 6950 5050 60  0001 L CNN "Digi-Key_PN"
F 5 "2N7000" H 6950 5150 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 6950 5250 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 6950 5350 60  0001 L CNN "Family"
F 8 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 6950 5450 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/on-semiconductor/2N7000/2N7000FS-ND/244278" H 6950 5550 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET N-CH 60V 200MA TO-92" H 6950 5650 60  0001 L CNN "Description"
F 11 "ON Semiconductor" H 6950 5750 60  0001 L CNN "Manufacturer"
F 12 "Active" H 6950 5850 60  0001 L CNN "Status"
	1    6750 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C2
U 1 1 5D05D279
P 5250 2350
F 0 "C2" H 5365 2396 50  0000 L CNN
F 1 "100uF" H 5365 2305 50  0000 L CNN
F 2 "" H 5250 2350 50  0001 C CNN
F 3 "~" H 5250 2350 50  0001 C CNN
	1    5250 2350
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M?
U 1 1 5D04B3AA
P 8650 3600
F 0 "M?" V 8355 3550 50  0000 C CNN
F 1 "Motor_DC" V 8446 3550 50  0000 C CNN
F 2 "" H 8650 3510 50  0001 C CNN
F 3 "~" H 8650 3510 50  0001 C CNN
	1    8650 3600
	0    1    1    0   
$EndComp
$Comp
L dk_Battery-Holders-Clips-Contacts:BS-7 BAT?
U 1 1 5D056864
P 10150 3950
F 0 "BAT?" H 10278 4003 60  0000 L CNN
F 1 "BS-7" H 10278 3897 60  0000 L CNN
F 2 "digikey-footprints:Battery_Holder_Coin_2032_BS-7" H 10350 4150 60  0001 L CNN
F 3 "http://www.memoryprotectiondevices.com/datasheets/BS-7-datasheet.pdf" V 10350 4250 60  0001 L CNN
F 4 "BS-7-ND" H 10350 4350 60  0001 L CNN "Digi-Key_PN"
F 5 "BS-7" H 10350 4450 60  0001 L CNN "MPN"
F 6 "Battery Products" H 10350 4550 60  0001 L CNN "Category"
F 7 "Battery Holders, Clips, Contacts" H 10350 4650 60  0001 L CNN "Family"
F 8 "http://www.memoryprotectiondevices.com/datasheets/BS-7-datasheet.pdf" H 10350 4750 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/mpd-memory-protection-devices/BS-7/BS-7-ND/389447" H 10350 4850 60  0001 L CNN "DK_Detail_Page"
F 10 "BATTERY HOLDER COIN 20MM PC PIN" H 10350 4950 60  0001 L CNN "Description"
F 11 "MPD (Memory Protection Devices)" H 10350 5050 60  0001 L CNN "Manufacturer"
F 12 "Active" H 10350 5150 60  0001 L CNN "Status"
	1    10150 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 3750 10150 2200
Wire Wire Line
	10150 2200 9250 2200
Wire Wire Line
	3850 2200 3850 3550
Wire Wire Line
	3850 3550 3500 3550
Connection ~ 5250 2200
Wire Wire Line
	3850 3550 3850 3900
Connection ~ 3850 3550
Connection ~ 9250 2200
$Comp
L dk_Transistors-FETs-MOSFETs-Single:IRLB3034PBF Q3
U 1 1 5D068F62
P 7850 4750
F 0 "Q3" H 7958 4803 60  0000 L CNN
F 1 "IRFZ44" H 7958 4697 60  0000 L CNN
F 2 "digikey-footprints:TO-220-3" H 8050 4950 60  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irlb3034pbf.pdf?fileId=5546d462533600a40153566027b22585" H 8050 5050 60  0001 L CNN
F 4 "IRLB3034PBF-ND" H 8050 5150 60  0001 L CNN "Digi-Key_PN"
F 5 "IRLB3034PBF" H 8050 5250 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 8050 5350 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 8050 5450 60  0001 L CNN "Family"
F 8 "https://www.infineon.com/dgdl/irlb3034pbf.pdf?fileId=5546d462533600a40153566027b22585" H 8050 5550 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/infineon-technologies/IRLB3034PBF/IRLB3034PBF-ND/2096638" H 8050 5650 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET N-CH 40V 195A TO220AB" H 8050 5750 60  0001 L CNN "Description"
F 11 "Infineon Technologies" H 8050 5850 60  0001 L CNN "Manufacturer"
F 12 "Active" H 8050 5950 60  0001 L CNN "Status"
	1    7850 4750
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:IRLB3034PBF Q4
U 1 1 5D06A53F
P 9250 4750
F 0 "Q4" H 9358 4803 60  0000 L CNN
F 1 "IRFZ44" H 9358 4697 60  0000 L CNN
F 2 "digikey-footprints:TO-220-3" H 9450 4950 60  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irlb3034pbf.pdf?fileId=5546d462533600a40153566027b22585" H 9450 5050 60  0001 L CNN
F 4 "IRLB3034PBF-ND" H 9450 5150 60  0001 L CNN "Digi-Key_PN"
F 5 "IRLB3034PBF" H 9450 5250 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 9450 5350 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 9450 5450 60  0001 L CNN "Family"
F 8 "https://www.infineon.com/dgdl/irlb3034pbf.pdf?fileId=5546d462533600a40153566027b22585" H 9450 5550 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/infineon-technologies/IRLB3034PBF/IRLB3034PBF-ND/2096638" H 9450 5650 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET N-CH 40V 195A TO220AB" H 9450 5750 60  0001 L CNN "Description"
F 11 "Infineon Technologies" H 9450 5850 60  0001 L CNN "Manufacturer"
F 12 "Active" H 9450 5950 60  0001 L CNN "Status"
	1    9250 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3700 3650 3700
Wire Wire Line
	3500 4050 3950 4050
Wire Wire Line
	3650 3700 3650 4850
Wire Wire Line
	3950 4050 3950 4850
Wire Wire Line
	3650 5150 3650 5300
Wire Wire Line
	3650 5300 3950 5300
Wire Wire Line
	3950 5150 3950 5300
Connection ~ 3950 5300
Wire Wire Line
	3950 5300 4600 5300
Wire Wire Line
	3650 3700 4600 3700
Connection ~ 3650 3700
$Comp
L h_bridge:LT1161 U?
U 1 1 5D06997F
P 5100 3500
F 0 "U?" H 5075 3865 50  0000 C CNN
F 1 "LT1161" H 5075 3774 50  0000 C CNN
F 2 "" H 4850 3600 50  0001 C CNN
F 3 "" H 4850 3600 50  0001 C CNN
	1    5100 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 3900 3500 3900
Wire Wire Line
	3950 4050 3950 4000
Wire Wire Line
	3950 4000 4600 4000
Connection ~ 3950 4050
Wire Wire Line
	4600 4750 4600 5300
Wire Wire Line
	5750 2200 5750 3400
Wire Wire Line
	5750 3400 5550 3400
Connection ~ 5750 2200
Wire Wire Line
	5750 2200 5250 2200
Wire Wire Line
	5550 3550 5750 3550
Wire Wire Line
	5750 3550 5750 3400
Connection ~ 5750 3400
Wire Wire Line
	5550 4750 5750 4750
Wire Wire Line
	5750 4750 5750 3850
Connection ~ 5750 3550
Wire Wire Line
	5550 3850 5750 3850
Connection ~ 5750 3850
Wire Wire Line
	5750 3850 5750 3550
Wire Wire Line
	5550 3700 6050 3700
Wire Wire Line
	5550 4000 6250 4000
Wire Wire Line
	6250 4000 6250 3700
Wire Wire Line
	6250 3700 6400 3700
Wire Wire Line
	6050 3700 6050 3250
Wire Wire Line
	6050 3250 6700 3250
Wire Wire Line
	6700 3250 6700 3400
Connection ~ 6050 3700
Wire Wire Line
	6700 3250 7550 3250
Wire Wire Line
	7550 3250 7550 2750
Connection ~ 6700 3250
Wire Wire Line
	7550 3250 7550 4050
Wire Wire Line
	7550 4050 8600 4050
Wire Wire Line
	8600 4050 8600 4850
Wire Wire Line
	8600 4850 8950 4850
Connection ~ 7550 3250
Wire Wire Line
	10150 5300 10150 4150
Connection ~ 4600 5300
Wire Wire Line
	7850 4950 7850 5300
Connection ~ 7850 5300
Wire Wire Line
	7850 5300 9250 5300
Wire Wire Line
	9250 4950 9250 5300
Connection ~ 9250 5300
Wire Wire Line
	9250 5300 10150 5300
Wire Wire Line
	6700 3800 6700 4050
Wire Wire Line
	6700 4050 7200 4050
Wire Wire Line
	7200 4050 7200 5300
Connection ~ 7200 5300
Wire Wire Line
	7200 5300 7850 5300
Wire Wire Line
	9250 2200 9250 2450
Wire Wire Line
	5750 2200 7850 2200
Wire Wire Line
	7850 2200 7850 2450
Connection ~ 7850 2200
Wire Wire Line
	7850 2200 9250 2200
Wire Wire Line
	6250 4250 6250 4000
Connection ~ 6250 4000
Wire Wire Line
	4600 5300 6750 5300
Wire Wire Line
	6050 4750 6450 4750
Wire Wire Line
	6050 3700 6050 4750
Wire Wire Line
	6750 4850 6750 5300
Connection ~ 6750 5300
Wire Wire Line
	6750 5300 7200 5300
Wire Wire Line
	6750 4450 6750 4250
Wire Wire Line
	6250 4250 6750 4250
Wire Wire Line
	6750 4250 7350 4250
Wire Wire Line
	7350 4250 7350 4850
Wire Wire Line
	7350 4850 7550 4850
Connection ~ 6750 4250
Wire Wire Line
	7350 4250 8100 4250
Wire Wire Line
	8100 4250 8100 2750
Wire Wire Line
	8100 2750 8950 2750
Connection ~ 7350 4250
Wire Wire Line
	7850 2850 7850 3600
Wire Wire Line
	7850 3600 8250 3600
Connection ~ 7850 3600
Wire Wire Line
	7850 3600 7850 4550
Wire Wire Line
	9250 2850 9250 3600
Wire Wire Line
	8850 3600 8950 3600
Connection ~ 9250 3600
Wire Wire Line
	9250 3600 9250 4550
$Comp
L Device:C C3
U 1 1 5D099535
P 8600 3900
F 0 "C3" V 8650 3800 50  0000 C CNN
F 1 "100nF" V 8650 4000 50  0000 C CNN
F 2 "" H 8638 3750 50  0001 C CNN
F 3 "~" H 8600 3900 50  0001 C CNN
	1    8600 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	3850 2200 4650 2200
$Comp
L Device:C C1
U 1 1 5D09A17D
P 4650 2350
F 0 "C1" H 4765 2396 50  0000 L CNN
F 1 "100pF" H 4765 2305 50  0000 L CNN
F 2 "" H 4688 2200 50  0001 C CNN
F 3 "~" H 4650 2350 50  0001 C CNN
	1    4650 2350
	1    0    0    -1  
$EndComp
Connection ~ 4650 2200
Wire Wire Line
	4650 2200 5250 2200
Wire Wire Line
	8250 3600 8250 3900
Wire Wire Line
	8250 3900 8450 3900
Connection ~ 8250 3600
Wire Wire Line
	8250 3600 8350 3600
Wire Wire Line
	8750 3900 8950 3900
Wire Wire Line
	8950 3900 8950 3600
Connection ~ 8950 3600
Wire Wire Line
	8950 3600 9250 3600
Wire Wire Line
	2150 3550 2550 3550
Wire Wire Line
	2150 4050 2550 4050
Wire Wire Line
	2550 3700 2300 3700
Wire Wire Line
	2300 3700 2300 3900
Wire Wire Line
	2300 3900 2550 3900
$Comp
L power:VCC BP_Out1
U 1 1 5D0A45D8
P 1600 3550
F 0 "BP_Out1" V 1600 4000 50  0000 R CNN
F 1 "VCC" V 1618 3496 50  0001 R CNN
F 2 "" H 1600 3550 50  0001 C CNN
F 3 "" H 1600 3550 50  0001 C CNN
	1    1600 3550
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC BP_Out2
U 1 1 5D0A659F
P 1600 4050
F 0 "BP_Out2" V 1600 4500 50  0000 R CNN
F 1 "VCC" V 1618 4177 50  0001 L CNN
F 2 "" H 1600 4050 50  0001 C CNN
F 3 "" H 1600 4050 50  0001 C CNN
	1    1600 4050
	0    -1   -1   0   
$EndComp
$Comp
L power:GND BP_GND
U 1 1 5D0A6B2F
P 1550 4650
F 0 "BP_GND" V 1550 4400 50  0000 C CNN
F 1 "GND" V 1550 4500 50  0001 R CNN
F 2 "" H 1550 4650 50  0001 C CNN
F 3 "" H 1550 4650 50  0001 C CNN
	1    1550 4650
	0    1    1    0   
$EndComp
Wire Wire Line
	1600 3550 1850 3550
Wire Wire Line
	1600 4050 1850 4050
Wire Wire Line
	1550 4650 2300 4650
Wire Wire Line
	2300 4650 2300 3900
Connection ~ 2300 3900
$Comp
L power:GND #PWR?
U 1 1 5D0AEE7C
P 5250 2650
F 0 "#PWR?" H 5250 2400 50  0001 C CNN
F 1 "GND" H 5255 2477 50  0000 C CNN
F 2 "" H 5250 2650 50  0001 C CNN
F 3 "" H 5250 2650 50  0001 C CNN
	1    5250 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 2500 4650 2650
Wire Wire Line
	5250 2500 5250 2650
Wire Wire Line
	4650 2650 5250 2650
Connection ~ 5250 2650
$EndSCHEMATC
