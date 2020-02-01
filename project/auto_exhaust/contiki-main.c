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

unsigned int idle_count = 0;

uint16_t i;

PROCESS(hello_world_process, "Hello world");
/*PROCESS 宏实际上声明一个函数并定义一个进程控制块，新创建的进程 next 指针指向空，进程名称为“Hello world”，
进程执行体函数指针为process_thread_hello_world_process ， 保存行数的pt为0 ，状态为 0( 即PROCESS_STATE_NONE)，
优先级标记位 needspoll 也为 0(即普通优先级)。*/

PROCESS(led_process, "led");
PROCESS(stm32_shell_process, "shell");

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

PROCESS_THREAD(stm32_shell_process, ev, data)
{
    PROCESS_BEGIN();

    serial_shell_init();//???shell
	/* add the shell blink command */
   // shell_blink_init();
    PROCESS_END();
}

int main()
{
  gd_eval_led_init(LED2);
  gd_eval_led_init(LED3);
  gd_eval_led_init(LED4);
  gd_eval_led_init(LED5);
  bsp_uart_init(BSP_COM0, 115200U);
  dbg_setup_uart();
  
	printf("Initialising\r\n"); 
	clock_init();
	process_init();
  #if 1
  nvic_irq_enable(USART0_IRQn, 0, 0);
          
  uart1_set_input(serial_line_input_byte);
  serial_line_init();
  //serial_shell_init();

  /* enable USART0 receive interrupt */
  usart_interrupt_enable(USART0, USART_INT_RBNE);
  
  /* enable USART0 transmit interrupt */
  //usart_interrupt_enable(USART0, USART_INT_TBE);
#endif
	process_start(&etimer_process, NULL);//启动系统进程
  process_start(&stm32_shell_process, NULL);
	autostart_start(autostart_processes);//启动用户自启动进程
	//EvbUart1Printf("Processes running\r\n");
	while(1) {
		do {
		} while(process_run() > 0);
		idle_count++;
  }
}

