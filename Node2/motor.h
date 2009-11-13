#include <avr/io.h>

#define KVAD_OE PORTA2
#define KVAD_RST PORTA3
#define KVAD_SEL PORTA4
#define MOTOR_EN PORTA5
#define MOTOR_DIR PORTA6




void motor_init(void);
void motor_reset(void);
void motor_set_reference(int8_t);
int8_t motor_get_position(void);
void motor_set_input(int8_t input);
void motor_regulator(void);

