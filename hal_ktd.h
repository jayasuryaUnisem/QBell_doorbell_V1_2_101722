#ifndef HAL_KTD_H
#define HAL_KTD_H
#include "main.h"

#define KTD_DEVICE_ADDR                     0x68


#define SEL_LED_1                           0x87    //135
#define SEL_LED_2                           0x78    //120
#define SEL_LED_B                           0x88    //136
#define OFF_LED_B                           0x00

#define FADE_INT_31MS                       0xA0    //160
#define FADE_INT_63MS                       0xA1    //161
#define FADE_INT_125MS                      0xA2    //162
#define FADE_INT_250MS                      0xA3    //163
#define FADE_INT_500MS                      0xA4    //164
#define FADE_INT_1S                         0xA5    //165
#define FADE_INT_2S                         0xA6    //166
#define FADE_INT_3S                         0xA7    //167

typedef unsigned char u8;

extern void ktd_en();
extern void ktd_clear();
extern void ktd_rgb_ctrl(u8 r, u8 g, u8 b);
extern void ktd_ring(u8 r, u8 g, u8 b, 
                                u8 a12, u8 a32, 
                                u8 b12, u8 b32,
                                u8 c12, u8 c32);
extern void ktd_clockwise_ring(u8 r, u8 g, u8 b, int delay);
extern void ktd_anticlockwise_ring(u8 r, u8 g, u8 b, int delay);

#endif