#include "adc_can/adc_can_bsp.h"

#include <stdint.h>
#include <stdio.h>

#include <sys/clock.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <sys/etimer.h>
#include <sys/autostart.h>

unsigned int idle_count = 0;

uint16_t i;

PROCESS(hello_world_process, "Hello world");
/*PROCESS ��ʵ��������һ������������һ�����̿��ƿ飬�´����Ľ��� next ָ��ָ��գ���������Ϊ��Hello world����
����ִ���庯��ָ��Ϊprocess_thread_hello_world_process �� ����������ptΪ0 ��״̬Ϊ 0( ��PROCESS_STATE_NONE)��
���ȼ����λ needspoll ҲΪ 0(����ͨ���ȼ�)��*/

PROCESS(led_process, "led");

AUTOSTART_PROCESSES(&led_process);

#define LED_TIMER CLOCK_SECOND/2
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
int main()
{
	adc_can_com_init(ADC_CAN_COM0);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);
    gd_eval_led_init(LED5);
    
	//EvbUart1Printf("Initialising\r\n"); 
	clock_init();
	process_init();

	process_start(&etimer_process, NULL);//����ϵͳ����
	autostart_start(autostart_processes);//�����û�����������
	//EvbUart1Printf("Processes running\r\n");
	while(1) {
		do {
		} while(process_run() > 0);
		idle_count++;
  }
}

