#include "hal_adc.h"

extern int_flags intF;

/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*********************************************//
// name		:	adc_inti
// input	:	none
// output	:	none
// function :	ADC enable and intiate function. 
//				1. frequency set
//				2. ADC Enable
//				3. ADC channel select
//				4. ADC Value alignment selection
//				5. ADC reference voltage selection
//*********************************************//

void adc_init()
{
	ADC_CLOCK_FSYS();
	ADC_ENABLE();
	ADC_EXT_ANA_CHANNEL();
	ADC_VALUE_RIGHT_ALIG();
	ADC_REF_VOLTAGE_VDD();
		

	adc_selectChannel(ADC_CH2);
	//ADC_ENABLE();
	lvd_init();
	delay(1);	
}


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*********************************************//
// name		:	adc_selectChannel
// input	:	u8 ADC_Channel -> selecting adc channels
// output	:	none
// function : 	Function for selecting a ADC channel (11 ADC channels avaiable in BC66F3652)
//*********************************************//
void adc_selectChannel(u8 ADC_Channel)
{
	_sadc1 &= 0b00001111;
	
	if(ADC_Channel < 11)
	{
		_sadc0 &= 0b11110000;
		
		_sadc0 |= ADC_Channel;
		
		switch (ADC_Channel)
		{
			case ADC_CH0:
				_pbs01 = 1; _pbs00 = 0;
				break;
			
			case ADC_CH1:
				_pbs03 = 1; _pbs02 = 0;
				break;
			
			case ADC_CH2:
				_pbs05 = 1; _pbs04 = 1;
				break;
			
			case ADC_CH3:
				_pas11 = 1; _pas10 = 1;
				break;
			
			case ADC_CH4:
				_pas13 = 1; _pas12 = 0;
				break;
			
			case ADC_CH5:
				_pas15 = 1; _pas14 = 0;
				break;
			
			case ADC_CH6:
				_pas17 = 1; _pas16 = 1;
				break;
			
			case ADC_CH7:
				_pbs07 = 1; _pbs06 = 1;
				break;
			
			case ADC_CH8:
				_pbs11 = 1; _pbs10 = 1;
				break;
			
			case ADC_CH9:
				_pds07 = 1; _pds06 = 1;
				break;
			
			case ADC_CH10:
				_pds05 = 1; _pds04 = 1;
				break;
				
			case ADC_CH11:
				_pds03 = 1; _pds02 = 1;
				break;
				
			default:
				break;
		}
	}
	
	else
	{
		_sadc1 |= ADC_Channel;
	}
}



/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*********************************************//
// name		:	adc_getValue
// input	:	none
// output	:	u16 -> return the 12 bit adc value in unsigned short data type
// function :	this api will return the adc 12 bit value in  u16 data type.
//*********************************************//
u16 adc_getValue()
{
	ADC_VBGREN_ENABLE();
	u16 value;
	ADC_START();
	
	while(1 == _adbz);
	
#if ADC_SELECT_RIGHT_ALIG
	{
		value = ((u16)_sadoh <<8) | _sadol;
	}
#elif ADC_SELECT_LET_ALIG
	{
		value = ((u16)_sadoh <<4) | (_sadol >>4);
	}
#endif
	ADC_VBGREN_DISABLE();
	return value;
}

void lvd_init(void)
{
	_lve = 1;
	LVD_CTRL(EN);
	VLVD_3V3();
}

u8 lvd_read()
{
	if(LVDO)
		intF.lvdF = 1;
	else
		intF.lvdF = 0;
	return intF.lvdF;
}

void lvd_mode_select(u8 mode)
{
	_lve = 1;
	LVD_CTRL(EN);
	switch (mode)
	{
	case MODE_3V3:
		VLVD_3V3();
		uart_printf("3v3 LVD Mode \n");
		break;
	
	case MODE_3V0:
		VLVD_3V0();
		uart_printf("3V0 LVD Mode\n");
		break;
	
	case MODE_2V7:
		VLVD_2V7();
		uart_printf("2V7 LVD Mode\n");
		break;

	default:
		break;
	}
}

void lvd_read_select(u8 mode)
{
	switch (mode)
	{
	case MODE_3V3:
		if(LVDO)
			intF.mode3V3F = 1;
		else
			intF.mode3V3F = 0;
		break;
	
	case MODE_3V0:
		if(LVDO)
			intF.mode3V0F = 1;
		else
			intF.mode3V0F = 0;
		break;
	
	case MODE_2V7:
		if(LVDO)
			intF.mode2V7F = 1;
		else
			intF.mode2V7F = 0;
		break;
		
	default:
		break;
	}
}