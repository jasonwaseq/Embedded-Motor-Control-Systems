/*
 * Stepper.c
 *
 * Timer3-driven STEP/DIR/EN stepper module for the dedicated driver board
 * used in ECE118 Lab 3 Part 6.
 */

#include <xc.h>
#include <sys/attribs.h>
#include "Stepper.h"

#ifndef F_CPU
#define F_CPU          80000000L
#define F_PB           (F_CPU / 2)
#define F_PB_DIV8      (F_PB / 8)
#define ONE_KHZ_RATE   ((F_PB_DIV8 - 1) / 1000)
#endif

#define MED_HZ_RATE       77
#define LOW_HZ_RATE       10
#define DEFAULT_STEP_RATE 100
#define MAX_STEP_RATE     20000

#define DRIVER_DISABLE()  (STEPPER_EN_LAT = STEPPER_ENABLE_INACTIVE_LEVEL)
#define DRIVER_ENABLE()   (STEPPER_EN_LAT = STEPPER_ENABLE_ACTIVE_LEVEL)

static int32_t stepCount = 0;
static uint32_t overflowReps = 0;
static uint8_t stepDir = FORWARD;
static uint16_t stepsPerSecondRate = DEFAULT_STEP_RATE;

static enum {
    off,
    inited,
    stepping,
    halted
} stepperState = off;

static uint32_t CalculateOverflowPeriod(uint16_t rate);
static void EmitStepPulse(void);

int8_t Stepper_Init(void)
{
    uint16_t overflowPeriod;

    if (stepperState != off) {
        return ERROR;
    }

    stepCount = 0;
    overflowReps = 0;
    stepDir = FORWARD;
    stepsPerSecondRate = DEFAULT_STEP_RATE;

    STEPPER_STEP_TRIS = 0;
    STEPPER_DIR_TRIS = 0;
    STEPPER_EN_TRIS = 0;

    STEPPER_STEP_LAT = 0;
    STEPPER_DIR_LAT = 0;
    DRIVER_DISABLE();

    overflowPeriod = CalculateOverflowPeriod(stepsPerSecondRate);

    T3CON = 0;
    T3CONbits.TCKPS = 0b011;
    TMR3 = 0;
    PR3 = overflowPeriod;

    IFS0bits.T3IF = 0;
    IPC3bits.T3IP = 3;
    IEC0bits.T3IE = 1;
    T3CONbits.ON = 1;

    stepperState = inited;
    return SUCCESS;
}

int8_t Stepper_SetRate(uint16_t rate)
{
    uint16_t overflowPeriod;

    if ((stepperState == off) || (rate > MAX_STEP_RATE)) {
        return ERROR;
    }

    T3CONbits.ON = 0;
    overflowPeriod = CalculateOverflowPeriod(rate);
    PR3 = overflowPeriod;
    TMR3 = 0;
    stepsPerSecondRate = rate;

    if (stepperState != halted) {
        T3CONbits.ON = 1;
    }
    return SUCCESS;
}

uint16_t Stepper_GetRate(void)
{
    return stepsPerSecondRate;
}

int8_t Stepper_SetSteps(uint8_t direction, int32_t steps)
{
    if ((stepperState == off) || (steps <= 0)) {
        return ERROR;
    }
    if ((direction != FORWARD) && (direction != REVERSE)) {
        return ERROR;
    }

    stepDir = direction;
    stepCount = steps;
    return SUCCESS;
}

int8_t Stepper_InitSteps(uint8_t direction, int32_t steps)
{
    if (Stepper_SetSteps(direction, steps) == ERROR) {
        return ERROR;
    }
    return Stepper_StartSteps();
}

int8_t Stepper_StartSteps(void)
{
    if ((stepperState == off) || (stepperState == stepping) || (stepCount <= 0)) {
        return ERROR;
    }

    DRIVER_ENABLE();
    stepperState = stepping;
    return SUCCESS;
}

int8_t Stepper_StopsSteps(void)
{
    if ((stepperState == off) || (stepperState == halted)) {
        return ERROR;
    }

    DRIVER_DISABLE();
    STEPPER_STEP_LAT = 0;
    stepperState = halted;
    return SUCCESS;
}

int32_t Stepper_GetRemainingSteps(void)
{
    return stepCount;
}

int8_t Stepper_GetDirection(void)
{
    return stepDir;
}

int8_t Stepper_IsStepping(void)
{
    return (stepperState == stepping);
}

int8_t Stepper_End(void)
{
    if (stepperState == off) {
        return ERROR;
    }

    T3CONbits.ON = 0;
    IEC0bits.T3IE = 0;

    DRIVER_DISABLE();
    STEPPER_STEP_LAT = 0;
    STEPPER_DIR_LAT = 0;

    STEPPER_STEP_TRIS = 1;
    STEPPER_DIR_TRIS = 1;
    STEPPER_EN_TRIS = 1;

    stepCount = 0;
    overflowReps = 0;
    stepDir = FORWARD;
    stepsPerSecondRate = DEFAULT_STEP_RATE;
    stepperState = off;
    return SUCCESS;
}

static uint32_t CalculateOverflowPeriod(uint16_t rate)
{
    if (rate == 0) {
        overflowReps = 2000;
        return ONE_KHZ_RATE;
    }
    if (rate > MED_HZ_RATE) {
        overflowReps = 0;
        return ((F_PB_DIV8 - 1) / rate);
    }
    if (rate > LOW_HZ_RATE) {
        overflowReps = (1000 - 1) / rate;
        return ONE_KHZ_RATE;
    }

    overflowReps = (1000 - 1) / rate + 1;
    return ONE_KHZ_RATE;
}

static void EmitStepPulse(void)
{
    STEPPER_DIR_LAT = (stepDir == FORWARD) ? 1 : 0;
    STEPPER_STEP_LAT = 1;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    STEPPER_STEP_LAT = 0;
}

void __ISR(_TIMER_3_VECTOR, IPL3SOFT) Timer3IntHandler(void)
{
    static uint16_t timerLoopCount = 0;

    timerLoopCount++;
    if (timerLoopCount > overflowReps) {
        timerLoopCount = 0;

        switch (stepperState) {
        case off:
            T3CONbits.ON = 0;
            DRIVER_DISABLE();
            IEC0bits.T3IE = 0;
            break;

        case inited:
        case halted:
            if (stepCount < 0) {
                stepCount = 0;
            }
            break;

        case stepping:
            if (stepCount > 0) {
                EmitStepPulse();
                stepCount--;
            }
            if (stepCount <= 0) {
                stepCount = 0;
                DRIVER_DISABLE();
                stepperState = halted;
            }
            break;
        }
    }

    IFS0bits.T3IF = 0;
}
