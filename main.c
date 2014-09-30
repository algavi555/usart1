#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_conf.h"


int ReceiveData;
GPIO_InitTypeDef gpio;

void USART1_IRQHandler(void)
{
	GPIOE->ODR = 0; //turn the light off
if (USART1->SR & USART_SR_RXNE) // receiver is not empty
{
GPIOE->BSRR = GPIO_BSRR_BS8;	//zazhech,esli chto-to peredaet**
USART1->SR &= ~USART_SR_RXNE;	//clear not empty flag
ReceiveData=USART1->DR;	//write received data to variable
USART1->DR = ReceiveData;	//come back
}
if (USART1->SR & USART_SR_TXE) //transmitter is empty
{
USART1->SR &= ~USART_SR_TXE;	//clear empty flag
}
if (USART1->SR & USART_SR_TC) // transmission complete
{
GPIOE->BSRR = GPIO_BSRR_BS15;	
USART1->SR &= ~USART_SR_TC;	//clear transmission flag
}
}

int main()
{ 
	
//*peripheral clocking*/
	
RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//portA enable
RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;//alternate function for port enable
RCC->APB2ENR |= RCC_APB2ENR_USART1EN;//Usart enable
RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;//portE enable	

	/*portE configuration*/

	gpio.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_15; 
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOE, &gpio);	
	
/* portA configuration */
	
GPIOA->CRH |= GPIO_CRH_CNF9_1;//alternate push-pull
GPIOA->CRH |= GPIO_CRH_MODE9; //pin 9 output mode max 50 MHz//pin10 input mode
GPIOA->CRH |= GPIO_CRH_CNF10_0;//floating input

USART1->CR1 |= USART_CR1_RE|USART_CR1_TE|USART_CR1_UE|USART_CR1_RXNEIE;//enable receiver,transmitter,usart,interrupts
USART1->DR=0xFFFF;//set anything to data register
	
NVIC_EnableIRQ(USART1_IRQn);//usart interrupt enable		
//sign of ready
GPIOE->BSRR = GPIO_BSRR_BS10;		
 while (1);
}
