
#ifndef ADC_CAN_BSP_H
#define ADC_CAN_BSP_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f10x.h"
#include "adc_can_eval_bsp.h"
 
#define COMn                                2U

#define ADC_CAN_COM0                        USART0
#define ADC_CAN_COM0_CLK                    RCU_USART0
#define ADC_CAN_COM0_TX_PIN                 GPIO_PIN_9
#define ADC_CAN_COM0_RX_PIN                 GPIO_PIN_10
#define ADC_CAN_COM0_GPIO_PORT              GPIOA
#define ADC_CAN_COM0_GPIO_CLK               RCU_GPIOA

#define ADC_CAN_COM1                        USART1
#define ADC_CAN_COM1_CLK                    RCU_USART1
#define ADC_CAN_COM1_TX_PIN                 GPIO_PIN_2
#define ADC_CAN_COM1_RX_PIN                 GPIO_PIN_3
#define ADC_CAN_COM1_GPIO_PORT              GPIOA
#define ADC_CAN_COM1_GPIO_CLK               RCU_GPIOA

/* configure COM port */
void adc_can_com_init(uint32_t com);

#ifdef cplusplus
}
#endif

#endif /* ADC_CAN_BSP_H */
