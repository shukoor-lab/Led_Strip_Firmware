#ifndef IR_H_
#define IR_H_

#include <avr/io.h>

#define DOOR_TASK_INTERVAL 100

typedef enum {
    DOOR_CLOSED,
    DOOR_OPENING,
    DOOR_OPEN,
    DOOR_CLOSING,
    DOOR_USER_MODE
} door_state_t;

extern door_state_t door_state;

/* ===== API ===== */
void IR_Task(void);

void Door_Task(void);

#endif