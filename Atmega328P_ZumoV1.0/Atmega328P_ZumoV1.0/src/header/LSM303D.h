/*
 * LSM303D.h
 *
 * Created: 4/14/2021 2:20:03 AM
 *  Author: vladi
 */ 


#ifndef LSM303D_H_
#define LSM303D_H_


/*		Sensibilidades	*/
#define	Factor_Conversion_LSM303D_Mag 0.00008		/*	Sensibilidad para +/- 2 gauss		*/
#define	Factor_Conversion_LSM303D_Acc 0.000061F		/*	Sensibilidad para +/- 2 g			*/
/*		Registros		*/
#define   LSM303D_SA0_HIGH_ADDRESS   0b0011101		/*	Pin SAO conectado a Vcc		*/
#define   LSM303D_SA0_LOW_ADDRESS    0b0011110		/*	Pin SAO conectado a GND		*/
#define   LSM303D_TEMP_OUT_L         0x05
#define   LSM303D_TEMP_OUT_H         0x06
#define   LSM303D_STATUS_M           0x07
#define   LSM303D_OUT_X_L_M          0x08
#define   LSM303D_OUT_X_H_M          0x09
#define   LSM303D_OUT_Y_L_M          0x0A
#define   LSM303D_OUT_Y_H_M          0x0B
#define   LSM303D_OUT_Z_L_M          0x0C
#define   LSM303D_OUT_Z_H_M          0x0D
#define   LSM303D_WHO_AM_I           0x0F
#define   LSM303D_INT_CTRL_M         0x12
#define   LSM303D_INT_SRC_M          0x13
#define   LSM303D_INT_THS_L_M        0x14
#define   LSM303D_INT_THS_H_M        0x15
#define   LSM303D_OFFSET_X_L_M       0x16
#define   LSM303D_OFFSET_X_H_M       0x17
#define   LSM303D_OFFSET_Y_L_M       0x18
#define   LSM303D_OFFSET_Y_H_M       0x19
#define   LSM303D_OFFSET_Z_L_M       0x1A
#define   LSM303D_OFFSET_Z_H_M       0x1B
#define   LSM303D_REFERENCE_X        0x1C
#define   LSM303D_REFERENCE_Y        0x1D
#define   LSM303D_REFERENCE_Z        0x1E
#define   LSM303D_CTRL0              0x1F
#define   LSM303D_CTRL1              0x20		/*	0x57 = 0b01010111 AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)	*/
#define   LSM303D_CTRL2              0x21		/*	0x00 = 0b00000000 AFS = 0 (+/- 2 g full scale)											*/
#define   LSM303D_CTRL3              0x22
#define   LSM303D_CTRL4              0x23
#define   LSM303D_CTRL5              0x24		/*	0x64 = 0b01100100 M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)			*/
#define   LSM303D_CTRL6              0x25		/*	0x00 = 0b00100000 MFS = 00 (+/- 2 gauss full scale)										*/
#define   LSM303D_CTRL7              0x26		/*	0x00 = 0b00000000 MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)	*/
#define   LSM303D_STATUS_A           0x27
#define   LSM303D_OUT_X_L_A          0x28
#define   LSM303D_OUT_X_H_A          0x29
#define   LSM303D_OUT_Y_L_A          0x2A
#define   LSM303D_OUT_Y_H_A          0x2B
#define   LSM303D_OUT_Z_L_A          0x2C
#define   LSM303D_OUT_Z_H_A          0x2D
#define   LSM303D_FIFO_CTRL          0x2E
#define   LSM303D_FIFO_SRC           0x2F
#define   LSM303D_IG_CFG1            0x30
#define   LSM303D_IG_SRC1            0x31
#define   LSM303D_IG_THS1            0x32
#define   LSM303D_IG_DUR1            0x33
#define   LSM303D_IG_CFG2            0x34
#define   LSM303D_IG_SRC2            0x35
#define   LSM303D_IG_THS2            0x36
#define   LSM303D_IG_DUR2            0x37
#define   LSM303D_CLICK_CFG          0x38
#define   LSM303D_CLICK_SRC          0x39
#define   LSM303D_CLICK_THS          0x3A
#define   LSM303D_TIME_LIMIT         0x3B
#define   LSM303D_TIME_LATENCY       0x3C
#define   LSM303D_TIME_WINDOW        0x3D
#define   LSM303D_Act_THS            0x3E
#define   LSM303D_Act_DUR            0x3F


#endif /* LSM303D_H_ */