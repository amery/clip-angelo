typedef struct _DBF_FIELD_
{
   char name[11];
   char type;
   unsigned char offs[4];
   union
   {
      char len[2];
      struct
      {
	 char len;
	 char dec;
      } num;
   } len;
   char flags;
   char flags2;
   char waid;
   char reserved2[2];
   char setfields;
   char reserved3[7];
   char indexed;
} DBF_FIELD;
