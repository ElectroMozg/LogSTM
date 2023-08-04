/*
 * usartMy.c
 *
 *  Created on: Aug 4, 2023
 *      Author: user
 */

#include "usartMy.h"
#include "queue.h"

#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_gpio.h"

#include <stdlib.h>
#include <string.h>


#define ARRAY_LEN(x)            (sizeof(x) / sizeof((x)[0]))

uint8_t usart_rx_dma_buffer[24];
extern   Queue queue;



void uart_init(void){

	LL_USART_InitTypeDef USART_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	/*
	 * USART2 GPIO Configuration
	 *
	 * PA2   ------> USART2_TX
	 * PA3   ------> USART2_RX
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USART2 DMA Init */
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_USART2_RX);
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_BYTE);

	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_1, LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_RECEIVE));
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)usart_rx_dma_buffer);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ARRAY_LEN(usart_rx_dma_buffer));

	/* Enable HT & TC interrupts */
	LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

	/* DMA interrupt init */
	NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	/* Configure USART2 */
	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	USART_InitStruct.BaudRate = 115200;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART2, &USART_InitStruct);
	LL_USART_SetTXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_DisableFIFO(USART2);
	LL_USART_ConfigAsyncMode(USART2);
	LL_USART_EnableDMAReq_RX(USART2);
	LL_USART_EnableIT_IDLE(USART2);

	/* USART interrupt */
	NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_EnableIRQ(USART2_IRQn);

	/* Enable USART and DMA */
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
	LL_USART_Enable(USART2);
}

void uart_transmit_buffer(uint8_t* data, uint32_t size){

    for (uint32_t i = 0; i < size; i++)
    {
        // Проверка, готов ли UART для передачи данных
        while (!LL_USART_IsActiveFlag_TXE(USART2));

        // Запись данных в регистр передачи UART
        LL_USART_TransmitData8(USART2, data[i]);
    }
}

void uart_rx_check(void){

    static size_t old_pos;
    size_t pos;

    /* Calculate current position in buffer and check for new data available */
    pos = ARRAY_LEN(usart_rx_dma_buffer) - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_1);
    if (pos != old_pos) {                       /* Check change in received data */

    	char string[20];
        if (pos > old_pos) {                    /* Current position is over previous one */

            for (int i = 0; i <= pos - old_pos -1; i++) {
            	string[i] = (char)usart_rx_dma_buffer[old_pos+i]; // Явное приведение типов
            }

            string[strlen(string)] = '\0';
        	enqueue(&queue, string);
            //usart_process_data(&usart_rx_dma_buffer[old_pos], pos - old_pos);
        } else {

            for (int i = 0; i <= ARRAY_LEN(usart_rx_dma_buffer) - old_pos -1; i++) {
            	string[i] = (char)usart_rx_dma_buffer[old_pos+i]; // Явное приведение типов
            }


            //usart_process_data(&usart_rx_dma_buffer[old_pos], ARRAY_LEN(usart_rx_dma_buffer) - old_pos);
            if (pos > 0) {
                //usart_process_data(&usart_rx_dma_buffer[0], pos);

                for (int i = 0; i <= pos; i++) {
                	string[ARRAY_LEN(usart_rx_dma_buffer) - old_pos+ i] = (char)usart_rx_dma_buffer[0+i]; // Явное приведение типов
                }
            }
            string[strlen(string)] = '\0';
            enqueue(&queue, string);
        }
        old_pos = pos;                          /* Save current position as old for next transfers */
    }
}
