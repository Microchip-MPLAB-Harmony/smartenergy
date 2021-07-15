# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""

############################################################################
#### G3 Coupling Parameters ####
############################################################################

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
gain_vlow_fcc  = [364, 480, 408]
drv_conf_fcc   = 5

rms_high_fcc_himp   = [1064, 763, 549, 394, 283, 204, 148, 108]
rms_vlow_fcc_himp   = [3614, 2775, 2009, 1431, 1019, 725, 516, 367]
thrs_high_fcc_himp  = [0, 0, 0, 0, 0, 0, 0, 0, 929, 668, 480, 345, 247, 179, 129, 94]
thrs_vlow_fcc_himp  = [0, 0, 0, 0, 0, 0, 0, 0, 9668, 6931, 4955, 3538, 2520, 1793, 1276, 909]
dacc_fcc_himp       = [0, 0, 0x00000100, 0x00000100, 0, 0, 0x4F5000FF, 0x1B1B1B1B, 0, 0, 0x00000006, 0x00000355, 0, 0x001020F0, 0x00000355, 0, 0x001020FF]
gain_high_fcc_himp  = [105, 50, 256]
gain_vlow_fcc_himp  = [364, 180, 408]
drv_conf_fcc_himp   = 8

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
thrs_high_cenb_c12 = [0, 0, 0, 0, 0, 0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF]
thrs_vlow_cenb_c12 = [0, 0, 0, 0, 0, 0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF]
dacc_cenb_c12      = [0, 0, 0x3F3F3F3F, 0x3F3F3F3F, 0x00000FFF, 0, 0x58CA00FF, 0x19191919, 0, 0, 0x0FD20004, 0x000000FF, 0x0F000000, 0x00102000, 0x000000FF, 0x0F000000, 0x00102000]
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
gain_high_fcc_c11  = [364, 50, 256]
gain_vlow_fcc_c11  = [364, 50, 408]

rms_high_arib_c11   = rms_vlow_arib_c06
rms_vlow_arib_c11   = rms_vlow_arib_c06
thrs_high_arib_c11  = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
thrs_vlow_arib_c11  = thrs_vlow_arib_c06
dacc_arib_c11       = dacc_fcc_c11
gain_high_arib_c11  = [120, 60, 281]
gain_vlow_arib_c11  = [403, 200, 451]

############################################################################
#### PRIME Coupling Parameters ####
############################################################################


############################################################################

def updatePlcCouplingParameters():
    global pCoupPLCRAuxBranch
    global pCoupPLCMainPhyG3Band
    global pCoupPLCAuxPhyG3Band

    if Database.getSymbolValue("drvPlcPhy", "DRV_PLC_MODE") != None:
        print("------------------------- [CHRIS_dbg]: Encontrado PLC_PHY driver, atualizamos parametros")
        plcDriver = "drvPlcPhy"
    elif Database.getSymbolValue("drvG3MacRt", "DRV_PLC_MODE") != None:
        print("------------------------- [CHRIS_dbg]: Encontrado PLC_G3_MACRT driver, atualizamos parametros")
        plcDriver = "drvG3MacRt"
    else:
        plcDriver = ""
        print("------------------------- [CHRIS_dbg]: NO Encontrado DRV_PLC_MODE")
        return
    
    plcDevice = Database.getSymbolValue(plcDriver, "DRV_PLC_MODE")
    plcPhyBand = Database.getSymbolValue(plcDriver, "DRV_PLC_G3_BAND")
    plcHighAtt = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_G3_HIGH_ATTENUATION")
    plcMultiband = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_G3_MULTIBAND")
    plcInternal = Database.getSymbolValue(plcDriver, "DRV_PLC_COUP_G3_INTERNAL")
    plcBandAux = Database.getSymbolValue(plcDriver, "DRV_PLC_G3_BAND_AUX")

    pCoupPLCRAuxBranch.setVisible(False)
    auxiliaryBand = False
    pCoupPLCAuxPhyG3Band.setValue("None")

    if (plcDevice == "PL460"):
        if plcPhyBand == "CEN-A":
            print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters -> PL460 G3 CEN-A")
            rms_high  = rms_high_cena 
            rms_vlow  = rms_vlow_cena 
            thrs_high = thrs_high_cena
            thrs_vlow = thrs_vlow_cena
            dacc      = dacc_cena     
            gain_high = gain_high_cena
            gain_vlow = gain_vlow_cena
            line_drv  = drv_conf_cena
            pCoupPLCMainPhyG3Band.setValue("CEN-A")

        elif plcPhyBand == "CEN-B":
            print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL460 G3 CEN-B")
            rms_high  = rms_high_cenb
            rms_vlow  = rms_vlow_cenb 
            thrs_high = thrs_high_cenb
            thrs_vlow = thrs_vlow_cenb
            dacc      = dacc_cenb     
            gain_high = gain_high_cenb
            gain_vlow = gain_vlow_cenb
            line_drv  = drv_conf_cenb
            pCoupPLCMainPhyG3Band.setValue("CEN-B")

        elif plcPhyBand == "FCC" or plcPhyBand == "ARIB":
            if (plcPhyBand == "FCC"):
                pCoupPLCMainPhyG3Band.setValue("FCC")
            else:
                pCoupPLCMainPhyG3Band.setValue("ARIB")

            if (plcHighAtt == True):
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB HIGHT ATT")
                rms_high  = rms_high_fcc_himp 
                rms_vlow  = rms_vlow_fcc_himp 
                thrs_high = thrs_high_fcc_himp
                thrs_vlow = thrs_vlow_fcc_himp
                dacc      = dacc_fcc_himp     
                gain_high = gain_high_fcc_himp
                gain_vlow = gain_vlow_fcc_himp
                line_drv  = drv_conf_fcc_himp
            else:
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL460 G3 FCC / G3 ARIB")
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
                    print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL460 G3 AUX CEN-A")
                    rms_high_aux  = rms_high_cena_c07 
                    rms_vlow_aux  = rms_vlow_cena_c07 
                    thrs_high_aux = thrs_high_cena_c07
                    thrs_vlow_aux = thrs_vlow_cena_c07
                    dacc_aux      = dacc_cena_c07_drv     
                    gain_high_aux = gain_high_cena_c07
                    gain_vlow_aux = gain_vlow_cena_c07
                    line_drv_aux  = drv_conf_cena_c07
                    pCoupPLCAuxPhyG3Band.setValue("CEN-A")
                else:
                    # "CEN-B"
                    print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL460 G3 AUX CEN-B")
                    rms_high_aux  = rms_high_cenb_c14 
                    rms_vlow_aux  = rms_vlow_cenb_c14 
                    thrs_high_aux = thrs_high_cenb_c14
                    thrs_vlow_aux = thrs_vlow_cenb_c14
                    dacc_aux      = dacc_cenb_c14_drv     
                    gain_high_aux = gain_high_cenb_c14
                    gain_vlow_aux = gain_vlow_cenb_c14
                    line_drv_aux  = drv_conf_cenb_c14
                    pCoupPLCAuxPhyG3Band.setValue("CEN-B")

    else: # "PL360"
        line_drv = 0
        line_drv_aux = 0
        if plcPhyBand == "CEN-A":
            print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters -> PL360 G3 CEN-A")
            rms_high  = rms_high_cena_c07 
            rms_vlow  = rms_vlow_cena_c07 
            thrs_high = thrs_high_cena_c07
            thrs_vlow = thrs_vlow_cena_c07
            dacc      = dacc_cena_c07
            gain_high = gain_high_cena_c07
            gain_vlow = gain_vlow_cena_c07
            pCoupPLCMainPhyG3Band.setValue("CEN-A")

        elif plcPhyBand == "CEN-B":
            pCoupPLCMainPhyG3Band.setValue("CEN-B")
            if (plcInternal == True):
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL360 G3 CEN-B Internal")
                rms_high  = rms_high_cenb_c12
                rms_vlow  = rms_vlow_cenb_c12 
                thrs_high = thrs_high_cenb_c12
                thrs_vlow = thrs_vlow_cenb_c12
                dacc      = dacc_cenb_c12     
                gain_high = gain_high_cenb_c12
                gain_vlow = gain_vlow_cenb_c12
            else:
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL360 G3 CEN-B External")
                rms_high  = rms_high_cenb_c14
                rms_vlow  = rms_vlow_cenb_c14 
                thrs_high = thrs_high_cenb_c14
                thrs_vlow = thrs_vlow_cenb_c14
                dacc      = dacc_cenb_c14     
                gain_high = gain_high_cenb_c14
                gain_vlow = gain_vlow_cenb_c14

        elif plcPhyBand == "FCC":
            pCoupPLCMainPhyG3Band.setValue("FCC")
            if (plcHighAtt == True):
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL360 G3 FCC HIGHT ATT")
                rms_high  = rms_high_fcc_c11 
                rms_vlow  = rms_vlow_fcc_c11 
                thrs_high = thrs_high_fcc_c11
                thrs_vlow = thrs_vlow_fcc_c11
                dacc      = dacc_fcc_c11     
                gain_high = gain_high_fcc_c11
                gain_vlow = gain_vlow_fcc_c11
            else:
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL360 G3 FCC")
                rms_high  = rms_high_fcc_c06 
                rms_vlow  = rms_vlow_fcc_c06 
                thrs_high = thrs_high_fcc_c06
                thrs_vlow = thrs_vlow_fcc_c06
                dacc      = dacc_fcc_c06     
                gain_high = gain_high_fcc_c06
                gain_vlow = gain_vlow_fcc_c06

            if (plcMultiband == True):
                auxiliaryBand = True
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL360 G3 AUX CEN-A")
                rms_high_aux  = rms_high_cena_c07 
                rms_vlow_aux  = rms_vlow_cena_c07 
                thrs_high_aux = thrs_high_cena_c07
                thrs_vlow_aux = thrs_vlow_cena_c07
                dacc_aux      = dacc_cena_c07     
                gain_high_aux = gain_high_cena_c07
                gain_vlow_aux = gain_vlow_cena_c07
                pCoupPLCAuxPhyG3Band.setValue("CEN-A")

        elif plcPhyBand == "ARIB":
            pCoupPLCMainPhyG3Band.setValue("ARIB")
            if (plcHighAtt == True):
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL360 G3 ARIB HIGHT ATT")
                rms_high  = rms_high_arib_c11 
                rms_vlow  = rms_vlow_arib_c11 
                thrs_high = thrs_high_arib_c11
                thrs_vlow = thrs_vlow_arib_c11
                dacc      = dacc_arib_c11     
                gain_high = gain_high_arib_c11
                gain_vlow = gain_vlow_arib_c11
            else:
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL360 G3 ARIB")
                rms_high  = rms_high_arib_c06 
                rms_vlow  = rms_vlow_arib_c06 
                thrs_high = thrs_high_arib_c06
                thrs_vlow = thrs_vlow_arib_c06
                dacc      = dacc_arib_c06     
                gain_high = gain_high_arib_c06
                gain_vlow = gain_vlow_arib_c06

            if (plcMultiband == True):
                auxiliaryBand = True
                print("------------------------- [CHRIS_dbg]: updatePlcCouplingParameters ->  PL360 G3 AUX CEN-A")
                rms_high_aux  = rms_high_cena_c07 
                rms_vlow_aux  = rms_vlow_cena_c07 
                thrs_high_aux = thrs_high_cena_c07
                thrs_vlow_aux = thrs_vlow_cena_c07
                dacc_aux      = dacc_cena_c07     
                gain_high_aux = gain_high_cena_c07
                gain_vlow_aux = gain_vlow_cena_c07
                pCoupPLCAuxPhyG3Band.setValue("CEN-A")

    # Update Values of the Main Branch in Configuration Window
    Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_LINE_DRIVER", line_drv)

    for idx in range(8):
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_RMS_HIGH_" + str(idx), rms_high[idx])
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_RMS_VLOW_" + str(idx), rms_vlow[idx])

    for idx in range(16):
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_THRS_HIGH_" + str(idx), thrs_high[idx])
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_THRS_VLOW_" + str(idx), thrs_vlow[idx])

    # for idx in range(17):
    #     print("[CHRIS_dbg]: Writing ... dacc[idx] = " + str(dacc[idx]) + " - hex(dacc[idx]) = " + str(hex(dacc[idx])))
    #     Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_DACC_" + str(idx), hex(dacc[idx]))
    #     tmp = Database.getSymbolValue("srv_pcoup", "SRV_PCOUP_DACC_" + str(idx))
    #     print("[CHRIS_dbg]: Read ... tmp = " + str(tmp) + " - hex(tmp) = " + str(hex(dacc[idx])))

    for idx in range(3):
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_GAIN_HIGH_" + str(idx), gain_high[idx])
        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_GAIN_VLOW_" + str(idx), gain_vlow[idx])

    # Update Values of the Auxiliary Branch in Configuration Window
    if (auxiliaryBand == True):
        pCoupPLCRAuxBranch.setVisible(True)

        Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_LINE_DRIVER", line_drv_aux)

        for idx in range(8):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_RMS_HIGH_" + str(idx), rms_high_aux[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_RMS_VLOW_" + str(idx), rms_vlow_aux[idx])

        for idx in range(16):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_THRS_HIGH_" + str(idx), thrs_high_aux[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_THRS_VLOW_" + str(idx), thrs_vlow_aux[idx])

        # for idx in range(17):
        #     print("[CHRIS_dbg]: Writing ... dacc[idx] = " + str(dacc[idx]) + " - hex(dacc[idx]) = " + str(hex(dacc[idx])))
        #     Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_DACC_" + str(idx), hex(dacc_aux[idx]))
        #     tmp = Database.getSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_DACC_" + str(idx))
        #     print("[CHRIS_dbg]: Read ... tmp = " + str(tmp) + " - hex(tmp) = " + str(hex(dacc_aux[idx])))

        for idx in range(3):
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_GAIN_HIGH_" + str(idx), gain_high_aux[idx])
            Database.setSymbolValue("srv_pcoup", "SRV_PCOUP_AUX_GAIN_VLOW_" + str(idx), gain_vlow_aux[idx])


def handleMessage(messageID, args):
    result_dict = {}

    print ("[CHRIS_dbg]: srv_pcoup handleMessage:" + messageID)

    if (messageID == "SRV_PCOUP_UPDATE_PARAMETERS"):
        updatePlcCouplingParameters()

    return result_dict            


def dependenciesHandler(symbol, event):
    updatePlcCouplingParameters()

def instantiateComponent(pCoupComponentCommon):
    
    Log.writeInfoMessage("Loading PLC Phy Coupling service")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    ############################################################################
    #### Main Transmission Branch ####
    ############################################################################

    pCoupPLCMainBranch = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_MAIN_BRANCH", None)
    pCoupPLCMainBranch.setLabel("Main Transmission branch")

    global pCoupPLCMainPhyG3Band
    pCoupPLCMainPhyG3Band = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_MAIN_G3_BAND", pCoupPLCMainBranch, ["CEN-A", "CEN-B", "FCC", "ARIB"])
    pCoupPLCMainPhyG3Band.setLabel("Phy G3 Band")
    pCoupPLCMainPhyG3Band.setDefaultValue("CEN-A")
    pCoupPLCMainPhyG3Band.setReadOnly(True)
    
    pCoupPLCNumTxLvl = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_NUM_TX_LVL", pCoupPLCMainBranch)
    pCoupPLCNumTxLvl.setLabel("Number of TX levels")
    pCoupPLCNumTxLvl.setDefaultValue(8)
    # Use an auxiliary control to handle dependencies with drvPlcPhy
    pCoupPLCNumTxLvl.setDependencies(dependenciesHandler, ["drvPlcPhy.DRV_PLC_COUP_G3_HIGH_ATTENUATION", "drvPlcPhy.DRV_PLC_COUP_G3_INTERNAL"])

    pCoupPLCRMSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_RMS_HIGH", pCoupPLCMainBranch)
    pCoupPLCRMSHigh.setLabel("Target RMS high values")
    pCoupPLCRMSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_RMS_VLOW", pCoupPLCMainBranch)
    pCoupPLCRMSVlow.setLabel("Target RMS vlow values")

    for idx in range(8):
        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_RMS_HIGH_" + str(idx), pCoupPLCRMSHigh)
        pCoupPLCSymbol.setLabel("RMS_HIGH_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(rms_high_cena[idx])

        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_RMS_VLOW_" + str(idx), pCoupPLCRMSVlow)
        pCoupPLCSymbol.setLabel("RMS_VLOW_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(rms_vlow_cena[idx])

    pCoupPLCTHRSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_THRS_HIGH", pCoupPLCMainBranch)
    pCoupPLCTHRSHigh.setLabel("Thresholds to update Tx Mode from HIGH mode")
    pCoupPLCTHRSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_THRS_VLOW", pCoupPLCMainBranch)
    pCoupPLCTHRSVlow.setLabel("Thresholds to update Tx Mode from VLOW mode")

    for idx in range(16):
        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_THRS_HIGH_" + str(idx), pCoupPLCTHRSHigh)
        pCoupPLCSymbol.setLabel("THRS_HIGH_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(thrs_high_cena[idx])

        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_THRS_VLOW_" + str(idx), pCoupPLCTHRSVlow)
        pCoupPLCSymbol.setLabel("THRS_VLOW_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(thrs_vlow_cena[idx])

    pCoupPLCDACC = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_DACC", pCoupPLCMainBranch)
    pCoupPLCDACC.setLabel("DACC Configuration values")

    for idx in range(17):
        pCoupPLCSymbol = pCoupComponentCommon.createHexSymbol("SRV_PCOUP_DACC_" + str(idx), pCoupPLCDACC)
        pCoupPLCSymbol.setLabel("DACC_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(dacc_cena[idx])
    
    pCoupPLCGainHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_GAIN_HIGH", pCoupPLCMainBranch)
    pCoupPLCGainHigh.setLabel("Gain values for HIGH Tx Mode")
    pCoupPLCGainVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_GAIN_VLOW", pCoupPLCMainBranch)
    pCoupPLCGainVlow.setLabel("Gain values for VLOW Tx Mode")

    for idx in range(3):
        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_GAIN_HIGH_" + str(idx), pCoupPLCGainHigh)
        pCoupPLCSymbol.setLabel("GAIN_HIGH_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(gain_high_cena[idx])

        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_GAIN_VLOW_" + str(idx), pCoupPLCGainVlow)
        pCoupPLCSymbol.setLabel("GAIN_VLOW_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(gain_vlow_cena[idx])

    global pCoupPLCLineDriver
    pCoupPLCLineDriver = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_LINE_DRIVER", pCoupPLCMainBranch)
    pCoupPLCLineDriver.setLabel("Line Driver settings")
    pCoupPLCLineDriver.setDefaultValue(drv_conf_cena)
    pCoupPLCLineDriver.setReadOnly(True)

    ############################################################################
    #### Auxiliary Transmission Branch ####
    ############################################################################

    global pCoupPLCRAuxBranch
    pCoupPLCRAuxBranch = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_AUX_BRANCH", None)
    pCoupPLCRAuxBranch.setLabel("Auxiliary Transmission branch")
    pCoupPLCRAuxBranch.setVisible(False)

    global pCoupPLCAuxPhyG3Band
    pCoupPLCAuxPhyG3Band = pCoupComponentCommon.createComboSymbol("SRV_PCOUP_AUX_G3_BAND", pCoupPLCRAuxBranch, ["None", "CEN-A", "CEN-B", "FCC", "ARIB"])
    pCoupPLCAuxPhyG3Band.setLabel("Phy G3 Band")
    pCoupPLCAuxPhyG3Band.setDefaultValue("None")
    pCoupPLCAuxPhyG3Band.setReadOnly(True)

    pCoupPLCAuxNumTxLvl = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_AUX_NUM_TX_LVL", pCoupPLCRAuxBranch)
    pCoupPLCAuxNumTxLvl.setLabel("Number of TX levels")
    pCoupPLCAuxNumTxLvl.setDefaultValue(8)
   
    pCoupPLCAuxRMSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_AUX_RMS_HIGH", pCoupPLCRAuxBranch)
    pCoupPLCAuxRMSHigh.setLabel("Target RMS high values")
    pCoupPLCAuxRMSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_AUX_RMS_VLOW", pCoupPLCRAuxBranch)
    pCoupPLCAuxRMSVlow.setLabel("Target RMS vlow values")

    for idx in range(8):
        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_AUX_RMS_HIGH_" + str(idx), pCoupPLCAuxRMSHigh)
        pCoupPLCSymbol.setLabel("RMS_HIGH_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(rms_high_cena[idx])

        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_AUX_RMS_VLOW_" + str(idx), pCoupPLCAuxRMSVlow)
        pCoupPLCSymbol.setLabel("RMS_VLOW_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(rms_vlow_cena[idx])

    pCoupPLCAuxTHRSHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_AUX_THRS_HIGH", pCoupPLCRAuxBranch)
    pCoupPLCAuxTHRSHigh.setLabel("Thresholds to update Tx Mode from HIGH mode")
    pCoupPLCAuxTHRSVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_AUX_THRS_VLOW", pCoupPLCRAuxBranch)
    pCoupPLCAuxTHRSVlow.setLabel("Thresholds to update Tx Mode from VLOW mode")

    for idx in range(16):
        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_AUX_THRS_HIGH_" + str(idx), pCoupPLCAuxTHRSHigh)
        pCoupPLCSymbol.setLabel("THRS_HIGH_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(thrs_high_cena[idx])

        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_AUX_THRS_VLOW_" + str(idx), pCoupPLCAuxTHRSVlow)
        pCoupPLCSymbol.setLabel("THRS_VLOW_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(thrs_vlow_cena[idx])

    pCoupPLCAuxDACC = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_AUX_DACC", pCoupPLCRAuxBranch)
    pCoupPLCAuxDACC.setLabel("DACC Configuration values")

    for idx in range(17):
        pCoupPLCSymbol = pCoupComponentCommon.createHexSymbol("SRV_PCOUP_AUX_DACC_" + str(idx), pCoupPLCAuxDACC)
        pCoupPLCSymbol.setLabel("DACC_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(dacc_cena[idx])
    
    pCoupPLCAuxGainHigh = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_AUX_GAIN_HIGH", pCoupPLCRAuxBranch)
    pCoupPLCAuxGainHigh.setLabel("Gain values for HIGH Tx Mode")
    pCoupPLCAuxGainVlow = pCoupComponentCommon.createMenuSymbol("SRV_PCOUP_AUX_GAIN_VLOW", pCoupPLCRAuxBranch)
    pCoupPLCAuxGainVlow.setLabel("Gain values for VLOW Tx Mode")

    for idx in range(3):
        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_AUX_GAIN_HIGH_" + str(idx), pCoupPLCAuxGainHigh)
        pCoupPLCSymbol.setLabel("GAIN_HIGH_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(gain_high_cena[idx])

        pCoupPLCSymbol = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_AUX_GAIN_VLOW_" + str(idx), pCoupPLCAuxGainVlow)
        pCoupPLCSymbol.setLabel("GAIN_VLOW_" + str(idx))
        pCoupPLCSymbol.setDefaultValue(gain_vlow_cena[idx])

    global pCoupPLCAuxLineDriver
    pCoupPLCAuxLineDriver = pCoupComponentCommon.createIntegerSymbol("SRV_PCOUP_AUX_LINE_DRIVER", pCoupPLCRAuxBranch)
    pCoupPLCAuxLineDriver.setLabel("Line Driver settings")
    pCoupPLCAuxLineDriver.setDefaultValue(0)
    pCoupPLCAuxLineDriver.setReadOnly(True)

    #### Phy Coupling Files ######################################################
    pCoupG3SourceFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_SOURCE", None)
    pCoupG3SourceFile.setSourcePath("service/pcoup/srv_pcoup_g3.c.ftl")
    pCoupG3SourceFile.setOutputName("srv_pcoup.c")
    pCoupG3SourceFile.setDestPath("service/pcoup")
    pCoupG3SourceFile.setProjectPath("config/" + configName + "/service/pcoup/")
    pCoupG3SourceFile.setType("SOURCE")
    pCoupG3SourceFile.setMarkup(True)
    pCoupG3SourceFile.setOverwrite(True)
    pCoupG3SourceFile.setEnabled(True)

    pCoupG3HeaderFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_HEADER", None)
    pCoupG3HeaderFile.setSourcePath("service/pcoup/srv_pcoup_g3.h.ftl")
    pCoupG3HeaderFile.setOutputName("srv_pcoup.h")
    pCoupG3HeaderFile.setDestPath("service/pcoup")
    pCoupG3HeaderFile.setProjectPath("config/" + configName + "/service/pcoup/")
    pCoupG3HeaderFile.setType("HEADER")
    pCoupG3HeaderFile.setMarkup(True)
    pCoupG3HeaderFile.setOverwrite(True)
    pCoupG3HeaderFile.setEnabled(True)

    #### FreeMaker System Files ######################################################

    pCoupSystemDefFile = pCoupComponentCommon.createFileSymbol("SRV_PCOUP_SYSTEM_DEF", None)
    pCoupSystemDefFile.setType("STRING")
    pCoupSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    pCoupSystemDefFile.setSourcePath("service/pcoup/templates/system/system_definitions.h.ftl")
    pCoupSystemDefFile.setMarkup(True)

    # Update PLC Profile according to PLC PHY driver configuration
    updatePlcCouplingParameters()
