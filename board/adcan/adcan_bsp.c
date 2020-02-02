#include "gd32f10x.h"
#include <sys/clock.h>

#include "adcan_bsp.h"

void bsp_irq_enable(void)
{
  nvic_irq_enable(USART0_IRQn, 0, 0);
  /* enable USART0 receive interrupt */
  usart_interrupt_enable(USART0, USART_INT_RBNE);
  
  /* enable USART0 transmit interrupt */
  //usart_interrupt_enable(USART0, USART_INT_TBE);

}

static clock_time_t delay_ts;
void delay_clock(uint32_t t)
{
  delay_ts = clock_time();
  while(clock_time() < delay_ts+t);
}
