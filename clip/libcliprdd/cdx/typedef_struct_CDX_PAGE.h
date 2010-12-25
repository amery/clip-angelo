typedef struct _CDX_PAGE_
{
   unsigned char attrs[2];
   unsigned char nkeys[2];
   unsigned char left[4];
   unsigned char right[4];
   unsigned char res1[500];
} CDX_PAGE;
