#include "hal_uart.h"

u8 ReceiveData;

char* temPtr;

char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}


void uart_prot_init(void)
{
    UART_RX_PU = 1;
    UART_RX_CTR = 1;
    UART_TX_CTR = 0;

    UART_PxSx |= 0x14;
    UART_IFS &= 0xfe;

    _ucr1 = (0x80|(UART_PREN<<5)|(UART_PRT<<4)|(UART_STOP_LEN<<3));
 	_ucr2 = (0xC0|(UART_HBAUD_EN<<5));
 	_brg = BRG; 
 	
	uart_int_init();
}

void uart_int_init(void)
{
    _rie = 1;
    _wake = 1;
    _tiie = 0;
    _teie = 0;
    _uarte = 1;
    _uartf = 0;
}

void uart_send(u8 data)
{
    while(0 == _tidle);
    _txr_rxr = data;
    while(0 == _tidle);
}

u8 uart_receive()
{
    u8 data = 0;
    while(0 == _ridle);
        data = _txr_rxr;
    return data;
}

void uart_printf(char* payload)
{
    while(*payload)
        uart_send(*payload++);
}

void uart_int(u8 value)
{
    memcpy(temPtr, '\0', 10);
    itoa(value, temPtr, 10);
    uart_printf(temPtr);
}

