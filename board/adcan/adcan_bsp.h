
#ifndef ADCAN_BSP_H
#define ADCAN_BSP_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f10x.h"
#include "adcan_bsp_uart.h"
#include "adcan_bsp_eval.h"
 
void bsp_irq_enable(void);
#ifdef cplusplus
}
#endif

#endif /* ADCAN_BSP_H */
