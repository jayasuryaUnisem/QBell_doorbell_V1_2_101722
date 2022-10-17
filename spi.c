/*------------------------------------------------------------------*/
/*							Header									*/
/*------------------------------------------------------------------*/
#include "Configuration.h"

/*------------------------------------------------------------------*/
/*							Define									*/
/*------------------------------------------------------------------*/


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*******************************//
// name		:	SpiIOInti
// input	:	none
// output	:	none
// stack	:	1
//*******************************//
void SpiIOInti(void)
{
#if (RF_SPI_line==3)
	RF_SDIO_CTR = 1;	
	RF_SDIO_PU= 1;				// SDIO input & pull up
	RF_SCK = 0;
	RF_SCK_CTR = 0;				// SCK output & low
	RF_CSN = 1;
	RF_CSN_CTR = 0;				// SCN output & high
	
#else
	RF_PxSx_1 = 0x80;			// PC3 selected to SDO function
	RF_PxSx_2 = 0x2A;			// PC4 selected to SDI function
								// PC5 selected to SCK function
								// PC6 selected to SCS function
	RF_IFS 	  = 0x00; 							
	RF_MOSI_CTR = 0;			// MOSI set output
	RF_MISO_CTR = 1;			// MISO set input		
	RF_MISO_PU = 1;				// MISO set pull-high
	RF_SCK_CTR = 0;				// SCK	set output
	RF_CSN_CTR = 0;				// CSN	set output
	_simen = 0;
	RF_SPI_SIMC0 = 0x00;		// select SPI master mode & clk=fys/4
	RF_SPI_SIMC2 = 0x38;		// CKPOLB=1¡BCKEG=1¡BMLS=1
	_simen = 1;					// enable
#endif
}

//*******************************//
// name		:	SpiCSN_ON
// input	:	none
// output	:	none
// stack	:	1
//*******************************//
void SpiCSN_low(void)
{
#if (RF_SPI_line==3)
	RF_CSN = 0;
#else
	_csen = 1;
#endif
}

//*******************************//
// name		:	SpiCSN_OFF
// input	:	none
// output	:	none
// stack	:	1
//*******************************//
void SpiCSN_high(void)
{
#if (RF_SPI_line==3)
	RF_CSN = 1;
#else
	_csen = 0;
#endif	
}

//*******************************//
// name		:	SpiRead
// input	:	none
// output	:	SPIdata
// stack	:	1
//*******************************//
unsigned char SpiRead(void)
{
#if (RF_SPI_line==3)
	unsigned char cnt,SIMD=0;
	RF_SDIO_CTR = 1;
	RF_SCK	= 0;
	for	(cnt=0;cnt<8;cnt++)
	{
		SIMD = SIMD<<1;
		RF_SCK	= 1;
		if(RF_SDIO)
			SIMD |= 0x01; 
		RF_SCK	= 0;
	}
	return SIMD;
#else
	unsigned char SIMD=0;
	
	do
	{	_wcol = 0;
		_simd = 0xff;
	}
	while(_wcol);
	
	while(!_trf);
	_trf = 0;
	SIMD = _simd;
	return SIMD;
#endif	
}

//*******************************//
// name		:	SpiWrite
// input	:	SPIdata
// output	:	none
// stack	:	1
//*******************************//
void SpiWrite(unsigned char SIMD)
{
#if (RF_SPI_line==3)
	unsigned char	cnt;
	RF_SDIO_CTR = 0;
	for	(cnt=0;cnt<8;cnt++)
	{
		RF_SCK = 0;
		if( SIMD & 0x80)
			RF_SDIO = 1; 
		else
			RF_SDIO = 0;
		SIMD = SIMD<<1;
		RF_SCK = 1;
	}
	RF_SCK = 0;
#else
	do
	{	_wcol = 0;
		_simd = SIMD;
	}
	while(_wcol);
	
	while(!_trf);
	_trf = 0;
#endif	
}

