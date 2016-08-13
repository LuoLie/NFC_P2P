/************************************************************
  Copyright (C), 2014-2015, Giayee Tech. Co., Ltd.
  FileName: bsp.h
  Author: Giayee       Version : V1.0        Date:15/07/15
  Description:       
    This file provides all battery information functions.
  User can call function of get_bat_value() to get battery
  information.
***********************************************************/

#ifndef __BSP_H__
#define __BSP_H__

#include "stm32f10x.h"

/*USART1 Pin*/
#define USART1_PORT					GPIOA
#define USART1_TX					GPIO_Pin_9
#define USART1_RX					GPIO_Pin_10
#define	RCC_APB2Periph_GPIO_USART1	RCC_APB2Periph_USART1

/*USART2 Pin*/
#define USART2_PORT					GPIOA
#define USART2_TX					GPIO_Pin_2
#define USART2_RX					GPIO_Pin_3

/*LED pin*/
#define LED_PORT					GPIOB
#define LED_R						GPIO_Pin_6
#define LED_G						GPIO_Pin_8
#define LED_B						GPIO_Pin_9

/*OLED Pin*/
#define OLED_PORT					GPIOB
#define OLED_RST_PORT				GPIOA
#define LCD_RST						GPIO_Pin_8
#define SPI_CS						GPIO_Pin_12
#define SPI_CLK						GPIO_Pin_13
#define SPI_SI						GPIO_Pin_15

/*pn532 pin*/
#define NFC_PORT					GPIOA
#define NFC_RSTPDN					GPIO_Pin_1
#define NFC_UART_RX					GPIO_Pin_2
#define NFC_UART_TX					GPIO_Pin_3
#define NFC_IRQ						GPIO_Pin_5
#define NFC_P32						GPIO_Pin_6

/*power pin*/
#define PWR_PORT					GPIOB
#define PWR_HOLD					GPIO_Pin_11
#define PWR_KEY						GPIO_Pin_3

/*MOTOR pin*/
#define MOTOR_PORT					GPIOB
#define MOTOR_CON					GPIO_Pin_1

/*charge pin*/
#define CHG_PORT					GPIOA
#define	CHG_LED						GPIO_Pin_7
#define CHG_STA						GPIO_Pin_15

/*adc pin*/
#define BAT_PORT					GPIOB
#define BAT_ADC						GPIO_Pin_0


void EXTI3_Configuration(EXTITrigger_TypeDef );
void TIM2_Int_Init(u16 arr,u16 psc);

/*************************************************
  Function:       led_on
  Description:    light up the led
  Input:          led_pin: CHG_LED | LED_R | LED_G | LED_B              
  Output:         None
  Return:		  None
*************************************************/
void led_on(uint16_t led_pin);

/*************************************************
  Function:       led_off
  Description:    light off the led
  Input:          led_pin: CHG_LED | LED_R | LED_G | LED_B              
  Output:         None
  Return:		  None
*************************************************/
void led_off(uint16_t led_pin);

/*************************************************
  Function:       chg_led_on
  Description:    turn on the led that represent it's charging
  Input:          None              
  Output:         None
  Return:		  None
*************************************************/
void chg_led_on(void);

/*************************************************
  Function:       chg_led_off
  Description:    turn off the led that represent it's charged full
  Input:          None              
  Output:         None
  Return:		  None
*************************************************/
void chg_led_off(void);

/*************************************************
  Function:       power_on
  Description:    turn on the nfc watch
  Input:          None              
  Output:         None
  Return:		  None
*************************************************/
void power_on(void);

/*************************************************
  Function:       power_off
  Description:    turn off the nfc watch
  Input:          None              
  Output:         None
  Return:		  None
*************************************************/
void power_off(void);
	  
/*************************************************
  Function:       motor_on
  Description:    turn on the motor for a delay time
  Input:          delay: time to delay (Uint:ms)  Range:(1~(65535/2))           
  Output:         None
  Return:		  None
*************************************************/
void motor_on(uint16_t delay);

/*************************************************
  Function:       BSP_Init
  Description:    initialize the NVIC, GPIO, USART, TIM2, TIM3, EXTI3, 
				  EXTI15, DMA, ADC, delay_init(72000000), power_on();
				  OLED_Init(), display_init(), OLED_ShowLogo();
				  OLED_ShowSurface().
  Input:          None              
  Output:         None
  Return:		  None
*************************************************/
void BSP_Init(void);

/*************************************************
  Function:       led_flicker
  Description:    To a certain period of flashing LED 
  Input:          GPIO_TypeDef*: LED_PORT | CHG_PORT   
				  led_pin: LED_R | LED_G | LED_B | CHG_LED
				  fre:Flicker frequency (Uint:ms)
				  time:Flicker period
  Output:         None
  Return:		  None
*************************************************/
void led_flicker(GPIO_TypeDef* GPIOx, uint16_t led_pin, uint32_t fre, uint8_t time);

/*************************************************
  Function:       pn532_power_on
  Description:    switch on pn532
  Input:          None              
  Output:         None
  Return:		  None
*************************************************/
void pn532_power_on(void);

/*************************************************
  Function:       pn532_hard_power_down
  Description:    hard power down the pn532
  Input:          None              
  Output:         None
  Return:		  None
*************************************************/
void pn532_hard_power_down(void);

void close_tim2_IT(void);
void open_tim2_IT(void);

#endif
