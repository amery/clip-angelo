TASK_DLLEXPORT TaskMessage *
Task_peekMessage(void)
{
   Task *ct;

   TaskMessage *mp;

   Task_yield();
   ct = currTask;
   if (empty_List(&ct->recvlist))
      return 0;
   first_List(&ct->recvlist);
   mp = (TaskMessage *) ct->recvlist.current;
   remove_List(&ct->recvlist);
   append_List(&ct->proclist, mp);
   return mp;
}
