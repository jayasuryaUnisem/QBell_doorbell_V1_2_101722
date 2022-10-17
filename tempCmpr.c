/*+---------------------------------------------------------------------+*/
/*?				Document description					   		?*/
/*+---------------------------------------------------------------------?*/
/*?  Name    :  QBell DoorBell Device Main Code   						?*/
/*?  Author  :  Jayasurya Ramasamy  									?*/
/*?  Date    :  2022/07/16	  										   	?*/
/*?  Version :  V1.2  											  	   	?*/
/*+---------------------------------------------------------------------?*/
/*?				    Function description		   			  	   		?*/
/*+---------------------------------------------------------------------?*/
/*?  1.																	?*/
/*?  2. 															 	?*/
/*?  3.																	?*/
/*?  4. 					   											?*/
/*?  5.					   												?*/
/*?  6.																   	?*/
/*?  7.																   	?*/
/*+---------------------------------------------------------------------|*/

/*----------------------------------------------------------------------*/
/*							Header								    	*/
/*----------------------------------------------------------------------*/


#include "main.h"
#include <string.h>

extern int_flags intF;

char* a;

u16 ADC_Value;


extern struct define_flag Mflag;
//
extern _BC3602_device_ BC3602_T;


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//**********************************************************************//
// name		:	convert_integer_to_string
// input	:	char* const str -> pointer will stores that string 
//				value, unsigned char number -> number convert to string
// output	:	none
// function	:	for converting interger (unsigned short) to string(char*)
//**********************************************************************//

void convert_integer_to_string(char* const str, unsigned short number)
{
   
	u16 n, len = 0, i, rem;
	n = number;
	if(0 == n)
		len = 1;
	while (0 != n)
	{
		len++;
		n /= 10;	
	}
	 
	for ( i = 0; i < len; i++ )
	{
		rem = number % 10;
		number = number / 10;
		str[len - (i + 1)] = rem + '0';
	}
	uart_printf("Hello ADC: ");
	uart_printf(str);
	uart_printf("\n");
	str[len] = '\0';
}


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*********************************************//
// name		:	delay
// input	:	unsigned short time -> in milliseconds
// output	:	none
// function : 	Blocking delay
//*********************************************//

void delay(unsigned short time)
{
    unsigned short a;
	unsigned char b,c;
	
    for(a=0;a<time;a++)
    {
    	for(b=0;b<5;b++)
    	{
    		for(c=0;c<102;c++) asm("nop");
    	}
    }
}


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*********************************************//
// name		:	sysInit
// input	:	none
// output	:	none
// function :	which includes watchdog time clear, uart init, adc init,
//				i2c init, ktd rgb led driver init and other gpio inits
//*********************************************//

void sysInit()
{
    WDTC();
    uart_prot_init();
    delay(100);
    adc_init();
    IIC_Init();
    ktd_en();
    ktd_clear();
    delay(10);
   	int_init();
    delay(10); 
}


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*********************************************//
// name		:	main
// input	:	none
// output	:	none
// function :	main function
//*********************************************//
void main()
{
	WDTC();
	//eeprom_init();
	//uart_prot_init();
	Sys_init();
   	sysInit();

	RF_Init();																				// RF Initialization
	RF_Parameter_loading();

	BC3602_WriteRegister(IO2_REGS,0xcd);													// debug use
	BC3602_T.mode = RF_RX;	

	
	uart_printf("EEPROM Function Test Read only \n");
	//eeprom_write(1, 23);
	GCC_CLRWDT(); 
	delay(10);

	uart_printf("Device Address: ");

	eepromDeviceAddrRead();
	delay(10);
	eepromPstatusCheck();
	delay(10);

	u8 j;
	uart_printf("Device ID: ");

	for(j = 0; j<4; j++)
		uart_send(intF.deviceID[j]);

	uart_printf("\n");
	delay(10);
	
	switch (intF.pairStatus)
	{
		u8 i = 0;
	case NO_DEVICE_FOUND:
		uart_printf("No Device Paired Yet\n");
		break;
	
	case P1_DEVICE_CONN:
		eepromP1Read();
		uart_printf("P1 Device Paired: ");
		
		for(i = 0; i<4; i++)
			uart_send(intF.p1DeviceID[i]);
		uart_printf("\n");
		break;
	
	case P2_DEVICE_CONN:
		eepromP2Read();
		uart_printf("P2 Device Paired: ");
		
		for(i = 0; i<4; i++)
			uart_send(intF.p2DeviceID[i]);
		uart_printf("\n");
		break;
	
	case BOTH_DEVICE_CONN:
		eepromP1Read();
		eepromP2Read();
		uart_printf("Both the Device are Paired\n");
		uart_printf("P1 Device ID: ");
		
		for(i = 0; i<4; i++)
			uart_send(intF.p1DeviceID[i]);
		uart_printf("\nP2 Device ID: ");
		
		for(i = 0; i<4; i++)
			uart_send(intF.p2DeviceID[i]);
		uart_printf("\n");
		break;	
	
	default:
		break;
	}

	uart_printf("\n");

	delay(2000);
	
	while(1)
	{
		
		GCC_CLRWDT();
		if(1 == intF.pairingAck)
		{
			uart_printf("Pairing Ack Received\n");
			intF.pairingAck = 0;
			eepromPstatusCheck();
			delay(10);
			u8 i = 0;
			u8 tempArr[5];
			for(i = 0;i<4;i++)
				tempArr[i] = intF.rxPayload[i+1];
			switch (intF.pairStatus)
			{
			case NO_DEVICE_FOUND:
				uart_printf("Writing the ID to the P1 Address: ");
				eepromP1Write();
				delay(10);
				eepromP1Read();
				delay(10);
				eepromPstatusWrite(P1_DEVICE_CONN);
				delay(10);
				for(i = 0;i<4;i++)
					uart_send(intF.p1DeviceID[i]);
				uart_printf("\n");
				break;
			
			case P1_DEVICE_CONN:
				eepromP1Read();
				delay(10);
				if(memcmp(tempArr, intF.p1DeviceID, sizeof(intF.p1DeviceID)))
				{
					uart_printf("Writing the ID to the P2 Address: ");
					eepromP2Write();
					delay(10);
					eepromP2Read();
					delay(10);
					eepromPstatusWrite(BOTH_DEVICE_CONN);
					delay(10);
					for(i = 0;i<4;i++)
						uart_send(intF.p2DeviceID[i]);
					uart_printf("\n");
				}

				else
				{
					uart_printf("This device is already paired\n");
				}
				
				break;
			
			case P2_DEVICE_CONN:
				eepromP2Read();
				if(memcmp(tempArr, intF.p2DeviceID, sizeof(intF.p2DeviceID)))
				{
					uart_printf("Writing trhe ID to the P1 Address: ");
					eepromP1Write();
					delay(10);
					eepromP1Read();
					delay(10);
					eepromPstatusWrite(BOTH_DEVICE_CONN);
					delay(10);
					for(i = 0;i<4;i++)
						uart_send(intF.p1DeviceID[i]);
					uart_printf("\n");
				}
				else
				{
					uart_printf("This is already Paired with this device\n");
				}
				break;
			
			case BOTH_DEVICE_CONN:
				uart_printf("P1 and P2 both the address are busy\n");
				break;
			
			default:
				break;
			}
		}
		
		#if RF_Payload_Length>64
		if(BC3602_T.mode == RF_TX)	
		{
			uart_printf("Data Transmitting\n");														// while RF mode flag = TX
			ExtendFIFO_TX_Process(&BC3602_T);												// RF Extend FIFO TX porcess
		}
			
		if(BC3602_T.mode == RF_RX)															// while RF mode flag = RX
			ExtendFIFO_RX_Process(&BC3602_T);												// RF Extend FIFO RX porcess	
		
		#endif
		RF_Finished_Process(&BC3602_T);		
		
	}
	
	
}



// // //*********************************************//
// // // name		:	TB0_proc
// // //*********************************************//
// void __attribute((interrupt(0x1C))) TB0_proc(void)
// {
// 	Mflag.fs++;
// 	if(Mflag.fs>=10)
// 	{
// 		Mflag.fm = 1;
// 		Mflag.fs = 0;
// 	}
// 	Mflag.f8ms++;
// 	_pb6 = ~_pb6;
// }

