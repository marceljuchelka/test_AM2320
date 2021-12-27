/*
 * mj_am2320b.c
 *
 *  Created on: 1. 10. 2020
 *      Author: marcel
 */


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mj_am2320b.h"
#include "driver/i2c.h"





int am2320_getdata(uint8_t select)
{
	uint16_t humid = 0, crc = 0, ret = 0;
	int16_t temp = 0;
	AM2320_VAR_T AM_val;
	uint8_t i = 0;
	int teplota;

	/*probuzeni sensoru 800us az 3ms  */
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (AM2320B_address) |I2C_MASTER_WRITE, I2C_MASTER_ACK);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 1 / portTICK_RATE_MS);			//odesli
	i2c_master_stop(cmd);
	vTaskDelay(1/portTICK_PERIOD_MS);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_RATE_MS)	;	//odesli

	/* nastaveni commandu nacti 4 byte */
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (AM2320B_address) |I2C_MASTER_WRITE, I2C_MASTER_ACK);
	i2c_master_write_byte(cmd, 0x03, I2C_MASTER_ACK);									//prikaz 3
	i2c_master_write_byte(cmd, 0x00, I2C_MASTER_ACK);									//od registru 0
	i2c_master_write_byte(cmd, 0x04, I2C_MASTER_ACK);									//4 byte
	i2c_master_stop(cmd);
	vTaskDelay(2/portTICK_PERIOD_MS);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 5 / portTICK_RATE_MS);					//proved

	vTaskDelay(2/portTICK_PERIOD_MS);

	/*nacti pozadovane byte ze zarizeni prvni dva byte jsou blbosti */
    i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (AM2320B_address) | I2C_MASTER_READ, I2C_MASTER_ACK);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_RATE_MS);
	i2c_master_read(cmd, &AM_val.AM_Bytes[0], 8, I2C_MASTER_ACK);
	i2c_master_stop(cmd);
	os_delay_us(30);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);


	for(i=0;i<8;i++){
		printf(" i=%d %X\n",i,AM_val.AM_Bytes[i]);
	}
	humid = AM_val.humid_hi;
	humid = (humid<<8) + AM_val.humid_lo;
	printf("humid_hi =%X\n",AM_val.humid_hi);
	printf("humid_lo =%X\n",AM_val.humid_lo);
	printf("humid =%d\n",humid);
	temp = AM_val.temp_hi;
	temp = (temp<<8) + AM_val.temp_lo;
	printf("temp_hi =%X\n",AM_val.temp_hi);
	printf("temp_lo =%X\n",AM_val.temp_lo);
	printf("temp =%d\n",temp);
	crc = AM_val.crc;
	printf("crc =%X\n",AM_val.crc);
	return humid/10;
	if (temp & 0x8000) teplota = 0 - (temp & 0x7fff);	// zaporna teplota?
	else {
		teplota = (int)temp;							//kladna teplota
	}
	if (select == temperat) {
		return teplota/10;			//navrat teploty / 10
	}
	else {
		return humid/10;								//navrat vlhkosti /10
	}
	return AM2320_ERROR;								//jinak navrat error
}

esp_err_t am2320_test_address(){
	esp_err_t chyba = 0;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (AM2320B_address) |I2C_MASTER_WRITE, I2C_MASTER_ACK);
	chyba = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return chyba;
}

unsigned short crc16(unsigned char *ptr, unsigned char len){
	 unsigned short crc =0xFFFF;
	 unsigned char i;
	 while(len--){
		 crc ^=*ptr++;
		 for(i=0;i<8;i++){
			 if(crc & 0x01){
				 crc>>=1;
				 crc^=0xA001;
			 }
			 else{
				 crc>>=1;
			 }
		 }
	 }
 return crc;
}

