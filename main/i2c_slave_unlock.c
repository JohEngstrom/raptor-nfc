#include "driver/i2c.h"
#include "i2c_slave_unlock.h"
#include <string.h>

// Adjust for your setup:
#define I2C_SLAVE_NUM         I2C_NUM_0
#define I2C_SLAVE_SDA_IO      13
#define I2C_SLAVE_SCL_IO      14
#define I2C_SLAVE_ADDR        0x28
#define I2C_SLAVE_RX_BUF_LEN  128
#define I2C_SLAVE_TX_BUF_LEN  128

static unlock_msg_t last_unlock_msg;

void i2c_slave_init(void)
{
    i2c_config_t conf_slave = {
        .mode = I2C_MODE_SLAVE,
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .slave = {
            .addr_10bit_en = 0,
            .slave_addr = I2C_SLAVE_ADDR
        }
    };
    i2c_param_config(I2C_SLAVE_NUM, &conf_slave);
    i2c_driver_install(I2C_SLAVE_NUM, I2C_MODE_SLAVE, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}

void i2c_slave_send_unlock_basic(const uint8_t *uid, uint8_t uid_len)
{
    memset(&last_unlock_msg, 0, sizeof(last_unlock_msg));
    last_unlock_msg.unlock_type = UNLOCK_BASIC;
    last_unlock_msg.uid_len = uid_len > sizeof(last_unlock_msg.uid) ? sizeof(last_unlock_msg.uid) : uid_len;
    memcpy(last_unlock_msg.uid, uid, last_unlock_msg.uid_len);

    // Write to the TX buffer so the master can read it
    i2c_slave_write_buffer(I2C_SLAVE_NUM, (uint8_t *)&last_unlock_msg, sizeof(last_unlock_msg), 100 / portTICK_PERIOD_MS);
}
