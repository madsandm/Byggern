#ifndef CONTROLLER_H
#define CONTROLLER_H

#define CONTROLLER_P 1
#define CONTROLLER_I 0
#define CONTROLLER_D 0

#define CONTROLLER_INTERVAL_MS 30

void motorController_init();
void motorController_setTarget(int target);

#endif // CONTROLLER_H