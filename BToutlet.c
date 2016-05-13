/* 
 * File:    BToutlet.c
 * Authors: Knoll, Walthers, White
 *
 * Created on December 11, 2015
 */
/******************************************************************************/
/*  Files to Include                                                          */
/******************************************************************************/
    
#include <plib.h>           /* Include to use PIC32 peripheral libraries      */
#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */
#include <string.h>         /* For use in the UART buffer                     */
#include <p32xxxx.h>        /* Designed for PIC32MX230F256B                   */
#include "circularbuff.h"   /* Contains the circular buffer for the UART      */
#include "uart.h"           /* Initializes and performs UART functions        */

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = OFF            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = OFF             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = OFF            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = OFF           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRC              // Oscillator Selection Bits (Fast RC Osc (FRC))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Port Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)


#define DEBOUNCE_TIME 100               //debounce 100 cycles
#define ONMOSFET  LATAbits.LATA0        //Location for the on MOSFET
#define OFFMOSFET LATAbits.LATA1        //Location for the off MOSFET
#define RELAY_TIME 5000                 //Time for the MOSFET to be on


//Toggles the bit for turning on the relay with the MOSFET
void turn_off(){
    int j;
    ONMOSFET = 1;
    for (j = 0; j < RELAY_TIME; j++){
            Nop();
    }
    ONMOSFET = 0;
}

//Toggles the bit for turning on the relay with the MOSFET
void turn_on(){
    int j;
    OFFMOSFET = 1;
    for (j = 0; j < RELAY_TIME; j++){
            Nop();
    }
    OFFMOSFET = 0;
}

int main()
{
    //Section to set up the UART peripheral pin selection
    int status1, status2;
    mSYSTEMUnlock(status1,status2); //Disable interrupts and unlock the device
    PPSUnLock;                      //Unlock the device's peripheral pin select
    PPSInput(3 ,U1RX, RPA2);        //Set UART1's RX to RPA2
    PPSOutput(1, RPB4, U1TX);       //Set UART1's TX to RPB4
    PPSInput(2, U1CTS, RPB5);       //Set UART1's clear to send to RPB5
    PPSOutput(4, RPA3, U1RTS);      //Set ready to send to RPB4
    PPSLock;                        //Lock the device's peripheral pin select
    mSYSTEMLock(status1,status2);   //RE-enable interrupts and lock the device
    
    //Set up the TRIS and analog registers
    ANSELAbits.ANSA0 = 0;           //Turn off the analog peripherals

    TRISAbits.TRISA2 = 1;           //RX input
    
    TRISBbits.TRISB4 = 0;           //TX output
    
    TRISBbits.TRISB5 = 1;           //CTS input
    TRISAbits.TRISA3 = 0;           //RTS output
    
    TRISBbits.TRISB9 = 0;           //~RESET pin output
    LATBbits.LATB9 = 1;             //~RESET pin goes to high
    TRISAbits.TRISA0 = 0;           //MOSFET gate output
    TRISAbits.TRISA1 = 0;           //MOSFET gate output
    
    
    //Set up the UART and start the read loop
    uart_init ();                   //initialize UART to desired settings

    char ch;                        //character to store UART input
    int val;                        //integer to store the converted UART char
    while(1) {        
        while (! UARTReceivedDataIsAvailable(UART1));       //Wait for input
        
        ch = UARTGetDataByte(UART1);                        //Capture the incoming char
        val = (int)ch;                                      //Convert to an integer

        if(val == 0x0031){                                  //If the input character is 1
                turn_on();                                  //Turn on the PIC's relay
                uart_puts("ON\n");                          //Print out "ON" to the terminal
        }else
        if(val == 0x0030){                                  //If the input character is 0
                turn_off();                                 //Turn off the PIC's relay
                uart_puts("OFF\n");                         //Print out "OFF" to the terminal
        }
    }
    return(EXIT_SUCCESS);
}

 