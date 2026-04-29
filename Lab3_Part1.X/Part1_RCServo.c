/*
 * Part1_RCServo.c
 *
 * ECE118 Lab 3, Part 1 – Driving an RC Servo
 *
 * Reads a potentiometer on AD_PORTW3, maps the 10-bit ADC reading to a servo
 * pulse width between MINPULSE and MAXPULSE, and mirrors the scaled value on
 * all three LED banks (12 LEDs total, lower = fewer lit).
 *
 * Hardware connections:
 *   Potentiometer wiper → V3  (analog input, voltage divider keeps 0–3.3 V)
 *   Servo signal        → V4  (RC_PORTV04, digital output 0–3.3 V PWM)
 *   Servo power         → 5 V rail (regulated, from ATX or LDO)
 *   Servo ground        → common GND
 *   LED banks 1–3       → all enabled (jumpers in place on I/O board)
 *
 * Timer4 is consumed internally by the RC_Servo module.
 */

#include <xc.h>
#include "BOARD.h"
#include "AD.h"
#include "LED.h"
#include "RC_Servo.h"

/* ADC full-scale (10-bit) */
#define ADC_MAX   1023

/* Number of LEDs across all three banks (4 per bank × 3 banks) */
#define NUM_LEDS  12

/*
 * Map a 10-bit ADC reading to a pulse width in µs.
 * Linear interpolation: MINPULSE at adc=0, MAXPULSE at adc=ADC_MAX.
 */
static unsigned short adc_to_pulse(unsigned int adc)
{
    return (unsigned short)(MINPULSE +
        ((unsigned long)(MAXPULSE - MINPULSE) * adc) / ADC_MAX);
}

/*
 * Display val (0–ADC_MAX) on the 12 LED bar.
 * Divides the range into 12 equal buckets; each bucket adds one LED.
 * Bank1 (red) = LEDs 0-3, Bank2 (yellow) = 4-7, Bank3 (green) = 8-11.
 * Bit 0 of each pattern is the innermost LED.
 */
static void update_leds(unsigned int adc)
{
    /* How many of the 12 LEDs should be on? */
    unsigned int count = (adc * NUM_LEDS) / (ADC_MAX + 1);  /* 0-11 */

    /* Build a 4-bit pattern for each bank.
     * count   bank1  bank2  bank3
     *   0      0000   0000   0000
     *   4      1111   0000   0000
     *   8      1111   1111   0000
     *  12      1111   1111   1111
     * Each bank gets min(count, 4) ones, starting from bit 0.            */
    unsigned char masks[3];
    unsigned int remaining = count;
    for (int b = 0; b < 3; b++) {
        unsigned int leds_this_bank = (remaining > 4) ? 4 : remaining;
        masks[b] = (leds_this_bank == 4) ? 0x0F : (unsigned char)((1u << leds_this_bank) - 1u);
        remaining -= leds_this_bank;
    }

    LED_SetBank(LED_BANK1, masks[0]);
    LED_SetBank(LED_BANK2, masks[1]);
    LED_SetBank(LED_BANK3, masks[2]);
}

int main(void)
{
    BOARD_Init();

    AD_Init();
    AD_AddPins(AD_PORTV3);

    LED_Init();
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);

    RC_Init();
    RC_AddPins(RC_PORTV04);

    while (1) {
        if (AD_IsNewDataReady()) {
            unsigned int adc = AD_ReadADPin(AD_PORTV3);
            RC_SetPulseTime(RC_PORTV04, adc_to_pulse(adc));
            update_leds(adc);
        }
    }

    return 0;
}
