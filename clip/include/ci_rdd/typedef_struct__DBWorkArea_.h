typedef struct _DBWorkArea_
{
   int rd_item;
   struct _RDD_DATA_ *rd;

   char driver[9];
   char idx_driver[6];
   char memo_driver[6];
   char *name;
   char *alias;
   long aliasHash;
   int no;
   int found;
   int used;
   long trighash;
   char *trigger;
   int trig_active;
} DBWorkArea;
