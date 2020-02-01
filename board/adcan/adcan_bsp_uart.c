#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "gd32f10x.h"
#include "adcan_bsp_uart.h"

#define ADCAN_COM0_CLK                    RCU_USART0
#define ADCAN_COM0_TX_PIN                 GPIO_PIN_9
#define ADCAN_COM0_RX_PIN                 GPIO_PIN_10
#define ADCAN_COM0_GPIO_PORT              GPIOA
#define ADCAN_COM0_GPIO_CLK               RCU_GPIOA

#define ADCAN_COM1_CLK                    RCU_USART1
#define ADCAN_COM1_TX_PIN                 GPIO_PIN_2
#define ADCAN_COM1_RX_PIN                 GPIO_PIN_3
#define ADCAN_COM1_GPIO_PORT              GPIOA
#define ADCAN_COM1_GPIO_CLK               RCU_GPIOA

/* private variables */
static rcu_periph_enum COM_CLK[COMn] = {ADCAN_COM0_CLK, ADCAN_COM1_CLK};
static uint32_t COM_TX_PIN[COMn] = {ADCAN_COM0_TX_PIN, ADCAN_COM1_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {ADCAN_COM0_RX_PIN, ADCAN_COM1_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn] = {ADCAN_COM0_GPIO_PORT, ADCAN_COM1_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn] = {ADCAN_COM0_GPIO_CLK, ADCAN_COM1_GPIO_CLK};

#ifndef NULL
#define NULL (void *)0
#endif

static int (* uart1_input_handler)(unsigned char c);

void uart1_set_input(int (* input)(unsigned char c))
{
    uart1_input_handler = input;
}

/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        BSP_COM0: COM0 on the board
      \arg        BSP_COM1: COM1 on the board
    \param[in]  bandrate: bandrated
    \param[out] none
    \retval     none
*/
void bsp_uart_init(uint32_t com, uint32_t bandrate)
{
    uint32_t com_id = 0U;
    if(BSP_COM0 == com){
        com_id = 0U;
    }else if(BSP_COM1 == com){
        com_id = 1U;
    }
    
    /* enable GPIO clock */
    rcu_periph_clock_enable(COM_GPIO_CLK[com_id]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[com_id]);

    /* connect port to USARTx_Tx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, COM_TX_PIN[com_id]);

    /* connect port to USARTx_Rx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, COM_RX_PIN[com_id]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, bandrate);
    usart_word_length_set(com, USART_WL_8BIT);
    usart_stop_bit_set(com, USART_STB_1BIT);
    usart_parity_config(com, USART_PM_NONE);
    usart_hardware_flow_rts_config(com, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(com, USART_CTS_DISABLE);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
}

/**
 * \brief UART IRQ Handler
 * \param void
 *
 *
 */
void USART0_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
      /* read one byte from the receive data register */
      if(uart1_input_handler != NULL){
        /* Read one byte from the receive data register */
        uint8_t buf = (uint8_t)usart_data_receive(USART0);
        uart1_input_handler(buf);
      }	
    }       
    //if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE)){
    //}
}
