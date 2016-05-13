/* 
 * File:   uart.h
 * Author: tbriggs
 * Revisions by: Knoll, Walthers, White
 *
 * Created on October 7, 2013, 12:21 PM
 */

#ifndef UART_H
#define	UART_H
#include <stdio.h>

#include "circularbuff.h"

#ifdef	__cplusplus
extern "C" {
#endif


#define TXBUFFSZ CIRC_BUFF_SIZE     //TX circular buffer
#define RXBUFFSZ CIRC_BUFF_SIZE     //RX circular buffer
#define PBCLK 4000000               //peripheral bus clock
#define BAUDRATE 9600               //target baudrate
    
#define USEXONXOFF 1
extern circbuff_t txbuff, rxbuff;   // the tx and rx circular buffers are defined elsewhere


/*
 * Replace the fgets function
 */
#ifdef CUSTOM_FGETS
char *	fgets(char *ptr, int num, FILE *stream);
#else
char *	myfgets(char *ptr, int num, FILE *stream);
#endif

void uart_echo(int enable);             //echo the tx
void uart_init( );                      //initialization for UART
void uart_fast_puts(const char *str);   //print a string to the output without interrupts
void uart_puts (char *s);               //print a string to the output UART
void uart_putc (char c);                //print a character to the output UART
int uart_haschar( );                    //return true if the uart buffer contains data
int uart_getc(void);                    //get the char from the buffer

// enable CR to LF mapping
extern int eolfix;

// buffering of STDIN/STDOUT or not
extern int buffering;

extern void UART1_Config(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

