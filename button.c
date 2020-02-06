/*
 * button.c
 *
 *  Created on: Feb 12, 2019
 *      Author: tvpham
 */

/*
 *  button.c
 *  @author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *  @date: Jul 18, 2016
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 */

#include "button.h"
#include "lcd.h"

volatile int *button_event_ptr;
volatile int *button_num_ptr;

// The buttons are on PORTE 0:5
// GPIO_PORTE_DATA_R -- the register that must be checked on port E to determine which buttons are being pressed

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

uint8_t prev_Button; //must be set yourself in button_getButton()
uint8_t button; //current button being pressed, must be set yourself in button_getButton()

/**
 * Initialize PORTE and configure bits 0-5 to be used as inputs for the buttons.
 */
void button_init()
{
    static uint8_t initialized = 0;

    //Check if already initialized
    if (initialized)
    {
        return;
    }

    // To initialize and configure GPIO PORTE, visit pg. 656 in the Tiva datasheet.
    // Follow steps in 10.3 for initialization and configuration. Some steps have been outlined below.
    // Ignore all other steps in initialization and configuration that are not listed below. You will learn more
    // about additional steps in a later lab.

    //Turn on PORTE system clock
    //SYSCTL_RCGCGPIO_R |=

    SYSCTL_RCGCGPIO_R |= 0b010000; //Enables Port E clock

    //Set the buttons to inputs and enable
    //1. GPIO_PORTE_DIR_R &=
    //2. GPIO_PORTE_DEN_R |=

    GPIO_PORTE_DIR_R &= 0b00000000; //Sets all of Port E's bits as input
    GPIO_PORTE_DEN_R |= 0b11111111; //Enables pins 0-7

    initialized = 1;
}

/**
 * Interrupt handler -- executes when a hardware event occurs (a button is pressed)
 */
void init_button_interrupts(int *button_event_addr, int *button_num_addr)
{

    //  #warning: "Unimplemented function: void init_button_interrupts(int *button_event_addr, int *button_num_addr) -- You must configure GPIO to detect interrupts" // delete warning after implementing
    // In order to configure GPIO ports to detect interrupts, you will need to visit pg. 656 in the Tiva datasheet.
    // Notice that you already followed some steps in 10.3 for initialization and configuration of the GPIO ports in the function button_init().
    // Additional steps for setting up the GPIO port to detect interrupts have been outlined below.
    // TODO: Complete code below

    // Mask the bits for pins 0-5
    //1. GPIO_PORTE_IM_R &=

    GPIO_PORTE_IM_R &= 0b11000000; //Masks bits 0-5

    // Set pins 0-5 to use edge sensing
    //2. GPIO_PORTE_IS_R &=
    GPIO_PORTE_IS_R &= 0b11000000; //Sets pins 0-5 to enable edge-sensing

    // Set pins 0-5 to use both edges. We want to update the LCD
    // when a button is pressed, and when the button is released.
    //3. GPIO_PORTE_IBE_R |=
    GPIO_PORTE_IBE_R |= 0b00111111;

    // Clear the interrupts
    //4. GPIO_PORTE_ICR_R =
    GPIO_PORTE_ICR_R |= 0b11111111;

    // Unmask the bits for pins 0-5
    //5. GPIO_PORTE_IM_R |=
    GPIO_PORTE_IM_R |= 0b00111111;

    // TODO: Complete code below
    // Enable GPIO port E interrupt
    //6. NVIC_EN0_R |=
    NVIC_EN0_R |= 0b000010000;

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);

    button_event_ptr = button_event_addr;
    button_num_ptr = button_num_addr;
}

/**
 * Handles a hardware interrupt that occurs from a button being pressed
 */
void gpioe_handler()
{

//#warning: "Unimplemented function: void gpioe_handler() -- You must configure interrupts" // delete warning after implementing
    // Clear interrupt status register
    // GPIO_PORTE_ICR_R =
    GPIO_PORTE_ICR_R |= 0b11111111;
    // update *button_event_ptr = 1;
    *button_event_ptr = 1;
    *button_num_ptr = button_getButton();

}

/**
 * returns a 6-bit bit field, representing the push buttons. A 1 means the corresponding button is pressed.
 */
uint8_t button_checkButtons()
{
    return (~GPIO_PORTE_DATA_R) & (BIT6 - 1); //Return the button status
}

/**
 * Returns the position of the leftmost button being pushed.
 * @return the position of the leftmost button being pushed. A 6 is the leftmost button, 1 is the rightmost button. Return 0 if no button is being pressed.
 */
uint8_t button_getButton()
{

    //
    // DELETE ME - How bitmasking works
    // ----------------------------------------
    // In embedded programming, often we only care about one or a few bits in a piece of
    // data. There are several bitwise operators that we can apply to data in order
    // to "mask" the bits that we don't care about.
    //
    //  | = bitwise OR      & = bitwise AND     ^ = bitwise XOR     ~ = bitwise NOT
    //        << x = shift left by x bits        >> x = shift right by x bits
    //
    // Let's say we want to know if push button 3 (SW3) of GPIO_PORTE_DATA_R is
    // pushed.  Since push buttons are high (1) initially, and low (0) if pushed, PORTE should
    // look like:
    // GPIO_PORTE_DATA_R => 0b???? ?0?? if SW3 is pushed
    // GPIO_PORTE_DATA_R => 0b???? ?1?? if SW3 is not pushed
    //
    // Bitwise AND:
    // (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0000 if SW3 is pushed
    // (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0100 if SW3 is not pushed
    //
    // Bitwise OR:
    // (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1011 if SW3 is pushed
    // (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1111 if SW3 is not pushed
    //
    // Other techniques (Shifting and bitwise AND)
    // ((GPIO_PORTE_DATA_R >> 2) & 1) => 0 if SW3 is pushed
    // ((GPIO_PORTE_DATA_R >> 2) & 1) => 1 if SW3 is not pushed

    // TODO: Write code below -- Checks the leftmost bit, which corresponds to 6

    if ((button_checkButtons() & 0b00000001) == 0b00000001)
    {
        button = 1;
    }
    if ((button_checkButtons() & 0b00000010) == 0b00000010)
    {
        button = 2;
    }
    if ((button_checkButtons() & 0b00000100) == 0b00000100)
    {
        button = 3;
    }
    if ((button_checkButtons() & 0b00001000) == 0b00001000)
    {
        button = 4;
    }
    if ((button_checkButtons() & 0b00010000) == 0b00010000)
    {
        button = 5;
    }
    if ((button_checkButtons() & 0b00100000) == 0b00100000)
    {
        button = 6;
    }
    if (button_checkButtons() == 0b00000000)
    {
        button = 0;
    }

    return button; //EDIT ME
}
