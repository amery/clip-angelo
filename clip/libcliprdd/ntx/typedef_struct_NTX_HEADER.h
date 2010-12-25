typedef struct _NTX_HEADER_
{
   unsigned char sig[2];
   unsigned char ver[2];
   unsigned char root[4];
   unsigned char fuu[4];
   unsigned char itemsize[2];
   unsigned char keysize[2];
   unsigned char dec[2];
   unsigned char fullpage[2];
   unsigned char halfpage[2];
   char key[256];
   char unique;
   char reserved1;
   char descend;
   char reserver2;
   char forexpr[256];
   char tag[12];
   char custom;
   char reserved3;
   unsigned char reserved2[472];
} NTX_HEADER;
