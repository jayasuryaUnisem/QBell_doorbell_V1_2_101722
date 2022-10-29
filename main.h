#ifndef MAIN_H
#define MAIN_H

#include "BC66F3652.h"
#include <string.h>
#include "hal_i2c.h"
#include "hal_ktd.h"
#include "hal_adc.h"
#include "hal_uart.h"
#include "hal_gpio.h"
#include "hal_rf.h"
#include "hal_eeprom.h"
#include "app.h"

#define WDTC()                                  (_wdtc=0xa8)
#define NULL()									'\0'
#define BATT_FULL                               0x08
#define BATT_AVG                                0x09
#define BATT_AVG2                               0x10
#define BATT_LOW                                0x11

//typedef unsigned char u8;
//typedef unsigned short u16;

extern void delay(unsigned short);
extern void sysInit();

typedef struct int_flags
{
    unsigned char int0_isr_flag;
    unsigned char int1_isr_flag;
    unsigned char battStatus;
    unsigned char rxPayload[10];
    unsigned char deviceID[6];
    unsigned char p1DeviceID[5];
    unsigned char p2DeviceID[5];
    unsigned char pairStatus;
    unsigned char pairingAck;
    unsigned char waitingFlag;
    unsigned char sleepFlag;

    unsigned char deviceAddCpyAck;
    unsigned char deviceAddrCpy[5];
    unsigned char deviceAddrCmprF;

    unsigned char lvdF;
    unsigned char mode3V3F;
    unsigned char mode3V0F;
    unsigned char mode2V7F;
}int_flags;

//struct define_flag Mflag;


#endif