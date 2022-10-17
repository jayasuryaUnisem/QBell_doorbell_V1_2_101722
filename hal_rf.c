#include "hal_rf.h"

struct define_flag Mflag;

extern _BC3602_device_ BC3602_T;

extern int_flags intF;

const unsigned char BC3602_pn9_dataM[5] = 
{
	48,1,1,4,8
};

void Sys_init(void)
{
   _papu = 0xFF;																			// I/O Initialization
	_pbpu = 0xFF;
	_pcc = 0xFF;
	_pcpu = 0xFF;
	_pdc = 0xFF;
	_pdpu = 0xFF;

	fun_RamInit();																			// clear RAM all bank

	
	_psc0r = 0x02;																			// prescaler = fsub
	_scc = 0x00;
	_hircc = 0x01;																			// 8MHz, hircen 
	
	//_wdtc = 0x57;			
	_wdtc = 0xa8;																// wdt 8s
	
	/*
		Timer based interrupt enabled
	*/
	
	_psc0r = 0B00000010;																	// TB0 Initialization
	_tb0c  = 0B00000000;																	// TB0 = 8ms
	_tb0on = 1;																				// enable tb0
	_tb0e  = 1;	
	// _tb02 = 1;
	// _tb01 = 1;
	// _tb00 = 1;																			// enable tb0 interrupt
	
	RF_intxS1 = 1;																			// INT0 for RF IRQ
	RF_intxS0 = 0;																			// INT0 falling edge
	RF_INT_CTR = 1;																			// PA1 set input
	RF_INT_PU = 1;																			// PA1 set pull-high
	RF_intxPS = 1;																			// PA1 for INT0
	RF_INT_EN = 1;																			// enable INT0 interrupt
	
	// _emi   = 1;	

}
void fun_RamInit()
{
    for(_mp1h=0;_mp1h<4;_mp1h++)
    {
        _mp1l = 0x80;
        for(_tblp=0x80;_tblp>0;_tblp--)
        {
            _iar1 = 0;
            _mp1l++;
        }
    }
    _mp1h=0;
}


void RF_Process_Init(void)
{
    Sys_init();
    RF_Init();
    RF_Parameter_loading();
    //	BC3602_WriteRegister(IO2_REGS,0xcd);
    BC3602_T.mode = RF_RX;
}

void RF_Process(void)
{
    GCC_CLRWDT();

    if(Mflag.Key_Stas)
    {
        if(BC3602_T.mode != RF_TX)
        {
            KEY_Process(Mflag.Key_Stas, intF.battStatus);
        }
        Mflag.Key_Stas = 0;
    }

}





//*********************************************//
// name		:	KEY_Process
// input	:	key_status
// output	:	none
//*********************************************//
void KEY_Process(unsigned char key_status, unsigned char battLevel)
{
	switch(key_status)
	{
		case NOTIFI_REQ:
			RF_TXFIFO[0] = NOTIFI_REQ;	
			uart_printf("Sending Doorbell Notification\n");													// put data into RF_TXFIFO
			DATA_Process(RF_TXFIFO);														// put data into RF_TXFIFO
			BC3602_T.step = 0;																// reset RF step
			BC3602_T.mode = RF_TX;															// set RF mode flag as TX															
			break;
			
		case PAIR_REQ:
			RF_TXFIFO[0] = PAIR_REQ;
			uart_printf("Sending Pairing Request\n");
			DATA_Process(RF_TXFIFO);
			BC3602_T.step = 0;
			BC3602_T.mode = RF_TX;
			break;
			
		case UNPAIR_REQ:
			RF_TXFIFO[0] = UNPAIR_REQ;
			uart_printf("Sending Unpairing Request\n");
			DATA_Process(RF_TXFIFO);
			BC3602_T.step = 0;
			BC3602_T.mode = RF_TX;
			break;
			
		case PAIR_CONF_REQ:
			RF_TXFIFO[0] = PAIR_CONF_REQ;
			uart_printf("Sending Pairing Confirmation Request\n");
			DATA_Process(RF_TXFIFO);
			BC3602_T.step = 0;
			BC3602_T.mode = RF_TX;
			break;	
		
		case UNPAIR_CONF_REQ:
			RF_TXFIFO[0] = UNPAIR_CONF_REQ;
			uart_printf("Sending Unpairing Confirmation Request\n");
			DATA_Process(RF_TXFIFO);
			BC3602_T.step = 0;
			BC3602_T.mode = RF_TX;
			break;
			
		default:
			break;
	}
}

//*********************************************//
// name		:	RF_Finished_Process
// input	:	key_status
// output	:	none
//*********************************************//
void RF_Finished_Process(_BC3602_device_ *BC3602_T)
{
	
	if((*BC3602_T).tx_irq_f)															// while TX finished
	{
		//uart_printf("Sending..")
		switch(RF_TXFIFO[0])															// flash LED show TX success
		{
			case 0x01:
				RGB_G = 1;
				break;
			case 0x02:
				RGB_G = 1;     
				break;
			case 0x03:
				RGB_G = 1;
				break;
			case 0x04:
				RGB_G = 1;
				break;
			default:
				break;
		}		
		(*BC3602_T).tx_irq_f = 0;												
		(*BC3602_T).step = 0;
		(*BC3602_T).mode = RF_RX;														// set RF mode flag as RX
	}
	
	if((*BC3602_T).tx_fail_f)															// while TX finished
	{
		switch(RF_TXFIFO[0])															// flash LED 2 times show TX fail
		{
			case 0x01:
				break;
			case 0x02:
				break;
			case 0x03:
				break;
			case 0x04:
				break;
			default:
				break;
		}														
		(*BC3602_T).step = 0;
		(*BC3602_T).tx_fail_f = 0;	
		(*BC3602_T).mode = RF_RX;														// set RF mode flag as RX
	}
	
	if((*BC3602_T).rx_irq_f)															// while RX finished
	{
		u8 i = 0;
		for(i = 0; i<10; i++)
			intF.rxPayload[i]= RF_RXFIFO[i];
																									// while RX finished and CRC OK
		switch(RF_RXFIFO[0])															// check RF DATA
		{
			case NOTIFI_REQ:
				intF.pairingAck = 1;
				uart_printf("Doorbell Notification Ack Received: ");
				//delay(10);
				break;

			case PAIR_REQ:
				uart_printf("Pairing Request Received: ");
				intF.pairingAck = 1;
				break;

			case PAIR_CONF_REQ:
				uart_printf("Pairing Confirmation Request: ");
				intF.pairingAck = 1;
				break;

			case UNPAIR_REQ:
				uart_printf("Unpairing Request Received: ");
				unpairSpNode();
				intF.pairingAck = 1;
				break;

			case UNPAIR_CONF_REQ:
				uart_printf("Unpairing Confirmation Request Received: ");
				intF.pairingAck = 1;
				break;

			default:
				uart_printf("Unknow Packet Received: ");
				break;
		}											
		uart_printf((char *)intF.rxPayload);
		uart_printf("\n");

		(*BC3602_T).step = 0;
		(*BC3602_T).rx_irq_f = 0;
		(*BC3602_T).mode = RF_RX;														// set RF mode flag as RX		
	}
}

//*********************************************//
// name		:	DATA_Process
// input	:	RF_TXFIFO
// output	:	none
//*********************************************//
void DATA_Process(unsigned char *FIFO)
{
	unsigned char x;
	uart_printf("Transmitting Packet: ");
	#if !MODE_FLASHING
	intF.deviceID[4] = intF.battStatus;
	#endif

	#if MODE_FLASHING
	//BC3602_pn9_dataM[4] = intF.battStatus;
	#endif
	for(x=0;x<11-1;x++)
	{
		#if !MODE_FLASHING
		FIFO++;
		*FIFO = intF.deviceID[x];
		uart_printf(intF.deviceID[x]);
		#endif

		#if MODE_FLASHING
		FIFO++;
		*FIFO = BC3602_pn9_dataM[x];
		uart_send(BC3602_pn9_dataM[x]);
		#endif
	}
	uart_printf("\n");
}

//*********************************************//
// name		:	System_deepsleep
// input	:	none
// output	:	none
// remark	:	After wake up from this deepsleep mode ,
//				system must to execute the I/O init again.
//*********************************************//
void System_deepsleep(void)
{	
	//_pas0 = 0;
	//_pas1 = 0;
	// _pbs0 = 0;
	// _pbs1 = 0;
	// _pcs0 = 0;
	// _pcs1 = 0;
	// _pds0 = 0;
	
	// //_pa = 0xff;
	// //_pac = 0;
	// //_pawu = 0;
	// _pb = 0xff;
	// _pbc = 0;
	// _pc = 0xff;
	// _pcc = 0;
	// _pd = 0;

	_emi = 0;
	_wdtc = 0xa8;	//WDT off
	_lvden = 0;		//LVD off
	_fhiden =0;		//HIRC
	_fsiden =0;		//LIRC
	_halt();
}


// //*********************************************//
// // name		:	TB0_proc
// //*********************************************//
void __attribute((interrupt(0x1C))) TB0_proc(void)
{
	if(1)
	{
		Mflag.fs++;
	}
	//else _nop();
	
}