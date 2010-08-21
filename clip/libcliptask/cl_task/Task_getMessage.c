TASK_DLLEXPORT TaskMessage *
Task_getMessage(void)
{
   TaskMessage *mp;

   Task *ct = currTask;

   if (empty_List(&ct->recvlist))
   {
      /*  ����� */
      removeFromList(ct);
      addToMsg(ct);
      Task_yield();
      if (empty_List(&ct->recvlist))
	 return 0;
   }
   else
      Task_yield();

   first_List(&ct->recvlist);
   mp = (TaskMessage *) ct->recvlist.current;
   remove_List(&ct->recvlist);
   append_List(&ct->proclist, mp);
   return mp;
}
