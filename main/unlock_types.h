#ifndef UNLOCK_TYPES_H
#define UNLOCK_TYPES_H

#include <stdint.h>

typedef enum {
    UNLOCK_NONE = 0,
    UNLOCK_BASIC = 1,
    UNLOCK_ADVANCED = 2
} unlock_type_t;

typedef struct {
    unlock_type_t unlock_type;
    uint8_t uid[8];
    uint8_t uid_len;
} unlock_msg_t;

#endif // UNLOCK_TYPES_H
