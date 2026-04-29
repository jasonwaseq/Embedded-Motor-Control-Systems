/*
 * Stepper.h
 *
 * ECE118 Lab 3 stepper driver.
 *
 * The direct-drive modes use the L298 H-bridge wiring from the lab handout:
 *   A enable    -> Z4
 *   A direction -> Z3
 *   A inverse   -> Z12
 *   B enable    -> Z8
 *   B direction -> Z7
 *   B inverse   -> Z10
 *
 * Select exactly one drive mode below before building.
 */

#ifndef STEPPER_H
#define STEPPER_H

#include <BOARD.h>

/*******************************************************************************
 * STEPPER MODE #DEFINES
 ******************************************************************************/

#define FULL_STEP_DRIVE
/* #define HALF_STEP_DRIVE */
/* #define WAVE_DRIVE */
/* #define DRV8811_DRIVE */

#if defined FULL_STEP_DRIVE && (defined HALF_STEP_DRIVE || defined WAVE_DRIVE || defined DRV8811_DRIVE)
#error "Define only one stepper drive mode at a time"
#endif

#if defined HALF_STEP_DRIVE && (defined FULL_STEP_DRIVE || defined WAVE_DRIVE || defined DRV8811_DRIVE)
#error "Define only one stepper drive mode at a time"
#endif

#if defined WAVE_DRIVE && (defined HALF_STEP_DRIVE || defined FULL_STEP_DRIVE || defined DRV8811_DRIVE)
#error "Define only one stepper drive mode at a time"
#endif

#if defined DRV8811_DRIVE && (defined HALF_STEP_DRIVE || defined FULL_STEP_DRIVE || defined WAVE_DRIVE)
#error "Define only one stepper drive mode at a time"
#endif

/*******************************************************************************
 * PUBLIC #DEFINES
 ******************************************************************************/

#define FORWARD 1
#define REVERSE 0

#define TRIS_COIL_A_ENABLE        TRISFbits.TRISF1
#define TRIS_COIL_A_DIRECTION     TRISEbits.TRISE4
#define TRIS_COIL_A_DIRECTION_INV TRISFbits.TRISF2
#define TRIS_COIL_B_ENABLE        TRISDbits.TRISD8
#define TRIS_COIL_B_DIRECTION     TRISEbits.TRISE2
#define TRIS_COIL_B_DIRECTION_INV TRISFbits.TRISF3

#define COIL_A_ENABLE        LATFbits.LATF1
#define COIL_A_DIRECTION     LATEbits.LATE4
#define COIL_A_DIRECTION_INV LATFbits.LATF2
#define COIL_B_ENABLE        LATDbits.LATD8
#define COIL_B_DIRECTION     LATEbits.LATE2
#define COIL_B_DIRECTION_INV LATFbits.LATF3

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 ******************************************************************************/

int8_t Stepper_Init(void);
int8_t Stepper_SetRate(uint16_t rate);
uint16_t Stepper_GetRate(void);
int8_t Stepper_SetSteps(uint8_t direction, int32_t steps);
int8_t Stepper_InitSteps(uint8_t direction, int32_t steps);
int8_t Stepper_StartSteps(void);
int8_t Stepper_StopsSteps(void);
int32_t Stepper_GetRemainingSteps(void);
int8_t Stepper_GetDirection(void);
int8_t Stepper_IsStepping(void);
int8_t Stepper_End(void);

#endif
