/* 
 * File:   circularbuff.h
 * Author: tbriggs
 *
 * Created on October 7, 2013, 12:45 PM
 */

#ifndef CIRCULARBUFF_H
#define	CIRCULARBUFF_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CIRC_BUFF_SIZE 128          //circular buffer size

typedef struct {                    //struct to hold a circular buffer with parameters
    char buff[CIRC_BUFF_SIZE];
    int count;
    int max_size;
    int rd_pos;
    int wr_pos;
} circbuff_t;

void circbuff_init(circbuff_t *buff);           //initialize a new circular buffer
int circbuff_addch(circbuff_t *buff, char ch);  //add a character to the buffer
int circbuff_getch(circbuff_t *buff);           //get a character from the buffer
int circbuff_isfull(circbuff_t *buff);          //return true if the buffer is full
int circbuff_isempty(circbuff_t *buff);         //return true if the buffer is empty
int circbuff_hasdata(circbuff_t *buff);         //return true if the buffer contains data
int circbuff_almostfull(circbuff_t *buff);      //return true if the buffer is almost full


#ifdef	__cplusplus
}
#endif

#endif	/* CIRCULARBUFF_H */

