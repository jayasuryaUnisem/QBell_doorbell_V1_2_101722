#ifndef HAL_ADC_H
#define HAL_ADC_H
#include "main.h"

typedef unsigned char u8;
typedef unsigned short u16;

#define	ADC_ENABLE()					(_adcen=1)
#define ADC_DISABLE()					(_adcen=0)

#define ADC_OPA_ENABLE()				(_adpgaen=1)
#define ADC_OPA_DISABLE()				(_adpgaen=0)

#define ADC_VBGREN_ENABLE()				(_vbgren=1)
#define ADC_VBGREN_DISABLE()			(_vbgren=0)

#define ADC_VALUE_RIGHT_ALIG()			(_adrfs=1)			
#define ADC_VALUE_LEFT_ALIG()			(_adrfs=0)

#define ADC_SELECT_RIGHT_ALIG			1
#define ADC_SELECT_LET_ALIG				0

#define ADC_BATT_LEVEL_1				2600
#define ADC_BATT_LEVEL_2				2300
#define ADC_BATT_LEVEL_3 				2080
#define ADC_BATT_LEVEL_4				2270
#define ADC_BATT_LEVEL_5				2270				

/*
	ADC clock frequency selection
*/
#define ADC_CLOCK_FSYS()				{_sacks2=0; _sacks1=0; _sacks0=0;}
#define ADC_CLOCK_FSYS2()				{_sacks2=0; _sacks1=0; _sacks0=1;}
#define ADC_CLOCK_FSYS4()				{_sacks2=0; _sacks1=1; _sacks0=0;}
#define ADC_CLOCK_FSYS8()				{_sacks2=0; _sacks1=1; _sacks0=1;}
#define ADC_CLOCK_FSYS16()				{_sacks2=1; _sacks1=0; _sacks0=0;}
#define ADC_CLOCK_FSYS32()				{_sacks2=1; _sacks1=0; _sacks0=1;}
#define ADC_CLOCK_FSYS64()				{_sacks2=1; _sacks1=1; _sacks0=0;}
#define ADC_CLOCK_FSYS128()				{_sacks2=1; _sacks1=1; _sacks0=1;}


/*
	Setting AD convert referemce voltage
*/
#define ADC_REF_VOLTAGE_VDD()			{_savrs1=0; _savrs0=0;}
#define ADC_REF_VOLTAGE_VREF()			{_savrs1=0; _savrs0=1;}
#define ADC_REF_VOLTAGE_PGA()			{_savrs1=1; _savrs0=1;}

#define ADC_EXT_ANA_CHANNEL()			{_sains3 = 0; _sains2 = 0; _sains1 = 0; _sains0 = 0;}

#define ADC_START()						{_start=0; _start=1; _start=0;}

/*
	LVD function
*/
#define EN								1
#define DIS								0
#define LVD_CTRL(MODE)					(_lvden = MODE)
#define VLVD_1V8()						{_vlvd2 = 0; _vlvd1 = 0; _vlvd0 = 0;}
#define VLVD_2V0()						{_vlvd2 = 0; _vlvd1 = 0; _vlvd0 = 1;}
#define VLVD_2V4()						{_vlvd2 = 0; _vlvd1 = 1; _vlvd0 = 0;}
#define VLVD_2V7()						{_vlvd2 = 0; _vlvd1 = 1; _vlvd0 = 1;}
#define VLVD_3V0()						{_vlvd2 = 1; _vlvd1 = 0; _vlvd0 = 0;}
#define VLVD_3V3()						{_vlvd2 = 1; _vlvd1 = 0; _vlvd0 = 1;}
#define LVDO							_lvdo

#define MODE_3V3						1
#define MODE_3V0						2
#define MODE_2V7						3

extern void adc_init(void);
extern void adc_selectChannel(u8);
extern void lvd_init(void);
extern u8 lvd_read();
extern void lvd_mode_select(u8);
extern void lvd_read_select(u8);
extern u16 adc_getValue(void);

typedef enum
{
	ADC_CH0 = (u8)0x00,
	ADC_CH1 = (u8)0x01,
	ADC_CH2 = (u8)0x02,
	ADC_CH3 = (u8)0x03,
	ADC_CH4 = (u8)0x04,
	ADC_CH5 = (u8)0x05,
	ADC_CH6 = (u8)0x06,
	ADC_CH7 = (u8)0x07,
	ADC_CH8 = (u8)0x08,
	ADC_CH9 = (u8)0x09,
	ADC_CH10 = (u8)0x0A,
	ADC_CH11 = (u8)0x0B,
}ADC_Channel_TypeDef;


#endif