#include "MCU.h"
#include "spi.h"
#include "Configuration.h"
#include "BC3602.h"
#include "BC3602_CMD_REG.h"

/*------------------------------------------------------------------*/
/*							Variable								*/
/*------------------------------------------------------------------*/
extern unsigned char RF_TXFIFO[];		//FIFO length defined in Configuration.h
extern unsigned char RF_RXFIFO[];		//FIFO length defined in Configuration.h
extern unsigned char RF_IRQ;
extern unsigned char mRFSync[];			//SYNC length defined in Configuration.h

/*#if RF_Payload_Length>64				//Extend Mode*/
extern unsigned char ExtTxLength;
extern unsigned char ExtRxLength;
/*#endif*/
/*------------------------------------------------------------------*/
/*							Define									*/
/*------------------------------------------------------------------*/
#define	RF_DeepSleep			0x00
#define	RF_Idle					0x01
#define	RF_LightSleep			0x02
#define	RF_Standby				0x03
#define	RF_TX					0x04
#define	RF_RX					0x05
#define	RF_VCO					0x06


/*------------------------------------------------------------------*/
/*							Type Define								*/
/*------------------------------------------------------------------*/
typedef struct 
{
//	unsigned char	rf_band;
//	unsigned long	freq;
	unsigned char	mode;
	unsigned char	step;
	unsigned char	pairing;
	unsigned char	*sync_word;
	unsigned char	sync_word_len;
	unsigned char	irq_status;

	unsigned char	data_rate;
		
	unsigned char	tx_power;
	unsigned char	tx_preamble_len;
	unsigned int	tx_packet_len;
	unsigned char	*tx_payload_buffer;
	
	unsigned char	tx_irq_f;
	unsigned char	tx_fail_f;
	
//	unsigned char	en_wor;
	unsigned char	rx_preamble_len;
	unsigned int	rx_packet_len;
	unsigned int	rec_data_len;
	unsigned char	*rx_payload_buffer;
	unsigned char	rx_irq_f;
	unsigned char	rssi;
//	unsigned char	env_rssi;
} _BC3602_device_;


/*------------------------------------------------------------------*/
/*					Application Programming Interface				*/
/*------------------------------------------------------------------*/
extern void SpiWriteStrobeCMD(unsigned char CMD);
extern void SpiWriteRegCMD(unsigned char REG,unsigned char DATA);
extern unsigned char SpiReadRegCMD(unsigned char REG);

extern void RFWriteBuf(unsigned char CMD,unsigned char length,unsigned char data[]);
extern void RFWriteBuf2(unsigned char CMD,unsigned char length,const unsigned char *data);
extern void RFReadBuf(unsigned char CMD,unsigned char length,unsigned char data[]);
extern void RFReadBuf2(unsigned char CMD,unsigned char length,unsigned char *data);

//extern void ResetLvRF(void);
//extern void BC3602_Config(void);
//extern void RFCalibration(void);
//extern void LircCalibration(void);
//extern void RFWriteFreqTABLE(void);
//extern void RFWriteSyncword(void);
//extern void RFReadSyncword(void);
//extern void RFReadSyncword(void);
//extern void RFSetDRPram(void);

extern void RFWriteFreq(unsigned char *buffer);
extern void RFSetPower(unsigned char band_sel,unsigned char power);
extern void RF_WriteFreq(unsigned char band_sel,unsigned char *buffer);
extern void RF_SetSniffPrm(unsigned char *buffer);
extern void RF_SetAddr(unsigned char *buffer);

extern void WriteFIFO(unsigned char *rf_fifo,unsigned char payload_length);
extern void ReadFIFO(unsigned char *rf_fifo,unsigned char length);
extern unsigned char RFGetClrRFIrq(void);
extern void ResetTXFIFO(void);
extern void ResetRXFIFO(void);
extern void EntryLightSleep(void);
extern void EntryDeepSleep(void);
extern void EntryIdleMode(void);
extern void RFXtalReady(void);

extern void RF_Init(void);
extern void RF_TxCarrier(void);
extern void RF_RxBER(void);
extern void RF_EntryRxMode(void);
extern void RF_EntrySniffMode(void);
extern void RF_EntryWOT_Mode(void);
extern void RF_SendData(void);
extern void RF_ResendData(void);
extern void RF_ReadRxData(void);
extern unsigned char RFReadRssi(void);
extern void RFEnATR(void);
extern void RFDisATR(void);
extern void Extend_FIFO_Enable(void);
extern void Extend_FIFO_Disable(void);
extern void RFEnARK(void);
extern void RFDisARK(void);

extern void delay(unsigned short time);

extern void DATA_Process(unsigned char *FIFO);

void RF_Parameter_loading(void);
void SimpleFIFO_TX_Process(_BC3602_device_ *BC3602_T);
void SimpleFIFO_RX_Process(_BC3602_device_ *BC3602_T);
void ExtendFIFO_TX_Process(_BC3602_device_ *BC3602_T);
void ExtendFIFO_RX_Process(_BC3602_device_ *BC3602_T);
void ATR_WOT_Process(_BC3602_device_ *BC3602_T);
void ATR_WOR_Process(_BC3602_device_ *BC3602_T);
void SimpleFIFO_TX_Resent(_BC3602_device_ *BC3602_T,unsigned int Resent_Times);


/*------------------------------------------------------------------*/
/*							General Function						*/
/*------------------------------------------------------------------*/
#define  BC3602_StrobeCommand(cmd) 			SpiWriteStrobeCMD(cmd)
#define  BC3602_WriteRegister(reg,wd) 		SpiWriteRegCMD(reg,wd)
#define  BC3602_ReadRegister(reg) 			SpiReadRegCMD(reg)
#define  BC3602_RegisterBank(BC3602_BANKn)	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANKn);
#define  BC3602_SetTxPayloadWidth(wd)      	SpiWriteRegCMD(PKT5_REGS,wd)
#define  BC3602_SetRxPayloadWidth(wd)      	SpiWriteRegCMD(PKT6_REGS,wd)
#define  BC3602_GetRxPayloadWidth()        	SpiReadRegCMD(PKT6_REGS)
#define  BC3602_SetTxPreambleWidth(wd)     	SpiWriteRegCMD(PKT1_REGS,wd)
#define  BC3602_SetTxPayloadSAddr(ad)      	SpiWriteRegCMD(FIFO1_REGS,ad)
#define  BC3602_GetIRQState()               SpiReadRegCMD(IRQ3_REGS)
#define  BC3602_ClearIRQFlag(sts)           SpiWriteRegCMD(IRQ3_REGS,sts)  
#define  BC3602_GetOperatState()            SpiReadRegCMD(STA1_REGS)
#define  BC3602_ReadRxPayload(pbuf,len)  	RFReadBuf2(READ_FIFO_CMD,len,pbuf)
#define  BC3602_WriteTxPayload(pbuf,len)	RFWriteBuf2(WRITE_FIFO_CMD,len,pbuf)
#define  BC3602_ReadSyncWord(pbuf,len)   	RFReadBuf2(READ_SYNCWORD_CMD,len,pbuf)
#define  BC3602_ReadChipID(pbuf)   			RFReadBuf2(READ_CHIPID_CMD,4,pbuf)
#define  BC3602_WriteSyncWord(pbuf,len)  	RFWriteBuf2(WRITE_SYNCWORD_CMD,len,pbuf)
