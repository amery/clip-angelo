typedef struct
{
   int sign;			/* check data of _C_ITEM_TYPE_DATETIME */
   int year;
   int month;
   int day;
   int hour;
   int min;
   int sec;
   int msec;
   char null;
}
ClipDateTime;
