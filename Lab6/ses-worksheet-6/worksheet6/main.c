/*
	Copyright 2012, Michael Scott

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/*changed USART3 to USART2 for Rev B boards - craig 18/2/14 */
#include "com_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

int __io_putchar(int c) {

	/* Wait until ready to send */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
	}

	USART_SendData(USART2, (u16) c);

	return c;
}

int __io_getchar(void) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET) {
	
	}
	
	return (int)USART_ReceiveData(USART2);
}

void COMPortInit ( void ) {

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE ); 

	/* Enable USART2 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );

	/* Remap USART, as USART2 uses alternate pins */
	GPIO_PinRemapConfig( GPIO_Remap_USART2,ENABLE );

	/* Configure USART2 Tx pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   /*changed from 8 to 5 for USART craig 2*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOD, &GPIO_InitStructure );

	/* Configure USART2 Rx pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; /* changed from 9 to 6 for USART 2 craig */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOD, &GPIO_InitStructure );

	/* Configure USART 8N1 */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init( USART2, &USART_InitStructure );

	/* Enable USART */
	USART_Cmd( USART2, ENABLE );
}

void debug_info(void) {
	register char *stack_ptr asm("sp");
	void *heap_ptr = sbrk(0);
	extern char end asm("end");
	
	printf("\r\n--- Debug Info ---\r\n");
	printf("Stack pointer: %p\r\n", stack_ptr);
	printf("Heap pointer: %p\r\n", heap_ptr);
	printf("Data Segment (end): %p\r\n", &end);
}

int strlen(const char *s) {
	int length = 0;
	while (s[length] != '\0') {
		length++;
	}
	return length;
}

int main(void) {
  int i,c;

	/* char input_char; */
	int a, b, answer, correct_answer;
	char seed;
	
	COMPortInit();
	
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	
	for (i = 0; i < 5; i++) {
		printf("hello world\r\n");
	}
	
	/* printf("Press any key to begin:\r\n");
	
	seed = getchar();
	srand(seed);
	
	while(1) {
		a = (rand() % 20) + 1;
		b = (rand() % 20) + 1;
		correct_answer = a + b;
		debug_info();
		
		printf("\r\nWhat is %d + %d?\r\n", a, b);
		printf("Your answer: ");
		
		while (scanf("%d", &answer) != 1) {
			int ch;
			
			while ((ch = getchar()) != '\n' && ch != '\r' && ch != EOF) {
			}
			
			printf("\r\nInvalid input type. Please enter a numeric value. \r\n");
			printf("Your answer: ");
		}
		
		if (answer == correct_answer) {
			printf("\r\nCorrect! Well done\r\n");
		} else {
			printf("\r\nIncorrect. The answer was %d", correct_answer);
		}
	} */

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

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
