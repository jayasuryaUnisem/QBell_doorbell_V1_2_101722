#include "hal_ktd.h"

void ktd_en()
{
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x02, 0x00);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x02, FADE_INT_250MS);
  delay(1);
}

void ktd_clear()
{
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x03, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x04, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x05, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x06, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x07, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x08, 0x00);
  //delay(1);

  WriteI2C_Int(KTD_DEVICE_ADDR, 0x09, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x0A, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x0B, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x0C, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x0D, 0x00);
  //delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x0E, 0x00);
  //delay(1);
}

void ktd_rgb_ctrl(u8 r, u8 g, u8 b)
{
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x03, r);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x04, g);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x05, b);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x06, 0x00);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x07, 0x00);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x08, 0x00);
  delay(1);

  WriteI2C_Int(KTD_DEVICE_ADDR, 0x09, SEL_LED_B);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x0A, SEL_LED_B);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x0B, SEL_LED_B);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0x0C, SEL_LED_B);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0X0D, SEL_LED_B);
  delay(1);
  WriteI2C_Int(KTD_DEVICE_ADDR, 0X0E, SEL_LED_B);
  delay(1);
}

void ktd_ring(u8 r, u8 g, u8 b, 
                                u8 a12, u8 a32, 
                                u8 b12, u8 b32,
                                u8 c12, u8 c32)
{
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x03, r);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x04, g);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x05, b);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x06, 0x00);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x07, 0x00);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x08, 0x00);
    delay(1);

    WriteI2C_Int(KTD_DEVICE_ADDR, 0x09, a12);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x0A, a32);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x0B, b12);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x0C, b32);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x0D, c12);
    delay(1);
    WriteI2C_Int(KTD_DEVICE_ADDR, 0x0E, c32);
    delay(1);
}

void ktd_clockwise_ring(u8 r, u8 g, u8 b, int msDelay)
{
	int i = 0;
    for (i = 0; i<12; i++)
    {
      switch(i)
      {
        case 0:
          ktd_ring(r, g, b, 
                    SEL_LED_1, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 1:
          ktd_ring(r, g, b, 
                    SEL_LED_2, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 2:
          ktd_ring(r, g, b, 
                    OFF_LED_B, SEL_LED_1, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 3:
          ktd_ring(r, g, b, 
                    OFF_LED_B, SEL_LED_2, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 4:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    SEL_LED_1, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 5:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    SEL_LED_2, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;

        case 6:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, SEL_LED_1,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 7:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, SEL_LED_2,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 8:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    SEL_LED_1, OFF_LED_B);
          break;
        
        case 9:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    SEL_LED_2, OFF_LED_B);
          break;
        
        case 10:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, SEL_LED_1);
          break;
        
        case 11:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, SEL_LED_2);
          break;
      }
      delay(msDelay);
    }
}


void ktd_anticlockwise_ring(u8 r, u8 g, u8 b, int msDelay)
{
	int i = 0;
    for (i = 0; i<12; i++)
    {
      switch(i)
      {
        case 11:
          ktd_ring(r, g, b, 
                    SEL_LED_1, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 10:
          ktd_ring(r, g, b, 
                    SEL_LED_2, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 9:
          ktd_ring(r, g, b, 
                    OFF_LED_B, SEL_LED_1, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 8:
          ktd_ring(r, g, b, 
                    OFF_LED_B, SEL_LED_2, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 7:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    SEL_LED_1, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 6:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    SEL_LED_2, OFF_LED_B,
                    OFF_LED_B, OFF_LED_B);
          break;

        case 5:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, SEL_LED_1,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 4:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, SEL_LED_2,
                    OFF_LED_B, OFF_LED_B);
          break;
        
        case 3:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    SEL_LED_1, OFF_LED_B);
          break;
        
        case 2:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    SEL_LED_2, OFF_LED_B);
          break;
        
        case 1:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, SEL_LED_1);
          break;
        
        case 0:
          ktd_ring(r, g, b, 
                    OFF_LED_B, OFF_LED_B, 
                    OFF_LED_B, OFF_LED_B,
                    OFF_LED_B, SEL_LED_2);
          break;
      }
      delay(msDelay);
    }
}