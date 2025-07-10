# coding: utf-8
"""
Copyright (C) 2021, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
"""

############################################################################
#### G3 Coupling Parameters ####
############################################################################

pCoupG3DACC = []
pCoupG3DACCAux = []

rms_high_cena  = [2146, 1532, 1093, 778, 554, 395, 283, 202]
rms_vlow_cena  = [5838, 4515, 3272, 2333, 1657, 1175, 832, 590]
thrs_high_cena = [0, 0, 0, 0, 0, 0, 0, 0, 1823, 1302, 929, 661, 471, 335, 240, 172]
thrs_vlow_cena = [0, 0, 0, 0, 0, 0, 0, 0, 9433, 6840, 4924, 3539, 2535, 1824, 1307, 941]
dacc_cena      = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x9D5C00FF, 0x14141414, 0, 0, 0x00000004, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_cena = [126, 60, 336]
gain_vlow_cena = [532, 230, 597]
drv_conf_cena  = 5

rms_high_cenb  = [1071, 761, 541, 383, 272, 194, 139, 99]
rms_vlow_cenb  = [2650, 2031, 1466, 1040, 737, 521, 369, 263]
thrs_high_cenb = [0, 0, 0, 0, 0, 0, 0, 0, 912, 648, 460, 326, 231, 164, 118, 84]
thrs_vlow_cenb = [0, 0, 0, 0, 0, 0, 0, 0, 3743, 2720, 1941, 1378, 975, 691, 489, 346]
dacc_cenb      = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x9C1800FF, 0x19191919, 0, 0, 0x00000004, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_cenb = [221, 110, 496]
gain_vlow_cenb = [786, 350, 883]
drv_conf_cenb  = 5

rms_high_fcc   = [1201, 850, 602, 427, 304, 217, 155, 112]
rms_vlow_fcc   = [4617, 3509, 2512, 1769, 1247, 882, 624, 443]
thrs_high_fcc  = [0, 0, 0, 0, 0, 0, 0, 0, 1020, 721, 510, 362, 257, 183, 131, 95]
thrs_vlow_fcc  = [0, 0, 0, 0, 0, 0, 0, 0, 10327, 7247, 5074, 3562, 2510, 1776, 1261, 896]
dacc_fcc       = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x4F5000FF, 0x1B1B1B1B, 0, 0, 0x00000006, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_fcc  = [49, 20, 256]
gain_vlow_fcc  = [364, 180, 408]
drv_conf_fcc   = 5

rms_high_fcc_hiAtt   = [755, 548, 399, 289, 209, 151, 109, 79]
rms_vlow_fcc_hiAtt   = [3672, 2819, 2039, 1454, 1036, 738, 525, 373]
thrs_high_fcc_hiAtt  = [0, 0, 0, 0, 0, 0, 0, 0, 639, 464, 338, 244, 176, 128, 92, 67]
thrs_vlow_fcc_hiAtt  = [0, 0, 0, 0, 0, 0, 0, 0, 10998, 7864, 5607, 3994, 2834, 2012, 1425, 1013]
dacc_fcc_hiAtt       = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x4F5000FF, 0x1B1B1B1B, 0, 0, 0x00000006, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_fcc_hiAtt  = [105, 50, 256]
gain_vlow_fcc_hiAtt  = [364, 180, 408]
drv_conf_fcc_hiAtt   = 7

rms_high_mbsb_cena  = [485, 346, 248, 176, 125, 89, 64, 48]
rms_vlow_mbsb_cena  = [4631, 3576, 2545, 1756, 1200, 818, 557, 385]
thrs_high_mbsb_cena = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
thrs_vlow_mbsb_cena = [0, 0, 0, 0, 0, 0, 0, 0, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000]
dacc_mbsb_cena      = [0, 0, 0x00000100, 0x00000100, 0, 0, 0xFFFF00FF, 0x1D1D1D1D, 0, 0, 0x00000006, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_mbsb_cena = [90, 90, 90]
gain_vlow_mbsb_cena = [597, 597, 597]
drv_conf_mbsb_cena  = 0xA7

rms_high_mbsb_fcc   = [1166, 836, 597, 426, 305, 218, 157, 113]
rms_vlow_mbsb_fcc   = [4274, 3244, 2323, 1633, 1148, 811, 575, 409]
thrs_high_mbsb_fcc  = [0, 0, 0, 0, 0, 0, 0, 0, 995, 714, 510, 367, 262, 189, 135, 96]
thrs_vlow_mbsb_fcc  = [0, 0, 0, 0, 0, 0, 0, 0, 11055, 7791, 5477, 3856, 2724, 1941, 1391, 1002]
dacc_mbsb_fcc       = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x4F5000FF, 0x1B1B1B1B, 0, 0, 0x00000006, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_mbsb_fcc  = [49, 20, 196]
gain_vlow_mbsb_fcc  = [364, 180, 408]
drv_conf_mbsb_fcc   = 0x47

rms_high_cena_c07  = [1991, 1381, 976, 695, 495, 351, 250, 179]
rms_vlow_cena_c07  = [6356, 4706, 3317, 2308, 1602, 1112, 778, 546]
thrs_high_cena_c07 = [0, 0, 0, 0, 0, 0, 0, 0, 1685, 1173, 828, 589, 419, 298, 212, 151]
thrs_vlow_cena_c07 = [0, 0, 0, 0, 0, 0, 0, 0, 8988, 6370, 4466, 3119, 2171, 1512, 1061, 752]
dacc_cena_c07      = [0, 0x00002120, 0x0000073F, 0x00003F3F, 0x00000333, 0, 0xA20000FF, 0x14141414, 0x00002020, 0x00000044, 0x0FD20004, 0x00000355, 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]
dacc_cena_c07_drv  = [0, 0x21200000, 0x073F0000, 0x3F3F0000, 0x00000CCC, 0, 0xA20000FF, 0x14141414, 0x20200000, 0x00004400, 0x0FD20004, 0x000003AA, 0xF0000000, 0x001020F0, 0x000003AA, 0xF0000000, 0x001020FF]
gain_high_cena_c07 = [142, 70, 336]
gain_vlow_cena_c07 = [474, 230, 597]
drv_conf_cena_c07  = 8

rms_high_cenb_c14  = [1133, 793, 559, 396, 280, 199, 143, 108]
rms_vlow_cenb_c14  = [2871, 2120, 1498, 1054, 740, 519, 366, 259]
thrs_high_cenb_c14 = [0, 0, 0, 0, 0, 0, 0, 0, 950, 667, 471, 334, 238, 169, 122, 90]
thrs_vlow_cenb_c14 = [0, 0, 0, 0, 0, 0, 0, 0, 3878, 2749, 1935, 1362, 965, 686, 493, 353]
dacc_cenb_c14      = [0, 0x00002120, 0x0000073F, 0x00003F3F, 0x00000333, 0, 0xA0BC00FF, 0x19191919, 0x00002020, 0x00000044, 0x0FD20004, 0x00000355, 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]
dacc_cenb_c14_drv  = [0, 0x21200000, 0x073F0000, 0x3F3F0000, 0x00000CCC, 0, 0xA0BC00FF, 0x19191919, 0x20200000, 0x00004400, 0x0FD20004, 0x000003AA, 0xF0000000, 0x001020F0, 0x000003AA, 0xF0000000, 0x001020FF]
gain_high_cenb_c14 = [248, 119, 496]
gain_vlow_cenb_c14 = [701, 350, 883]
drv_conf_cenb_c14  = 8

rms_high_cenb_c12  = [0, 0, 0, 0, 0, 0, 0, 0]
rms_vlow_cenb_c12  = [0, 0, 0, 0, 0, 0, 0, 0]
thrs_high_cenb_c12 = [0, 0, 0, 0, 0, 0, 0, 0, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647]
thrs_vlow_cenb_c12 = [0, 0, 0, 0, 0, 0, 0, 0, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647]
dacc_cenb_c12      = [0, 0, 0x3F3F3F3F, 0x3F3F3F3F, 0x00000FFF, 0, 0xA0BC00FF, 0x19191919, 0, 0, 0x0FD20004, 0x000000FF, 0x0F000000, 0x00102000, 0x000000FF, 0x0F000000, 0x00102000]
gain_high_cenb_c12 = [645, 645, 645]
gain_vlow_cenb_c12 = [645, 645, 645]

rms_high_fcc_c06   = [1355, 960, 681, 485, 345, 246, 177, 129]
rms_vlow_fcc_c06   = [5656, 4174, 2877, 1987, 1413, 1020, 736, 532]
thrs_high_fcc_c06  = [0, 0, 0, 0, 0, 0, 0, 0, 1147, 811, 576, 409, 291, 208, 150, 109]
thrs_vlow_fcc_c06  = [0, 0, 0, 0, 0, 0, 0, 0, 8153, 5718, 4007, 2871, 2080, 1506, 1083, 778]
dacc_fcc_c06       = [0, 0x10102120, 0x033F073F, 0x3F3F3F3F, 0x00000FFF, 0, 0x57C000FF, 0x1B1B1B1B, 0x10101010, 0x00001111, 0x04380006, 0x000003AA, 0xF0000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]
gain_high_fcc_c06  = [109, 50, 256]
gain_vlow_fcc_c06  = [364, 180, 408]

rms_high_arib_c06   = [872, 618, 438, 311, 221, 158, 113, 83]
rms_vlow_arib_c06   = [3383, 2463, 1689, 1168, 832, 603, 438, 316]
thrs_high_arib_c06  = [0, 0, 0, 0, 0, 0, 0, 0, 744, 527, 374, 266, 188, 134, 96, 67]
thrs_vlow_arib_c06  = [0, 0, 0, 0, 0, 0, 0, 0, 4914, 3462, 2458, 1777, 1288, 931, 669, 465]
dacc_arib_c06       = dacc_fcc_c06
gain_high_arib_c06  = [120, 60, 281]
gain_vlow_arib_c06  = [403, 200, 451]

rms_high_fcc_c11   = rms_vlow_fcc_c06
rms_vlow_fcc_c11   = rms_vlow_fcc_c06
thrs_high_fcc_c11  = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
thrs_vlow_fcc_c11  = thrs_vlow_fcc_c06
dacc_fcc_c11       = [0, 0x21200000, 0x073F0000, 0x3F3F0000, 0x00000CCC, 0, 0x2A3000FF, 0x1B1B1B1B, 0x10100000, 0x00001100, 0x04380006, 0x000003AA, 0xF0000000, 0x001020FF, 0x000003AA, 0xF0000000, 0x001020FF]
gain_high_fcc_c11  = [364, 50, 408]
gain_vlow_fcc_c11  = [364, 50, 408]

rms_high_arib_c11   = rms_vlow_arib_c06
rms_vlow_arib_c11   = rms_vlow_arib_c06
thrs_high_arib_c11  = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
thrs_vlow_arib_c11  = thrs_vlow_arib_c06
dacc_arib_c11       = dacc_fcc_c11
gain_high_arib_c11  = [120, 60, 451]
gain_vlow_arib_c11  = [403, 200, 451]

############################################################################
#### PRIME Coupling Parameters ####
############################################################################

pCoupPRIMEMenuChn = []
pCoupPRIMEMenu2Chn = []
pCoupPRIMEDACCCENA = []
pCoupPRIMEDACCFCC = []
pCoupPRIMEDACC2CHN = []

thrs_high_dummy = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
thrs_vlow_dummy = [0, 0, 0, 0, 0, 0, 0, 0, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000]

#### Channel 1 #############################################################
dacc_chn1_c07      = [0x00000000, 0x00002120, 0x0000073F, 0x00003F3F, 0x00000333, 0x00000000, 0xA92C00FF, 0x1A1A1A1A, \
				      0x00002020, 0x00000044, 0x0FD20005, 0x00000355, 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]

dacc_chn1_drv_sb   = [0x00000000, 0x00000000, 0x00000100, 0x00000100, 0x00000000, 0x00000000, 0x9F7800FF, 0x1A1A1A1A, \
				      0x00000000, 0x00000000, 0x00000005, 0x00000355, 0x00000000, 0x001020F0, 0x00000355, 0x00000000, 0x001020FF]

dacc_chn1_drv_aux  = [0x00000000, 0x21200000, 0x073F0000, 0x3F3F0000, 0x00000CCC, 0x00000000, 0xA92C00FF, 0x1A1A1A1A, \
				      0x20200000, 0x00004400, 0x0FD20005, 0x000003AA, 0xF0000000, 0x001020F0, 0x000003AA, 0xF0000000, 0x001020FF]

dacc_chn1_drv_mbsb = [0x00000000, 0x00000000, 0x00000100, 0x00000100, 0x00000000, 0x00000000, 0xFFFF00FF, 0x1B1B1B1B, \
					  0x00000000, 0x00000000, 0x00000006, 0x00000355, 0x00000000, 0x001020F0, 0x00000355, 0x00000000, 0x001020FF]

rms_high_chn1_c07  = [1725, 1522, 1349, 1202, 1071, 957, 855, 764]
rms_vlow_chn1_c07  = [4874, 4427, 3986, 3555, 3157, 2795, 2470, 2184]
thrs_high_chn1_c07 = [0, 0, 0, 0, 0, 0, 0, 0, 1467, 1292, 1145, 1019, 910, 811, 725, 648]
thrs_vlow_chn1_c07 = [0, 0, 0, 0, 0, 0, 0, 0, 8479, 7515, 6665, 5874, 5192, 4576, 4030, 3557]
gain_high_chn1_c07 = [81, 40, 128]
gain_vlow_chn1_c07 = [256, 128, 281]

rms_high_chn1_drv_sb  = [2383, 2126, 1898, 1696, 1517, 1355, 1211, 1081]
rms_vlow_chn1_drv_sb  = [4951, 4531, 4105, 3691, 3300, 2945, 2624, 2338]
thrs_high_chn1_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 2024, 1806, 1613, 1441, 1288, 1151, 1029, 918]
thrs_vlow_chn1_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 8309, 7393, 6644, 5921, 5287, 4716, 4212, 3762]
gain_high_chn1_drv_sb = [81, 40, 128]
gain_vlow_chn1_drv_sb = [256, 128, 281]

rms_high_chn1_drv_aux  = rms_high_chn1_c07
rms_vlow_chn1_drv_aux  = rms_vlow_chn1_c07
thrs_high_chn1_drv_aux = thrs_high_chn1_c07
thrs_vlow_chn1_drv_aux = thrs_vlow_chn1_c07
gain_high_chn1_drv_aux = gain_high_chn1_c07
gain_vlow_chn1_drv_aux = gain_vlow_chn1_c07

rms_high_chn1_drv_mbsb  = [463, 413, 370, 331, 296, 265, 236, 211]
rms_vlow_chn1_drv_mbsb  = [3462, 3130, 2806, 2489, 2202, 1945, 1716, 1514]
thrs_high_chn1_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
thrs_vlow_chn1_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000]
gain_high_chn1_drv_mbsb = [51, 51, 51]
gain_vlow_chn1_drv_mbsb = [256, 256, 256]

#### Channels 2 - 8 ########################################################
dacc_chn_fcc_c06      = [0x00000000, 0x10102120, 0x033F073F, 0x3F3F3F3F, 0x00000FFF, 0x00000000, 0xFFFF00FF, 0x1B1B1B1B, \
                         0x10101010, 0x00001111, 0x04380006, 0x000003AA, 0xF0000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]

dacc_chn_fcc_c11      = [0x00000000, 0x21202120, 0x073F073F, 0x3F3F3F3F, 0x00000FFF, 0x00000000, 0xFFFF00FF, 0x1B1B1B1B, \
                         0x10101010, 0x00001111, 0x04380006, 0x00000355, 0x0F000000, 0x001020FF, 0x000003AA, 0xF0000000, 0x001020FF]

dacc_chn_fcc_drv      = [0x00000000, 0x00000000, 0x00000100, 0x00000100, 0x00000000, 0x00000000, 0xFFFF00FF, 0x1B1B1B1B, \
				         0x00000000, 0x00000000, 0x00000006, 0x00000355, 0x00000000, 0x001020F0, 0x00000355, 0x00000000, 0x001020FF]

dacc_chn_fcc_drv_mbsb = [0x00000000, 0x00000000, 0x00000100, 0x00000100, 0x00000000, 0x00000000, 0xFFFF00FF, 0x20202020, \
				         0x00000000, 0x00000000, 0x00000006, 0x00000355, 0x00000000, 0x001020F0, 0x00000355, 0x00000000, 0x001020FF]

# dacc_chn_fcc_drv_full = [0x00000000, 0x00002120, 0x0000073F, 0x00003F3F, 0x00000333, 0x00000000, 0xA92C00FF, 0x1A1A1A1A, \
# 				         0x00002020, 0x00000044, 0x0FD20005, 0x00000355, 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]  #TBD

rms_high_chn2_c06 = [779, 690, 613, 544, 484, 431, 383, 341]
rms_high_chn3_c06 = [1500, 1332, 1184, 1053, 937, 834, 742, 661]
rms_high_chn4_c06 = [1625, 1447, 1290, 1150, 1024, 913, 815, 726]
rms_high_chn5_c06 = [1653, 1471, 1310, 1167, 1040, 926, 825, 736]
rms_high_chn6_c06 = [1674, 1489, 1326, 1183, 1055, 940, 838, 747]
rms_high_chn7_c06 = [1564, 1393, 1242, 1108, 988, 882, 787, 703]
rms_high_chn8_c06 = [1268, 1132, 1010, 902, 805, 719, 642, 573]

rms_vlow_chn2_c06 = [3595, 3208, 2840, 2493, 2177, 1903, 1666, 1465]
rms_vlow_chn3_c06 = [3754, 3358, 2973, 2611, 2283, 2000, 1753, 1544]
rms_vlow_chn4_c06 = [4012, 3591, 3179, 2795, 2444, 2136, 1874, 1648]
rms_vlow_chn5_c06 = [4071, 3655, 3252, 2875, 2529, 2227, 1966, 1742]
rms_vlow_chn6_c06 = [4090, 3684, 3293, 2927, 2591, 2297, 2041, 1819]
rms_vlow_chn7_c06 = [4278, 3860, 3455, 3079, 2735, 2433, 2166, 1935]
rms_vlow_chn8_c06 = [4630, 4158, 3709, 3297, 2923, 2595, 2311, 2064]

thrs_high_chn2_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 661, 586, 520, 461, 410, 365, 325, 290]
thrs_high_chn3_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1269, 1127, 1002, 892, 794, 708, 631, 562]
thrs_high_chn4_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1374, 1224, 1089, 971, 867, 773, 690, 615]
thrs_high_chn5_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1401, 1247, 1110, 987, 881, 784, 699, 623]
thrs_high_chn6_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1428, 1271, 1132, 1008, 899, 801, 714, 636]
thrs_high_chn7_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1328, 1184, 1055, 940, 840, 749, 668, 595]
thrs_high_chn8_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1073, 958, 855, 764, 682, 610, 544, 485]

thrs_vlow_chn2_c06 = thrs_vlow_dummy
thrs_vlow_chn3_c06 = thrs_vlow_dummy
thrs_vlow_chn4_c06 = thrs_vlow_dummy
thrs_vlow_chn5_c06 = thrs_vlow_dummy
thrs_vlow_chn6_c06 = thrs_vlow_dummy
thrs_vlow_chn7_c06 = thrs_vlow_dummy
thrs_vlow_chn8_c06 = thrs_vlow_dummy

gain_high_chn2_c06 = [120, 60, 256]
gain_vlow_chn2_c06 = [256, 128, 256]
gain_high_chn3_c06 = [120, 60, 256]
gain_vlow_chn3_c06 = [256, 128, 256]
gain_high_chn4_c06 = [120, 60, 256]
gain_vlow_chn4_c06 = [256, 128, 256]
gain_high_chn5_c06 = [120, 60, 256]
gain_vlow_chn5_c06 = [256, 128, 256]
gain_high_chn6_c06 = [120, 60, 256]
gain_vlow_chn6_c06 = [256, 128, 256]
gain_high_chn7_c06 = [120, 60, 256]
gain_vlow_chn7_c06 = [256, 128, 256]
gain_high_chn8_c06 = [120, 60, 256]
gain_vlow_chn8_c06 = [256, 128, 256]

rms_high_c06 = [rms_high_chn2_c06, rms_high_chn3_c06, rms_high_chn4_c06, rms_high_chn5_c06, rms_high_chn6_c06, rms_high_chn7_c06, rms_high_chn8_c06]
rms_vlow_c06 = [rms_vlow_chn2_c06, rms_vlow_chn3_c06, rms_vlow_chn4_c06, rms_vlow_chn5_c06, rms_vlow_chn6_c06, rms_vlow_chn7_c06, rms_vlow_chn8_c06]
thrs_high_c06 = [thrs_high_chn2_c06, thrs_high_chn3_c06, thrs_high_chn4_c06, thrs_high_chn5_c06, thrs_high_chn6_c06, thrs_high_chn7_c06, thrs_high_chn8_c06]
thrs_vlow_c06 = [thrs_vlow_chn2_c06, thrs_vlow_chn3_c06, thrs_vlow_chn4_c06, thrs_vlow_chn5_c06, thrs_vlow_chn6_c06, thrs_vlow_chn7_c06, thrs_vlow_chn8_c06]
gain_high_c06 = [gain_high_chn2_c06, gain_high_chn3_c06, gain_high_chn4_c06, gain_high_chn5_c06, gain_high_chn6_c06, gain_high_chn7_c06, gain_high_chn8_c06]
gain_vlow_c06 = [gain_vlow_chn2_c06, gain_vlow_chn3_c06, gain_vlow_chn4_c06, gain_vlow_chn5_c06, gain_vlow_chn6_c06, gain_vlow_chn7_c06, gain_vlow_chn8_c06]

rms_high_chn2_c11 = rms_high_chn2_c06
rms_high_chn3_c11 = rms_high_chn3_c06
rms_high_chn4_c11 = rms_high_chn4_c06
rms_high_chn5_c11 = rms_high_chn5_c06
rms_high_chn6_c11 = rms_high_chn6_c06
rms_high_chn7_c11 = rms_high_chn7_c06
rms_high_chn8_c11 = rms_high_chn8_c06

rms_vlow_chn2_c11 = rms_vlow_chn2_c06
rms_vlow_chn3_c11 = rms_vlow_chn3_c06
rms_vlow_chn4_c11 = rms_vlow_chn4_c06
rms_vlow_chn5_c11 = rms_vlow_chn5_c06
rms_vlow_chn6_c11 = rms_vlow_chn6_c06
rms_vlow_chn7_c11 = rms_vlow_chn7_c06
rms_vlow_chn8_c11 = rms_vlow_chn8_c06

thrs_high_chn2_c11 = thrs_high_dummy
thrs_high_chn3_c11 = thrs_high_dummy
thrs_high_chn4_c11 = thrs_high_dummy
thrs_high_chn5_c11 = thrs_high_dummy
thrs_high_chn6_c11 = thrs_high_dummy
thrs_high_chn7_c11 = thrs_high_dummy
thrs_high_chn8_c11 = thrs_high_dummy

thrs_vlow_chn2_c11 = thrs_vlow_dummy
thrs_vlow_chn3_c11 = thrs_vlow_dummy
thrs_vlow_chn4_c11 = thrs_vlow_dummy
thrs_vlow_chn5_c11 = thrs_vlow_dummy
thrs_vlow_chn6_c11 = thrs_vlow_dummy
thrs_vlow_chn7_c11 = thrs_vlow_dummy
thrs_vlow_chn8_c11 = thrs_vlow_dummy

gain_high_chn2_c11 = [120, 120, 120]
gain_high_chn3_c11 = [120, 120, 120]
gain_high_chn4_c11 = [120, 120, 120]
gain_high_chn5_c11 = [120, 120, 120]
gain_high_chn6_c11 = [120, 120, 120]
gain_high_chn7_c11 = [120, 120, 120]
gain_high_chn8_c11 = [120, 120, 120]

gain_vlow_chn2_c11 = gain_vlow_chn2_c06
gain_vlow_chn3_c11 = gain_vlow_chn3_c06
gain_vlow_chn4_c11 = gain_vlow_chn4_c06
gain_vlow_chn5_c11 = gain_vlow_chn5_c06
gain_vlow_chn6_c11 = gain_vlow_chn6_c06
gain_vlow_chn7_c11 = gain_vlow_chn7_c06
gain_vlow_chn8_c11 = gain_vlow_chn8_c06

rms_high_c11 = [rms_high_chn2_c11, rms_high_chn3_c11, rms_high_chn4_c11, rms_high_chn5_c11, rms_high_chn6_c11, rms_high_chn7_c11, rms_high_chn8_c11]
rms_vlow_c11 = [rms_vlow_chn2_c11, rms_vlow_chn3_c11, rms_vlow_chn4_c11, rms_vlow_chn5_c11, rms_vlow_chn6_c11, rms_vlow_chn7_c11, rms_vlow_chn8_c11]
thrs_high_c11 = [thrs_high_chn2_c11, thrs_high_chn3_c11, thrs_high_chn4_c11, thrs_high_chn5_c11, thrs_high_chn6_c11, thrs_high_chn7_c11, thrs_high_chn8_c11]
thrs_vlow_c11 = [thrs_vlow_chn2_c11, thrs_vlow_chn3_c11, thrs_vlow_chn4_c11, thrs_vlow_chn5_c11, thrs_vlow_chn6_c11, thrs_vlow_chn7_c11, thrs_vlow_chn8_c11]
gain_high_c11 = [gain_high_chn2_c11, gain_high_chn3_c11, gain_high_chn4_c11, gain_high_chn5_c11, gain_high_chn6_c11, gain_high_chn7_c11, gain_high_chn8_c11]
gain_vlow_c11 = [gain_vlow_chn2_c11, gain_vlow_chn3_c11, gain_vlow_chn4_c11, gain_vlow_chn5_c11, gain_vlow_chn6_c11, gain_vlow_chn7_c11, gain_vlow_chn8_c11]

rms_high_chn2_drv_sb = [792, 693, 607, 533, 468, 412, 362, 319]
rms_high_chn3_drv_sb = [728, 642, 566, 501, 443, 392, 347, 308]
rms_high_chn4_drv_sb = [1655, 1483, 1332, 1194, 1071, 961, 861, 772]
rms_high_chn5_drv_sb = [1801, 1609, 1438, 1285, 1149, 1027, 918, 820]
rms_high_chn6_drv_sb = [1233, 1099, 980, 874, 780, 695, 620, 553]
rms_high_chn7_drv_sb = [1411, 1253, 1114,991, 881, 784, 698, 621]
rms_high_chn8_drv_sb = [877, 778, 691, 614, 546, 487, 433, 385]

rms_vlow_chn2_drv_sb = [1627, 1473, 1319, 1168, 1030, 908, 800, 705]
rms_vlow_chn3_drv_sb = [2801, 2570, 2334, 2097, 1864, 1649, 1458, 1289]
rms_vlow_chn4_drv_sb = [3572, 3252, 2931, 2616, 2324, 2061, 1827, 1619]
rms_vlow_chn5_drv_sb = [3924, 3584, 3241, 2902, 2589, 2308, 2055, 1830]
rms_vlow_chn6_drv_sb = [3757, 3429, 3097, 2773, 2474, 2207, 1968, 1754]
rms_vlow_chn7_drv_sb = [3771, 3433, 3093, 2762, 2460, 2191, 1952, 1739]
rms_vlow_chn8_drv_sb = [4295, 3937, 3575, 3213, 2861, 2544, 2262, 2013]

thrs_high_chn2_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 670, 586, 514, 451, 396, 348, 306, 270]
thrs_high_chn3_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 620, 547, 482, 427, 378, 334, 296, 262]
thrs_high_chn4_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1407, 1263, 1133, 1016, 912, 818, 733, 657]
thrs_high_chn5_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1530, 1367, 1223, 1093, 976, 872, 780, 697]
thrs_high_chn6_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1048, 934, 833, 742, 663, 590, 527, 470]
thrs_high_chn7_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1200, 1065, 946, 842, 749, 666, 593, 527]
thrs_high_chn8_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 745, 660, 587, 521, 464, 413, 367, 326]

thrs_vlow_chn2_drv_sb = thrs_vlow_dummy
thrs_vlow_chn3_drv_sb = thrs_vlow_dummy
thrs_vlow_chn4_drv_sb = thrs_vlow_dummy
thrs_vlow_chn5_drv_sb = thrs_vlow_dummy
thrs_vlow_chn6_drv_sb = thrs_vlow_dummy
thrs_vlow_chn7_drv_sb = thrs_vlow_dummy
thrs_vlow_chn8_drv_sb = thrs_vlow_dummy

gain_high_chn2_drv_sb = [120, 30, 256]
gain_vlow_chn2_drv_sb = [256, 128, 287]
gain_high_chn3_drv_sb = [30, 30, 256]
gain_vlow_chn3_drv_sb = [287, 128, 287]
gain_high_chn4_drv_sb = [60, 30, 256]
gain_vlow_chn4_drv_sb = [256, 128, 287]
gain_high_chn5_drv_sb = [85, 30, 256]
gain_vlow_chn5_drv_sb = [256, 128, 287]
gain_high_chn6_drv_sb = [60, 30, 256]
gain_vlow_chn6_drv_sb = [256, 128, 287]
gain_high_chn7_drv_sb = [60, 30, 256]
gain_vlow_chn7_drv_sb = [256, 128, 287]
gain_high_chn8_drv_sb = [30, 30, 256]
gain_vlow_chn8_drv_sb = [287, 128, 287]

rms_high_drv_sb = [rms_high_chn2_drv_sb, rms_high_chn3_drv_sb, rms_high_chn4_drv_sb, rms_high_chn5_drv_sb, rms_high_chn6_drv_sb, rms_high_chn7_drv_sb, rms_high_chn8_drv_sb]
rms_vlow_drv_sb = [rms_vlow_chn2_drv_sb, rms_vlow_chn3_drv_sb, rms_vlow_chn4_drv_sb, rms_vlow_chn5_drv_sb, rms_vlow_chn6_drv_sb, rms_vlow_chn7_drv_sb, rms_vlow_chn8_drv_sb]
thrs_high_drv_sb = [thrs_high_chn2_drv_sb, thrs_high_chn3_drv_sb, thrs_high_chn4_drv_sb, thrs_high_chn5_drv_sb, thrs_high_chn6_drv_sb, thrs_high_chn7_drv_sb, thrs_high_chn8_drv_sb]
thrs_vlow_drv_sb = [thrs_vlow_chn2_drv_sb, thrs_vlow_chn3_drv_sb, thrs_vlow_chn4_drv_sb, thrs_vlow_chn5_drv_sb, thrs_vlow_chn6_drv_sb, thrs_vlow_chn7_drv_sb, thrs_vlow_chn8_drv_sb]
gain_high_drv_sb = [gain_high_chn2_drv_sb, gain_high_chn3_drv_sb, gain_high_chn4_drv_sb, gain_high_chn5_drv_sb, gain_high_chn6_drv_sb, gain_high_chn7_drv_sb, gain_high_chn8_drv_sb]
gain_vlow_drv_sb = [gain_vlow_chn2_drv_sb, gain_vlow_chn3_drv_sb, gain_vlow_chn4_drv_sb, gain_vlow_chn5_drv_sb, gain_vlow_chn6_drv_sb, gain_vlow_chn7_drv_sb, gain_vlow_chn8_drv_sb]

rms_high_chn2_drv_hiAtt = [674, 594, 523, 462, 408, 361, 320, 283]
rms_high_chn3_drv_hiAtt = [3068, 2677, 2341, 2049, 1796, 1574, 1381, 1214]
rms_high_chn4_drv_hiAtt = [3188, 2830, 2513, 2230, 1979, 1755, 1557, 1381]
rms_high_chn5_drv_hiAtt = [4094, 3646, 3255, 2912, 2603, 2331, 2085, 1869]
rms_high_chn6_drv_hiAtt = [4349, 3834, 3404, 3024, 2695, 2408, 2155, 1930]
rms_high_chn7_drv_hiAtt = [4038, 3616, 3239, 2907, 2617, 2359, 2132, 1929]
rms_high_chn8_drv_hiAtt = [2976, 2701, 2442, 2198, 1975, 1777, 1601, 1444]

rms_vlow_chn2_drv_hiAtt = [3755, 3462, 3164, 2855, 2560, 2274, 2024, 1793]
rms_vlow_chn3_drv_hiAtt = [4695, 4305, 3939, 3555, 3175, 2827, 2510, 2223]
rms_vlow_chn4_drv_hiAtt = [6668, 6121, 5610, 5069, 4539, 4041, 3587, 3181]
rms_vlow_chn5_drv_hiAtt = [3542, 3243, 2938, 2647, 2372, 2121, 1896, 1693]
rms_vlow_chn6_drv_hiAtt = [6986, 6406, 5797, 5224, 4690, 4193, 3754, 3357]
rms_vlow_chn7_drv_hiAtt = [7406, 6826, 6226, 5628, 5064, 4535, 4054, 3627]
rms_vlow_chn8_drv_hiAtt = [2935, 2678, 2418, 2173, 1945, 1739, 1554, 138]

thrs_high_chn2_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 609, 537, 473, 417, 369, 325, 288, 255]
thrs_high_chn3_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 2612, 2276, 1989, 1742, 1524, 1336, 1171, 1029]
thrs_high_chn4_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 2592, 2300, 2040, 1811, 1607, 1426, 1265, 1121]
thrs_high_chn5_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 3679, 3280, 2928, 2618, 2342, 2096, 1878, 1682]
thrs_high_chn6_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 3940, 3474, 3080, 2739, 2442, 2182, 1952, 1748]
thrs_high_chn7_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 3674, 3283, 2943, 2643, 2383, 2152, 1945, 1759]
thrs_high_chn8_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 2634, 2393, 2160, 1948, 1752, 1578, 1423, 1282]

thrs_vlow_chn2_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 7554, 6637, 5853, 5139, 4515, 3958, 3484, 3057]
thrs_vlow_chn3_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 21209, 18855, 16710, 14809, 13106, 11596, 10245, 9050]
thrs_vlow_chn4_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 17489, 15781, 14226, 12820, 11549, 10404, 9368, 8436]
thrs_vlow_chn5_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 4924, 4413, 3947, 3529, 3153, 2816, 2514, 2245]
thrs_vlow_chn6_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 12790, 11371, 10102, 8987, 7987, 7097, 6304, 5605]
thrs_vlow_chn7_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 19208, 17003, 15043, 13309, 11773, 10426, 9231, 8174]
thrs_vlow_chn8_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 8278, 7410, 6613, 5897, 5259, 4690, 4182, 3727]

gain_high_chn2_drv_hiAtt = [120, 60, 256]
gain_vlow_chn2_drv_hiAtt = [287, 128, 322]
gain_high_chn3_drv_hiAtt = [120, 60, 256]
gain_vlow_chn3_drv_hiAtt = [287, 128, 322]
gain_high_chn4_drv_hiAtt = [85, 40, 256]
gain_vlow_chn4_drv_hiAtt = [287, 128, 322]
gain_high_chn5_drv_hiAtt = [120, 60, 256]
gain_vlow_chn5_drv_hiAtt = [256, 128, 287]
gain_high_chn6_drv_hiAtt = [120, 60, 256]
gain_vlow_chn6_drv_hiAtt = [256, 128, 287]
gain_high_chn7_drv_hiAtt = [169, 80, 256]
gain_vlow_chn7_drv_hiAtt = [287, 128, 322]
gain_high_chn8_drv_hiAtt = [240, 120, 256]
gain_vlow_chn8_drv_hiAtt = [256, 128, 287]

rms_high_drv_hiAtt = [rms_high_chn2_drv_hiAtt, rms_high_chn3_drv_hiAtt, rms_high_chn4_drv_hiAtt, rms_high_chn5_drv_hiAtt, rms_high_chn6_drv_hiAtt, rms_high_chn7_drv_hiAtt, rms_high_chn8_drv_hiAtt]
rms_vlow_drv_hiAtt = [rms_vlow_chn2_drv_hiAtt, rms_vlow_chn3_drv_hiAtt, rms_vlow_chn4_drv_hiAtt, rms_vlow_chn5_drv_hiAtt, rms_vlow_chn6_drv_hiAtt, rms_vlow_chn7_drv_hiAtt, rms_vlow_chn8_drv_hiAtt]
thrs_high_drv_hiAtt = [thrs_high_chn2_drv_hiAtt, thrs_high_chn3_drv_hiAtt, thrs_high_chn4_drv_hiAtt, thrs_high_chn5_drv_hiAtt, thrs_high_chn6_drv_hiAtt, thrs_high_chn7_drv_hiAtt, thrs_high_chn8_drv_hiAtt]
thrs_vlow_drv_hiAtt = [thrs_vlow_chn2_drv_hiAtt, thrs_vlow_chn3_drv_hiAtt, thrs_vlow_chn4_drv_hiAtt, thrs_vlow_chn5_drv_hiAtt, thrs_vlow_chn6_drv_hiAtt, thrs_vlow_chn7_drv_hiAtt, thrs_vlow_chn8_drv_hiAtt]
gain_high_drv_hiAtt = [gain_high_chn2_drv_hiAtt, gain_high_chn3_drv_hiAtt, gain_high_chn4_drv_hiAtt, gain_high_chn5_drv_hiAtt, gain_high_chn6_drv_hiAtt, gain_high_chn7_drv_hiAtt, gain_high_chn8_drv_hiAtt]
gain_vlow_drv_hiAtt = [gain_vlow_chn2_drv_hiAtt, gain_vlow_chn3_drv_hiAtt, gain_vlow_chn4_drv_hiAtt, gain_vlow_chn5_drv_hiAtt, gain_vlow_chn6_drv_hiAtt, gain_vlow_chn7_drv_hiAtt, gain_vlow_chn8_drv_hiAtt]

rms_high_chn2_drv_mbsb = [2412, 2138, 1898, 1687, 1501, 1336, 1190, 1060]
rms_high_chn3_drv_mbsb = [669, 595, 530, 472, 420, 374, 333, 297]
rms_high_chn4_drv_mbsb = [1610, 1430, 1272, 1131, 1006, 895, 796, 708]
rms_high_chn5_drv_mbsb = [2967, 2624, 2328, 2068, 1838, 1634, 1451, 1290]
rms_high_chn6_drv_mbsb = [2461, 2176, 1943, 1735, 1554, 1391, 1244, 1111]
rms_high_chn7_drv_mbsb = [2121, 1888, 1693, 1526, 1375, 1241, 1120, 1010]
rms_high_chn8_drv_mbsb = [420, 379, 342, 308, 271, 239, 208, 199]

rms_vlow_chn2_drv_mbsb = [5439, 5028, 4603, 4169, 3734, 3323, 2947, 2609]
rms_vlow_chn3_drv_mbsb = [4292, 3930, 3559, 3190, 2842, 2522, 2235, 1978]
rms_vlow_chn4_drv_mbsb = [3667, 3361, 3045, 2732, 2440, 2172, 1930, 1711]
rms_vlow_chn5_drv_mbsb = [3813, 3519, 3218, 2916, 2615, 2337, 2086, 1860]
rms_vlow_chn6_drv_mbsb = [4051, 3760, 3418, 3078, 2757, 2483, 2199, 1961]
rms_vlow_chn7_drv_mbsb = [4370, 4022, 3681, 3333, 2986, 2669, 2403, 2152]
rms_vlow_chn8_drv_mbsb = [4734, 4402, 4056, 3709, 3354, 3002, 2679, 2388]

thrs_high_chn2_drv_mbsb = thrs_high_dummy
thrs_high_chn3_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 567, 505, 449, 400, 357, 317, 283, 252]
thrs_high_chn4_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 1368, 1215, 1080, 961, 855, 760, 676, 601]
thrs_high_chn5_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 2524, 2232, 1980, 1759, 1564, 1391, 1235, 1098]
thrs_high_chn6_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 2091, 1853, 1651, 1476, 1322, 1184, 1059, 946]
thrs_high_chn7_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 1801, 1603, 1439, 1297, 1170, 1055, 952, 858]
thrs_high_chn8_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 357, 323, 289, 260, 231, 206, 188, 168]

thrs_vlow_chn2_drv_mbsb = thrs_vlow_dummy
thrs_vlow_chn3_drv_mbsb = thrs_vlow_dummy
thrs_vlow_chn4_drv_mbsb = thrs_vlow_dummy
thrs_vlow_chn5_drv_mbsb = thrs_vlow_dummy
thrs_vlow_chn6_drv_mbsb = thrs_vlow_dummy
thrs_vlow_chn7_drv_mbsb = thrs_vlow_dummy
thrs_vlow_chn8_drv_mbsb = thrs_vlow_dummy

gain_high_chn2_drv_mbsb = [120, 120, 120]
gain_vlow_chn2_drv_mbsb = [287, 287, 287]
gain_high_chn3_drv_mbsb = [30, 15, 256]
gain_vlow_chn3_drv_mbsb = [256, 128, 287]
gain_high_chn4_drv_mbsb = [60, 30, 256]
gain_vlow_chn4_drv_mbsb = [256, 128, 287]
gain_high_chn5_drv_mbsb = [85, 30, 256]
gain_vlow_chn5_drv_mbsb = [287, 128, 287]
gain_high_chn6_drv_mbsb = [60, 30, 256]
gain_vlow_chn6_drv_mbsb = [287, 128, 287]
gain_high_chn7_drv_mbsb = [60, 30, 256]
gain_vlow_chn7_drv_mbsb = [287, 128, 287]
gain_high_chn8_drv_mbsb = [15, 8, 256]
gain_vlow_chn8_drv_mbsb = [322, 128, 322]

rms_high_drv_mbsb = [rms_high_chn2_drv_mbsb, rms_high_chn3_drv_mbsb, rms_high_chn4_drv_mbsb, rms_high_chn5_drv_mbsb, rms_high_chn6_drv_mbsb, rms_high_chn7_drv_mbsb, rms_high_chn8_drv_mbsb]
rms_vlow_drv_mbsb = [rms_vlow_chn2_drv_mbsb, rms_vlow_chn3_drv_mbsb, rms_vlow_chn4_drv_mbsb, rms_vlow_chn5_drv_mbsb, rms_vlow_chn6_drv_mbsb, rms_vlow_chn7_drv_mbsb, rms_vlow_chn8_drv_mbsb]
thrs_high_drv_mbsb = [thrs_high_chn2_drv_mbsb, thrs_high_chn3_drv_mbsb, thrs_high_chn4_drv_mbsb, thrs_high_chn5_drv_mbsb, thrs_high_chn6_drv_mbsb, thrs_high_chn7_drv_mbsb, thrs_high_chn8_drv_mbsb]
thrs_vlow_drv_mbsb = [thrs_vlow_chn2_drv_mbsb, thrs_vlow_chn3_drv_mbsb, thrs_vlow_chn4_drv_mbsb, thrs_vlow_chn5_drv_mbsb, thrs_vlow_chn6_drv_mbsb, thrs_vlow_chn7_drv_mbsb, thrs_vlow_chn8_drv_mbsb]
gain_high_drv_mbsb = [gain_high_chn2_drv_mbsb, gain_high_chn3_drv_mbsb, gain_high_chn4_drv_mbsb, gain_high_chn5_drv_mbsb, gain_high_chn6_drv_mbsb, gain_high_chn7_drv_mbsb, gain_high_chn8_drv_mbsb]
gain_vlow_drv_mbsb = [gain_vlow_chn2_drv_mbsb, gain_vlow_chn3_drv_mbsb, gain_vlow_chn4_drv_mbsb, gain_vlow_chn5_drv_mbsb, gain_vlow_chn6_drv_mbsb, gain_vlow_chn7_drv_mbsb, gain_vlow_chn8_drv_mbsb]

#### Double Channels ########################################################
dacc_2chn_fcc_c06      = [0x00000000, 0x10102120, 0x033F073F, 0x3F3F3F3F, 0x00000FFF, 0x00000000, 0xFFFF00FF, 0x17171717, \
					      0x10101010, 0x00001111, 0x04380006, 0x000003AA, 0xF0000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]

dacc_2chn_fcc_c11      = [0x00000000, 0x21202120, 0x073F073F, 0x3F3F3F3F, 0x00000FFF, 0x00000000, 0xFFFF00FF, 0x17171717, \
					      0x10101010, 0x00001111, 0x04380006, 0x00000355, 0x0F000000, 0x001020FF, 0x000003AA, 0xF0000000, 0x001020FF]

dacc_2chn_fcc_drv      = [0x00000000, 0x00000000, 0x00000100, 0x00000100, 0x00000000, 0x00000000, 0xFFFF00FF, 0x17171717, \
						  0x00000000, 0x00000000, 0x00000006, 0x00000355, 0x00000000, 0x001020F0, 0x00000355, 0x00000000, 0x001020FF]

# dacc_chn_fcc_drv_full = [0x00000000, 0x00002120, 0x0000073F, 0x00003F3F, 0x00000333, 0x00000000, 0xA92C00FF, 0x1A1A1A1A, \
# 				         0x00002020, 0x00000044, 0x0FD20005, 0x00000355, 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]  #TBD

rms_high_2chn12_c06 = [423, 374, 332, 295, 263, 234, 208, 185]
rms_high_2chn23_c06 = [922, 819, 729, 649, 578, 515, 459, 409]
rms_high_2chn34_c06 = [1223, 1090, 971, 866, 772, 688, 614, 547]
rms_high_2chn45_c06 = [1282, 1142, 1018, 908, 809, 722, 644, 574]
rms_high_2chn56_c06 = [1317, 1173, 1045, 930, 829, 739, 659, 588]
rms_high_2chn67_c06 = [1287, 1146, 1022, 910, 812, 724, 645, 575]
rms_high_2chn78_c06 = [1128, 1006, 897, 801, 715, 638, 570, 508]

rms_vlow_2chn12_c06 = [2054, 1832, 1617, 1422, 1251, 1102, 976, 867]
rms_vlow_2chn23_c06 = [2855, 2541, 2237, 1962, 1719, 1507, 1328, 1173]
rms_vlow_2chn34_c06 = [3002, 2674, 2355, 2065, 1811, 1587, 1397, 1235]
rms_vlow_2chn45_c06 = [3179, 2839, 2506, 2203, 1937, 1704, 1502, 1332]
rms_vlow_2chn56_c06 = [3214, 2881, 2554, 2258, 1997, 1767, 1569, 1399]
rms_vlow_2chn67_c06 = [3296, 2960, 2635, 2342, 2080, 1849, 1649, 1475]
rms_vlow_2chn78_c06 = [3494, 3138, 2793, 2481, 2210, 1967, 1756, 1569]

thrs_high_2chn12_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 360, 319, 284, 252, 224, 199, 177, 158]
thrs_high_2chn23_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 782, 695, 618, 550, 491, 437, 390, 347]
thrs_high_2chn34_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1038, 924, 822, 731, 652, 582, 518, 462]
thrs_high_2chn45_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1090, 971, 866, 770, 687, 612, 546, 485]
thrs_high_2chn56_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1119, 995, 887, 791, 704, 629, 560, 500]
thrs_high_2chn67_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 1089, 970, 865, 772, 689, 615, 549, 490]
thrs_high_2chn78_c06 = [0, 0, 0, 0, 0, 0, 0, 0, 954, 851, 759, 677, 604, 539, 481, 430]

thrs_vlow_2chn12_c06 = thrs_vlow_dummy
thrs_vlow_2chn23_c06 = thrs_vlow_dummy
thrs_vlow_2chn34_c06 = thrs_vlow_dummy
thrs_vlow_2chn45_c06 = thrs_vlow_dummy
thrs_vlow_2chn56_c06 = thrs_vlow_dummy
thrs_vlow_2chn67_c06 = thrs_vlow_dummy
thrs_vlow_2chn78_c06 = thrs_vlow_dummy

gain_high_2chn12_c06 = [120, 60, 256]
gain_vlow_2chn12_c06 = [256, 128, 256]
gain_high_2chn23_c06 = [120, 60, 256]
gain_vlow_2chn23_c06 = [256, 128, 256]
gain_high_2chn34_c06 = [120, 60, 256]
gain_vlow_2chn34_c06 = [256, 128, 256]
gain_high_2chn45_c06 = [120, 60, 256]
gain_vlow_2chn45_c06 = [256, 128, 256]
gain_high_2chn56_c06 = [120, 60, 256]
gain_vlow_2chn56_c06 = [256, 128, 256]
gain_high_2chn67_c06 = [120, 60, 256]
gain_vlow_2chn67_c06 = [256, 128, 256]
gain_high_2chn78_c06 = [120, 60, 256]
gain_vlow_2chn78_c06 = [256, 128, 256]

rms_high_2chn_c06 = [rms_high_2chn12_c06, rms_high_2chn23_c06, rms_high_2chn34_c06, rms_high_2chn45_c06, rms_high_2chn56_c06, rms_high_2chn67_c06, rms_high_2chn78_c06]
rms_vlow_2chn_c06 = [rms_vlow_2chn12_c06, rms_vlow_2chn23_c06, rms_vlow_2chn34_c06, rms_vlow_2chn45_c06, rms_vlow_2chn56_c06, rms_vlow_2chn67_c06, rms_vlow_2chn78_c06]
thrs_high_2chn_c06 = [thrs_high_2chn12_c06, thrs_high_2chn23_c06, thrs_high_2chn34_c06, thrs_high_2chn45_c06, thrs_high_2chn56_c06, thrs_high_2chn67_c06, thrs_high_2chn78_c06]
thrs_vlow_2chn_c06 = [thrs_vlow_2chn12_c06, thrs_vlow_2chn23_c06, thrs_vlow_2chn34_c06, thrs_vlow_2chn45_c06, thrs_vlow_2chn56_c06, thrs_vlow_2chn67_c06, thrs_vlow_2chn78_c06]
gain_high_2chn_c06 = [gain_high_2chn12_c06, gain_high_2chn23_c06, gain_high_2chn34_c06, gain_high_2chn45_c06, gain_high_2chn56_c06, gain_high_2chn67_c06, gain_high_2chn78_c06]
gain_vlow_2chn_c06 = [gain_vlow_2chn12_c06, gain_vlow_2chn23_c06, gain_vlow_2chn34_c06, gain_vlow_2chn45_c06, gain_vlow_2chn56_c06, gain_vlow_2chn67_c06, gain_vlow_2chn78_c06]

rms_high_2chn12_c11 = rms_high_2chn12_c06
rms_high_2chn23_c11 = rms_high_2chn23_c06
rms_high_2chn34_c11 = rms_high_2chn34_c06
rms_high_2chn45_c11 = rms_high_2chn45_c06
rms_high_2chn56_c11 = rms_high_2chn56_c06
rms_high_2chn67_c11 = rms_high_2chn67_c06
rms_high_2chn78_c11 = rms_high_2chn78_c06

rms_vlow_2chn12_c11 = rms_vlow_2chn12_c06
rms_vlow_2chn23_c11 = rms_vlow_2chn23_c06
rms_vlow_2chn34_c11 = rms_vlow_2chn34_c06
rms_vlow_2chn45_c11 = rms_vlow_2chn45_c06
rms_vlow_2chn56_c11 = rms_vlow_2chn56_c06
rms_vlow_2chn67_c11 = rms_vlow_2chn67_c06
rms_vlow_2chn78_c11 = rms_vlow_2chn78_c06

thrs_high_2chn12_c11 = thrs_high_dummy
thrs_high_2chn23_c11 = thrs_high_dummy
thrs_high_2chn34_c11 = thrs_high_dummy
thrs_high_2chn45_c11 = thrs_high_dummy
thrs_high_2chn56_c11 = thrs_high_dummy
thrs_high_2chn67_c11 = thrs_high_dummy
thrs_high_2chn78_c11 = thrs_high_dummy

thrs_vlow_2chn12_c11 = thrs_vlow_dummy
thrs_vlow_2chn23_c11 = thrs_vlow_dummy
thrs_vlow_2chn34_c11 = thrs_vlow_dummy
thrs_vlow_2chn45_c11 = thrs_vlow_dummy
thrs_vlow_2chn56_c11 = thrs_vlow_dummy
thrs_vlow_2chn67_c11 = thrs_vlow_dummy
thrs_vlow_2chn78_c11 = thrs_vlow_dummy

gain_high_2chn12_c11 = [120, 120, 120]
gain_high_2chn23_c11 = [120, 120, 120]
gain_high_2chn34_c11 = [120, 120, 120]
gain_high_2chn45_c11 = [120, 120, 120]
gain_high_2chn56_c11 = [120, 120, 120]
gain_high_2chn67_c11 = [120, 120, 120]
gain_high_2chn78_c11 = [120, 120, 120]

gain_vlow_2chn12_c11 = gain_vlow_2chn12_c06
gain_vlow_2chn23_c11 = gain_vlow_2chn23_c06
gain_vlow_2chn34_c11 = gain_vlow_2chn34_c06
gain_vlow_2chn45_c11 = gain_vlow_2chn45_c06
gain_vlow_2chn56_c11 = gain_vlow_2chn56_c06
gain_vlow_2chn67_c11 = gain_vlow_2chn67_c06
gain_vlow_2chn78_c11 = gain_vlow_2chn78_c06

rms_high_2chn_c11 = [rms_high_2chn12_c11, rms_high_2chn23_c11, rms_high_2chn34_c11, rms_high_2chn45_c11, rms_high_2chn56_c11, rms_high_2chn67_c11, rms_high_2chn78_c11]
rms_vlow_2chn_c11 = [rms_vlow_2chn12_c11, rms_vlow_2chn23_c11, rms_vlow_2chn34_c11, rms_vlow_2chn45_c11, rms_vlow_2chn56_c11, rms_vlow_2chn67_c11, rms_vlow_2chn78_c11]
thrs_high_2chn_c11 = [thrs_high_2chn12_c11, thrs_high_2chn23_c11, thrs_high_2chn34_c11, thrs_high_2chn45_c11, thrs_high_2chn56_c11, thrs_high_2chn67_c11, thrs_high_2chn78_c11]
thrs_vlow_2chn_c11 = [thrs_vlow_2chn12_c11, thrs_vlow_2chn23_c11, thrs_vlow_2chn34_c11, thrs_vlow_2chn45_c11, thrs_vlow_2chn56_c11, thrs_vlow_2chn67_c11, thrs_vlow_2chn78_c11]
gain_high_2chn_c11 = [gain_high_2chn12_c11, gain_high_2chn23_c11, gain_high_2chn34_c11, gain_high_2chn45_c11, gain_high_2chn56_c11, gain_high_2chn67_c11, gain_high_2chn78_c11]
gain_vlow_2chn_c11 = [gain_vlow_2chn12_c11, gain_vlow_2chn23_c11, gain_vlow_2chn34_c11, gain_vlow_2chn45_c11, gain_vlow_2chn56_c11, gain_vlow_2chn67_c11, gain_vlow_2chn78_c11]

rms_high_2chn12_drv_sb = [401, 353, 310, 273, 240, 212, 187, 165]
rms_high_2chn23_drv_sb = [1650, 1447, 1277, 1131, 1001, 887, 786, 696]
rms_high_2chn34_drv_sb = [2291, 2033, 1810, 1616, 1444, 1290, 1153, 1031]
rms_high_2chn45_drv_sb = [2136, 1912, 1712, 1535, 1375, 1232, 1104, 988]
rms_high_2chn56_drv_sb = [1841, 1641, 1465, 1308, 1167, 1042, 930, 830]
rms_high_2chn67_drv_sb = [1966, 1746, 1552, 1380, 1228, 1093, 972, 866]
rms_high_2chn78_drv_sb = [2398, 2121, 1882, 1672, 1487, 1323, 1177, 1046]

rms_vlow_2chn12_drv_sb = [850, 767, 685, 609, 539, 475, 420, 370]
rms_vlow_2chn23_drv_sb = [1566, 1413, 1263, 1123, 994, 878, 776, 686]
rms_vlow_2chn34_drv_sb = [2283, 2064, 1850, 1648, 1464, 1296, 1148, 1017]
rms_vlow_2chn45_drv_sb = [2768, 2509, 2255, 2017, 1798, 1598, 1420, 1262]
rms_vlow_2chn56_drv_sb = [2840, 2579, 2323, 2082, 1860, 1658, 1477, 1316]
rms_vlow_2chn67_drv_sb = [2773, 2517, 2266, 2031, 1814, 1617, 1441, 1284]
rms_vlow_2chn78_drv_sb = [2823, 2557, 2299, 2057, 1836, 1635, 1456, 1297]

thrs_high_2chn12_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 341, 300, 264, 232, 204, 181, 159, 140]
thrs_high_2chn23_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1402, 1231, 1086, 962, 850, 754, 669, 593]
thrs_high_2chn34_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1947, 1729, 1541, 1375, 1229, 1099, 981, 877]
thrs_high_2chn45_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1815, 1625, 1456, 1304, 1169, 1048, 939, 840]
thrs_high_2chn56_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1564, 1394, 1245, 1110, 991, 885, 790, 705]
thrs_high_2chn67_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1671, 1484, 1320, 1174, 1044, 929, 827, 736]
thrs_high_2chn78_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 2042, 1806, 1603, 1424, 1266, 1127, 1002, 891]

thrs_vlow_2chn12_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn23_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn34_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn45_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn56_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn67_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn78_drv_sb = thrs_vlow_dummy

gain_high_2chn12_drv_sb = [120, 60, 256]
gain_vlow_2chn12_drv_sb = [256, 128, 287]
gain_high_2chn23_drv_sb = [120, 60, 256]
gain_vlow_2chn23_drv_sb = [256, 128, 287]
gain_high_2chn34_drv_sb = [120, 60, 256]
gain_vlow_2chn34_drv_sb = [256, 128, 287]
gain_high_2chn45_drv_sb = [120, 60, 256]
gain_vlow_2chn45_drv_sb = [256, 128, 287]
gain_high_2chn56_drv_sb = [120, 60, 256]
gain_vlow_2chn56_drv_sb = [256, 128, 287]
gain_high_2chn67_drv_sb = [120, 60, 256]
gain_vlow_2chn67_drv_sb = [256, 128, 287]
gain_high_2chn78_drv_sb = [120, 60, 256]
gain_vlow_2chn78_drv_sb = [256, 128, 287]

rms_high_2chn_drv_sb = [rms_high_2chn12_drv_sb, rms_high_2chn23_drv_sb, rms_high_2chn34_drv_sb, rms_high_2chn45_drv_sb, rms_high_2chn56_drv_sb, rms_high_2chn67_drv_sb, rms_high_2chn78_drv_sb]
rms_vlow_2chn_drv_sb = [rms_vlow_2chn12_drv_sb, rms_vlow_2chn23_drv_sb, rms_vlow_2chn34_drv_sb, rms_vlow_2chn45_drv_sb, rms_vlow_2chn56_drv_sb, rms_vlow_2chn67_drv_sb, rms_vlow_2chn78_drv_sb]
thrs_high_2chn_drv_sb = [thrs_high_2chn12_drv_sb, thrs_high_2chn23_drv_sb, thrs_high_2chn34_drv_sb, thrs_high_2chn45_drv_sb, thrs_high_2chn56_drv_sb, thrs_high_2chn67_drv_sb, thrs_high_2chn78_drv_sb]
thrs_vlow_2chn_drv_sb = [thrs_vlow_2chn12_drv_sb, thrs_vlow_2chn23_drv_sb, thrs_vlow_2chn34_drv_sb, thrs_vlow_2chn45_drv_sb, thrs_vlow_2chn56_drv_sb, thrs_vlow_2chn67_drv_sb, thrs_vlow_2chn78_drv_sb]
gain_high_2chn_drv_sb = [gain_high_2chn12_drv_sb, gain_high_2chn23_drv_sb, gain_high_2chn34_drv_sb, gain_high_2chn45_drv_sb, gain_high_2chn56_drv_sb, gain_high_2chn67_drv_sb, gain_high_2chn78_drv_sb]
gain_vlow_2chn_drv_sb = [gain_vlow_2chn12_drv_sb, gain_vlow_2chn23_drv_sb, gain_vlow_2chn34_drv_sb, gain_vlow_2chn45_drv_sb, gain_vlow_2chn56_drv_sb, gain_vlow_2chn67_drv_sb, gain_vlow_2chn78_drv_sb]

rms_high_2chn12_drv_hiAtt = [331, 292, 258, 228, 202, 179, 159, 141]
rms_high_2chn23_drv_hiAtt = [1474, 1292, 1132, 994, 873, 768, 675, 594]
rms_high_2chn34_drv_hiAtt = [1673, 1486, 1321, 1174, 1043, 926, 822, 730]
rms_high_2chn45_drv_hiAtt = [1684, 1498, 1333, 1185, 1054, 937, 833, 741]
rms_high_2chn56_drv_hiAtt = [2164, 1930, 1722, 1542, 1381, 1238, 1108, 993]
rms_high_2chn67_drv_hiAtt = [2187, 1950, 1744, 1563, 1401, 1259, 1129, 1013]
rms_high_2chn78_drv_hiAtt = [2173, 1954, 1759, 1588, 1436, 1299, 1176, 1066]

rms_vlow_2chn12_drv_hiAtt = [1957, 1793, 1624, 1458, 1300, 1155, 1026, 909]
rms_vlow_2chn23_drv_hiAtt = [2475, 2259, 2045, 1837, 1637, 1454, 1289, 1143]
rms_vlow_2chn34_drv_hiAtt = [2581, 2357, 2135, 1914, 1706, 1513, 1340, 1186]
rms_vlow_2chn45_drv_hiAtt = [3554, 3256, 2949, 2649, 2364, 2099, 1859, 1649]
rms_vlow_2chn56_drv_hiAtt = [3685, 3348, 3026, 2719, 2434, 2177, 1947, 1740]
rms_vlow_2chn67_drv_hiAtt = [3888, 3577, 3254, 2939, 2636, 2360, 2110, 1886]
rms_vlow_2chn78_drv_hiAtt = [3848, 3534, 3216, 2896, 2601, 2328, 2081, 1860]

thrs_high_2chn12_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 297, 263, 232, 205, 182, 161, 143, 127]
thrs_high_2chn23_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 1256, 1100, 965, 846, 744, 653, 575, 505]
thrs_high_2chn34_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 1378, 1225, 1088, 967, 858, 763, 677, 601]
thrs_high_2chn45_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 1368, 1217, 1082, 962, 855, 761, 676, 601]
thrs_high_2chn56_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 1956, 1744, 1558, 1394, 1248, 1119, 1003, 897]
thrs_high_2chn67_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 1985, 1769, 1582, 1418, 1272, 1142, 1026, 920]
thrs_high_2chn78_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 1965, 1766, 1593, 1441, 1301, 1180, 1067, 969]

thrs_vlow_2chn12_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 3704, 3264, 2875, 2528, 2225, 1959, 1723, 1515]
thrs_vlow_2chn23_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 10548, 9351, 8276, 7323, 6471, 5721, 5048, 4451]
thrs_vlow_2chn34_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 10726, 9526, 8459, 7505, 6653, 5894, 5220, 4617]
thrs_vlow_2chn45_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 9423, 8502, 7666, 6902, 6216, 5595, 5033, 4533]
thrs_vlow_2chn56_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 6652, 5914, 5260, 4675, 4155, 3696, 3286, 2921]
thrs_vlow_2chn67_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 9830, 8704, 7703, 6819, 6038, 5348, 4739, 4200]
thrs_vlow_2chn78_drv_hiAtt = [0, 0, 0, 0, 0, 0, 0, 0, 11974, 10648, 9455, 8391, 7451, 6612, 5865, 5201]

gain_high_2chn12_drv_hiAtt = [120, 60, 256]
gain_vlow_2chn12_drv_hiAtt = [287, 128, 322]
gain_high_2chn23_drv_hiAtt = [120, 60, 256]
gain_vlow_2chn23_drv_hiAtt = [287, 128, 322]
gain_high_2chn34_drv_hiAtt = [85, 40, 256]
gain_vlow_2chn34_drv_hiAtt = [287, 128, 322]
gain_high_2chn45_drv_hiAtt = [85, 40, 256]
gain_vlow_2chn45_drv_hiAtt = [287, 128, 322]
gain_high_2chn56_drv_hiAtt = [120, 60, 256]
gain_vlow_2chn56_drv_hiAtt = [256, 128, 287]
gain_high_2chn67_drv_hiAtt = [120, 60, 256]
gain_vlow_2chn67_drv_hiAtt = [287, 128, 322]
gain_high_2chn78_drv_hiAtt = [169, 80, 256]
gain_vlow_2chn78_drv_hiAtt = [287, 128, 322]

rms_high_2chn_drv_hiAtt = [rms_high_2chn12_drv_hiAtt, rms_high_2chn23_drv_hiAtt, rms_high_2chn34_drv_hiAtt, rms_high_2chn45_drv_hiAtt, rms_high_2chn56_drv_hiAtt, rms_high_2chn67_drv_hiAtt, rms_high_2chn78_drv_hiAtt]
rms_vlow_2chn_drv_hiAtt = [rms_vlow_2chn12_drv_hiAtt, rms_vlow_2chn23_drv_hiAtt, rms_vlow_2chn34_drv_hiAtt, rms_vlow_2chn45_drv_hiAtt, rms_vlow_2chn56_drv_hiAtt, rms_vlow_2chn67_drv_hiAtt, rms_vlow_2chn78_drv_hiAtt]
thrs_high_2chn_drv_hiAtt = [thrs_high_2chn12_drv_hiAtt, thrs_high_2chn23_drv_hiAtt, thrs_high_2chn34_drv_hiAtt, thrs_high_2chn45_drv_hiAtt, thrs_high_2chn56_drv_hiAtt, thrs_high_2chn67_drv_hiAtt, thrs_high_2chn78_drv_hiAtt]
thrs_vlow_2chn_drv_hiAtt = [thrs_vlow_2chn12_drv_hiAtt, thrs_vlow_2chn23_drv_hiAtt, thrs_vlow_2chn34_drv_hiAtt, thrs_vlow_2chn45_drv_hiAtt, thrs_vlow_2chn56_drv_hiAtt, thrs_vlow_2chn67_drv_hiAtt, thrs_vlow_2chn78_drv_hiAtt]
gain_high_2chn_drv_hiAtt = [gain_high_2chn12_drv_hiAtt, gain_high_2chn23_drv_hiAtt, gain_high_2chn34_drv_hiAtt, gain_high_2chn45_drv_hiAtt, gain_high_2chn56_drv_hiAtt, gain_high_2chn67_drv_hiAtt, gain_high_2chn78_drv_hiAtt]
gain_vlow_2chn_drv_hiAtt = [gain_vlow_2chn12_drv_hiAtt, gain_vlow_2chn23_drv_hiAtt, gain_vlow_2chn34_drv_hiAtt, gain_vlow_2chn45_drv_hiAtt, gain_vlow_2chn56_drv_hiAtt, gain_vlow_2chn67_drv_hiAtt, gain_vlow_2chn78_drv_hiAtt]

rms_high_2chn12_drv_mbsb = [592, 526, 468, 417, 370, 330, 293, 261]
rms_high_2chn23_drv_mbsb = [391, 348, 309, 275, 245, 218, 194, 173]
rms_high_2chn34_drv_mbsb = [453, 403, 359, 319, 284, 253, 225, 200]
rms_high_2chn45_drv_mbsb = [1140, 1012, 899, 799, 710, 631, 561, 499]
rms_high_2chn56_drv_mbsb = [1419, 1261, 1123, 1000, 891, 792, 704, 627]
rms_high_2chn67_drv_mbsb = [1445, 1291, 1158, 1039, 931, 833, 746, 666]
rms_high_2chn78_drv_mbsb = [391, 350, 316, 284, 255, 229, 208, 184]

rms_vlow_2chn12_drv_mbsb = [3790, 3452, 3119, 2794, 2488, 2209, 1955, 1729]
rms_vlow_2chn23_drv_mbsb = [3242, 2956, 2670, 2391, 2130, 1890, 1673, 1481]
rms_vlow_2chn34_drv_mbsb = [2554, 2308, 2071, 1849, 1643, 1457, 1291, 1144]
rms_vlow_2chn45_drv_mbsb = [2484, 2270, 2056, 1847, 1650, 1469, 1305, 1158]
rms_vlow_2chn56_drv_mbsb = [2435, 2227, 2018, 1814, 1622, 1446, 1285, 1142]
rms_vlow_2chn67_drv_mbsb = [2661, 2426, 2196, 1977, 1770, 1580, 1407, 1254]
rms_vlow_2chn78_drv_mbsb = [2754, 2517, 2281, 2051, 1836, 1638, 1459, 1299]

thrs_high_2chn12_drv_mbsb = thrs_high_dummy
thrs_high_2chn23_drv_mbsb = thrs_high_dummy
thrs_high_2chn34_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 385, 342, 305, 271, 241, 215, 191, 170]
thrs_high_2chn45_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 969, 861, 765, 680, 604, 537, 477, 425]
thrs_high_2chn56_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 1206, 1072, 955, 850, 757, 674, 600, 533]
thrs_high_2chn67_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 1231, 1100, 986, 884, 793, 710, 634, 567]
thrs_high_2chn78_drv_mbsb = [0, 0, 0, 0, 0, 0, 0, 0, 333, 298, 269, 242, 217, 196, 177, 158]

thrs_vlow_2chn12_drv_mbsb = thrs_vlow_dummy
thrs_vlow_2chn23_drv_mbsb = thrs_vlow_dummy
thrs_vlow_2chn34_drv_mbsb = thrs_vlow_dummy
thrs_vlow_2chn45_drv_mbsb = thrs_vlow_dummy
thrs_vlow_2chn56_drv_mbsb = thrs_vlow_dummy
thrs_vlow_2chn67_drv_mbsb = thrs_vlow_dummy
thrs_vlow_2chn78_drv_mbsb = thrs_vlow_dummy

gain_high_2chn12_drv_mbsb = [51, 51, 51]
gain_vlow_2chn12_drv_mbsb = [287, 287, 287]
gain_high_2chn23_drv_mbsb = [30, 30, 30]
gain_vlow_2chn23_drv_mbsb = [287, 287, 287]
gain_high_2chn34_drv_mbsb = [30, 15, 256]
gain_vlow_2chn34_drv_mbsb = [256, 128, 287]
gain_high_2chn45_drv_mbsb = [60, 30, 256]
gain_vlow_2chn45_drv_mbsb = [287, 128, 287]
gain_high_2chn56_drv_mbsb = [60, 30, 256]
gain_vlow_2chn56_drv_mbsb = [287, 128, 287]
gain_high_2chn67_drv_mbsb = [60, 30, 256]
gain_vlow_2chn67_drv_mbsb = [287, 128, 287]
gain_high_2chn78_drv_mbsb = [18, 8, 256]
gain_vlow_2chn78_drv_mbsb = [287, 128, 287]

rms_high_2chn_drv_mbsb = [rms_high_2chn12_drv_mbsb, rms_high_2chn23_drv_mbsb, rms_high_2chn34_drv_mbsb, rms_high_2chn45_drv_mbsb, rms_high_2chn56_drv_mbsb, rms_high_2chn67_drv_mbsb, rms_high_2chn78_drv_mbsb]
rms_vlow_2chn_drv_mbsb = [rms_vlow_2chn12_drv_mbsb, rms_vlow_2chn23_drv_mbsb, rms_vlow_2chn34_drv_mbsb, rms_vlow_2chn45_drv_mbsb, rms_vlow_2chn56_drv_mbsb, rms_vlow_2chn67_drv_mbsb, rms_vlow_2chn78_drv_mbsb]
thrs_high_2chn_drv_mbsb = [thrs_high_2chn12_drv_mbsb, thrs_high_2chn23_drv_mbsb, thrs_high_2chn34_drv_mbsb, thrs_high_2chn45_drv_mbsb, thrs_high_2chn56_drv_mbsb, thrs_high_2chn67_drv_mbsb, thrs_high_2chn78_drv_mbsb]
thrs_vlow_2chn_drv_mbsb = [thrs_vlow_2chn12_drv_mbsb, thrs_vlow_2chn23_drv_mbsb, thrs_vlow_2chn34_drv_mbsb, thrs_vlow_2chn45_drv_mbsb, thrs_vlow_2chn56_drv_mbsb, thrs_vlow_2chn67_drv_mbsb, thrs_vlow_2chn78_drv_mbsb]
gain_high_2chn_drv_mbsb = [gain_high_2chn12_drv_mbsb, gain_high_2chn23_drv_mbsb, gain_high_2chn34_drv_mbsb, gain_high_2chn45_drv_mbsb, gain_high_2chn56_drv_mbsb, gain_high_2chn67_drv_mbsb, gain_high_2chn78_drv_mbsb]
gain_vlow_2chn_drv_mbsb = [gain_vlow_2chn12_drv_mbsb, gain_vlow_2chn23_drv_mbsb, gain_vlow_2chn34_drv_mbsb, gain_vlow_2chn45_drv_mbsb, gain_vlow_2chn56_drv_mbsb, gain_vlow_2chn67_drv_mbsb, gain_vlow_2chn78_drv_mbsb]

############################################################################
#### Meters & More Coupling Parameters ####
############################################################################

# PLCOUP007
mm_rms_high_c07  = [324, 293, 267, 242, 218, 194, 172, 153]
mm_rms_vlow_c07  = [2432, 2172, 1932, 1714, 1515, 1342, 1189, 1052]
mm_thrs_high_c07 = [0, 0, 0, 0, 0, 0, 0, 0, 274, 247, 225, 205, 184, 164, 146, 130]
mm_thrs_vlow_c07 = [0, 0, 0, 0, 0, 0, 0, 0, 5579, 4972, 4414, 3920, 3477, 3079, 2738, 2433]
mm_gain_high_c07 = [25, 12, 50]
mm_gain_vlow_c07 = [228, 114, 256]
mm_dacc_c07      = [0, 0x00002120, 0x0000073F, 0x00003F3F, 0x00000333, 0, 0xFFFF00FF, 0x17171717, 0x00002020, 0x00000044, 0x0FD20001, 0x00000355, 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]
mm_drv_conf_c07  = 0

# PL460 Auxiliary Branch
mm_dacc_aux      = [0, 0x21200000, 0x073F0000, 0x3F3F0000, 0x00000CCC, 0, 0xFFFF00FF, 0x17171717, 0x20200000, 0x00004400, 0x0FD20001, 0x000003AA, 0xF0000000, 0x001020F0, 0x000003AA, 0xF0000000, 0x001020FF]
mm_drv_conf_aux  = 8

# PL460 CEN-A Single Branch
mm_rms_high_drv_sb  = [614, 548, 491, 438, 391, 348, 311, 278]
mm_rms_vlow_drv_sb  = [3979, 3608, 3263, 2938, 2636, 2359, 2109, 1880]
mm_thrs_high_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 511, 458, 411, 367, 328, 292, 262, 235]
mm_thrs_vlow_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 5815, 5364, 4862, 4399, 3982, 3606, 3264, 2949]
mm_gain_high_drv_sb = [22, 11, 44]
mm_gain_vlow_drv_sb = [256, 128, 256]
mm_dacc_drv_sb      = [0, 0, 0x00000100, 0x00000100, 0, 0, 0xFFFF00FF, 0x17171717, 0, 0, 0x00000001, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
mm_drv_conf_drv_sb  = 5

# PL460 Multiband Single Branch
mm_rms_high_drv_mbsb  = [0, 0, 0, 0, 0, 0, 0, 0]
mm_rms_vlow_drv_mbsb  = [0, 0, 0, 0, 0, 0, 0, 0]
mm_thrs_high_drv_mbsb = thrs_vlow_dummy
mm_thrs_vlow_drv_mbsb = thrs_high_dummy
mm_gain_high_drv_mbsb = [16, 16, 16]
mm_gain_vlow_drv_mbsb = [16, 16, 16]
mm_dacc_drv_mbsb      = [0, 0, 0x00000100, 0x00000100, 0, 0, 0xFFFF00FF, 0x15151515, 0, 0, 0x00000006, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020F0]
mm_drv_conf_drv_mbsb  = 0xA7

############################################################################

srv_pcoup_helpkeyword = "mcc_h3_srv_pcoup_configurations"

############################################################################
# Update PLC Profile according to PLC PHY driver configuration
############################################################################
def updateCouplingParameters():

    plcProfile = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PROFILE")

    if plcProfile == "PRIME":
        updatePRIMECouplingParameters()
    elif plcProfile == "Meters&More":
        updateMMCouplingParameters()
    else:
        updateG3CouplingParameters()
        

############################################################################
#### G3 function to update the Coupling Parameters ####
############################################################################

def updateG3CouplingParameters():
    global pCoupG3AuxBranch
    global pCoupG3MainPhyBand
    global pCoupG3AuxPhyBand
    global pCoupG3SourceFile
    global pCoupG3HeaderFile
    global pCoupPRIMESourceFile
    global pCoupPRIMEHeaderFile
    global pCoupPRIMETXChannels
    global pCoupG3TXBranches

    plcDevice = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_MODE")
    if plcDevice != None:
        plcDriver = "drvPlcPhy"
    else:
        plcDevice = Database.getSymbolValue("drvG3MacRt", "DRV_PLC_MODE")
        if plcDevice != None:
            plcDriver = "drvG3MacRt"
        else:
            plcDriver = ""

    if (plcDevice == "PL460"):
        g3_coupSettings = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_G3_SETTING_PL460")
    else:
        g3_coupSettings = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_G3_SETTING_PL360")
    
    if g3_coupSettings == None:
        print("updateG3CouplingParameters: DRV PLC Coupling Settings not found")
        return

    # Show G3 setting, hide PRIME setting
    pCoupG3TXBranches.setVisible(True)
    pCoupPRIMETXChannels.setVisible(False)

    # Enable G3 COUP files
    pCoupG3SourceFile.setEnabled(True)
    pCoupG3HeaderFile.setEnabled(True)
    pCoupPRIMESourceFile.setEnabled(False)
    pCoupPRIMEHeaderFile.setEnabled(False)
    pCoupMMSourceFile.setEnabled(False)
    pCoupMMHeaderFile.setEnabled(False)

    pCoupG3MainPhyBand.setLabel("G3-PLC PHY Band")
    pCoupG3MainPhyBand.setVisible(True)

    pCoupG3MainBranch.setVisible(False)
    mainBand = True
    pCoupG3MainPhyBand.setValue("None")

    pCoupG3AuxBranch.setVisible(False)
    auxiliaryBand = False
    pCoupG3AuxPhyBand.setValue("None")

    if (plcDevice == "PL460"):
        if (g3_coupSettings == "CEN-A (CENELEC-A only; main branch)"):
            # print("UpdatePlcCouplingParameters -> PL460 G3 CEN-A")
            rms_high  = rms_high_cena
            rms_vlow  = rms_vlow_cena
            thrs_high = thrs_high_cena
            thrs_vlow = thrs_vlow_cena
            dacc      = dacc_cena
            gain_high = gain_high_cena
            gain_vlow = gain_vlow_cena
            line_drv  = drv_conf_cena
            pCoupG3MainPhyBand.setValue("CEN-A")
            pCoupG3MainBranch.setLabel("Main Transmission branch")

        elif (g3_coupSettings == "CEN-B (CENELEC-B only; main branch)"):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 CEN-B")
            rms_high  = rms_high_cenb
            rms_vlow  = rms_vlow_cenb
            thrs_high = thrs_high_cenb
            thrs_vlow = thrs_vlow_cenb
            dacc      = dacc_cenb
            gain_high = gain_high_cenb
            gain_vlow = gain_vlow_cenb
            line_drv  = drv_conf_cenb
            pCoupG3MainPhyBand.setValue("CEN-B")
            pCoupG3MainBranch.setLabel("Main Transmission branch")
        
        elif ("FCC default (FCC only" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB")
            rms_high  = rms_high_fcc
            rms_vlow  = rms_vlow_fcc
            thrs_high = thrs_high_fcc
            thrs_vlow = thrs_vlow_fcc
            dacc      = dacc_fcc
            gain_high = gain_high_fcc
            gain_vlow = gain_vlow_fcc
            line_drv  = drv_conf_fcc
            pCoupG3MainPhyBand.setValue("FCC")
            pCoupG3MainBranch.setLabel("Main Transmission branch")
        
        elif ("FCC high attenuation (FCC only" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB HIGHT ATT")
            rms_high  = rms_high_fcc_hiAtt
            rms_vlow  = rms_vlow_fcc_hiAtt
            thrs_high = thrs_high_fcc_hiAtt
            thrs_vlow = thrs_vlow_fcc_hiAtt
            dacc      = dacc_fcc_hiAtt
            gain_high = gain_high_fcc_hiAtt
            gain_vlow = gain_vlow_fcc_hiAtt
            line_drv  = drv_conf_fcc_hiAtt
            pCoupG3MainPhyBand.setValue("FCC")
            pCoupG3MainBranch.setLabel("Main Transmission branch")
        
        elif ("FCC default & CEN-A (FCC + CENELEC-A" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB")
            rms_high  = rms_high_fcc
            rms_vlow  = rms_vlow_fcc
            thrs_high = thrs_high_fcc
            thrs_vlow = thrs_vlow_fcc
            dacc      = dacc_fcc
            gain_high = gain_high_fcc
            gain_vlow = gain_vlow_fcc
            line_drv  = drv_conf_fcc
            pCoupG3MainPhyBand.setValue("FCC")
            pCoupG3MainBranch.setLabel("Main Transmission branch")

            # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-A")
            auxiliaryBand = True
            rms_high_aux  = rms_high_cena_c07
            rms_vlow_aux  = rms_vlow_cena_c07
            thrs_high_aux = thrs_high_cena_c07
            thrs_vlow_aux = thrs_vlow_cena_c07
            dacc_aux      = dacc_cena_c07_drv
            gain_high_aux = gain_high_cena_c07
            gain_vlow_aux = gain_vlow_cena_c07
            line_drv_aux  = drv_conf_cena_c07
            pCoupG3AuxPhyBand.setValue("CEN-A")
            pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")
        
        elif ("FCC default & CEN-A (CENELEC-A only" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-A")
            auxiliaryBand = True
            mainBand = False
            rms_high_aux  = rms_high_cena_c07
            rms_vlow_aux  = rms_vlow_cena_c07
            thrs_high_aux = thrs_high_cena_c07
            thrs_vlow_aux = thrs_vlow_cena_c07
            dacc_aux      = dacc_cena_c07_drv
            gain_high_aux = gain_high_cena_c07
            gain_vlow_aux = gain_vlow_cena_c07
            line_drv_aux  = drv_conf_cena_c07
            pCoupG3AuxPhyBand.setValue("CEN-A")
            pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")

        elif ("FCC default & CEN-B (FCC + CENELEC-B" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB")
            rms_high  = rms_high_fcc
            rms_vlow  = rms_vlow_fcc
            thrs_high = thrs_high_fcc
            thrs_vlow = thrs_vlow_fcc
            dacc      = dacc_fcc
            gain_high = gain_high_fcc
            gain_vlow = gain_vlow_fcc
            line_drv  = drv_conf_fcc
            pCoupG3MainPhyBand.setValue("FCC")
            pCoupG3MainBranch.setLabel("Main Transmission branch")

            # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-B")
            auxiliaryBand = True
            rms_high_aux  = rms_high_cenb_c14
            rms_vlow_aux  = rms_vlow_cenb_c14
            thrs_high_aux = thrs_high_cenb_c14
            thrs_vlow_aux = thrs_vlow_cenb_c14
            dacc_aux      = dacc_cenb_c14_drv
            gain_high_aux = gain_high_cenb_c14
            gain_vlow_aux = gain_vlow_cenb_c14
            line_drv_aux  = drv_conf_cenb_c14
            pCoupG3AuxPhyBand.setValue("CEN-B")
            pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")
        
        elif ("FCC default & CEN-B (CENELEC-B only" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-B")
            auxiliaryBand = True
            mainBand = False
            rms_high_aux  = rms_high_cenb_c14
            rms_vlow_aux  = rms_vlow_cenb_c14
            thrs_high_aux = thrs_high_cenb_c14
            thrs_vlow_aux = thrs_vlow_cenb_c14
            dacc_aux      = dacc_cenb_c14_drv
            gain_high_aux = gain_high_cenb_c14
            gain_vlow_aux = gain_vlow_cenb_c14
            line_drv_aux  = drv_conf_cenb_c14
            pCoupG3AuxPhyBand.setValue("CEN-B")
            pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")

        elif ("FCC high attenuation & CEN-A (FCC + CENELEC-A" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB HIGHT ATT")
            rms_high  = rms_high_fcc_hiAtt
            rms_vlow  = rms_vlow_fcc_hiAtt
            thrs_high = thrs_high_fcc_hiAtt
            thrs_vlow = thrs_vlow_fcc_hiAtt
            dacc      = dacc_fcc_hiAtt
            gain_high = gain_high_fcc_hiAtt
            gain_vlow = gain_vlow_fcc_hiAtt
            line_drv  = drv_conf_fcc_hiAtt
            pCoupG3MainPhyBand.setValue("FCC")
            pCoupG3MainBranch.setLabel("Main Transmission branch")

            # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-A")
            auxiliaryBand = True
            rms_high_aux  = rms_high_cena_c07
            rms_vlow_aux  = rms_vlow_cena_c07
            thrs_high_aux = thrs_high_cena_c07
            thrs_vlow_aux = thrs_vlow_cena_c07
            dacc_aux      = dacc_cena_c07_drv
            gain_high_aux = gain_high_cena_c07
            gain_vlow_aux = gain_vlow_cena_c07
            line_drv_aux  = drv_conf_cena_c07
            pCoupG3AuxPhyBand.setValue("CEN-A")
            pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")
        
        elif ("FCC high attenuation & CEN-A (CENELEC-A only" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-A")
            auxiliaryBand = True
            mainBand = False
            rms_high_aux  = rms_high_cena_c07
            rms_vlow_aux  = rms_vlow_cena_c07
            thrs_high_aux = thrs_high_cena_c07
            thrs_vlow_aux = thrs_vlow_cena_c07
            dacc_aux      = dacc_cena_c07_drv
            gain_high_aux = gain_high_cena_c07
            gain_vlow_aux = gain_vlow_cena_c07
            line_drv_aux  = drv_conf_cena_c07
            pCoupG3AuxPhyBand.setValue("CEN-A")
            pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")

        elif ("FCC high attenuation & CEN-B (FCC + CENELEC-B" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB HIGHT ATT")
            rms_high  = rms_high_fcc_hiAtt
            rms_vlow  = rms_vlow_fcc_hiAtt
            thrs_high = thrs_high_fcc_hiAtt
            thrs_vlow = thrs_vlow_fcc_hiAtt
            dacc      = dacc_fcc_hiAtt
            gain_high = gain_high_fcc_hiAtt
            gain_vlow = gain_vlow_fcc_hiAtt
            line_drv  = drv_conf_fcc_hiAtt
            pCoupG3MainPhyBand.setValue("FCC")
            pCoupG3MainBranch.setLabel("Main Transmission branch")

            # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-B")
            auxiliaryBand = True
            rms_high_aux  = rms_high_cenb_c14
            rms_vlow_aux  = rms_vlow_cenb_c14
            thrs_high_aux = thrs_high_cenb_c14
            thrs_vlow_aux = thrs_vlow_cenb_c14
            dacc_aux      = dacc_cenb_c14_drv
            gain_high_aux = gain_high_cenb_c14
            gain_vlow_aux = gain_vlow_cenb_c14
            line_drv_aux  = drv_conf_cenb_c14
            pCoupG3AuxPhyBand.setValue("CEN-B")
            pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")
        
        elif ("FCC high attenuation & CEN-B (CENELEC-B only" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-B")
            auxiliaryBand = True
            mainBand = False
            rms_high_aux  = rms_high_cenb_c14
            rms_vlow_aux  = rms_vlow_cenb_c14
            thrs_high_aux = thrs_high_cenb_c14
            thrs_vlow_aux = thrs_vlow_cenb_c14
            dacc_aux      = dacc_cenb_c14_drv
            gain_high_aux = gain_high_cenb_c14
            gain_vlow_aux = gain_vlow_cenb_c14
            line_drv_aux  = drv_conf_cenb_c14
            pCoupG3AuxPhyBand.setValue("CEN-B")
            pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")

        elif ("Multiband single-branch FCC & CEN-A (FCC + CENELEC-A" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC MULTIBAND SINGLE-BRANCH")
            rms_high  = rms_high_mbsb_fcc
            rms_vlow  = rms_vlow_mbsb_fcc
            thrs_high = thrs_high_mbsb_fcc
            thrs_vlow = thrs_vlow_mbsb_fcc
            dacc      = dacc_mbsb_fcc
            gain_high = gain_high_mbsb_fcc
            gain_vlow = gain_vlow_mbsb_fcc
            line_drv  = drv_conf_mbsb_fcc
            pCoupG3MainPhyBand.setValue("FCC")
            pCoupG3MainBranch.setLabel("Main Transmission branch (ASO0)")

            # print("UpdatePlcCouplingParameters ->  PL460 G3 CEN-A MULTIBAND SINGLE-BRANCH")
            auxiliaryBand = True
            rms_high_aux  = rms_high_mbsb_cena
            rms_vlow_aux  = rms_vlow_mbsb_cena
            thrs_high_aux = thrs_high_mbsb_cena
            thrs_vlow_aux = thrs_vlow_mbsb_cena
            dacc_aux      = dacc_mbsb_cena
            gain_high_aux = gain_high_mbsb_cena
            gain_vlow_aux = gain_vlow_mbsb_cena
            line_drv_aux  = drv_conf_mbsb_cena
            pCoupG3AuxPhyBand.setValue("CEN-A")
            pCoupG3AuxBranch.setLabel("Main Transmission branch (ASO1)")

        elif ("Multiband single-branch FCC & CEN-A (FCC only" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC MULTIBAND SINGLE-BRANCH")
            rms_high  = rms_high_mbsb_fcc
            rms_vlow  = rms_vlow_mbsb_fcc
            thrs_high = thrs_high_mbsb_fcc
            thrs_vlow = thrs_vlow_mbsb_fcc
            dacc      = dacc_mbsb_fcc
            gain_high = gain_high_mbsb_fcc
            gain_vlow = gain_vlow_mbsb_fcc
            line_drv  = drv_conf_mbsb_fcc
            pCoupG3MainPhyBand.setValue("FCC")
            pCoupG3MainBranch.setLabel("Main Transmission branch (ASO0)")
        
        elif ("Multiband single-branch FCC & CEN-A (CENELEC-A only" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters ->  PL460 G3 CEN-A MULTIBAND SINGLE-BRANCH")
            auxiliaryBand = True
            mainBand = False
            rms_high_aux  = rms_high_mbsb_cena
            rms_vlow_aux  = rms_vlow_mbsb_cena
            thrs_high_aux = thrs_high_mbsb_cena
            thrs_vlow_aux = thrs_vlow_mbsb_cena
            dacc_aux      = dacc_mbsb_cena
            gain_high_aux = gain_high_mbsb_cena
            gain_vlow_aux = gain_vlow_mbsb_cena
            line_drv_aux  = drv_conf_mbsb_cena
            pCoupG3AuxPhyBand.setValue("CEN-A")
            pCoupG3AuxBranch.setLabel("Main Transmission branch (ASO1)")

        else: # "CEN-A" as default configuration
            # print("UpdatePlcCouplingParameters -> PL460 G3 CEN-A")
            rms_high  = rms_high_cena
            rms_vlow  = rms_vlow_cena
            thrs_high = thrs_high_cena
            thrs_vlow = thrs_vlow_cena
            dacc      = dacc_cena
            gain_high = gain_high_cena
            gain_vlow = gain_vlow_cena
            line_drv  = drv_conf_cena
            pCoupG3MainPhyBand.setValue("CEN-A")
            pCoupG3MainBranch.setLabel("Main Transmission branch")

    else: # "PL360"
        line_drv = 0
        line_drv_aux = 0
        if ("PLCOUP007" in g3_coupSettings):
            # print("UpdatePlcCouplingParameters -> PL360 G3 CEN-A")
            rms_high  = rms_high_cena_c07
            rms_vlow  = rms_vlow_cena_c07
            thrs_high = thrs_high_cena_c07
            thrs_vlow = thrs_vlow_cena_c07
            dacc      = dacc_cena_c07
            gain_high = gain_high_cena_c07
            gain_vlow = gain_vlow_cena_c07
            pCoupG3MainPhyBand.setValue("CEN-A")
            pCoupG3MainBranch.setLabel("Transmission branch 0")

        elif ("PLCOUP014" in g3_coupSettings):
            pCoupG3MainPhyBand.setValue("CEN-B")
            # print("UpdatePlcCouplingParameters ->  PL360 G3 CEN-B External")
            rms_high  = rms_high_cenb_c14
            rms_vlow  = rms_vlow_cenb_c14
            thrs_high = thrs_high_cenb_c14
            thrs_vlow = thrs_vlow_cenb_c14
            dacc      = dacc_cenb_c14
            gain_high = gain_high_cenb_c14
            gain_vlow = gain_vlow_cenb_c14
            pCoupG3MainBranch.setLabel("Transmission branch 0")

        elif ("PLCOUP012" in g3_coupSettings):
            pCoupG3MainPhyBand.setValue("CEN-B")
            # print("UpdatePlcCouplingParameters ->  PL360 G3 CEN-B Internal")
            rms_high  = rms_high_cenb_c12
            rms_vlow  = rms_vlow_cenb_c12
            thrs_high = thrs_high_cenb_c12
            thrs_vlow = thrs_vlow_cenb_c12
            dacc      = dacc_cenb_c12
            gain_high = gain_high_cenb_c12
            gain_vlow = gain_vlow_cenb_c12
            pCoupG3MainBranch.setLabel("Transmission branch 0")
        
        elif ("PLCOUP006" in g3_coupSettings):
            pCoupG3MainPhyBand.setValue("FCC")
            # print("UpdatePlcCouplingParameters ->  PL360 G3 FCC 2B")
            rms_high  = rms_high_fcc_c06
            rms_vlow  = rms_vlow_fcc_c06
            thrs_high = thrs_high_fcc_c06
            thrs_vlow = thrs_vlow_fcc_c06
            dacc      = dacc_fcc_c06
            gain_high = gain_high_fcc_c06
            gain_vlow = gain_vlow_fcc_c06
            pCoupG3MainBranch.setLabel("Transmission branch 0 and 1")
        
        elif ("PLCOUP011 (FCC only" in g3_coupSettings):
            pCoupG3MainPhyBand.setValue("FCC")
            # print("UpdatePlcCouplingParameters ->  PL360 G3 FCC SB")
            rms_high  = rms_high_fcc_c11
            rms_vlow  = rms_vlow_fcc_c11
            thrs_high = thrs_high_fcc_c11
            thrs_vlow = thrs_vlow_fcc_c11
            dacc      = dacc_fcc_c11
            gain_high = gain_high_fcc_c11
            gain_vlow = gain_vlow_fcc_c11
            pCoupG3MainBranch.setLabel("Transmission branch 1")
        
        elif ("PLCOUP011 (FCC + CENELEC-A" in g3_coupSettings):
            auxiliaryBand = True

            # print("UpdatePlcCouplingParameters ->  PL360 G3 AUX CEN-A")
            rms_high  = rms_high_cena_c07
            rms_vlow  = rms_vlow_cena_c07
            thrs_high = thrs_high_cena_c07
            thrs_vlow = thrs_vlow_cena_c07
            dacc      = dacc_cena_c07
            gain_high = gain_high_cena_c07
            gain_vlow = gain_vlow_cena_c07
            pCoupG3MainPhyBand.setValue("CEN-A")
            pCoupG3MainBranch.setLabel("Transmission branch 0")

            # print("UpdatePlcCouplingParameters ->  PL360 G3 FCC SB")
            rms_high_aux  = rms_high_fcc_c11
            rms_vlow_aux  = rms_vlow_fcc_c11
            thrs_high_aux = thrs_high_fcc_c11
            thrs_vlow_aux = thrs_vlow_fcc_c11
            dacc_aux      = dacc_fcc_c11
            gain_high_aux = gain_high_fcc_c11
            gain_vlow_aux = gain_vlow_fcc_c11
            pCoupG3AuxPhyBand.setValue("FCC")
            pCoupG3AuxBranch.setLabel("Transmission branch 1")

        else: # "CEN-A" as default configuration
            # print("UpdatePlcCouplingParameters -> PL360 G3 CEN-A")
            rms_high  = rms_high_cena_c07
            rms_vlow  = rms_vlow_cena_c07
            thrs_high = thrs_high_cena_c07
            thrs_vlow = thrs_vlow_cena_c07
            dacc      = dacc_cena_c07
            gain_high = gain_high_cena_c07
            gain_vlow = gain_vlow_cena_c07
            pCoupG3MainPhyBand.setValue("CEN-A")
            pCoupG3MainBranch.setLabel("Transmission branch 0")

    # Update Values of the Main Branch in Configuration Window
    if (mainBand == True):
        pCoupG3MainBranch.setVisible(True)
        
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_LINE_DRIVER", line_drv)

        for idx in range(8):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_RMS_HIGH_" + str(idx), rms_high[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_RMS_VLOW_" + str(idx), rms_vlow[idx])

        for idx in range(16):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_THRS_HIGH_" + str(idx), thrs_high[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_THRS_VLOW_" + str(idx), thrs_vlow[idx])

        for idx in range(17):
            pCoupG3DACC[idx].setValue(dacc[idx])

        for idx in range(3):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_GAIN_HIGH_" + str(idx), gain_high[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_GAIN_VLOW_" + str(idx), gain_vlow[idx])

    # Update Values of the Auxiliary Branch in Configuration Window
    if (auxiliaryBand == True):
        pCoupG3AuxBranch.setVisible(True)

        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_AUX_LINE_DRIVER", line_drv_aux)

        for idx in range(8):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_AUX_RMS_HIGH_" + str(idx), rms_high_aux[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_AUX_RMS_VLOW_" + str(idx), rms_vlow_aux[idx])

        for idx in range(16):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_AUX_THRS_HIGH_" + str(idx), thrs_high_aux[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_AUX_THRS_VLOW_" + str(idx), thrs_vlow_aux[idx])

        for idx in range(17):
            pCoupG3DACCAux[idx].setValue(dacc_aux[idx])

        for idx in range(3):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_AUX_GAIN_HIGH_" + str(idx), gain_high_aux[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_AUX_GAIN_VLOW_" + str(idx), gain_vlow_aux[idx])

############################################################################
#### PRIME function to update the Coupling Parameters ####
############################################################################

def pCoupConfigureDACC(plcDevice, channels, coupSettings):
    global pCoupPRIMEDACCCENAMenu
    global pCoupPRIMEDACCFCCMenu
    global pCoupPRIMEDACC2CHNMenu

    cen_enable = False
    fcc_enable = False
    double_chn_enable = False

    if (plcDevice == "PL360"):
        if (channels & 1):
            # C07
            cen_enable = True
            dacc_cen = dacc_chn1_c07

        if (channels & 0xFE):
            fcc_enable = True
            # Channels 2 - 8
            if ("PLCOUP011" in coupSettings):
                # C11
                dacc_fcc = dacc_chn_fcc_c11
            else:
                # C06
                dacc_fcc = dacc_chn_fcc_c06

        if (channels & 0x7F00):
            double_chn_enable = True
            # Double Channels
            if ("PLCOUP011" in coupSettings):
                # C11
                dacc_2chn = dacc_2chn_fcc_c11
            else:
                # C06
                dacc_2chn = dacc_2chn_fcc_c06
    else:
        if (channels & 1):
            cen_enable = True
            if ("auxiliary" in coupSettings):
                # drv_aux
                dacc_cen = dacc_chn1_drv_aux
            elif ("Multiband single-branch" in coupSettings):
                # drv_mbsb
                dacc_cen = dacc_chn1_drv_mbsb
            else:
                # drv_sb
                dacc_cen = dacc_chn1_drv_sb

        if (channels & 0xFE):
            fcc_enable = True
            # Channels 2 - 8
            if ("Multiband single-branch" in coupSettings):
                # drv_mbsb
                dacc_fcc = dacc_chn_fcc_drv_mbsb
            else:
                # drv_sb / drv_hiAtt
                dacc_fcc = dacc_chn_fcc_drv

        if (channels & 0x7F00):
            double_chn_enable = True
            # Double Channels
            # drv_sb / drv_hiAtt / drv_mbsb
            dacc_2chn = dacc_2chn_fcc_drv

    if (cen_enable == True):
        for idx in range(17):
            pCoupPRIMEDACCCENA[idx].setValue(dacc_cen[idx])

    if (fcc_enable == True):
        for idx in range(17):
            pCoupPRIMEDACCFCC[idx].setValue(dacc_fcc[idx])

    if (double_chn_enable == True):
        for idx in range(17):
            pCoupPRIMEDACC2CHN[idx].setValue(dacc_2chn[idx])

    pCoupPRIMEDACCCENAMenu.setVisible(cen_enable)
    pCoupPRIMEDACCFCCMenu.setVisible(fcc_enable)
    pCoupPRIMEDACC2CHNMenu.setVisible(double_chn_enable)

def pCoupConfigureChannelDetImp(channels, coupSettings):
    global pCoupPRIMEChannelsImpDetect

    channelsImpDetect = 0x7CFD
    chnImpDetect = 0
    if "CHN1 only" in coupSettings:
        # Only channel 1 valid for impedance detection
        channelsImpDetect = 0x0001
        chnImpDetect = 1
    elif "FCC default (FCC channels only" in coupSettings:
        # All FCC channels valid for impedance detection
        channelsImpDetect = 0x7CFC
        chnImpDetect = 5
    elif "FCC high attenuation (FCC channels only" in coupSettings:
        # Channels 3, 4, 6, 10, 11, 12 valid for impedance detection
        channelsImpDetect = 0x0E2C
        chnImpDetect = 4
    elif "Multiband high attenuation" in coupSettings:
        # Channels 1, 3, 4, 6, 10, 11, 12 valid for impedance detection
        channelsImpDetect = 0x0E2D
        chnImpDetect = 4
    elif "Multiband single-branch" in coupSettings:
        # All channels but 1, 2, 9, 10 valid for impedance detection
        channelsImpDetect = 0x7CFC
        chnImpDetect = 5
    elif "PLCOUP006" in coupSettings:
        # All FCC channels valid for impedance detection
        channelsImpDetect = 0x7CFC
        chnImpDetect = 5
    elif "PLCOUP011" in coupSettings:
        # Only channel 1 valid for impedance detection
        channelsImpDetect = 0x0001
        chnImpDetect = 1

    pCoupPRIMEChannelsImpDetect.setValue(channelsImpDetect)

    if chnImpDetect != 0:
        if (channels & (1 << (chnImpDetect - 1))) == 0:
            chnImpDetect = 0

    if chnImpDetect == 0:
        for chn in range(1, 16):
            if (channels & channelsImpDetect & (1 << (chn - 1))) != 0:
                chnImpDetect = chn
                break

    if chnImpDetect == 0:
        return "CHN_INVALID"
    elif chnImpDetect > 8:
        return "CHN%d_CHN%d"%(chnImpDetect - 8, chnImpDetect - 7)

    return "CHN%d"%(chnImpDetect)

def pCoupConfigureChannel(plcDevice, channel, coupSettings):

    if (plcDevice == "PL360"):
        line_drv = 0
        if (channel == 1):
            # C07
            rms_high  = rms_high_chn1_c07
            rms_vlow  = rms_vlow_chn1_c07
            thrs_high = thrs_high_chn1_c07
            thrs_vlow = thrs_vlow_chn1_c07
            gain_high = gain_high_chn1_c07
            gain_vlow = gain_vlow_chn1_c07
        else:
            # Channels 2 - 8
            if ("PLCOUP011" in coupSettings):
                # C11
                rms_high  = rms_high_c11[channel - 2]
                rms_vlow  = rms_vlow_c11[channel - 2]
                thrs_high = thrs_high_c11[channel - 2]
                thrs_vlow = thrs_vlow_c11[channel - 2]
                gain_high = gain_high_c11[channel - 2]
                gain_vlow = gain_vlow_c11[channel - 2]
            else:
                # C06
                rms_high  = rms_high_c06[channel - 2]
                rms_vlow  = rms_vlow_c06[channel - 2]
                thrs_high = thrs_high_c06[channel - 2]
                thrs_vlow = thrs_vlow_c06[channel - 2]
                gain_high = gain_high_c06[channel - 2]
                gain_vlow = gain_vlow_c06[channel - 2]
    else:
        if (channel == 1):
            if ("auxiliary" in coupSettings):
                # drv_aux
                line_drv = 8
                rms_high  = rms_high_chn1_drv_aux
                rms_vlow  = rms_vlow_chn1_drv_aux
                thrs_high = thrs_high_chn1_drv_aux
                thrs_vlow = thrs_vlow_chn1_drv_aux
                gain_high = gain_high_chn1_drv_aux
                gain_vlow = gain_vlow_chn1_drv_aux
            elif ("Multiband single-branch" in coupSettings):
                # drv_mbsb
                line_drv = 0xA7
                rms_high  = rms_high_chn1_drv_mbsb
                rms_vlow  = rms_vlow_chn1_drv_mbsb
                thrs_high = thrs_high_chn1_drv_mbsb
                thrs_vlow = thrs_vlow_chn1_drv_mbsb
                gain_high = gain_high_chn1_drv_mbsb
                gain_vlow = gain_vlow_chn1_drv_mbsb
            else:
                # drv_sb
                line_drv = 5
                rms_high  = rms_high_chn1_drv_sb
                rms_vlow  = rms_vlow_chn1_drv_sb
                thrs_high = thrs_high_chn1_drv_sb
                thrs_vlow = thrs_vlow_chn1_drv_sb
                gain_high = gain_high_chn1_drv_sb
                gain_vlow = gain_vlow_chn1_drv_sb
        else:
            # Channels 2 - 8
            if ("high attenuation" in coupSettings):
                # drv_hmip
                line_drv = 7
                rms_high  = rms_high_drv_hiAtt[channel - 2]
                rms_vlow  = rms_vlow_drv_hiAtt[channel - 2]
                thrs_high = thrs_high_drv_hiAtt[channel - 2]
                thrs_vlow = thrs_vlow_drv_hiAtt[channel - 2]
                gain_high = gain_high_drv_hiAtt[channel - 2]
                gain_vlow = gain_vlow_drv_hiAtt[channel - 2]
            elif ("Multiband single-branch" in coupSettings):
                # drv_mbsb
                line_drv = 0x47
                rms_high  = rms_high_drv_mbsb[channel - 2]
                rms_vlow  = rms_vlow_drv_mbsb[channel - 2]
                thrs_high = thrs_high_drv_mbsb[channel - 2]
                thrs_vlow = thrs_vlow_drv_mbsb[channel - 2]
                gain_high = gain_high_drv_mbsb[channel - 2]
                gain_vlow = gain_vlow_drv_mbsb[channel - 2]
            else:
                # drv_sb
                line_drv = 5
                rms_high  = rms_high_drv_sb[channel - 2]
                rms_vlow  = rms_vlow_drv_sb[channel - 2]
                thrs_high = thrs_high_drv_sb[channel - 2]
                thrs_vlow = thrs_vlow_drv_sb[channel - 2]
                gain_high = gain_high_drv_sb[channel - 2]
                gain_vlow = gain_vlow_drv_sb[channel - 2]

    # Update Values of the pCoup symbols
    symbol_id = "SRV_PCOUP_PRIME_CHN" + str(channel)
    Database.setSymbolValue("srv_pcoup", symbol_id + "_LINE_DRIVER", line_drv)

    for idx in range(8):
        Database.setSymbolValue("srv_pcoup", symbol_id + "_RMS_HIGH_" + str(idx), rms_high[idx])
        Database.setSymbolValue("srv_pcoup", symbol_id + "_RMS_VLOW_" + str(idx), rms_vlow[idx])

    for idx in range(16):
        Database.setSymbolValue("srv_pcoup", symbol_id + "_THRS_HIGH_" + str(idx), thrs_high[idx])
        Database.setSymbolValue("srv_pcoup", symbol_id + "_THRS_VLOW_" + str(idx), thrs_vlow[idx])

    for idx in range(3):
        Database.setSymbolValue("srv_pcoup", symbol_id + "_GAIN_HIGH_" + str(idx), gain_high[idx])
        Database.setSymbolValue("srv_pcoup", symbol_id + "_GAIN_VLOW_" + str(idx), gain_vlow[idx])

def pCoupConfigure2Channel(plcDevice, channel, coupSettings):

    if (plcDevice == "PL360"):
        line_drv = 0
        if ("PLCOUP011" in coupSettings):
            # C11
            rms_high  = rms_high_2chn_c11[channel]
            rms_vlow  = rms_vlow_2chn_c11[channel]
            thrs_high = thrs_high_2chn_c11[channel]
            thrs_vlow = thrs_vlow_2chn_c11[channel]
            gain_high = gain_high_2chn_c11[channel]
            gain_vlow = gain_vlow_2chn_c11[channel]
        else:
            # C06
            rms_high  = rms_high_2chn_c06[channel]
            rms_vlow  = rms_vlow_2chn_c06[channel]
            thrs_high = thrs_high_2chn_c06[channel]
            thrs_vlow = thrs_vlow_2chn_c06[channel]
            gain_high = gain_high_2chn_c06[channel]
            gain_vlow = gain_vlow_2chn_c06[channel]
    else:
        if ("high attenuation" in coupSettings):
            # drv_hmip
            line_drv = 7
            rms_high  = rms_high_2chn_drv_hiAtt[channel]
            rms_vlow  = rms_vlow_2chn_drv_hiAtt[channel]
            thrs_high = thrs_high_2chn_drv_hiAtt[channel]
            thrs_vlow = thrs_vlow_2chn_drv_hiAtt[channel]
            gain_high = gain_high_2chn_drv_hiAtt[channel]
            gain_vlow = gain_vlow_2chn_drv_hiAtt[channel]
        elif ("Multiband single-branch" in coupSettings):
            # drv_mbsb
            line_drv = 0x47
            rms_high   = rms_high_2chn_drv_mbsb[channel]
            rms_vlow  = rms_vlow_2chn_drv_mbsb[channel]
            thrs_high = thrs_high_2chn_drv_mbsb[channel]
            thrs_vlow = thrs_vlow_2chn_drv_mbsb[channel]
            gain_high = gain_high_2chn_drv_mbsb[channel]
            gain_vlow = gain_vlow_2chn_drv_mbsb[channel]
        else:
            # drv_sb
            line_drv = 5
            rms_high  = rms_high_2chn_drv_sb[channel]
            rms_vlow  = rms_vlow_2chn_drv_sb[channel]
            thrs_high = thrs_high_2chn_drv_sb[channel]
            thrs_vlow = thrs_vlow_2chn_drv_sb[channel]
            gain_high = gain_high_2chn_drv_sb[channel]
            gain_vlow = gain_vlow_2chn_drv_sb[channel]

    # Update Values of the pCoup symbols
    symbol_id = "SRV_PCOUP_PRIME_2CHN" + str(channel + 1) + str(channel + 2)
    Database.setSymbolValue("srv_pcoup", symbol_id, True)
    Database.setSymbolValue("srv_pcoup", symbol_id + "_LINE_DRIVER", line_drv)

    for idx in range(8):
        Database.setSymbolValue("srv_pcoup", symbol_id + "_RMS_HIGH_" + str(idx), rms_high[idx])
        Database.setSymbolValue("srv_pcoup", symbol_id + "_RMS_VLOW_" + str(idx), rms_vlow[idx])

    for idx in range(16):
        Database.setSymbolValue("srv_pcoup", symbol_id + "_THRS_HIGH_" + str(idx), thrs_high[idx])
        Database.setSymbolValue("srv_pcoup", symbol_id + "_THRS_VLOW_" + str(idx), thrs_vlow[idx])

    for idx in range(3):
        Database.setSymbolValue("srv_pcoup", symbol_id + "_GAIN_HIGH_" + str(idx), gain_high[idx])
        Database.setSymbolValue("srv_pcoup", symbol_id + "_GAIN_VLOW_" + str(idx), gain_vlow[idx])

def updatePRIMECouplingParameters():
    global pCoupG3SourceFile
    global pCoupG3HeaderFile
    global pCoupPRIMESourceFile
    global pCoupPRIMEHeaderFile
    global pCoupPRIMEChannelsSelected
    global pCoupPRIMETXChannels
    global pCoupG3TXBranches
    global pCoupPRIMEChannelImpedanceDetection

    if Database.getSymbolValue("drvPlcPhy", "DRV_PLC_MODE") != None:
        plcDriver = "drvPlcPhy"
    else:
        plcDriver = ""
        print("updatePRIMECouplingParameters: DRV PLC MODE not found")
        return

    # Show PRIME setting, hide G3 setting
    pCoupG3TXBranches.setVisible(False)
    pCoupPRIMETXChannels.setVisible(True)

    # Enable PRIME COUP files
    pCoupG3SourceFile.setEnabled(False)
    pCoupG3HeaderFile.setEnabled(False)
    pCoupMMSourceFile.setEnabled(False)
    pCoupMMHeaderFile.setEnabled(False)
    pCoupPRIMESourceFile.setEnabled(True)
    pCoupPRIMEHeaderFile.setEnabled(True)

    plcDevice = Database.getSymbolValue(plcDriver, "DRV_PLC_MODE")
    channels_sel = Database.getSymbolValue(plcDriver, "DRV_PLC_PRIME_CHANNELS_SELECTED")

    if (plcDevice == "PL460"):
        prime_coupSettings = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_PRIME_SETTING_PL460")
    else:
        prime_coupSettings = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_PRIME_SETTING_PL360")
    
    if prime_coupSettings == None:
        print("updatePRIMECouplingParameters: DRV PLC Coupling Settings not found")
        return

    # Configure DACC
    pCoupConfigureDACC(plcDevice, channels_sel, prime_coupSettings)
    channel_imp_det = pCoupConfigureChannelDetImp(channels_sel, prime_coupSettings)

    # Configure Single channels selected in PHY driver
    for idx in range(8):
        symbol_id = "SRV_PCOUP_PRIME_CHN" + str(idx + 1)
        if (channels_sel & (1 << idx)):
            pCoupConfigureChannel(plcDevice, idx + 1, prime_coupSettings)
            pCoupPRIMEMenuChn[idx].setVisible(True)
            Database.setSymbolValue("srv_pcoup", symbol_id, True)
        else:
            pCoupPRIMEMenuChn[idx].setVisible(False)
            Database.setSymbolValue("srv_pcoup", symbol_id, False)

    # Configure Double channels selected in PHY driver
    for idx in range(7):
        symbol_id = "SRV_PCOUP_PRIME_2CHN" + str(idx + 1) + str(idx + 2)
        if (channels_sel & (1 << (idx + 8))):
            pCoupConfigure2Channel(plcDevice, idx, prime_coupSettings)
            pCoupPRIMEMenu2Chn[idx].setVisible(True)
            Database.setSymbolValue("srv_pcoup", symbol_id, True)
        else:
            pCoupPRIMEMenu2Chn[idx].setVisible(False)
            Database.setSymbolValue("srv_pcoup", symbol_id, False)

    pCoupPRIMEChannelsSelected.setValue(channels_sel)
    pCoupPRIMEChannelImpedanceDetection.setSelectedKey(channel_imp_det)

############################################################################
#### Meters&More function to update the Coupling Parameters ####
############################################################################
def updateMMCouplingParameters():
    global pCoupG3AuxBranch
    global pCoupG3MainPhyBand
    global pCoupG3AuxPhyBand
    global pCoupG3SourceFile
    global pCoupG3HeaderFile
    global pCoupPRIMESourceFile
    global pCoupPRIMEHeaderFile
    global pCoupPRIMETXChannels
    global pCoupG3TXBranches
    global pCoupMMSourceFile
    global pCoupMMHeaderFile

    if Database.getSymbolValue("drvPlcPhy", "DRV_PLC_MODE") != None:
        plcDriver = "drvPlcPhy"
    else:
        plcDriver = ""
        print("updateMMCouplingParameters: DRV PLC MODE not found")
        return

    # Show G3 setting, hide PRIME setting
    pCoupG3TXBranches.setVisible(True)
    pCoupPRIMETXChannels.setVisible(False)

    plcDevice = Database.getSymbolValue(plcDriver, "DRV_PLC_MODE")
    mm_coupSettings = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_MM_SETTING_PL460")

    # Enable MM COUP files
    pCoupG3SourceFile.setEnabled(False)
    pCoupG3HeaderFile.setEnabled(False)
    pCoupPRIMESourceFile.setEnabled(False)
    pCoupPRIMEHeaderFile.setEnabled(False)
    pCoupMMSourceFile.setEnabled(True)
    pCoupMMHeaderFile.setEnabled(True)

    pCoupG3MainBranch.setVisible(True)
    pCoupG3MainPhyBand.setVisible(False)
    pCoupG3AuxBranch.setVisible(False)

    if (plcDevice == "PL460"):
        if ("auxiliary branch" in mm_coupSettings):
            # PL460 Auxiliary Branch
            line_drv  = mm_drv_conf_aux
            dacc      = mm_dacc_aux
            rms_high  = mm_rms_high_c07
            rms_vlow  = mm_rms_vlow_c07
            thrs_high = mm_thrs_high_c07
            thrs_vlow = mm_thrs_vlow_c07
            gain_high = mm_gain_high_c07
            gain_vlow = mm_gain_vlow_c07
            line_drv  = mm_drv_conf_aux
            dacc      = mm_dacc_aux
            pCoupG3MainBranch.setLabel("Auxiliary Transmission branch")

        elif ("Multiband single-branch" in mm_coupSettings):
            # PL460 Multiband Single Branch
            line_drv  = mm_drv_conf_drv_mbsb
            dacc      = mm_dacc_drv_mbsb
            rms_high  = mm_rms_high_drv_mbsb
            rms_vlow  = mm_rms_vlow_drv_mbsb
            thrs_high = mm_thrs_high_drv_mbsb
            thrs_vlow = mm_thrs_vlow_drv_mbsb
            gain_high = mm_gain_high_drv_mbsb
            gain_vlow = mm_gain_vlow_drv_mbsb
            line_drv  = mm_drv_conf_drv_mbsb
            dacc      = mm_dacc_drv_mbsb
            pCoupG3MainBranch.setLabel("Main Transmission branch (ASO1)")
        
        else:
            # PL460 CEN-A Single Branch
            line_drv  = mm_drv_conf_drv_sb
            dacc      = mm_dacc_drv_sb
            rms_high  = mm_rms_high_drv_sb
            rms_vlow  = mm_rms_vlow_drv_sb
            thrs_high = mm_thrs_high_drv_sb
            thrs_vlow = mm_thrs_vlow_drv_sb
            gain_high = mm_gain_high_drv_sb
            gain_vlow = mm_gain_vlow_drv_sb
            line_drv  = mm_drv_conf_drv_sb
            dacc      = mm_dacc_drv_sb
            pCoupG3MainBranch.setLabel("Main Transmission branch")        

    else: # "PL360"
        line_drv  = mm_drv_conf_c07
        dacc      = mm_dacc_c07
        rms_high  = mm_rms_high_c07
        rms_vlow  = mm_rms_vlow_c07
        thrs_high = mm_thrs_high_c07
        thrs_vlow = mm_thrs_vlow_c07
        gain_high = mm_gain_high_c07
        gain_vlow = mm_gain_vlow_c07
        pCoupG3MainBranch.setLabel("Transmission branch 0")

    # Update Values of the Main Branch in Configuration Window
    Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_LINE_DRIVER", line_drv)

    for idx in range(8):
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_RMS_HIGH_" + str(idx), rms_high[idx])
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_RMS_VLOW_" + str(idx), rms_vlow[idx])

    for idx in range(16):
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_THRS_HIGH_" + str(idx), thrs_high[idx])
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_THRS_VLOW_" + str(idx), thrs_vlow[idx])

    for idx in range(17):
        pCoupG3DACC[idx].setValue(dacc[idx])

    for idx in range(3):
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_GAIN_HIGH_" + str(idx), gain_high[idx])
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_G3_GAIN_VLOW_" + str(idx), gain_vlow[idx])


def handleMessage(messageID, args):
    result_dict = {}

    if (messageID == "SRV_PCOUP_UPDATE_G3_PARAMETERS"):
        updateG3CouplingParameters()
    elif (messageID == "SRV_PCOUP_UPDATE_PRIME_PARAMETERS"):
        updatePRIMECouplingParameters()
    elif (messageID == "SRV_PCOUP_UPDATE_MM_PARAMETERS"):
        updateMMCouplingParameters()

    return result_dict


def dependenciesHandler(symbol, event):
    updateCouplingParameters()

def gainUpdMinValue(symbol, event):
    symbol.setMin(event["value"])

def onAttachmentConnected(source, target):
    updateCouplingParameters()

def instantiateComponent(pCoupComponentCommon):

    Log.writeInfoMessage("Loading PLC Phy Coupling service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    ############################################################################
    #### G3 Main Transmission Branch ####
    ############################################################################
    global pCoupG3TXBranches
    pCoupG3TXBranches = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_TX_BRANCHES", None)
    pCoupG3TXBranches.setLabel("Transmission Branches")
    pCoupG3TXBranches.setHelp(srv_pcoup_helpkeyword)

    global pCoupG3MainBranch
    pCoupG3MainBranch = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_MAIN_BRANCH", pCoupG3TXBranches)
    pCoupG3MainBranch.setLabel("Main Transmission branch")
    pCoupG3MainBranch.setHelp(srv_pcoup_helpkeyword)

    global pCoupG3MainPhyBand
    # pCoupG3MainPhyBand = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_G3_MAIN_G3_BAND", pCoupG3MainBranch, ["CEN-A", "CEN-B", "FCC", "ARIB"])
    pCoupG3MainPhyBand = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_G3_MAIN_BAND", pCoupG3MainBranch, ["None", "CEN-A", "CEN-B", "FCC"])
    pCoupG3MainPhyBand.setLabel("G3-PLC PHY Band")
    pCoupG3MainPhyBand.setDefaultValue("FCC")
    pCoupG3MainPhyBand.setReadOnly(True)
    pCoupG3MainPhyBand.setHelp(srv_pcoup_helpkeyword)

    pCoupG3NumTxLvl = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_NUM_TX_LVL", pCoupG3MainBranch)
    pCoupG3NumTxLvl.setLabel("Number of TX levels")
    pCoupG3NumTxLvl.setDefaultValue(8)
    pCoupG3NumTxLvl.setMin(1)
    pCoupG3NumTxLvl.setMax(8)
    pCoupG3NumTxLvl.setHelp(srv_pcoup_helpkeyword)
    # Use an auxiliary control to handle dependencies with drvPlcPhy
    pCoupG3NumTxLvl.setDependencies(dependenciesHandler, ["drvPlcPhy.DRV_PLC_COUP_G3_SETTING_PL460",
                                                          "drvPlcPhy.DRV_PLC_COUP_G3_SETTING_PL360",
                                                          "drvPlcPhy.DRV_PLC_MODE",
                                                          "drvG3MacRt.DRV_PLC_COUP_G3_SETTING_PL460",
                                                          "drvG3MacRt.DRV_PLC_COUP_G3_SETTING_PL360",
                                                          "drvG3MacRt.DRV_PLC_MODE"])

    pCoupG3RMSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_RMS_HIGH", pCoupG3MainBranch)
    pCoupG3RMSHigh.setLabel("Target RMS values in HIGH mode for dynamic Tx gain")
    pCoupG3RMSHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3RMSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_RMS_VLOW", pCoupG3MainBranch)
    pCoupG3RMSVlow.setLabel("Target RMS values in VLOW mode for dynamic Tx gain")
    pCoupG3RMSVlow.setHelp(srv_pcoup_helpkeyword)

    for idx in range(8):
        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_RMS_HIGH_" + str(idx), pCoupG3RMSHigh)
        pCoupG3Symbol.setLabel("RMS_HIGH_" + str(idx))
        pCoupG3Symbol.setDefaultValue(rms_high_fcc[idx])
        pCoupG3Symbol.setMin(0)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)

        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_RMS_VLOW_" + str(idx), pCoupG3RMSVlow)
        pCoupG3Symbol.setLabel("RMS_VLOW_" + str(idx))
        pCoupG3Symbol.setDefaultValue(rms_vlow_fcc[idx])
        pCoupG3Symbol.setMin(0)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)

    pCoupG3THRSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_THRS_HIGH", pCoupG3MainBranch)
    pCoupG3THRSHigh.setLabel("Threshold RMS values in HIGH mode for dynamic Tx mode")
    pCoupG3THRSHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3THRSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_THRS_VLOW", pCoupG3MainBranch)
    pCoupG3THRSVlow.setLabel("Threshold RMS values in VLOW mode for dynamic Tx mode")
    pCoupG3THRSVlow.setHelp(srv_pcoup_helpkeyword)

    for idx in range(16):
        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_THRS_HIGH_" + str(idx), pCoupG3THRSHigh)
        pCoupG3Symbol.setDefaultValue(thrs_high_fcc[idx])
        pCoupG3Symbol.setMin(0)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)
        if (idx < 8):
            pCoupG3Symbol.setLabel("THRESHOLD_HIGH_LOW_" + str(idx))
        else:
            pCoupG3Symbol.setLabel("THRESHOLD_HIGH_VLOW_" + str(idx - 8))

        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_THRS_VLOW_" + str(idx), pCoupG3THRSVlow)
        pCoupG3Symbol.setDefaultValue(thrs_vlow_fcc[idx])
        pCoupG3Symbol.setMin(0)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)
        if (idx < 8):
            pCoupG3Symbol.setLabel("THRESHOLD_VLOW_LOW_" + str(idx))
        else:
            pCoupG3Symbol.setLabel("THRESHOLD_VLOW_HIGH_" + str(idx - 8))

    pCoupG3DACCMenu = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_DACC", pCoupG3MainBranch)
    pCoupG3DACCMenu.setLabel("PLC DACC peripheral settings")
    pCoupG3DACCMenu.setHelp(srv_pcoup_helpkeyword)

    for idx in range(17):
        pCoupG3DACC.append(pCoupComponentCommon.createHexSymbol("SRV_PCOUP_G3_DACC_" + str(idx), pCoupG3DACCMenu))
        pCoupG3DACC[idx].setLabel("DACC_" + str(idx))
        pCoupG3DACC[idx].setDefaultValue(dacc_fcc[idx])
        pCoupG3DACC[idx].setMin(0)
        pCoupG3DACC[idx].setMax(0xFFFFFFFF)
        pCoupG3DACC[idx].setHelp(srv_pcoup_helpkeyword)

    pCoupG3GainHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_GAIN_HIGH", pCoupG3MainBranch)
    pCoupG3GainHigh.setLabel("Tx gain values for HIGH mode")
    pCoupG3GainHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3GainVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_GAIN_VLOW", pCoupG3MainBranch)
    pCoupG3GainVlow.setLabel("Tx gain values for VLOW mode")
    pCoupG3GainVlow.setHelp(srv_pcoup_helpkeyword)

    pCoupG3GainHighSymbols = []
    pCoupG3GainVlowSymbols = []
    for idx in range(3):
        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_GAIN_HIGH_" + str(idx), pCoupG3GainHigh)
        pCoupG3Symbol.setDefaultValue(gain_high_fcc[idx])
        pCoupG3Symbol.setMax(0xFFFF)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)
        pCoupG3GainHighSymbols.append(pCoupG3Symbol)

        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_GAIN_VLOW_" + str(idx), pCoupG3GainVlow)
        pCoupG3Symbol.setDefaultValue(gain_vlow_fcc[idx])
        pCoupG3Symbol.setMax(0xFFFF)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)
        pCoupG3GainVlowSymbols.append(pCoupG3Symbol)

    pCoupG3GainHighSymbols[0].setLabel("GAIN_HIGH_INI")
    pCoupG3GainHighSymbols[0].setDependencies(gainUpdMinValue, ["SRV_PCOUP_G3_GAIN_HIGH_1"])
    pCoupG3GainHighSymbols[1].setLabel("GAIN_HIGH_MIN")
    pCoupG3GainHighSymbols[1].setMin(1)
    pCoupG3GainHighSymbols[2].setLabel("GAIN_HIGH_MAX")
    pCoupG3GainHighSymbols[2].setDependencies(gainUpdMinValue, ["SRV_PCOUP_G3_GAIN_HIGH_0"])

    pCoupG3GainVlowSymbols[0].setLabel("GAIN_VLOW_INI")
    pCoupG3GainVlowSymbols[0].setDependencies(gainUpdMinValue, ["SRV_PCOUP_G3_GAIN_VLOW_1"])
    pCoupG3GainVlowSymbols[1].setLabel("GAIN_VLOW_MIN")
    pCoupG3GainVlowSymbols[1].setMin(1)
    pCoupG3GainVlowSymbols[2].setLabel("GAIN_VLOW_MAX")
    pCoupG3GainVlowSymbols[2].setDependencies(gainUpdMinValue, ["SRV_PCOUP_G3_GAIN_VLOW_0"])

    global pCoupG3LineDriver
    pCoupG3LineDriver = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_LINE_DRIVER", pCoupG3MainBranch)
    pCoupG3LineDriver.setLabel("PLC Line Driver settings")
    pCoupG3LineDriver.setDefaultValue(drv_conf_fcc)
    pCoupG3LineDriver.setReadOnly(True)
    pCoupG3LineDriver.setHelp(srv_pcoup_helpkeyword)

    ############################################################################
    #### G3 Auxiliary Transmission Branch ####
    ############################################################################

    global pCoupG3AuxBranch
    pCoupG3AuxBranch = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_BRANCH", pCoupG3TXBranches)
    pCoupG3AuxBranch.setLabel("Auxiliary Transmission branch")
    pCoupG3AuxBranch.setVisible(False)
    pCoupG3AuxBranch.setHelp(srv_pcoup_helpkeyword)

    global pCoupG3AuxPhyBand
    # pCoupG3AuxPhyBand = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_G3_AUX_G3_BAND", pCoupG3AuxBranch, ["None", "CEN-A", "CEN-B", "FCC", "ARIB"])
    pCoupG3AuxPhyBand = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_G3_AUX_BAND", pCoupG3AuxBranch, ["None", "CEN-A", "CEN-B", "FCC"])
    pCoupG3AuxPhyBand.setLabel("G3-PLC PHY Band")
    pCoupG3AuxPhyBand.setDefaultValue("CEN-A")
    pCoupG3AuxPhyBand.setReadOnly(True)
    pCoupG3AuxPhyBand.setHelp(srv_pcoup_helpkeyword)

    pCoupG3AuxNumTxLvl = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_NUM_TX_LVL", pCoupG3AuxBranch)
    pCoupG3AuxNumTxLvl.setLabel("Number of TX levels")
    pCoupG3AuxNumTxLvl.setDefaultValue(8)
    pCoupG3AuxNumTxLvl.setHelp(srv_pcoup_helpkeyword)

    pCoupG3AuxRMSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_RMS_HIGH", pCoupG3AuxBranch)
    pCoupG3AuxRMSHigh.setLabel("Target RMS values in HIGH mode for dynamic Tx gain")
    pCoupG3AuxRMSHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3AuxRMSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_RMS_VLOW", pCoupG3AuxBranch)
    pCoupG3AuxRMSVlow.setLabel("Target RMS values in VLOW mode for dynamic Tx gain")
    pCoupG3AuxRMSVlow.setHelp(srv_pcoup_helpkeyword)

    for idx in range(8):
        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_RMS_HIGH_" + str(idx), pCoupG3AuxRMSHigh)
        pCoupG3Symbol.setLabel("RMS_HIGH_" + str(idx))
        pCoupG3Symbol.setDefaultValue(rms_high_cena_c07[idx])
        pCoupG3Symbol.setMin(0)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)

        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_RMS_VLOW_" + str(idx), pCoupG3AuxRMSVlow)
        pCoupG3Symbol.setLabel("RMS_VLOW_" + str(idx))
        pCoupG3Symbol.setDefaultValue(rms_vlow_cena_c07[idx])
        pCoupG3Symbol.setMin(0)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)

    pCoupG3AuxTHRSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_THRS_HIGH", pCoupG3AuxBranch)
    pCoupG3AuxTHRSHigh.setLabel("Threshold RMS values in HIGH mode for dynamic Tx mode")
    pCoupG3AuxTHRSHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3AuxTHRSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_THRS_VLOW", pCoupG3AuxBranch)
    pCoupG3AuxTHRSVlow.setLabel("Threshold RMS values in VLOW mode for dynamic Tx mode")
    pCoupG3AuxTHRSVlow.setHelp(srv_pcoup_helpkeyword)

    for idx in range(16):
        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_THRS_HIGH_" + str(idx), pCoupG3AuxTHRSHigh)
        pCoupG3Symbol.setDefaultValue(thrs_high_cena_c07[idx])
        pCoupG3Symbol.setMin(0)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)
        if (idx < 8):
            pCoupG3Symbol.setLabel("THRESHOLD_HIGH_LOW_" + str(idx))
        else:
            pCoupG3Symbol.setLabel("THRESHOLD_HIGH_VLOW_" + str(idx - 8))

        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_THRS_VLOW_" + str(idx), pCoupG3AuxTHRSVlow)
        pCoupG3Symbol.setDefaultValue(thrs_vlow_cena_c07[idx])
        pCoupG3Symbol.setMin(0)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)
        if (idx < 8):
            pCoupG3Symbol.setLabel("THRESHOLD_VLOW_LOW_" + str(idx))
        else:
            pCoupG3Symbol.setLabel("THRESHOLD_VLOW_HIGH_" + str(idx - 8))

    pCoupG3AuxDACCMenu = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_DACC", pCoupG3AuxBranch)
    pCoupG3AuxDACCMenu.setLabel("PLC DACC peripheral settings")
    pCoupG3AuxDACCMenu.setHelp(srv_pcoup_helpkeyword)

    for idx in range(17):
        pCoupG3DACCAux.append(pCoupComponentCommon.createHexSymbol("SRV_PCOUP_G3_AUX_DACC_" + str(idx), pCoupG3AuxDACCMenu))
        pCoupG3DACCAux[idx].setLabel("DACC_" + str(idx))
        pCoupG3DACCAux[idx].setDefaultValue(dacc_cena_c07_drv[idx])
        pCoupG3DACCAux[idx].setMin(0)
        pCoupG3DACCAux[idx].setMax(0xFFFFFFFF)
        pCoupG3DACCAux[idx].setHelp(srv_pcoup_helpkeyword)

    pCoupG3AuxGainHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_GAIN_HIGH", pCoupG3AuxBranch)
    pCoupG3AuxGainHigh.setLabel("Tx gain values for HIGH mode")
    pCoupG3AuxGainHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3AuxGainVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_GAIN_VLOW", pCoupG3AuxBranch)
    pCoupG3AuxGainVlow.setLabel("Tx gain values for VLOW mode")
    pCoupG3AuxGainVlow.setHelp(srv_pcoup_helpkeyword)

    pCoupG3GainHighSymbols = []
    pCoupG3GainVlowSymbols = []
    for idx in range(3):
        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_GAIN_HIGH_" + str(idx), pCoupG3AuxGainHigh)
        pCoupG3Symbol.setDefaultValue(gain_high_cena_c07[idx])
        pCoupG3Symbol.setMax(0xFFFF)
        pCoupG3GainHighSymbols.append(pCoupG3Symbol)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)

        pCoupG3Symbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_GAIN_VLOW_" + str(idx), pCoupG3AuxGainVlow)
        pCoupG3Symbol.setDefaultValue(gain_vlow_cena_c07[idx])
        pCoupG3Symbol.setMax(0xFFFF)
        pCoupG3GainVlowSymbols.append(pCoupG3Symbol)
        pCoupG3Symbol.setHelp(srv_pcoup_helpkeyword)

    pCoupG3GainHighSymbols[0].setLabel("GAIN_HIGH_INI")
    pCoupG3GainHighSymbols[0].setDependencies(gainUpdMinValue, ["SRV_PCOUP_G3_AUX_GAIN_HIGH_1"])
    pCoupG3GainHighSymbols[1].setLabel("GAIN_HIGH_MIN")
    pCoupG3GainHighSymbols[1].setMin(1)
    pCoupG3GainHighSymbols[2].setLabel("GAIN_HIGH_MAX")
    pCoupG3GainHighSymbols[2].setDependencies(gainUpdMinValue, ["SRV_PCOUP_G3_AUX_GAIN_HIGH_0"])

    pCoupG3GainVlowSymbols[0].setLabel("GAIN_VLOW_INI")
    pCoupG3GainVlowSymbols[0].setDependencies(gainUpdMinValue, ["SRV_PCOUP_G3_AUX_GAIN_VLOW_1"])
    pCoupG3GainVlowSymbols[1].setLabel("GAIN_VLOW_MIN")
    pCoupG3GainVlowSymbols[1].setMin(1)
    pCoupG3GainVlowSymbols[2].setLabel("GAIN_VLOW_MAX")
    pCoupG3GainVlowSymbols[2].setDependencies(gainUpdMinValue, ["SRV_PCOUP_G3_AUX_GAIN_VLOW_0"])

    global pCoupG3AuxLineDriver
    pCoupG3AuxLineDriver = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_LINE_DRIVER", pCoupG3AuxBranch)
    pCoupG3AuxLineDriver.setLabel("PLC Line Driver settings")
    pCoupG3AuxLineDriver.setDefaultValue(0)
    pCoupG3AuxLineDriver.setReadOnly(True)
    pCoupG3AuxLineDriver.setHelp(srv_pcoup_helpkeyword)

    ############################################################################
    #### PRIME Transmission Channels ####
    ############################################################################

    global pCoupPRIMETXChannels
    pCoupPRIMETXChannels = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_PRIME_CHANNELS", None)
    pCoupPRIMETXChannels.setLabel("Transmission Channels")
    pCoupPRIMETXChannels.setHelp(srv_pcoup_helpkeyword)

    global pCoupPRIMEChannelsSelected
    pCoupPRIMEChannelsSelected = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_PRIME_CHANNELS_SELECTED", pCoupPRIMETXChannels)
    pCoupPRIMEChannelsSelected.setLabel("PRIME Channels selected")
    pCoupPRIMEChannelsSelected.setVisible(False)
    pCoupPRIMEChannelsSelected.setDefaultValue(0)

    global pCoupPRIMEChannelsImpDetect
    pCoupPRIMEChannelsImpDetect = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_PRIME_CHANNELS_IMP_DETECT", pCoupPRIMETXChannels)
    pCoupPRIMEChannelsImpDetect.setLabel("PRIME Channels for Impedance Detection")
    pCoupPRIMEChannelsImpDetect.setVisible(False)
    pCoupPRIMEChannelsImpDetect.setReadOnly(True)
    pCoupPRIMEChannelsImpDetect.setDefaultValue(0)

    global pCoupPRIMEChannelImpedanceDetection
    pCoupPRIMEChannelImpedanceDetection = pCoupComponentCommon.createKeyValueSetSymbol("SRV_PCOUP_PRIME_CHANNEL_IMP_DET", pCoupPRIMETXChannels)
    pCoupPRIMEChannelImpedanceDetection.setLabel("Impedance Detection")
    pCoupPRIMEChannelImpedanceDetection.setVisible(True)
    pCoupPRIMEChannelImpedanceDetection.setReadOnly(True)
    pCoupPRIMEChannelImpedanceDetection.setOutputMode("Key")
    pCoupPRIMEChannelImpedanceDetection.setDisplayMode("Description")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN1", "1", "CHN1")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN2", "2", "CHN2")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN3", "3", "CHN3")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN4", "4", "CHN4")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN5", "5", "CHN5")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN6", "6", "CHN6")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN7", "7", "CHN7")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN8", "8", "CHN8")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN1_CHN2", "9", "CHN1_CHN2")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN2_CHN3", "10", "CHN2_CHN3")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN3_CHN4", "11", "CHN3_CHN4")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN4_CHN5", "12", "CHN4_CHN5")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN5_CHN6", "13", "CHN5_CHN6")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN6_CHN7", "14", "CHN6_CHN7")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN7_CHN8", "15", "CHN7_CHN8")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN_INVALID", "0", "CHN_INVALID")

    global pCoupPRIMEDACCCENAMenu
    pCoupPRIMEDACCCENAMenu = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_DACC_CENA", pCoupPRIMETXChannels)
    pCoupPRIMEDACCCENAMenu.setLabel("PLC DACC peripheral settings (Channel 1)")
    pCoupPRIMEDACCCENAMenu.setVisible(False)
    pCoupPRIMEDACCCENAMenu.setHelp(srv_pcoup_helpkeyword)
    global pCoupPRIMEDACCFCCMenu
    pCoupPRIMEDACCFCCMenu = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_DACC_FCC", pCoupPRIMETXChannels)
    pCoupPRIMEDACCFCCMenu.setLabel("PLC DACC peripheral settings (Channel 2 - Channel 8)")
    pCoupPRIMEDACCFCCMenu.setVisible(False)
    pCoupPRIMEDACCFCCMenu.setHelp(srv_pcoup_helpkeyword)
    global pCoupPRIMEDACC2CHNMenu
    pCoupPRIMEDACC2CHNMenu = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_DACC_2CHN", pCoupPRIMETXChannels)
    pCoupPRIMEDACC2CHNMenu.setLabel("PLC DACC peripheral settings (Double Channels)")
    pCoupPRIMEDACC2CHNMenu.setVisible(False)
    pCoupPRIMEDACC2CHNMenu.setHelp(srv_pcoup_helpkeyword)

    for idx in range(17):
        pCoupPRIMEDACCCENA.append(pCoupComponentCommon.createHexSymbol("SRV_PCOUP_DACC_CENA_" + str(idx), pCoupPRIMEDACCCENAMenu))
        pCoupPRIMEDACCCENA[idx].setLabel("DACC_" + str(idx))
        pCoupPRIMEDACCCENA[idx].setDefaultValue(dacc_chn1_drv_aux[idx])
        pCoupPRIMEDACCCENA[idx].setMin(0)
        pCoupPRIMEDACCCENA[idx].setMax(0xFFFFFFFF)
        pCoupPRIMEDACCCENA[idx].setHelp(srv_pcoup_helpkeyword)

        pCoupPRIMEDACCFCC.append(pCoupComponentCommon.createHexSymbol("SRV_PCOUP_DACC_FCC_" + str(idx), pCoupPRIMEDACCFCCMenu))
        pCoupPRIMEDACCFCC[idx].setLabel("DACC_" + str(idx))
        pCoupPRIMEDACCFCC[idx].setDefaultValue(dacc_chn_fcc_drv[idx])
        pCoupPRIMEDACCFCC[idx].setMin(0)
        pCoupPRIMEDACCFCC[idx].setMax(0xFFFFFFFF)
        pCoupPRIMEDACCFCC[idx].setHelp(srv_pcoup_helpkeyword)

        pCoupPRIMEDACC2CHN.append(pCoupComponentCommon.createHexSymbol("SRV_PCOUP_DACC_2CHN_" + str(idx), pCoupPRIMEDACC2CHNMenu))
        pCoupPRIMEDACC2CHN[idx].setLabel("DACC_" + str(idx))
        pCoupPRIMEDACC2CHN[idx].setDefaultValue(dacc_2chn_fcc_drv[idx])
        pCoupPRIMEDACC2CHN[idx].setMin(0)
        pCoupPRIMEDACC2CHN[idx].setMax(0xFFFFFFFF)
        pCoupPRIMEDACC2CHN[idx].setHelp(srv_pcoup_helpkeyword)

    #### Single channel ########################################################
    for chn_idx in range(8):
        pCoupPRIMEMenuChn.append(pCoupComponentCommon.createMenuSymbol("DRV_PLC_PRIME_MENU_CH" + str(chn_idx + 1), pCoupPRIMETXChannels))
        pCoupPRIMEMenuChn[chn_idx].setLabel("Channel " + str(chn_idx + 1))
        pCoupPRIMEMenuChn[chn_idx].setDescription("Coupling Settings for channel " + str(chn_idx + 1))
        pCoupPRIMEMenuChn[chn_idx].setVisible(False)
        pCoupPRIMEMenuChn[chn_idx].setHelp(srv_pcoup_helpkeyword)

        symbol_id = "SRV_PCOUP_PRIME_CHN" + str(chn_idx + 1)

        pCoupPRIMENumChnEnable = pCoupComponentCommon.createBooleanSymbol(symbol_id, pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMENumChnEnable.setLabel("Channel Enable")
        pCoupPRIMENumChnEnable.setDefaultValue(False)
        pCoupPRIMENumChnEnable.setVisible(False)

        pCoupPRIMENumTxLvl = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_NUM_TX_LVL", pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMENumTxLvl.setLabel("Number of TX levels")
        pCoupPRIMENumTxLvl.setDefaultValue(8)
        pCoupPRIMENumTxLvl.setMin(1)
        pCoupPRIMENumTxLvl.setMax(8)
        pCoupPRIMENumTxLvl.setHelp(srv_pcoup_helpkeyword)

        pCoupPRIMERMSHigh = pCoupComponentCommon.createMenuSymbol(symbol_id + "_RMS_HIGH", pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMERMSHigh.setLabel("Target RMS values in HIGH mode for dynamic Tx gain")
        pCoupPRIMERMSHigh.setHelp(srv_pcoup_helpkeyword)
        pCoupPRIMERMSVlow = pCoupComponentCommon.createMenuSymbol(symbol_id + "_RMS_VLOW", pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMERMSVlow.setLabel("Target RMS values in VLOW mode for dynamic Tx gain")
        pCoupPRIMERMSVlow.setHelp(srv_pcoup_helpkeyword)

        for idx in range(8):
            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_RMS_HIGH_" + str(idx), pCoupPRIMERMSHigh)
            pCoupPRIMESymbol.setLabel("RMS_HIGH_" + str(idx))
            pCoupPRIMESymbol.setMin(0)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            if chn_idx == 0:
                pCoupPRIMESymbol.setDefaultValue(rms_high_chn1_drv_aux[idx])
            else:
                pCoupPRIMESymbol.setDefaultValue(rms_high_drv_sb[chn_idx - 1][idx])

            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_RMS_VLOW_" + str(idx), pCoupPRIMERMSVlow)
            pCoupPRIMESymbol.setLabel("RMS_VLOW_" + str(idx))
            pCoupPRIMESymbol.setMin(0)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            if chn_idx == 0:
                pCoupPRIMESymbol.setDefaultValue(rms_vlow_chn1_drv_aux[idx])
            else:
                pCoupPRIMESymbol.setDefaultValue(rms_vlow_drv_sb[chn_idx - 1][idx])

        pCoupPRIMETHRSHigh = pCoupComponentCommon.createMenuSymbol(symbol_id + "_THRS_HIGH", pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMETHRSHigh.setLabel("Threshold RMS values in HIGH mode for dynamic Tx mode")
        pCoupPRIMETHRSHigh.setHelp(srv_pcoup_helpkeyword)
        pCoupPRIMETHRSVlow = pCoupComponentCommon.createMenuSymbol(symbol_id + "_THRS_VLOW", pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMETHRSVlow.setLabel("Threshold RMS values in VLOW mode for dynamic Tx mode")
        pCoupPRIMETHRSVlow.setHelp(srv_pcoup_helpkeyword)

        for idx in range(16):
            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_THRS_HIGH_" + str(idx), pCoupPRIMETHRSHigh)
            pCoupPRIMESymbol.setMin(0)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            if chn_idx == 0:
                pCoupPRIMESymbol.setDefaultValue(thrs_high_chn1_drv_aux[idx])
            else:
                pCoupPRIMESymbol.setDefaultValue(thrs_high_drv_sb[chn_idx - 1][idx])

            if (idx < 8):
                pCoupPRIMESymbol.setLabel("THRESHOLD_HIGH_LOW_" + str(idx))
            else:
                pCoupPRIMESymbol.setLabel("THRESHOLD_HIGH_VLOW_" + str(idx - 8))

            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_THRS_VLOW_" + str(idx), pCoupPRIMETHRSVlow)
            pCoupPRIMESymbol.setMin(0)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            if chn_idx == 0:
                pCoupPRIMESymbol.setDefaultValue(thrs_vlow_chn1_drv_aux[idx])
            else:
                pCoupPRIMESymbol.setDefaultValue(thrs_vlow_drv_sb[chn_idx - 1][idx])

            if (idx < 8):
                pCoupPRIMESymbol.setLabel("THRESHOLD_VLOW_LOW_" + str(idx))
            else:
                pCoupPRIMESymbol.setLabel("THRESHOLD_VLOW_HIGH_" + str(idx - 8))

        pCoupPRIMEGainHigh = pCoupComponentCommon.createMenuSymbol(symbol_id + "_GAIN_HIGH", pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMEGainHigh.setLabel("Tx gain values for HIGH mode")
        pCoupPRIMEGainHigh.setHelp(srv_pcoup_helpkeyword)
        pCoupPRIMEGainVlow = pCoupComponentCommon.createMenuSymbol(symbol_id + "_GAIN_VLOW", pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMEGainVlow.setLabel("Tx gain values for VLOW mode")
        pCoupPRIMEGainVlow.setHelp(srv_pcoup_helpkeyword)

        pCoupPRIMEGainHighSymbols = []
        pCoupPRIMEGainVlowSymbols = []
        for idx in range(3):
            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_GAIN_HIGH_" + str(idx), pCoupPRIMEGainHigh)
            pCoupPRIMESymbol.setMax(0xFFFF)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            pCoupPRIMEGainHighSymbols.append(pCoupPRIMESymbol)

            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_GAIN_VLOW_" + str(idx), pCoupPRIMEGainVlow)
            pCoupPRIMESymbol.setMax(0xFFFF)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            pCoupPRIMEGainVlowSymbols.append(pCoupPRIMESymbol)

            if chn_idx == 0:
                pCoupPRIMEGainHighSymbols[idx].setDefaultValue(gain_high_chn1_drv_aux[idx])
                pCoupPRIMEGainVlowSymbols[idx].setDefaultValue(gain_vlow_chn1_drv_aux[idx])
            else:
                pCoupPRIMEGainHighSymbols[idx].setDefaultValue(gain_high_drv_sb[chn_idx - 1][idx])
                pCoupPRIMEGainVlowSymbols[idx].setDefaultValue(gain_vlow_drv_sb[chn_idx - 1][idx])

        pCoupPRIMEGainHighSymbols[0].setLabel("GAIN_HIGH_INI")
        pCoupPRIMEGainHighSymbols[0].setDependencies(gainUpdMinValue, [symbol_id + "_GAIN_HIGH_1"])
        pCoupPRIMEGainHighSymbols[1].setLabel("GAIN_HIGH_MIN")
        pCoupPRIMEGainHighSymbols[1].setMin(1)
        pCoupPRIMEGainHighSymbols[2].setLabel("GAIN_HIGH_MAX")
        pCoupPRIMEGainHighSymbols[2].setDependencies(gainUpdMinValue, [symbol_id + "_GAIN_HIGH_0"])

        pCoupPRIMEGainVlowSymbols[0].setLabel("GAIN_VLOW_INI")
        pCoupPRIMEGainVlowSymbols[0].setDependencies(gainUpdMinValue, [symbol_id + "_GAIN_VLOW_1"])
        pCoupPRIMEGainVlowSymbols[1].setLabel("GAIN_VLOW_MIN")
        pCoupPRIMEGainVlowSymbols[1].setMin(1)
        pCoupPRIMEGainVlowSymbols[2].setLabel("GAIN_VLOW_MAX")
        pCoupPRIMEGainVlowSymbols[2].setDependencies(gainUpdMinValue, [symbol_id + "_GAIN_VLOW_0"])

        pCoupPRIMELineDriver = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_LINE_DRIVER", pCoupPRIMEMenuChn[chn_idx])
        pCoupPRIMELineDriver.setLabel("PLC Line Driver settings")
        pCoupPRIMELineDriver.setDefaultValue(drv_conf_cena)
        pCoupPRIMELineDriver.setReadOnly(True)

    #### Double channel ########################################################
    for chn_idx in range(7):
        pCoupPRIMEMenu2Chn.append(pCoupComponentCommon.createMenuSymbol("DRV_PLC_PRIME_MENU_2CH" + str(chn_idx + 1) + str(chn_idx + 2), pCoupPRIMETXChannels))
        pCoupPRIMEMenu2Chn[chn_idx].setLabel("Channels " + str(chn_idx + 1) + " - " + str(chn_idx + 2))
        pCoupPRIMEMenu2Chn[chn_idx].setDescription("Coupling Settings for channels " + str(chn_idx + 1) + " - " + str(chn_idx + 2))
        pCoupPRIMEMenu2Chn[chn_idx].setVisible(False)
        pCoupPRIMEMenu2Chn[chn_idx].setHelp(srv_pcoup_helpkeyword)

        symbol_id = "SRV_PCOUP_PRIME_2CHN" + str(chn_idx + 1) + str(chn_idx + 2)

        pCoupPRIMENum2ChnEnable = pCoupComponentCommon.createBooleanSymbol(symbol_id, pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMENum2ChnEnable.setLabel("Double Channel Enable")
        pCoupPRIMENum2ChnEnable.setDefaultValue(False)
        pCoupPRIMENum2ChnEnable.setVisible(False)

        pCoupPRIMENumTxLvl = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_NUM_TX_LVL", pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMENumTxLvl.setLabel("Number of TX levels")
        pCoupPRIMENumTxLvl.setDefaultValue(8)
        pCoupPRIMENumTxLvl.setMax(1)
        pCoupPRIMENumTxLvl.setMax(8)
        pCoupPRIMENumTxLvl.setHelp(srv_pcoup_helpkeyword)

        pCoupPRIMERMSHigh = pCoupComponentCommon.createMenuSymbol(symbol_id + "_RMS_HIGH", pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMERMSHigh.setLabel("Target RMS values in HIGH mode for dynamic Tx gain")
        pCoupPRIMERMSHigh.setHelp(srv_pcoup_helpkeyword)
        pCoupPRIMERMSVlow = pCoupComponentCommon.createMenuSymbol(symbol_id + "_RMS_VLOW", pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMERMSVlow.setLabel("Target RMS values in VLOW mode for dynamic Tx gain")
        pCoupPRIMERMSVlow.setHelp(srv_pcoup_helpkeyword)

        for idx in range(8):
            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_RMS_HIGH_" + str(idx), pCoupPRIMERMSHigh)
            pCoupPRIMESymbol.setLabel("RMS_HIGH_" + str(idx))
            pCoupPRIMESymbol.setDefaultValue(rms_high_2chn_drv_sb[chn_idx][idx])
            pCoupPRIMESymbol.setMin(0)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)

            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_RMS_VLOW_" + str(idx), pCoupPRIMERMSVlow)
            pCoupPRIMESymbol.setLabel("RMS_VLOW_" + str(idx))
            pCoupPRIMESymbol.setDefaultValue(rms_vlow_2chn_drv_sb[chn_idx][idx])
            pCoupPRIMESymbol.setMin(0)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)

        pCoupPRIMETHRSHigh = pCoupComponentCommon.createMenuSymbol(symbol_id + "_THRS_HIGH", pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMETHRSHigh.setLabel("Threshold RMS values in HIGH mode for dynamic Tx mode")
        pCoupPRIMETHRSHigh.setHelp(srv_pcoup_helpkeyword)
        pCoupPRIMETHRSVlow = pCoupComponentCommon.createMenuSymbol(symbol_id + "_THRS_VLOW", pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMETHRSVlow.setLabel("Threshold RMS values in VLOW mode for dynamic Tx mode")
        pCoupPRIMETHRSVlow.setHelp(srv_pcoup_helpkeyword)

        for idx in range(16):
            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_THRS_HIGH_" + str(idx), pCoupPRIMETHRSHigh)
            pCoupPRIMESymbol.setDefaultValue(thrs_high_2chn_drv_sb[chn_idx][idx])
            pCoupPRIMESymbol.setMin(0)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            if (idx < 8):
                pCoupPRIMESymbol.setLabel("THRESHOLD_HIGH_LOW_" + str(idx))
            else:
                pCoupPRIMESymbol.setLabel("THRESHOLD_HIGH_VLOW_" + str(idx - 8))

            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_THRS_VLOW_" + str(idx), pCoupPRIMETHRSVlow)
            pCoupPRIMESymbol.setDefaultValue(thrs_vlow_2chn_drv_sb[chn_idx][idx])
            pCoupPRIMESymbol.setMin(0)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            if (idx < 8):
                pCoupPRIMESymbol.setLabel("THRESHOLD_VLOW_LOW_" + str(idx))
            else:
                pCoupPRIMESymbol.setLabel("THRESHOLD_VLOW_HIGH_" + str(idx - 8))

        pCoupPRIMEGainHigh = pCoupComponentCommon.createMenuSymbol(symbol_id + "_GAIN_HIGH", pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMEGainHigh.setLabel("Tx gain values for HIGH mode")
        pCoupPRIMEGainHigh.setHelp(srv_pcoup_helpkeyword)
        pCoupPRIMEGainVlow = pCoupComponentCommon.createMenuSymbol(symbol_id + "_GAIN_VLOW", pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMEGainVlow.setLabel("Tx gain values for VLOW mode")
        pCoupPRIMEGainVlow.setHelp(srv_pcoup_helpkeyword)

        pCoupPRIMEGainHighSymbols = []
        pCoupPRIMEGainVlowSymbols = []
        for idx in range(3):
            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_GAIN_HIGH_" + str(idx), pCoupPRIMEGainHigh)
            pCoupPRIMESymbol.setDefaultValue(gain_high_2chn_drv_sb[chn_idx][idx])
            pCoupPRIMESymbol.setMax(0xFFFF)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            pCoupPRIMEGainHighSymbols.append(pCoupPRIMESymbol)

            pCoupPRIMESymbol = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_GAIN_VLOW_" + str(idx), pCoupPRIMEGainVlow)
            pCoupPRIMESymbol.setDefaultValue(gain_vlow_2chn_drv_sb[chn_idx][idx])
            pCoupPRIMESymbol.setMax(0xFFFF)
            pCoupPRIMESymbol.setHelp(srv_pcoup_helpkeyword)
            pCoupPRIMEGainVlowSymbols.append(pCoupPRIMESymbol)

        pCoupPRIMEGainHighSymbols[0].setLabel("GAIN_HIGH_INI")
        pCoupPRIMEGainHighSymbols[0].setDependencies(gainUpdMinValue, [symbol_id + "_GAIN_HIGH_1"])
        pCoupPRIMEGainHighSymbols[1].setLabel("GAIN_HIGH_MIN")
        pCoupPRIMEGainHighSymbols[1].setMin(1)
        pCoupPRIMEGainHighSymbols[2].setLabel("GAIN_HIGH_MAX")
        pCoupPRIMEGainHighSymbols[2].setDependencies(gainUpdMinValue, [symbol_id + "_GAIN_HIGH_0"])

        pCoupPRIMEGainVlowSymbols[0].setLabel("GAIN_VLOW_INI")
        pCoupPRIMEGainVlowSymbols[0].setDependencies(gainUpdMinValue, [symbol_id + "_GAIN_VLOW_1"])
        pCoupPRIMEGainVlowSymbols[1].setLabel("GAIN_VLOW_MIN")
        pCoupPRIMEGainVlowSymbols[1].setMin(1)
        pCoupPRIMEGainVlowSymbols[2].setLabel("GAIN_VLOW_MAX")
        pCoupPRIMEGainVlowSymbols[2].setDependencies(gainUpdMinValue, [symbol_id + "_GAIN_VLOW_0"])

        pCoupPRIMELineDriver = pCoupComponentCommon.createIntegerSymbol(symbol_id + "_LINE_DRIVER", pCoupPRIMEMenu2Chn[chn_idx])
        pCoupPRIMELineDriver.setLabel("PLC Line Driver settings")
        pCoupPRIMELineDriver.setDefaultValue(drv_conf_cena)
        pCoupPRIMELineDriver.setReadOnly(True)
        pCoupPRIMELineDriver.setHelp(srv_pcoup_helpkeyword)

    #### Phy Coupling Files ######################################################
    global pCoupG3SourceFile
    pCoupG3SourceFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_G3_SOURCE", None)
    pCoupG3SourceFile.setSourcePath("service/pcoup/templates/srv_pcoup_g3.c.ftl")
    pCoupG3SourceFile.setOutputName("srv_pcoup.c")
    pCoupG3SourceFile.setDestPath("service/pcoup")
    pCoupG3SourceFile.setProjectPath("config/" + configName + "/service/pcoup/")
    pCoupG3SourceFile.setType("SOURCE")
    pCoupG3SourceFile.setMarkup(True)
    pCoupG3SourceFile.setOverwrite(True)
    pCoupG3SourceFile.setEnabled(True)

    global pCoupG3HeaderFile
    pCoupG3HeaderFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_G3_HEADER", None)
    pCoupG3HeaderFile.setSourcePath("service/pcoup/templates/srv_pcoup_g3.h.ftl")
    pCoupG3HeaderFile.setOutputName("srv_pcoup.h")
    pCoupG3HeaderFile.setDestPath("service/pcoup")
    pCoupG3HeaderFile.setProjectPath("config/" + configName + "/service/pcoup/")
    pCoupG3HeaderFile.setType("HEADER")
    pCoupG3HeaderFile.setMarkup(True)
    pCoupG3HeaderFile.setOverwrite(True)
    pCoupG3HeaderFile.setEnabled(True)

    global pCoupPRIMESourceFile
    pCoupPRIMESourceFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_PRIME_SOURCE", None)
    pCoupPRIMESourceFile.setSourcePath("service/pcoup/templates/srv_pcoup_prime.c.ftl")
    pCoupPRIMESourceFile.setOutputName("srv_pcoup.c")
    pCoupPRIMESourceFile.setDestPath("service/pcoup")
    pCoupPRIMESourceFile.setProjectPath("config/" + configName + "/service/pcoup/")
    pCoupPRIMESourceFile.setType("SOURCE")
    pCoupPRIMESourceFile.setMarkup(True)
    pCoupPRIMESourceFile.setOverwrite(True)
    pCoupPRIMESourceFile.setEnabled(False)

    global pCoupPRIMEHeaderFile
    pCoupPRIMEHeaderFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_PRIME_HEADER", None)
    pCoupPRIMEHeaderFile.setSourcePath("service/pcoup/templates/srv_pcoup_prime.h.ftl")
    pCoupPRIMEHeaderFile.setOutputName("srv_pcoup.h")
    pCoupPRIMEHeaderFile.setDestPath("service/pcoup")
    pCoupPRIMEHeaderFile.setProjectPath("config/" + configName + "/service/pcoup/")
    pCoupPRIMEHeaderFile.setType("HEADER")
    pCoupPRIMEHeaderFile.setMarkup(True)
    pCoupPRIMEHeaderFile.setOverwrite(True)
    pCoupPRIMEHeaderFile.setEnabled(False)
    
    global pCoupMMSourceFile
    pCoupMMSourceFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_MM_SOURCE", None)
    pCoupMMSourceFile.setSourcePath("service/pcoup/templates/srv_pcoup_mm.c.ftl")
    pCoupMMSourceFile.setOutputName("srv_pcoup.c")
    pCoupMMSourceFile.setDestPath("service/pcoup")
    pCoupMMSourceFile.setProjectPath("config/" + configName + "/service/pcoup/")
    pCoupMMSourceFile.setType("SOURCE")
    pCoupMMSourceFile.setMarkup(True)
    pCoupMMSourceFile.setOverwrite(True)
    pCoupMMSourceFile.setEnabled(False)

    global pCoupMMHeaderFile
    pCoupMMHeaderFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_MM_HEADER", None)
    pCoupMMHeaderFile.setSourcePath("service/pcoup/templates/srv_pcoup_mm.h.ftl")
    pCoupMMHeaderFile.setOutputName("srv_pcoup.h")
    pCoupMMHeaderFile.setDestPath("service/pcoup")
    pCoupMMHeaderFile.setProjectPath("config/" + configName + "/service/pcoup/")
    pCoupMMHeaderFile.setType("HEADER")
    pCoupMMHeaderFile.setMarkup(True)
    pCoupMMHeaderFile.setOverwrite(True)
    pCoupMMHeaderFile.setEnabled(False)

    #### FreeMaker System Files ######################################################

    pCoupSystemDefFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_SYSTEM_DEF", None)
    pCoupSystemDefFile.setType("STRING")
    pCoupSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pCoupSystemDefFile.setSourcePath("service/pcoup/templates/system/system_definitions.h.ftl")
    pCoupSystemDefFile.setMarkup(True)
