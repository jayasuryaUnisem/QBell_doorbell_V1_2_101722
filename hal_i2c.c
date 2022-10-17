#include "hal_i2c.h"

typedef unsigned char u8;

/**************************/
//Global Variables
/**************************/
volatile u8 Rx_Buff;
unsigned char DeviceACK_Flag=1;
unsigned char DeviceACK_CMD_Flag=1;
unsigned char Fg_I2c_WrRd=1;

u8 u8_I2c_WrRd_1ms;
u8 u8_IIC_Count;
u8 u16_I2cError_Cnt=0;


void IIC_Init()
{
	DeviceACK_Flag=0;
	DeviceACK_CMD_Flag=0;
	Fg_I2c_WrRd=0;
//	u8_IIC_Count=0;
     //DeviceACK_Flag = 0;
}

//IIC Start ??   
void IIC_Start()
{
	scl=1;	scl_c=0;
	DatHi();
	GCC_DELAY(I2CSP);	//V3?????????,????8MHz,?????16us
	scl=1;		GCC_DELAY(I2CSP);
	DatLo();	GCC_DELAY(I2CSP);
	scl=0;		GCC_DELAY(I2CSP);
}


//IIC Stop??   
void IIC_Stop()    //data?? ??????? FYT
{
	scl=0; scl_c=0;
	DatLo();
	GCC_DELAY(I2CSP);
	scl=1;	GCC_DELAY(I2CSP);
	DatHi();GCC_DELAY(I2CSP);
//	SCL=0;	GCC_DELAY(I2CSP);
}


void Wr_Byte(unsigned short I2CData)
{
	unsigned short temp=0b10000000;
	scl=0;
	do
	{
		if(I2CData&temp)	DatHi();
		else	DatLo();
			GCC_DELAY(I2CSP/2);
		scl=1;	GCC_DELAY(I2CSP);  // ?????????
		scl=0;	GCC_DELAY(I2CSP/2);
		temp>>=1;	
	}while(temp!=0);
}


//read one Byte Data ??  //DATA ???? OK  FYT
unsigned short Rd_Byte()
{
	unsigned short I2CData=0,temp=0b10000000;
	sda_c=1;
	do
	{
		scl=1;	GCC_DELAY(I2CSP);
		if(sda)	I2CData|=temp;
		scl=0;	GCC_DELAY(I2CSP);
		temp>>=1;
	}while(temp!=0);
	return I2CData;
}



//ACK ??   
void checkAck()
{
	unsigned char i=20;
	sda_c=1;	GCC_DELAY(I2CSP);
	scl=1;	GCC_DELAY(I2CSP);
	while(i--)
	{
		if(!sda)
		{
			DeviceACK_Flag=1;	//SDA=0,??????
			break;
		}
		GCC_DELAY(I2CSP);	
	}
	scl=0;
	GCC_DELAY(I2CSP);
}

void WriteI2C_Int(unsigned short DeviceAddr, unsigned short DataAddr, unsigned short value)
{
    IIC_Start();
    Wr_Byte((DeviceAddr<<1)|0);
    checkAck();

    Wr_Byte(DataAddr);
    checkAck();
    
    Wr_Byte(value);
    checkAck();

    IIC_Stop();
}