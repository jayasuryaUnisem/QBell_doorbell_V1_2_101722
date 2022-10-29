#ifndef HAL_EEPROM_H
#define HAL_EEPROM_H
#include "main.h"

#define EEPROM_MODE_BYTE()                          _mode = 0
#define EEPROM_MODE_PAGE()                          _mode = 1

#define EEPROM_WR_EN()                              _wren = 1
#define EEPROM_WR_DIS()                             _wren = 0

#define EEPROM_WR_CTR()                             _wr

#define EEPROM_RD_EN()                              _rden = 1
#define EEPROM_RD_DIS()                             _rden = 0

#define EEPROM_RD_CTR()                             _rd

#define EEPROM_ER_EN()                              _eren = 1
#define EEPROM_ER_DIS()                             _eren = 0

#define EEPROM_ER_CTR()                             _er

#define EEPROM_EEA()                                _eea
#define EEPROM_EED()                                _eed

#define EEPROM_ADDR_1                               0x01

#define EEPROM_MP_LOW()                             _mp1l = 0x40
#define EEPROM_MP_HIGH()                            _mp1h = 0x01

#define EEPROM_EEC_CLR()                            _iar1 = 0      

#define EEPROM_DEVICE_START_ADDR                    0
#define EEPROM_P1_DEVICE_START_ADDR                 4
#define EEPROM_P2_DEVICE_START_ADDR                 8
#define EEPROM_PAIRED_STATUS_ADDR                   12

#define NO_DEVICE_FOUND                             0
#define P1_DEVICE_CONN                              1
#define P2_DEVICE_CONN                              2
#define BOTH_DEVICE_CONN                            3

#define EEPROM_DEVICE_ADDR_CPY_ACK                  4
#define EEPROM_DEVICE_ADDR_CPY                      5
#define EEPROM_DEVICE_ADDR_CRC                      9  //select the address size depends on the crc byte size
#define EEPROM_DEVICE_ADDR_CPY_CRC                  10//11 either 10 or 11

#define EEPROM_DEVICE_ADDR_CPY_ACK_VALUE            8

typedef unsigned char u8;
typedef unsigned short u16;

extern void eeprom_init(void);
extern void eeprom_write(u8 addr, u8 byteValue);
extern u8 eeprom_read(u8 addr);


extern void eepromDeviceAddrRead();


extern void eepromDeviceAddrCpyCheck(void);
extern void eepromDeviceAddrCpyRead(void);
extern void eepromDeviceAddrCpyWrite(void);
extern u8 eepromDeviceAddrCpyAckRead(void);
extern void eepromDeviceAddrCpyAckWrite(u8);

#endif