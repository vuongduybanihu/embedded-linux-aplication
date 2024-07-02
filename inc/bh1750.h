#ifndef BH1750_H_
#define BH1750_H_

#include "bh1750.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

// Định nghĩa địa chỉ I2C của cảm biến BH1750
#define BH1750_ADDR 0x23

void bh1750_send_command(uint8_t cmd);

float bh1750_read_data();
// Hàm để khởi tạo cảm biến BH1750
int bh1750_init();

// Hàm để đọc giá trị ánh sáng từ cảm biến BH1750 (trả về giá trị lux)
float bh1750_read();

#endif /* BH1750_H_ */

