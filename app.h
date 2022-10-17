#ifndef APP_C
#define APP_C
#include "main.h"
#include <string.h>

#define CLEAR_MODE                          0x00
#define START_MODE                          0x01
#define PAIR_MODE                           0x02
#define UNPAIR_MODE                         0x03
#define ERROR_MODE                          0x04
#define NOTIFI_MODE                         0x05

#define RED                                 0x01
#define GREEN                               0x02
#define BLUE                                0x03

#define DOORBELL                            0x30
#define MASTER_1                            0x40
#define MASTER_2                            0x50

extern void app_pair_status_check();
extern void app_pair_status_update();
extern void app_pair_status_upadte2();
extern void app();
extern void unpairSpNode();
extern void LED(u8 mode);
extern void rgb_blink(u8 color);
extern void initSys(void);

#endif