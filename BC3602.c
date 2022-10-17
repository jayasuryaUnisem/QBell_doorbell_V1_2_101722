/*------------------------------------------------------------------*/
/*							Header									*/
/*------------------------------------------------------------------*/
#include "MCU.h"
#include "BC3602_CONFIG_TABLE.h"
#include "BC3602.h"

/*------------------------------------------------------------------*/
/*					      Statement									*/
/*------------------------------------------------------------------*/
unsigned char ExtTxLength=255;
unsigned char ExtRxLength;
unsigned char RF_TXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h
unsigned char RF_RXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h

unsigned char RF_IRQ;
unsigned char mRFSync[RF_SYNC_Length];			//SYNC length defined in Configuration.h
   
         
/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*******************************//
// name		:	SpiWriteRegCMD
// input	:	CMD,DATA
// output	:	none
//*******************************//
void SpiWriteRegCMD(unsigned char REG,unsigned char DATA)
{
	SpiCSN_low();
	SpiWrite(REG | WRITE_REGS_CMD);
	SpiWrite(DATA);
	SpiCSN_high();
}

//*******************************//
// name		:	SpiWriteStrobeCMD
// input	:	CMD
// output	:	none
//*******************************//
void SpiWriteStrobeCMD(unsigned char CMD)
{
	SpiCSN_low();
	SpiWrite(CMD);
	SpiCSN_high();
}

//*******************************//
// name		:	SpiWriteByte
// input	:	CMD
// output	:	ReadDATA
//*******************************//
unsigned char SpiReadRegCMD(unsigned char REG)
{
	unsigned char ReadDATA;
	SpiCSN_low();
	SpiWrite(REG | READ_REGS_CMD);
	ReadDATA = SpiRead();
	SpiCSN_high();
	return	ReadDATA;
}

//*******************************//
// name		:	RFWriteBuf
// input	:	CMD,length,data[]
// output	:	none
//*******************************//
void RFWriteBuf(unsigned char CMD,unsigned char length,unsigned char data[])
{
	unsigned char a;
	SpiCSN_low();
	SpiWrite(CMD);
	for(a=0;a<length;a++) SpiWrite(data[a]);
	SpiCSN_high();
}
void RFWriteBuf2(unsigned char CMD,unsigned char length,const unsigned char *data)
{
	unsigned char a;
	SpiCSN_low();
	SpiWrite(CMD);
	for(a=0;a<length;a++) 
	{
		SpiWrite(*data);
		data++;
	}
	SpiCSN_high();
}

//*******************************//
// name		:	RFReadBuf
// input	:	CMD,length
// output	:	data[]
//*******************************//
void RFReadBuf(unsigned char CMD,unsigned char length,unsigned char data[])
{
	unsigned char a;
	SpiCSN_low();
	SpiWrite(CMD);
	for(a=0;a<length;a++) data[a]=SpiRead();
	SpiCSN_high();
}

void RFReadBuf2(unsigned char CMD,unsigned char length,unsigned char *data)
{
	unsigned char a;
	SpiCSN_low();
	SpiWrite(CMD);
	for(a=0;a<length;a++) 
	{
		*data=SpiRead();
		data++;
	}
	SpiCSN_high();
}
//*******************************//
// name		:	WriteFIFO
// input	:	none
// output	:	none
//*******************************//
void WriteFIFO(unsigned char *rf_fifo,unsigned char payload_length)
{
	unsigned char a=0;	
	SpiCSN_low();
	SpiWrite(WRITE_FIFO_CMD);
	for(a=0;a<payload_length;a++)
	{
		SpiWrite(*rf_fifo);
		rf_fifo++;
	}
	SpiCSN_high();
}

//*******************************//
// name		:	ReadFIFO
// input	:	none
// output	:	none
//*******************************//
void ReadFIFO(unsigned char *rf_fifo,unsigned char length)
{
	unsigned char a=0;	
	SpiCSN_low();
	SpiWrite(READ_FIFO_CMD);
	for(a=0;a<length;a++)
	{
		*rf_fifo = SpiRead();
		rf_fifo++;
	}
	SpiCSN_high();
}

//*******************************//
// name		:	RFEnATR
// input	:	none
// output	:	none
//*******************************//
void RFEnATR(void)
{
	unsigned char a=0;
	a  = SpiReadRegCMD(ATR1_REGS);
	a |= 0x01;
	SpiWriteRegCMD(ATR1_REGS,a);
}

//*******************************//
// name		:	RFDisATR
// input	:	none
// output	:	none
//*******************************//
void RFDisATR(void)
{
	unsigned char a=0;
	a  = SpiReadRegCMD(ATR1_REGS);
	a &= 0xFE;
	SpiWriteRegCMD(ATR1_REGS,a);
}

//*******************************//
// name		:	RFXtalReady
// input	:	none
// output	:	none
//*******************************//
void RFXtalReady(void)
{
	unsigned char a=0;
	while(a == 0)
	{
		a=SpiReadRegCMD(RC1_REGS);
		a &= 0x20;
	}
}

//*******************************//
// name		:	ResetLvRF
// input	:	none
// output	:	none
//*******************************//
void ResetLvRF(void)
{
	unsigned char a=0;
	while(a == 0)
	{
		a=SpiReadRegCMD(RC1_REGS);
		a |= 0x01;
		SpiWriteRegCMD(RC1_REGS,a);
		a=SpiReadRegCMD(RC1_REGS);
		a &= 0xFE;
		SpiWriteRegCMD(RC1_REGS,a);
	}
}

//*******************************//
// name		:	BC3602_Config
// input	:	none
// output	:	none
//*******************************//
void BC3602_Config(void)
{
	unsigned char a=0;
	//	BC3601 IRQ/IO Configure	//
	for(a=0;a<(sizeof IRQIO_REGS_TALBE/2);a++)	SpiWriteRegCMD(((IRQIO_REGS_TALBE[a]>>8)), IRQIO_REGS_TALBE[a]);
	//	BC3602 packet format Configure	//
	for(a=0;a<(sizeof PACKET_REGS_TALBE/2);a++)	SpiWriteRegCMD(((PACKET_REGS_TALBE[a]>>8)), PACKET_REGS_TALBE[a]);
	//	BC3602 common Configure	//
	for(a=0;a<(sizeof COMMON_REGS_TALBE/2);a++)	SpiWriteRegCMD(((COMMON_REGS_TALBE[a]>>8)), COMMON_REGS_TALBE[a]);
	
	//	BC3602 Bank0 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
	for(a=0;a<(sizeof BANK0_REGS_TALBE/2);a++)	SpiWriteRegCMD(((BANK0_REGS_TALBE[a]>>8)), BANK0_REGS_TALBE[a]);
	//	BC3602 Bank1 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK1);
	for(a=0;a<(sizeof BANK1_REGS_TALBE/2);a++)	SpiWriteRegCMD(((BANK1_REGS_TALBE[a]>>8)), BANK1_REGS_TALBE[a]);
	//	BC3602 Bank2 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK2);
	for(a=0;a<(sizeof BANK2_REGS_TALBE/2);a++)	SpiWriteRegCMD(((BANK2_REGS_TALBE[a]>>8)), BANK2_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
}

//*******************************//
// name		:	RFWriteSyncword
// input	:	none
// output	:	none
//*******************************//
void RFWriteSyncword(void)
{
	unsigned char a;
	
#if RF_SYNC_BCHcoding
	
	#if (RF_SYNC_Length != 4)
	unsigned char temp1[RF_SYNC_Length];
	#endif
	
	#if (RF_SYNC_Length == 4)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
	#if (RF_SYNC_Length == 6)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a]=mRFSync[a];
	for(a=0;a<2;a++) mLap[a]=BC3602_SYNCWORD[a+4];
	mLap[2]=0;
	mLap[3]=0;
	BCH32_Syncword();
	for(a=0;a<2;a++) temp1[a+4]=mRFSync[a];
	for(a=0;a<6;a++) mRFSync[a]=temp1[a];
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
	#if (RF_SYNC_Length == 8)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a]=mRFSync[a];
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a+4];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a+4]=mRFSync[a];
	for(a=0;a<8;a++) mRFSync[a]=temp1[a];
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
#else
	
	for(a=0;a<RF_SYNC_Length;a++)
		mRFSync[a] = BC3602_SYNCWORD[a];
	RFWriteBuf(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);
	
#endif
	
}

//*******************************//
// name		:	RFReadSyncword
// input	:	none
// output	:	none
//*******************************//
void RFReadSyncword(void)
{
	//RFReadBuf(READ_SYNCWORD_CMD,RF_SYNC_Length,rd_temp);
}

//*******************************//
// name		:	RFSetDRPram
// input	:	none
// output	:	none
//*******************************//
void RFSetDRPram(void)
{
	unsigned char a=0;
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
	for(a=0;a<(sizeof DM_REGS_TALBE/2);a++)	SpiWriteRegCMD(((DM_REGS_TALBE[a]>>8)), DM_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK1);
	for(a=0;a<(sizeof FCF_REGS_TALBE/2);a++)	SpiWriteRegCMD(((FCF_REGS_TALBE[a]>>8)), FCF_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK2);
	for(a=0;a<(sizeof CBPF_REGS_TALBE/2);a++)	SpiWriteRegCMD(((CBPF_REGS_TALBE[a]>>8)), CBPF_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
}

//*******************************//
// name		:	RFWriteFreqTABLE
// input	:	none
// output	:	none
//*******************************//
void RFWriteFreqTABLE(void)
{
	unsigned char a=0;
	a = SpiReadRegCMD(OM_REGS);
	a = (a&0x9f)|BAND_SEL;
	SpiWriteRegCMD(OM_REGS,a);
	
	for(a=0;a<(sizeof Frequency_REGS_TALBE/2);a++)	SpiWriteRegCMD(((Frequency_REGS_TALBE[a]>>8)), Frequency_REGS_TALBE[a]);
}

//*******************************//
// name		:	RFWriteFreq
// input	:	none
// output	:	none
//*******************************//
void RFWriteFreq(unsigned char *buffer)
{
	unsigned char a=0;	

	a = SpiReadRegCMD(OM_REGS);
	a = (a&0x9f)|BAND_SEL;
	SpiWriteRegCMD(OM_REGS,a);
	
	for(a=0;a<4;a++)
	{
		SpiWriteRegCMD(((Frequency_REGS_TALBE[a]>>8)), *buffer);
		buffer++;
	}	
}

//*******************************//
// name		:	RFSetPower
// input	:	power
// output	:	none
//*******************************//
void RFSetPower(unsigned char band_sel,unsigned char power)
{
	SpiWriteRegCMD(TX2_REGS,TxPowerValue[band_sel][power]);
}

//*******************************//
// name		:	RFCalibration
// input	:	none
// output	:	none
//*******************************//
void RFCalibration(void)
{
	unsigned char a=0;
	a=SpiReadRegCMD(OM_REGS);
	a |= 0x08;
	SpiWriteRegCMD(OM_REGS,a);
	while(a)
	{
		a=SpiReadRegCMD(OM_REGS);
		a &= 0x08;
	}
}

//*******************************//
// name		:	RFGetClrRFIrq
// input	:	none
// output	:	irq_status
//*******************************//
unsigned char RFGetClrRFIrq(void)
{
	unsigned char irq;
	
	irq=SpiReadRegCMD(IRQ3_REGS);
	SpiWriteRegCMD(IRQ3_REGS,irq);
	
	return irq;
}

//*******************************//
// name		:	Reset TX/RX FIFO
// input	:	none
// output	:	none
//*******************************//
void ResetTXFIFO(void)
{
	SpiWriteStrobeCMD(REST_TX_POS_CMD);
}

void ResetRXFIFO(void)
{
	SpiWriteStrobeCMD(REST_RX_POS_CMD);
}

//*******************************//
// name		:	EntryLightSleep
// input	:	none
// output	:	none
//*******************************//
void EntryLightSleep(void)
{
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
}

//*******************************//
// name		:	EntryDeepSleep
// input	:	none
// output	:	none
//*******************************//
void EntryDeepSleep(void)
{
	SpiWriteStrobeCMD(DEEP_SLEEP_CMD);
}

//*******************************//
// name		:	EntryIdleMode
// input	:	none
// output	:	none
//*******************************//
void EntryIdleMode(void)
{
	SpiWriteStrobeCMD(IDLE_MODE_CMD);
}

//*******************************//
// name		:	LircCalibration
// input	:	none
// output	:	none
//*******************************//
void LircCalibration(void)
{
	unsigned char a=0;
	a=SpiReadRegCMD(XO3_REGS);
	a |= 0x81;
	SpiWriteRegCMD(XO3_REGS,a);
	while(a)
	{
		a=SpiReadRegCMD(XO3_REGS);
		a &= 0x80;
	}
}

//*******************************//
// name		:	RF_Init
// input	:	none
// output	:	none
//*******************************//
void RF_Init(void)
{
	SpiIOInti();												// RF SPI I/O Initialization
	
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);							// Entry Lightsleep mode
	
	//RFXtalReady();											// Whaiting XTAL ready
	ResetLvRF();												// Reset LV
	SpiWriteStrobeCMD(SOFT_RESET_CMD);							// RF software reset
	
	//RFXtalReady();											// Whaiting XTAL ready
	_emi = 0;													// Disable EMI
	BC3602_Config();											// RF register configuration
	RFWriteSyncword();											// Set RF SYNC
	//RFReadSyncword();
	RFWriteFreqTABLE();												// Set RF working frequency
	RFSetDRPram();												// Set RF datarate
	RFSetPower(RF_BAND,RF_TxPower);								// Set RF output power
	_emi = 1;													// Enable EMI
	
	RFXtalReady();												// Whaiting XTAL ready
	RFCalibration();											// RF Calibration
	LircCalibration();											// LIRC Calibration
	RFGetClrRFIrq();											// Clear RF IRQ status
	_ifs |= 0x10;
	SpiWriteStrobeCMD(DEEP_SLEEP_CMD);							// Entry Deepsleep mode
}

//*******************************//
// name		:	RF_TxCarrier
// input	:	none
// output	:	none
//*******************************//
void RF_TxCarrier(void)
{
	unsigned char a=0;
	
	a = SpiReadRegCMD(CFG1_REGS);
	a |= 0x10;
	SpiWriteRegCMD(CFG1_REGS,a);
	
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK1);
	SpiWriteRegCMD(FCF2_REGS,0x00);
	SpiWriteRegCMD(FCF3_REGS,0x00);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
	
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	RFXtalReady();
	
	a = SpiReadRegCMD(OM_REGS);
	a |= 0x03;
	SpiWriteRegCMD(OM_REGS,a);
	a |= 0x04;
	SpiWriteRegCMD(OM_REGS,a);
}

//*******************************//
// name		:	RF_RxBER
// input	:	none
// output	:	none
//*******************************//
void RF_RxBER(void)
{
	unsigned char a=0;
	
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
	a = SpiReadRegCMD(CFG1_REGS);
	a |= 0x10;
	SpiWriteRegCMD(CFG1_REGS,a);		//direct mode
	a = SpiReadRegCMD(PKT2_REGS);	
	a &= 0xFC;
	SpiWriteRegCMD(PKT2_REGS,a);		//RX preamble set 0
	
	SpiWriteRegCMD(IO2_REGS,0x40);
	a = SpiReadRegCMD(IO3_REGS);	
	a |= 0x10;
	SpiWriteRegCMD(IO3_REGS,a);		//GIO4 set RX data out
	
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	RFXtalReady();
	
	a = SpiReadRegCMD(OM_REGS);
	a |= 0x01;
	SpiWriteRegCMD(OM_REGS,a);
	a |= 0x04;
	SpiWriteRegCMD(OM_REGS,a);
}

//*******************************//
// name		:	RF_WriteFreq
// input	:	band_sel,buffer[3:0] for SX1~4
// output	:	none
//*******************************//
void RF_WriteFreq(unsigned char band_sel,unsigned char *buffer)
{
	unsigned char a=0;
	//set rf band regs OM band_sel bit
	a = SpiReadRegCMD(OM_REGS);
	a |= (band_sel << 5);
	SpiWriteRegCMD(OM_REGS,a);
	
	//set rf band regs SX1~4
	//for(a=0;a<4;a++)	SpiWriteRegCMD(((SX1_REGS+a)),buffer[a]);
	RFWriteBuf2((SX1_REGS | WRITE_REGS_CMD),4,buffer);
}

//*******************************//
// name		:	RF_SetSniffPrm
// input	:	buffer[4:0] for ATR2~6
// output	:	none
//*******************************//
void RF_SetSniffPrm(unsigned char *buffer)
{

	//set ATR regs ATR2~6
	//unsigned char a=0;
	//for(a=0;a<4;a++)	SpiWriteRegCMD(((ATR2_REGS+a)),buffer[a]);
	RFWriteBuf2((ATR2_REGS | WRITE_REGS_CMD),4,buffer);
}

//*******************************//
// name		:	RF_SetAddr
// input	:	buffer[1:0] for PKT8~9
// output	:	none
//*******************************//
void RF_SetAddr(unsigned char *buffer)
{
	
	
	//set ATR regs ATR2~6
	//unsigned char a=0;
	//for(a=0;a<2;a++)	SpiWriteRegCMD(((PKT8_REGS+a)),buffer[a]);
	RFWriteBuf2((PKT8_REGS | WRITE_REGS_CMD),2,buffer);
}

//*******************************//
// name		:	RF_EntryRxMode	
// input	:	none
// output	:	none
//*******************************//
void RF_EntryRxMode(void)
{
//	unsigned char a=0;
	
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	RFXtalReady();
	
//	//enable FSYCK_EN
//	a = SpiReadRegCMD(RC1_REGS);
//	a |= 0x02;
//	SpiWriteRegCMD(RC1_REGS,a);
//	//set GIO4=FSYCK output
//	SpiWriteRegCMD(IO2_REGS,0xA0);
	
	RFDisATR();
	RFCalibration();
	
	SpiWriteStrobeCMD(REST_RX_POS_CMD);
	SpiWriteRegCMD(PKT6_REGS,RF_Payload_Length);
	
	SpiWriteStrobeCMD(RX_MODE_CMD);
}

//*******************************//
// name		:	RF_EntrySniffMode
// input	:	none
// output	:	none
//*******************************//
void RF_EntrySniffMode(void)
{
	unsigned char a;
	
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	RFXtalReady();
	
	RFDisATR();
	a  = SpiReadRegCMD(ATR1_REGS);
	a &= 0xF9;
	a |= 0x02;
	SpiWriteRegCMD(ATR1_REGS,a);
	
	RFCalibration();
	RFEnATR();
	SpiWriteStrobeCMD(IDLE_MODE_CMD);
}

//*******************************//
// name		:	RF_EntryWOT_Mode
// input	:	none
// output	:	none
//*******************************//
void RF_EntryWOT_Mode(void)
{
	unsigned char a;

	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	RFXtalReady();
	
	RFDisATR();
	a  = SpiReadRegCMD(ATR1_REGS);
	a &= 0xF9;
	SpiWriteRegCMD(ATR1_REGS,a);
	
	RFCalibration();
	RFEnATR();
	SpiWriteStrobeCMD(IDLE_MODE_CMD);
}

//*******************************//
// name		:	RF_SendData	
// input	:	none
// output	:	none
//*******************************//
void RF_SendData(void)
{
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	RFXtalReady();
	
	RFDisATR();
	RFCalibration();
	
	SpiWriteRegCMD(PKT5_REGS,RF_Payload_Length);
	SpiWriteStrobeCMD(REST_TX_POS_CMD);
#if RF_Payload_Length>64
	WriteFIFO(RF_TXFIFO,64);
#else
	WriteFIFO(RF_TXFIFO,RF_Payload_Length);
#endif
	
	SpiWriteStrobeCMD(TX_MODE_CMD);
}

//*******************************//
// name		:	RF_ResendData	
// input	:	none
// output	:	none
//*******************************//
void RF_ResendData(void)
{
	//SpiWriteStrobeCMD(REST_TX_POS_CMD);
	SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);
	RFXtalReady();
	SpiWriteStrobeCMD(TX_MODE_CMD);
}

//*******************************//
// name		:	RF_ReadRxData
// input	:	none
// output	:	none
//*******************************//
void RF_ReadRxData(void)
{
#if RF_Payload_Length > 64
	ReadFIFO(RF_RXFIFO,64);
#else
	ReadFIFO(RF_RXFIFO,RF_Payload_Length);
#endif
	SpiWriteStrobeCMD(REST_RX_POS_CMD);
}

//*******************************//
// name		:	RFReadRssi
// input	:	none
// output	:	RSSI_value
//*******************************//
unsigned char RFReadRssi(void)
{
	unsigned char rssi;
	rssi = SpiReadRegCMD(RSSI3_REGS);
	return rssi;
}

//*******************************//
// name		:	Extend_FIFO_Enable
// input	:	none
// output	:	none
//*******************************//
void Extend_FIFO_Enable(void)
{
	unsigned char a;
	a  = SpiReadRegCMD(FIFO2_REGS);
	a &= 0xF3;
	a |= 0x07;		// 32 bytes limit
	SpiWriteRegCMD(FIFO2_REGS,a);
	
	a  = SpiReadRegCMD(IRQ2_REGS);
	a |= 0x21;		// enable FIFO low IRQ
	SpiWriteRegCMD(IRQ2_REGS,a);
}

//*******************************//
// name		:	Extend_FIFO_Disable
// input	:	none
// output	:	none
//*******************************//
void Extend_FIFO_Disable(void)
{
	unsigned char a;
	
	a  = SpiReadRegCMD(FIFO2_REGS);
	a &= 0xF8;
	SpiWriteRegCMD(FIFO2_REGS,a);
	
	a  = SpiReadRegCMD(IRQ2_REGS);
	a &= 0xDF;		// disable FIFO low IRQ
	SpiWriteRegCMD(IRQ2_REGS,a);
}

//*******************************//
// name		:	RFEnARK
// input	:	none
// output	:	none
//*******************************//
void RFEnARK(void)
{
	unsigned char a;

	a  = SpiReadRegCMD(ATR7_REGS);
	a &= 0xFE;
	a |= 0x01;
	SpiWriteRegCMD(ATR7_REGS,a);
	
	a  = SpiReadRegCMD(IRQ2_REGS);	
	a &= 0x7F;	
	a |= 0x80;
	SpiWriteRegCMD(IRQ2_REGS,a);
	
}

//*******************************//
// name		:	RFDisARK
// input	:	none
// output	:	none
//*******************************//
void RFDisARK(void)
{
	unsigned char a;
	
	a  = SpiReadRegCMD(ATR7_REGS);
	a &= 0xFE;
	SpiWriteRegCMD(ATR7_REGS,a);
}

//*******************************//
// name		:	CRC16_CCITT_FALSE
// input	:	buf[],length
// output	:	none
//*******************************//
unsigned int CRC16_CCITT_FALSE(unsigned char *buf, int len)
{
    unsigned char  a,b,temp_data;
	unsigned int crc,polynomial;
	
	bit bit_cnt;
	bit C15;
	
	crc = 0xffff;
	polynomial = 0x1021;
	
	for(a=0;a<len;a++)
	{
		temp_data = *buf;
		for(b=0;b<8;b++)
		{
			bit_cnt = (temp_data>>(7-b));
			C15 = (crc>>15);
			crc <<= 1;
			if(bit_cnt^C15)
				crc ^= polynomial;
			
		}
		buf++;
	}
    return crc;
}
