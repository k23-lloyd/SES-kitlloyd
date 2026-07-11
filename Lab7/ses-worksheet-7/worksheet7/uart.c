/******************************************************************************** 
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team 
  * @version V3.5.0 
  * @date    08-April-2011 
  * @brief   Main program body. 
  ****************************************************************************** 
  * @attention 
  * 
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS 
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE 
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY 
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING 
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS. 
  * 
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2> 
  ****************************************************************************** 
  */ 

/* Includes ------------------------------------------------------------------*/ 
#include <stm32f10x.h> 
#include <stm32f10x_rcc.h> 
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_usart.h>
#include <misc.h> 

#define BUF_SIZE 20
volatile char rx_buf[BUF_SIZE];
volatile int rx_head = 0, rx_tail = 0, rx_count = 0;
volatile char tx_buf[BUF_SIZE];
volatile int tx_head = 0, tx_tail = 0, tx_count = 0;

/* Private variables ---------------------------------------------------------*/ 
GPIO_InitTypeDef GPIO_InitStructure;
static int yellowledval = 0;
static int greenledval = 0;

void configure_leds(void)
{	 	            
  /* GPIOC/A Periph clock enable */ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

  /* Configure PC6 in output pushpull mode */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
} 

void configure_buttonsA(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Initialise the GPIOA for button WKUP */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	/* Connect Button EXTI Line to Button GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

	/*Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Configure interrupts for button */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

}

void configure_buttonsC(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init( GPIOC, &GPIO_InitStructure );
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void  EXTI0_IRQHandler()
{

	 
    if ( EXTI_GetITStatus(EXTI_Line0) == SET)
    {
	EXTI_ClearITPendingBit( EXTI_Line0 );	
	GPIO_WriteBit(GPIOC , GPIO_Pin_7 , (yellowledval) ? Bit_SET : Bit_RESET);
	yellowledval = 1 - yellowledval;
    }

}

void EXTI15_10_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line13) != RESET ) {
		EXTI_ClearITPendingBit(EXTI_Line13);
		
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, (greenledval) ? Bit_SET : Bit_RESET);
		greenledval = 1 - greenledval;
	}
}

void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		char c = (char)(USART_ReceiveData(USART2) & 0xFF);
		if (rx_count < BUF_SIZE) {
			rx_buf[rx_head] = c;
			rx_head = (rx_head + 1) % BUF_SIZE;
			rx_count++;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
		if (tx_count > 0) {
			USART_SendData(USART2, tx_buf[tx_tail]);
			tx_tail = (tx_tail + 1) % BUF_SIZE;
			tx_count--;
		}
		else {
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
	}
}

int putchar(int c) {
	while (tx_count >= BUF_SIZE);
	
	__disable_irq();
	
	tx_buf[tx_head] = (char)c;
	tx_head = (tx_head + 1) % BUF_SIZE;
	tx_count++;
	
	__enable_irq();
	
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	return c;
}

int getchar(void) {
	int c;
	
	while (rx_count == 0);
	
	__disable_irq();
	
	c = (int)rx_buf[rx_tail];
	rx_tail = (rx_tail + 1) % BUF_SIZE;
	rx_count--;
	
	__enable_irq();
	
	return c;
}

#define mainCOM_PORT_BAUD_RATE 115200

void COMPortInit(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 1. Enable Clocks for GPIOD, AFIO, and USART2 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* 2. Remap USART2 to alternate pins (Port D) */
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

    /* 3. Configure USART2 Tx pin (PD5) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* 4. Configure USART2 Rx pin (PD6) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* 5. Enable USART2 Hardware */
    USART_Cmd(USART2, ENABLE);

    /* 6. Configure USART settings (8N1) */
    USART_InitStructure.USART_BaudRate = mainCOM_PORT_BAUD_RATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);

    /* 7. Enable the Receive Interrupt (RXNE) */
    // Note: We deliberately do NOT enable TXE here; putchar() will handle that.
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    /* 8. Configure NVIC for USART2 Interrupts */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
   
    NVIC_EnableIRQ(USART2_IRQn);
}

int main(void) 
{   
  COMPortInit();
    
  while (1) 
  { 
  	char received_char = getchar();
  	putchar(received_char);
  	
  	for (volatile int i = 0; i < 2000000; i++);
  } 
} 

#ifdef  USE_FULL_ASSERT 

/** 
  * @brief  Reports the name of the source file and the source line number 
  *         where the assert_param error has occurred. 
  * @param  file: pointer to the source file name 
  * @param  line: assert_param error line source number 
  * @retval None 
  */ 
void assert_failed(uint8_t* file, uint32_t line) 
{ 
  /* User can add his own implementation to report the file name and line number, 
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */ 

  /* Infinite loop */ 
  while (1) 
  { 
  } 
} 

#endif 

