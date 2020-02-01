
#include "adc_can_bsp.h"

/* private variables */
static rcu_periph_enum COM_CLK[COMn] = {ADC_CAN_COM0_CLK, ADC_CAN_COM1_CLK};
static uint32_t COM_TX_PIN[COMn] = {ADC_CAN_COM0_TX_PIN, ADC_CAN_COM1_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {ADC_CAN_COM0_RX_PIN, ADC_CAN_COM1_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn] = {ADC_CAN_COM0_GPIO_PORT, ADC_CAN_COM1_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn] = {ADC_CAN_COM0_GPIO_CLK, ADC_CAN_COM1_GPIO_CLK};


/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        ADC_CAN_COM0: COM0 on the board
      \arg        ADC_CAN_COM1: COM1 on the board
    \param[out] none
    \retval     none
*/
void adc_can_com_init(uint32_t com)
{
    uint32_t com_id = 0U;
    if(ADC_CAN_COM0 == com){
        com_id = 0U;
    }else if(ADC_CAN_COM1 == com){
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
    usart_baudrate_set(com, 115200U);
    usart_word_length_set(com, USART_WL_8BIT);
    usart_stop_bit_set(com, USART_STB_1BIT);
    usart_parity_config(com, USART_PM_NONE);
    usart_hardware_flow_rts_config(com, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(com, USART_CTS_DISABLE);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
}
