typedef struct
{
   unsigned char type;		/* 'C' 'N' 'D' 'L' 'M' */
   unsigned char len;
   unsigned char dec;
   unsigned char flags;
   union
   {
      double n;
      struct
      {
	 char *str;
	 int len;
      }
      c;
      int l;
      long d;			/* julian */
   }
   u;
}
DbfData;
