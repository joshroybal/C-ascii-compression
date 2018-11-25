#ifndef _ASCII_H__
#define _ASCII_H__

#include <stdio.h>
#define RECLEN 80
#define BUFLEN 70

void encode(char buf[], char record[]);
void decode(char record[], char buf[]);
int readbuf(char buf[], int n, FILE *fp);
void writebuf(unsigned char buf[], int n, FILE *fp);
void hexdump(char buf[], int n, int *charcount);
void octdump(char buf[], int n, int *charcount);

#endif
