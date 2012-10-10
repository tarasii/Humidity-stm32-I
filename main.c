#include "main.h"

static volatile uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

//configureADC
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;

//configureDMA
DMA_InitTypeDef DMA_InitStructure;

//acquireTemperatureData
CALIB_TypeDef calibdata;    /* field storing temp sensor calibration data */
volatile bool flag_ADCDMA_TransferComplete;
uint16_t ADC_ConvertedValueBuff[ADC_CONV_BUFF_SIZE];

uint32_t refAVG, tempAVG, preasureAVG;
int32_t temperature_C;
float voltage_V, preasure_V; 

volatile uint16_t systick_ms = 0;

float humidity, capacitance;

float temperature;
uint16_t temperature_data;
uint8_t idbuf[2][8];
uint8_t num_ow;

volatile uint16_t dirty_cycle = 0, period = 0; //, gpioa_state;

volatile uint8_t mode = 0, first_time_in_mode = 1, flag_UserButton = 0;

void SysTick_Handler(void) {
    TimingDelay_Decrement();
}

void EXTI0_IRQHandler(void)
{
  /* set UserButton Flag */
	flag_UserButton = 1;
  EXTI_ClearITPendingBit(EXTI_Line0);
}

void TIM2_IRQHandler(void)
{
  //if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  //{
    /* Даём знать, что обработали прерывание */
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

	  period = TIM_GetCapture1(TIM2);
    dirty_cycle = TIM_GetCapture2(TIM2);
	//}
}

void DMA1_Channel1_IRQHandler    (void)
{
  DMA_ClearFlag(DMA1_IT_TC1);
  setADCDMA_TransferComplete();  /* set flag_ADCDMA_TransferComplete global flag */
}



int main(void){

	uint8_t i, curx, buftime;
	int dt;
	char strDisp[25] ;
	uint32_t datetime;
	
	//static uint32_t toggle_ms = 0;
	RTC_DateTypeDef RTCDateStr;
	RTC_TimeTypeDef RTCTimeStr;
	
	uint8_t num_ow;
	
	RCC_Configuration();
	
	Init_GPIOs();
	
	Init_ext_GLCD_GPIOs();
		
	configureDMA();
	
	configureADC_Temp();	
	
	DAC_Config();
	
	Init_Ext_GLCD();
	
	GotoXY(0,0);
	Write_GLCD("HUMIDITY SENSOR");
	
	Init_tim_cnt();

	
	num_ow = OW_Scan((uint8_t *)idbuf, 2);
	
  OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ);
	
	//owSetDate((uint8_t *)idbuf[0]);
	
	datetime = owGetDate((uint8_t *) idbuf[0]);
	
	ConvertDateTime(datetime, &RTCTimeStr, &RTCDateStr);
	
	RTC_SetTime(RTC_Format_BIN, &RTCTimeStr);
	RTC_SetDate(RTC_Format_BIN, &RTCDateStr);
	
	
	while(1){
	
    if (flag_UserButton == 1){
			if(++mode == 3){mode = 0;}
      flag_UserButton = 0;
			first_time_in_mode = 1;
    }

// 		if (uint16_time_diff(systick_ms, toggle_ms) >= 500)
// 		{
// 			toggle_ms = systick_ms;
// 			if (stop==0) {GPIO_TOGGLE(LD_PORT,LD_BLUE);}	
// 		}
				
		if (period!=0){
			capacitance = period * 1000 / capasitive_coefficient;
			humidity = (capacitance-zero_capacitance)*100/60; 
			if(humidity > 100){humidity = 99;}
		}
 
		acquireTemperatureData();

 		while (!flag_ADCDMA_TransferComplete);
 				
 		processTempData();

 		RTC_GetTime(RTC_Format_BIN, &RTCTimeStr);
 		RTC_GetDate(RTC_Format_BIN, &RTCDateStr);

 		temperature_data = GetTemperature((uint8_t *) idbuf[1]); 
 		temperature = CalculateTemperature(temperature_data);		
		
		DAC_SetChannel1Data(DAC_Align_8b_R, 2048 + temperature_data);
		DAC_SetChannel2Data(DAC_Align_8b_R, 1024);
		
		switch (mode){
			case 0:				
				if (first_time_in_mode==1) {
					
					ClearFullScreen();
					first_time_in_mode = 0;
					
					GPIO_LOW(LD_PORT,LD_GREEN);
					GPIO_LOW(LD_PORT,LD_BLUE);
					
					GotoXY(0,3);				
					Write_GLCD((unsigned char *) "R");
					GotoXY(0,4);				
					Write_GLCD((unsigned char *) "H");
					GotoXY(0,6);				
					Write_GLCD((unsigned char *) "P");
					Rectangle(5,24,99,62);
					drawLine(5,40,99,40);
					
					curx=6; 
					buftime = 80;

				}
				
 				sprintf(strDisp, "%02d/%02d/%02d %02d:%02d:%02d", RTCDateStr.RTC_Year, RTCDateStr.RTC_Month, RTCDateStr.RTC_Date, RTCTimeStr.RTC_Hours, RTCTimeStr.RTC_Minutes, RTCTimeStr.RTC_Seconds);
 				GotoXY(0,0);
 				Write_GLCD((unsigned char *) strDisp);

 				sprintf(strDisp, "t=%2.1f°C RH=%2.1f%%", temperature, humidity);				
 				GotoXY(0,1);
 				Write_GLCD((unsigned char *) strDisp);
				
 				sprintf(strDisp, "P=%2.1fmmHg", preasure_V);
 				GotoXY(0,2);				
 				Write_GLCD((unsigned char *) strDisp);
				
				datetime = owGetDate((uint8_t *) idbuf[0]);

				dt = RTCTimeStr.RTC_Minutes - buftime;
				if(dt<0){dt=-dt;}
				if(dt>15){
					PutPixel(curx,61-(preasureAVG-997));
					PutPixel(curx,44-(humidity/5));
					buftime = RTCTimeStr.RTC_Minutes;
					if (++curx>98){
						curx=6;
						first_time_in_mode = 1;
					}
				}
				Delay(500);
				break;
			case 1:
				if (first_time_in_mode==1) {
					
					ClearFullScreen();
					first_time_in_mode = 0;
					
					GPIO_TOGGLE(LD_PORT,LD_GREEN);
				}
					
 				sprintf(strDisp, "%02d/%02d/%02d %02d:%02d:%02d", RTCDateStr.RTC_Year, RTCDateStr.RTC_Month, RTCDateStr.RTC_Date, RTCTimeStr.RTC_Hours, RTCTimeStr.RTC_Minutes, RTCTimeStr.RTC_Seconds);
 				GotoXY(0,0);
 				Write_GLCD((unsigned char *) strDisp);
					
				sprintf(strDisp, "n=%d %d %d", period, dirty_cycle,num_ow);				
				GotoXY(0,1);
				Write_GLCD((unsigned char *) strDisp);
				
				sprintf(strDisp, "C=%2.1fpf RH=%2.1f%%", capacitance, humidity);				
				GotoXY(0,2);
				Write_GLCD((unsigned char *) strDisp);
				
				sprintf(strDisp, "%x (%d) t=%2.1f°C", temperature_data, temperature_data, temperature);				
 				GotoXY(0,3);
 				Write_GLCD((unsigned char *) strDisp);
		
				sprintf(strDisp, "%d Vref=%2.2fV", refAVG, voltage_V);				
				GotoXY(0,4);
				Write_GLCD((unsigned char *) strDisp);
	
				sprintf(strDisp, "%d t(core)=%d°C", tempAVG, temperature_C);
				GotoXY(0,5);				
				Write_GLCD((unsigned char *) strDisp);
				
				sprintf(strDisp, "%d %2.1fmmHg", preasureAVG, preasure_V);
				GotoXY(0,6);				
				Write_GLCD((unsigned char *) strDisp);
				
				sprintf(strDisp, "%x %d", datetime, datetime);
				GotoXY(0,7);				
				Write_GLCD((unsigned char *) strDisp);
										
				break;
			case 2:
				if (first_time_in_mode==1) {
					
					ClearFullScreen();
					
					first_time_in_mode = 0;
					
					GPIO_TOGGLE(LD_PORT,LD_GREEN);
					GPIO_TOGGLE(LD_PORT,LD_BLUE);
					
				}
					
				for (i=0;i<2;i++){
					sprintf(strDisp, "ID=%02x%02x%02x%02x%02x%02x%02x%02x", idbuf[i][0], idbuf[i][1], idbuf[i][2], idbuf[i][3], idbuf[i][4], idbuf[i][5], idbuf[i][6], idbuf[i][7]);
					GotoXY(0,i);				
					Write_GLCD((unsigned char *) strDisp);
				}
				break;
		}			

	}
	
}

void acquireTemperatureData(void)
{
  
 /* Enable ADC clock */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* Enable DMA1 clock */
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  /* Enable ADC1 */
  //ADC_Cmd(ADC1, ENABLE);

  /* Wait until the ADC1 is ready */
  //while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET); 

  /* re-initialize DMA -- is it needed ?*/
  DMA_DeInit(DMA1_Channel1);
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* Enable DMA channel 1 Transmit complete interrupt*/
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

  /* Disable DMA mode for ADC1 */ 
  ADC_DMACmd(ADC1, DISABLE);

   /* Enable DMA mode for ADC1 */  
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Clear global flag for DMA transfert complete */
  clearADCDMA_TransferComplete(); 
  
  /* Start ADC conversion */
  ADC_SoftwareStartConv(ADC1);
}

void insertionSort(uint16_t *numbers, uint32_t array_size) 
{
  
	uint32_t i, j;
	uint32_t index;

  for (i=1; i < array_size; i++) {
    index = numbers[i];
    j = i;
    while ((j > 0) && (numbers[j-1] > index)) {
      numbers[j] = numbers[j-1];
      j = j - 1;
    }
    numbers[j] = index;
  }
}

uint32_t interquartileMean(uint16_t *array, uint32_t numOfSamples)
{
    uint32_t sum=0;
    uint32_t  index, maxindex;
    /* discard  the lowest and the highest data samples */ 
	maxindex = 3 * numOfSamples / 4;
    for (index = (numOfSamples / 4); index < maxindex; index++){
            sum += array[index];
    }
	/* return the mean value of the remaining samples value*/
    return ( sum / (numOfSamples / 2) );
}

void processTempData(void)
{
  uint32_t index, dataSum;

  /* sort received data in */
  insertionSort(ADC_ConvertedValueBuff, MAX_TEMP_CHNL);
  
  /* Calculate the Interquartile mean */
  //tempAVG = interquartileMean(ADC_ConvertedValueBuff, MAX_TEMP_CHNL);
	tempAVG = ADC_ConvertedValueBuff[0];

  //dataSum = 0;
  //for (index=0; index < MAX_TEMP_CHNL; index++){
  //  dataSum += ADC_ConvertedValueBuff[index];
  //}
  //tempAVG = dataSum / MAX_TEMP_CHNL ;
  //tempAVG = ADC_ConvertedValueBuff[1] ;
  
  dataSum = 0;
  /* Sum up all mesured data for reference temperature average calculation */ 
  for (index=MAX_TEMP_CHNL; index < ADC_CONV_BUFF_SIZE-4; index++){
    dataSum += ADC_ConvertedValueBuff[index];
  }
  /* Devide sum up result by 4 for the temperature average calculation*/
  preasureAVG = dataSum / 4 ;

  dataSum = 0;
  /* Sum up all mesured data for reference temperature average calculation */ 
  for (index=MAX_TEMP_CHNL+4; index < ADC_CONV_BUFF_SIZE; index++){
    dataSum += ADC_ConvertedValueBuff[index];
  }
  /* Devide sum up result by 4 for the temperature average calculation*/
  refAVG = dataSum / 4 ;


  /* Calculate temperature in °C from Interquartile mean */
  temperature_C = tempAVG - (int32_t) calibdata.TS_CAL_COLD;	
  temperature_C = temperature_C * (int32_t)(HOT_CAL_TEMP - COLD_CAL_TEMP);                      
  temperature_C = temperature_C / 
                  (int32_t)(calibdata.TS_CAL_HOT - calibdata.TS_CAL_COLD); 
  temperature_C = temperature_C + COLD_CAL_TEMP; 
	
  /* Calculate voltage in V from average */
  voltage_V = (VREF/refAVG) * ADC_CONV;

  /* Calculate preasure in mmHg */
  preasure_V = (preasure_ref * preasureAVG) / preasure_conv;
	
}

uint16_t GetTemperature(uint8_t *idbuf){
	uint16_t dirtytemp;
 	uint8_t buf[2];
 	uint8_t cmd[12];
 	uint8_t i;
	
// 	  OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ);
//   	Delay(2);
//    OW_Send(OW_SEND_RESET, "\xcc\xbe\xff\xff", 4, buf, 2, 2);
 	
	cmd[0]=0x55;
	for (i=1;i<9;i++)
	{
		cmd[i]=idbuf[i-1];
	}
	cmd[9]=0x44;
	
 	OW_Send(OW_SEND_RESET, cmd, 10, NULL, NULL, OW_NO_READ);
  
	Delay(10);
	
	cmd[9]=0xbe;
	cmd[10]=0xff;
	cmd[11]=0xff;
  OW_Send(OW_SEND_RESET, cmd, 12, buf, 2, 10);
	
	dirtytemp = buf[1]*0x100+buf[0];
	
	return dirtytemp;
}

uint32_t owGetDate(uint8_t *idbuf){
 	uint8_t cmd[10];
 	uint8_t i;
 	uint8_t buf[4];
 	
	cmd[0]=0x55;
	for (i=1;i<9;i++)
	{
		cmd[i]=idbuf[i-1];
	}
	
 	OW_Send(OW_SEND_RESET, cmd, 9, NULL, NULL, OW_NO_READ);

	Delay(10);
	
 	OW_Send(OW_NO_RESET, "\x66\xff\xff\xff\xff\xff", 6, buf, 4, 2);
  
	return buf[3]*0x1000000+buf[2]*0x10000+buf[1]*0x100+buf[0];
	//return (uint32_t) *buf;
}

void owSetDate(uint8_t *idbuf){
 	uint8_t cmd[15];
 	uint8_t i;
	
 	
	cmd[0]=0x55;
	for (i=1;i<9;i++)
	{
		cmd[i]=idbuf[i-1];
	}
	cmd[9]=0x99;
	cmd[10]=0x0c;
	cmd[11]=0xff;
	cmd[12]=0xef;
	cmd[13]=0x72;
	cmd[14]=0x50;
	
 	OW_Send(OW_SEND_RESET, cmd, 15, NULL, NULL, OW_NO_READ);
  
}

void ConvertDateTime(uint32_t datetime, RTC_TimeTypeDef* RTC_TimeStruct, RTC_DateTypeDef* RTC_DateStruct){
	uint32_t nd;
	uint16_t yy, yd, ds = 0;
	uint8_t mm;
	uint8_t ma[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	
 	nd = datetime / SEC_PER_DAY;
	
	yy = START_YEAR + nd / DAY_PER_YEAR;
	yy = yy - (yy / 100) * 100;
	
	if (yy % 4 == 0){ma[1]+=1;}
	
	yd = nd / DAY_PER_YEAR;
	yd = nd - (yd * DAY_PER_YEAR + yd / 4);
	
	for (mm=0;(ds<yd)&&(mm<12);mm++){
		ds += ma[mm];
	}
	
	RTC_DateStruct->RTC_Year = yy;
	RTC_DateStruct->RTC_Month = mm;
	RTC_DateStruct->RTC_Date = yd - (ds - ma[mm-1]) + 1;
	RTC_TimeStruct->RTC_Hours = (datetime % SEC_PER_DAY) / SEC_PER_HOUR;
	RTC_TimeStruct->RTC_Minutes = (datetime % SEC_PER_HOUR) / SEC_PER_MINUTE;
	RTC_TimeStruct->RTC_Seconds = datetime % SEC_PER_MINUTE;
}

float CalculateTemperature(uint16_t dirtytemp){
	float temp;
	if( dirtytemp > tempminusthreshold ) { 
		dirtytemp = tempminusconst - dirtytemp; 
		dirtytemp = -dirtytemp; 
	}
	temp = dirtytemp * temperature_resolution;
	if (temp>125){temp=0;}
	return temp;
}


void Init_tim_cnt(void){
	TIM_TimeBaseInitTypeDef TIM_InitBaseStructure;
 	TIM_ICInitTypeDef TIM_ICInitStruct;
  //NVIC_InitTypeDef NVIC_InitStructure; //create NVIC structure
	
	TIM_TimeBaseStructInit(&TIM_InitBaseStructure);
	TIM_InitBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM2, &TIM_InitBaseStructure);
	
  TIM_ICStructInit(&TIM_ICInitStruct);
  //TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
  //TIM_ICInit(TIM2, &TIM_ICInitStruct);
	TIM_PWMIConfig(TIM2, &TIM_ICInitStruct); 

	TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

  TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM2, ENABLE);
	
	NVIC_EnableIRQ(TIM2_IRQn);	
}

void DAC_Config(void)
{
  DAC_InitTypeDef DAC_InitStructure;

  /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

  /* DAC Channel1 Init */
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

void Init_ext_GLCD_GPIOs (void){
  GPIO_InitTypeDef GPIO_InitStructure;
         
/* Configure Output for LCD */
/* Port B */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;  
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_9 \
//                                  | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( GPIOB, &GPIO_InitStructure);
    
/* Configure Output for LCD */
/* Port C*/  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  ;                               
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init( GPIOC, &GPIO_InitStructure);  
	
} 

void RCC_Configuration(void){

  //SysTick_Config(SystemCoreClock / 4000);
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 2000);

  /* Enable the GPIOs Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);     
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_USART2 | RCC_APB1Periph_DAC, ENABLE);	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG, ENABLE);

	/* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

//   /* Reset Backup Domain */
//   RCC_RTCResetCmd(ENABLE);
//   RCC_RTCResetCmd(DISABLE);

  /* LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON); //do not touch LSE to prevent RTC calendar reset

  /* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
  
  RCC_RTCCLKCmd(ENABLE);
   
  /* LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //rtc 
}  


void Init_GPIOs (void){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  
//   /* USER button and WakeUP button init: GPIO set in input interrupt active mode */
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure User Button pin as input */
  GPIO_InitStructure.GPIO_Pin = USER_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);

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

/* Configure the GPIO_LED pins  LD3 & LD4*/
  GPIO_InitStructure.GPIO_Pin = LD_GREEN|LD_BLUE;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(LD_PORT, &GPIO_InitStructure);
  GPIO_LOW(LD_PORT,LD_GREEN);	
  GPIO_LOW(LD_PORT,LD_BLUE);
  
/* ADC input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3  ;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  

/* TIM input humidity */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1  ;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5  ;                               
//   //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);
//   GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
	
/* USART input-output temperature */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);

/* ADC input */
  GPIO_InitStructure.GPIO_Pin = IDD_MEASURE  ;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init( IDD_MEASURE_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3  ; //preasure metr                              
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init( GPIOC, &GPIO_InitStructure);  

  /* Configure PA.04 (DAC_OUT1), PA.05 (DAC_OUT2) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
} 

FunctionalState  testUserCalibData(void)
{
  int32_t testdiff;
  FunctionalState retval = DISABLE;
  
  testdiff = USER_CALIB_DATA->TS_CAL_HOT - USER_CALIB_DATA->TS_CAL_COLD;
  
  if ( testdiff > TEST_CALIB_DIFF )    retval = ENABLE;
  
  return retval;
}

FunctionalState  testFactoryCalibData(void)
{
  int32_t testdiff;
  FunctionalState retval = DISABLE;
  
  testdiff = FACTORY_CALIB_DATA->TS_CAL_HOT - FACTORY_CALIB_DATA->TS_CAL_COLD;
  
  if ( testdiff > TEST_CALIB_DIFF )    retval = ENABLE;
  
  return retval;
}

void  writeCalibData(CALIB_TypeDef* calibStruct)
{
	__IO FLASH_Status FLASHStatus = FLASH_COMPLETE; 

  uint32_t  Address = 0;
  uint32_t  dataToWrite;
  
  /* Unlock the FLASH PECR register and Data EEPROM memory */
  DATA_EEPROM_Unlock();
  
  /* Clear all pending flags */      
  FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                  | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);	
  
  /*  Data EEPROM Fast Word program of FAST_DATA_32 at addresses defined by 
      DATA_EEPROM_START_ADDR and DATA_EEPROM_END_ADDR */  
 
  Address = (uint32_t) USER_CALIB_DATA;


  dataToWrite = 0x00;
  dataToWrite = (uint32_t)(calibStruct->TS_CAL_COLD) << 16;
  
  FLASHStatus = DATA_EEPROM_ProgramWord(Address, dataToWrite);

  if(FLASHStatus != FLASH_COMPLETE)
  {
    while(1); /* stay in loop in case of crittical programming error */
  }

  Address += 4;

  dataToWrite = 0x00;
  dataToWrite = (uint32_t)(calibStruct->TS_CAL_HOT) << 16;
  
  FLASHStatus = DATA_EEPROM_ProgramWord(Address, dataToWrite);
  
}

void configureADC_Temp(void)
{
  uint32_t ch_index;
	__IO uint16_t 	T_StartupTimeDelay;

  /* Test user or factory temperature sensor calibration value */
  if ( testUserCalibData() == ENABLE ) calibdata = *USER_CALIB_DATA;
  else if ( testFactoryCalibData() == ENABLE ) calibdata = *FACTORY_CALIB_DATA;
  else {
    calibdata.TS_CAL_COLD = DEFAULT_COLD_VAL;
    calibdata.TS_CAL_HOT = DEFAULT_HOT_VAL;
    writeCalibData(&calibdata);
    calibdata = *USER_CALIB_DATA;
  }

  /* Enable ADC clock & SYSCFG */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Enable the internal connection of Temperature sensor and with the ADC channels*/
  ADC_TempSensorVrefintCmd(ENABLE); 
  
  /* Wait until ADC + Temp sensor start */
  T_StartupTimeDelay = 1024;
  while (T_StartupTimeDelay--);

  /* Setup ADC common init struct */
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInit(&ADC_CommonInitStructure);
  
  
  /* Initialise the ADC1 by using its init structure */
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	          // Set conversion resolution to 12bit
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;	                          // Enable Scan mode (single conversion for each channel of the group)
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			  // Disable Continuous conversion
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; // Disable external conversion trigger
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  // Set conversion data alignement to right
  ADC_InitStructure.ADC_NbrOfConversion = ADC_CONV_BUFF_SIZE;             // Set conversion data alignement to ADC_CONV_BUFF_SIZE
  //ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular Temperature sensor channel16 and internal reference channel17 configuration */ 

    for (ch_index = 1; ch_index <= MAX_TEMP_CHNL; ch_index++){
      ADC_RegularChannelConfig(ADC1, ADC_Channel_16, ch_index, 
                               ADC_SampleTime_384Cycles);
    }

  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 13, ADC_SampleTime_384Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 14, ADC_SampleTime_384Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 15, ADC_SampleTime_384Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 16, ADC_SampleTime_384Cycles);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 17, ADC_SampleTime_384Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 18, ADC_SampleTime_384Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 19, ADC_SampleTime_384Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 20, ADC_SampleTime_384Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Wait until the ADC1 is ready */
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET); 
		
}

void configureDMA(void)
{
  /* Declare NVIC init Structure */
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable DMA1 clock */
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* De-initialise  DMA */
  DMA_DeInit(DMA1_Channel1);
  
  //DMA_InitTypeDef DMA_InitStructure;
	
	/* DMA1 channel1 configuration */
  DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);	     // Set DMA channel Peripheral base address to ADC Data register
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValueBuff;  // Set DMA channel Memeory base addr to ADC_ConvertedValueBuff address
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                         // Set DMA channel direction to peripheral to memory
  DMA_InitStructure.DMA_BufferSize = ADC_CONV_BUFF_SIZE;                     // Set DMA channel buffersize to peripheral to ADC_CONV_BUFF_SIZE
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	     // Disable DMA channel Peripheral address auto increment
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    // Enable Memeory increment (To be verified ....)
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// set Peripheral data size to 8bit 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	     // set Memeory data size to 8bit 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                              // Set DMA in normal mode
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                     // Set DMA channel priority to High
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                               // Disable memory to memory option 
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);								 // Use Init structure to initialise channel1 (channel linked to ADC)

  /* Enable Transmit Complete Interrup for DMA channel 1 */ 
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
  
  /* Setup NVIC for DMA channel 1 interrupt request */
  NVIC_InitStructure.NVIC_IRQChannel =   DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}

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

void setADCDMA_TransferComplete(void)
{
  flag_ADCDMA_TransferComplete = TRUE;
}

void clearADCDMA_TransferComplete(void)
{
  flag_ADCDMA_TransferComplete = FALSE;
}


