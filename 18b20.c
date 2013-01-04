#include "18b20.h"

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

uint16_t GetTemperature(uint8_t *idbuf){
	uint16_t dirtytemp;
 	uint8_t buf[2];
 	uint8_t cmd[12];
 	uint8_t i;
	 	
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

uint16_t GetSingleTemperature(void){
	uint16_t dirtytemp;
 	uint8_t buf[2];
	
 	OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ);
  Delay(2);
  OW_Send(OW_SEND_RESET, "\xcc\xbe\xff\xff", 4, buf, 2, 2);
 		
	dirtytemp = buf[1]*0x100+buf[0];
	
	return dirtytemp;
}

