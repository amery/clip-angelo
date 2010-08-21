typedef struct _DBF_HEADER_
{
   unsigned char version;
   unsigned char yy;
   unsigned char mm;
   unsigned char dd;
   unsigned char recs[4];
   unsigned char hdrsize[2];
   unsigned char recsize[2];
   unsigned char reserved1[2];
   unsigned char trans;
   unsigned char encrypted;
   unsigned char thread[4];
   unsigned char reserved2[8];
   unsigned char mdx;
   unsigned char lang;
   unsigned char reserved3[2];
} DBF_HEADER;
