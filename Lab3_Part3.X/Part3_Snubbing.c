/*
 * Lab3_Part3_Snubbing.c
 *
 * ECE118 Lab 3, Part 3 - Snubbing inductive kickback.
 *
 * The snubber measurements use the same DS3658 software drive as Part 2 while
 * the hardware snubbing network is changed. V3 commands duty cycle, X11 outputs
 * PWM to the DS3658 input, and the LEDs show the commanded duty.
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

/* Set to 1 for scope captures at a fixed duty, then rebuild. */
#define USE_FIXED_DUTY 0
#define FIXED_DUTY     500

static unsigned int adc_to_duty(unsigned int adc)
{
    return ((unsigned long)adc * MAX_PWM + (ADC_MAX / 2)) / ADC_MAX;
}

static void update_leds_from_duty(unsigned int duty)
{
    unsigned int count = ((unsigned long)duty * NUM_LEDS + (MAX_PWM / 2)) / MAX_PWM;
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
            unsigned int duty;

#if USE_FIXED_DUTY
            (void)AD_ReadADPin(POT_PIN);
            duty = FIXED_DUTY;
#else
            duty = adc_to_duty(AD_ReadADPin(POT_PIN));
#endif

            PWM_SetDutyCycle(MOTOR_PWM, duty);
            update_leds_from_duty(duty);
        }
    }

    return 0;
}
