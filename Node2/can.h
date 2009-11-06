#include "mcp2515.h"

void CAN_init(void);
int CAN_send(char*, int);
int CAN_test(void);
int CAN_receive(CAN_message*, int);
void CAN_init_interrupt(void);
void sig_interrupt4(void);
