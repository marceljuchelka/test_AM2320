/*
 * main.h
 *
 *  Created on: 24. 12. 2021
 *      Author: marcel
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#include "driver/i2c.h"

/* deklarace */
static void my_i2c_config();


#define I2C_SCL_PIN         	14               /*!< gpio number for I2C master clock */
#define I2C_SDA_PIN        		12               /*!< gpio number for I2C master data  */
//#define I2C_1_MASTER_NUM            	I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_TX_BUF_DISABLE   	0                /*!< I2C master do not need buffer */
#define I2C_RX_BUF_DISABLE   	0                /*!< I2C master do not need buffer */
#define test



#endif /* MAIN_MAIN_H_ */
