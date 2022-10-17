/*------------------------------------------------------------------*/
/*							Header									*/
/*------------------------------------------------------------------*/
#include "BC3602_CMD_REG.h"
#include "Configuration.h"

/*------------------------------------------------------------------*/
/*						  Calculate									*/
/*------------------------------------------------------------------*/
#define		ATR_Cycle			((RF_ATR_Cycle*1024L)/125)
#define		ATR_Cycle_low		(ATR_Cycle&0xff)
#define		ATR_Cycle_high		((ATR_Cycle>>8)&0xff)
#define		ATR_RX_Window		(RF_ATR_RX_Window/250L)
#define		ATR_RX_Window_low	(ATR_RX_Window&0xff)
#define		ATR_RX_Window_high	((ATR_RX_Window>>8)&0x07)
#define		ATR_RX_extra		(RF_ATR_RX_extra*4L)
#define		ATR_RX_extra_low	(ATR_RX_extra&0xff)
#define		ATR_RX_extra_high	((ATR_RX_extra>>8)&0xff)
#define		ARK_RX_Window		(RF_ARK_RX_Window/250L)

/*------------------------------------------------------------------*/
/*							Define									*/
/*------------------------------------------------------------------*/
//	BC3601 IRQ/IO Configure	//
const unsigned short IRQIO_REGS_TALBE[]=
{
	((	IO2_REGS	<< 8)	+	0x00	),	//b[7:4]=GIO4S[3:0],b[3:0]=GIO3S[3:0]
#if (RF_SPI_line!=3)
	((	IO1_REGS	<< 8)	+	0x4D	),	//b[7:6]=PADDS[1:0],b[5:3]=GIO2S[2:0],b[2:0]=GIO1S[2:0]
	((	IO3_REGS	<< 8)	+	0xFF	),	//b7=SDO_TEN,b6=SPIPU,b5=reserved,b[4:1]=GIOPU[4:1],b0=reserved
//	((	IO3_REGS	<< 8)	+	0x88	),	//b7=SDO_TEN,b6=SPIPU,b5=reserved,b[4:1]=GIOPU[4:1],b0=reserved
#else
	((	IO1_REGS	<< 8)	+	0x45	),	//b[7:6]=PADDS[1:0],b[5:3]=GIO2S[2:0],b[2:0]=GIO1S[2:0]
#endif
	((	IRQ1_REGS	<< 8)	+	0x0E	),	//b7=RXTO, b6=RXFFOW, b4=RXCRCF, b[3:2]=RXDETS[1:0], b1=IRQCPOR, b0=IRQPOR
	((	IRQ2_REGS	<< 8)	+	0x13	),	//b7=ARKTFIE, b6=ARTCIE, b5=FIFOLTIE, b4=RXERRIE, b3=RXDETIE, b2=CALCMPIE, b1=RXCMPIE, b0=TXCMPIE
	((	IRQ3_REGS	<< 8)	+	0xFF	),	//b7=ARKTFIF, b6=ARTCIF, b5=FIFOLTIF, b4=RXERRIF, b3=RXDETIF, b2=CALCMPIF, b1=RXCMPIF, b0=TXCMPIF
};


//	BC3602 packet format Configure	//
/* CRC_EN=1, PLLEN_EN=0, PAYLOAD = 16-byte */
const unsigned short PACKET_REGS_TALBE[]=
{
	((	FIFO1_REGS	<< 8)	+	0x00	),	//b[5:0]=TXFFSA[5:0]
	((	FIFO2_REGS	<< 8)	+	0x01	),	//b4=RXPL2F_EN,b3=FFINF_EN,b2=FFMG_EN,b[1:0]=FFMG[1:0]
	((	PKT1_REGS	<< 8)	+	RF_TxPreamble_Length	),	//TXPMLEN[7:0] (+1)
	#if RF_SYNC_Length==4
	((	PKT2_REGS	<< 8)	+	(0x04|(RF_Trailer_EN<<5)|(RF_RxPreamble_Length))	),	//b[7:6]=PID[1:0],b[3:2]=SYNCLEN[1:0],b[1:0]=RXPMLEN[1:0]
	#endif
	#if RF_SYNC_Length==6
	((	PKT2_REGS	<< 8)	+	(0x08|(RF_Trailer_EN<<5)|(RF_RxPreamble_Length))	),	//b[7:6]=PID[1:0],b[3:2]=SYNCLEN[1:0],b[1:0]=RXPMLEN[1:0]
	#endif
	#if RF_SYNC_Length==8
	((	PKT2_REGS	<< 8)	+	(0x0C|(RF_Trailer_EN<<5)|(RF_RxPreamble_Length))	),	//b[7:6]=PID[1:0],b[3:2]=SYNCLEN[1:0],b[1:0]=RXPMLEN[1:0]
	#endif
	#if	RF_ARK_Enable == 1
	((	PKT3_REGS	<< 8)	+	(0x00|(RF_Manchester_EN<<7)|(RF_FEC_EN<<6)|(RF_ARK_Enable<<5)|(RF_CRC_Format<<4)|(RF_ARK_Enable<<3)|(RF_PLHAC_EN<<2)|(RF_Header_Length<<1)|RF_ARK_Enable)	),	//b7=MCH_EN, b6=FEC_EN, b5=CRC_EN, b4=CRCFMT, b3=PLLEN_EN, b2=PLHAC_EN, b1=PLHLEN, b0=PLH_EN
	#else
	((	PKT3_REGS	<< 8)	+	(0x00|(RF_Manchester_EN<<7)|(RF_FEC_EN<<6)|(RF_CRC_EN<<5)|(RF_CRC_Format<<4)|(RF_PLLEN_EN<<3)|(RF_PLHAC_EN<<2)|(RF_Header_Length<<1)|RF_Header_EN)	),	//b7=MCH_EN, b6=FEC_EN, b5=CRC_EN, b4=CRCFMT, b3=PLLEN_EN, b2=PLHAC_EN, b1=PLHLEN, b0=PLH_EN
	#endif
	((	PKT4_REGS	<< 8)	+	(0x00|(RF_whiteining_EN<<7)|RF_whiteining_Seed)	),	//b7=WHT_EN, b[6:0]=WHTSD[6:0]
	((	PKT5_REGS	<< 8)	+	RF_Payload_Length	),	//TXDLEN
	#if RF_ARK_Enable==1
	((	PKT6_REGS	<< 8)	+	0x00	),				//RXDLEN
	#else
	((	PKT6_REGS	<< 8)	+	RF_Payload_Length	),	//RXDLEN
	#endif
	((	PKT7_REGS	<< 8)	+	0x27	),	//b[7:6]=RXPID[1:0], b[5:3]=DLY_RXS[2:0], b[2:0]=DLY_TXS[2:0]
	((	PKT8_REGS	<< 8)	+	RF_PLHA_Address	),	//b[5:0]=PLHA[5:0]
	((	PKT9_REGS	<< 8)	+	0x00	),	//b[7:0]=PLHEA[7:0]
};

//	BC3602 common Configure	//
const unsigned short COMMON_REGS_TALBE[]=
{
	((	CFG1_REGS	<< 8)	+	0x40	),	//b[5:0]=TXFFSA[5:0]
};

//	BC3602 Bank0 Configure	//
const unsigned short BANK0_REGS_TALBE[]=
{
	((	ATR1_REGS	<< 8)	+	0x42	),	//_ATR1_		ATRCLK = 8192Hz / WOR
	((	ATR2_REGS	<< 8)	+	ATR_Cycle_low	),	//_ATR2_
	((	ATR3_REGS	<< 8)	+	ATR_Cycle_high	),	//_ATR3_	ATR Cycle = 0x8000 * 0.125 = 4s
	((	ATR4_REGS	<< 8)	+	ATR_RX_Window_low	),	//_ATR4_	RX cycle = (0x25+1) * 250us = 9.5ms
	((	ATR5_REGS	<< 8)	+	ATR_RX_extra_low	),	//_ATR5_	
	((	ATR6_REGS	<< 8)	+	ATR_RX_extra_high	),	//_ATR6_	RX extra time = (0x3E7+1) * 250us = 250ms
	((	ATR7_REGS	<< 8)	+	(((0x02|((RF_ARK_Resen_Count)<<4)))|RF_ARK_Enable	)),	//_ATR7_
	((	ATR8_REGS	<< 8)	+	(ARK_RX_Window&0xFF)		),	//_ATR8_	RX cycle = (ARK_RX_Window+1) * 250us = ms
	((	ATR11_REGS	<< 8)	+	ATR_RX_Window_high		),
	((	XO1_REGS	<< 8)	+	0x40	),	//
	((	XO2_REGS	<< 8)	+	0x03	),	//
};

//	BC3602 Bank1 Configure	//
const unsigned short BANK1_REGS_TALBE[]=
{
	((	AGC2_REGS	<< 8)	+	0x40	),	//
	((	AGC3_REGS	<< 8)	+	0x24	),	//
	((	AGC5_REGS	<< 8)	+	0x00	),	//
	((	AGC7_REGS	<< 8)	+	0x30	),	//
};

//	BC3602 Bank2 Configure	//
const unsigned short BANK2_REGS_TALBE[]=
{
	((	CP3_REGS	<< 8)	+	0xA3	),	//	CP3
	((	ODC1_REGS	<< 8)	+	0x80	),	//  OD1		
	((	VC3_REGS	<< 8)	+	0x16	),	//	
	((	RX3_REGS	<< 8)	+	0x00	),	//
	((	TX1_REGS	<< 8)	+	0x64	),	//		
	((	LDC2_REGS	<< 8)	+	0x94	),	//
	
#if	RF_BAND==_315MHz_	
	((	CA1_REGS	<< 8)	+	0xBC	),	//	
	((	RX1_REGS	<< 8)	+	0x64	),	//	
#endif	
#if	RF_BAND==_433MHz_	
	((	CA1_REGS	<< 8)	+	0xBC	),	//
	((	RX1_REGS	<< 8)	+	0x64	),	//	
#endif	
#if	RF_BAND==_470MHz_	
	((	CA1_REGS	<< 8)	+	0xBC	),	//	
	((	RX1_REGS	<< 8)	+	0x64	),	//		
#endif	
#if	RF_BAND==_868MHz_	
	((	CA1_REGS	<< 8)	+	0x9C	),	//	
	((	RX1_REGS	<< 8)	+	0x74	),	//	
#endif	
#if	RF_BAND==_915MHz_	
	((	CA1_REGS	<< 8)	+	0x9C	),	//
	((	RX1_REGS	<< 8)	+	0x74	),	//	
#endif	

};

//	BC3602 SYNCWORD	Configure	//
const unsigned char BC3602_SYNCWORD[RF_SYNC_Length]=
{
	#if (RF_SYNC_Length==4)
		(unsigned char)RF_SYNC_L,
		(unsigned char)(RF_SYNC_L>>8),
		(unsigned char)(RF_SYNC_L>>16),
		(unsigned char)(RF_SYNC_L>>24),
	#endif
	#if (RF_SYNC_Length==6)
		(unsigned char)RF_SYNC_L,
		(unsigned char)(RF_SYNC_L>>8),
		(unsigned char)(RF_SYNC_L>>16),
		(unsigned char)(RF_SYNC_L>>24),
		
		(unsigned char)RF_SYNC_H,
		(unsigned char)(RF_SYNC_L>>8),
	#endif
	#if (RF_SYNC_Length==8)
		(unsigned char)RF_SYNC_L,
		(unsigned char)(RF_SYNC_L>>8),
		(unsigned char)(RF_SYNC_L>>16),
		(unsigned char)(RF_SYNC_L>>24),
		(unsigned char)RF_SYNC_H,
		(unsigned char)(RF_SYNC_H>>8),
		(unsigned char)(RF_SYNC_H>>16),
		(unsigned char)(RF_SYNC_H>>24)		
	#endif
};

//	BC3602 TX Power value	//
const unsigned char TxPowerValue[5][5]=
{
	//0dBm, 5dBm,10dBm,13dBm
	{ 0x06, 0x08, 0x0B, 0x0F },	//315MHz
	{ 0x03, 0x05, 0x09, 0x0D },	//433MHz
	{ 0x03, 0x05, 0x09, 0x0D },	//470MHz
	{ 0x03, 0x06, 0x0A, 0x0E },	//868MHz
	{ 0x06, 0x08, 0x0B, 0x0F }	//915MHz
};

//	BC3602 Frequency	Configure//
#define		_RF_XTAL_		16000			//kHz
#define		_RF_XTAL_D_		(_RF_XTAL_/128)	//kHz
/**********************************************/
#if	RF_BAND==_315MHz_
/**********************************************/
#define		BAND_SEL		0x00
#define		_RF_FREQ_		230000
#define		_ODDIV_			1
#endif
/**********************************************/
#if	RF_BAND==_433MHz_
/**********************************************/
#define		BAND_SEL		0x20
#define		_RF_FREQ_		433920
#define		_ODDIV_			1
#endif
/**********************************************/
#if	RF_BAND==_470MHz_
/**********************************************/
#define		BAND_SEL		0x40
#define		_RF_FREQ_		470000
#define		_ODDIV_			1
#endif
/**********************************************/
#if	RF_BAND==_868MHz_
/**********************************************/
#define		BAND_SEL		0x60
#define		_RF_FREQ_		868300
#define		_ODDIV_			1
#endif
/**********************************************/
#if	RF_BAND==_915MHz_
/**********************************************/
#define		BAND_SEL		0x60
#define		_RF_FREQ_		915000
#define		_ODDIV_			1
#endif

#define		_D_N_		((_RF_FREQ_*_ODDIV_)/_RF_XTAL_)
#define		_D_K_		(((_RF_FREQ_*_ODDIV_)-(_D_N_*_RF_XTAL_))*8192/_RF_XTAL_D_)
#define		_D_K_L_		(_D_K_ & 0xff)
#define		_D_K_M_		((_D_K_>>8) & 0xff)
#define		_D_K_H_		((_D_K_>>16) & 0xff)

const unsigned short Frequency_REGS_TALBE[]=
{
	((	SX1_REGS	<< 8)	+	_D_N_		),	//
	((	SX2_REGS	<< 8)	+	_D_K_L_		),	//
	((	SX3_REGS	<< 8)	+	_D_K_M_		),	//
	((	SX4_REGS	<< 8)	+	_D_K_H_		),	//
};


//	BC3602 RF_Datarate	Configure//
/**********************************************/
#if RF_Datarate==_2kbps_
/**********************************************/
const unsigned short DM_REGS_TALBE[]=
{
	((	MOD1_REGS	<< 8)	+	249		),	//
	((	MOD2_REGS	<< 8)	+	0x60	),	//
	((	MOD3_REGS	<< 8)	+	0x66	),	//
	((	DM1_REGS	<< 8)	+	0x31	),	//
	((	DM2_REGS	<< 8)	+	0x49	),	//
	((	DM3_REGS	<< 8)	+	0xE6	),	//
	((	DM4_REGS	<< 8)	+	0x08	),	//
	((	DM5_REGS	<< 8)	+	0x1A	),	//
	((	DM6_REGS	<< 8)	+	0x00	),	//
	((	DM7_REGS	<< 8)	+	0x00	),	//
	((	DM8_REGS	<< 8)	+	0x05	),	//
};
const unsigned short FCF_REGS_TALBE[]=
{
	((	FCF1_REGS	<< 8)	+	0x36	),	//
	((	FCF2_REGS	<< 8)	+	0x21	),	//
	((	FCF3_REGS	<< 8)	+	0x00	),	//
	((	FCF4_REGS	<< 8)	+	0x00	),	//
	((	FCF5_REGS	<< 8)	+	0x00	),	//
	((	FCF6_REGS	<< 8)	+	0x00	),	//
	((	FCF7_REGS	<< 8)	+	0x00	),	//
	((	FCF8_REGS	<< 8)	+	0x02	),	//
	((	FCF9_REGS	<< 8)	+	0x03	),	//
	((	FCF10_REGS	<< 8)	+	0x00	),	//
	((	FCF11_REGS	<< 8)	+	0x00	),	//
	((	FCF12_REGS	<< 8)	+	0x00	),	//
	((	FCF13_REGS	<< 8)	+	0x00	),	//
	((	FCF14_REGS	<< 8)	+	0x00	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0x00	),	//
	((	FCF17_REGS	<< 8)	+	0x00	),	//
	((	FCF18_REGS	<< 8)	+	0x00	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TALBE[]=
{
	((	RX2_REGS	<< 8)	+	0x44	),	//
	((	CP1_REGS	<< 8)	+	0x03	),	//
	((	CP2_REGS	<< 8)	+	0x88	),	//	
};
#endif
/**********************************************/
#if RF_Datarate==_10kbps_
/**********************************************/
const unsigned short DM_REGS_TALBE[]=
{
	((	MOD1_REGS	<< 8)	+	49		),	//
	((	MOD2_REGS	<< 8)	+	0x60	),	//
	((	MOD3_REGS	<< 8)	+	0x66	),	//
	((	DM1_REGS	<< 8)	+	0x09	),	//
	((	DM2_REGS	<< 8)	+	0x49	),	//
	((	DM3_REGS	<< 8)	+	0xE6	),	//
	((	DM4_REGS	<< 8)	+	0x08	),	//
	((	DM5_REGS	<< 8)	+	0x1A	),	//
	((	DM6_REGS	<< 8)	+	0x00	),	//
	((	DM7_REGS	<< 8)	+	0x00	),	//
	((	DM8_REGS	<< 8)	+	0x1A	),	//
};
const unsigned short FCF_REGS_TALBE[]=
{
	((	FCF1_REGS	<< 8)	+	0x16	),	//
	((	FCF2_REGS	<< 8)	+	0xA4	),	//
	((	FCF3_REGS	<< 8)	+	0x00	),	//
	((	FCF4_REGS	<< 8)	+	0x00	),	//
	((	FCF5_REGS	<< 8)	+	0x00	),	//
	((	FCF6_REGS	<< 8)	+	0x00	),	//
	((	FCF7_REGS	<< 8)	+	0x00	),	//
	((	FCF8_REGS	<< 8)	+	0x10	),	//
	((	FCF9_REGS	<< 8)	+	0x03	),	//
	((	FCF10_REGS	<< 8)	+	0x00	),	//
	((	FCF11_REGS	<< 8)	+	0x00	),	//
	((	FCF12_REGS	<< 8)	+	0x00	),	//
	((	FCF13_REGS	<< 8)	+	0x00	),	//
	((	FCF14_REGS	<< 8)	+	0x00	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0x00	),	//
	((	FCF17_REGS	<< 8)	+	0x00	),	//
	((	FCF18_REGS	<< 8)	+	0x00	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TALBE[]=
{
	((	RX2_REGS	<< 8)	+	0x44	),	//
	((	CP1_REGS	<< 8)	+	0x03	),	//
	((	CP2_REGS	<< 8)	+	0x88	),	//	
};
#endif
/**********************************************/
#if RF_Datarate==_50kbps_
/**********************************************/
const unsigned short DM_REGS_TALBE[]=
{
	((	MOD1_REGS	<< 8)	+	9		),	//
	((	MOD2_REGS	<< 8)	+	0x60	),	//
	((	MOD3_REGS	<< 8)	+	0x66	),	//
	((	DM1_REGS	<< 8)	+	0x13	),	//
	((	DM2_REGS	<< 8)	+	0x40	),	//
	((	DM3_REGS	<< 8)	+	0xE0	),	//
	((	DM4_REGS	<< 8)	+	0x08	),	//
	((	DM5_REGS	<< 8)	+	0x30	),	//
	((	DM6_REGS	<< 8)	+	0x00	),	//
	((	DM7_REGS	<< 8)	+	0x00	),	//
	((	DM8_REGS	<< 8)	+	0x0D	),	//
};
const unsigned short FCF_REGS_TALBE[]=
{
	((	FCF1_REGS	<< 8)	+	0x06	),	//
	((	FCF2_REGS	<< 8)	+	0x4C	),	//
	((	FCF3_REGS	<< 8)	+	0x00	),	//
	((	FCF4_REGS	<< 8)	+	0x00	),	//
	((	FCF5_REGS	<< 8)	+	0x00	),	//
	((	FCF6_REGS	<< 8)	+	0x00	),	//
	((	FCF7_REGS	<< 8)	+	0x00	),	//
	((	FCF8_REGS	<< 8)	+	0x00	),	//
	((	FCF9_REGS	<< 8)	+	0x00	),	//
	((	FCF10_REGS	<< 8)	+	0x00	),	//
	((	FCF11_REGS	<< 8)	+	0x00	),	//
	((	FCF12_REGS	<< 8)	+	0x00	),	//
	((	FCF13_REGS	<< 8)	+	0x00	),	//
	((	FCF14_REGS	<< 8)	+	0x00	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0x00	),	//
	((	FCF17_REGS	<< 8)	+	0x00	),	//
	((	FCF18_REGS	<< 8)	+	0x00	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TALBE[]=
{
	((	RX2_REGS	<< 8)	+	0x44	),	//
	((	CP1_REGS	<< 8)	+	0x03	),	//
	((	CP2_REGS	<< 8)	+	0x88	),	//	
};
#endif
/**********************************************/
#if RF_Datarate==_125kbps_
/**********************************************/
const unsigned short DM_REGS_TALBE[]=
{
	((	MOD1_REGS	<< 8)	+	3		),	//
	((	MOD2_REGS	<< 8)	+	0x90	),	//
	((	MOD3_REGS	<< 8)	+	0x9A	),	//
	((	DM1_REGS	<< 8)	+	0x07	),	//
	((	DM2_REGS	<< 8)	+	0x40	),	//
	((	DM3_REGS	<< 8)	+	0xE0	),	//
	((	DM4_REGS	<< 8)	+	0x08	),	//
	((	DM5_REGS	<< 8)	+	0x30	),	//
	((	DM6_REGS	<< 8)	+	0x00	),	//
	((	DM7_REGS	<< 8)	+	0x00	),	//
	((	DM8_REGS	<< 8)	+	0x20	),	//
};
const unsigned short FCF_REGS_TALBE[]=
{
	((	FCF1_REGS	<< 8)	+	0x06	),	//
	((	FCF2_REGS	<< 8)	+	0x19	),	//
	((	FCF3_REGS	<< 8)	+	0x01	),	//
	((	FCF4_REGS	<< 8)	+	0x1D	),	//
	((	FCF5_REGS	<< 8)	+	0x00	),	//
	((	FCF6_REGS	<< 8)	+	0x46	),	//
	((	FCF7_REGS	<< 8)	+	0x03	),	//
	((	FCF8_REGS	<< 8)	+	0x22	),	//
	((	FCF9_REGS	<< 8)	+	0x00	),	//
	((	FCF10_REGS	<< 8)	+	0x31	),	//
	((	FCF11_REGS	<< 8)	+	0x03	),	//
	((	FCF12_REGS	<< 8)	+	0x86	),	//
	((	FCF13_REGS	<< 8)	+	0x03	),	//
	((	FCF14_REGS	<< 8)	+	0x12	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0x08	),	//
	((	FCF17_REGS	<< 8)	+	0x00	),	//
	((	FCF18_REGS	<< 8)	+	0x08	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TALBE[]=
{
	((	RX2_REGS	<< 8)	+	0x54	),	//
	((	CP1_REGS	<< 8)	+	0x03	),	//
	((	CP2_REGS	<< 8)	+	0x88	),	//	
};
#endif
/**********************************************/
#if RF_Datarate==_250kbps_
/**********************************************/
const unsigned short DM_REGS_TALBE[]=
{
	((	MOD1_REGS	<< 8)	+	1		),	//
	((	MOD2_REGS	<< 8)	+	0x90	),	//
	((	MOD3_REGS	<< 8)	+	0x9A	),	//
	((	DM1_REGS	<< 8)	+	0x03	),	//
	((	DM2_REGS	<< 8)	+	0x40	),	//
	((	DM3_REGS	<< 8)	+	0xE0	),	//
	((	DM4_REGS	<< 8)	+	0x08	),	//
	((	DM5_REGS	<< 8)	+	0x30	),	//
	((	DM6_REGS	<< 8)	+	0x00	),	//
	((	DM7_REGS	<< 8)	+	0x00	),	//
	((	DM8_REGS	<< 8)	+	0x40	),	//
};
const unsigned short FCF_REGS_TALBE[]=
{
	((	FCF1_REGS	<< 8)	+	0x06	),	//
	((	FCF2_REGS	<< 8)	+	0x94	),	//
	((	FCF3_REGS	<< 8)	+	0x02	),	//
	((	FCF4_REGS	<< 8)	+	0xCA	),	//
	((	FCF5_REGS	<< 8)	+	0x02	),	//
	((	FCF6_REGS	<< 8)	+	0x62	),	//
	((	FCF7_REGS	<< 8)	+	0x00	),	//
	((	FCF8_REGS	<< 8)	+	0x58	),	//
	((	FCF9_REGS	<< 8)	+	0x03	),	//
	((	FCF10_REGS	<< 8)	+	0xE9	),	//
	((	FCF11_REGS	<< 8)	+	0x03	),	//
	((	FCF12_REGS	<< 8)	+	0xB3	),	//
	((	FCF13_REGS	<< 8)	+	0x03	),	//
	((	FCF14_REGS	<< 8)	+	0x3E	),	//
	((	FCF15_REGS	<< 8)	+	0x00	),	//
	((	FCF16_REGS	<< 8)	+	0xE9	),	//
	((	FCF17_REGS	<< 8)	+	0x03	),	//
	((	FCF18_REGS	<< 8)	+	0x39	),	//
	((	FCF19_REGS	<< 8)	+	0x00	),	//
};
const unsigned short CBPF_REGS_TALBE[]=
{
	((	RX2_REGS	<< 8)	+	0x54	),	//
	((	CP1_REGS	<< 8)	+	0x03	),	//
	((	CP2_REGS	<< 8)	+	0x88	),	//	
};
#endif