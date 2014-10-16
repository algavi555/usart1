#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

volatile unsigned int ReceiveData=0;

unsigned short data;

void USART2_IRQHandler(void)
{
	unsigned short temp;
  temp=USART2->SR	;
	
	if (temp & USART_SR_RXNE) //if receiver not empty  
	{
		GPIOE->BSRR = 1<<10; 
		data=USART2->DR;
		USART2->DR=data;
		USART2->SR &= ~USART_SR_RXNE; //clear the flag

	}
	//if (temp & USART_SR_TXE)  //if tranceiver is empty
	//{
		GPIOE->BSRR = 1<<11; 
		USART2->SR &= ~USART_SR_TXE; //clear the flag
	//}
	if (temp & USART_SR_TC)   //if transmission complete
	{
	GPIOE->BSRR = 1<<12; 
	USART2->SR=temp&~USART_SR_TC; //clear the flag
	}
}

int main(void)
{	
	//clocking
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;
	 
	//sign of ready
	GPIOE->CRH    = 0x33333333;//general purpoise PP_50MHz
	GPIOD->CRL |= GPIO_CRL_CNF5_1;
	GPIOD->CRL |= GPIO_CRL_MODE5;
	//GPIOE->BSRR = 1<<8|1<<9; 
	
	//usart init
	AFIO->MAPR = 0x8;//alternate function for usart2 enable

		//UART
	USART2->BRR=0x1D4C;
	USART2->CR1 |= USART_CR1_RE|USART_CR1_TE|USART_CR1_UE|USART_CR1_RXNEIE; //enable receiver,transmitter,update,rec. interrupt
	
		//interrupts
	NVIC_EnableIRQ(USART2_IRQn);
	//GPIOE->ODR = 0;
	USART2->DR = 0x31;

while (1){USART2->DR = 0x26;}
}
