#include <string.h>
#include "ascii.h"

/* command line parameters
   mandatory
   argv[1] compress / decompress option
   argv[2] input data file
   argv[3] output data file
   optional
   argv[4] hexadecimal/octal (x/o) binay output flag
*/
int main(int argc, char *argv[])
{
   int sl, dl, n = 0, charcount= 0;
   char record[RECLEN];
   unsigned char buf[BUFLEN];
   char fmtstr[2][20];
   char *dst, *src;
   FILE *fpin;
   FILE *fpout;

   /* pointer to function prototypes */
   void (*pfdump)(char buf[], int n, int *charcount);
   void (*pfprocess)(char *dst, char *src);

   /* error trap for missing command line arguments */
   if (strlen(argv[1]) == 0 || strlen(argv[2]) == 0 || strlen(argv[3]) == 0) {
      printf(" error - insufficient command line arguments\n");
      return(1);  /* insufficient command line arguments */
   }

   /* error trap for incorrect compress/decompress option flag */
   if (strcmp(argv[1], "compress") != 0 && strcmp(argv[1], "decompress") != 0) {
      printf(" error - incorrect command line argument (option flag)\n");
      return(1);  /* incorrect command line arguments */
   }

   /* check if user wanted a binary dump */
   if (argc > 4) {
      if (strcmp(argv[4], "o") == 0)  /* octal only if requested */
         pfdump = &octdump;
      else
         pfdump = &hexdump;  /* otherwise hexadecimal */
   }
   else
         pfdump = &hexdump;  /* otherwise hexadecimal */


   /* open input file and check for failure */
   fpin = fopen(argv[2], "r");
   if (fpin == NULL) {
      printf("error opening input file\n");
      return(1);  /* positive failure */
   }

   /* open output file and check for failure */
   fpout = fopen(argv[3], "w");
   if (fpout == NULL) {
      printf("error opening output file\n");
      return(1);  /* positive failure */
   }   

   if (strcmp(argv[1], "compress") == 0) {
      pfprocess = &encode;
      src = record;
      dst = buf;
      sl = RECLEN;
      dl = BUFLEN;
   }

   if (strcmp(argv[1], "decompress") == 0) {
      pfprocess = &decode;
      src = buf;
      dst = record;
      sl = BUFLEN;
      dl = RECLEN;
   }

   /* main processing loop */
   while (!feof(fpin)) {
      n += readbuf(src, sl, fpin);  /* read sl bytes from the input data file */
      pfprocess(dst, src);    /* encode or decode as per user choice */
      if (argc > 4) 
         pfdump(dst, dl, &charcount);    /* binary dump as per user choice */
      writebuf(dst, dl, fpout);   /* write dl bytes to the output data file */
   } /* end while loop */

   fclose(fpin);  /* close input file */
   fclose(fpout); /* close output file */

   if (argc > 4) {
      printf("\n %d input characters processed\n", n);
      printf(" %d output characters processed\n", charcount);
   }
   printf(" output written to file %s\n", argv[3]);
   return(0);
}
