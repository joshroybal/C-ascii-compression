#include <stdio.h>
#include <string.h>
#include "ascii.h"

/* convert 80 byte string to 70 byte 7-bit ascii buffer */
void encode(char buf[], char record[])
{
   int i, j, m, n;
   unsigned char byte1, byte2;

   memset(buf, '\0', BUFLEN);  /* pack the buffer with null chars */
   m = 0;
   for (i = 0; i < RECLEN; i += 8)
      for (j = 0; j < 7 && m < BUFLEN; ++j) {
         if (record[i + j] == '\0') /* check for no more processing sentinel */
            return;
         /* first get the lowest 7 bits from record[j] */
         byte1 = record[i + j] << 1;
         /* then get the (6 - j)th bit from record[7] */
         byte2 = record[i + 7] >> (6 - j);
         byte2 &= 0x01;  /* mask out all but the rightmost bit with zeros */
         buf[m] = byte1 | byte2;
         ++m;
      }
}

/* convert 70 byte 7-bit ascii character buffer into 80 byte record record */
void decode(char record[], char buf[])
{
   int i, j, k, n;
   unsigned char byte;

   memset(record, '\0', RECLEN);
   j = 0;
   k = 0;
   byte = 0;
   for (i = 0; i < BUFLEN; ++i) {
      byte |= ( ((unsigned char)buf[i] & 0x01) << (6 - j) ); /* put the (6 - j) bit back */
      j++;
      if (j > 6)
         j = 0;
      record[k] = (unsigned char)buf[i] >> 1;
      ++k;
      if ((i + 1) % 7 == 0) {
         record[k] = byte;
         ++k;
         byte = 0;
      }
   }
}

/* read n bytes of char data from data file */
int readbuf(char buf[], int n, FILE *fp)
{
   int i = 0;

   while (!feof(fp) && i < n) {
      buf[i] = getc(fp);
      ++i;
   }
   if (feof(fp)) buf[--i] = '\0';
   return(i);
}

/* write n bytes of char data to data file */
void writebuf(unsigned char buf[], int n, FILE *fp)
{
   int i, m;

   m = strlen((char *) buf);
   if (m < n)
      n = m;
   for (i = 0; i < n; i++)
      fputc(buf[i], fp);
}

/* hexadecimal dump char buffer */
void hexdump(char buf[], int n, int *charcount)
{
   int i;

   for (i = 0; i < n && buf[i] != '\0'; ++i) {
      if (*charcount % 8 == 0)
         printf("\n %#010x :", *charcount);
         printf(" %#04x", (unsigned char) buf[i]);
         ++(*charcount);
   }
}

/* octal dump char buffer */
void octdump(char buf[], int n, int *charcount)
{
    int i;

   for (i = 0; i < n && buf[i] != '\0'; ++i) {
      if (*charcount % 8 == 0)
         printf("\n %#012o :", *charcount);
      printf(" %#04o", (unsigned char) buf[i]);
      ++(*charcount);
   }
}
