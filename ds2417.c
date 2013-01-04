#include "ds2417.h"

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
