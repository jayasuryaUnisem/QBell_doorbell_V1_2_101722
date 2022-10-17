#include "hal_gpio.h"

int_flags intF;

void gpio_init()
{
#if INT0_EN
    _pbc0 = 1;  _pbpu0 = 1;
    _pbs01 = 0; _pbs00 = 0;
#endif
#if INT1_EN
    _pbc1 = 1;  _pbpu1 = 1;
    _pbs03 = 0; _pbs02 = 0;
#endif
    
#if RGB_EN
    RGB_R = 1;
    RGB_B = 1;
    RGB_G = 1;
    RGB_INIT();
#endif

#if MOTOR_EN
    MOTOR_DRV_PIN = 0;
    MOTOR_DRV_PIN_CTR();
#endif

#if WAKEUP_EN
    WAKEUP_PIN = 0;
    WAKEUP_INIT();

    UNPAIR_BUTTON = 0;
    UNPAIR_BTT_INIT();
#endif

}


void int_init()
{
    
    INT0_EDGE_RISING_CTR();
    INT1_EDGE_RISING_CTR();
    INT0_REG_FLAG_NOREQ(); //Interrupt0 flag cleared
    INT1_REG_FLAG_NOREQ();
    INT0_ISR_CTR_EN(); //Enable external interrupt
    INT1_ISR_CTR_EN();

    ISR_CTR_EN();

    gpio_init();
    //delay(10);
}


void rgb_led_ctrl(u8 r, u8 g, u8 b)
{
    RGB_R = r;
    RGB_G = g;
    RGB_B = b;
}


extern void __attribute((interrupt(0x04))) INT0_proc(void)
{
	intF.int0_isr_flag = 1;
	//_nop();
}

extern void __attribute((interrupt(0x24))) INT1_proc(void)
{
    intF.int1_isr_flag = 1;
//	_nop();
}
