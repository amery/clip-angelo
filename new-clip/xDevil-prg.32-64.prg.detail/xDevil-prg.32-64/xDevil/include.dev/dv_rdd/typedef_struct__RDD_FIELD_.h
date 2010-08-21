typedef struct _RDD_FIELD_
{
   char type;
   char name[11];
   int len;
   int dec;
   int offs;
   char binary;
   char nullable;
} RDD_FIELD;
