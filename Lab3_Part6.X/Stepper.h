/*
 * Stepper.h
 *
 * Dedicated step/direction driver interface for ECE118 Lab 3 Part 6.
 *
 * Pin mapping:
 *   X3 -> STEP
 *   X4 -> DIR
 *   X5 -> EN
 *
 * If your board uses the inverse enable polarity, flip
 * STEPPER_ENABLE_ACTIVE_LEVEL below.
 */

#ifndef LAB3_PART6_STEPPER_H
#define LAB3_PART6_STEPPER_H

#include "BOARD.h"
#include "IO_Ports.h"

#define FORWARD 1
#define REVERSE 0

#define STEPPER_STEP_TRIS    PORTX03_TRIS
#define STEPPER_STEP_LAT     PORTX03_LAT
#define STEPPER_DIR_TRIS     PORTX04_TRIS
#define STEPPER_DIR_LAT      PORTX04_LAT
#define STEPPER_EN_TRIS      PORTX05_TRIS
#define STEPPER_EN_LAT       PORTX05_LAT

#define STEPPER_ENABLE_ACTIVE_LEVEL   1
#define STEPPER_ENABLE_INACTIVE_LEVEL 0

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
