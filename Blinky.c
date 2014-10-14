#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

volatile unsigned int ReceiveData=0;


void USART1_IRQHandler(void)
{
	if (USART1->SR & USART_SR_RXNE) //if receiver not empty  
	{
		GPIOE->BSRR = 1<<10; 
	}
	if (USART1->SR & USART_SR_TXE)  //if tranceiver is empty
	{
		GPIOE->BSRR = 1<<11; 
	}
	if (USART1->SR & USART_SR_TC)   //if transmission complete
	{
	GPIOE->BSRR = 1<<11; 
}
}

int main(void)
{	
	//clocking
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN|RCC_APB2ENR_USART1EN|RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR |=  1 <<  6; 
		
		//sign of ready
	GPIOE->CRH    = 0x33333333;//general purpoise PP_50MHz
	GPIOE->BSRR = 1<<8|1<<9; 
	
		//UART
	USART1->BRR = 0x09C4;
	USART1->CR1 |= USART_CR1_RE|USART_CR1_TE|USART_CR1_UE|USART_CR1_RXNEIE; //enable receiver,transmitter,update,rec. interrupt
	
		//interrupts
	NVIC_EnableIRQ(USART1_IRQn);


while (1){}
}
