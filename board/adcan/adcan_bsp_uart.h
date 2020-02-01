#ifndef ADCAN_BSP_UART_H
#define ADCAN_BSP_UART_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f10x.h"
 
#define COMn                            2U
#define BSP_COM0                        USART0
#define BSP_COM1                        USART1

/* configure COM port */
void bsp_uart_init(uint32_t com, uint32_t bandrate);
int32_t bsp_uart_dma_config(uint32_t com);
void uart1_set_input(int (* input)(unsigned char c));

#ifdef cplusplus
}
#endif

#endif /* ADCAN_BSP_UART_H */
