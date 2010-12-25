TASK_DLLEXPORT int
Task_sendMessage(long receiver, /* new */ TaskMessage * msg)
{
   Task *tp = Task_findTask(receiver);

   if (!tp)
      return 0;

   if (tp->state == Task_zombie)
      return 0;

   msg->needResp = 0;
   msg->needDel = 1;
   msg->receiver = tp->id;
   msg->sender = currTask->id;

   /*  ����������� ����������� */
   append_List(&tp->recvlist, msg);

   /*  �� ��� �� ����� ������ � */
   if (tp->state == Task_msg)
   {
      removeFromList(tp);
      addToReady(tp);
   }

   Task_yield();

   return 1;
}
