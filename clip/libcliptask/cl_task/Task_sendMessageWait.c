TASK_DLLEXPORT int
Task_sendMessageWait(int receiver, TaskMessage * msg)
{
   Task *tp = Task_findTask(receiver);

   Task *ct;

   if (!tp)
      return 0;

   if (tp->state == Task_zombie)
      return 0;

   ct = currTask;

   msg->needResp = 1;
   msg->needDel = 1;
   msg->receiver = tp->id;
   msg->sender = ct->id;

   /*  ����������� ����������� */
   append_List(&tp->recvlist, msg);

   /*  �� ��� �� ����� ������ � */
   if (tp->state == Task_msg)
   {
      removeFromList(tp);
      addToReady(tp);
   }

   /*  �� ��� */
   removeFromList(ct);
   addToResp(ct);

   Task_yield();

   return 1;
}
