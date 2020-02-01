#include "debug-uart.h"
#include <string.h>
#include "gd32f10x.h"
#include <stdio.h>

#ifndef DBG_UART
#define DBG_UART USART0
#endif

void dbg_setup_uart(void)
{
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
  usart_data_transmit(DBG_UART, (uint8_t)ch);
  while(RESET == usart_flag_get(DBG_UART, USART_FLAG_TBE));
  return ch;
}
