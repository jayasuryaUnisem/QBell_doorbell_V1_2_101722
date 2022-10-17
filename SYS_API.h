#include "Configuration.h"
/*------------------------------------------------------------------*/
/*							Variable								*/
/*------------------------------------------------------------------*/


/*------------------------------------------------------------------*/
/*							Define									*/
/*------------------------------------------------------------------*/
/*		LED		*/
#define		LED1			0x01
#define		LED2			0x02
#define		LED3			0x04
#define		LED4			0x08
#define		LED_OFF			0x00

/*		KEY		*/
#define		KEY1		0x01
#define		KEY2		0x02
#define		KEY3		0x03
#define		KEY4		0x04
#define     KEY5     0x05

///*		ADC		*/
//extern unsigned int ADC_data;		//A/D Converter Data
//extern unsigned char ADC_Stas;		//show ADC status [7:4:NC 2:Converting 1:finish 0:Error]


/*  	UART		*/



/*  	LCM		*/
enum
{
   DSP_NORMAL = 0,
   DSP_INVERT,
   DSP_CURSOR,
   DSP_CLEAR
};



