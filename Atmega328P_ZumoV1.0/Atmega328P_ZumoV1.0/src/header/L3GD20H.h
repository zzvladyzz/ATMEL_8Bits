/*
 * L3GD20H.h
 *
 * Created: 4/14/2021 2:20:57 AM
 *  Author: vladi
 */ 


#ifndef L3GD20H_H_
#define L3GD20H_H_

/*		Sensibilidad	*/
#define Factor_Conversion_L3GD20H_Gyr 0.00875F		/*	Sensibilidad para +/- 245 dps		*/
/*		Registros		*/
#define L3GD20H_SA0_HIGH_ADDRESS      0b1101011		/*	Direccion del esclavo si pin SDO/SAO se encuentra en Vcc (Diagrama esta asi del ZUMO_V1.2)// Es la direccion que se manda	*/
#define L3GD20H_SA0_LOW_ADDRESS       0b1101010		/*	Direccion del esclavo si pin SDO/SAO se encuentra en GND				*/
#define L3GD20H_WHO_ID			      0xD7			/*	0b11010111 Esto es el valor del registro WHO_AM_I solo es de tipo lectura,no es relevante			*/

#define L3GD20H_WHO_AM_I        0x0F
#define L3GD20H_CTRL1           0x20    /*		0x6F = 0b01101111 para DR = 01 (200 Hz ODR=0); BW = 10 (50 Hz bandwidth); PD = 1 (normal mode); Zen = Yen = Xen = 1 (Todos los ejes habilitados)	*/
#define L3GD20H_CTRL2           0x21
#define L3GD20H_CTRL3           0x22
#define L3GD20H_CTRL4           0x23    /*		0x00 = 0b00000000 FS = 00 (+/- 245 Escala del dps)  con sensibilidad de 8.45 mdps/LSB y Zero-rate de +/-25 dps		*/
#define L3GD20H_CTRL5           0x24
#define L3GD20H_REFERENCE       0x25
#define L3GD20H_OUT_TEMP        0x26
#define L3GD20H_STATUS          0x27
#define L3GD20H_OUT_X_L         0x28
#define L3GD20H_OUT_X_H         0x29
#define L3GD20H_OUT_Y_L         0x2A
#define L3GD20H_OUT_Y_H         0x2B
#define L3GD20H_OUT_Z_L         0x2C
#define L3GD20H_OUT_Z_H         0x2D
#define L3GD20H_FIFO_CTRL       0x2E    /*		0x00 Para que el bufer trabaje en modo Bypass		*/
#define L3GD20H_FIFO_SRC        0x2F
#define L3GD20H_IG_CFG          0x30
#define L3GD20H_IG_SRC          0x31
#define L3GD20H_IG_THS_XH       0x32
#define L3GD20H_IG_THS_XL       0x33
#define L3GD20H_IG_THS_YH       0x34
#define L3GD20H_IG_THS_YL       0x35
#define L3GD20H_IG_THS_ZH       0x36
#define L3GD20H_IG_THS_ZL       0x37
#define L3GD20H_IG_DURATION     0x38
#define L3GD20H_LOW_ODR         0x39    /*		Este Registro poner a 0b00000000 o 0x00 para deshabilitar low speed ODR		*/


#endif /* L3GD20H_H_ */