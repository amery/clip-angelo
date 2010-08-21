typedef struct _CDX_LEAF_
{
   unsigned char attrs[2];
   unsigned char nkeys[2];
   unsigned char left[4];
   unsigned char right[4];
   unsigned char free[2];
   unsigned char recmask[4];
   unsigned char dupmask;
   unsigned char trailmask;
   unsigned char recbits;
   unsigned char dupbits;
   unsigned char trailbits;
   unsigned char bytes;
   unsigned char keys[488];
} CDX_LEAF;
