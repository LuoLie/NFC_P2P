/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: bsp.c
  Author: Giayee       Version : V1.1        Date:15/08/10
  Description:       
    This file provides configuration of NVIC, GPIO, USART,
TIM2, TIM3, EXTI3, EXTI15, DMA and ADC. and some function's 
initialization.
***********************************************************/

#include "bsp.h"
#include "stdio.h"
#include "tim.h"
#include "oled.h"
#include "battery.h"
#include "PN532_debug.h"

extern uint16_t adcbuffer[ADC_BUFFER_SIZE];	
uint32_t ADC1_DR_ADDRESS = (uint32_t)0x4001244C;
extern void OLED_ShowLogo(void);
extern void OLED_ShowSurface(void);

static void BSP_NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure the NVIC Preemption Priority Bits */ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/***********************************************************/
	//usart1 nvic config
	NVIC_InitStructure.NVIC_IRQChannel  = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        
	NVIC_Init(&NVIC_InitStructure);
	
	//usart2 nvic config
	NVIC_InitStructure.NVIC_IRQChannel  = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        
	NVIC_Init(&NVIC_InitStructure);	
	
	//time2 nvic config												
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//time3 nvic config 													
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//time4 nvic config 													
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/***********************************************************/
	
	/* Enable and set EXTI3 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/***********************************************************/
	/* EXTI15_10_IRQn nvic config*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//EXTI9_5_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* EXTI9_5_IRQn nvic config*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void BSP_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE);
	
	/*USART1 Pin*/
	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART1_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);
	
	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART1_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);
	
	/*USART2 Pin*/
	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART2_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART2_PORT, &GPIO_InitStructure);
	
	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART2_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_PORT, &GPIO_InitStructure);
	
	/*LED*/  
	GPIO_InitStructure.GPIO_Pin = LED_R | LED_G | LED_B;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	
	/*SPI2 for OLED
	**SPI_CS---PB12
	**SPI_CLK--PB13
	**SPI_SI---PB15
	*/
	GPIO_InitStructure.GPIO_Pin = LCD_RST;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OLED_RST_PORT, &GPIO_InitStructure);

	/* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
	/* Confugure CS ,CLK, SI pin as Output Push-pull*/
	GPIO_InitStructure.GPIO_Pin = SPI_CS | SPI_CLK | SPI_SI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OLED_PORT, &GPIO_InitStructure);
	
	/*pn532 pin*/
	GPIO_InitStructure.GPIO_Pin = NFC_RSTPDN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(NFC_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = NFC_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(NFC_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = NFC_P32;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(NFC_PORT, &GPIO_InitStructure);
	
	/*power pin*/
	GPIO_InitStructure.GPIO_Pin = PWR_HOLD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PWR_PORT, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitStructure.GPIO_Pin = PWR_KEY;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(PWR_PORT, &GPIO_InitStructure);
	
	/*Motor pin*/
	GPIO_InitStructure.GPIO_Pin = MOTOR_CON;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
	
	/*charge pin*/
	GPIO_InitStructure.GPIO_Pin = CHG_LED;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CHG_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CHG_STA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(CHG_PORT, &GPIO_InitStructure);
	
	/*ADC pin*/
	GPIO_InitStructure.GPIO_Pin = BAT_ADC;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(BAT_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : TIM4_Configuration
* Description    : TIM4_Configuration program.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM4_Configuration(uint16_t delay)
{
	if(delay >= 65535 / 2)
	{
		return ;
	}
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = (delay * 2 - 1);
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock / 2000) - 1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);							
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
	TIM4->CR1 |= 0x0008;
	TIM_Cmd(TIM4, ENABLE);	
}

/*******************************************************************************
* Function Name  : TIM3_Configuration
* Description    : TIM3_Configuration program.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 20;
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock / 2000) - 1);//4000
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);							
	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);	
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period = arr; 	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //Enable specified interrupt TIM3, allow Update interrupt

	TIM_Cmd(TIM2, ENABLE);	//Enable TIM2					 
}

void close_tim2_IT(void)
{
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE );
}

void open_tim2_IT(void)
{
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );
}

static void BSP_USART_Configuration(void)
{ 
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);	
	
	USART_Cmd(USART1, ENABLE);
	
	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART1_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);
	
	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART1_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);
	
	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART2 Receive interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);

	/* Configure USARTx_Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART2_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART2_PORT, &GPIO_InitStructure);
	
	/* Configure USARTx_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART2_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_PORT, &GPIO_InitStructure);
}

void EXTI3_Configuration(EXTITrigger_TypeDef trigger)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);		//for power key 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = trigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void EXTI15_Configuration(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);	//for charge station 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void EXTI5_Configuration(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);	//for NFC_IRQ
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void DMA_Configuration(void)
{
	DMA_InitTypeDef		DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel1);	
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)adcbuffer; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
	DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;			
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;				
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_28Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1); 
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
	uint32_t temp = 0;
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain */
	BKP_DeInit();
	
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while((!(RCC->BDCR&0X02))&&temp<5000)
	{
		temp++;
	};
	if(temp>=5000)
	{
		led_flicker(LED_PORT, LED_B, 200, 5);
		return;
	}
	
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

void led_on(uint16_t led_pin)
{
	assert_param(IS_GPIO_PIN(led_pin));
	
	uint16_t led = led_pin;
	
	if(led & CHG_LED)
	{
		GPIO_SetBits(CHG_PORT, CHG_LED);
	}
	
	if(led & LED_R)
	{
		GPIO_SetBits(LED_PORT, LED_R);
	}
	
	if(led & LED_G)
	{
		GPIO_SetBits(LED_PORT, LED_G);
	}
	
	if(led & LED_B)
	{
		GPIO_SetBits(LED_PORT, LED_B);
	}
}

void led_off(uint16_t led_pin)
{
	assert_param(IS_GPIO_PIN(led_pin));
	
	uint16_t led = led_pin;
	
	if(led & CHG_LED)
	{
		GPIO_ResetBits(CHG_PORT, CHG_LED);
	}
	
	if(led & LED_R)
	{
		GPIO_ResetBits(LED_PORT, LED_R);
	}
	
	if(led & LED_G)
	{
		GPIO_ResetBits(LED_PORT, LED_G);
	}
	
	if(led & LED_B)
	{
		GPIO_ResetBits(LED_PORT, LED_B);
	}
}

void power_on()
{
	GPIO_SetBits(PWR_PORT, PWR_HOLD);
}

void power_off()
{
	GPIO_ResetBits(PWR_PORT, PWR_HOLD);
}

void motor_on(uint16_t delay)
{
	GPIO_SetBits(MOTOR_PORT, MOTOR_CON);
	TIM4_Configuration(delay);
}

void led_flicker(GPIO_TypeDef* GPIOx, uint16_t led_pin, uint32_t fre, uint8_t time)
{
	if(time == 0)
	{
		while(1)
		{
			GPIO_SetBits(GPIOx, led_pin);
			delay_ms(fre);
			GPIO_ResetBits(GPIOx, led_pin);
			delay_ms(fre);
		}
	}
	else
	{
		while(time--)
		{
			GPIO_SetBits(GPIOx, led_pin);
			delay_ms(fre);
			GPIO_ResetBits(GPIOx, led_pin);
			delay_ms(fre);
		}
	}
}

void pn532_power_on(void)
{
	GPIO_SetBits(NFC_PORT, NFC_RSTPDN);
}

void pn532_hard_power_down(void)
{
	GPIO_ResetBits(NFC_PORT, NFC_RSTPDN);
}

/*read half data from flah*/
uint16_t read_half_word_data(uint32_t addr)
{
	return *(uint16_t *)addr;
}

void BSP_Init(void)
{
	BSP_NVIC_Configuration();
	BSP_GPIO_Configuration();
	power_on();
	BSP_USART_Configuration();
	TIM2_Int_Init(9999,(SystemCoreClock / 10000) - 1);
	TIM3_Configuration();
	EXTI3_Configuration(EXTI_Trigger_Falling);
	EXTI15_Configuration();
	DMA_Configuration();
	ADC_Configuration();
//	RTC_Configuration();
	
	pn532_power_on();
	
	OLED_Init();
//	display_init();
	OLED_ShowLogo();
//	OLED_ShowSurface(); after nfc read
	
	xprintf("\r\nPheripheral Initial Success...\r\n");
}
