#include "delay.h"

__IO uint32_t TimingDelay;
__IO uint16_t systick_ms = 0, toggle_ms = 0;

uint16_t uint16_time_diff(uint16_t now, uint16_t before)
{
  return (now >= before) ? (now - before) : (UINT16_MAX - before + now);
}

void Delay(uint32_t nTime){
  TimingDelay = nTime;

  while(TimingDelay != 0);
  
}

void TimingDelay_Decrement(void){

  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
	
	++systick_ms;
}

void SysTick_conf(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
  //SysTick_Config(SystemCoreClock / 4000);
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 2000);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

