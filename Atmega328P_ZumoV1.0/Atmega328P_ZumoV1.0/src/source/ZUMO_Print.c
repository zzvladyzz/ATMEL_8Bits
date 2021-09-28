/*
 * Zumo_Print.c
 *
 * Created: 6/17/2021 2:36:30 PM
 *  Author: vladi
 */ 
#include "../header/ZUMO.h"
void Serial_Print_Sensor()
{
	char Send[]=" ";
	char a[]=" | ";
	USART_TransmitString(a,strlen(a));	sprintf(Send,"%u",ZUMO.QTR.Sensors[0]);	USART_TransmitString(Send,strlen(Send));
	char b[]=" | ";
	USART_TransmitString(b,strlen(b));	sprintf(Send,"%u",ZUMO.QTR.Sensors[1]); USART_TransmitString(Send,strlen(Send));
	char c[]=" | ";
	USART_TransmitString(c,strlen(c)); 	sprintf(Send,"%u",ZUMO.QTR.Sensors[2]); USART_TransmitString(Send,strlen(Send));
	char d[]=" | ";
	USART_TransmitString(d,strlen(d));	sprintf(Send,"%u",ZUMO.QTR.Sensors[3]); USART_TransmitString(Send,strlen(Send));
	char e[]=" | ";
	USART_TransmitString(e,strlen(e)); sprintf(Send,"%u",ZUMO.QTR.Sensors[4]); USART_TransmitString(Send,strlen(Send));
	char f[]=" | ";
	USART_TransmitString(f,strlen(f)); sprintf(Send,"%u",ZUMO.QTR.Sensors[5]); USART_TransmitString(Send,strlen(Send));
	char g[]=" | ";
	USART_TransmitString(g,strlen(g)); sprintf(Send,"%u",ZUMO.QTR.Position); USART_TransmitString(Send,strlen(Send));
	char y[]=" | ";
	USART_TransmitString(y,strlen(y)); Float_to_Str(Send,ZUMO.IMU.yaw); USART_TransmitString(Send,strlen(Send));
	
	sprintf(Send,"%s","\n\r");
	USART_TransmitString(Send,strlen(Send));
	
}
void Serial_Print_SensorMax()
{
	char Send[]=" ";
	char a[]=" | ";
	USART_TransmitString(a,strlen(a));	sprintf(Send,"%u",ZUMO.QTR.Sensors_Max[0]);	USART_TransmitString(Send,strlen(Send));
	char b[]=" | ";
	USART_TransmitString(b,strlen(b));	sprintf(Send,"%u",ZUMO.QTR.Sensors_Max[1]); USART_TransmitString(Send,strlen(Send));
	char c[]=" | ";
	USART_TransmitString(c,strlen(c)); 	sprintf(Send,"%u",ZUMO.QTR.Sensors_Max[2]); USART_TransmitString(Send,strlen(Send));
	char d[]=" | ";
	USART_TransmitString(d,strlen(d));	sprintf(Send,"%u",ZUMO.QTR.Sensors_Max[3]); USART_TransmitString(Send,strlen(Send));
	char e[]=" | ";
	USART_TransmitString(e,strlen(e)); sprintf(Send,"%u",ZUMO.QTR.Sensors_Max[4]); USART_TransmitString(Send,strlen(Send));
	char f[]=" | ";
	USART_TransmitString(f,strlen(f)); sprintf(Send,"%u",ZUMO.QTR.Sensors_Max[5]); USART_TransmitString(Send,strlen(Send));

	sprintf(Send,"%s","\n\r");
	USART_TransmitString(Send,strlen(Send));
	
}
void Serial_Print_SensorMin()
{
	char Send[]=" ";
	char a[]=" | ";
	USART_TransmitString(a,strlen(a));	sprintf(Send,"%u",ZUMO.QTR.Sensors_Min[0]);	USART_TransmitString(Send,strlen(Send));
	char b[]=" | ";
	USART_TransmitString(b,strlen(b));	sprintf(Send,"%u",ZUMO.QTR.Sensors_Min[1]); USART_TransmitString(Send,strlen(Send));
	char c[]=" | ";
	USART_TransmitString(c,strlen(c)); 	sprintf(Send,"%u",ZUMO.QTR.Sensors_Min[2]); USART_TransmitString(Send,strlen(Send));
	char d[]=" | ";
	USART_TransmitString(d,strlen(d));	sprintf(Send,"%u",ZUMO.QTR.Sensors_Min[3]); USART_TransmitString(Send,strlen(Send));
	char e[]=" | ";
	USART_TransmitString(e,strlen(e)); sprintf(Send,"%u",ZUMO.QTR.Sensors_Min[4]); USART_TransmitString(Send,strlen(Send));
	char f[]=" | ";
	USART_TransmitString(f,strlen(f)); sprintf(Send,"%u",ZUMO.QTR.Sensors_Min[5]); USART_TransmitString(Send,strlen(Send));

	sprintf(Send,"%s","\n\r");
	USART_TransmitString(Send,strlen(Send));
	
}
void Serial_Print_Calibrated()
{
	char Send[]=" ";
	char a[]=" Mx ";
	USART_TransmitString(a,strlen(a));	Float_to_Str(Send,ZUMO.IMU.M_x);	USART_TransmitString(Send,strlen(Send));
	char b[]=" My ";
	USART_TransmitString(b,strlen(b));	Float_to_Str(Send,ZUMO.IMU.M_y); USART_TransmitString(Send,strlen(Send));
	char c[]=" MinX ";
	USART_TransmitString(c,strlen(c)); 	Float_to_Str(Send,ZUMO.IMU.Min_Mx); USART_TransmitString(Send,strlen(Send));
	char d[]=" MaxX ";
	USART_TransmitString(d,strlen(d));	Float_to_Str(Send,ZUMO.IMU.Max_Mx); USART_TransmitString(Send,strlen(Send));
	char e[]=" Miny ";
	USART_TransmitString(e,strlen(e)); Float_to_Str(Send,ZUMO.IMU.Min_My); USART_TransmitString(Send,strlen(Send));
	char f[]=" Maxy ";
	USART_TransmitString(f,strlen(f)); Float_to_Str(Send,ZUMO.IMU.Max_My); USART_TransmitString(Send,strlen(Send));
	char g[]=" alpha ";
	USART_TransmitString(g,strlen(g)); Float_to_Str(Send,ZUMO.IMU.Alpha); USART_TransmitString(Send,strlen(Send));
	char h[]=" beta ";
	USART_TransmitString(h,strlen(h)); Float_to_Str(Send,ZUMO.IMU.Beta); USART_TransmitString(Send,strlen(Send));

	sprintf(Send,"%s","\n\r");
	USART_TransmitString(Send,strlen(Send));	
}
void Serial_Print_IMU()
{
	char Send[]=" ";
	char a[]=" roll ";
	USART_TransmitString(a,strlen(a));	Float_to_Str(Send,ZUMO.IMU.roll);	USART_TransmitString(Send,strlen(Send));
	char b[]=" pitch ";
	USART_TransmitString(b,strlen(b));	Float_to_Str(Send,ZUMO.IMU.pitch); USART_TransmitString(Send,strlen(Send));
	char c[]=" yaw ";
	USART_TransmitString(c,strlen(c)); 	Float_to_Str(Send,ZUMO.IMU.yaw); USART_TransmitString(Send,strlen(Send));

	sprintf(Send,"%s","\n\r");
	USART_TransmitString(Send,strlen(Send));
}
void Serial_Print_DAQ()
{	
	///////////////////////Parallax  
	char clear[]="CLEARDATA\r";
	char label[]="LABEL,Time,X,Y,mx,Mx,my,My,\r";
	char reset[]="RESETTIMER\r";
	USART_TransmitString(clear,strlen(clear));
	USART_TransmitString(label,strlen(label));
	USART_TransmitString(reset,strlen(reset));
	///////////////////////////////////	
	char datatime[]="DATA,TIME,";
	USART_TransmitString(datatime,strlen(datatime));
	char Send[]=" ";
	Float_to_Str(Send,ZUMO.IMU.M_x);	USART_TransmitString(Send,strlen(Send));
	char b[]=",";
	USART_TransmitString(b,strlen(b));	Float_to_Str(Send,ZUMO.IMU.M_y); USART_TransmitString(Send,strlen(Send));
	char c[]=",";
	USART_TransmitString(c,strlen(c)); 	Float_to_Str(Send,ZUMO.IMU.Min_Mx); USART_TransmitString(Send,strlen(Send));
	char d[]=",";
	USART_TransmitString(d,strlen(d));	Float_to_Str(Send,ZUMO.IMU.Max_Mx); USART_TransmitString(Send,strlen(Send));
	char e[]=",";
	USART_TransmitString(e,strlen(e)); Float_to_Str(Send,ZUMO.IMU.Min_My); USART_TransmitString(Send,strlen(Send));
	char f[]=",";
	USART_TransmitString(f,strlen(f)); Float_to_Str(Send,ZUMO.IMU.Max_My); USART_TransmitString(Send,strlen(Send));
	char g[]=",";
	USART_TransmitString(g,strlen(g)); Float_to_Str(Send,ZUMO.IMU.Alpha); USART_TransmitString(Send,strlen(Send));
	char h[]=",";
	USART_TransmitString(h,strlen(h)); Float_to_Str(Send,ZUMO.IMU.Beta); USART_TransmitString(Send,strlen(Send));

	sprintf(Send,"%s","\n\r");
	USART_TransmitString(Send,strlen(Send));
	//				Aceloremetro
	/*char Send[]=" ";
	//Float_to_Str(Send,yaw);
	Float_to_Str(Send,ZUMO.IMU.yaw);	USART_TransmitString(Send,strlen(Send));
	char b[]=",";
	USART_TransmitString(b,strlen(b));
	//Float_to_Str(Send,pitch);
	Float_to_Str(Send,ZUMO.IMU.pitch);
	USART_TransmitString(Send,strlen(Send));
	char c[]=",";
	USART_TransmitString(c,strlen(c));
	//Float_to_Str(Send,C_Mz);
	Float_to_Str(Send,ZUMO.IMU.yaw);
	USART_TransmitString(Send,strlen(Send));
	sprintf(Send,"%s","\r");
	USART_TransmitString(Send,strlen(Send));
	*/
}
void Float_to_Str(char *buffer, float f)
{
	float fenteros;
	float fdecimales;
	int entero, decimal;
	fenteros = floor(f);
	fdecimales = f - fenteros;
	entero = (int)fenteros;
	decimal = (int)10000 * fdecimales;
	sprintf(buffer, "%d.%03i", entero, decimal);
}

