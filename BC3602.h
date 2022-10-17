/*------------------------------------------------------------------*/
/*					      Statement									*/
/*------------------------------------------------------------------*/
void SpiWriteRegCMD(unsigned char REG,unsigned char DATA);
void SpiWriteStrobeCMD(unsigned char CMD);
unsigned char SpiReadRegCMD(unsigned char REG);
void RFWriteBuf(unsigned char CMD,unsigned char length,unsigned char data[]);
void RFWriteBuf2(unsigned char CMD,unsigned char length,const unsigned char *data);
void RFReadBuf(unsigned char CMD,unsigned char length,unsigned char data[]);
void RFReadBuf2(unsigned char CMD,unsigned char length,unsigned char *data);

void RFEnATR(void);
void RFDisATR(void);
void RFXtalReady(void);
void ResetLvRF(void);
void BC3602_Config(void);
void RFWriteFreqTABLE(void);
void RFWriteFreq(unsigned char *buffer);
void RFWriteSyncword(void);
void RFReadSyncword(void);
void RFReadSyncword(void);
void RFSetDRPram(void);
void RFSetPower(unsigned char band_sel,unsigned char power);
void RFCalibration(void);
unsigned char RFGetClrRFIrq(void);
void LircCalibration(void);
void RF_Init(void);
void RF_TxCarrier(void);
void RF_RxBER(void);
void WriteFIFO(unsigned char *rf_fifo,unsigned char payload_length);
void ReadFIFO(unsigned char *rf_fifo,unsigned char length);
void RF_WriteFreq(unsigned char band_sel,unsigned char *buffer);
void RF_SetSniffPrm(unsigned char *buffer);
void RF_SetAddr(unsigned char *buffer);
void RF_EntryRxMode(void);
void RF_EntrySniffMode(void);
void RF_EntryWOT_Mode(void);
void RF_SendData(void);
void RF_ResendData(void);
void RF_ReadRxData(void);
unsigned char RFReadRssi(void);
void Extend_FIFO_Enable(void);
void Extend_FIFO_Disable(void);
void RFEnARK(void);
void RFDisARK(void);
unsigned int CRC16_CCITT_FALSE(unsigned char *buf, int len);


/*------------------------------------------------------------------*/
/*						Extern Define								*/
/*------------------------------------------------------------------*/
extern void BCH32_Syncword(void);
extern unsigned char mLap[];

extern void SpiIOInti(void);
extern void SpiCSN_low(void);
extern void SpiCSN_high(void);
extern void SpiWrite(unsigned char SIMD);
extern unsigned char SpiRead(void);         