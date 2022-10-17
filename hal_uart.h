#ifndef HAL_UART_H
#define HAL_UART_H
#include "main.h"
#include "string.h"

typedef unsigned char u8;
typedef unsigned short u16;

#define BAUD_RATE                           9600
#define UART_HBAUD_EN                       0

#define UART_PREN                           0
#define UART_PRT                            0
#define UART_STOP_LEN                       0

#define UART_RX_PROT                        _pd1
#define UART_RX_CTR                         _pdc1
#define UART_RX_PU                          _pdpu1

#define UART_TX_PROT                        _pd2
#define UART_TX_CTR                         _pdc2

#define UART_PxSx                           _pds0
#define UART_IFS                            _ifs

#define	fH		8000000

#if UART_HBaud_EN
#define	BRG 	((fH/(BAUD_RATE*16L))-1)
#else
#define	BRG 	((fH/(BAUD_RATE*64L))-1)
#endif

extern void uart_prot_init(void);
extern void uart_int_init(void);
extern void uart_send(unsigned char);
extern void uart_printf(char*);
extern void uart_int(u8);
extern u8 uart_receive();
extern char* itoa(int value, char* result, int base);


#endif