#include "hal_eeprom.h"

extern int_flags intF;

static volatile unsigned char EEPROM_data[64] __attribute__ ((at(0x280)));

void eeprom_init()
{
	EEPROM_WR_CTR() = 0;
	_mp1h = 0;
}

void eeprom_write(u8 addrC, u8 byteValue)
{

    #if 0
	
	_emi = 0;		//disable global interrupt

	EEPROM_MP_LOW(); //mp1l = 40H
	EEPROM_MP_HIGH(); //mp1h = 01H

	EEPROM_MODE_BYTE();		//set byte mode

	EEPROM_EEA() = addrC;	   //address register
	EEPROM_EED() = byteValue;	//data register

	EEPROM_WR_EN();           //enable write mode
    EEPROM_WR_CTR() = 1;      

	 while(EEPROM_ER_CTR() == 1); //wait till write process is done

	EEPROM_EEC_CLR();   //clearing IAR1 registers
	_mp1h = 0;         //clearing the memory pointer high

	_emi = 1;		//enable global interrupt			
	return;
    #endif 

	#if 1

	u8 buffer = 0;

	buffer = _emi;
	_emi = 0;
	
	_eea = addrC;
	_eed = byteValue;
	_mp1l = 0x40;
	_mp1h = 0x01;
	
	_iar1 |= 0b00000000; //set to byte mode
	_iar1 |= 0b00001000;
	_iar1 |= 0b00000100;

	while((_iar1 & 0b00000100));
	_emi = buffer;
	_iar1 = 0x00;
	_mp1h = 0;

	#endif

}

u8 eeprom_read(u8 addrV)
{
    #if 0
   
	EEPROM_MP_LOW();   //mp1l = 40H
	EEPROM_MP_HIGH();  //mp1h = 01H
 
	EEPROM_MODE_BYTE();  //set to byte mode

	EEPROM_EEA() = addrV; //address register
	
	_rd = 1;             //read control reg
    EEPROM_RD_EN();     //enable read mode

    while(EEPROM_RD_CTR() == 1); //wait till the read preocess complete

	EEPROM_EEC_CLR();    //clearing IAR1 reg
	_mp1h = 0;
		
	result = EEPROM_EED();		//Read data
    return result;
    #endif

	#if 1
	u8 buffer = 0;
	buffer = _emi;
	_emi = 0;

	_eea = addrV;
	_mp1l = 0x40;
	_mp1h = 0x01;
	delay(10);
	_iar1 |= 0b00000000; //set to byte mode

	_iar1 |= 0b00000010;
	_iar1 |= 0b00000001; 
	while((_iar1 & 0b00000001));
	_emi = buffer;
	_iar1 = 0x00;
	_mp1h = 0;
	return _eed;


	#endif
}


void eepromDeviceAddrRead()
{
	u8 i = 0;
	for(i = EEPROM_DEVICE_START_ADDR; i<EEPROM_DEVICE_START_ADDR+4; i++)
		intF.deviceID[i] = eeprom_read(i);
}


void eepromDeviceAddrCpyRead(void)
{
	u8 i = 0;
	for(i = EEPROM_DEVICE_ADDR_CPY; i<EEPROM_DEVICE_ADDR_CPY+4; i++)
		intF.deviceAddrCpy[i-EEPROM_DEVICE_ADDR_CPY] = eeprom_read(i);
	uart_printf("EEPROM Device Address Copy Read Done\n");
}

u8 eepromDeviceAddrCpyAckRead(void)
{
	intF.deviceAddCpyAck = eeprom_read(EEPROM_DEVICE_ADDR_CPY_ACK);
	return intF.deviceAddCpyAck;
}

void eepromDeviceAddrCpyWrite(void)
{
	u8 i = 0;
	for (i = EEPROM_DEVICE_ADDR_CPY; i<EEPROM_DEVICE_ADDR_CPY+5; i++)
		eeprom_write(i, intF.deviceID[i-EEPROM_DEVICE_ADDR_CPY]);
	uart_printf("EEPROM Device Address Copy Write Done\n");
}

void eepromDeviceAddrCpyAckWrite(u8 byteValue)
{
	eeprom_write(EEPROM_DEVICE_ADDR_CPY_ACK, byteValue);
	uart_printf("EEPROM Device Address Copy Ack Write Done\n");
}