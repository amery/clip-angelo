typedef struct _HS_HEADER_
{
   unsigned char sig[2];
   unsigned char fuu[4];
   char lcase;
   char filtset;
   unsigned char pagesize[2];
   unsigned char lastrec[4];
   char unused[498];
} HS_HEADER;
