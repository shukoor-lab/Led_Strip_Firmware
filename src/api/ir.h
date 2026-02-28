#ifndef IR_H_
#define IR_H_

#include <avr/io.h>

extern uint16_t IR1_value;
extern uint16_t IR2_value;

/* ===== API ===== */
void IR_Task(void);

#endif