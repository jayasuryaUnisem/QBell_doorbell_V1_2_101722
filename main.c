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
    //WDTC();
    uart_prot_init();
    adc_init();
    IIC_Init();
    ktd_en();
    ktd_clear();
   	int_init();
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
	
	app_pair_status_check();
	delay(10);

	_pb4 = 0;
	_pbc4 = 1;
	
	while(1)
	{
		GCC_CLRWDT();
		//_emi = 1;
		app();
	}
	
	
}





