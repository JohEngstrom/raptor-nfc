#ifndef I2C_SLAVE_UNLOCK_H
#define I2C_SLAVE_UNLOCK_H

#include <stdint.h>
#include <stddef.h>
#include "unlock_types.h"  // If you have your unlock_msg_t here

void i2c_slave_init(void);
void i2c_slave_send_unlock_basic(const uint8_t *uid, uint8_t uid_len);

#endif
