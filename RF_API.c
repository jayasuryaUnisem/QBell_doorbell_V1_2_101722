/*------------------------------------------------------------------*/
/*							Header									*/
/*------------------------------------------------------------------*/
#include "RF_API.h"
#include "sys_api.h"
         
/*------------------------------------------------------------------*/
/*							 Define									*/
/*------------------------------------------------------------------*/      
_BC3602_device_ BC3602_T;

volatile unsigned int Resent_Count;
unsigned char syncword[RF_SYNC_Length];
const unsigned char	margin_len[] = {4, 8, 16, 32};

/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*******************************//
// name		:	RF_Parameter_loading
// input	:	BC3602_T
// output	:	none
//*******************************//
void RF_Parameter_loading(void)
{
	unsigned char mode_sts;
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	mode_sts = BC3602_T.mode;
	BC3602_T.mode = RF_LightSleep;	
	BC3602_T.sync_word_len = RF_SYNC_Length;
	
	BC3602_ReadSyncWord(syncword,RF_SYNC_Length);
	BC3602_T.sync_word = (unsigned char*)syncword;
	
	BC3602_T.data_rate = RF_Datarate;
	BC3602_T.tx_power = RF_TxPower;
	
	BC3602_T.irq_status = BC3602_ReadRegister(IRQ3_REGS);
	
	BC3602_T.tx_preamble_len = BC3602_ReadRegister(PKT1_REGS);
	BC3602_T.tx_packet_len = BC3602_ReadRegister(PKT5_REGS);
	BC3602_T.tx_payload_buffer = RF_TXFIFO;

	BC3602_T.rx_preamble_len = BC3602_ReadRegister(PKT2_REGS) & 0x03;
	BC3602_T.rx_packet_len = BC3602_T.tx_packet_len;
	BC3602_T.rec_data_len = 0;
	BC3602_T.rx_payload_buffer = RF_RXFIFO;
//	BC3602_T.rssi = BC3602_ReadRegister(RSSI3_REGS);
	
	BC3602_T.mode = mode_sts;
	//BC3602_T.step = 0;
}


//*******************************//
// name		:	SimpleFIFO_TX_Process
// input	:	BC3602_T
// output	:	RF_SimpleFIFO_Mode
//*******************************//
#if RF_Payload_Length<65
void SimpleFIFO_TX_Process(_BC3602_device_ *BC3602_T)
{
//	unsigned char x;	
	switch((*BC3602_T).step)
	{
		case 0:
			(*BC3602_T).irq_status = 0;	
			BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
			(*BC3602_T).mode = RF_LightSleep;			 											
			BC3602_WriteRegister(IRQ2_REGS,_TXFSHI_);							// enable tx int
			
			#if RF_ARK_Enable
			RFEnARK();
			x=BC3602_ReadRegister(PKT2_REGS);											//PID
			if(x & 0x80)						
				x &= 0x7F;
			else
				x |= 0x80;
			BC3602_WriteRegister(PKT2_REGS,x);	
			#endif
			
			BC3602_SetTxPreambleWidth((*BC3602_T).tx_preamble_len);
			BC3602_StrobeCommand(REST_TX_POS_CMD);													
			BC3602_SetTxPayloadSAddr(0);
			BC3602_SetTxPayloadWidth((*BC3602_T).tx_packet_len);									
			BC3602_WriteTxPayload((*BC3602_T).tx_payload_buffer,(*BC3602_T).tx_packet_len);		
			BC3602_StrobeCommand(TX_MODE_CMD);	
			(*BC3602_T).mode = RF_TX;
			(*BC3602_T).step++;
			break;
			
		case 1:
			if(!RF_INT)
			{	
				(*BC3602_T).irq_status = BC3602_GetIRQState();	
			}
			if((*BC3602_T).irq_status & _TXFSHI_)			
			{	
				(*BC3602_T).tx_irq_f = 1;
				BC3602_ClearIRQFlag(_TXFSHI_);
				GCC_DELAY(100);
				BC3602_StrobeCommand(LIGHT_SLEEP_CMD);
				(*BC3602_T).mode = RF_RX;
				(*BC3602_T).irq_status &= ~_TXFSHI_;
				(*BC3602_T).step = 0;	
			}
			
			#if RF_ARK_Enable == 1
			if((*BC3602_T).irq_status & _ARKFAILI_)
			{
				(*BC3602_T).tx_fail_f = 1;
				BC3602_ClearIRQFlag(_ARKFAILI_);
				BC3602_StrobeCommand(LIGHT_SLEEP_CMD);
				(*BC3602_T).mode = RF_RX;
				(*BC3602_T).irq_status &= ~_ARKFAILI_;
				(*BC3602_T).step = 0;	
			}
			#endif
			
			if((*BC3602_T).irq_status)
			{
				BC3602_ClearIRQFlag((*BC3602_T).irq_status);
				(*BC3602_T).irq_status = 0;
				(*BC3602_T).step = 0;	
			}			
			break;	
			default: break;	
	}			
}
#endif

//*******************************//
// name		:	SimpleFIFO_RX_Process
// input	:	BC3602_T
// output	:	RF_SimpleFIFO_Mode
//*******************************//
#if RF_Payload_Length<65
void SimpleFIFO_RX_Process(_BC3602_device_ *BC3602_T)
{
	switch((*BC3602_T).step)
	{
		case 0:	
			(*BC3602_T).irq_status = 0;	
			BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
			(*BC3602_T).mode = RF_LightSleep;
																
//			memset((*BC3602_T).rx_payload_buffer,0,64);						// clear RX FIFO										
			BC3602_WriteRegister(IRQ2_REGS,0x12);							// enable rx int 								

			BC3602_RegisterBank(BC3602_BANK0);
			BC3602_StrobeCommand(REST_RX_POS_CMD);										
			BC3602_SetRxPayloadWidth((*BC3602_T).rx_packet_len)	;

			BC3602_StrobeCommand(RX_MODE_CMD);
			(*BC3602_T).mode = RF_RX;
			
			(*BC3602_T).step++;
			break;
			
		case 1:
			if(!RF_INT)
			{	
				(*BC3602_T).irq_status = BC3602_GetIRQState();	
			}
			if((*BC3602_T).irq_status & (_RXFSHI_|_RXFAILI_))			
			{	
				if((*BC3602_T).irq_status & _RXFSHI_)
				{
					(*BC3602_T).rx_irq_f = 1;
					(*BC3602_T).rec_data_len = BC3602_GetRxPayloadWidth();										// read packet length
					BC3602_ReadRxPayload((*BC3602_T).rx_payload_buffer,(*BC3602_T).rec_data_len);				// read payload
				}
				BC3602_ClearIRQFlag(_RXFSHI_|_RXFAILI_);
				
				#if RF_ARK_Enable
					GCC_DELAY(100000);																			//delay 50ms
				#endif
				(*BC3602_T).mode = RF_RX;
				BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
				(*BC3602_T).irq_status &= ~(_RXFSHI_|_RXFAILI_);
				(*BC3602_T).step= 0;			
			}
			if((*BC3602_T).irq_status)
			{
				BC3602_ClearIRQFlag((*BC3602_T).irq_status);
				(*BC3602_T).irq_status = 0;
			}
			break;	
		default: break;	
	}	
}
#endif

//*******************************//
// name		:	ExtendFIFO_TX_Process
// input	:	BC3602_T
// output	:	RF_ExtendFIFO_Mode
//*******************************//
#if RF_Payload_Length>64
void ExtendFIFO_TX_Process(_BC3602_device_ *BC3602_T)
{	
	unsigned char ExtTxLength = 0;			
	unsigned char x;
		
	switch((*BC3602_T).step)
	{
		case 0:	
			BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
			(*BC3602_T).mode = RF_LightSleep;													// RF entry LightSleep mode							
			Extend_FIFO_Enable();																// Enable TXCMPIF and FIFOLTIE,LowFIFO 32 bytes		
			x = (BC3602_ReadRegister(ATR1_REGS) & 0xfe);								
			BC3602_WriteRegister(ATR1_REGS,x);
			
			BC3602_SetTxPreambleWidth((*BC3602_T).tx_preamble_len);
			ExtTxLength=(*BC3602_T).tx_packet_len;
			BC3602_StrobeCommand(REST_TX_POS_CMD);													
			BC3602_SetTxPayloadSAddr(0);											
			BC3602_SetTxPayloadWidth((*BC3602_T).tx_packet_len);									
			if(ExtTxLength >= 11)
			{
				x = 25;	
			}
			else
			{
				x = ExtTxLength;
			}
			ExtTxLength -= x;	
			BC3602_WriteTxPayload((*BC3602_T).tx_payload_buffer,x);	
			(*BC3602_T).rec_data_len = x;
			BC3602_StrobeCommand(TX_MODE_CMD);
			(*BC3602_T).mode = RF_TX;
			(*BC3602_T).step++;
			break;

		case 1:
			if(!RF_INT)
			{	
				(*BC3602_T).irq_status = BC3602_GetIRQState();	
			}
			if((*BC3602_T).irq_status & _TRXFFMGI_)					
			{
				x = BC3602_ReadRegister(FIFO2_REGS) & 0x03;
				x = 25 - margin_len[x];	
				if(ExtTxLength < x)
				{
					x = ExtTxLength;	
				}
				ExtTxLength -= x;
				
				BC3602_WriteTxPayload((*BC3602_T).tx_payload_buffer+(*BC3602_T).rec_data_len,x);
				(*BC3602_T).rec_data_len += x;		
				BC3602_ClearIRQFlag(_TRXFFMGI_);					
				(*BC3602_T).irq_status &= ~ _TRXFFMGI_;	
			}		
			if((*BC3602_T).irq_status & _TXFSHI_)					
			{	
				(*BC3602_T).tx_irq_f = 1;
				BC3602_ClearIRQFlag(_TXFSHI_);
				BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
				(*BC3602_T).mode = RF_LightSleep;
				(*BC3602_T).irq_status &= ~ _TXFSHI_;	
			}
//			if((*BC3602_T).irq_status & _ARKFAILI_)					
//			{	
//				(*BC3602_T).tx_fail_f = 1;
//				BC3602_ClearIRQFlag(_ARKFAILI_);
//				BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
//				(*BC3602_T).mode = RF_LightSleep;
//				(*BC3602_T).irq_status &= ~ _ARKFAILI_;	
//			}
			if((*BC3602_T).irq_status)
			{
				BC3602_ClearIRQFlag((*BC3602_T).irq_status);
				(*BC3602_T).irq_status = 0;
			}
			break;
		default: break;	
	}	
}
#endif

//*******************************//
// name		:	ExtendFIFO_RX_Process
// input	:	BC3602_T
// output	:	RF_ExtendFIFO_Mode
//*******************************//
#if RF_Payload_Length>64
void ExtendFIFO_RX_Process(_BC3602_device_ *BC3602_T)
{
	unsigned char x;
	unsigned char rx_length;

	switch((*BC3602_T).step)
	{
		case 0:														
			(*BC3602_T).irq_status = 0;
			(*BC3602_T).rec_data_len = 0;	
			BC3602_StrobeCommand(LIGHT_SLEEP_CMD);													
			(*BC3602_T).mode = RF_LightSleep;
			//memset(RxPayloadData,0,64);						//clear RXpadloadData										
			x = (BC3602_ReadRegister(FIFO2_REGS)&0xf3)|0x07;					
			BC3602_WriteRegister(FIFO2_REGS,x);

			BC3602_WriteRegister(IRQ2_REGS,0x32);
						
			BC3602_RegisterBank(BC3602_BANK0);
//			if((*BC3602_T).en_wor)
//				x = (BC3602_ReadRegister(ATR1_REGS) & 0xF8) | 0x03;
//			else
				x = (BC3602_ReadRegister(ATR1_REGS) & 0xFE);
			BC3602_WriteRegister(ATR1_REGS,x);

			ExtTxLength=(*BC3602_T).rx_packet_len;
			BC3602_StrobeCommand(REST_RX_POS_CMD);										
			BC3602_WriteRegister(PKT6_REGS,ExtTxLength);
			
//			if((*BC3602_T).en_wor)
//			{
//				BC3602_StrobeCommand(IDLE_MODE_CMD);
//				(*BC3602_T).mode = RF_Idle;
//			}
//			else
//			{	
				BC3602_StrobeCommand(RX_MODE_CMD);
				(*BC3602_T).mode = RF_RX;
//			}
			(*BC3602_T).step++;
			break;
			
		case 1:
//			if((*BC3602_T).mode == RF_RX)
//				(*BC3602_T).env_rssi = BC3602_ReadRegister(RSSI3_REGS);			
			if(!RF_INT)
			{	
				(*BC3602_T).irq_status = BC3602_GetIRQState();
			}			
			if((*BC3602_T).irq_status & _TRXFFMGI_)				
			{		
				rx_length  = BC3602_ReadRegister(PKT6_REGS);				
				ExtRxLength -= rx_length;
				BC3602_ReadRxPayload((*BC3602_T).rx_payload_buffer+(*BC3602_T).rec_data_len,rx_length);						
				(*BC3602_T).rec_data_len += rx_length;
				BC3602_ClearIRQFlag(_TRXFFMGI_);
				(*BC3602_T).irq_status &= ~_TRXFFMGI_;						
			}
			if((*BC3602_T).irq_status & (_RXFSHI_|_RXFAILI_))		
			{	
				if((*BC3602_T).irq_status & _RXFSHI_)
					(*BC3602_T).rx_irq_f = 1;
				rx_length = BC3602_ReadRegister(PKT6_REGS);															// packet length
				(*BC3602_T).rssi = BC3602_ReadRegister(RSSI4_REGS);
				BC3602_ReadRxPayload((*BC3602_T).rx_payload_buffer+(*BC3602_T).rec_data_len,rx_length);				// read payload
				(*BC3602_T).rec_data_len += rx_length;
				BC3602_ClearIRQFlag(_RXFSHI_|_RXFAILI_);
				BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
				(*BC3602_T).mode = RF_RX;
				(*BC3602_T).irq_status &= ~(_RXFSHI_|_RXFAILI_);	
				(*BC3602_T).step = 0;	
			}
//		#if	_TEST_WOR_IRQ_
//			if((*BC3602_T).irq_status & _WOTWKPI_)					
//			{
//				_pb4 = 1;
//				BC3602_ClearIRQFlag(_WOTWKPI_);
//				_pb4 = 0;
//			}
//		#endif
			if((*BC3602_T).irq_status)
			{
				BC3602_ClearIRQFlag((*BC3602_T).irq_status);
				(*BC3602_T).irq_status = 0;
			}
			break;	
		default: break;	
	}
}
#endif

//*******************************//
// name		:	ATR_WOT_Process
// input	:	BC3602_T
// output	:	RF_WOT_Mode
//*******************************//
#if RF_ATR_Enable == 1
void ATR_WOT_Process(_BC3602_device_ *BC3602_T)
{
	unsigned char x;
	RF_TXFIFO[0] = 0x01;															// user must put data into RF_TXFIFO[0] 
	DATA_Process(RF_TXFIFO);														// user must put data into RF_TXFIFO[1 ~ TXpadloadlenth-1]
	
	BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
	(*BC3602_T).mode = RF_LightSleep;
	BC3602_StrobeCommand(REST_TX_POS_CMD);											// reset TX
	BC3602_WriteTxPayload((*BC3602_T).tx_payload_buffer,(*BC3602_T).tx_packet_len);	
	BC3602_SetTxPreambleWidth((*BC3602_T).tx_preamble_len);
	BC3602_SetTxPayloadSAddr(0);
	BC3602_SetTxPayloadWidth((*BC3602_T).tx_packet_len);	
	
	x = BC3602_ReadRegister(ATR1_REGS);												// Enable WOT mode
	x &= 0xf9;
	BC3602_WriteRegister(ATR1_REGS,x);	
	
	RFEnATR();																		// Enable ATR	
	BC3602_StrobeCommand(IDLE_MODE_CMD);											// Enable Idel mode
}
#endif


//*******************************//
// name		:	ATR_WOR_Process
// input	:	BC3602_T
// output	:	RF_WOR_Mode
//*******************************//
#if RF_ATR_Enable == 1
void ATR_WOR_Process(_BC3602_device_ *BC3602_T)
{
	switch((*BC3602_T).step)
	{
		case 0:	
			BC3602_StrobeCommand(LIGHT_SLEEP_CMD);	
			(*BC3602_T).mode = RF_LightSleep;
			RFEnATR();																		// Enable ATR	
			BC3602_StrobeCommand(IDLE_MODE_CMD);											// Enable Idel mode	
			
			(*BC3602_T).step++;
			break;
		case 1:
			if(!RF_INT)
			{	
				(*BC3602_T).irq_status = BC3602_GetIRQState();	
			}
			if((*BC3602_T).irq_status & (_RXFSHI_|_RXFAILI_))			
			{	
				if((*BC3602_T).irq_status & _RXFSHI_)
					(*BC3602_T).rx_irq_f = 1;
			
				(*BC3602_T).rec_data_len = BC3602_GetRxPayloadWidth();										// read packet length
				BC3602_ReadRxPayload((*BC3602_T).rx_payload_buffer,(*BC3602_T).rec_data_len);				// read payload
				
				BC3602_ClearIRQFlag(_RXFSHI_|_RXFAILI_);
				BC3602_StrobeCommand(IDLE_MODE_CMD);	
				(*BC3602_T).irq_status &= ~(_RXFSHI_|_RXFAILI_);
				(*BC3602_T).step= 0;		
			}
			if((*BC3602_T).irq_status)
			{
				BC3602_ClearIRQFlag((*BC3602_T).irq_status);
				(*BC3602_T).irq_status = 0;
			}
			break;	
		default: break;	
	}		
}
#endif
