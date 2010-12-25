typedef struct _CTX_HEADER_
{
   unsigned char sig[2];
   unsigned char ntags;
   unsigned char res1[13];
   CTX_TAG tags[63];
} CTX_HEADER;
