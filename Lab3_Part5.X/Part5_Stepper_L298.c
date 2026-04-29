/*
 * Lab3_Part5_Stepper_L298.c
 *
 * ECE118 Lab 3, Part 5 - Stepper control through the L298 H-bridge.
 *
 * The Stepper module sequences the four L298 coil-control signals on Port Z.
 * V3 controls step rate, W4 is an active-low direction switch, and the LEDs
 * show rate plus direction by filling from opposite ends of the bar.
 */

#include <xc.h>
#include "BOARD.h"
#include "AD.h"
#include "IO_Ports.h"
#include "LED.h"
#include "Stepper.h"

#define POT_PIN          AD_PORTV3
#define DIR_SWITCH       PORTW04_BIT
#define DIR_SWITCH_TRIS  PORTW04_TRIS

#define ADC_MAX          1023
#define NUM_LEDS         12
#define RATE_DEADBAND    8
#define MIN_STEP_RATE_HZ 5
#define MAX_STEP_RATE_HZ 600
#define STEP_CHUNK       200

static uint16_t adc_to_rate(unsigned int adc)
{
    if (adc <= RATE_DEADBAND) {
        return 0;
    }
    return (uint16_t)(MIN_STEP_RATE_HZ +
            ((unsigned long)(MAX_STEP_RATE_HZ - MIN_STEP_RATE_HZ) * adc) / ADC_MAX);
}

static uint8_t read_forward_switch(void)
{
    return DIR_SWITCH ? FORWARD : REVERSE;
}

static void update_leds(uint16_t rate, uint8_t direction)
{
    unsigned int count;
    unsigned char masks[3] = {0, 0, 0};

    if (rate == 0) {
        count = 0;
    } else {
        count = ((unsigned long)rate * NUM_LEDS +
                (MAX_STEP_RATE_HZ / 2)) / MAX_STEP_RATE_HZ;
    }
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
    uint8_t lastDirection = FORWARD;

    BOARD_Init();

    /*
     * Part 5 now uses Z10 and Z12 for coil-drive signals. Those pins overlap
     * the FTDI UART pins, so disable UART1 after BOARD_Init() to return them
     * to GPIO control for the stepper bridge.
     */
    IEC0bits.U1RXIE = 0;
    IEC0bits.U1TXIE = 0;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
    U1STAbits.UTXEN = 0;
    U1STAbits.URXEN = 0;
    U1MODEbits.ON = 0;

    AD_Init();
    AD_AddPins(POT_PIN);

    DIR_SWITCH_TRIS = 1;

    LED_Init();
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);

    Stepper_Init();

    while (1) {
        if (AD_IsNewDataReady()) {
            unsigned int adc = AD_ReadADPin(POT_PIN);
            uint16_t rate = adc_to_rate(adc);
            uint8_t direction = read_forward_switch();

            if (rate == 0) {
                if (Stepper_IsStepping()) {
                    Stepper_StopsSteps();
                }
            } else {
                Stepper_SetRate(rate);
                if (direction != lastDirection && Stepper_IsStepping()) {
                    Stepper_SetSteps(direction, STEP_CHUNK);
                }
                if (!Stepper_IsStepping()) {
                    Stepper_InitSteps(direction, STEP_CHUNK);
                }
            }

            lastDirection = direction;
            update_leds(rate, direction);
        }
    }

    return 0;
}
