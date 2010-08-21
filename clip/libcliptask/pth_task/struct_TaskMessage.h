struct TaskMessage
{

   long sender;
   long receiver;
   long id;
   int needResp:1;
   int needDel:1;

   void *data;
   void (*destroy) (void *data);

};
