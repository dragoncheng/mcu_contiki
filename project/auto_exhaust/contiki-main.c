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

#define USE_EVAL_BOARD 1

unsigned int idle_count = 0;

uint16_t i;

PROCESS(hello_world_process, "Hello world");
#if USE_EVAL_BOARD
PROCESS(led_process, "led");

AUTOSTART_PROCESSES(&led_process);

#define LED_TIMER CLOCK_SECOND/4
PROCESS_THREAD(led_process, ev, data)
{
  PROCESS_BEGIN();//�����ڸ�������ϵ���Ϣ (������ ),��ʵ�ֵĴ��������ں� PROCESS_BEGIN �� PROCESS_END ֮��
  while(1)
  {
    static struct etimer et;
    etimer_set(&et, LED_TIMER);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    bsp_eval_led_on(LED2);
    //EvbUart1Printf("LED0 ----ON-----\r\n");
	 
    bsp_eval_led_off(LED3);
    //EvbUart1Printf("LED1 -----OFF-----\r\n"); 
 
    etimer_set(&et, LED_TIMER);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		
    bsp_eval_led_off(LED2);
    //EvbUart1Printf("LED0 -----OFF-----\r\n");  
    bsp_eval_led_on(LED3);
    //EvbUart1Printf("LED1 ----ON-----\r\n");
    
  }
   PROCESS_END();//�����ڸ�������ϵ���Ϣ (������ )
}
#endif

PROCESS_THREAD(hello_world_process, ev, data)
{ 
    PROCESS_BEGIN();//�����ڸ�������ϵ���Ϣ (������ )
    while(1)
    {
      static struct etimer et1;
      etimer_set(&et1, CLOCK_SECOND*3);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et1));
      //EvbUart1Printf("Hello, world!\r\n");
    }
    PROCESS_END();//�����ڸ�������ϵ���Ϣ (������ )
}

int main()
{
#if USE_EVAL_BOARD
  bsp_eval_led_init(LED2);
  bsp_eval_led_init(LED3);
#endif
  
  bsp_uart_init(BSP_COM0, 115200U);
  bsp_adc_init();

  dbg_setup_uart();
  
	clock_init();
	process_init();
          
  uart1_set_input(serial_line_input_byte);
  serial_line_init();
  serial_shell_init();

  bsp_irq_enable();
  
  process_start(&etimer_process, NULL);
	autostart_start(autostart_processes);
	while(1) {
		do {
		} while(process_run() > 0);
		idle_count++;
  }
}

