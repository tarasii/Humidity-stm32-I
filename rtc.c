#include "rtc.h"

RTC_DateTypeDef RTCDateStr;
RTC_TimeTypeDef RTCTimeStr;

void rtc_init(void){

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

//void rtc_example(void){
//	char strDisp[25];
//	RTC_DateTypeDef RTCDateStr;
//	RTC_TimeTypeDef RTCTimeStr;
//	//to set time
//	RTCDateStr.RTC_Year = 15;
//	RTCDateStr.RTC_Month = 1;
//	RTCDateStr.RTC_Date = 1;
//	RTCTimeStr.RTC_Hours = 12;
//	RTCTimeStr.RTC_Minutes = 0;
//	RTCTimeStr.RTC_Seconds = 0;
//	RTC_SetTime(RTC_Format_BIN, &RTCTimeStr);
//	RTC_SetDate(RTC_Format_BIN, &RTCDateStr);
//	//to get time
//	RTC_GetTime(RTC_Format_BIN, &RTCTimeStr);
//	RTC_GetDate(RTC_Format_BIN, &RTCDateStr);
//	printf(strDisp, "%02d/%02d/%02d %02d:%02d:%02d", RTCDateStr.RTC_Year, RTCDateStr.RTC_Month, RTCDateStr.RTC_Date, RTCTimeStr.RTC_Hours, RTCTimeStr.RTC_Minutes, RTCTimeStr.RTC_Seconds);
//}
