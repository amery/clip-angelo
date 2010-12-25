typedef struct _CDX_BRANCH_
{
   unsigned char attrs[2];
   unsigned char nkeys[2];
   unsigned char left[4];
   unsigned char right[4];
   unsigned char keys[500];
} CDX_BRANCH;
