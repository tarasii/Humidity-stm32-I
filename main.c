#include "main.h"

float preasure_V, altTemp_V; 

float humidity, capacitance;
float temperature;

volatile uint16_t dirty_cycle = 0, period = 0; 

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
  //if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
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

	uint8_t i, curx, buftime ;
	int dt; uint32_t cury, rd;
	char strDisp[25]; uint8_t dhtbuf[5];
	//uint32_t datetime;
	
	//static uint32_t toggle_ms = 0;
	
	rtc_init();
	
	SysTick_conf();
	
	button_init_irq();
	led_init();
	
	USART_open(USART1, 9600);
	USART_open(OW_USART, 115200);
				
	ADC_init();	
	
	//dac_init(DAC_Channel_1);
	
	Init_Ext_GLCD();
	
	GotoXY(0,0);
	Write_GLCD("HUMIDITY SENSOR");

	tim_init_pwm_cnt(GPIOA, GPIO_Pin_1);
  
	ow_num = OW_Scan((uint8_t *)ow_idbuf, owdevnum);
	
  OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ);
	
	Delay(200);
	

	//owSetDate((uint8_t *)idbuf[0]);
	
	//datetime = owGetDate((uint8_t *) idbuf[0]);
	//ConvertDateTime(datetime, &RTCTimeStr, &RTCDateStr);
	
	//RTC_SetTime(RTC_Format_BIN, &RTCTimeStr);
	//RTC_SetDate(RTC_Format_BIN, &RTCDateStr);
	
	
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
 
		ADC_AcquireData();

 		while (!flag_ADCDMA_TransferComplete);
 				
 		ADC_ProcessData();
		preasure_V = (preasure_ref * ADC_RES.Chanel13AVG) / preasure_conv;	
		altTemp_V = (ADC_RES.voltage_V * ADC_RES.Chanel04AVG * 100)/ ADC_CONV;

 		RTC_GetTime(RTC_Format_BIN, &RTCTimeStr);
 		RTC_GetDate(RTC_Format_BIN, &RTCDateStr);

 		//temperature_data = GetTemperature((uint8_t *) idbuf[1]); 
 		//temperature_data = GetTemperature((uint8_t *) idbuf[0]); 
 		//temperature = CalculateTemperature(temperature_data);
		
		for (ow_i = 0; ow_i<ow_num; ow_i++){
			ow_raw[ow_i] = GetTemperature((uint8_t *) ow_idbuf[ow_i]);
		}
 		temperature = CalculateTemperature(ow_raw[0]);		
		
		DHT11_RawRead(dhtbuf);
		
		//DAC_SetChannel1Data(DAC_Align_12b_R, 160+temperature*3);
		//DAC_SetChannel2Data(DAC_Align_8b_R, 1024);

		if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET)
		{
			rd = USART_ReceiveData(USART1);
			rd = USART_ReceiveData(USART1);
			Delay(100);
			if (rd == 122) 
			{				
				sprintf(strDisp, "begin;%2.1f;%2.1f;", temperature, altTemp_V);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));		
				sprintf(strDisp, "%d.%d;%d.%d;", dhtbuf[2], dhtbuf[3], dhtbuf[0], dhtbuf[1]);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));		
				sprintf(strDisp, "%2.1f;%2.1f;end\n", humidity, preasure_V);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
			}
			else
			{
				sprintf(strDisp, "UART=%d;", rd);		
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "V_ref_RAW=%d;", ADC_RES.refAVG);		
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "Vref=%2.2fV;", ADC_RES.voltage_V);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "t_RAW_ds=%d;", ow_raw[0]);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "t_ds=%2.1fC;", temperature);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "t_RAW_lm=%d;", ADC_RES.Chanel04AVG);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "t_lm=%2.1fC;", altTemp_V);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));		
				sprintf(strDisp, "t_dht=%2d.%dC;", dhtbuf[2], dhtbuf[3]);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "RH_dht=%2d.%d%%;", dhtbuf[0], dhtbuf[1]);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "n_RH=%d;", period);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "C_RH=%2.1fpf;", capacitance);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "RH=%2.1f%%;", humidity);				
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "P_RAW=%d;", ADC_RES.Chanel13AVG);
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				sprintf(strDisp, "P=%2.1fmmHg.\n", preasure_V);
				USART_DMA_send(USART1, strDisp, strlen(strDisp));
				}				
			}
			
 		if (uint16_time_diff(systick_ms, toggle_ms) >= 500)
 		{
 			toggle_ms = systick_ms;
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
						//drawLine(5,40,99,40);
						
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
					
					//datetime = owGetDate((uint8_t *) idbuf[0]);

					dt = RTCTimeStr.RTC_Minutes - buftime;
					if(dt<0){dt=-dt;}
					if(dt>15){				
						if (ADC_RES.Chanel13AVG<988) cury=0; else cury = ADC_RES.Chanel13AVG-988;
						//if (cury>20) cury=20;
						PutPixel(curx,61 - cury);
						PutPixel(curx,44-(humidity/5));
						buftime = RTCTimeStr.RTC_Minutes;
						if (++curx>98){
							curx=6;
							first_time_in_mode = 1;
						}
					}
					
						
					//Delay(300);
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
						
					sprintf(strDisp, "n=%d %d %d %2.1f", period, dirty_cycle, ow_num, altTemp_V);				
					GotoXY(0,1);
					Write_GLCD((unsigned char *) strDisp);
					
					sprintf(strDisp, "C=%2.1fpf RH=%2.1f%%", capacitance, humidity);				
					GotoXY(0,2);
					Write_GLCD((unsigned char *) strDisp);
					
					sprintf(strDisp, "%x (%d) t=%2.1f°C", ow_raw[0], ow_raw[0], temperature);				
					GotoXY(0,3);
					Write_GLCD((unsigned char *) strDisp);
			
					sprintf(strDisp, "%d Vref=%2.2fV", ADC_RES.refAVG, ADC_RES.voltage_V);				
					GotoXY(0,4);
					Write_GLCD((unsigned char *) strDisp);
		
					sprintf(strDisp, "%d t(core)=%d°C", ADC_RES.tempAVG, ADC_RES.temperature_C);
					GotoXY(0,5);				
					Write_GLCD((unsigned char *) strDisp);
					
					sprintf(strDisp, "%d P=%2.1fmmHg", ADC_RES.Chanel13AVG, preasure_V);
					GotoXY(0,6);				
					Write_GLCD((unsigned char *) strDisp);
									
					//sprintf(strDisp, "%x %d", datetime, datetime);
					sprintf(strDisp, "dht t=%2dC RH=%2d%%", dhtbuf[2], dhtbuf[0]);				
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
						
					for (i=0;i<owdevnum;i++){
						sprintf(strDisp, "ID=%02x%02x%02x%02x%02x%02x%02x%02x", ow_idbuf[i][0], ow_idbuf[i][1], ow_idbuf[i][2], ow_idbuf[i][3], ow_idbuf[i][4], ow_idbuf[i][5], ow_idbuf[i][6], ow_idbuf[i][7]);
						GotoXY(0,i);				
						Write_GLCD((unsigned char *) strDisp);
					}
					sprintf(strDisp, "dht %02x%02x%02x%02x", dhtbuf[0], dhtbuf[1], dhtbuf[2], dhtbuf[3]);				
					GotoXY(0,i);				
					Write_GLCD((unsigned char *) strDisp);
				break;
			}			

		}
	}
}


