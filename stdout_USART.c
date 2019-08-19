#include <string.h>
#include "stdbool.h"
#include "stm32f4xx_hal.h"

UART_HandleTypeDef huart1;

static bool uart_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_UART5_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_12;  //TX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;   //RX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    return true;
}

bool uart_init(void)
{
    uart_gpio_init();
    huart1.Instance = UART5;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_OK == HAL_UART_Init(&huart1)){
        return true;
    }
    return false;
}

//serial_out() 
bool serial_out(uint8_t chByte)
{
    if(UART5->SR & USART_SR_TC){
        UART5->SR &=~ USART_SR_TC;
        UART5->DR = chByte; 
        return true;
    }
    return false;
}

bool serial_in(uint8_t *pchByte)
{
    if(NULL == pchByte){
        return false;
    }
    if(UART5->SR & USART_SR_RXNE){
        *pchByte = UART5->DR;
        return true;
    }
    return false;
}




