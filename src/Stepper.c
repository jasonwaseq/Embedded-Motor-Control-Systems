/*
 * Stepper.c
 *
 * Timer3 driven stepper motor module for ECE118 Lab 3. The module can drive a
 * bipolar stepper directly through an L298 H-bridge using full-step, wave, or
 * half-step sequencing. The public API matches the provided course starter.
 */

#include <Stepper.h>
#include <xc.h>
#include <sys/attribs.h>

#ifndef F_CPU
#define F_CPU          80000000L
#define F_PB           (F_CPU / 2)
#define F_PB_DIV8      (F_PB / 8)
#define F_PB_DIV64     (F_PB / 64)
#define ONE_KHZ_RATE   ((F_PB_DIV8 - 1) / 1000)
#endif

#define MED_HZ_RATE            77
#define LOW_HZ_RATE            10
#define ONE_HUNDRED_HZ         100
#define TWENTY_KILOHERTZ       20000
#define DEFAULT_STEP_RATE      ONE_HUNDRED_HZ

#define ShutDownDrive()        (COIL_A_ENABLE = 0, COIL_B_ENABLE = 0)
#define TurnOnDrive()          (COIL_A_ENABLE = 1, COIL_B_ENABLE = 1)

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

static enum {
    step_one,
    step_two,
    step_three,
    step_four,
    step_five,
    step_six,
    step_seven,
    step_eight
} coilState = step_one;

static uint32_t CalculateOverflowPeriod(uint16_t rate);
static void DriveCoils(uint8_t aEnable, uint8_t aForward,
        uint8_t bEnable, uint8_t bForward);
static void FullStepDrive(void);
static void WaveDrive(void);
static void HalfStepDrive(void);

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
    coilState = step_one;

    ShutDownDrive();
    DriveCoils(TRUE, TRUE, TRUE, TRUE);

    TRIS_COIL_A_DIRECTION = 0;
    TRIS_COIL_A_DIRECTION_INV = 0;
    TRIS_COIL_A_ENABLE = 0;
    TRIS_COIL_B_DIRECTION = 0;
    TRIS_COIL_B_DIRECTION_INV = 0;
    TRIS_COIL_B_ENABLE = 0;

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

    if (stepperState == off || rate > TWENTY_KILOHERTZ) {
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
    if (stepperState == off || steps <= 0) {
        return ERROR;
    }
    if ((direction == FORWARD) || (direction == REVERSE)) {
        stepDir = direction;
        stepCount = steps;
        return SUCCESS;
    }
    return ERROR;
}

int8_t Stepper_InitSteps(uint8_t direction, int32_t steps)
{
    int8_t result;

    result = Stepper_SetSteps(direction, steps);
    if (result == ERROR) {
        return result;
    }
    return Stepper_StartSteps();
}

int8_t Stepper_StartSteps(void)
{
    if ((stepCount <= 0) || (stepperState == stepping) || (stepperState == off)) {
        return ERROR;
    }
    stepperState = stepping;
    TurnOnDrive();
    return SUCCESS;
}

int8_t Stepper_StopsSteps(void)
{
    if ((stepperState == off) || (stepperState == halted)) {
        return ERROR;
    }
    stepperState = halted;
    ShutDownDrive();
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
    ShutDownDrive();

    TRIS_COIL_A_DIRECTION = 1;
    TRIS_COIL_A_DIRECTION_INV = 1;
    TRIS_COIL_A_ENABLE = 1;
    TRIS_COIL_B_DIRECTION = 1;
    TRIS_COIL_B_DIRECTION_INV = 1;
    TRIS_COIL_B_ENABLE = 1;

    stepCount = 0;
    overflowReps = 0;
    stepDir = FORWARD;
    coilState = step_one;
    stepsPerSecondRate = DEFAULT_STEP_RATE;
    stepperState = off;

    return SUCCESS;
}

static uint32_t CalculateOverflowPeriod(uint16_t rate)
{
    if (rate == 0) {
        overflowReps = 2000;
        T3CONbits.TCKPS = 0b011;
        return ONE_KHZ_RATE;
    }
    if (rate > MED_HZ_RATE) {
        overflowReps = 0;
        T3CONbits.TCKPS = 0b011;
        return ((F_PB_DIV8 - 1) / rate);
    }
    if (rate > LOW_HZ_RATE) {
        overflowReps = (1000 - 1) / rate;
        T3CONbits.TCKPS = 0b011;
        return ONE_KHZ_RATE;
    }

    overflowReps = (1000 - 1) / rate + 1;
    T3CONbits.TCKPS = 0b011;
    return ONE_KHZ_RATE;
}

static void DriveCoils(uint8_t aEnable, uint8_t aForward,
        uint8_t bEnable, uint8_t bForward)
{
    COIL_A_ENABLE = aEnable ? 1 : 0;
    COIL_A_DIRECTION = aForward ? 1 : 0;
    COIL_A_DIRECTION_INV = aForward ? 0 : 1;

    COIL_B_ENABLE = bEnable ? 1 : 0;
    COIL_B_DIRECTION = bForward ? 1 : 0;
    COIL_B_DIRECTION_INV = bForward ? 0 : 1;
}

static void FullStepDrive(void)
{
    switch (coilState) {
    case step_one:
        DriveCoils(TRUE, TRUE, TRUE, TRUE);
        coilState = (stepDir == FORWARD) ? step_two : step_four;
        break;

    case step_two:
        DriveCoils(TRUE, TRUE, TRUE, FALSE);
        coilState = (stepDir == FORWARD) ? step_three : step_one;
        break;

    case step_three:
        DriveCoils(TRUE, FALSE, TRUE, FALSE);
        coilState = (stepDir == FORWARD) ? step_four : step_two;
        break;

    default:
        DriveCoils(TRUE, FALSE, TRUE, TRUE);
        coilState = (stepDir == FORWARD) ? step_one : step_three;
        break;
    }
}

static void WaveDrive(void)
{
    switch (coilState) {
    case step_one:
        DriveCoils(TRUE, TRUE, FALSE, TRUE);
        coilState = (stepDir == FORWARD) ? step_two : step_four;
        break;

    case step_two:
        DriveCoils(FALSE, TRUE, TRUE, TRUE);
        coilState = (stepDir == FORWARD) ? step_three : step_one;
        break;

    case step_three:
        DriveCoils(TRUE, FALSE, FALSE, TRUE);
        coilState = (stepDir == FORWARD) ? step_four : step_two;
        break;

    default:
        DriveCoils(FALSE, TRUE, TRUE, FALSE);
        coilState = (stepDir == FORWARD) ? step_one : step_three;
        break;
    }
}

static void HalfStepDrive(void)
{
    switch (coilState) {
    case step_one:
        DriveCoils(TRUE, TRUE, FALSE, TRUE);
        coilState = (stepDir == FORWARD) ? step_two : step_eight;
        break;

    case step_two:
        DriveCoils(TRUE, TRUE, TRUE, TRUE);
        coilState = (stepDir == FORWARD) ? step_three : step_one;
        break;

    case step_three:
        DriveCoils(FALSE, TRUE, TRUE, TRUE);
        coilState = (stepDir == FORWARD) ? step_four : step_two;
        break;

    case step_four:
        DriveCoils(TRUE, FALSE, TRUE, TRUE);
        coilState = (stepDir == FORWARD) ? step_five : step_three;
        break;

    case step_five:
        DriveCoils(TRUE, FALSE, FALSE, TRUE);
        coilState = (stepDir == FORWARD) ? step_six : step_four;
        break;

    case step_six:
        DriveCoils(TRUE, FALSE, TRUE, FALSE);
        coilState = (stepDir == FORWARD) ? step_seven : step_five;
        break;

    case step_seven:
        DriveCoils(FALSE, TRUE, TRUE, FALSE);
        coilState = (stepDir == FORWARD) ? step_eight : step_six;
        break;

    default:
        DriveCoils(TRUE, TRUE, TRUE, FALSE);
        coilState = (stepDir == FORWARD) ? step_one : step_seven;
        break;
    }
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
            ShutDownDrive();
            IEC0bits.T3IE = 0;
            break;

        case inited:
        case halted:
            if (stepCount < 0) {
                stepCount = 0;
            }
            break;

        case stepping:
            if (--stepCount <= 0) {
                stepCount = 0;
                stepperState = halted;
                ShutDownDrive();
                break;
            }

#ifdef FULL_STEP_DRIVE
            FullStepDrive();
#endif
#ifdef HALF_STEP_DRIVE
            HalfStepDrive();
#endif
#ifdef WAVE_DRIVE
            WaveDrive();
#endif
#ifdef DRV8811_DRIVE
#error "DRV8811 mode is not implemented in this direct H-bridge module"
#endif
            break;
        }
    }
    IFS0bits.T3IF = 0;
}
