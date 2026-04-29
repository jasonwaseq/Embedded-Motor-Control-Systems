/*
 * Lab3_Part4_L298_DC.c
 *
 * ECE118 Lab 3, Part 4 - Bidirectional DC motor control with the L298.
 *
 * V3 supplies speed command, W4 is an active-low direction switch, X11 drives
 * the L298 enable pin with PWM, and Z3/Z5 drive one L298 input pair.
 */

#include <xc.h>
#include "BOARD.h"
#include "AD.h"
#include "IO_Ports.h"
#include "LED.h"
#include "pwm.h"

#define POT_PIN       AD_PORTV3
#define DIR_SWITCH    PORTW04_BIT
#define DIR_SWITCH_TRIS PORTW04_TRIS

#define MOTOR_PWM     PWM_PORTX11
#define MOTOR_PWM_HZ  PWM_1KHZ

#define L298_IN1      PORTZ03_LAT
#define L298_IN2      PORTZ05_LAT
#define L298_IN1_TRIS PORTZ03_TRIS
#define L298_IN2_TRIS PORTZ05_TRIS

#define ADC_MAX       1023
#define NUM_LEDS      12
#define FORWARD       1
#define REVERSE       0

static unsigned int adc_to_duty(unsigned int adc)
{
    return ((unsigned long)adc * MAX_PWM + (ADC_MAX / 2)) / ADC_MAX;
}

static uint8_t read_forward_switch(void)
{
    return DIR_SWITCH ? FORWARD : REVERSE;
}

static void set_motor_direction(uint8_t direction)
{
    if (direction == FORWARD) {
        L298_IN1 = 1;
        L298_IN2 = 0;
    } else {
        L298_IN1 = 0;
        L298_IN2 = 1;
    }
}

static void update_leds(unsigned int duty, uint8_t direction)
{
    unsigned int count = ((unsigned long)duty * NUM_LEDS + (MAX_PWM / 2)) / MAX_PWM;
    unsigned char masks[3] = {0, 0, 0};

    if (count > NUM_LEDS) {
        count = NUM_LEDS;
    }

    for (unsigned int led = 0; led < count; led++) {
        unsigned int index = (direction == FORWARD) ? led : (NUM_LEDS - 1 - led);
        masks[index / 4] |= (unsigned char)(1u << (index % 4));
    }

    LED_SetBank(LED_BANK1, masks[0]);
    LED_SetBank(LED_BANK2, masks[1]);
    LED_SetBank(LED_BANK3, masks[2]);
}

int main(void)
{
    BOARD_Init();

    AD_Init();
    AD_AddPins(POT_PIN);

    DIR_SWITCH_TRIS = 1;
    L298_IN1_TRIS = 0;
    L298_IN2_TRIS = 0;
    set_motor_direction(FORWARD);

    LED_Init();
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);

    PWM_Init();
    PWM_SetFrequency(MOTOR_PWM_HZ);
    PWM_AddPins(MOTOR_PWM);
    PWM_SetDutyCycle(MOTOR_PWM, MIN_PWM);

    while (1) {
        if (AD_IsNewDataReady()) {
            unsigned int adc = AD_ReadADPin(POT_PIN);
            unsigned int duty = adc_to_duty(adc);
            uint8_t direction = read_forward_switch();

            set_motor_direction(direction);
            PWM_SetDutyCycle(MOTOR_PWM, duty);
            update_leds(duty, direction);
        }
    }

    return 0;
}
