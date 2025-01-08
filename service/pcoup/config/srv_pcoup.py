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

rms_high_cena  = [2226, 1586, 1132, 805, 573, 408, 290, 206]
rms_vlow_cena  = [5920, 4604, 3331, 2374, 1686, 1193, 846, 599]
thrs_high_cena = [0, 0, 0, 0, 0, 0, 0, 0, 1884, 1341, 955, 677, 483, 341, 243, 173]
thrs_vlow_cena = [0, 0, 0, 0, 0, 0, 0, 0, 9551, 6881, 4936, 3541, 2532, 1805, 1290, 922]
dacc_cena      = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x9D5C00FF, 0x14141414, 0, 0, 0x00000004, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_cena = [126, 60, 336]
gain_vlow_cena = [532, 230, 597]
drv_conf_cena  = 5

rms_high_cenb  = [1126, 800, 568, 404, 287, 204, 146, 104]
rms_vlow_cenb  = [3900, 2805, 1996, 1418, 1007, 715, 509, 362]
thrs_high_cenb = [0, 0, 0, 0, 0, 0, 0, 0, 998, 709, 504, 358, 254, 182, 129, 93]
thrs_vlow_cenb = [0, 0, 0, 0, 0, 0, 0, 0, 3900, 2805, 1996, 1418, 1007, 715, 509, 362]
dacc_cenb      = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x9C1800FF, 0x19191919, 0, 0, 0x00000004, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_cenb = [221, 110, 496]
gain_vlow_cenb = [786, 350, 883]
drv_conf_cenb  = 5

rms_high_fcc   = [1313, 937, 667, 477, 342, 247, 180, 131]
rms_vlow_fcc   = [4329, 3314, 2387, 1692, 1201, 853, 608, 432]
thrs_high_fcc  = [0, 0, 0, 0, 0, 0, 0, 0, 1025, 729, 519, 372, 265, 191, 140, 101]
thrs_vlow_fcc  = [0, 0, 0, 0, 0, 0, 0, 0, 10242, 7302, 5197, 3708, 2649, 1906, 1366, 979]
dacc_fcc       = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x4F5000FF, 0x1B1B1B1B, 0, 0, 0x00000006, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_fcc  = [49, 20, 256]
gain_vlow_fcc  = [364, 180, 408]
drv_conf_fcc   = 5

rms_high_fcc_himp   = [1064, 763, 549, 394, 283, 204, 148, 108]
rms_vlow_fcc_himp   = [3614, 2775, 2009, 1431, 1019, 725, 516, 367]
thrs_high_fcc_himp  = [0, 0, 0, 0, 0, 0, 0, 0, 929, 668, 480, 345, 247, 179, 129, 94]
thrs_vlow_fcc_himp  = [0, 0, 0, 0, 0, 0, 0, 0, 9668, 6931, 4955, 3538, 2520, 1793, 1276, 909]
dacc_fcc_himp       = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x4F5000FF, 0x1B1B1B1B, 0, 0, 0x00000006, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_fcc_himp  = [105, 50, 256]
gain_vlow_fcc_himp  = [364, 180, 408]
drv_conf_fcc_himp   = 7

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

# dacc_chn1_drv_full = [0x00000000, 0x00002120, 0x0000073F, 0x00003F3F, 0x00000333, 0x00000000, 0xA92C00FF, 0x1A1A1A1A, \
# 				      0x00002020, 0x00000044, 0x0FD20005, 0x00000355, 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF] #TBD

rms_high_chn1_c07  = [1725, 1522, 1349, 1202, 1071, 957, 855, 764]
rms_vlow_chn1_c07  = [4874, 4427, 3986, 3555, 3157, 2795, 2470, 2184]
thrs_high_chn1_c07 = [0, 0, 0, 0, 0, 0, 0, 0, 1467, 1292, 1145, 1019, 910, 811, 725, 648]
thrs_vlow_chn1_c07 = [0, 0, 0, 0, 0, 0, 0, 0, 8479, 7515, 6665, 5874, 5192, 4576, 4030, 3557]
gain_high_chn1_c07 = [81, 40, 128]
gain_vlow_chn1_c07 = [256, 128, 281]

rms_high_chn1_drv_sb  = [2581, 2301, 2053, 1831, 1633, 1456, 1297, 1157]
rms_vlow_chn1_drv_sb  = [4965, 4547, 4130, 3719, 3329, 2973, 2650, 2362]
thrs_high_chn1_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 2115, 1886, 1683, 1502, 1339, 1194, 1064, 948]
thrs_vlow_chn1_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 8010, 7179, 6390, 5723, 5105, 4547, 4061, 3615]
gain_high_chn1_drv_sb = [81, 40, 128]
gain_vlow_chn1_drv_sb = [256, 128, 281]

rms_high_chn1_drv_aux  = rms_high_chn1_c07
rms_vlow_chn1_drv_aux  = rms_vlow_chn1_c07
thrs_high_chn1_drv_aux = thrs_high_chn1_c07
thrs_vlow_chn1_drv_aux = thrs_vlow_chn1_c07
gain_high_chn1_drv_aux = gain_high_chn1_c07
gain_vlow_chn1_drv_aux = gain_vlow_chn1_c07

# rms_high_chn1_drv_full  = [2226, 1586, 1132, 805, 573, 408, 290, 206] #TBD
# rms_vlow_chn1_drv_full  = [5920, 4604, 3331, 2374, 1686, 1193, 846, 599] #TBD
# thrs_high_chn1_drv_full = [0, 0, 0, 0, 0, 0, 0, 0, 1884, 1341, 955, 677, 483, 341, 243, 173] #TBD
# thrs_vlow_chn1_drv_full = [0, 0, 0, 0, 0, 0, 0, 0, 9551, 6881, 4936, 3541, 2532, 1805, 1290, 922] #TBD
# gain_high_chn1_drv_full = [126, 60, 336] #TBD
# gain_vlow_chn1_drv_full = [532, 230, 597] #TBD
# drv_conf_chn1_drv_full  = 5 #TBD

#### Channels 2 - 8 ########################################################
dacc_chn_fcc_c06      = [0x00000000, 0x10102120, 0x033F073F, 0x3F3F3F3F, 0x00000FFF, 0x00000000, 0xFFFF00FF, 0x1B1B1B1B, \
                         0x10101010, 0x00001111, 0x04380006, 0x000003AA, 0xF0000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]

dacc_chn_fcc_c11      = [0x00000000, 0x21202120, 0x073F073F, 0x3F3F3F3F, 0x00000FFF, 0x00000000, 0xFFFF00FF, 0x1B1B1B1B, \
                         0x10101010, 0x00001111, 0x04380006, 0x00000355, 0x0F000000, 0x001020FF, 0x000003AA, 0xF0000000, 0x001020FF]

dacc_chn_fcc_drv      = [0x00000000, 0x00000000, 0x00000100, 0x00000100, 0x00000000, 0x00000000, 0xFFFF00FF, 0x1B1B1B1B, \
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

rms_high_chn2_drv_sb = [737, 654, 581, 517, 460, 409, 364, 323]
rms_high_chn3_drv_sb = [746, 661, 584, 516, 457, 404, 358, 318]
rms_high_chn4_drv_sb = [1610, 1443, 1294, 1160, 1040, 932, 835, 748]
rms_high_chn5_drv_sb = [1794, 1602, 1430, 1277, 1141, 1019, 910, 813]
rms_high_chn6_drv_sb = [1243, 1108, 987, 880, 784, 699, 623, 556]
rms_high_chn7_drv_sb = [1441, 1280, 1137, 1010, 897, 798, 710, 631]
rms_high_chn8_drv_sb = [894, 794, 705, 626, 556, 495, 440, 392]

rms_vlow_chn2_drv_sb = [3492, 3124, 2766, 2430, 2122, 1853, 1622, 1425]
rms_vlow_chn3_drv_sb = [3573, 3288, 2997, 2703, 2413, 2145, 1905, 1690]
rms_vlow_chn4_drv_sb = [3465, 3160, 2854, 2552, 2271, 2018, 1793, 1593]
rms_vlow_chn5_drv_sb = [3749, 3421, 3087, 2758, 2455, 2182, 1937, 1719]
rms_vlow_chn6_drv_sb = [3694, 3368, 3037, 2713, 2416, 2149, 1911, 1698]
rms_vlow_chn7_drv_sb = [3277, 2986, 2692, 2406, 2145, 1911, 1703, 1517]
rms_vlow_chn8_drv_sb = [3016, 2770, 2524, 2276, 2035, 1815, 1620, 1446]

thrs_high_chn2_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 668, 595, 529, 469, 415, 370, 329, 292]
thrs_high_chn3_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 561, 496, 439, 388, 344, 304, 270, 239]
thrs_high_chn4_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1285, 1152, 1034, 927, 831, 744, 666, 597]
thrs_high_chn5_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1557, 1392, 1241, 1108, 990, 885, 790, 706]
thrs_high_chn6_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1081, 963, 858, 765, 682, 607, 541, 483]
thrs_high_chn7_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1226, 1088, 967, 859, 764, 679, 603, 537]
thrs_high_chn8_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 688, 611, 543, 481, 429, 381, 338, 302]

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

rms_high_chn2_drv_himp = [737, 654, 581, 517, 460, 409, 364, 323]
rms_high_chn3_drv_himp = [800, 697, 608, 532, 466, 409, 359, 315]
rms_high_chn4_drv_himp = [1311, 1171, 1047, 936, 837, 748, 669, 597]
rms_high_chn5_drv_himp = [1831, 1629, 1453, 1297, 1159, 1035, 925, 827]
rms_high_chn6_drv_himp = [1707, 1520, 1360, 1219, 1096, 987, 890, 803]
rms_high_chn7_drv_himp = [1248, 1125, 1016, 917, 830, 752, 682, 618]
rms_high_chn8_drv_himp = [953, 868, 784, 706, 635, 573, 516, 466]

rms_vlow_chn2_drv_himp = [3492, 3124, 2766, 2430, 2122, 1853, 1622, 1425]
rms_vlow_chn3_drv_himp = [2459, 2265, 2069, 1868, 1676, 1495, 1330, 1183]
rms_vlow_chn4_drv_himp = [3124, 2868, 2609, 2358, 2114, 1882, 1673, 1486]
rms_vlow_chn5_drv_himp = [3344, 3048, 2749, 2456, 2188, 1948, 1734, 1543]
rms_vlow_chn6_drv_himp = [3074, 2814, 2546, 2283, 2042, 1825, 1632, 1459]
rms_vlow_chn7_drv_himp = [2553, 2339, 2129, 1924, 1723, 1542, 1379, 1233]
rms_vlow_chn8_drv_himp = [1838, 1668, 1500, 1345, 1203, 1076, 963, 861]

thrs_high_chn2_drv_himp = thrs_high_dummy
thrs_high_chn3_drv_himp = thrs_high_dummy
thrs_high_chn4_drv_himp = thrs_high_dummy
thrs_high_chn5_drv_himp = thrs_high_dummy
thrs_high_chn6_drv_himp = [0, 0, 0, 0, 0, 0, 0, 0, 1485, 1321, 1181, 1060, 951, 857, 774, 697]
thrs_high_chn7_drv_himp = thrs_high_dummy
thrs_high_chn8_drv_himp = thrs_high_dummy

thrs_vlow_chn2_drv_himp = thrs_vlow_dummy
thrs_vlow_chn3_drv_himp = thrs_vlow_dummy
thrs_vlow_chn4_drv_himp = thrs_vlow_dummy
thrs_vlow_chn5_drv_himp = thrs_vlow_dummy
thrs_vlow_chn6_drv_himp = thrs_vlow_dummy
thrs_vlow_chn7_drv_himp = thrs_vlow_dummy
thrs_vlow_chn8_drv_himp = thrs_vlow_dummy

gain_high_chn2_drv_himp = [120, 120, 120]
gain_vlow_chn2_drv_himp = [256, 256, 256]
gain_high_chn3_drv_himp = [120, 120, 120]
gain_vlow_chn3_drv_himp = [287, 287, 287]
gain_high_chn4_drv_himp = [85, 85, 85]
gain_vlow_chn4_drv_himp = [287, 287, 287]
gain_high_chn5_drv_himp = [120, 120, 120]
gain_vlow_chn5_drv_himp = [256, 256, 256]
gain_high_chn6_drv_himp = [120, 30, 256]
gain_vlow_chn6_drv_himp = [256, 128, 287]
gain_high_chn7_drv_himp = [169, 169, 169]
gain_vlow_chn7_drv_himp = [287, 287, 287]
gain_high_chn8_drv_himp = [240, 240, 240]
gain_vlow_chn8_drv_himp = [256, 256, 256]

rms_high_drv_himp = [rms_high_chn2_drv_himp, rms_high_chn3_drv_himp, rms_high_chn4_drv_himp, rms_high_chn5_drv_himp, rms_high_chn6_drv_himp, rms_high_chn7_drv_himp, rms_high_chn8_drv_himp]
rms_vlow_drv_himp = [rms_vlow_chn2_drv_himp, rms_vlow_chn3_drv_himp, rms_vlow_chn4_drv_himp, rms_vlow_chn5_drv_himp, rms_vlow_chn6_drv_himp, rms_vlow_chn7_drv_himp, rms_vlow_chn8_drv_himp]
thrs_high_drv_himp = [thrs_high_chn2_drv_himp, thrs_high_chn3_drv_himp, thrs_high_chn4_drv_himp, thrs_high_chn5_drv_himp, thrs_high_chn6_drv_himp, thrs_high_chn7_drv_himp, thrs_high_chn8_drv_himp]
thrs_vlow_drv_himp = [thrs_vlow_chn2_drv_himp, thrs_vlow_chn3_drv_himp, thrs_vlow_chn4_drv_himp, thrs_vlow_chn5_drv_himp, thrs_vlow_chn6_drv_himp, thrs_vlow_chn7_drv_himp, thrs_vlow_chn8_drv_himp]
gain_high_drv_himp = [gain_high_chn2_drv_himp, gain_high_chn3_drv_himp, gain_high_chn4_drv_himp, gain_high_chn5_drv_himp, gain_high_chn6_drv_himp, gain_high_chn7_drv_himp, gain_high_chn8_drv_himp]
gain_vlow_drv_himp = [gain_vlow_chn2_drv_himp, gain_vlow_chn3_drv_himp, gain_vlow_chn4_drv_himp, gain_vlow_chn5_drv_himp, gain_vlow_chn6_drv_himp, gain_vlow_chn7_drv_himp, gain_vlow_chn8_drv_himp]

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

rms_high_2chn12_drv_sb = [417, 370, 329, 293, 260, 232, 206, 183]
rms_high_2chn23_drv_sb = [884, 787, 701, 624, 556, 495, 441, 392]
rms_high_2chn34_drv_sb = [1143, 1017, 906, 808, 722, 644, 575, 512]
rms_high_2chn45_drv_sb = [1206, 1074, 957, 853, 760, 678, 604, 539]
rms_high_2chn56_drv_sb = [1229, 1095, 976, 870, 775, 691, 616, 550]
rms_high_2chn67_drv_sb = [1199, 1066, 950, 848, 757, 676, 603, 537]
rms_high_2chn78_drv_sb = [1045, 931, 831, 742, 662, 591, 527, 471]

rms_vlow_2chn12_drv_sb = [1826, 1629, 1438, 1265, 1113, 982, 870, 771]
rms_vlow_2chn23_drv_sb = [2729, 2441, 2153, 1890, 1653, 1447, 1269, 1120]
rms_vlow_2chn34_drv_sb = [3015, 2685, 2362, 2072, 1819, 1597, 1407, 1246]
rms_vlow_2chn45_drv_sb = [3138, 2804, 2480, 2186, 1928, 1701, 1506, 1338]
rms_vlow_2chn56_drv_sb = [3143, 2823, 2513, 2232, 1983, 1763, 1572, 1405]
rms_vlow_2chn67_drv_sb = [3238, 2919, 2607, 2325, 2074, 1849, 1651, 1476]
rms_vlow_2chn78_drv_sb = [3453, 3108, 2773, 2473, 2205, 1967, 1756, 1571]

thrs_high_2chn12_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 354, 315, 281, 249, 221, 196, 175, 156]
thrs_high_2chn23_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 731, 650, 580, 517, 460, 411, 365, 326]
thrs_high_2chn34_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 981, 874, 778, 692, 616, 549, 489, 436]
thrs_high_2chn45_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1024, 912, 814, 725, 647, 577, 515, 459]
thrs_high_2chn56_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1041, 929, 829, 739, 657, 585, 521, 465]
thrs_high_2chn67_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 1012, 902, 806, 719, 641, 571, 509, 454]
thrs_high_2chn78_drv_sb = [0, 0, 0, 0, 0, 0, 0, 0, 895, 798, 713, 635, 567, 505, 451, 402]

thrs_vlow_2chn12_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn23_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn34_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn45_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn56_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn67_drv_sb = thrs_vlow_dummy
thrs_vlow_2chn78_drv_sb = thrs_vlow_dummy

gain_high_2chn12_drv_sb = [120, 60, 256]
gain_vlow_2chn12_drv_sb = [256, 128, 256]
gain_high_2chn23_drv_sb = [120, 60, 256]
gain_vlow_2chn23_drv_sb = [256, 128, 256]
gain_high_2chn34_drv_sb = [120, 60, 256]
gain_vlow_2chn34_drv_sb = [256, 128, 256]
gain_high_2chn45_drv_sb = [120, 60, 256]
gain_vlow_2chn45_drv_sb = [256, 128, 256]
gain_high_2chn56_drv_sb = [120, 60, 256]
gain_vlow_2chn56_drv_sb = [256, 128, 256]
gain_high_2chn67_drv_sb = [120, 60, 256]
gain_vlow_2chn67_drv_sb = [256, 128, 256]
gain_high_2chn78_drv_sb = [120, 60, 256]
gain_vlow_2chn78_drv_sb = [256, 128, 256]

rms_high_2chn_drv_sb = [rms_high_2chn12_drv_sb, rms_high_2chn23_drv_sb, rms_high_2chn34_drv_sb, rms_high_2chn45_drv_sb, rms_high_2chn56_drv_sb, rms_high_2chn67_drv_sb, rms_high_2chn78_drv_sb]
rms_vlow_2chn_drv_sb = [rms_vlow_2chn12_drv_sb, rms_vlow_2chn23_drv_sb, rms_vlow_2chn34_drv_sb, rms_vlow_2chn45_drv_sb, rms_vlow_2chn56_drv_sb, rms_vlow_2chn67_drv_sb, rms_vlow_2chn78_drv_sb]
thrs_high_2chn_drv_sb = [thrs_high_2chn12_drv_sb, thrs_high_2chn23_drv_sb, thrs_high_2chn34_drv_sb, thrs_high_2chn45_drv_sb, thrs_high_2chn56_drv_sb, thrs_high_2chn67_drv_sb, thrs_high_2chn78_drv_sb]
thrs_vlow_2chn_drv_sb = [thrs_vlow_2chn12_drv_sb, thrs_vlow_2chn23_drv_sb, thrs_vlow_2chn34_drv_sb, thrs_vlow_2chn45_drv_sb, thrs_vlow_2chn56_drv_sb, thrs_vlow_2chn67_drv_sb, thrs_vlow_2chn78_drv_sb]
gain_high_2chn_drv_sb = [gain_high_2chn12_drv_sb, gain_high_2chn23_drv_sb, gain_high_2chn34_drv_sb, gain_high_2chn45_drv_sb, gain_high_2chn56_drv_sb, gain_high_2chn67_drv_sb, gain_high_2chn78_drv_sb]
gain_vlow_2chn_drv_sb = [gain_vlow_2chn12_drv_sb, gain_vlow_2chn23_drv_sb, gain_vlow_2chn34_drv_sb, gain_vlow_2chn45_drv_sb, gain_vlow_2chn56_drv_sb, gain_vlow_2chn67_drv_sb, gain_vlow_2chn78_drv_sb]

rms_high_2chn12_drv_himp = rms_high_2chn12_drv_sb
rms_high_2chn23_drv_himp = rms_high_2chn23_drv_sb
rms_high_2chn34_drv_himp = rms_high_2chn34_drv_sb
rms_high_2chn45_drv_himp = rms_high_2chn45_drv_sb
rms_high_2chn56_drv_himp = rms_high_2chn56_drv_sb
rms_high_2chn67_drv_himp = rms_high_2chn67_drv_sb
rms_high_2chn78_drv_himp = rms_high_2chn78_drv_sb

rms_vlow_2chn12_drv_himp = rms_vlow_2chn12_drv_sb
rms_vlow_2chn23_drv_himp = rms_vlow_2chn23_drv_sb
rms_vlow_2chn34_drv_himp = rms_vlow_2chn34_drv_sb
rms_vlow_2chn45_drv_himp = rms_vlow_2chn45_drv_sb
rms_vlow_2chn56_drv_himp = rms_vlow_2chn56_drv_sb
rms_vlow_2chn67_drv_himp = rms_vlow_2chn67_drv_sb
rms_vlow_2chn78_drv_himp = rms_vlow_2chn78_drv_sb

thrs_high_2chn12_drv_himp = thrs_high_dummy
thrs_high_2chn23_drv_himp = thrs_high_dummy
thrs_high_2chn34_drv_himp = thrs_high_dummy
thrs_high_2chn45_drv_himp = thrs_high_dummy
thrs_high_2chn56_drv_himp = thrs_high_dummy
thrs_high_2chn67_drv_himp = thrs_high_dummy
thrs_high_2chn78_drv_himp = thrs_high_dummy

thrs_vlow_2chn12_drv_himp = thrs_vlow_dummy
thrs_vlow_2chn23_drv_himp = thrs_vlow_dummy
thrs_vlow_2chn34_drv_himp = thrs_vlow_dummy
thrs_vlow_2chn45_drv_himp = thrs_vlow_dummy
thrs_vlow_2chn56_drv_himp = thrs_vlow_dummy
thrs_vlow_2chn67_drv_himp = thrs_vlow_dummy
thrs_vlow_2chn78_drv_himp = thrs_vlow_dummy

gain_high_2chn12_drv_himp = [120, 60, 256]
gain_vlow_2chn12_drv_himp = [256, 128, 256]
gain_high_2chn23_drv_himp = [120, 60, 256]
gain_vlow_2chn23_drv_himp = [256, 128, 256]
gain_high_2chn34_drv_himp = [120, 60, 256]
gain_vlow_2chn34_drv_himp = [256, 128, 256]
gain_high_2chn45_drv_himp = [120, 60, 256]
gain_vlow_2chn45_drv_himp = [256, 128, 256]
gain_high_2chn56_drv_himp = [120, 60, 256]
gain_vlow_2chn56_drv_himp = [256, 128, 256]
gain_high_2chn67_drv_himp = [120, 60, 256]
gain_vlow_2chn67_drv_himp = [256, 128, 256]
gain_high_2chn78_drv_himp = [120, 60, 256]
gain_vlow_2chn78_drv_himp = [256, 128, 256]

rms_high_2chn_drv_himp = [rms_high_2chn12_drv_himp, rms_high_2chn23_drv_himp, rms_high_2chn34_drv_himp, rms_high_2chn45_drv_himp, rms_high_2chn56_drv_himp, rms_high_2chn67_drv_himp, rms_high_2chn78_drv_himp]
rms_vlow_2chn_drv_himp = [rms_vlow_2chn12_drv_himp, rms_vlow_2chn23_drv_himp, rms_vlow_2chn34_drv_himp, rms_vlow_2chn45_drv_himp, rms_vlow_2chn56_drv_himp, rms_vlow_2chn67_drv_himp, rms_vlow_2chn78_drv_himp]
thrs_high_2chn_drv_himp = [thrs_high_2chn12_drv_himp, thrs_high_2chn23_drv_himp, thrs_high_2chn34_drv_himp, thrs_high_2chn45_drv_himp, thrs_high_2chn56_drv_himp, thrs_high_2chn67_drv_himp, thrs_high_2chn78_drv_himp]
thrs_vlow_2chn_drv_himp = [thrs_vlow_2chn12_drv_himp, thrs_vlow_2chn23_drv_himp, thrs_vlow_2chn34_drv_himp, thrs_vlow_2chn45_drv_himp, thrs_vlow_2chn56_drv_himp, thrs_vlow_2chn67_drv_himp, thrs_vlow_2chn78_drv_himp]
gain_high_2chn_drv_himp = [gain_high_2chn12_drv_himp, gain_high_2chn23_drv_himp, gain_high_2chn34_drv_himp, gain_high_2chn45_drv_himp, gain_high_2chn56_drv_himp, gain_high_2chn67_drv_himp, gain_high_2chn78_drv_himp]
gain_vlow_2chn_drv_himp = [gain_vlow_2chn12_drv_himp, gain_vlow_2chn23_drv_himp, gain_vlow_2chn34_drv_himp, gain_vlow_2chn45_drv_himp, gain_vlow_2chn56_drv_himp, gain_vlow_2chn67_drv_himp, gain_vlow_2chn78_drv_himp]

############################################################################
#### Meters & More Coupling Parameters ####
############################################################################

pCoupMMDACC = []

# from rms_high_cena_c07
mm_rms_high_cena  = [324, 293, 267, 242, 218, 194, 172, 153]
mm_rms_vlow_cena  = [2432, 2172, 1932, 1714, 1515, 1342, 1189, 1052]
mm_thrs_high_cena = [0, 0, 0, 0, 0, 0, 0, 0, 274, 247, 225, 205, 184, 164, 146, 130]
mm_thrs_vlow_cena = [0, 0, 0, 0, 0, 0, 0, 0, 5579, 4972, 4414, 3920, 3477, 3079, 2738, 2433]
mm_dacc_cena      = [0, 0x00002120, 0x0000073F, 0x00003F3F, 0x00000333, 0, 0xFFFF00FF, 0x17171717, 0x00002020, 0x00000044, 0x0FD20001, 0x00000355, 0x0F000000, 0x001020F0, 0x00000355, 0x0F000000, 0x001020FF]
mm_dacc_cena_drv  = [0, 0x21200000, 0x073F0000, 0x3F3F0000, 0x00000CCC, 0, 0xFFFF00FF, 0x17171717, 0x20200000, 0x00004400, 0x0FD20001, 0x000003AA, 0xF0000000, 0x001020F0, 0x000003AA, 0xF0000000, 0x001020FF]
mm_gain_high_cena = [25, 12, 50]
mm_gain_vlow_cena = [228, 114, 256]
mm_drv_conf_cena  = 8

############################################################################

srv_pcoup_helpkeyword = "mcc_h3_srv_pcoup_configurations"

############################################################################
# Update PLC Profile according to PLC PHY driver configuration
############################################################################
def updateCouplingParameters():

    plcProfile = Database.getSymbolValue("drvPlcPhy", "DRV_PLC_PROFILE")

    if plcProfile == "PRIME":
        updatePRIMECouplingParameters()
    elif plcProfile == "G3-PLC":
        updateG3CouplingParameters()
    else:
        updateMMCouplingParameters()

############################################################################
#### G3 function to update the Coupling Parameters ####
############################################################################

def updateG3CouplingParameters():
    global pCoupG3RAuxBranch
    global pCoupG3MainPhyBand
    global pCoupG3AuxPhyBand
    global pCoupG3SourceFile
    global pCoupG3HeaderFile
    global pCoupPRIMESourceFile
    global pCoupPRIMEHeaderFile
    global pCoupPRIMETXChannels
    global pCoupG3TXBranches

    if Database.getSymbolValue("drvPlcPhy", "DRV_PLC_MODE") != None:
        plcDriver = "drvPlcPhy"
    elif Database.getSymbolValue("drvG3MacRt", "DRV_PLC_MODE") != None:
        plcDriver = "drvG3MacRt"
    else:
        plcDriver = ""
        print("updateG3CouplingParameters: DRV PLC MODE not found")
        return

    # Show G3 setting, hide PRIME setting
    pCoupG3TXBranches.setVisible(True)
    pCoupPRIMETXChannels.setVisible(False)

    plcDevice = Database.getSymbolValue(plcDriver, "DRV_PLC_MODE")

    # Enable G3 COUP files
    pCoupG3SourceFile.setEnabled(True)
    pCoupG3HeaderFile.setEnabled(True)
    pCoupPRIMESourceFile.setEnabled(False)
    pCoupPRIMEHeaderFile.setEnabled(False)
    pCoupMMSourceFile.setEnabled(False)
    pCoupMMHeaderFile.setEnabled(False)

    pCoupG3RAuxBranch.setVisible(False)
    auxiliaryBand = False
    pCoupG3AuxPhyBand.setValue("None")

    plcPhyBand = Database.getSymbolValue(plcDriver, "DRV_PLC_G3_BAND")
    plcHighAtt = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_G3_HIGH_ATTENUATION")
    plcMultiband = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_G3_MULTIBAND")
    plcInternal = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_G3_INTERNAL")
    plcBandAux = Database.getSymbolValue(plcDriver, "DRV_PLC_G3_BAND_AUX")

    pCoupG3MainPhyBand.setLabel("G3-PLC PHY Band")
    pCoupG3MainPhyBand.setVisible(True)
            
    if (plcDevice == "PL460"):
        if plcPhyBand == "CEN-A":
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

        elif plcPhyBand == "CEN-B":
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

        elif plcPhyBand == "FCC" or plcPhyBand == "ARIB":
            if (plcPhyBand == "FCC"):
                pCoupG3MainPhyBand.setValue("FCC")
            else:
                pCoupG3MainPhyBand.setValue("ARIB")

            if (plcHighAtt == True):
                # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB HIGHT ATT")
                rms_high  = rms_high_fcc_himp
                rms_vlow  = rms_vlow_fcc_himp
                thrs_high = thrs_high_fcc_himp
                thrs_vlow = thrs_vlow_fcc_himp
                dacc      = dacc_fcc_himp
                gain_high = gain_high_fcc_himp
                gain_vlow = gain_vlow_fcc_himp
                line_drv  = drv_conf_fcc_himp
            else:
                # print("UpdatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB")
                rms_high  = rms_high_fcc
                rms_vlow  = rms_vlow_fcc
                thrs_high = thrs_high_fcc
                thrs_vlow = thrs_vlow_fcc
                dacc      = dacc_fcc
                gain_high = gain_high_fcc
                gain_vlow = gain_vlow_fcc
                line_drv  = drv_conf_fcc

            if (plcMultiband == True):
                auxiliaryBand = True

                if (plcBandAux == "CEN-A"):
                    # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-A")
                    rms_high_aux  = rms_high_cena_c07
                    rms_vlow_aux  = rms_vlow_cena_c07
                    thrs_high_aux = thrs_high_cena_c07
                    thrs_vlow_aux = thrs_vlow_cena_c07
                    dacc_aux      = dacc_cena_c07_drv
                    gain_high_aux = gain_high_cena_c07
                    gain_vlow_aux = gain_vlow_cena_c07
                    line_drv_aux  = drv_conf_cena_c07
                    pCoupG3AuxPhyBand.setValue("CEN-A")
                else:
                    # "CEN-B"
                    # print("UpdatePlcCouplingParameters ->  PL460 G3 AUX CEN-B")
                    rms_high_aux  = rms_high_cenb_c14
                    rms_vlow_aux  = rms_vlow_cenb_c14
                    thrs_high_aux = thrs_high_cenb_c14
                    thrs_vlow_aux = thrs_vlow_cenb_c14
                    dacc_aux      = dacc_cenb_c14_drv
                    gain_high_aux = gain_high_cenb_c14
                    gain_vlow_aux = gain_vlow_cenb_c14
                    line_drv_aux  = drv_conf_cenb_c14
                    pCoupG3AuxPhyBand.setValue("CEN-B")

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

    else: # "PL360"
        line_drv = 0
        line_drv_aux = 0
        if plcPhyBand == "CEN-A":
            # print("UpdatePlcCouplingParameters -> PL360 G3 CEN-A")
            rms_high  = rms_high_cena_c07
            rms_vlow  = rms_vlow_cena_c07
            thrs_high = thrs_high_cena_c07
            thrs_vlow = thrs_vlow_cena_c07
            dacc      = dacc_cena_c07
            gain_high = gain_high_cena_c07
            gain_vlow = gain_vlow_cena_c07
            pCoupG3MainPhyBand.setValue("CEN-A")

        elif plcPhyBand == "CEN-B":
            pCoupG3MainPhyBand.setValue("CEN-B")
            if (plcInternal == True):
                # print("UpdatePlcCouplingParameters ->  PL360 G3 CEN-B Internal")
                rms_high  = rms_high_cenb_c12
                rms_vlow  = rms_vlow_cenb_c12
                thrs_high = thrs_high_cenb_c12
                thrs_vlow = thrs_vlow_cenb_c12
                dacc      = dacc_cenb_c12
                gain_high = gain_high_cenb_c12
                gain_vlow = gain_vlow_cenb_c12
            else:
                # print("UpdatePlcCouplingParameters ->  PL360 G3 CEN-B External")
                rms_high  = rms_high_cenb_c14
                rms_vlow  = rms_vlow_cenb_c14
                thrs_high = thrs_high_cenb_c14
                thrs_vlow = thrs_vlow_cenb_c14
                dacc      = dacc_cenb_c14
                gain_high = gain_high_cenb_c14
                gain_vlow = gain_vlow_cenb_c14

        elif plcPhyBand == "FCC":
            pCoupG3MainPhyBand.setValue("FCC")
            if (plcMultiband == True):
                auxiliaryBand = True
                # print("UpdatePlcCouplingParameters ->  PL360 G3 FCC SB")
                rms_high  = rms_high_fcc_c11
                rms_vlow  = rms_vlow_fcc_c11
                thrs_high = thrs_high_fcc_c11
                thrs_vlow = thrs_vlow_fcc_c11
                dacc      = dacc_fcc_c11
                gain_high = gain_high_fcc_c11
                gain_vlow = gain_vlow_fcc_c11

                # print("UpdatePlcCouplingParameters ->  PL360 G3 AUX CEN-A")
                rms_high_aux  = rms_high_cena_c07
                rms_vlow_aux  = rms_vlow_cena_c07
                thrs_high_aux = thrs_high_cena_c07
                thrs_vlow_aux = thrs_vlow_cena_c07
                dacc_aux      = dacc_cena_c07
                gain_high_aux = gain_high_cena_c07
                gain_vlow_aux = gain_vlow_cena_c07
                pCoupG3AuxPhyBand.setValue("CEN-A")
            else:
                # print("UpdatePlcCouplingParameters ->  PL360 G3 FCC 2B")
                rms_high  = rms_high_fcc_c06
                rms_vlow  = rms_vlow_fcc_c06
                thrs_high = thrs_high_fcc_c06
                thrs_vlow = thrs_vlow_fcc_c06
                dacc      = dacc_fcc_c06
                gain_high = gain_high_fcc_c06
                gain_vlow = gain_vlow_fcc_c06

        elif plcPhyBand == "ARIB":
            pCoupG3MainPhyBand.setValue("ARIB")
            if (plcMultiband == True):
                auxiliaryBand = True
                # print("UpdatePlcCouplingParameters ->  PL360 G3 ARIB SB")
                rms_high  = rms_high_arib_c11
                rms_vlow  = rms_vlow_arib_c11
                thrs_high = thrs_high_arib_c11
                thrs_vlow = thrs_vlow_arib_c11
                dacc      = dacc_arib_c11
                gain_high = gain_high_arib_c11
                gain_vlow = gain_vlow_arib_c11

                # print("UpdatePlcCouplingParameters ->  PL360 G3 AUX CEN-A")
                rms_high_aux  = rms_high_cena_c07
                rms_vlow_aux  = rms_vlow_cena_c07
                thrs_high_aux = thrs_high_cena_c07
                thrs_vlow_aux = thrs_vlow_cena_c07
                dacc_aux      = dacc_cena_c07
                gain_high_aux = gain_high_cena_c07
                gain_vlow_aux = gain_vlow_cena_c07
                pCoupG3AuxPhyBand.setValue("CEN-A")
            else:
                # print("UpdatePlcCouplingParameters ->  PL360 G3 ARIB 2B")
                rms_high  = rms_high_arib_c06
                rms_vlow  = rms_vlow_arib_c06
                thrs_high = thrs_high_arib_c06
                thrs_vlow = thrs_vlow_arib_c06
                dacc      = dacc_arib_c06
                gain_high = gain_high_arib_c06
                gain_vlow = gain_vlow_arib_c06

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

    # Update Values of the Auxiliary Branch in Configuration Window
    if (auxiliaryBand == True):
        pCoupG3RAuxBranch.setVisible(True)

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

def pCoupConfigureDACC(plcDevice, channels, auxBranch, highImp):
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
            if (cen_enable == True):
                # C11
                dacc_fcc = dacc_chn_fcc_c11
            else:
                # C06
                dacc_fcc = dacc_chn_fcc_c06

        if (channels & 0x7F00):
            double_chn_enable = True
            # Double Channels
            if (cen_enable == True):
                # C11
                dacc_2chn = dacc_2chn_fcc_c11
            else:
                # C06
                dacc_2chn = dacc_2chn_fcc_c06
    else:
        if (channels & 1):
            cen_enable = True
            if (auxBranch == True):
                # drv_aux
                dacc_cen = dacc_chn1_drv_aux
            else:
                # drv_sb
                dacc_cen = dacc_chn1_drv_sb

        if (channels & 0xFE):
            fcc_enable = True
            # Channels 2 - 8
            # drv_sb / drv_himp
            dacc_fcc = dacc_chn_fcc_drv

        if (channels & 0x7F00):
            double_chn_enable = True
            # Double Channels
            # drv_sb / drv_himp
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

def pCoupConfigureChannelDetImp(channels):
    if (channels & 1):
        return "CHN1"
    else:
        return "CHN6"

def pCoupConfigureChannel(plcDevice, channel, multiband, auxBranch, highImp):

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
            if (multiband):
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
            if (auxBranch == True):
                # drv_aux
                line_drv = 8
                rms_high  = rms_high_chn1_drv_aux
                rms_vlow  = rms_vlow_chn1_drv_aux
                thrs_high = thrs_high_chn1_drv_aux
                thrs_vlow = thrs_vlow_chn1_drv_aux
                gain_high = gain_high_chn1_drv_aux
                gain_vlow = gain_vlow_chn1_drv_aux
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
            if (highImp == True):
                # drv_hmip
                line_drv = 7
                rms_high  = rms_high_drv_himp[channel - 2]
                rms_vlow  = rms_vlow_drv_himp[channel - 2]
                thrs_high = thrs_high_drv_himp[channel - 2]
                thrs_vlow = thrs_vlow_drv_himp[channel - 2]
                gain_high = gain_high_drv_himp[channel - 2]
                gain_vlow = gain_vlow_drv_himp[channel - 2]
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

def pCoupConfigure2Channel(plcDevice, channel, multiband, highImp):

    if (plcDevice == "PL360"):
        line_drv = 0
        if (multiband):
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
        if (highImp == True):
            # drv_hmip
            line_drv = 7
            rms_high  = rms_high_2chn_drv_himp[channel]
            rms_vlow  = rms_vlow_2chn_drv_himp[channel]
            thrs_high = thrs_high_2chn_drv_himp[channel]
            thrs_vlow = thrs_vlow_2chn_drv_himp[channel]
            gain_high = gain_high_2chn_drv_himp[channel]
            gain_vlow = gain_vlow_2chn_drv_himp[channel]
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
    auxBranch = Database.getSymbolValue(plcDriver, "DRV_PLC_PRIME_BAND_AUX")
    highImp = Database.getSymbolValue(plcDriver, "DRV_PLC_PRIME_HIGH_ATTENUATION")
    multiband = (channels_sel & 1) and (channels_sel & 0x7FFE)

    # Configure DACC
    pCoupConfigureDACC(plcDevice, channels_sel, auxBranch, highImp)
    channel_imp_det = pCoupConfigureChannelDetImp(channels_sel)

    # Configure Single channels selected in PHY driver
    for idx in range(8):
        symbol_id = "SRV_PCOUP_PRIME_CHN" + str(idx + 1)
        if (channels_sel & (1 << idx)):
            pCoupConfigureChannel(plcDevice, idx + 1, multiband, auxBranch, highImp)
            pCoupPRIMEMenuChn[idx].setVisible(True)
            Database.setSymbolValue("srv_pcoup", symbol_id, True)
        else:
            pCoupPRIMEMenuChn[idx].setVisible(False)
            Database.setSymbolValue("srv_pcoup", symbol_id, False)

    # Configure Double channels selected in PHY driver
    for idx in range(7):
        symbol_id = "SRV_PCOUP_PRIME_2CHN" + str(idx + 1) + str(idx + 2)
        if (channels_sel & (1 << (idx + 8))):
            pCoupConfigure2Channel(plcDevice, idx, multiband, highImp)
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
    global pCoupG3RAuxBranch
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

    # Enable MM COUP files
    pCoupG3SourceFile.setEnabled(False)
    pCoupG3HeaderFile.setEnabled(False)
    pCoupPRIMESourceFile.setEnabled(False)
    pCoupPRIMEHeaderFile.setEnabled(False)
    pCoupMMSourceFile.setEnabled(True)
    pCoupMMHeaderFile.setEnabled(True)

    pCoupG3RAuxBranch.setVisible(False)
    pCoupG3AuxPhyBand.setValue("None")

    pCoupG3MainPhyBand.setVisible(False)
    pCoupG3MainPhyBand.setValue("CEN-A")

    rms_high  = mm_rms_high_cena
    rms_vlow  = mm_rms_vlow_cena
    thrs_high = mm_thrs_high_cena
    thrs_vlow = mm_thrs_vlow_cena
    gain_high = mm_gain_high_cena
    gain_vlow = mm_gain_vlow_cena

    if (plcDevice == "PL460"):
        # print("UpdatePlcCouplingParameters -> PL460 MM CEN-A")
        line_drv  = mm_drv_conf_cena
        dacc      = mm_dacc_cena_drv

    else: # "PL360"
        # print("UpdatePlcCouplingParameters -> PL360 MM CEN-A")
        line_drv  = 0
        dacc      = mm_dacc_cena

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
    pCoupG3MainPhyBand = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_G3_MAIN_BAND", pCoupG3MainBranch, ["CEN-A", "CEN-B", "FCC"])
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
    pCoupG3NumTxLvl.setDependencies(dependenciesHandler, ["drvPlcPhy.DRV_PLC_COUP_G3_HIGH_ATTENUATION",
                                                          "drvPlcPhy.DRV_PLC_COUP_G3_INTERNAL",
                                                          "drvPlcPhy.DRV_PLC_MODE",
                                                          "drvG3MacRt.DRV_PLC_COUP_G3_HIGH_ATTENUATION",
                                                          "drvG3MacRt.DRV_PLC_COUP_G3_INTERNAL",
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

    global pCoupG3RAuxBranch
    pCoupG3RAuxBranch = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_BRANCH", pCoupG3TXBranches)
    pCoupG3RAuxBranch.setLabel("Auxiliary Transmission branch")
    pCoupG3RAuxBranch.setVisible(False)
    pCoupG3RAuxBranch.setHelp(srv_pcoup_helpkeyword)

    global pCoupG3AuxPhyBand
    # pCoupG3AuxPhyBand = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_G3_AUX_G3_BAND", pCoupG3RAuxBranch, ["None", "CEN-A", "CEN-B", "FCC", "ARIB"])
    pCoupG3AuxPhyBand = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_G3_AUX_BAND", pCoupG3RAuxBranch, ["None", "CEN-A", "CEN-B", "FCC"])
    pCoupG3AuxPhyBand.setLabel("G3-PLC PHY Band")
    pCoupG3AuxPhyBand.setDefaultValue("CEN-A")
    pCoupG3AuxPhyBand.setReadOnly(True)
    pCoupG3AuxPhyBand.setHelp(srv_pcoup_helpkeyword)

    pCoupG3AuxNumTxLvl = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_NUM_TX_LVL", pCoupG3RAuxBranch)
    pCoupG3AuxNumTxLvl.setLabel("Number of TX levels")
    pCoupG3AuxNumTxLvl.setDefaultValue(8)
    pCoupG3AuxNumTxLvl.setHelp(srv_pcoup_helpkeyword)

    pCoupG3AuxRMSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_RMS_HIGH", pCoupG3RAuxBranch)
    pCoupG3AuxRMSHigh.setLabel("Target RMS values in HIGH mode for dynamic Tx gain")
    pCoupG3AuxRMSHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3AuxRMSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_RMS_VLOW", pCoupG3RAuxBranch)
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

    pCoupG3AuxTHRSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_THRS_HIGH", pCoupG3RAuxBranch)
    pCoupG3AuxTHRSHigh.setLabel("Threshold RMS values in HIGH mode for dynamic Tx mode")
    pCoupG3AuxTHRSHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3AuxTHRSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_THRS_VLOW", pCoupG3RAuxBranch)
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

    pCoupG3AuxDACCMenu = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_DACC", pCoupG3RAuxBranch)
    pCoupG3AuxDACCMenu.setLabel("PLC DACC peripheral settings")
    pCoupG3AuxDACCMenu.setHelp(srv_pcoup_helpkeyword)

    for idx in range(17):
        pCoupG3DACCAux.append(pCoupComponentCommon.createHexSymbol("SRV_PCOUP_G3_AUX_DACC_" + str(idx), pCoupG3AuxDACCMenu))
        pCoupG3DACCAux[idx].setLabel("DACC_" + str(idx))
        pCoupG3DACCAux[idx].setDefaultValue(dacc_cena_c07_drv[idx])
        pCoupG3DACCAux[idx].setMin(0)
        pCoupG3DACCAux[idx].setMax(0xFFFFFFFF)
        pCoupG3DACCAux[idx].setHelp(srv_pcoup_helpkeyword)

    pCoupG3AuxGainHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_GAIN_HIGH", pCoupG3RAuxBranch)
    pCoupG3AuxGainHigh.setLabel("Tx gain values for HIGH mode")
    pCoupG3AuxGainHigh.setHelp(srv_pcoup_helpkeyword)
    pCoupG3AuxGainVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_G3_AUX_GAIN_VLOW", pCoupG3RAuxBranch)
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
    pCoupG3AuxLineDriver = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_G3_AUX_LINE_DRIVER", pCoupG3RAuxBranch)
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

    global pCoupPRIMEChannelImpedanceDetection
    pCoupPRIMEChannelImpedanceDetection = pCoupComponentCommon.createKeyValueSetSymbol("SRV_PCOUP_PRIME_CHANNEL_IMP_DET", pCoupPRIMETXChannels)
    pCoupPRIMEChannelImpedanceDetection.setLabel("Impedance Detection")
    pCoupPRIMEChannelImpedanceDetection.setVisible(True)
    pCoupPRIMEChannelImpedanceDetection.setReadOnly(True)
    pCoupPRIMEChannelImpedanceDetection.setOutputMode("Key")
    pCoupPRIMEChannelImpedanceDetection.setDisplayMode("Description")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN1", "1", "CHN1")
    pCoupPRIMEChannelImpedanceDetection.addKey("CHN6", "6", "CHN6")

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
