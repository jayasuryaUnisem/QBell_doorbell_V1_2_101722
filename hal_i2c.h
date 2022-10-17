#ifndef HAL_I2C_H
#define HAL_I2C_H
#include "main.h"



#define sda_c	_pbc4 //_pbc4
//#define sda_pu	_pbpu4
#define sda		_pb4 //_pb4
#define scl_c   _pbc2 //_pbc2
#define scl   	_pb2 //_pb2

#define DatHi()		sda_c=1/*,sda_pu=1 */ 
//#define DatHi()		SDAC=0,SDA=1 
#define DatLo()		sda_c=0,sda=0
#define I2CSP		160


#define	DEVICE_ID			0X80	//Slave Address  ???
#define IIC_TIMEOUT			1000	//2000*1ms


extern void IIC_Init();
extern void IIC_Start();
extern void IIC_Stop();
extern void Wr_Byte(unsigned short);
//extern short Rd_Byte(); 
extern void checkAck();
extern void WriteI2C_Int(unsigned short, unsigned short, unsigned short);


#endif