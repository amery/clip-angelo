typedef struct _CDX_HEADER_
{
   unsigned char root[4];
   unsigned char fuu[4];
   unsigned char reserved1[4];
   unsigned char keysize[2];
   unsigned char options;
   unsigned char sign;
   unsigned char reserved2[483];
   unsigned char ic;
   unsigned char autonom[2];
   unsigned char sort[2];
   unsigned char totlen[2];
   unsigned char forlen[2];
   unsigned char reserved3[2];
   unsigned char explen[2];
   unsigned char keyforexpr[512];
} CDX_HEADER;
