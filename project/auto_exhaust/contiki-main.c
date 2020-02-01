#include "adcan/adcan_bsp.h"

#include <stdint.h>
#include <stdio.h>

#include <sys/clock.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <sys/etimer.h>
#include <sys/autostart.h>

#include <dev/serial-line.h>
#include "serial-shell/serial-shell.h"
#include <debug-uart.h>

#ifdef SHELL_CONF_PROMPT
char shell_prompt_text[] = "Ruijie> ";
#endif

#ifdef SHELL_CONF_BANNER
char shell_banner_text[] = "Ruijie command shell";
#endif


unsigned int idle_count = 0;

uint16_t i;

PROCESS(hello_world_process, "Hello world");

PROCESS(led_process, "led");

AUTOSTART_PROCESSES(&led_process);

#define LED_TIMER CLOCK_SECOND/4
PROCESS_THREAD(led_process, ev, data)
{
  PROCESS_BEGIN();//宏用于辅助保存断点信息 (即行数 ),欲实现的代码必须放在宏 PROCESS_BEGIN 与 PROCESS_END 之间
  while(1)
  {
    static struct etimer et;
    etimer_set(&et, LED_TIMER);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    gd_eval_led_on(LED2);
    //EvbUart1Printf("LED0 ----ON-----\r\n");
	 
    gd_eval_led_off(LED3);
    //EvbUart1Printf("LED1 -----OFF-----\r\n"); 
 
    etimer_set(&et, LED_TIMER);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		
    gd_eval_led_off(LED2);
    //EvbUart1Printf("LED0 -----OFF-----\r\n");  
    gd_eval_led_on(LED3);
    //EvbUart1Printf("LED1 ----ON-----\r\n");
    
  }
   PROCESS_END();//宏用于辅助保存断点信息 (即行数 )
}


PROCESS_THREAD(hello_world_process, ev, data)
{ 
    PROCESS_BEGIN();//宏用于辅助保存断点信息 (即行数 )
    while(1)
    {
      static struct etimer et1;
      etimer_set(&et1, CLOCK_SECOND*3);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et1));
      //EvbUart1Printf("Hello, world!\r\n");
    }
    PROCESS_END();//宏用于辅助保存断点信息 (即行数 )
}

int main()
{
  gd_eval_led_init(LED2);
  gd_eval_led_init(LED3);
  gd_eval_led_init(LED4);
  gd_eval_led_init(LED5);
  bsp_uart_init(BSP_COM0, 115200U);
  dbg_setup_uart();
  
	clock_init();
	process_init();

  nvic_irq_enable(USART0_IRQn, 0, 0);
          
  uart1_set_input(serial_line_input_byte);
  serial_line_init();
  serial_shell_init();

  /* enable USART0 receive interrupt */
  usart_interrupt_enable(USART0, USART_INT_RBNE);
  
  /* enable USART0 transmit interrupt */
  //usart_interrupt_enable(USART0, USART_INT_TBE);

  process_start(&etimer_process, NULL);//启动系统进程
	autostart_start(autostart_processes);//启动用户自启动进程
	while(1) {
		do {
		} while(process_run() > 0);
		idle_count++;
  }
}

