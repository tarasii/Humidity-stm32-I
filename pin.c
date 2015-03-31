#include "pin.h"

void pin_mode(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, uint8_t GPIO_Pin_Mode){

  uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;
	
  uint32_t	GPIO_Mode;
  uint16_t	GPIO_OType;
  uint32_t	GPIO_PuPd;
  uint32_t	GPIO_Speed;
	
	GPIO_Mode = GPIO_Pin_Mode & 0x03;
	GPIO_Speed = (GPIO_Pin_Mode >> 2) & 0x03;
	GPIO_OType = (GPIO_Pin_Mode >> 4) & 0x03;
	GPIO_PuPd = (GPIO_Pin_Mode >> 6) & 0x03;
	
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

	if (GPIOx == GPIOA) {
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	}else if (GPIOx == GPIOB) {
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	}else	if (GPIOx == GPIOC) {
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	}
	
  /* -------------------------Configure the port pins---------------- */
  /*-- GPIO Mode Configuration --*/
  for (pinpos = 0x00; pinpos < 0x10; pinpos++)
  {
    pos = ((uint32_t)0x01) << pinpos;

    /* Get the port pins position */
    currentpin = GPIO_Pin_x & pos;

    if (currentpin == pos)
    {
      GPIOx->MODER  &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
      GPIOx->MODER |= (GPIO_Mode << (pinpos * 2));

      if ((GPIO_Mode == 0x01) || (GPIO_Mode == 0x02))
      {
        /* Speed mode configuration */
        GPIOx->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pinpos * 2));
        GPIOx->OSPEEDR |= (GPIO_Speed << (pinpos * 2));

        /* Output mode configuration */
        GPIOx->OTYPER  &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)pinpos)) ;
        GPIOx->OTYPER |= (uint16_t)(GPIO_OType << ((uint16_t)pinpos));
      }

      /* Pull-up Pull down resistor configuration */
      GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
      GPIOx->PUPDR |= (GPIO_PuPd << (pinpos * 2));
    }
  }

}

void init_RCC_for_all_GPIO(void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);     
}

void led_init(void){

	pin_mode(LD_PORT, LD_GREEN|LD_BLUE, GPIO_MODE_OUT2_PP);
  GPIO_LOW(LD_PORT, LD_GREEN);	
  GPIO_LOW(LD_PORT, LD_BLUE);
	
}

void button_init (void){
	pin_mode(BUTTON_GPIO_PORT, USER_GPIO_PIN, GPIO_MODE_IN);
}

void button_init_irq (void){
   /* USER button and WakeUP button init: GPIO set in input interrupt active mode */
	
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	button_init();
	
  /* Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

  /* Configure User Button and IDD_WakeUP EXTI line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 ;  // PA0 for User button AND IDD_WakeUP
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set User Button and IDD_WakeUP EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure); 

} 

