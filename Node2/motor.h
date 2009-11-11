#include <avr/io.h>

void motor_init(void);
void motor_reset(void);
void motor_set_reference(int8_t);
int8_t motor_read_position(void);
void motor_set_input(int8_t input);
void motor_regulator(void);
