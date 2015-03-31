#include "dht11.h"

uint16_t read_cycle(uint16_t cur_tics, uint8_t neg_tic){
	uint16_t cnt_tics;
 	if (cur_tics < MAX_TICS) cnt_tics = 0;
	if (neg_tic){
		while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)&&(cnt_tics<MAX_TICS)){
			cnt_tics++;
		}
	}
	else {
		while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)&&(cnt_tics<MAX_TICS)){
			cnt_tics++;
		}
	}
 	return cnt_tics;
}

uint8_t DHT11_RawRead(uint8_t *buf){
//  GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t dt[42];
	uint16_t cnt;
	uint8_t i, check_sum; 
	
//  GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
	pin_mode(DHT11_PORT, DHT11_PIN, GPIO_MODE_OUT2_PP);
	

	//reset DHT11
	Delay(500);
 	GPIO_LOW(DHT11_PORT,DHT11_PIN);
	Delay(20);
 	GPIO_HIGH(DHT11_PORT,DHT11_PIN);
	
//  GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//  GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
	pin_mode(DHT11_PORT, DHT11_PIN, GPIO_MODE_IN);
	
  //start reading	
 	cnt = 0; 
	for(i=0;i<83 && cnt<MAX_TICS;i++){
		if (i & 1){
			cnt = read_cycle(cnt, 1);
		}
		else {
			cnt = read_cycle(cnt, 0);
			dt[i/2]= cnt;
		}
	}
	
 	//release line
	GPIO_HIGH(DHT11_PORT, DHT11_PIN);

	for (i = 0; i<5; i++) buf[i]=0;
	
	if (cnt>=MAX_TICS) return DHT11_NO_CONN;
	
	//convert data
 	for(i=2;i<42;i++){
		(*buf) <<= 1;
  	if (dt[i]>20) {
			(*buf)++;
 		}
		if (!((i-1)%8) && (i>2)) {
			buf++;
		}
 	}
	
	//calculate checksum
	buf -= 5;
	check_sum = 0;
 	for(i=0;i<4;i++){
		check_sum += *buf;
		buf++;
	}
	
	if (*buf != check_sum) return DHT11_CS_ERROR;
				
	return DHT11_OK;	
	//return check_sum;
}

uint8_t DHT11_pwm_Read(uint8_t *buf, uint32_t *dt, uint32_t *cnt){
	uint8_t i, check_sum; 
	
	*cnt = 0;
	for (i = 0; i<43; i++) dt[i]=0;

	pin_mode(DHT11_PORT, DHT11_PIN, GPIO_MODE_OUT2_PP);

	GPIO_LOW(DHT11_PORT,	DHT11_PIN);
	Delay(20);
	GPIO_HIGH(DHT11_PORT,	DHT11_PIN);
	tim_init_pwm_cnt(DHT11_PORT,	DHT11_PIN);
	Delay(20);

	//pin_mode(TIM2_GPIO, TIM2_CH1, GPIO_MODE_OUT2_PP);
	//GPIO_HIGH(TIM2_GPIO,	TIM2_CH1);

	for (i = 0; i<5; i++) buf[i]=0;
		
	if (*cnt==0) return DHT11_NO_CONN;
	
	//convert data
 	for(i=3;i<42;i++){
		(*buf) <<= 1;
  	if (dt[i]>2000) {
			(*buf)++;
 		}
		if (!((i-2)%8) && (i>3)) {
			buf++;
		}
 	}
	
	//calculate checksum
	buf -= 5;
	check_sum = 0;
 	for(i=0;i<4;i++){
		check_sum += *buf;
		buf++;
	}
	
	if (*buf != check_sum) return DHT11_CS_ERROR;
				
	return DHT11_OK;	
	//return check_sum;
}


float DHT22_Humidity(uint8_t *buf){
	float res;
	res = buf[0] * 256 + buf[1];
	res /= 10.0;
	return res;
}

float DHT22_Temperature(uint8_t *buf){
	float res;
	res = (buf[2] & 0x7F)* 256 + buf[3];
  res /= 10.0;
  if (buf[2] & 0x80) res *= -1;
	return res;
}

uint8_t DHT11_Humidity(uint8_t *buf){
	return buf[0];
}

uint8_t DHT11_Temperature(uint8_t *buf){
	return buf[2];;
}

