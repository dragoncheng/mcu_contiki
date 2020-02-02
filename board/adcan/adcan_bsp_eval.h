
#ifndef ADC_CAN_EVAL_BSP_H
#define ADC_CAN_EVAL_BSP_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f10x.h"
     
/* exported types */
typedef enum 
{
    LED2 = 0,
    LED3 = 1,
    LED4 = 2,
    LED5 = 3
} led_typedef_enum;

typedef enum 
{
    KEY_WAKEUP = 0,
    KEY_TAMPER = 1,
    KEY_USER   = 2
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

/* function declarations */
/* configure led GPIO */
void bsp_eval_led_init(led_typedef_enum lednum);
/* turn on selected led */
void bsp_eval_led_on(led_typedef_enum lednum);
/* turn off selected led */
void bsp_eval_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void bsp_eval_led_toggle(led_typedef_enum lednum);
/* configure key */
void bsp_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected key state */
uint8_t bsp_eval_key_state_get(key_typedef_enum key);

#ifdef cplusplus
}
#endif

#endif /* ADC_CAN_EVAL_BSP_H */
