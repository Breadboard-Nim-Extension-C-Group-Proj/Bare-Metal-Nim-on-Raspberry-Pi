// This file contains the declarations and functions used for the manipulation with the Raspberry PI registers and pins 

#include "nim.h"

#define PERIPHERAL_BASE 0x3F000000 
#define GPIO_BASE       (PERIPHERAL_BASE + 0x200000)
#define SYSTMR_BASE     (PERIPHERAL_BASE + 0x003000)

volatile uint32_t *GPPUD     = (uint32_t *)(GPIO_BASE + 0x94);
volatile uint32_t *GPPUDCLK0 = (uint32_t *)(GPIO_BASE + 0x98);
/**
 * GPIO Function Select Registers (GPFSEL0 - GPFSEL5).
 * 
 * Structure: An array of memory-mapped registers starting at offset 0x00 from GPIO_BASE.
 * 
 * Length: 32 bits per register. It takes 6 registers in total to cover all 54 physical 
 * GPIO pins on the Raspberry Pi. Each pin requires 3 consecutive bits to define its state, 
 * allowing 10 pins to be configured per 32-bit register.
 * 
 * Meaning: Defines the operating mode (function) of the pins. Setting a pin's 3-bit field 
 * to '000' configures it as an Input (e.g., for reading buttons), while '001' configures it 
 * as an Output (e.g., for driving LEDs).
 */
volatile uint32_t *GPFSEL = (uint32_t *)(GPIO_BASE + 0x00);

/**
 * GPIO Pin Output Set Registers (GPSET0 - GPSET1).
 * 
 * Structure: An array of memory-mapped registers starting at offset 0x1C from GPIO_BASE.
 * 
 * Length: 32 bits per register. It takes 2 registers to cover all 54 GPIO pins (1 bit 
 * per pin). GPSET0 controls pins 0-31, and GPSET1 controls pins 32-53.
 * 
 * Meaning: Used to drive an output pin HIGH (turn an LED on). Writing a '1' to a specific 
 * bit position sets the corresponding physical pin to a high voltage state. Writing a '0' 
 * has absolutely no effect, which makes it safe to modify one pin without accidentally 
 * altering the state of others.
 */
volatile uint32_t *GPSET  = (uint32_t *)(GPIO_BASE + 0x1C);

/**
 * GPIO Pin Output Clear Registers (GPCLR0 - GPCLR1).
 * Structure: An array of memory-mapped registers starting at offset 0x28 from GPIO_BASE.
 * Length: 32 bits per register. It takes 2 registers to cover all 54 GPIO pins (1 bit 
 * per pin). GPCLR0 controls pins 0-31, and GPCLR1 controls pins 32-53.
 * Meaning: Used to drive an output pin LOW (turn an LED off). Writing a '1' to a specific 
 * bit position clears the corresponding physical pin to a low voltage state. Similar to 
 * GPSET, writing a '0' is completely ignored by the hardware, ensuring independent pin control.
 */
volatile uint32_t *GPCLR  = (uint32_t *)(GPIO_BASE + 0x28);

/**
 * GPIO Pin Level Registers (GPLEV0 - GPLEV1).
 * 
 * Structure: An array of memory-mapped registers starting at offset 0x34 from GPIO_BASE.
 * 
 * Length: 32 bits per register. It takes 2 registers to cover all 54 GPIO pins (1 bit 
 * per pin). GPLEV0 reflects the status of pins 0-31, and GPLEV1 reflects pins 32-53.
 * 
 * Meaning: Read-only registers that return the actual, real-time physical logic level of 
 * the pins. Reading a '1' in a specific bit position means the corresponding pin is 
 * currently HIGH, and '0' means it is LOW. This is primarily used to detect button presses.
 */
volatile uint32_t *GPLEV  = (uint32_t *)(GPIO_BASE + 0x34);

/**
 * System Timer Counter Lower 32 Bits (SYSTMR_CLO).
 * 
 * Structure: A single memory-mapped hardware register at offset 0x04 from SYSTMR_BASE.
 * 
 * Length: 32 bits. It represents the lower half (lower 32 bits) of the Raspberry Pi's 
 * 64-bit free-running system timer.
 * 
 * Meaning: This hardware counter automatically increments by 1 every single microsecond 
 * (running at 1 MHz) from the moment the Raspberry Pi is powered on. Reading this register 
 * provides a highly accurate timestamp, which is essential for implementing non-blocking 
 * delays, LED blink intervals, and button switch debouncing without relying on an OS.
 */
volatile uint32_t *SYSTMR_CLO = (uint32_t *)(SYSTMR_BASE + 0x04);

uint32_t get_system_time_ms(void) {
    return (*SYSTMR_CLO) / 1000; 
}

void set_pin_mode(int pin, bool is_output) {
    int reg   = pin / 10;
    int shift = (pin % 10) * 3;
    
    uint32_t val = GPFSEL[reg];
    val &= ~(7 << shift); // 7 = 111,  so it only controls 3 bit of the val
    if (is_output) {
        val |= (1 << shift); // write 001 into that place or left it as 000
    }
    GPFSEL[reg] = val;
}

void set_pin(int pin, bool state) {
    int reg = pin / 32;
    int bit = pin % 32;
    
    if (state) {
        GPSET[reg] = (1 << bit);
    } else {
        GPCLR[reg] = (1 << bit);
    }
}

bool read_pin(int pin) {
    int reg = pin / 32;
    int bit = pin % 32;
    return (GPLEV[reg] & (1 << bit)) != 0;
}

void init_pins(PinsState *state){
    init_pins_for_pile(state->g_leds);
    init_pins_for_pile(state->b_leds);
    init_pins_for_pile(state->r_leds);
    init_pins_for_pile(state->y_leds);

    set_pin_mode(state->button_select_pile, false);
    set_pin_mode(state->button_select_count, false);
    set_pin_mode(state->button_confirm, false);

    enable_pull_up(state->button_select_pile);
    enable_pull_up(state->button_select_count);
    enable_pull_up(state->button_confirm);
}

void init_pins_for_pile(int *pile){
    int *end = pile + LEDS_COUNT;
    for(; pile < end; pile++){
        set_pin_mode(*pile, true);
        set_pin(*pile, false); 
    }
}

void enable_pull_up(int pin) {
    *GPPUD = 2;
    delay_ms(1);
    GPPUDCLK0[pin / 32] = (1 << (pin % 32)); 

    *GPPUD = 0;
    GPPUDCLK0[pin / 32] = 0;
}