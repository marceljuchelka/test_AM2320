/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
#include "../components/mylib/mylib.h"
#include "../components/MJ_AM2320B/mj_am2320b.h"
#include "main.h"

#define led_pin_mb	2




void app_main()
{
	uint8_t uroven = 0;
	my_i2c_config();
//	funkce_test();
	gpio_set_direction(led_pin_mb, GPIO_MODE_OUTPUT);
	while(1){
		uroven^= 1;
		gpio_set_level(led_pin_mb, uroven);
		vTaskDelay(3000/portTICK_PERIOD_MS);
		printf("stav %d \n",uroven);
		printf("teplota = %d \n", am2320_getdata(temperat));
//		printf("vlhkost = %X \n", am2320_getdata(humidy));
//		printf("test AM2320 = %d \n", am2320_test_address());
	}


}

static void my_i2c_config(){
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = I2C_SDA_PIN;
    conf.sda_pullup_en = 1;
	conf.scl_io_num = I2C_SCL_PIN;
    conf.scl_pullup_en = 1;
	conf.clk_stretch_tick = 100000;
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, conf.mode));
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
}
