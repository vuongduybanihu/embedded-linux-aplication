#include "bh1750.h"

#define BH1750_CONVERSION_TIME_MS 180

// Biến toàn cục để lưu địa chỉ thiết bị I2C
int bh1750_i2c_file;

// Hàm để gửi yêu cầu đọc dữ liệu từ cảm biến BH1750
void bh1750_send_command(uint8_t cmd) {
    if (write(bh1750_i2c_file, &cmd, 1) != 1) {
        perror("Failed to write to the I2C bus");
        exit(1);
    }
}

// Hàm để đọc dữ liệu từ cảm biến BH1750
float bh1750_read_data() {
    uint8_t data[2];
    if (read(bh1750_i2c_file, data, 2) != 2) {
        perror("Failed to read from the I2C bus");
        exit(1);
    }
    int light_level = (data[0] << 8) + data[1];
    return (float)light_level / 1.2;
}

// Hàm để khởi tạo cảm biến BH1750
int bh1750_init() {
    char *i2c_device = "/dev/i2c-1";
    if ((bh1750_i2c_file = open(i2c_device, O_RDWR)) < 0) {
        perror("Failed to open the I2C bus");
        exit(1);
    }
    if (ioctl(bh1750_i2c_file, I2C_SLAVE, BH1750_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        exit(1);
    }
    return bh1750_i2c_file;
}

// Hàm để đọc giá trị ánh sáng từ cảm biến BH1750
float bh1750_read() {
    // Gửi yêu cầu đo ánh sáng
    bh1750_send_command(0x10); // Chế độ chuyển đổi liên tục
    usleep(BH1750_CONVERSION_TIME_MS * 1000); // Chờ kết quả

    // Đọc dữ liệu từ cảm biến
    return bh1750_read_data();
}

