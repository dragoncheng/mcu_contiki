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
/*PROCESS ��ʵ��������һ������������һ�����̿��ƿ飬�´����Ľ��� next ָ��ָ��գ���������Ϊ��Hello world����
����ִ���庯��ָ��Ϊprocess_thread_hello_world_process �� ����������ptΪ0 ��״̬Ϊ 0( ��PROCESS_STATE_NONE)��
���ȼ����λ needspoll ҲΪ 0(����ͨ���ȼ�)��*/

PROCESS(led_process, "led");
PROCESS(stm32_shell_process, "shell");

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
   PROCESS_END();//�����ڸ�������ϵ���Ϣ (������ )
}


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
	process_start(&etimer_process, NULL);//����ϵͳ����
  process_start(&stm32_shell_process, NULL);
	autostart_start(autostart_processes);//�����û�����������
	//EvbUart1Printf("Processes running\r\n");
	while(1) {
		do {
		} while(process_run() > 0);
		idle_count++;
  }
}

