typedef struct _FPT_HEADER_
{
   char fuu[4];
   char reserved1[2];
   char blocksize[2];
   char sig0[16];
   char reserved2[488];
   char sig[12];
   char flexsize[4];
   char flexoffs[4];
   char reserved3[8];
   char sig1[4];
   char reserved4[480];
} FPT_HEADER;
