/*------------------------------------------------------------------*/
/*						BC3602 strobe command 						*/
/*------------------------------------------------------------------*/
#define WRITE_REGS_CMD			0x40 //Register write command
#define READ_REGS_CMD			0xc0 //Register read command
#define REGS_BANK_CMD      		0x20 //Set Register Bank command
#define WRITE_SYNCWORD_CMD  	0x10 //Write Sync-Word command
#define READ_SYNCWORD_CMD   	0x90 //Read Sync-Word command
#define WRITE_FIFO_CMD      	0x11 //Write TX FIFO command
#define READ_FIFO_CMD       	0x91 //Read RX FIFO command
#define SOFT_RESET_CMD      	0x08 //Software reset command
#define	READ_CHIPID_CMD			0x9F //Read Sync-Word command
#define REST_TX_POS_CMD     	0x09 //TX FIFO address pointer reset command
#define REST_RX_POS_CMD     	0x89 //RX FIFO address pointer reset command
#define DEEP_SLEEP_CMD      	0x0A //Enter Deep Sleep mode
#define IDLE_MODE_CMD       	0x0B //Enter Idle mode
#define LIGHT_SLEEP_CMD     	0x0C //Enter Light Sleep mode
#define STANDBY_MODE_CMD    	0x0D //Enter Standby mode
#define TX_MODE_CMD         	0x0E //Enter TX mode
#define RX_MODE_CMD         	0x8E //Enter RX mode
#define REGSADDR_MASK       	0x3F
	
	
//-----------------  Interrupt Control Register 2/3 -----------------
#define	_TXFSHI_					0x01	// TX Finish IRQ 
#define	_RXFSHI_         			0x02  // RX Finish IRQ with no error 
#define	_CALFSHI_      				0x04  // Calibration Finish IRQ. 
#define	_RXDETI_         			0x08  // RX Carry/Preamble/Syncword detected. 
#define	_RXFAILI_           		0x10  // RX failure,(CRC_EN and CRC failure) or (RX FIFO overwrite) 
#define	_TRXFFMGI_          		0x20  // TX/RX FIFO length less/greater than threshold 
#define	_WOTWKPI_        			0x40  
#define	_ARKFAILI_          		0x80  // ARK TX Failure 


/*------------------------------------------------------------------*/
/*						BC3602 Register Bank 						*/
/*------------------------------------------------------------------*/
#define BC3602_BANK0       		0x00
#define BC3602_BANK1      		0x01
#define BC3602_BANK2      		0x02

/*------------------------------------------------------------------*/
/*						BC3602 RF Band_SEL	 						*/
/*------------------------------------------------------------------*/
#define BAND_315MHz      		0x00
#define BAND_433MHz      		0x01
#define BAND_470MHz      		0x02
#define BAND_868MHz      		0x03
#define BAND_915MHz      		0x03

#define _315MHz_     			0x00
#define _433MHz_      			0x01
#define _470MHz_      			0x02
#define _868MHz_      			0x03
#define _915MHz_      			0x04

/*------------------------------------------------------------------*/
/*					  	  BC3602 RF MODE	 						*/
/*------------------------------------------------------------------*/
#define	RF_DeepSleep			0x00
#define	RF_Idle					0x01
#define	RF_LightSleep			0x02
#define	RF_Standby				0x03
#define	RF_TX					0x04
#define	RF_RX					0x05
#define	RF_VCO					0x06

/*------------------------------------------------------------------*/
/*					 BC3602 RF Power & DR SEL	 					*/
/*------------------------------------------------------------------*/
#define _0dBm_      			0x00
#define _5dBm_      			0x01
#define _10dBm_      			0x02
#define _13dBm_      			0x03

#define _2kbps_ 				0x00
#define _10kbps_ 				0x01
#define _50kbps_      			0x02
#define _125kbps_      			0x03
#define _250kbps_      			0x04

/*------------------------------------------------------------------*/
/*						 BC3602 Register	 						*/
/*------------------------------------------------------------------*/
//Common register
#define  CFG1_REGS            	0x00
#define  RC1_REGS             	0x01
#define  IRQ1_REGS            	0x02
#define  IRQ2_REGS            	0x03
#define  IRQ3_REGS            	0x04
#define  IRQ4_REGS            	0x05
#define  IO1_REGS             	0x06
#define  IO2_REGS             	0x07
#define  IO3_REGS             	0x08
#define  FIFO1_REGS           	0x09
#define  FIFO2_REGS           	0x0A
#define  PKT1_REGS            	0x0B
#define  PKT2_REGS            	0x0C
#define  PKT3_REGS            	0x0D
#define  PKT4_REGS            	0x0E
#define  PKT5_REGS            	0x0F
#define  PKT6_REGS            	0x10
#define  PKT7_REGS            	0x11
#define  PKT8_REGS            	0x12
#define  PKT9_REGS            	0x13
#define  MOD1_REGS            	0x14
#define  MOD2_REGS            	0x15
#define  MOD3_REGS            	0x16
#define  DM1_REGS             	0x17
#define  DM2_REGS             	0x18
#define  DM3_REGS             	0x19
#define  DM4_REGS             	0x1A
#define  DM5_REGS             	0x1B
#define  DM6_REGS             	0x1C
#define  DM7_REGS             	0x1D
#define  DM8_REGS             	0x1E
//BANK0 register
#define  OM_REGS              	0x20
#define  SX1_REGS             	0x22
#define  SX2_REGS             	0x23
#define  SX3_REGS             	0x24
#define  SX4_REGS             	0x25
#define  STA1_REGS            	0x26
#define  RSSI2_REGS           	0x28
#define  RSSI3_REGS           	0x29
#define  RSSI4_REGS           	0x2A
#define  ATR1_REGS           	0x2B
#define  ATR2_REGS           	0x2C
#define  ATR3_REGS           	0x2D
#define  ATR4_REGS           	0x2E
#define  ATR5_REGS           	0x2F
#define  ATR6_REGS           	0x30
#define  ATR7_REGS           	0x31
#define  ATR8_REGS           	0x32
#define  ATR9_REGS           	0x33
#define  ATR10_REGS           	0x34
#define  ATR11_REGS           	0x35
#define  XO1_REGS           	0x3C
#define  XO2_REGS           	0x3D
#define  XO3_REGS           	0x3E
#define  TX2_REGS           	0x3F
//BANK1 register
#define  AGC2_REGS           	0x21
#define  AGC3_REGS           	0x22
#define  AGC4_REGS           	0x23
#define  AGC5_REGS           	0x24
#define  AGC6_REGS           	0x25
#define  AGC7_REGS           	0x26
#define  FCF1_REGS           	0x2C
#define  FCF2_REGS           	0x2D
#define  FCF3_REGS           	0x2E
#define  FCF4_REGS           	0x2F
#define  FCF5_REGS           	0x30
#define  FCF6_REGS           	0x31
#define  FCF7_REGS           	0x32
#define  FCF8_REGS           	0x33
#define  FCF9_REGS           	0x34
#define  FCF10_REGS           	0x35
#define  FCF11_REGS           	0x36
#define  FCF12_REGS           	0x37
#define  FCF13_REGS           	0x38
#define  FCF14_REGS           	0x39
#define  FCF15_REGS           	0x3A
#define  FCF16_REGS           	0x3B
#define  FCF17_REGS           	0x3C
#define  FCF18_REGS           	0x3D
#define  FCF19_REGS           	0x3E
//BANK2 register
#define	 RXG_REGS				0x21
#define	 CP1_REGS				0x26
#define	 CP2_REGS				0x27
#define	 CP3_REGS				0x28
#define	 ODC1_REGS				0x29
#define	 ODC2_REGS				0x2A
#define	 VC3_REGS				0x2D
#define	 RX1_REGS				0x2E
#define	 RX2_REGS				0x2F
#define	 RX3_REGS				0x30
#define	 TX1_REGS				0x31
#define	 TX3_REGS				0x33
#define	 CA1_REGS				0x34
#define	 MMD1_REGS				0x35
#define	 LDC1_REGS				0x39
#define	 LDC2_REGS				0x3A
#define	 LDC3_REGS				0x3B

