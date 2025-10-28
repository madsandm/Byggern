#include "controller.h"
#include "decoder.h"
#include "motor_driver.h"

int motorTarget = 0;
int prevError = 0;
int errorIntegrated = 0;

void motorController_calibrate() {
    motor_driver_set_vel(-500);
    printf("Calibrating motor...\n");

    for(int i = 0; i < 2000; i++) {}

    int lastReading = 10000;
    int currentReading;

    do {
        lastReading = (currentReading * 10 + lastReading * 90) / 100;
        currentReading = decoder_read();
        for (int i = 0; i < 100000; i++) {}
    } while (lastReading > currentReading + 10);

    motor_driver_set_vel(0);
    decoder_setnullpoint();
    printf("Motor calibration finished\n");
}

void motorController_init() {
    motorController_calibrate();
    // Disable writeprotection
    PMC->PMC_WPMR = (0x504D43 << 8);
    TC0->TC_WPMR = (0x54494D << 8);

    // Enable peripheral clock
    PMC->PMC_PCER0 |= (1 << ID_TC0); 

    // Configure timer
    TC0->TC_CHANNEL[0].TC_CMR = 
        TC_CMR_TCCLKS_TIMER_CLOCK2    // MCK/8
        | TC_CMR_WAVE
        | TC_CMR_WAVSEL_UP_RC;          // Reset counter when RC is reached
    
    TC0->TC_CHANNEL[0].TC_RC = 10500 * CONTROLLER_INTERVAL_MS; // Set the interval

    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;   // Interrupt on RC compare
    NVIC_EnableIRQ(TC0_IRQn);

    // Start the timer
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}


void motorControl_loop() {
    int meassurement = decoder_read() / 3;
    int error = motorTarget - meassurement;

    int controlVariable = (CONTROLLER_P * error
        + CONTROLLER_I * errorIntegrated * CONTROLLER_INTERVAL_MS
        + CONTROLLER_D * (error - prevError) / CONTROLLER_INTERVAL_MS) / 100;

    errorIntegrated += error;
    prevError = error;

    printf("y=%d r=%d u=%d\n", meassurement, motorTarget, controlVariable);

    motor_driver_set_vel(controlVariable);
}

void TC0_Handler(void)
{
    TC0->TC_CHANNEL[0].TC_SR; // clear interrupt flag
    motorControl_loop();
}

void motorController_setTarget(int target) {
    motorTarget = target;
}