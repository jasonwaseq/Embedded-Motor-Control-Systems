/*
 * Lab3_Part2_DS3658.c
 *
 * ECE118 Lab 3, Part 2 - Unidirectional DC motor drive.
 *
 * Reads a potentiometer on V3, maps the reading to a 0-100% PWM duty cycle on
 * X11, and mirrors the speed command on the 12 LED bar. The PWM output drives
 * the DS3658 input; the motor high side is powered from the protected motor
 * supply path described in the lab.
 */

#include <xc.h>
#include "BOARD.h"
#include "AD.h"
#include "LED.h"
#include "pwm.h"

#define POT_PIN       AD_PORTV3
#define MOTOR_PWM     PWM_PORTX11
#define MOTOR_PWM_HZ  PWM_1KHZ

#define ADC_MAX       1023
#define NUM_LEDS      12

static unsigned int adc_to_duty(unsigned int adc)
{
    return ((unsigned long)adc * MAX_PWM + (ADC_MAX / 2)) / ADC_MAX;
}

static void update_leds(unsigned int adc)
{
    unsigned int count = ((unsigned long)adc * NUM_LEDS + (ADC_MAX / 2)) / ADC_MAX;
    unsigned char masks[3];

    if (count > NUM_LEDS) {
        count = NUM_LEDS;
    }

    for (int bank = 0; bank < 3; bank++) {
        unsigned int leds_this_bank = (count > 4) ? 4 : count;
        masks[bank] = (leds_this_bank == 4) ? 0x0F :
                (unsigned char)((1u << leds_this_bank) - 1u);
        count -= leds_this_bank;
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

    LED_Init();
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);

    PWM_Init();
    PWM_SetFrequency(MOTOR_PWM_HZ);
    PWM_AddPins(MOTOR_PWM);
    PWM_SetDutyCycle(MOTOR_PWM, MIN_PWM);

    while (1) {
        if (AD_IsNewDataReady()) {
            unsigned int adc = AD_ReadADPin(POT_PIN);
            PWM_SetDutyCycle(MOTOR_PWM, adc_to_duty(adc));
            update_leds(adc);
        }
    }

    return 0;
}
