#ifndef PWM_H
#define PWM_H

#include "types.h"

// Support functions for driving a servo motor. Run init first with the desired
// channel number. Position is set to (pos % max_pos) so you can't damage the
// servo by forcing it to a position too high.

void pwm_init(u8 channel);
void pwm_set(u8 channel, u8 pos); 

#define MAX_POS 25

#endif